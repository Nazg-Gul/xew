#!/usr/bin/env python
#
# Copyright (c) 2018, X Wrangler authors
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.
#
# Author: Sergey Sharybin (sergey.vfx@gmail.com)

from __future__ import print_function

import clang.cindex
import collections
import os
import re
import sys

from clang.cindex import TokenKind


# Function attributes which are equivalent to __attribute__((noreturn))
ATTR_NO_RETURN = ('_X_NORETURN', )
# Attributes which we simply ignore.
ATTR_IGNORE = ('_XLIB_COLD', )

MODULES = (
  {"ignore": False,
   "name": "XKBCommon",
   "libraries": {
      "xkbcommon": {
        "paths": {"linux": ("libxkbcommon.so.1", "libxkbcommon.so")},
        "required": True,
      },
   },
   "include_directory": "/usr/include/xkbcommon",
   "headers": ("xkbcommon-names.h",
               "xkbcommon-keysyms.h",
               "xkbcommon.h",
               "xkbcommon-compat.h",
               "xkbcommon-compose.h"),
   "extra_wrangler_includes": ("stdint.h", "stdio.h", "stdarg.h"),
   "extra_wrangler_code": "",
   "ignore_tokens": (),
   "wrangle-typedefs": True,
   "use_wrapper": True},
  {"ignore": False,
   "name": "XCB",
   "libraries": {
     "xcb": {
       "paths": {"linux": ("libxcb.so.1", "libxcb.so")},
       "required": True,
       "headers": ("xcb.h",
                   "xproto.h"),
     },
     "xcb_keysyms": {
       "paths": {"linux": ("libxcb-keysyms.so.1", "libxcb-keysyms.so")},
       "required": True,
       "headers": ("xcb_keysyms.h",)
     },
   },
   "include_directory": "/usr/include/xcb",
   "headers": ("xcb.h",
               "xproto.h",
               "xcb_keysyms.h"),
   "extra_wrangler_includes": ("stdarg.h",
                               "stdio.h",
                               "stdint.h",
                               "sys/types.h"),
   "extra_wrangler_code": "",
   "ignore_tokens": (),
   "wrangle-typedefs": True,
   "use_wrapper": True},
  {"ignore": False,
   "name": "X11",
   "libraries": {
     "x11": {
       "paths": {"linux": ("libX11.so.6", "libX11.so")},
       "required": True,
       "headers": ("XKBlib.h", "Xlib.h", "Xlibint.h", "Xutil.h"),
     },
     "x11_xcb": {
       "paths": {"linux": ("libX11-xcb.so.6", "libX11-xcb.so")},
       "required": True,
       "headers": ("Xlib-xcb.h",)
     },
     "x11_xinerama": {
       "paths": {"linux": ("libXinerama.so.1", "libXinerama.so")},
       "required": False,
       "headers": ("Xinerama.h",)
     }
   },
   "include_directory": "/usr/include/X11",
   "headers": ("Xlib.h",
               "Xlib-xcb.h",
               "Xlibint.h",
               "XKBlib.h",
               "Xutil.h",
               "extensions/Xinerama.h"),
   "extra_wrangler_includes": ("X11/keysym.h",
                               "X11/Xatom.h",
                               "X11/XKBlib.h",
                               "X11/Xlib.h",
                               "X11/Xlibint.h",
                               "X11/Xutil.h",
                               "X11/extensions/Xinerama.h"),
   "extra_wrangler_code": """
// min/max conflicts with STL includes.
\n#undef min\n#undef max
// This is to prevent us from include Xlib-xcb, which will cause type conflicts
// with out wrangler.
typedef struct xcb_connection_t xcb_connection_t;
enum XEventQueueOwner {
  XlibOwnsEventQueue = 0,
  XCBOwnsEventQueue
};
// Since thw whole header is ignored, we manually define symbols from it.
xcb_connection_t* XGetXCBConnection(Display* dpy);
void XSetEventQueueOwner(Display* dpy, enum XEventQueueOwner owner);""",
   "ignore_tokens": ("_XFUNCPROTOBEGIN", "_XFUNCPROTOEND", "_X_DEPRECATED"),
   "wrangle-typedefs": False,
   "use_wrapper": True},
  {"ignore": True,
   "name": "test",
   "libraries": {
     "test": {
       "paths": {"linux": ("libtest.so.1", "libtest.so")},
       "required": True,
     },
   },
   "include_directory": ".",
   "headers": ("test.h",),
   "extra_wrangler_includes": (),
   "extra_wrangler_code": "",
   "ignore_tokens": (),
   "wrangle-typedefs": False,
   "use_wrapper": True},
)


class ModuleContext:
    # Name of the corresponding module
    module = None
    # Per-filename original file structure.
    file_structure = None
    # Per-filename function information in this module.
    functions = None

    def __init__(self):
        self.module = None
        self.file_structure = collections.OrderedDict({})
        self.functions = collections.OrderedDict({})


class Argument:
    name = None
    type = None


class Function:
    name = None
    return_type = None
    arguments = None
    no_return = None

    def __init__(self):
        self.name = None
        self.return_type = None
        self.arguments = None
        self.no_return = None


def is_function_prototype(tokens):
    """
    Check whether sequence of tokens denotes a function prototype.
    """
    # Simple heuristic: if there is no punctuation, this is not a
    # function prototype.
    for token in tokens:
        if token.kind == TokenKind.PUNCTUATION:
            return True
    return False


def join_type_tokens(tokens):
    """
    Join tokens together into a type definition.
    """
    result = ""
    for token in tokens:
        if result:
            if token.spelling in ('*', ',', ):
                pass
            elif token.spelling == ')' or result[-1] == '(':
                pass
            elif token.spelling == '(' and result[-1] == ")":
                pass
            elif result.endswith("(*"):
                pass
            else:
                result += " "
        result += token.spelling
    return result


