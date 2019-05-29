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

#include <stdio.h>
#include <stdlib.h>
#include "xew_x11.h"
#include "xew_xcb.h"

static int xineramaIsActive(xcb_connection_t* xcb_connection) {
  if (xcb_connection == NULL) {
    return 0;
  }
  // if (!xcb_get_extension_data(xcb_connection, &xcb_xinerama_id)->present) {
  //  return 0;
  // }
  xcb_xinerama_is_active_reply_t* reply = xcb_xinerama_is_active_reply(
      xcb_connection, xcb_xinerama_is_active_unchecked(xcb_connection), NULL);
  if (reply == NULL) {
    return 0;
  }
  const int is_active = reply->state;
  free(reply);
  return is_active;
}

static int xineramaScreenCount(xcb_connection_t* xcb_connection) {
  if (xcb_connection == NULL) {
    return 0;
  }
  xcb_xinerama_query_screens_reply_t* reply = xcb_xinerama_query_screens_reply(
      xcb_connection, xcb_xinerama_query_screens_unchecked(xcb_connection),
      NULL);
  if (reply == NULL) {
    return 0;
  }
  const int num_screens = reply->number;
  free(reply);
  return num_screens;
}

static int xineramaScreenGeometry(xcb_connection_t* xcb_connection,
                                  int screen_index, int* x_org, int* y_org,
                                  int* width, int* height) {
  *x_org = *y_org = *width = *height = 0;
  if (xcb_connection == NULL) {
    return 0;
  }
  // NOTE: Ideally screens will be queried once and then cached, so then the
  // geometyr query is not O(N^2). But for the purpose of example this should
  // be good enough.
  xcb_xinerama_query_screens_reply_t* reply = xcb_xinerama_query_screens_reply(
      xcb_connection, xcb_xinerama_query_screens_unchecked(xcb_connection),
      NULL);
  if (reply == NULL) {
    return 0;
  }
  int result = 0;
  xcb_xinerama_screen_info_iterator_t iter =
      xcb_xinerama_query_screens_screen_info_iterator(reply);
  for (; iter.rem; --screen_index, xcb_xinerama_screen_info_next(&iter)) {
    if (screen_index == 0) {
      *x_org = iter.data->x_org;
      *y_org = iter.data->y_org;
      *width = iter.data->width;
      *height = iter.data->height;
      result = 1;
    }
  }
  free(reply);
  return result;
}

static void testXinerama(void) {
  const XewErrorCode init_result = xewX11Init();
  if (init_result != XEW_SUCCESS) {
    printf("Error initializing X, can not test Xinerama.\n");
    return;
  }
  Display* display = XOpenDisplay(NULL);
  xcb_connection_t* xcb_connection = XGetXCBConnection(display);
  if (!xineramaIsActive(xcb_connection)) {
    printf("Xinerama is not active.\n");
    return;
  }
  printf("Xinerama is active.\n");
  const int num_screens = xineramaScreenCount(xcb_connection);
  printf("Number of Xinerama screens: %d\n", num_screens);
  for (int i = 0; i < num_screens; ++i) {
    int x_org, y_org, width, height;
    if (!xineramaScreenGeometry(xcb_connection, i, &x_org, &y_org, &width,
                                &height)) {
      printf("- Error querying geometry of screen %d.\n", i);
      continue;
    }
    printf("- Screen %d: origin:(%d, %d) resolution:(%d, %d)\n", i, x_org,
           y_org, width, height);
  }
  XCloseDisplay(display);
}

int main(int argc, char* argv[]) {
  (void)argc;  // Ignored.
  (void)argv;  // Ignored.
  const XewErrorCode init_result = xewXCBInit();
  if (init_result != XEW_SUCCESS) {
    fprintf(stderr, "Failed initializing xcb: %s\n",
            xewErrorString(init_result));
    return EXIT_FAILURE;
  }
  // Some basic checks.
  printf("XCB initialized successfully\n");
  printf("HAS(xcb_connect): %s\n", XCB_HAS_SYMBOL(xcb_connect) ? "YES" : "NO");
  // Poke actual API.
  char* host = NULL;
  int display = 0, screen = 0;
  if (xcb_parse_display("127.0.0.1:0.1", &host, &display, &screen) == 0) {
    fprintf(stderr, "xcb_parse_display() failed\n");
  }
  printf("host: %s, display: %d, screen: %d\n", host ? host : "<NULL>", display,
         screen);
  if (host != NULL) {
    free(host);
  }
  if (XCB_HAS_SYMBOL(xcb_xinerama_is_active)) {
    printf("Xinerama is available.\n");
    testXinerama();
  }
  return EXIT_SUCCESS;
}
