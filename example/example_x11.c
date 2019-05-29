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
#include <string.h>
#include "xew_x11.h"

static void openTestWindow(void) {
  Display *display;
  Window window;
  int screen;
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }
  screen = DefaultScreen(display);
  window = XCreateSimpleWindow(display,
                               RootWindow(display, screen),
                               10, 10,
                               100, 100,
                               1,
                               BlackPixel(display, screen),
                               WhitePixel(display, screen));
  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);
  while (1) {
    XEvent event;
    XNextEvent(display, &event);
    if (event.type == Expose) {
      const char* message = "Hello, World!";
      GC gc = DefaultGC(display, screen);
      XFillRectangle(display, window, gc, 20, 20, 10, 10);
      XDrawString(display, window, gc, 10, 50, message, strlen(message));
    }
    if (event.type == KeyPress) {
      break;
    }
  }
  XCloseDisplay(display);
}

static void testXinerama(void) {
  Display* display = XOpenDisplay(NULL);
  if (!XineramaIsActive(display)) {
    printf("Xinerama is not active.\n");
    XCloseDisplay(display);
    return;
  }
  printf("Xinerama is active.\n");
  int num_screens;
  XineramaScreenInfo* screen_info = XineramaQueryScreens(display, &num_screens);
  printf("Number of Xinerama screens: %d\n", num_screens);
  for (int i = 0; i < num_screens; ++i) {
    printf("- Screen %d: origin:(%d, %d) resolution:(%d, %d)\n",
           i,
           screen_info[i].x_org, screen_info[i].y_org,
           screen_info[i].width, screen_info[i].height);
  }
  XFree(screen_info);
  XCloseDisplay(display);
}

int main(int argc, char* argv[]) {
  (void) argc;  // Ignored.
  (void) argv;  // Ignored.
  if (xewX11Init() == XEW_SUCCESS) {
    printf("X11 found\n");
    if (X11_HAS_SYMBOL(XineramaIsActive)) {
      printf("Found Xinerama\n");
      testXinerama();
    }
    openTestWindow();
  }
  else {
    printf("X11 not found\n");
  }
  return EXIT_SUCCESS;
}