def get_function_return_type(tokens):
    """
    Get a function return type from sequence of tokens.
    """
    got_type = False
    got_name = False
    inside_argument_list = 0
    done_argument_list = False
    result = []
    for token in tokens:
        # TODO(sergey): Find a more generic solution.
        if token.spelling == "_X_SENTINEL":
            break
        if token.kind == TokenKind.KEYWORD:
            if token.spelling != 'extern':
                got_type = True
        elif token.kind == TokenKind.IDENTIFIER:
            if got_type:
                got_name = True
            else:
                got_type = True
        elif token.kind == TokenKind.PUNCTUATION:
            if token.spelling == "(":
                if got_name:
                    inside_argument_list += 1
            elif token.spelling == ")":
                if got_name:
                    inside_argument_list -= 1
                    if inside_argument_list == 0:
                        done_argument_list = True
                        continue
        if token.spelling != "extern" and (not got_name or done_argument_list):
            result.append(token)
    result = join_type_tokens(result)
    result = result.replace("(* )", "(*)")
    return result


def get_function_name(tokens):
    """
    Get a function name from sequence of tokens.
    """
    got_type = False
    for token in tokens:
        if token.kind == TokenKind.KEYWORD:
            if token.spelling != 'extern':
                got_type = True
        elif token.kind == TokenKind.IDENTIFIER:
            if got_type:
                return token.spelling
            else:
                got_type = True
    return ""


def get_argument_name_from_comment(comment):
    """
    Deducts argument name from a comment. Used to get maeningful argument name
    from declaration like:

        void foo(int /* interesting_argument */,
                 float /* even_more_interesting_argument */

    """
    name = comment.replace("/*", "").replace("*/", "").strip()
    return name.split(" ")[0]


def strip_leading_comments(tokens):
    """
    Removes all possible leading comments from the list of tokens.
    """
    new_tokens = []
    leading_skipped = False
    for token in tokens:
        if token.kind == TokenKind.COMMENT:
            if leading_skipped:
                new_tokens.append(token)
        else:
            leading_skipped = True
            new_tokens.append(token)
    return new_tokens


def leave_single_trailing_comment(tokens):
    """
    Removes all trailing comments for until at a max one is remained.
    """
    while len(tokens) > 2:
        if (    tokens[-1].kind == TokenKind.COMMENT and \
                tokens[-2].kind == TokenKind.COMMENT):
            tokens = tokens[:-1]
        else:
            break
    if tokens[-1].kind == TokenKind.COMMENT:
        if '<' in tokens[-1].spelling:
            tokens = tokens[:-1]
    return tokens


def merge_complex_argument_types(tokens):
    """
    Expects tokens list to start with a non-comment. Will merge `struct foo`
    and `enum foo` into a single fake token, for the ease of following parsing.
    """
    if not (tokens[0].kind == TokenKind.KEYWORD and
            tokens[0].spelling in ("enum", "struct", "union", )):
        return tokens
    code = tokens[0].spelling + " " + tokens[1].spelling
    tokens = [FakeToken(code)] + tokens[2:]
    return tokens


def get_argument_info_from_tokens(tokens):
    tokens = strip_leading_comments(tokens)
    tokens = leave_single_trailing_comment(tokens)
    tokens = merge_complex_argument_types(tokens)
    name = ""
    type_tokens = tokens
    if tokens[-1].kind == TokenKind.COMMENT:
        name = get_argument_name_from_comment(tokens[-1].spelling)
        type_tokens = tokens[:-1]
    else:
        # TODO(sergey): Make the check more reliable.
        if tokens[-1].spelling in ("len", "str"):
            name = tokens[-1].spelling
            type_tokens = tokens[:-1]
        elif len(tokens) >= 2 and tokens[-2].spelling == '*':
            name = tokens[-1].spelling
            type_tokens = tokens[:-1]
        elif len(tokens) >= 2 and tokens[-2].kind == TokenKind.KEYWORD \
                              and tokens[-1].kind != TokenKind.KEYWORD \
                              and tokens[-1].kind != TokenKind.PUNCTUATION:
            name = tokens[-1].spelling
            type_tokens = tokens[:-1]
        elif len(tokens) == 2 and \
             tokens[-1].kind not in (TokenKind.KEYWORD, TokenKind.PUNCTUATION):
            name = tokens[-1].spelling
            type_tokens = tokens[:-1]
        elif tokens[-1].spelling not in ('void', '...', ')'):
            name = "arg"
            type_tokens = tokens[:]

    argument_type = join_type_tokens(type_tokens)
    argument_type = argument_type.replace("(* )", "(*)")
    argument_type = argument_type.replace("[ ", "[").replace(" ]", "]")
    # Make argument names for use when included from C++ code.
    if name in ("class", "delete", "protected", "new", ):
        name += "_"
    argument = Argument()
    argument.name = name
    argument.type = argument_type
    return argument


def analyze_argument_tokens(arguments, tokens):
    if not tokens:
        return
    arg = get_argument_info_from_tokens(tokens)
    # TODO(sergey): Make this more generic.
    if arg.type == "unsigned int":
        if "keycode" in arg.name or arg.name == "kc":
            return
    arguments.append(arg)


