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

#ifndef XEW_H_
#define XEW_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum XewErrorCode {
  // Operation finished successfully 
  XEW_SUCCESS = 0,
  // Opening of one or more crucial libraries has failed.
  XEW_ERROR_OPEN_FAILED = -1,
  // Setting atexit() handler has failed.
  XEW_ERROR_ATEXIT_FAILED = -2,
} XewErrorCode;

const char* xewErrorString(XewErrorCode error);

#define _XEW_GLUE_IMPL(A, B) A ## B
#define _XEW_GLUE(A, B) _XEW_GLUE_IMPL(A, B)
#define _XEW_IMPL_SUFFIX _impl

#define XEW_HAS_SYMBOL_IMPL(symbol)  \
    (_XEW_GLUE(symbol, _XEW_IMPL_SUFFIX) != NULL)

#ifdef __cplusplus
}
#endif

#endif  // XEW_H_
