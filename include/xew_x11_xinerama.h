// Copyright (c) 2019, X Wrangler authors
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

#ifndef XEW_X11_XINERAMA_H_
#define XEW_X11_XINERAMA_H_

#include "xew_x11.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int screen_number;
  short x_org;
  short y_org;
  short width;
  short height;
} XineramaScreenInfo;

Bool XineramaQueryExtension(Display *dpy, int *event_base, int *error_base);

Status XineramaQueryVersion(Display *dpy,
                            int *major_versionp,
                            int *minor_versionp);

Bool XineramaIsActive(Display *dpy);

XineramaScreenInfo *XineramaQueryScreens(Display *dpy, int *number);

#ifdef __cplusplus
}
#endif

#endif  // XEW_X11_XINERAMA_H_