def get_function_arguments(tokens):
    arguments = []
    got_type = False
    got_name = False
    inside_arglist_interior = 0
    inside_preprocessor = None
    argument_tokens = []
    for token in tokens:
        if got_name:
            if inside_arglist_interior > 0:
                if inside_preprocessor is not None:
                    if inside_preprocessor == '#':
                        if token.spelling == 'if':
                            inside_preprocessor = 'if'
                        elif token.spelling == 'else':
                            inside_preprocessor = None
                        elif token.spelling == 'endif':
                            inside_preprocessor = None
                    elif inside_preprocessor == 'if':
                        inside_preprocessor = None
                    continue
                elif token.kind == TokenKind.PUNCTUATION:
                    if token.spelling == ')':
                        inside_arglist_interior -= 1
                        if inside_arglist_interior == 0:
                            analyze_argument_tokens(arguments, argument_tokens)
                            break
                    elif token.spelling == '(':
                        inside_arglist_interior += 1
                    elif token.spelling == ',':
                        if inside_arglist_interior == 1:
                            analyze_argument_tokens(arguments, argument_tokens)
                            argument_tokens = []
                            continue
                    elif token.spelling == '#':
                        inside_preprocessor = '#'
                        continue
                argument_tokens.append(token)
            elif token.kind == TokenKind.PUNCTUATION:
                if token.spelling == '(':
                    inside_arglist_interior += 1
        elif token.kind == TokenKind.KEYWORD:
            if token.spelling != 'extern':
                got_type = True
        elif token.kind == TokenKind.IDENTIFIER:
            if got_type:
                got_name = True
            else:
                got_type = True
    return arguments


def analyze_function_tokens(functions, tokens):
    """
    Analyze tokens sequence between extern and ;.
    """
    if not is_function_prototype(tokens):
        return None
    function = Function()
    function.return_type = get_function_return_type(tokens)
    function.name = get_function_name(tokens)
    function.arguments = get_function_arguments(tokens)
    function.no_return = False
    functions.append(function)
    return function


def check_function_has_varargs(function):
    for argument in function.arguments:
        if argument.type == "...":
            return True
    return False


def generate_function_arguments_declaration(function, start_column=0):
    arguments = function.arguments
    num_arguments = len(arguments)
    argument_index = 0
    result = ""
    single_line = True
    # First, check if we can put everything in a single line.
    total_length = start_column + 2
    for argument in arguments:
        if argument.type == "KeyCode":
            single_line = False
            break
        total_length += len(argument.type) + len(argument.name)
        if total_length >= 80:
            single_line = False
            break
    # Setup padding and EOL, used to join arguments together.
    if single_line:
        EOL = " "
        padding = ""
    else:
        EOL = "\n"
        padding = "    "
        result = "\n"
    # Combine actual result.
    for argument in arguments:
        is_last_argument = (argument_index == num_arguments - 1)
        current = "{}{}{}" . format(
                padding,
                argument.type,
                " " + argument.name if argument.name else "")
        if not is_last_argument:
            current += ",{}" . format(EOL)
        # TODO(sergey): Generalize this somehow.
        if argument.type == "KeyCode":
            alt_current = "{}unsigned int {}" . format(padding, argument.name)
            if not is_last_argument:
                alt_current += ",\n"
            current = ("#if NeedWidePrototypes\n" +
                       "{}\n" +
                       "#else\n" +
                       "{}\n" +
                       "#endif\n") . format(alt_current.rstrip(),
                                            current.rstrip())
        elif "(*)" in argument.type:
            current = padding + argument.type.replace(
                    "(*)", "(*" + argument.name + ")")
            if not is_last_argument:
                current += ",{}" . format(EOL)
        elif "[" in argument.type:
            arg = argument.type.replace("[", argument.name + "[")
            current = "{}{}" . format(padding, arg)
        result += current
        argument_index += 1
    return result


def wrap_code_with_ifdef(function_name, code):
    """
    For a given funtion name, wraps code with corresponding ifdef.

    This is annoying manual way to deal with functions delcared
    inside of if-def blocks.
    """
    # TODO(sergey): This also needs to be generalized.
    if function_name in ("_XOpenFileMode",
                         "_XOpenFile",
                         "_XFopenFile",
                         "_XAccessFile"):
        return "#if defined(WIN32)\n" + code + "#endif\n"
    elif function_name in ("__XOS2RedirRoot", ):
        return "#if defined(__UNIXOS2__)\n" + code + "#endif\n"
    elif function_name == "Data":
        return "#ifdef DataRoutineIsProcedure\n" + code + "#endif\n"
    elif function_name in ('XDestroyImage',
                           'XGetPixel',
                           'XPutPixel',
                           'XSubImage',
                           'XAddPixel'):
        return "#ifdef XUTIL_DEFINE_FUNCTIONS\n" + code + "#endif\n"
    return code


def generate_function_declaration(function):
    """
    Generate declaration for the function.
    """
    if "(*)" in function.return_type:
        arguments = generate_function_arguments_declaration(function, 80)
        type_name_with_arguments = "(* {}({}))".format(function.name,
                                                       arguments)
        type_with_return = function.return_type.replace(
                "(*)",
                type_name_with_arguments)
        result = type_with_return
    else:
        result = "{} {}(" . format(function.return_type,
                                   function.name)
        arguments = generate_function_arguments_declaration(function)
        result += arguments + ")"
    return result


def generate_function_typedef(function):
    typedef = ""
    if "(*)" in function.return_type:
        arguments = generate_function_arguments_declaration(function, 80)
        type_name_with_arguments = "(* (*t{})({}))".format(function.name,
                                                           arguments)
        type_with_return = function.return_type.replace(
                "(*)",
                type_name_with_arguments)
        typedef = "typedef {};" . format(type_with_return)
    else:
        typedef = "typedef {} (*t{})(" . format(function.return_type,
                                                function.name)
        arguments = generate_function_arguments_declaration(function,
                                                            len(typedef))
        typedef += arguments + ");"
    return typedef


