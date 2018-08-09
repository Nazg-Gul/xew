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

#include <stdlib.h>
#include <stdio.h>
#include "xew_xkbcommon.h"

int main(int argc, char* argv[]) {
  (void) argc;  // Ignored.
  (void) argv;  // Ignored.
  const XewErrorCode init_result = xewXKBCommonInit();
  if (init_result != XEW_SUCCESS) {
    fprintf(stderr, "Failed initializing xkbcommon: %s\n",
            xewErrorString(init_result));
    return EXIT_FAILURE;
  }
  printf("xkbcommon initialized successfully\n");
  printf("HAS(xkb_context_new): %s\n",
         XKBCOMMON_HAS_SYMBOL(xkb_context_new) ? "YES" : "NO");
  // TODO(sergey): Perform more extense test, to see API actually works.
  // As in, call some functions, check result etc.
  return EXIT_SUCCESS;
}
