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

#define XEW_IMPL

#include "xew_%module_name%.h"

#include "xew_util.h"

// Per-platform list of libraries to be queried/
%LIBRARY_PATHS%

%DYNAMIC_LIBRARIES%

////////////////////////////////////////////////////////////////////////////////
// Functor definitions.

%FUNCTOR_DEFINITIONS%

////////////////////////////////////////////////////////////////////////////////
// Function wrappers.

%FUNCTION_WRAPPERS%

////////////////////////////////////////////////////////////////////////////////
// Main wrangling logic.

static void atExitHandler(void) {
  if (%module_name%_lib != NULL) {
    // Currently we ignore errors.
    // TODO(sergey): It might be a good idea to print errors to stderr at least?
    dynamic_library_close(%module_name%_lib);
    %module_name%_lib = NULL;
  }
}

static XewErrorCode installAtExitHandler(void) {
  int error = atexit(atExitHandler);
  if (error) {
    return XEW_ERROR_ATEXIT_FAILED;
  }
  return XEW_SUCCESS;
}

static XewErrorCode openLibraries() {
%LIBRARIES_OPEN%
  return XEW_SUCCESS;
}

static void fetchPointersFromLibrary(void) {
%FUNCTION_LOADERS%
}

XewErrorCode xew%ModuleName%Init(void) {
  // Check whether we've already attempted to initialize this module.
  static int is_initialized = 0;
  static XewErrorCode result = 0;
  if (is_initialized) {
    return result;
  }
  is_initialized = 1;
  // Install handler which s run when application quits.
  result = installAtExitHandler();
  if (result != XEW_SUCCESS) {
    return result;
  }
  // Open libraries
  result = openLibraries();
  if (result != XEW_SUCCESS) {
    return result;
  }
  // Fetch all function pointers.
  fetchPointersFromLibrary();
  result = XEW_SUCCESS;
  return result;
}