def generate_function_declarations(function):
    """
    Generate function declaration, which matches its type.
    """
    declaration = ""
    if "(*)" in function.return_type:
        arguments = generate_function_arguments_declaration(function, 80)
        type_name_with_arguments = "{}({})".format(function.name,
                                                   arguments)
        type_with_return = function.return_type.replace(
                "(*)",
                type_name_with_arguments)
        declaration = "{};" . format(type_with_return)
    else:
        declaration = "{} {}(" . format(function.return_type, function.name)
        arguments = generate_function_arguments_declaration(function,
                                                            len(declaration))
        declaration += arguments + ");"
    return declaration


def generate_function_typedefs(functions):
    """
    Generate type definitions for symbols which we dlsym() from
    library.
    """
    result = ""
    for function in functions:
        if result:
            result += "\n"
        result += generate_function_typedef(function)
    return result


def generate_functor_declarations(functions, use_wrapper):
    """
    Generate declaration for implementation functions.
    """
    result = ""
    suffix = "_impl" if use_wrapper else ""
    for function in functions:
        name = function.name
        result += "extern t{} {}{};\n" . format(name, name, suffix)
    return result


def generate_functor_defintiions(functions, use_wrapper):
    """
    Generate definition for implementation functions.
    """
    result = ""
    suffix = "_impl" if use_wrapper else ""
    for function in functions:
        name = function.name
        result += "t{} {}{};\n" . format(name, name, suffix)
    return result


def extract_argument_name(argument):
    if argument.name:
        return argument.name
    t = argument.type
    if t == "void":
        return ""
    t = re.sub("^([A-Za-z0-9\s]+)", "", t)
    if t.startswith("(*"):
        return re.sub("^\(\*([A-Za-z0-9_]+).*", "\\1", t)
    return "MISSING_ARGUMENT"


def generate_function_wrapper(function):
    if check_function_has_varargs(function):
        return ""
    argument_names = [extract_argument_name(arg) for arg in function.arguments]
    joined_arguments = ", ".join(argument_names)
    declaration = generate_function_declaration(function)
    code = declaration + " {\n"
    call = "{}_impl({})" . format(function.name, joined_arguments)
    if function.no_return:
        call = "  " + call + ";\n"
        call += "  abort();\n"
    else:
        call = "  return " + call + ";\n"
    code += call
    code += "}\n"
    return wrap_code_with_ifdef(function.name, code) + "\n"


def generate_function_wrappers(module_context):
    result = ""
    for filename, functions in module_context.functions.iteritems():
        if not functions:
            continue
        result += "// " + filename + "\n"
        for function in functions:
            result += generate_function_wrapper(function)
    return result


def generate_vararg_function_wrapper(function):
    if not check_function_has_varargs(function):
        return ""
    return "#  define {} {}_impl\n" . format(function.name, function.name)


def generate_vararg_function_wrappers(module_context):
    result = ""
    for filename, functions in module_context.functions.iteritems():
        if not functions:
            continue
        file_result = ""
        for function in functions:
            current_result = generate_vararg_function_wrapper(function)
            if filename and not file_result and current_result:
                file_result = "// " + filename + "\n" + current_result
            else:
                file_result += current_result
        result += file_result
    return result


def get_library_for_header(module, filename):
    libraries = module["libraries"]
    num_libraries = len(libraries)
    if num_libraries == 1:
        first_key = libraries.keys()[0]
        return first_key + "_lib"
    for name, library in libraries.iteritems():
        if filename in library["headers"]:
            return name + "_lib"
    raise Exception("Missing library for header {}" . format(filename))


def generate_function_loaders(module_context):
    result = ""
    module = module_context.module
    loader = "_LIBRARY_FIND_IMPL" if module["use_wrapper"] else "_LIBRARY_FIND"
    for filename, functions in module_context.functions.iteritems():
        if not functions:
            continue
        library = get_library_for_header(module, filename)
        result += "  // {}\n" . format(filename)
        for function in functions:
            result += "  {}({}, {});\n" . format(loader,
                                                 library,
                                                 function.name)
    # Strip trailing \n, will be added when substituded into template.
    result = result[:-1]
    return result


def generate_single_libraries_list_code(name, libraries, platform):
    """
    Generate single list of libraries to be queries.
    """
    platform_libraries = libraries["paths"].get(platform) or []
    result = "static const char* {}_paths[] = " . format(name) + "{"
    indentation = " " * len(result)
    for index, library in enumerate(platform_libraries):
        if index:
            result += ",\n" + indentation
        result += '"' + library + '"'
    if platform_libraries:
        result += ",\n" + indentation
    result += "NULL};"
    return result


def generate_libraries_list_code(module):
    """
    Generate code which contains platform-specific list of libraries
    to be attempted to be loaded.
    """
    win32_libraries = ""
    apple_libraries = ""
    linux_libraries = ""
    is_first_time = True
    for name, libraries in module["libraries"].iteritems():
        if not is_first_time:
            win32_libraries += "\n"
            apple_libraries += "\n"
            linux_libraries += "\n"
        is_first_time = False
        win32_libraries += generate_single_libraries_list_code(
                name, libraries, "win32")
        apple_libraries += generate_single_libraries_list_code(
                name, libraries, "apple")
        linux_libraries += generate_single_libraries_list_code(
                name, libraries, "linux")
    return """#ifdef _WIN32
{}
#elif defined(__APPLE__)
{}
#else
{}
#endif""".format(win32_libraries, apple_libraries, linux_libraries)


def generate_libraries_open_code(module):
    """
    Generate code which opens libraries.
    """
    result = ""
    for name, library in module["libraries"].iteritems():
        if result:
            result += "\n"
        result += "  {}_lib = xew_dynamic_library_open_find({}_paths);".format(
                name, name)
        if library["required"]:
            result += "\n  if (" + name + "_lib == NULL) {\n" \
                      "    return XEW_ERROR_OPEN_FAILED;\n" \
                      "  }"
    return result

