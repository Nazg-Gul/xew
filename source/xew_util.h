// Copyright (c) 2018, X Wrangler authors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
// Author: Sergey Sharybin (sergey.vfx@gmail.com)

#ifndef XEW_UTIL_H_
#define XEW_UTIL_H_

#ifdef _MSC_VER
#  if _MSC_VER < 1900
#    define snprintf _snprintf
#  endif
#  define popen _popen
#  define pclose _pclose
#  define _CRT_SECURE_NO_WARNINGS
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  define VC_EXTRALEAN
#  include <windows.h>

// Utility macros.

typedef HMODULE DynamicLibrary;

#  define dynamic_library_open(path)         LoadLibraryA(path)
#  define dynamic_library_close(lib)         FreeLibrary(lib)
#  define dynamic_library_find(lib, symbol)  GetProcAddress(lib, symbol)
#else
#  include <dlfcn.h>

typedef void* DynamicLibrary;

#  define dynamic_library_open(path)         dlopen(path, RTLD_NOW)
#  define dynamic_library_close(lib)         dlclose(lib)
#  define dynamic_library_find(lib, symbol)  dlsym(lib, symbol)
#endif

#define GLUE(A, B) _XEW_GLUE_IMPL(A, B)
#define IMPL_SUFFIX _XEW_IMPL_SUFFIX

// Used for sumbols which must exist.
#define _LIBRARY_FIND_CHECKED(lib, name)                     \
        do {                                                 \
          name = (t##name)dynamic_library_find(lib, #name);  \
          assert(name);                                      \
        } while (0)
#define _LIBRARY_FIND_IMPL_CHECKED(lib, name)                                  \
        do {                                                                   \
          GLUE(name, IMPL_SUFFIX) = (t##name)dynamic_library_find(lib, #name); \
          assert(GLUE(name, IMPL_SUFFIX));                                     \
        } while (0)

// Used for symbols which might miss in te library.
#define _LIBRARY_FIND(lib, name)                             \
        do {                                                 \
          name = (t##name)dynamic_library_find(lib, #name);  \
        } while (0)
#define _LIBRARY_FIND_IMPL(lib, name)                                          \
        do {                                                                   \
          GLUE(name, IMPL_SUFFIX) = (t##name)dynamic_library_find(lib, #name); \
        } while (0)

DynamicLibrary xew_dynamic_library_open_find(const char** paths);

#endif  // XEW_UTIL_H_
