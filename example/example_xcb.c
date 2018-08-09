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
#include "xew_xcb.h"

int main(int argc, char* argv[]) {
  (void) argc;  // Ignored.
  (void) argv;  // Ignored.
  const XewErrorCode init_result = xewXCBInit();
  if (init_result != XEW_SUCCESS) {
    fprintf(stderr, "Failed initializing xcb: %s\n",
            xewErrorString(init_result));
    return EXIT_FAILURE;
  }
  // Some basic checks.
  printf("XCB initialized successfully\n");
  printf("HAS(xcb_connect): %s\n",
         XCB_HAS_SYMBOL(xcb_connect) ? "YES" : "NO");
  // Poke actual API.
  char* host = NULL;
  int display = 0, screen = 0;
  if (xcb_parse_display("127.0.0.1:0.1", &host, &display, &screen) == 0) {
    fprintf(stderr, "xcb_parse_display() failed\n");
  }
  printf("host: %s, display: %d, screen: %d\n",
         host ? host : "<NULL>", display, screen);
  if (host != NULL) {
    free(host);
  }
  return EXIT_SUCCESS;
}