def generate_dynamic_libraries(module_context):
    module = module_context.module
    result = ""
    for name in module["libraries"]:
        if result:
            result += "\n"
        result += "static DynamicLibrary* {}_lib = NULL;" . format(name)
    return result

###############################################################################
# Parser


class FileStructureEntry:
    TYPE_UNKNOWN = 0
    TYPE_BARE_CODE = 1
    TYPE_FUNCTION_REFERENCE = 2

    type = TYPE_UNKNOWN
    argument = None

    def __init__(self, type, argument):
        self.type = type
        self.argument = argument

    def get_code(self):
        """
        For type of BARE_CODE get code associated with this entry.
        """
        assert self.type == self.TYPE_BARE_CODE
        return str(self.argument)

    def get_function(self):
        """
        For type of FUNCTION_REFERENCE get function associated with this entry.
        """
        assert self.type == self.TYPE_FUNCTION_REFERENCE
        return self.argument


class FakeToken:
    """
    Fake token constructed from piece of parsed code
    """
    kind = None
    spelling = ""

    def __init__(self, spelling):
        self.kind = TokenKind.IDENTIFIER
        self.spelling = spelling


class Tokenizer:
    """
    Helper class which allows to look ahead and put token back.
    """
    tokens = []
    num_tokens = -1
    current_token_index = -1
    next_token = None

    def __init__(self, tokens):
        self.tokens = list(tokens)
        self.num_tokens = len(self.tokens)
        self.current_token_index = 0
        self.next_token = None

    def has_tokens(self):
        return self.current_token_index < self.num_tokens

    def get_next_token(self, skip_comments=True):
        if self.next_token is not None:
            token = self.next_token
            self.next_token = None
            return token
        if self.current_token_index >= self.num_tokens:
            return None
        token = self.tokens[self.current_token_index]
        self.current_token_index += 1
        if skip_comments:
            while (self.current_token_index != self.num_tokens and
                   token.kind == TokenKind.COMMENT):
                token = self.tokens[self.current_token_index]
                self.current_token_index += 1
        return token

    def peek_token(self, skip_comments=True):
        if self.next_token is not None:
            return self.next_token
        if self.current_token_index >= self.num_tokens:
            return None
        index = self.current_token_index
        token = self.tokens[index]
        index += 1
        if skip_comments:
            while (self.current_token_index != self.num_tokens and
                   token.kind == TokenKind.COMMENT):
                token = self.tokens[index]
                index += 1

    def push_token(self, token):
        if self.next_token is not None:
            raise Exception("Attempt to push multiple tokens")
        self.next_token = token


class ParseContext:
    # Name of the currently parsing file
    filenae = None
    # Pr-element file structure. Each element is an object of
    # FileStructureEntry.
    file_structure = None
    # List of all functions in the parsed sources.
    functions = None
    # Object of Tokenizer.
    tokenizer = None
    # Lines of the currently parsing file.
    file_lines = None

    def __init__(self):
        self.file_structure = []
        self.functions = []
        self.file_lines = []


def struct_workaround_illegal_access(parse_context, next_token):
    """
    Deals with preprocessor inside of typedef, like XLIB_ILLEGAL_ACCESS
    """
    tokenizer = parse_context.tokenizer
    result = ""
    if next_token.spelling == "#":
        parse_context.tokenizer.push_token(next_token)
        result += "\n" + parse_preprocessor(parse_context) + "\n"
        next_token = tokenizer.get_next_token()
    return result, next_token


def parse_struct_as_is(parse_context, indent_level=0):
    tokenizer = parse_context.tokenizer
    current_indentation = "  " * indent_level
    next_indentation = "  " * (indent_level + 1)
    result = ""
    # Read the `struct`.
    # NOTE: This could also be `union`.
    struct_token = tokenizer.get_next_token()
    result += struct_token.spelling
    # Next is the identifier or {.
    next_token = tokenizer.get_next_token()
    code, next_token = struct_workaround_illegal_access(parse_context,
                                                        next_token)
    result += code
    if next_token.kind == TokenKind.IDENTIFIER:
        result += " " + next_token.spelling
        next_token = tokenizer.get_next_token()
    code, next_token = struct_workaround_illegal_access(parse_context,
                                                        next_token)
    result += code
    if next_token.kind == TokenKind.PUNCTUATION and next_token.spelling == "{":
        if result[-1] != "\n":
            result += " "
        result += "{\n"
        next_token = tokenizer.get_next_token()
        # Read struct fields.
        while next_token and next_token.spelling != "}":
            field = ""
            if next_token.spelling == '#':
                parse_context.tokenizer.push_token(next_token)
                result += parse_preprocessor(parse_context) + "\n"
                next_token = tokenizer.get_next_token()
                continue
            if next_token.kind == TokenKind.KEYWORD:
                if next_token.spelling in ("struct", "union"):
                    tokenizer.push_token(next_token)
                    struct_code = parse_struct_as_is(parse_context,
                                                     indent_level + 1)
                    field = struct_code
                    next_token = tokenizer.get_next_token()
                elif next_token.spelling == "enum":
                    raise Exception("Enum in structs/unions is not suppoeted")
            while next_token and next_token.spelling != ";":
                if field and next_token.spelling != "*":
                    field += " "
                field += next_token.spelling
                next_token = tokenizer.get_next_token()
            field += ";"
            result += next_indentation + field + "\n"
            next_token = tokenizer.get_next_token()
        result += current_indentation + "}"
        next_token = tokenizer.get_next_token()
    code, next_token = struct_workaround_illegal_access(parse_context,
                                                        next_token)
    result += code
    if next_token.kind == TokenKind.PUNCTUATION and next_token.spelling == ";":
            result += ";"
    else:
        # NOTE: Can be a part of function return type, (or typedef?)
        tokenizer.push_token(next_token)
    return result


