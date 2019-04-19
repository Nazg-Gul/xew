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

#ifndef %MODULE_NAME%_H_
#define %MODULE_NAME%_H_

#include "xew.h"

%EXTRA_INCLUDES%

#ifdef __cplusplus
extern "C" {
#endif

%EXTRA_CODE%
%FILE_STRUCTURE%
////////////////////////////////////////////////////////////////////////////////
// Functors declarations.

%FUNCTOR_DECLARATIONS%
////////////////////////////////////////////////////////////////////////////////
// Wrangler.

XewErrorCode xew%ModuleName%Init(void);

// Check whether given function is available.
#define %MODULE_NAME%_HAS_SYMBOL(symbol) (%SYMBOL_CHECK%(symbol))

////////////////////////////////////////////////////////////////////////////////
// Variadic functions
//
// NOTE: Those can not be used as a fully transparent DL_PRELOAD replacement for
// the real ones, since we can not pass variadic arguments from one function to
// another.
// This means, to use those the application eeds to be actually complied using
// this header file.

#ifndef XEW_IMPL
%VARARG_FUNCTION_WRAPPERS%
#endif  // XEW_IMPL

#ifdef __cplusplus
}
#endif

#endif  // %MODULE_NAME%_SYMBOLS_H_