def parse_typedef_function_identifier_as_is(parse_context):
    tokenizer = parse_context.tokenizer
    # Read opening parenthesis.
    result = ""
    next_token = tokenizer.get_next_token()
    result += "("
    next_token = tokenizer.get_next_token()
    while next_token.spelling != ")":
        result += next_token.spelling
        next_token = tokenizer.get_next_token()
    result += ")"
    return result


def parse_typedef_function_arguments_as_is(parse_context):
    tokenizer = parse_context.tokenizer
    # Read opening parenthesis.
    result = ""
    next_token = tokenizer.get_next_token()
    result += "("
    arguments = ""
    interior = 1
    while interior != 0:
        next_token = tokenizer.get_next_token()
        if arguments and next_token.kind != TokenKind.PUNCTUATION:
            arguments += " "
        arguments += next_token.spelling
        if next_token.kind == TokenKind.PUNCTUATION:
            if next_token.spelling == "(":
                interior += 1
            elif next_token.spelling == ")":
                interior -= 1
    return result + arguments


def parse_typedef_as_is(parse_context):
    tokenizer = parse_context.tokenizer
    result = ""
    # Read the `typedef`.
    typedef_token = tokenizer.get_next_token()
    result += typedef_token.spelling
    next_token = tokenizer.get_next_token()
    # Type which is used as typedef.
    if next_token.kind == TokenKind.KEYWORD and \
       next_token.spelling in ("struct", "union"):
        tokenizer.push_token(next_token)
        result += " " + parse_struct_as_is(parse_context)
    elif next_token.kind == TokenKind.KEYWORD and \
         next_token.spelling == "enum":
        tokenizer.push_token(next_token)
        result += " " + parse_enum_as_is(parse_context)
    else:
        result += " " + next_token.spelling
    next_token = tokenizer.get_next_token()
    # Type name, this is the new one.
    if next_token.kind == TokenKind.PUNCTUATION and next_token.spelling == "(":
        tokenizer.push_token(next_token)
        result += parse_typedef_function_identifier_as_is(parse_context)
        result += parse_typedef_function_arguments_as_is(parse_context)
        pass
    else:
        # Loop to deal with possible `unsigned long long`
        while next_token and next_token.spelling != ";":
            if next_token.spelling == '#':
                code, next_token = struct_workaround_illegal_access(
                        parse_context, next_token)
                result += code
                continue
            result += " " + next_token.spelling
            next_token = tokenizer.get_next_token()
        tokenizer.push_token(next_token)
    # Finish typedef
    next_token = tokenizer.get_next_token()
    if next_token.kind == TokenKind.PUNCTUATION and next_token.spelling == ";":
        result += ";"
    else:
        print(result)
        raise Exception("Expected semicolon")
    return result


def parse_enum_as_is(parse_context, indent_level=0):
    tokenizer = parse_context.tokenizer
    current_indentation = "  " * indent_level
    next_indentation = "  " * (indent_level + 1)
    result = ""
    # Read `enum`.
    enum_token = tokenizer.get_next_token()
    result += enum_token.spelling
    next_token = tokenizer.get_next_token()
    # Now it could be an enum identifier.
    if next_token.kind == TokenKind.IDENTIFIER:
        result += " " + next_token.spelling
        next_token = tokenizer.get_next_token()
    if next_token.kind == TokenKind.PUNCTUATION and next_token.spelling == "{":
        result += " {\n"
        next_token = tokenizer.get_next_token()
        while next_token is not None and next_token.spelling != "}":
            result += next_indentation
            item = ""
            while (next_token is not None and
                   next_token.spelling not in (",", "}", )):
                if item:
                    if next_token.spelling != ")" and item[-1] != "(":
                        item += " "
                item += next_token.spelling
                next_token = tokenizer.get_next_token()
            result += item
            if next_token.spelling != "}":
                result += next_token.spelling
                next_token = tokenizer.get_next_token()
            result += "\n"
        next_token = tokenizer.get_next_token()
        result += current_indentation + "}"
    if next_token.kind == TokenKind.PUNCTUATION and next_token.spelling == ";":
        result += ";"
    else:
        # NOTE: Can be a part of typedef
        tokenizer.push_token(next_token)
    return result


def get_file_lines(parse_context, begin_line, end_line):
    lines = parse_context.file_lines[begin_line - 1:end_line]
    return "".join(lines).strip()


def parse_preprocessor(parse_context):
    tokenizer = parse_context.tokenizer
    # Read `#`.
    next_token = tokenizer.get_next_token()
    begin_line = next_token.location.line
    ignore = False
    # Check whether we can handle the directive.
    next_token = tokenizer.get_next_token()
    if next_token.spelling == 'include':
        ignore = True
    else:
        tokenizer.push_token(next_token)
    # Count how many lines or preprocessor we've got.
    file_lines = parse_context.file_lines
    num_lines = len(file_lines)
    end_line = begin_line
    while end_line <= num_lines and file_lines[end_line - 1].endswith("\\\n"):
        end_line += 1
    while next_token and next_token.location.line < end_line + 1:
        # TODO(sergey): Join directive together. Or maybe leave as post-pro
        # after the parsing is done, to put lines as-is.
        next_token = tokenizer.get_next_token(skip_comments=False)
    # Push token back to queue, it belongs to the next line and is not to be
    # swallowed here
    if next_token:
        tokenizer.push_token(next_token)
    if ignore:
        return None
    return get_file_lines(parse_context, begin_line, end_line)


def strip_header_guards(file_structure):
    if len(file_structure) < 3:
        return
    if (    file_structure[0].type != FileStructureEntry.TYPE_BARE_CODE or
            file_structure[1].type != FileStructureEntry.TYPE_BARE_CODE):
        return
    if (    not file_structure[0].get_code().startswith("#ifndef") or
            not file_structure[1].get_code().startswith("#define")):
        return
    del file_structure[-1]
    del file_structure[0:2]


def strip_extern_c_guards_pass(file_structure):
    num_elements = len(file_structure)
    for line, entry in enumerate(file_structure):
        if line == num_elements - 1:
            break
        if entry.type != FileStructureEntry.TYPE_BARE_CODE:
            continue
        next_entry = file_structure[line + 1]
        if next_entry.type != FileStructureEntry.TYPE_BARE_CODE:
            continue
        if (    entry.get_code() == "#ifdef __cplusplus" and
                next_entry.get_code() == "#endif"):
            del file_structure[line:line + 2]
            return True
    return False


def strip_extern_c_guards(file_structure):
    while strip_extern_c_guards_pass(file_structure):
        pass


def postprocess_file_structure(parse_context):
    file_structure = parse_context.file_structure
    strip_header_guards(file_structure)
    strip_extern_c_guards(file_structure)
    file_structure.insert(
            0,
            FileStructureEntry(
                    FileStructureEntry.TYPE_BARE_CODE,
                    "/" * 80 +
                    "\n// " + os.path.basename(parse_context.filename) + "\n"))


def combine_file_structure(module, file_structure):
    lines = []
    wrangle_typedefs = module["wrangle-typedefs"]
    keep_bare_code = wrangle_typedefs
    for entry in file_structure:
        if entry.type == FileStructureEntry.TYPE_BARE_CODE:
            if keep_bare_code:
                lines.append(entry.get_code())
        elif entry.type == FileStructureEntry.TYPE_FUNCTION_REFERENCE:
            function = entry.get_function()
            lines.append(generate_function_typedef(function))
            if wrangle_typedefs:
                lines.append(generate_function_declarations(function))
    return "\n".join(lines)


def parse_header(module_context, filename):
    print("  Parsing header {}" . format(os.path.basename(filename)))
    module = module_context.module
    parse_context = ParseContext()
    parse_context.filename = filename
    with open(filename) as f:
        parse_context.file_lines = f.readlines()
    # Initialize state machine.
    in_extern = False
    extern_tokens = []
    is_first_opening_bracket = False
    is_expecting_no_star = False
    need_check_for_function = False
    function_no_return = False
    # Initialize parser.
    idx = clang.cindex.Index.create()
    args = ('-x', 'c-header')
    tu = idx.parse(filename, args)
    parse_context.tokenizer = Tokenizer(tu.get_tokens(extent=tu.cursor.extent))
    while parse_context.tokenizer.has_tokens():
        token = parse_context.tokenizer.get_next_token(skip_comments=False)
        if token.spelling in module["ignore_tokens"]:
            continue
        if token.spelling == "#":
            if not in_extern:
                parse_context.tokenizer.push_token(token)
                preprocessor_code = parse_preprocessor(parse_context)
                if preprocessor_code:
                    parse_context.file_structure.append(
                        FileStructureEntry(FileStructureEntry.TYPE_BARE_CODE,
                                           preprocessor_code))
                continue
        if token.spelling == '"C"' and in_extern:
            in_extern = False
        elif token.kind == TokenKind.COMMENT:
            if not in_extern:
                continue
            else:
                extern_tokens.append(token)
        elif token.kind == TokenKind.KEYWORD:
            if token.spelling == 'extern':
                in_extern = True
                is_first_opening_bracket = True
                is_expecting_no_star = False
                extern_tokens.append(token)
            elif token.spelling in ("struct", "union", ):
                parse_context.tokenizer.push_token(token)
                struct_code = parse_struct_as_is(parse_context)
                if struct_code[-1] == ";":
                    parse_context.file_structure.append(
                        FileStructureEntry(FileStructureEntry.TYPE_BARE_CODE,
                                           struct_code))
                else:
                    if extern_tokens and extern_tokens[-1].spelling == "const":
                        struct_code = "const " + struct_code
                        extern_tokens[-1] = FakeToken(struct_code)
                    else:
                        extern_tokens.append(FakeToken(struct_code))
                    if in_extern:
                        is_expecting_no_star = False
                    in_extern = True
            elif token.spelling in "typedef":
                parse_context.tokenizer.push_token(token)
                parse_context.file_structure.append(
                    FileStructureEntry(FileStructureEntry.TYPE_BARE_CODE,
                                       parse_typedef_as_is(parse_context)))
            elif token.spelling in "enum":
                parse_context.tokenizer.push_token(token)
                enum_code = parse_enum_as_is(parse_context)
                if enum_code[-1] == ";":
                    parse_context.file_structure.append(
                        FileStructureEntry(FileStructureEntry.TYPE_BARE_CODE,
                                           enum_code))
                else:
                    extern_tokens.append(FakeToken(enum_code))
                    if in_extern:
                        is_expecting_no_star = False
                    in_extern = True
            elif in_extern:
                extern_tokens.append(token)
                is_expecting_no_star = False
            else:
                in_extern = True
                is_first_opening_bracket = True
                is_expecting_no_star = False
                extern_tokens.append(token)
        elif token.kind == TokenKind.PUNCTUATION:
            if token.spelling == ';':
                if len(extern_tokens) > 1:
                    function = analyze_function_tokens(
                         parse_context.functions, extern_tokens)
                    if function:
                        function.no_return = function_no_return
                        parse_context.file_structure.append(
                            FileStructureEntry(
                                    FileStructureEntry.TYPE_FUNCTION_REFERENCE,
                                    function))
                        if need_check_for_function:
                            function = parse_context.functions[-1]
                            if function.name.endswith("_fn") or \
                            function.name.endswith("Function"):
                                parse_context.file_structure = \
                                    parse_context.file_structure[:-1]
                                parse_context.functions = \
                                        parse_context.functions[:-1]
                extern_tokens = []
                in_extern = False
                function_no_return = False
                need_check_for_function = False
            elif in_extern:
                if token.spelling == '(':
                    if is_first_opening_bracket:
                        is_expecting_no_star = True
                    else:
                        is_expecting_no_star = False
                    is_first_opening_bracket = False
                elif token.spelling == '*':
                    if is_expecting_no_star:
                        need_check_for_function = True
                else:
                    is_expecting_no_star = False
                extern_tokens.append(token)
        elif in_extern:
            is_expecting_no_star = False
            spelling = token.spelling
            if spelling in ATTR_NO_RETURN:
                function_no_return = True
                continue
            if spelling in ATTR_IGNORE:
                continue
            extern_tokens.append(token)
        elif not in_extern:
            # NOTE: Assuming we are entering function declaration.
            in_extern = True
            is_first_opening_bracket = True
            is_expecting_no_star = False
            extern_tokens.append(token)
    # Now we have list of all functions and can do some real
    # wrangler generation now.
    # function_typedefs = generateTypedefs(functions)
    # function_impl_declarations = generateImplDeclarations(functions)
    # function_impl_definitions = generateImplDefinitions(functions)
    # function_wrappers = generateFunctionWrappers(functions)
    # function_loaders = generateFunctionLoaders(functions)

    postprocess_file_structure(parse_context)

    basename = os.path.basename(filename)
    module_context.file_structure[basename] = combine_file_structure(
            module, parse_context.file_structure)
    module_context.functions[basename] = parse_context.functions


###############################################################################
# Wrangler Genration

def replace_template_variables(template_variables, data):
    """
    Replace variables like %foo% in template data with actual code.
    """
    for variable, value in template_variables.iteritems():
        data = data.replace("%" + variable + "%", value)
    return data


def write_wrangler_to_file(template_variables, template, destination):
    """
    Write wrangler symbols to a template.
    """
    with open(template, "r") as input:
        data = input.read()
        data = replace_template_variables(template_variables, data)
        with open(destination, "w") as output:
            output.write(data)


def combine_all_file_structures(module_context):
    lines = []
    for filename, value in module_context.file_structure.iteritems():
        if value:
            value += "\n"
        lines.append(value)
    return "\n".join(lines)


def combine_all_functor_declarations(module_context):
    lines = []
    use_wrapper = module_context.module["use_wrapper"]
    for filename, functions in module_context.functions.iteritems():
        if not functions:
            continue
        lines.append("// " + filename)
        lines.append(generate_functor_declarations(functions, use_wrapper))
    return "\n".join(lines)


def combine_all_functor_definitions(module_context):
    lines = []
    use_wrapper = module_context.module["use_wrapper"]
    for filename, functions in module_context.functions.iteritems():
        if not functions:
            continue
        lines.append("// " + filename)
        lines.append(generate_functor_defintiions(functions, use_wrapper))
    return "\n".join(lines)


def construct_extra_wrangler_includes(module_context):
    module = module_context.module
    lines = []
    for include in module["extra_wrangler_includes"]:
        lines.append("#include <{}>".format(include))
    return "\n".join(lines)


def write_wrangler_to_files(module_context):
    """
    Write generated data from module context to actual source files.
    """
    module = module_context.module
    module_name = module_context.module["name"]
    module_name_lower = module_name.lower()
    module_name_upper = module_name.upper()
    path = os.path.dirname(os.path.realpath(__file__))
    template_variables = {
        "module_name": module_name_lower,
        "MODULE_NAME": module_name_upper,
        "ModuleName": module_name,
        "FILE_STRUCTURE": combine_all_file_structures(module_context),
        "FUNCTOR_DECLARATIONS":
            combine_all_functor_declarations(module_context),
        "FUNCTOR_DEFINITIONS": combine_all_functor_definitions(module_context),
        "FUNCTION_WRAPPERS": generate_function_wrappers(module_context),
        "VARARG_FUNCTION_WRAPPERS": generate_vararg_function_wrappers(
                module_context),
        "EXTRA_INCLUDES": construct_extra_wrangler_includes(module_context),
        "EXTRA_CODE": module["extra_wrangler_code"],
        "LIBRARY_PATHS": generate_libraries_list_code(module),
        "LIBRARIES_OPEN": generate_libraries_open_code(module),
        "FUNCTION_LOADERS": generate_function_loaders(module_context),
        "SYMBOL_CHECK": "XEW_HAS_SYMBOL_IMPL" if module["use_wrapper"]
                                              else "_XEW_HAS_SYMBOL",
        "DYNAMIC_LIBRARIES": generate_dynamic_libraries(module_context),
    }
    write_wrangler_to_file(
            template_variables,
            os.path.join(path, "module.template.h"),
            os.path.join(path, "..", "include",
                         "xew_" + module_name_lower + ".h"))
    write_wrangler_to_file(
            template_variables,
            os.path.join(path, "module.template.c"),
            os.path.join(path, "..", "source",
                         "xew_" + module_name_lower + ".c"))


###############################################################################
# Entry points

def main(modules):
    for module in modules:
        if module["ignore"]:
            continue
        print("Wrangling module {}" . format(module["name"]))
        module_context = ModuleContext()
        module_context.module = module
        for header in module["headers"]:
            full_filename = os.path.join(module["include_directory"], header)
            parse_header(module_context, full_filename)
        write_wrangler_to_files(module_context)


if __name__ == "__main__":
    main(MODULES)
