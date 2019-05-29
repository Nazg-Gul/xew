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

#ifndef X11_H_
#define X11_H_

#include "xew.h"

#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include "xew_x11_xinerama.h"

#ifdef __cplusplus
extern "C" {
#endif


// min/max conflicts with STL includes.

#undef min
#undef max
// This is to prevent us from include Xlib-xcb, which will cause type conflicts
// with out wrangler.
typedef struct xcb_connection_t xcb_connection_t;
enum XEventQueueOwner {
  XlibOwnsEventQueue = 0,
  XCBOwnsEventQueue
};
// Since thw whole header is ignored, we manually define symbols from it.
xcb_connection_t* XGetXCBConnection(Display* dpy);
void XSetEventQueueOwner(Display* dpy, enum XEventQueueOwner owner);
typedef int (*t_Xmblen)(char* str, int len);
typedef XFontStruct* (*tXLoadQueryFont)(Display* display, _Xconst char* name);
typedef XFontStruct* (*tXQueryFont)(Display* display, XID font_ID);
typedef XTimeCoord* (*tXGetMotionEvents)(
    Display* display,
    Window w,
    Time start,
    Time stop,
    int* nevents_return);
typedef XModifierKeymap* (*tXDeleteModifiermapEntry)(
    XModifierKeymap* modmap,
#if NeedWidePrototypes
    unsigned int keycode_entry,
#else
    KeyCode keycode_entry,
#endif
    int modifier);
typedef XModifierKeymap* (*tXGetModifierMapping)(Display* display);
typedef XModifierKeymap* (*tXInsertModifiermapEntry)(
    XModifierKeymap* modmap,
#if NeedWidePrototypes
    unsigned int keycode_entry,
#else
    KeyCode keycode_entry,
#endif
    int modifier);
typedef XModifierKeymap* (*tXNewModifiermap)(int max_keys_per_mod);
typedef XImage* (*tXCreateImage)(
    Display* display,
    Visual* visual,
    unsigned int depth,
    int format,
    int offset,
    char* data,
    unsigned int width,
    unsigned int height,
    int bitmap_pad,
    int bytes_per_line);
typedef Status (*tXInitImage)(XImage* image);
typedef XImage* (*tXGetImage)(
    Display* display,
    Drawable d,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    unsigned long plane_mask,
    int format);
typedef XImage* (*tXGetSubImage)(
    Display* display,
    Drawable d,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    unsigned long plane_mask,
    int format,
    XImage* dest_image,
    int dest_x,
    int dest_y);
typedef Display* (*tXOpenDisplay)(_Xconst char* display_name);
typedef void (*tXrmInitialize)(void);
typedef char* (*tXFetchBytes)(Display* display, int* nbytes_return);
typedef char* (*tXFetchBuffer)(Display* display, int* nbytes_return, int buffer);
typedef char* (*tXGetAtomName)(Display* display, Atom atom);
typedef Status (*tXGetAtomNames)(
    Display* dpy,
    Atom* atoms,
    int count,
    char** names_return);
typedef char* (*tXGetDefault)(
    Display* display,
    _Xconst char* program,
    _Xconst char* option);
typedef char* (*tXDisplayName)(_Xconst char* string);
typedef char* (*tXKeysymToString)(KeySym keysym);
typedef int (* (*tXSynchronize)(
    Display* display,
    Bool onoff))(Display* /* display */);
typedef Atom (*tXInternAtom)(
    Display* display,
    _Xconst char* atom_name,
    Bool only_if_exists);
typedef Status (*tXInternAtoms)(
    Display* dpy,
    char** names,
    int count,
    Bool onlyIfExists,
    Atom* atoms_return);
typedef Colormap (*tXCopyColormapAndFree)(Display* display, Colormap colormap);
typedef Colormap (*tXCreateColormap)(
    Display* display,
    Window w,
    Visual* visual,
    int alloc);
typedef Cursor (*tXCreatePixmapCursor)(
    Display* display,
    Pixmap source,
    Pixmap mask,
    XColor* foreground_color,
    XColor* background_color,
    unsigned int x,
    unsigned int y);
typedef Cursor (*tXCreateGlyphCursor)(
    Display* display,
    Font source_font,
    Font mask_font,
    unsigned int source_char,
    unsigned int mask_char,
    XColor _Xconst* foreground_color,
    XColor _Xconst* background_color);
typedef Cursor (*tXCreateFontCursor)(Display* display, unsigned int shape);
typedef Font (*tXLoadFont)(Display* display, _Xconst char* name);
typedef GC (*tXCreateGC)(
    Display* display,
    Drawable d,
    unsigned long valuemask,
    XGCValues* values);
typedef GContext (*tXGContextFromGC)(GC gc);
typedef void (*tXFlushGC)(Display* display, GC gc);
typedef Pixmap (*tXCreatePixmap)(
    Display* display,
    Drawable d,
    unsigned int width,
    unsigned int height,
    unsigned int depth);
typedef Pixmap (*tXCreateBitmapFromData)(
    Display* display,
    Drawable d,
    _Xconst char* data,
    unsigned int width,
    unsigned int height);
typedef Pixmap (*tXCreatePixmapFromBitmapData)(
    Display* display,
    Drawable d,
    char* data,
    unsigned int width,
    unsigned int height,
    unsigned long fg,
    unsigned long bg,
    unsigned int depth);
typedef Window (*tXCreateSimpleWindow)(
    Display* display,
    Window parent,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    unsigned int border_width,
    unsigned long border,
    unsigned long background);
typedef Window (*tXGetSelectionOwner)(Display* display, Atom selection);
typedef Window (*tXCreateWindow)(
    Display* display,
    Window parent,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    unsigned int border_width,
    int depth,
    unsigned int class_,
    Visual* visual,
    unsigned long valuemask,
    XSetWindowAttributes* attributes);
typedef Colormap* (*tXListInstalledColormaps)(
    Display* display,
    Window w,
    int* num_return);
typedef char** (*tXListFonts)(
    Display* display,
    _Xconst char* pattern,
    int maxnames,
    int* actual_count_return);
typedef char** (*tXListFontsWithInfo)(
    Display* display,
    _Xconst char* pattern,
    int maxnames,
    int* count_return,
    XFontStruct** info_return);
typedef char** (*tXGetFontPath)(Display* display, int* npaths_return);
typedef char** (*tXListExtensions)(Display* display, int* nextensions_return);
typedef Atom* (*tXListProperties)(Display* display, Window w, int* num_prop_return);
typedef XHostAddress* (*tXListHosts)(
    Display* display,
    int* nhosts_return,
    Bool* state_return);
typedef KeySym (*tXKeycodeToKeysym)(
    Display* display,
#if NeedWidePrototypes
    unsigned int keycode,
#else
    KeyCode keycode,
#endif
    int index);
typedef KeySym (*tXLookupKeysym)(XKeyEvent* key_event, int index);
typedef KeySym* (*tXGetKeyboardMapping)(
    Display* display,
#if NeedWidePrototypes
    unsigned int first_keycode,
#else
    KeyCode first_keycode,
#endif
    int keycode_count,
    int* keysyms_per_keycode_return);
typedef KeySym (*tXStringToKeysym)(_Xconst char* string);
typedef long (*tXMaxRequestSize)(Display* display);
typedef long (*tXExtendedMaxRequestSize)(Display* display);
typedef char* (*tXResourceManagerString)(Display* display);
typedef char* (*tXScreenResourceString)(Screen* screen);
typedef unsigned long (*tXDisplayMotionBufferSize)(Display* display);
typedef VisualID (*tXVisualIDFromVisual)(Visual* visual);
typedef Status (*tXInitThreads)(void);
typedef void (*tXLockDisplay)(Display* display);
typedef void (*tXUnlockDisplay)(Display* display);
typedef XExtCodes* (*tXInitExtension)(Display* display, _Xconst char* name);
typedef XExtCodes* (*tXAddExtension)(Display* display);
typedef XExtData* (*tXFindOnExtensionList)(XExtData** structure, int number);
typedef XExtData** (*tXEHeadOfExtensionList)(XEDataObject object);
typedef Window (*tXRootWindow)(Display* display, int screen_number);
typedef Window (*tXDefaultRootWindow)(Display* display);
typedef Window (*tXRootWindowOfScreen)(Screen* screen);
typedef Visual* (*tXDefaultVisual)(Display* display, int screen_number);
typedef Visual* (*tXDefaultVisualOfScreen)(Screen* screen);
typedef GC (*tXDefaultGC)(Display* display, int screen_number);
typedef GC (*tXDefaultGCOfScreen)(Screen* screen);
typedef unsigned long (*tXBlackPixel)(Display* display, int screen_number);
typedef unsigned long (*tXWhitePixel)(Display* display, int screen_number);
typedef unsigned long (*tXAllPlanes)(void);
typedef unsigned long (*tXBlackPixelOfScreen)(Screen* screen);
typedef unsigned long (*tXWhitePixelOfScreen)(Screen* screen);
typedef unsigned long (*tXNextRequest)(Display* display);
typedef unsigned long (*tXLastKnownRequestProcessed)(Display* display);
typedef char* (*tXServerVendor)(Display* display);
typedef char* (*tXDisplayString)(Display* display);
typedef Colormap (*tXDefaultColormap)(Display* display, int screen_number);
typedef Colormap (*tXDefaultColormapOfScreen)(Screen* screen);
typedef Display* (*tXDisplayOfScreen)(Screen* screen);
typedef Screen* (*tXScreenOfDisplay)(Display* display, int screen_number);
typedef Screen* (*tXDefaultScreenOfDisplay)(Display* display);
typedef long (*tXEventMaskOfScreen)(Screen* screen);
typedef int (*tXScreenNumberOfScreen)(Screen* screen);
typedef XErrorHandler (*tXSetErrorHandler)(XErrorHandler handler);
typedef XIOErrorHandler (*tXSetIOErrorHandler)(XIOErrorHandler handler);
typedef XPixmapFormatValues* (*tXListPixmapFormats)(
    Display* display,
    int* count_return);
typedef int* (*tXListDepths)(Display* display, int screen_number, int* count_return);
typedef Status (*tXReconfigureWMWindow)(
    Display* display,
    Window w,
    int screen_number,
    unsigned int mask,
    XWindowChanges* changes);
typedef Status (*tXGetWMProtocols)(
    Display* display,
    Window w,
    Atom** protocols_return,
    int* count_return);
typedef Status (*tXSetWMProtocols)(
    Display* display,
    Window w,
    Atom* protocols,
    int count);
typedef Status (*tXIconifyWindow)(Display* display, Window w, int screen_number);
typedef Status (*tXWithdrawWindow)(Display* display, Window w, int screen_number);
typedef Status (*tXGetCommand)(
    Display* display,
    Window w,
    char*** argv_return,
    int* argc_return);
typedef Status (*tXGetWMColormapWindows)(
    Display* display,
    Window w,
    Window** windows_return,
    int* count_return);
typedef Status (*tXSetWMColormapWindows)(
    Display* display,
    Window w,
    Window* colormap_windows,
    int count);
typedef void (*tXFreeStringList)(char** list);
typedef int (*tXSetTransientForHint)(Display* display, Window w, Window prop_window);
typedef int (*tXActivateScreenSaver)(Display* display);
typedef int (*tXAddHost)(Display* display, XHostAddress* host);
typedef int (*tXAddHosts)(Display* display, XHostAddress* hosts, int num_hosts);
typedef int (*tXAddToExtensionList)(
    struct _XExtData** structure,
    XExtData* ext_data);
typedef int (*tXAddToSaveSet)(Display* display, Window w);
typedef Status (*tXAllocColor)(
    Display* display,
    Colormap colormap,
    XColor* screen_in_out);
typedef Status (*tXAllocColorCells)(
    Display* display,
    Colormap colormap,
    Bool contig,
    unsigned long* plane_masks_return,
    unsigned int nplanes,
    unsigned long* pixels_return,
    unsigned int npixels);
typedef Status (*tXAllocColorPlanes)(
    Display* display,
    Colormap colormap,
    Bool contig,
    unsigned long* pixels_return,
    int ncolors,
    int nreds,
    int ngreens,
    int nblues,
    unsigned long* rmask_return,
    unsigned long* gmask_return,
    unsigned long* bmask_return);
typedef Status (*tXAllocNamedColor)(
    Display* display,
    Colormap colormap,
    _Xconst char* color_name,
    XColor* screen_def_return,
    XColor* exact_def_return);
typedef int (*tXAllowEvents)(Display* display, int event_mode, Time time);
typedef int (*tXAutoRepeatOff)(Display* display);
typedef int (*tXAutoRepeatOn)(Display* display);
typedef int (*tXBell)(Display* display, int percent);
typedef int (*tXBitmapBitOrder)(Display* display);
typedef int (*tXBitmapPad)(Display* display);
typedef int (*tXBitmapUnit)(Display* display);
typedef int (*tXCellsOfScreen)(Screen* screen);
typedef int (*tXChangeActivePointerGrab)(
    Display* display,
    unsigned int event_mask,
    Cursor cursor,
    Time time);
typedef int (*tXChangeGC)(
    Display* display,
    GC gc,
    unsigned long valuemask,
    XGCValues* values);
typedef int (*tXChangeKeyboardControl)(
    Display* display,
    unsigned long value_mask,
    XKeyboardControl* values);
typedef int (*tXChangeKeyboardMapping)(
    Display* display,
    int first_keycode,
    int keysyms_per_keycode,
    KeySym* keysyms,
    int num_codes);
typedef int (*tXChangePointerControl)(
    Display* display,
    Bool do_accel,
    Bool do_threshold,
    int accel_numerator,
    int accel_denominator,
    int threshold);
typedef int (*tXChangeProperty)(
    Display* display,
    Window w,
    Atom property,
    Atom type,
    int format,
    int mode,
    _Xconst unsigned char* data,
    int nelements);
typedef int (*tXChangeSaveSet)(Display* display, Window w, int change_mode);
typedef int (*tXChangeWindowAttributes)(
    Display* display,
    Window w,
    unsigned long valuemask,
    XSetWindowAttributes* attributes);
typedef Bool (*tXCheckIfEvent)(
    Display* display,
    XEvent* event_return,
    Bool (*predicate)(Display* /* display */, XEvent* /* event */, XPointer /* arg */),
    XPointer arg);
typedef Bool (*tXCheckMaskEvent)(
    Display* display,
    long event_mask,
    XEvent* event_return);
typedef Bool (*tXCheckTypedEvent)(
    Display* display,
    int event_type,
    XEvent* event_return);
typedef Bool (*tXCheckTypedWindowEvent)(
    Display* display,
    Window w,
    int event_type,
    XEvent* event_return);
typedef Bool (*tXCheckWindowEvent)(
    Display* display,
    Window w,
    long event_mask,
    XEvent* event_return);
typedef int (*tXCirculateSubwindows)(Display* display, Window w, int direction);
typedef int (*tXCirculateSubwindowsDown)(Display* display, Window w);
typedef int (*tXCirculateSubwindowsUp)(Display* display, Window w);
typedef int (*tXClearArea)(
    Display* display,
    Window w,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    Bool exposures);
typedef int (*tXClearWindow)(Display* display, Window w);
typedef int (*tXCloseDisplay)(Display* display);
typedef int (*tXConfigureWindow)(
    Display* display,
    Window w,
    unsigned int value_mask,
    XWindowChanges* values);
typedef int (*tXConnectionNumber)(Display* display);
typedef int (*tXConvertSelection)(
    Display* display,
    Atom selection,
    Atom target,
    Atom property,
    Window requestor,
    Time time);
typedef int (*tXCopyArea)(
    Display* display,
    Drawable src,
    Drawable dest,
    GC gc,
    int src_x,
    int src_y,
    unsigned int width,
    unsigned int height,
    int dest_x,
    int dest_y);
typedef int (*tXCopyGC)(Display* display, GC src, unsigned long valuemask, GC dest);
typedef int (*tXCopyPlane)(
    Display* display,
    Drawable src,
    Drawable dest,
    GC gc,
    int src_x,
    int src_y,
    unsigned int width,
    unsigned int height,
    int dest_x,
    int dest_y,
    unsigned long plane);
typedef int (*tXDefaultDepth)(Display* display, int screen_number);
typedef int (*tXDefaultDepthOfScreen)(Screen* screen);
typedef int (*tXDefaultScreen)(Display* display);
typedef int (*tXDefineCursor)(Display* display, Window w, Cursor cursor);
typedef int (*tXDeleteProperty)(Display* display, Window w, Atom property);
typedef int (*tXDestroyWindow)(Display* display, Window w);
typedef int (*tXDestroySubwindows)(Display* display, Window w);
typedef int (*tXDoesBackingStore)(Screen* screen);
typedef Bool (*tXDoesSaveUnders)(Screen* screen);
typedef int (*tXDisableAccessControl)(Display* display);
typedef int (*tXDisplayCells)(Display* display, int screen_number);
typedef int (*tXDisplayHeight)(Display* display, int screen_number);
typedef int (*tXDisplayHeightMM)(Display* display, int screen_number);
typedef int (*tXDisplayKeycodes)(
    Display* display,
    int* min_keycodes_return,
    int* max_keycodes_return);
typedef int (*tXDisplayPlanes)(Display* display, int screen_number);
typedef int (*tXDisplayWidth)(Display* display, int screen_number);
typedef int (*tXDisplayWidthMM)(Display* display, int screen_number);
typedef int (*tXDrawArc)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    int angle1,
    int angle2);
typedef int (*tXDrawArcs)(Display* display, Drawable d, GC gc, XArc* arcs, int narcs);
typedef int (*tXDrawImageString)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    _Xconst char* string,
    int length);
typedef int (*tXDrawImageString16)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    _Xconst XChar2b* string,
    int length);
typedef int (*tXDrawLine)(Display* display, Drawable d, GC gc, int x1, int y1, int x2, int y2);
typedef int (*tXDrawLines)(
    Display* display,
    Drawable d,
    GC gc,
    XPoint* points,
    int npoints,
    int mode);
typedef int (*tXDrawPoint)(Display* display, Drawable d, GC gc, int x, int y);
typedef int (*tXDrawPoints)(
    Display* display,
    Drawable d,
    GC gc,
    XPoint* points,
    int npoints,
    int mode);
typedef int (*tXDrawRectangle)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    unsigned int width,
    unsigned int height);
typedef int (*tXDrawRectangles)(
    Display* display,
    Drawable d,
    GC gc,
    XRectangle* rectangles,
    int nrectangles);
typedef int (*tXDrawSegments)(
    Display* display,
    Drawable d,
    GC gc,
    XSegment* segments,
    int nsegments);
typedef int (*tXDrawString)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    _Xconst char* string,
    int length);
typedef int (*tXDrawString16)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    _Xconst XChar2b* string,
    int length);
typedef int (*tXDrawText)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    XTextItem* items,
    int nitems);
typedef int (*tXDrawText16)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    XTextItem16* items,
    int nitems);
typedef int (*tXEnableAccessControl)(Display* display);
typedef int (*tXEventsQueued)(Display* display, int mode);
typedef Status (*tXFetchName)(Display* display, Window w, char** window_name_return);
typedef int (*tXFillArc)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    int angle1,
    int angle2);
typedef int (*tXFillArcs)(Display* display, Drawable d, GC gc, XArc* arcs, int narcs);
typedef int (*tXFillPolygon)(
    Display* display,
    Drawable d,
    GC gc,
    XPoint* points,
    int npoints,
    int shape,
    int mode);
typedef int (*tXFillRectangle)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    unsigned int width,
    unsigned int height);
typedef int (*tXFillRectangles)(
    Display* display,
    Drawable d,
    GC gc,
    XRectangle* rectangles,
    int nrectangles);
typedef int (*tXFlush)(Display* display);
typedef int (*tXForceScreenSaver)(Display* display, int mode);
typedef int (*tXFree)(void* data);
typedef int (*tXFreeColormap)(Display* display, Colormap colormap);
typedef int (*tXFreeColors)(
    Display* display,
    Colormap colormap,
    unsigned long* pixels,
    int npixels,
    unsigned long planes);
typedef int (*tXFreeCursor)(Display* display, Cursor cursor);
typedef int (*tXFreeExtensionList)(char** list);
typedef int (*tXFreeFont)(Display* display, XFontStruct* font_struct);
typedef int (*tXFreeFontInfo)(char** names, XFontStruct* free_info, int actual_count);
typedef int (*tXFreeFontNames)(char** list);
typedef int (*tXFreeFontPath)(char** list);
typedef int (*tXFreeGC)(Display* display, GC gc);
typedef int (*tXFreeModifiermap)(XModifierKeymap* modmap);
typedef int (*tXFreePixmap)(Display* display, Pixmap pixmap);
typedef int (*tXGeometry)(
    Display* display,
    int screen,
    _Xconst char* position,
    _Xconst char* default_position,
    unsigned int bwidth,
    unsigned int fwidth,
    unsigned int fheight,
    int xadder,
    int yadder,
    int* x_return,
    int* y_return,
    int* width_return,
    int* height_return);
typedef int (*tXGetErrorDatabaseText)(
    Display* display,
    _Xconst char* name,
    _Xconst char* message,
    _Xconst char* default_string,
    char* buffer_return,
    int length);
typedef int (*tXGetErrorText)(
    Display* display,
    int code,
    char* buffer_return,
    int length);
typedef Bool (*tXGetFontProperty)(
    XFontStruct* font_struct,
    Atom atom,
    unsigned long* value_return);
typedef Status (*tXGetGCValues)(
    Display* display,
    GC gc,
    unsigned long valuemask,
    XGCValues* values_return);
typedef Status (*tXGetGeometry)(
    Display* display,
    Drawable d,
    Window* root_return,
    int* x_return,
    int* y_return,
    unsigned int* width_return,
    unsigned int* height_return,
    unsigned int* border_width_return,
    unsigned int* depth_return);
typedef Status (*tXGetIconName)(Display* display, Window w, char** icon_name_return);
typedef int (*tXGetInputFocus)(
    Display* display,
    Window* focus_return,
    int* revert_to_return);
typedef int (*tXGetKeyboardControl)(
    Display* display,
    XKeyboardState* values_return);
typedef int (*tXGetPointerControl)(
    Display* display,
    int* accel_numerator_return,
    int* accel_denominator_return,
    int* threshold_return);
typedef int (*tXGetPointerMapping)(
    Display* display,
    unsigned char* map_return,
    int nmap);
typedef int (*tXGetScreenSaver)(
    Display* display,
    int* timeout_return,
    int* interval_return,
    int* prefer_blanking_return,
    int* allow_exposures_return);
typedef Status (*tXGetTransientForHint)(
    Display* display,
    Window w,
    Window* prop_window_return);
typedef int (*tXGetWindowProperty)(
    Display* display,
    Window w,
    Atom property,
    long long_offset,
    long long_length,
    Bool delete_,
    Atom req_type,
    Atom* actual_type_return,
    int* actual_format_return,
    unsigned long* nitems_return,
    unsigned long* bytes_after_return,
    unsigned char** prop_return);
typedef Status (*tXGetWindowAttributes)(
    Display* display,
    Window w,
    XWindowAttributes* window_attributes_return);
typedef int (*tXGrabButton)(
    Display* display,
    unsigned int button,
    unsigned int modifiers,
    Window grab_window,
    Bool owner_events,
    unsigned int event_mask,
    int pointer_mode,
    int keyboard_mode,
    Window confine_to,
    Cursor cursor);
typedef int (*tXGrabKey)(
    Display* display,
    int keycode,
    unsigned int modifiers,
    Window grab_window,
    Bool owner_events,
    int pointer_mode,
    int keyboard_mode);
typedef int (*tXGrabKeyboard)(
    Display* display,
    Window grab_window,
    Bool owner_events,
    int pointer_mode,
    int keyboard_mode,
    Time time);
typedef int (*tXGrabPointer)(
    Display* display,
    Window grab_window,
    Bool owner_events,
    unsigned int event_mask,
    int pointer_mode,
    int keyboard_mode,
    Window confine_to,
    Cursor cursor,
    Time time);
typedef int (*tXGrabServer)(Display* display);
typedef int (*tXHeightMMOfScreen)(Screen* screen);
typedef int (*tXHeightOfScreen)(Screen* screen);
typedef int (*tXIfEvent)(
    Display* display,
    XEvent* event_return,
    Bool (*predicate)(Display* /* display */, XEvent* /* event */, XPointer /* arg */),
    XPointer arg);
typedef int (*tXImageByteOrder)(Display* display);
typedef int (*tXInstallColormap)(Display* display, Colormap colormap);
typedef KeyCode (*tXKeysymToKeycode)(Display* display, KeySym keysym);
typedef int (*tXKillClient)(Display* display, XID resource);
typedef Status (*tXLookupColor)(
    Display* display,
    Colormap colormap,
    _Xconst char* color_name,
    XColor* exact_def_return,
    XColor* screen_def_return);
typedef int (*tXLowerWindow)(Display* display, Window w);
typedef int (*tXMapRaised)(Display* display, Window w);
typedef int (*tXMapSubwindows)(Display* display, Window w);
typedef int (*tXMapWindow)(Display* display, Window w);
typedef int (*tXMaskEvent)(Display* display, long event_mask, XEvent* event_return);
typedef int (*tXMaxCmapsOfScreen)(Screen* screen);
typedef int (*tXMinCmapsOfScreen)(Screen* screen);
typedef int (*tXMoveResizeWindow)(
    Display* display,
    Window w,
    int x,
    int y,
    unsigned int width,
    unsigned int height);
typedef int (*tXMoveWindow)(Display* display, Window w, int x, int y);
typedef int (*tXNextEvent)(Display* display, XEvent* event_return);
typedef int (*tXNoOp)(Display* display);
typedef Status (*tXParseColor)(
    Display* display,
    Colormap colormap,
    _Xconst char* spec,
    XColor* exact_def_return);
typedef int (*tXParseGeometry)(
    _Xconst char* parsestring,
    int* x_return,
    int* y_return,
    unsigned int* width_return,
    unsigned int* height_return);
typedef int (*tXPeekEvent)(Display* display, XEvent* event_return);
typedef int (*tXPeekIfEvent)(
    Display* display,
    XEvent* event_return,
    Bool (*predicate)(Display* /* display */, XEvent* /* event */, XPointer /* arg */),
    XPointer arg);
typedef int (*tXPending)(Display* display);
typedef int (*tXPlanesOfScreen)(Screen* screen);
typedef int (*tXProtocolRevision)(Display* display);
typedef int (*tXProtocolVersion)(Display* display);
typedef int (*tXPutBackEvent)(Display* display, XEvent* event);
typedef int (*tXPutImage)(
    Display* display,
    Drawable d,
    GC gc,
    XImage* image,
    int src_x,
    int src_y,
    int dest_x,
    int dest_y,
    unsigned int width,
    unsigned int height);
typedef int (*tXQLength)(Display* display);
typedef Status (*tXQueryBestCursor)(
    Display* display,
    Drawable d,
    unsigned int width,
    unsigned int height,
    unsigned int* width_return,
    unsigned int* height_return);
typedef Status (*tXQueryBestSize)(
    Display* display,
    int class_,
    Drawable which_screen,
    unsigned int width,
    unsigned int height,
    unsigned int* width_return,
    unsigned int* height_return);
typedef Status (*tXQueryBestStipple)(
    Display* display,
    Drawable which_screen,
    unsigned int width,
    unsigned int height,
    unsigned int* width_return,
    unsigned int* height_return);
typedef Status (*tXQueryBestTile)(
    Display* display,
    Drawable which_screen,
    unsigned int width,
    unsigned int height,
    unsigned int* width_return,
    unsigned int* height_return);
typedef int (*tXQueryColor)(Display* display, Colormap colormap, XColor* def_in_out);
typedef int (*tXQueryColors)(
    Display* display,
    Colormap colormap,
    XColor* defs_in_out,
    int ncolors);
typedef Bool (*tXQueryExtension)(
    Display* display,
    _Xconst char* name,
    int* major_opcode_return,
    int* first_event_return,
    int* first_error_return);
typedef int (*tXQueryKeymap)(Display* display, char keys_return[32]);
typedef Bool (*tXQueryPointer)(
    Display* display,
    Window w,
    Window* root_return,
    Window* child_return,
    int* root_x_return,
    int* root_y_return,
    int* win_x_return,
    int* win_y_return,
    unsigned int* mask_return);
typedef int (*tXQueryTextExtents)(
    Display* display,
    XID font_ID,
    _Xconst char* string,
    int nchars,
    int* direction_return,
    int* font_ascent_return,
    int* font_descent_return,
    XCharStruct* overall_return);
typedef int (*tXQueryTextExtents16)(
    Display* display,
    XID font_ID,
    _Xconst XChar2b* string,
    int nchars,
    int* direction_return,
    int* font_ascent_return,
    int* font_descent_return,
    XCharStruct* overall_return);
typedef Status (*tXQueryTree)(
    Display* display,
    Window w,
    Window* root_return,
    Window* parent_return,
    Window** children_return,
    unsigned int* nchildren_return);
typedef int (*tXRaiseWindow)(Display* display, Window w);
typedef int (*tXReadBitmapFile)(
    Display* display,
    Drawable d,
    _Xconst char* filename,
    unsigned int* width_return,
    unsigned int* height_return,
    Pixmap* bitmap_return,
    int* x_hot_return,
    int* y_hot_return);
typedef int (*tXReadBitmapFileData)(
    _Xconst char* filename,
    unsigned int* width_return,
    unsigned int* height_return,
    unsigned char** data_return,
    int* x_hot_return,
    int* y_hot_return);
typedef int (*tXRebindKeysym)(
    Display* display,
    KeySym keysym,
    KeySym* list,
    int mod_count,
    _Xconst unsigned char* string,
    int bytes_string);
typedef int (*tXRecolorCursor)(
    Display* display,
    Cursor cursor,
    XColor* foreground_color,
    XColor* background_color);
typedef int (*tXRefreshKeyboardMapping)(XMappingEvent* event_map);
typedef int (*tXRemoveFromSaveSet)(Display* display, Window w);
typedef int (*tXRemoveHost)(Display* display, XHostAddress* host);
typedef int (*tXRemoveHosts)(Display* display, XHostAddress* hosts, int num_hosts);
typedef int (*tXReparentWindow)(Display* display, Window w, Window parent, int x, int y);
typedef int (*tXResetScreenSaver)(Display* display);
typedef int (*tXResizeWindow)(
    Display* display,
    Window w,
    unsigned int width,
    unsigned int height);
typedef int (*tXRestackWindows)(Display* display, Window* windows, int nwindows);
typedef int (*tXRotateBuffers)(Display* display, int rotate);
typedef int (*tXRotateWindowProperties)(
    Display* display,
    Window w,
    Atom* properties,
    int num_prop,
    int npositions);
typedef int (*tXScreenCount)(Display* display);
typedef int (*tXSelectInput)(Display* display, Window w, long event_mask);
typedef Status (*tXSendEvent)(
    Display* display,
    Window w,
    Bool propagate,
    long event_mask,
    XEvent* event_send);
typedef int (*tXSetAccessControl)(Display* display, int mode);
typedef int (*tXSetArcMode)(Display* display, GC gc, int arc_mode);
typedef int (*tXSetBackground)(Display* display, GC gc, unsigned long background);
typedef int (*tXSetClipMask)(Display* display, GC gc, Pixmap pixmap);
typedef int (*tXSetClipOrigin)(
    Display* display,
    GC gc,
    int clip_x_origin,
    int clip_y_origin);
typedef int (*tXSetClipRectangles)(
    Display* display,
    GC gc,
    int clip_x_origin,
    int clip_y_origin,
    XRectangle* rectangles,
    int n,
    int ordering);
typedef int (*tXSetCloseDownMode)(Display* display, int close_mode);
typedef int (*tXSetCommand)(Display* display, Window w, char** argv, int argc);
typedef int (*tXSetDashes)(
    Display* display,
    GC gc,
    int dash_offset,
    _Xconst char* dash_list,
    int n);
typedef int (*tXSetFillRule)(Display* display, GC gc, int fill_rule);
typedef int (*tXSetFillStyle)(Display* display, GC gc, int fill_style);
typedef int (*tXSetFont)(Display* display, GC gc, Font font);
typedef int (*tXSetFontPath)(Display* display, char** directories, int ndirs);
typedef int (*tXSetForeground)(Display* display, GC gc, unsigned long foreground);
typedef int (*tXSetFunction)(Display* display, GC gc, int function);
typedef int (*tXSetGraphicsExposures)(
    Display* display,
    GC gc,
    Bool graphics_exposures);
typedef int (*tXSetIconName)(Display* display, Window w, _Xconst char* icon_name);
typedef int (*tXSetInputFocus)(Display* display, Window focus, int revert_to, Time time);
typedef int (*tXSetLineAttributes)(
    Display* display,
    GC gc,
    unsigned int line_width,
    int line_style,
    int cap_style,
    int join_style);
typedef int (*tXSetModifierMapping)(Display* display, XModifierKeymap* modmap);
typedef int (*tXSetPlaneMask)(Display* display, GC gc, unsigned long plane_mask);
typedef int (*tXSetPointerMapping)(
    Display* display,
    _Xconst unsigned char* map,
    int nmap);
typedef int (*tXSetScreenSaver)(
    Display* display,
    int timeout,
    int interval,
    int prefer_blanking,
    int allow_exposures);
typedef int (*tXSetSelectionOwner)(
    Display* display,
    Atom selection,
    Window owner,
    Time time);
typedef int (*tXSetState)(
    Display* display,
    GC gc,
    unsigned long foreground,
    unsigned long background,
    int function,
    unsigned long plane_mask);
typedef int (*tXSetStipple)(Display* display, GC gc, Pixmap stipple);
typedef int (*tXSetSubwindowMode)(Display* display, GC gc, int subwindow_mode);
typedef int (*tXSetTSOrigin)(Display* display, GC gc, int ts_x_origin, int ts_y_origin);
typedef int (*tXSetTile)(Display* display, GC gc, Pixmap tile);
typedef int (*tXSetWindowBackground)(
    Display* display,
    Window w,
    unsigned long background_pixel);
typedef int (*tXSetWindowBackgroundPixmap)(
    Display* display,
    Window w,
    Pixmap background_pixmap);
typedef int (*tXSetWindowBorder)(
    Display* display,
    Window w,
    unsigned long border_pixel);
typedef int (*tXSetWindowBorderPixmap)(
    Display* display,
    Window w,
    Pixmap border_pixmap);
typedef int (*tXSetWindowBorderWidth)(Display* display, Window w, unsigned int width);
typedef int (*tXSetWindowColormap)(Display* display, Window w, Colormap colormap);
typedef int (*tXStoreBuffer)(
    Display* display,
    _Xconst char* bytes,
    int nbytes,
    int buffer);
typedef int (*tXStoreBytes)(Display* display, _Xconst char* bytes, int nbytes);
typedef int (*tXStoreColor)(Display* display, Colormap colormap, XColor* color);
typedef int (*tXStoreColors)(
    Display* display,
    Colormap colormap,
    XColor* color,
    int ncolors);
typedef int (*tXStoreName)(Display* display, Window w, _Xconst char* window_name);
typedef int (*tXStoreNamedColor)(
    Display* display,
    Colormap colormap,
    _Xconst char* color,
    unsigned long pixel,
    int flags);
typedef int (*tXSync)(Display* display, Bool discard);
typedef int (*tXTextExtents)(
    XFontStruct* font_struct,
    _Xconst char* string,
    int nchars,
    int* direction_return,
    int* font_ascent_return,
    int* font_descent_return,
    XCharStruct* overall_return);
typedef int (*tXTextExtents16)(
    XFontStruct* font_struct,
    _Xconst XChar2b* string,
    int nchars,
    int* direction_return,
    int* font_ascent_return,
    int* font_descent_return,
    XCharStruct* overall_return);
typedef int (*tXTextWidth)(XFontStruct* font_struct, _Xconst char* string, int count);
typedef int (*tXTextWidth16)(
    XFontStruct* font_struct,
    _Xconst XChar2b* string,
    int count);
typedef Bool (*tXTranslateCoordinates)(
    Display* display,
    Window src_w,
    Window dest_w,
    int src_x,
    int src_y,
    int* dest_x_return,
    int* dest_y_return,
    Window* child_return);
typedef int (*tXUndefineCursor)(Display* display, Window w);
typedef int (*tXUngrabButton)(
    Display* display,
    unsigned int button,
    unsigned int modifiers,
    Window grab_window);
typedef int (*tXUngrabKey)(
    Display* display,
    int keycode,
    unsigned int modifiers,
    Window grab_window);
typedef int (*tXUngrabKeyboard)(Display* display, Time time);
typedef int (*tXUngrabPointer)(Display* display, Time time);
typedef int (*tXUngrabServer)(Display* display);
typedef int (*tXUninstallColormap)(Display* display, Colormap colormap);
typedef int (*tXUnloadFont)(Display* display, Font font);
typedef int (*tXUnmapSubwindows)(Display* display, Window w);
typedef int (*tXUnmapWindow)(Display* display, Window w);
typedef int (*tXVendorRelease)(Display* display);
typedef int (*tXWarpPointer)(
    Display* display,
    Window src_w,
    Window dest_w,
    int src_x,
    int src_y,
    unsigned int src_width,
    unsigned int src_height,
    int dest_x,
    int dest_y);
typedef int (*tXWidthMMOfScreen)(Screen* screen);
typedef int (*tXWidthOfScreen)(Screen* screen);
typedef int (*tXWindowEvent)(
    Display* display,
    Window w,
    long event_mask,
    XEvent* event_return);
typedef int (*tXWriteBitmapFile)(
    Display* display,
    _Xconst char* filename,
    Pixmap bitmap,
    unsigned int width,
    unsigned int height,
    int x_hot,
    int y_hot);
typedef Bool (*tXSupportsLocale)(void);
typedef char* (*tXSetLocaleModifiers)(const char* modifier_list);
typedef XOM (*tXOpenOM)(
    Display* display,
    struct _XrmHashBucketRec* rdb,
    _Xconst char* res_name,
    _Xconst char* res_class);
typedef Status (*tXCloseOM)(XOM om);
typedef char* (*tXSetOMValues)(XOM om, ...);
typedef char* (*tXGetOMValues)(XOM om, ...);
typedef Display* (*tXDisplayOfOM)(XOM om);
typedef char* (*tXLocaleOfOM)(XOM om);
typedef XOC (*tXCreateOC)(XOM om, ...);
typedef void (*tXDestroyOC)(XOC oc);
typedef XOM (*tXOMOfOC)(XOC oc);
typedef char* (*tXSetOCValues)(XOC oc, ...);
typedef char* (*tXGetOCValues)(XOC oc, ...);
typedef XFontSet (*tXCreateFontSet)(
    Display* display,
    _Xconst char* base_font_name_list,
    char*** missing_charset_list,
    int* missing_charset_count,
    char** def_string);
typedef void (*tXFreeFontSet)(Display* display, XFontSet font_set);
typedef int (*tXFontsOfFontSet)(
    XFontSet font_set,
    XFontStruct*** font_struct_list,
    char*** font_name_list);
typedef char* (*tXBaseFontNameListOfFontSet)(XFontSet font_set);
typedef char* (*tXLocaleOfFontSet)(XFontSet font_set);
typedef Bool (*tXContextDependentDrawing)(XFontSet font_set);
typedef Bool (*tXDirectionalDependentDrawing)(XFontSet font_set);
typedef Bool (*tXContextualDrawing)(XFontSet font_set);
typedef XFontSetExtents* (*tXExtentsOfFontSet)(XFontSet font_set);
typedef int (*tXmbTextEscapement)(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text);
typedef int (*tXwcTextEscapement)(
    XFontSet font_set,
    _Xconst wchar_t* text,
    int num_wchars);
typedef int (*tXutf8TextEscapement)(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text);
typedef int (*tXmbTextExtents)(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return);
typedef int (*tXwcTextExtents)(
    XFontSet font_set,
    _Xconst wchar_t* text,
    int num_wchars,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return);
typedef int (*tXutf8TextExtents)(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return);
typedef Status (*tXmbTextPerCharExtents)(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text,
    XRectangle* ink_extents_buffer,
    XRectangle* logical_extents_buffer,
    int buffer_size,
    int* num_chars,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return);
typedef Status (*tXwcTextPerCharExtents)(
    XFontSet font_set,
    _Xconst wchar_t* text,
    int num_wchars,
    XRectangle* ink_extents_buffer,
    XRectangle* logical_extents_buffer,
    int buffer_size,
    int* num_chars,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return);
typedef Status (*tXutf8TextPerCharExtents)(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text,
    XRectangle* ink_extents_buffer,
    XRectangle* logical_extents_buffer,
    int buffer_size,
    int* num_chars,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return);
typedef void (*tXmbDrawText)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    XmbTextItem* text_items,
    int nitems);
typedef void (*tXwcDrawText)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    XwcTextItem* text_items,
    int nitems);
typedef void (*tXutf8DrawText)(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    XmbTextItem* text_items,
    int nitems);
typedef void (*tXmbDrawString)(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst char* text,
    int bytes_text);
typedef void (*tXwcDrawString)(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst wchar_t* text,
    int num_wchars);
typedef void (*tXutf8DrawString)(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst char* text,
    int bytes_text);
typedef void (*tXmbDrawImageString)(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst char* text,
    int bytes_text);
typedef void (*tXwcDrawImageString)(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst wchar_t* text,
    int num_wchars);
typedef void (*tXutf8DrawImageString)(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst char* text,
    int bytes_text);
typedef XIM (*tXOpenIM)(
    Display* dpy,
    struct _XrmHashBucketRec* rdb,
    char* res_name,
    char* res_class);
typedef Status (*tXCloseIM)(XIM im);
typedef char* (*tXGetIMValues)(XIM im, ...);
typedef char* (*tXSetIMValues)(XIM im, ...);
typedef Display* (*tXDisplayOfIM)(XIM im);
typedef char* (*tXLocaleOfIM)(XIM im);
typedef XIC (*tXCreateIC)(XIM im, ...);
typedef void (*tXDestroyIC)(XIC ic);
typedef void (*tXSetICFocus)(XIC ic);
typedef void (*tXUnsetICFocus)(XIC ic);
typedef wchar_t* (*tXwcResetIC)(XIC ic);
typedef char* (*tXmbResetIC)(XIC ic);
typedef char* (*tXutf8ResetIC)(XIC ic);
typedef char* (*tXSetICValues)(XIC ic, ...);
typedef char* (*tXGetICValues)(XIC ic, ...);
typedef XIM (*tXIMOfIC)(XIC ic);
typedef Bool (*tXFilterEvent)(XEvent* event, Window window);
typedef int (*tXmbLookupString)(
    XIC ic,
    XKeyPressedEvent* event,
    char* buffer_return,
    int bytes_buffer,
    KeySym* keysym_return,
    Status* status_return);
typedef int (*tXwcLookupString)(
    XIC ic,
    XKeyPressedEvent* event,
    wchar_t* buffer_return,
    int wchars_buffer,
    KeySym* keysym_return,
    Status* status_return);
typedef int (*tXutf8LookupString)(
    XIC ic,
    XKeyPressedEvent* event,
    char* buffer_return,
    int bytes_buffer,
    KeySym* keysym_return,
    Status* status_return);
typedef XVaNestedList (*tXVaCreateNestedList)(int unused, ...);
typedef Bool (*tXRegisterIMInstantiateCallback)(
    Display* dpy,
    struct _XrmHashBucketRec* rdb,
    char* res_name,
    char* res_class,
    XIDProc callback,
    XPointer client_data);
typedef Bool (*tXUnregisterIMInstantiateCallback)(
    Display* dpy,
    struct _XrmHashBucketRec* rdb,
    char* res_name,
    char* res_class,
    XIDProc callback,
    XPointer client_data);
typedef Status (*tXInternalConnectionNumbers)(
    Display* dpy,
    int** fd_return,
    int* count_return);
typedef void (*tXProcessInternalConnection)(Display* dpy, int fd);
typedef Status (*tXAddConnectionWatch)(
    Display* dpy,
    XConnectionWatchProc callback,
    XPointer client_data);
typedef void (*tXRemoveConnectionWatch)(
    Display* dpy,
    XConnectionWatchProc callback,
    XPointer client_data);
typedef void (*tXSetAuthorization)(char* name, int namelen, char* data, int datalen);
typedef int (*t_Xmbtowc)(wchar_t* wstr, char* str, int len);
typedef int (*t_Xwctomb)(char* str, wchar_t wc);
typedef Bool (*tXGetEventData)(Display* dpy, XGenericEventCookie* cookie);
typedef void (*tXFreeEventData)(Display* dpy, XGenericEventCookie* cookie);

typedef xcb_connection_t* (*tXGetXCBConnection)(Display* dpy);
typedef void (*tXSetEventQueueOwner)(Display* dpy, enum XEventQueueOwner owner);

typedef void* (*t_XGetRequest)(Display* dpy, CARD8 type, size_t len);
typedef void (*t_XFlushGCCache)(Display* dpy, GC gc);
typedef int (*t_XData32)(Display* dpy, register _Xconst long* data, unsigned len);
typedef void (*t_XRead32)(Display* dpy, register long* data, long len);
typedef void (*t_XDeqAsyncHandler)(Display* dpy, _XAsyncHandler* handler);
typedef void (*tData)(Display* dpy, char* data, long len);
typedef int (*t_XError)(Display* dpy, xError* rep);
typedef int (*t_XIOError)(Display* dpy);
typedef void (*t_XEatData)(Display* dpy, unsigned long n);
typedef void (*t_XEatDataWords)(Display* dpy, unsigned long n);
typedef char* (*t_XAllocScratch)(Display* dpy, unsigned long nbytes);
typedef char* (*t_XAllocTemp)(Display* dpy, unsigned long nbytes);
typedef void (*t_XFreeTemp)(Display* dpy, char* buf, unsigned long nbytes);
typedef Visual* (*t_XVIDtoVisual)(Display* dpy, VisualID id);
typedef unsigned long (*t_XSetLastRequestRead)(Display* dpy, xGenericReply* rep);
typedef int (*t_XGetHostname)(char* buf, int maxlen);
typedef Screen* (*t_XScreenOfWindow)(Display* dpy, Window w);
typedef Bool (*t_XAsyncErrorHandler)(
    Display* dpy,
    xReply* rep,
    char* buf,
    int len,
    XPointer data);
typedef char* (*t_XGetAsyncReply)(
    Display* dpy,
    char* replbuf,
    xReply* rep,
    char* buf,
    int len,
    int extra,
    Bool discard);
typedef void (*t_XGetAsyncData)(
    Display* dpy,
    char* data,
    char* buf,
    int len,
    int skip,
    int datalen,
    int discardtotal);
typedef void (*t_XFlush)(Display* dpy);
typedef int (*t_XEventsQueued)(Display* dpy, int mode);
typedef void (*t_XReadEvents)(Display* dpy);
typedef int (*t_XRead)(Display* dpy, char* data, long size);
typedef void (*t_XReadPad)(Display* dpy, char* data, long size);
typedef void (*t_XSend)(Display* dpy, _Xconst char* data, long size);
typedef Status (*t_XReply)(Display* dpy, xReply* rep, int extra, Bool discard);
typedef void (*t_XEnq)(Display* dpy, xEvent* event);
typedef void (*t_XDeq)(Display* dpy, _XQEvent* prev, _XQEvent* qelt);
typedef Bool (*t_XUnknownWireEvent)(Display* dpy, XEvent* re, xEvent* event);
typedef Bool (*t_XUnknownWireEventCookie)(
    Display* dpy,
    XGenericEventCookie* re,
    xEvent* event);
typedef Bool (*t_XUnknownCopyEventCookie)(
    Display* dpy,
    XGenericEventCookie* in,
    XGenericEventCookie* out);
typedef Status (*t_XUnknownNativeEvent)(Display* dpy, XEvent* re, xEvent* event);
typedef Bool (*t_XWireToEvent)(Display* dpy, XEvent* re, xEvent* event);
typedef Bool (*t_XDefaultWireError)(Display* display, XErrorEvent* he, xError* we);
typedef Bool (*t_XPollfdCacheInit)(Display* dpy);
typedef void (*t_XPollfdCacheAdd)(Display* dpy, int fd);
typedef void (*t_XPollfdCacheDel)(Display* dpy, int fd);
typedef XID (*t_XAllocID)(Display* dpy);
typedef void (*t_XAllocIDs)(Display* dpy, XID* ids, int count);
typedef int (*t_XFreeExtData)(XExtData* extension);
typedef int (* (*tXESetCreateGC)(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, GC /* gc */, XExtCodes* /* codes */)))(Display*, GC, XExtCodes*);
typedef int (* (*tXESetCopyGC)(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, GC /* gc */, XExtCodes* /* codes */)))(Display*, GC, XExtCodes*);
typedef int (* (*tXESetFlushGC)(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, GC /* gc */, XExtCodes* /* codes */)))(Display*, GC, XExtCodes*);
typedef int (* (*tXESetFreeGC)(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, GC /* gc */, XExtCodes* /* codes */)))(Display*, GC, XExtCodes*);
typedef int (* (*tXESetCreateFont)(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, XFontStruct* /* fs */, XExtCodes* /* codes */)))(Display*, XFontStruct*, XExtCodes*);
typedef int (* (*tXESetFreeFont)(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, XFontStruct* /* fs */, XExtCodes* /* codes */)))(Display*, XFontStruct*, XExtCodes*);
typedef int (* (*tXESetCloseDisplay)(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, XExtCodes* /* codes */)))(Display*, XExtCodes*);
typedef int (* (*tXESetError)(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, xError* /* err */, XExtCodes* /* codes */, int* /* ret_code */)))(Display*, xError*, XExtCodes*, int*);
typedef char* (* (*tXESetErrorString)(
    Display* display,
    int extension,
    char* (*proc)(Display* /* display */, int /* code */, XExtCodes* /* codes */, char* /* buffer */, int /* nbytes */)))(Display*, int, XExtCodes*, char*, int);
typedef void (* (*tXESetPrintErrorValues)(
    Display* display,
    int extension,
    void (*proc)(Display* /* display */, XErrorEvent* /* ev */, void* /* fp */)))(Display*, XErrorEvent*, void*);
typedef Bool (* (*tXESetWireToEvent)(
    Display* display,
    int event_number,
    Bool (*proc)(Display* /* display */, XEvent* /* re */, xEvent* /* event */)))(Display*, XEvent*, xEvent*);
typedef Bool (* (*tXESetWireToEventCookie)(
    Display* display,
    int extension,
    Bool (*proc)(Display* /* display */, XGenericEventCookie* /* re */, xEvent* /* event */)))(Display*, XGenericEventCookie*, xEvent*);
typedef Bool (* (*tXESetCopyEventCookie)(
    Display* display,
    int extension,
    Bool (*proc)(Display* /* display */, XGenericEventCookie* /* in */, XGenericEventCookie* /* out */)))(Display*, XGenericEventCookie*, XGenericEventCookie*);
typedef Status (* (*tXESetEventToWire)(
    Display* display,
    int event_number,
    Status (*proc)(Display* /* display */, XEvent* /* re */, xEvent* /* event */)))(Display*, XEvent*, xEvent*);
typedef Bool (* (*tXESetWireToError)(
    Display* display,
    int error_number,
    Bool (*proc)(Display* /* display */, XErrorEvent* /* he */, xError* /* we */)))(Display*, XErrorEvent*, xError*);
typedef void (* (*tXESetBeforeFlush)(
    Display* display,
    int error_number,
    void (*proc)(Display* /* display */, XExtCodes* /* codes */, _Xconst char* /* data */, long /* len */)))(Display*, XExtCodes*, _Xconst char*, long);
typedef Status (*t_XRegisterInternalConnection)(
    Display* dpy,
    int fd,
    _XInternalConnectionProc callback,
    XPointer call_data);
typedef void (*t_XUnregisterInternalConnection)(Display* dpy, int fd);
typedef void (*t_XProcessInternalConnection)(
    Display* dpy,
    struct _XConnectionInfo* conn_info);
typedef char* (*t__XOS2RedirRoot)(char* arg);
typedef int (*t_XTextHeight)(
    XFontStruct* font_struct,
    _Xconst char* string,
    int count);
typedef int (*t_XTextHeight16)(
    XFontStruct* font_struct,
    _Xconst XChar2b* string,
    int count);
typedef int (*t_XOpenFile)(_Xconst char* path, int flags);
typedef int (*t_XOpenFileMode)(_Xconst char* path, int flags, mode_t mode);
typedef void* (*t_XFopenFile)(_Xconst char* path, _Xconst char* mode);
typedef int (*t_XAccessFile)(_Xconst char* path);
typedef Status (*t_XEventToWire)(Display* dpy, XEvent* re, xEvent* event);
typedef int (*t_XF86LoadQueryLocaleFont)(
    Display* dpy,
    _Xconst char* name,
    XFontStruct** xfp,
    Font* fidp);
typedef void (*t_XProcessWindowAttributes)(
    register Display* dpy,
    xChangeWindowAttributesReq* req,
    register unsigned long valuemask,
    register XSetWindowAttributes* attributes);
typedef int (*t_XDefaultError)(Display* dpy, XErrorEvent* event);
typedef int (*t_XDefaultIOError)(Display* dpy);
typedef void (*t_XSetClipRectangles)(
    register Display* dpy,
    GC gc,
    int clip_x_origin,
    int clip_y_origin,
    XRectangle* rectangles,
    int n,
    int ordering);
typedef Status (*t_XGetWindowAttributes)(
    register Display* dpy,
    Window w,
    XWindowAttributes* attr);
typedef int (*t_XPutBackEvent)(register Display* dpy, register XEvent* event);
typedef Bool (*t_XIsEventCookie)(Display* dpy, XEvent* ev);
typedef void (*t_XFreeEventCookies)(Display* dpy);
typedef void (*t_XStoreEventCookie)(Display* dpy, XEvent* ev);
typedef Bool (*t_XFetchEventCookie)(Display* dpy, XGenericEventCookie* ev);
typedef Bool (*t_XCopyEventCookie)(
    Display* dpy,
    XGenericEventCookie* in,
    XGenericEventCookie* out);
typedef void (*txlocaledir)(char* buf, int buf_len);

typedef Bool (*tXkbIgnoreExtension)(Bool ignore);
typedef Display* (*tXkbOpenDisplay)(
    char* name,
    int* ev_rtrn,
    int* err_rtrn,
    int* major_rtrn,
    int* minor_rtrn,
    int* reason);
typedef Bool (*tXkbQueryExtension)(
    Display* dpy,
    int* opcodeReturn,
    int* eventBaseReturn,
    int* errorBaseReturn,
    int* majorRtrn,
    int* minorRtrn);
typedef Bool (*tXkbUseExtension)(Display* dpy, int* major_rtrn, int* minor_rtrn);
typedef Bool (*tXkbLibraryVersion)(int* libMajorRtrn, int* libMinorRtrn);
typedef unsigned int (*tXkbSetXlibControls)(
    Display* dpy,
    unsigned int affect,
    unsigned int values);
typedef unsigned int (*tXkbGetXlibControls)(Display* dpy);
typedef unsigned int (*tXkbXlibControlsImplemented)(void);
typedef void (*tXkbSetAtomFuncs)(
    XkbInternAtomFunc getAtom,
    XkbGetAtomNameFunc getName);
typedef KeySym (*tXkbKeycodeToKeysym)(
    Display* dpy,
#if NeedWidePrototypes
    unsigned int kc,
#else
    KeyCode kc,
#endif
    int group,
    int level);
typedef unsigned int (*tXkbKeysymToModifiers)(Display* dpy, KeySym ks);
typedef Bool (*tXkbLookupKeySym)(
    Display* dpy,
#if NeedWidePrototypes
    unsigned int keycode,
#else
    KeyCode keycode,
#endif
    unsigned int modifiers,
    unsigned int* modifiers_return,
    KeySym* keysym_return);
typedef int (*tXkbLookupKeyBinding)(
    Display* dpy,
    KeySym sym_rtrn,
    unsigned int mods,
    char* buffer,
    int nbytes,
    int* extra_rtrn);
typedef Bool (*tXkbTranslateKeyCode)(
    XkbDescPtr xkb,
#if NeedWidePrototypes
    unsigned int keycode,
#else
    KeyCode keycode,
#endif
    unsigned int modifiers,
    unsigned int* modifiers_return,
    KeySym* keysym_return);
typedef int (*tXkbTranslateKeySym)(
    Display* dpy,
    register KeySym* sym_return,
    unsigned int modifiers,
    char* buffer,
    int nbytes,
    int* extra_rtrn);
typedef Bool (*tXkbSetAutoRepeatRate)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int delay,
    unsigned int interval);
typedef Bool (*tXkbGetAutoRepeatRate)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int* delayRtrn,
    unsigned int* intervalRtrn);
typedef Bool (*tXkbChangeEnabledControls)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int affect,
    unsigned int values);
typedef Bool (*tXkbDeviceBell)(
    Display* dpy,
    Window win,
    int deviceSpec,
    int bellClass,
    int bellID,
    int percent,
    Atom name);
typedef Bool (*tXkbForceDeviceBell)(
    Display* dpy,
    int deviceSpec,
    int bellClass,
    int bellID,
    int percent);
typedef Bool (*tXkbDeviceBellEvent)(
    Display* dpy,
    Window win,
    int deviceSpec,
    int bellClass,
    int bellID,
    int percent,
    Atom name);
typedef Bool (*tXkbBell)(Display* dpy, Window win, int percent, Atom name);
typedef Bool (*tXkbForceBell)(Display* dpy, int percent);
typedef Bool (*tXkbBellEvent)(Display* dpy, Window win, int percent, Atom name);
typedef Bool (*tXkbSelectEvents)(
    Display* dpy,
    unsigned int deviceID,
    unsigned int affect,
    unsigned int values);
typedef Bool (*tXkbSelectEventDetails)(
    Display* dpy,
    unsigned int deviceID,
    unsigned int eventType,
    unsigned long affect,
    unsigned long details);
typedef void (*tXkbNoteMapChanges)(
    XkbMapChangesPtr old,
    XkbMapNotifyEvent* new_,
    unsigned int wanted);
typedef void (*tXkbNoteNameChanges)(
    XkbNameChangesPtr old,
    XkbNamesNotifyEvent* new_,
    unsigned int wanted);
typedef Status (*tXkbGetIndicatorState)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int* pStateRtrn);
typedef Status (*tXkbGetDeviceIndicatorState)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int ledClass,
    unsigned int ledID,
    unsigned int* pStateRtrn);
typedef Status (*tXkbGetIndicatorMap)(
    Display* dpy,
    unsigned long which,
    XkbDescPtr desc);
typedef Bool (*tXkbSetIndicatorMap)(
    Display* dpy,
    unsigned long which,
    XkbDescPtr desc);
typedef Bool (*tXkbGetNamedIndicator)(
    Display* dpy,
    Atom name,
    int* pNdxRtrn,
    Bool* pStateRtrn,
    XkbIndicatorMapPtr pMapRtrn,
    Bool* pRealRtrn);
typedef Bool (*tXkbGetNamedDeviceIndicator)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int ledClass,
    unsigned int ledID,
    Atom name,
    int* pNdxRtrn,
    Bool* pStateRtrn,
    XkbIndicatorMapPtr pMapRtrn,
    Bool* pRealRtrn);
typedef Bool (*tXkbSetNamedIndicator)(
    Display* dpy,
    Atom name,
    Bool changeState,
    Bool state,
    Bool createNewMap,
    XkbIndicatorMapPtr pMap);
typedef Bool (*tXkbSetNamedDeviceIndicator)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int ledClass,
    unsigned int ledID,
    Atom name,
    Bool changeState,
    Bool state,
    Bool createNewMap,
    XkbIndicatorMapPtr pMap);
typedef Bool (*tXkbLockModifiers)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int affect,
    unsigned int values);
typedef Bool (*tXkbLatchModifiers)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int affect,
    unsigned int values);
typedef Bool (*tXkbLockGroup)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int group);
typedef Bool (*tXkbLatchGroup)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int group);
typedef Bool (*tXkbSetServerInternalMods)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int affectReal,
    unsigned int realValues,
    unsigned int affectVirtual,
    unsigned int virtualValues);
typedef Bool (*tXkbSetIgnoreLockMods)(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int affectReal,
    unsigned int realValues,
    unsigned int affectVirtual,
    unsigned int virtualValues);
typedef Bool (*tXkbVirtualModsToReal)(
    XkbDescPtr xkb,
    unsigned int virtual_mask,
    unsigned int* mask_rtrn);
typedef Bool (*tXkbComputeEffectiveMap)(
    XkbDescPtr xkb,
    XkbKeyTypePtr type,
    unsigned char* map_rtrn);
typedef Status (*tXkbInitCanonicalKeyTypes)(
    XkbDescPtr xkb,
    unsigned int which,
    int keypadVMod);
typedef XkbDescPtr (*tXkbAllocKeyboard)(void);
typedef void (*tXkbFreeKeyboard)(XkbDescPtr xkb, unsigned int which, Bool freeDesc);
typedef Status (*tXkbAllocClientMap)(
    XkbDescPtr xkb,
    unsigned int which,
    unsigned int nTypes);
typedef Status (*tXkbAllocServerMap)(
    XkbDescPtr xkb,
    unsigned int which,
    unsigned int nActions);
typedef void (*tXkbFreeClientMap)(XkbDescPtr xkb, unsigned int what, Bool freeMap);
typedef void (*tXkbFreeServerMap)(XkbDescPtr xkb, unsigned int what, Bool freeMap);
typedef XkbKeyTypePtr (*tXkbAddKeyType)(
    XkbDescPtr xkb,
    Atom name,
    int map_count,
    Bool want_preserve,
    int num_lvls);
typedef Status (*tXkbAllocIndicatorMaps)(XkbDescPtr xkb);
typedef void (*tXkbFreeIndicatorMaps)(XkbDescPtr xkb);
typedef XkbDescPtr (*tXkbGetMap)(
    Display* dpy,
    unsigned int which,
    unsigned int deviceSpec);
typedef Status (*tXkbGetUpdatedMap)(
    Display* dpy,
    unsigned int which,
    XkbDescPtr desc);
typedef Status (*tXkbGetMapChanges)(
    Display* dpy,
    XkbDescPtr xkb,
    XkbMapChangesPtr changes);
typedef Status (*tXkbRefreshKeyboardMapping)(XkbMapNotifyEvent* event);
typedef Status (*tXkbGetKeyTypes)(
    Display* dpy,
    unsigned int first,
    unsigned int num,
    XkbDescPtr xkb);
typedef Status (*tXkbGetKeySyms)(
    Display* dpy,
    unsigned int first,
    unsigned int num,
    XkbDescPtr xkb);
typedef Status (*tXkbGetKeyActions)(
    Display* dpy,
    unsigned int first,
    unsigned int num,
    XkbDescPtr xkb);
typedef Status (*tXkbGetKeyBehaviors)(
    Display* dpy,
    unsigned int firstKey,
    unsigned int nKeys,
    XkbDescPtr desc);
typedef Status (*tXkbGetVirtualMods)(
    Display* dpy,
    unsigned int which,
    XkbDescPtr desc);
typedef Status (*tXkbGetKeyExplicitComponents)(
    Display* dpy,
    unsigned int firstKey,
    unsigned int nKeys,
    XkbDescPtr desc);
typedef Status (*tXkbGetKeyModifierMap)(
    Display* dpy,
    unsigned int firstKey,
    unsigned int nKeys,
    XkbDescPtr desc);
typedef Status (*tXkbGetKeyVirtualModMap)(
    Display* dpy,
    unsigned int first,
    unsigned int num,
    XkbDescPtr xkb);
typedef Status (*tXkbAllocControls)(XkbDescPtr xkb, unsigned int which);
typedef void (*tXkbFreeControls)(XkbDescPtr xkb, unsigned int which, Bool freeMap);
typedef Status (*tXkbGetControls)(Display* dpy, unsigned long which, XkbDescPtr desc);
typedef Bool (*tXkbSetControls)(Display* dpy, unsigned long which, XkbDescPtr desc);
typedef void (*tXkbNoteControlsChanges)(
    XkbControlsChangesPtr old,
    XkbControlsNotifyEvent* new_,
    unsigned int wanted);
typedef Status (*tXkbAllocCompatMap)(
    XkbDescPtr xkb,
    unsigned int which,
    unsigned int nInterpret);
typedef void (*tXkbFreeCompatMap)(XkbDescPtr xkb, unsigned int which, Bool freeMap);
typedef Status (*tXkbGetCompatMap)(Display* dpy, unsigned int which, XkbDescPtr xkb);
typedef Bool (*tXkbSetCompatMap)(
    Display* dpy,
    unsigned int which,
    XkbDescPtr xkb,
    Bool updateActions);
typedef XkbSymInterpretPtr (*tXkbAddSymInterpret)(
    XkbDescPtr xkb,
    XkbSymInterpretPtr si,
    Bool updateMap,
    XkbChangesPtr changes);
typedef Status (*tXkbAllocNames)(
    XkbDescPtr xkb,
    unsigned int which,
    int nTotalRG,
    int nTotalAliases);
typedef Status (*tXkbGetNames)(Display* dpy, unsigned int which, XkbDescPtr desc);
typedef Bool (*tXkbSetNames)(
    Display* dpy,
    unsigned int which,
    unsigned int firstType,
    unsigned int nTypes,
    XkbDescPtr desc);
typedef Bool (*tXkbChangeNames)(
    Display* dpy,
    XkbDescPtr xkb,
    XkbNameChangesPtr changes);
typedef void (*tXkbFreeNames)(XkbDescPtr xkb, unsigned int which, Bool freeMap);
typedef Status (*tXkbGetState)(
    Display* dpy,
    unsigned int deviceSpec,
    XkbStatePtr rtrnState);
typedef Bool (*tXkbSetMap)(Display* dpy, unsigned int which, XkbDescPtr desc);
typedef Bool (*tXkbChangeMap)(
    Display* dpy,
    XkbDescPtr desc,
    XkbMapChangesPtr changes);
typedef Bool (*tXkbSetDetectableAutoRepeat)(
    Display* dpy,
    Bool detectable,
    Bool* supported);
typedef Bool (*tXkbGetDetectableAutoRepeat)(Display* dpy, Bool* supported);
typedef Bool (*tXkbSetAutoResetControls)(
    Display* dpy,
    unsigned int changes,
    unsigned int* auto_ctrls,
    unsigned int* auto_values);
typedef Bool (*tXkbGetAutoResetControls)(
    Display* dpy,
    unsigned int* auto_ctrls,
    unsigned int* auto_ctrl_values);
typedef Bool (*tXkbSetPerClientControls)(
    Display* dpy,
    unsigned int change,
    unsigned int* values);
typedef Bool (*tXkbGetPerClientControls)(Display* dpy, unsigned int* ctrls);
typedef Status (*tXkbCopyKeyType)(XkbKeyTypePtr from, XkbKeyTypePtr into);
typedef Status (*tXkbCopyKeyTypes)(
    XkbKeyTypePtr from,
    XkbKeyTypePtr into,
    int num_types);
typedef Status (*tXkbResizeKeyType)(
    XkbDescPtr xkb,
    int type_ndx,
    int map_count,
    Bool want_preserve,
    int new_num_lvls);
typedef KeySym* (*tXkbResizeKeySyms)(XkbDescPtr desc, int forKey, int symsNeeded);
typedef XkbAction* (*tXkbResizeKeyActions)(
    XkbDescPtr desc,
    int forKey,
    int actsNeeded);
typedef Status (*tXkbChangeTypesOfKey)(
    XkbDescPtr xkb,
    int key,
    int num_groups,
    unsigned int groups,
    int* newTypes,
    XkbMapChangesPtr pChanges);
typedef Status (*tXkbChangeKeycodeRange)(
    XkbDescPtr xkb,
    int minKC,
    int maxKC,
    XkbChangesPtr changes);
typedef XkbComponentListPtr (*tXkbListComponents)(
    Display* dpy,
    unsigned int deviceSpec,
    XkbComponentNamesPtr ptrns,
    int* max_inout);
typedef void (*tXkbFreeComponentList)(XkbComponentListPtr list);
typedef XkbDescPtr (*tXkbGetKeyboard)(
    Display* dpy,
    unsigned int which,
    unsigned int deviceSpec);
typedef XkbDescPtr (*tXkbGetKeyboardByName)(
    Display* dpy,
    unsigned int deviceSpec,
    XkbComponentNamesPtr names,
    unsigned int want,
    unsigned int need,
    Bool load);
typedef int (*tXkbKeyTypesForCoreSymbols)(
    XkbDescPtr xkb,
    int map_width,
    KeySym* core_syms,
    unsigned int protected_,
    int* types_inout,
    KeySym* xkb_syms_rtrn);
typedef Bool (*tXkbApplyCompatMapToKey)(
    XkbDescPtr xkb,
#if NeedWidePrototypes
    unsigned int key,
#else
    KeyCode key,
#endif
    XkbChangesPtr changes);
typedef Bool (*tXkbUpdateMapFromCore)(
    XkbDescPtr xkb,
#if NeedWidePrototypes
    unsigned int first_key,
#else
    KeyCode first_key,
#endif
    int num_keys,
    int map_width,
    KeySym* core_keysyms,
    XkbChangesPtr changes);
typedef XkbDeviceLedInfoPtr (*tXkbAddDeviceLedInfo)(
    XkbDeviceInfoPtr devi,
    unsigned int ledClass,
    unsigned int ledId);
typedef Status (*tXkbResizeDeviceButtonActions)(
    XkbDeviceInfoPtr devi,
    unsigned int newTotal);
typedef XkbDeviceInfoPtr (*tXkbAllocDeviceInfo)(
    unsigned int deviceSpec,
    unsigned int nButtons,
    unsigned int szLeds);
typedef void (*tXkbFreeDeviceInfo)(
    XkbDeviceInfoPtr devi,
    unsigned int which,
    Bool freeDevI);
typedef void (*tXkbNoteDeviceChanges)(
    XkbDeviceChangesPtr old,
    XkbExtensionDeviceNotifyEvent* new_,
    unsigned int wanted);
typedef XkbDeviceInfoPtr (*tXkbGetDeviceInfo)(
    Display* dpy,
    unsigned int which,
    unsigned int deviceSpec,
    unsigned int ledClass,
    unsigned int ledID);
typedef Status (*tXkbGetDeviceInfoChanges)(
    Display* dpy,
    XkbDeviceInfoPtr devi,
    XkbDeviceChangesPtr changes);
typedef Status (*tXkbGetDeviceButtonActions)(
    Display* dpy,
    XkbDeviceInfoPtr devi,
    Bool all,
    unsigned int first,
    unsigned int nBtns);
typedef Status (*tXkbGetDeviceLedInfo)(
    Display* dpy,
    XkbDeviceInfoPtr devi,
    unsigned int ledClass,
    unsigned int ledId,
    unsigned int which);
typedef Bool (*tXkbSetDeviceInfo)(
    Display* dpy,
    unsigned int which,
    XkbDeviceInfoPtr devi);
typedef Bool (*tXkbChangeDeviceInfo)(
    Display* dpy,
    XkbDeviceInfoPtr desc,
    XkbDeviceChangesPtr changes);
typedef Bool (*tXkbSetDeviceLedInfo)(
    Display* dpy,
    XkbDeviceInfoPtr devi,
    unsigned int ledClass,
    unsigned int ledID,
    unsigned int which);
typedef Bool (*tXkbSetDeviceButtonActions)(
    Display* dpy,
    XkbDeviceInfoPtr devi,
    unsigned int first,
    unsigned int nBtns);
typedef char (*tXkbToControl)(char c);
typedef Bool (*tXkbSetDebuggingFlags)(
    Display* dpy,
    unsigned int mask,
    unsigned int flags,
    char* msg,
    unsigned int ctrls_mask,
    unsigned int ctrls,
    unsigned int* rtrn_flags,
    unsigned int* rtrn_ctrls);
typedef Bool (*tXkbApplyVirtualModChanges)(
    XkbDescPtr xkb,
    unsigned int changed,
    XkbChangesPtr changes);
typedef Bool (*tXkbUpdateActionVirtualMods)(
    XkbDescPtr xkb,
    XkbAction* act,
    unsigned int changed);
typedef void (*tXkbUpdateKeyTypeVirtualMods)(
    XkbDescPtr xkb,
    XkbKeyTypePtr type,
    unsigned int changed,
    XkbChangesPtr changes);

typedef int (*tXDestroyImage)(XImage* ximage);
typedef unsigned long (*tXGetPixel)(XImage* ximage, int x, int y);
typedef int (*tXPutPixel)(XImage* ximage, int x, int y, unsigned long pixel);
typedef XImage* (*tXSubImage)(
    XImage* ximage,
    int x,
    int y,
    unsigned int width,
    unsigned int height);
typedef int (*tXAddPixel)(XImage* ximage, long value);
typedef XClassHint* (*tXAllocClassHint)(void);
typedef XIconSize* (*tXAllocIconSize)(void);
typedef XSizeHints* (*tXAllocSizeHints)(void);
typedef XStandardColormap* (*tXAllocStandardColormap)(void);
typedef XWMHints* (*tXAllocWMHints)(void);
typedef int (*tXClipBox)(Region r, XRectangle* rect_return);
typedef Region (*tXCreateRegion)(void);
typedef const char* (*tXDefaultString)(void);
typedef int (*tXDeleteContext)(Display* display, XID rid, XContext context);
typedef int (*tXDestroyRegion)(Region r);
typedef int (*tXEmptyRegion)(Region r);
typedef int (*tXEqualRegion)(Region r1, Region r2);
typedef int (*tXFindContext)(
    Display* display,
    XID rid,
    XContext context,
    XPointer* data_return);
typedef Status (*tXGetClassHint)(
    Display* display,
    Window w,
    XClassHint* class_hints_return);
typedef Status (*tXGetIconSizes)(
    Display* display,
    Window w,
    XIconSize** size_list_return,
    int* count_return);
typedef Status (*tXGetNormalHints)(
    Display* display,
    Window w,
    XSizeHints* hints_return);
typedef Status (*tXGetRGBColormaps)(
    Display* display,
    Window w,
    XStandardColormap** stdcmap_return,
    int* count_return,
    Atom property);
typedef Status (*tXGetSizeHints)(
    Display* display,
    Window w,
    XSizeHints* hints_return,
    Atom property);
typedef Status (*tXGetStandardColormap)(
    Display* display,
    Window w,
    XStandardColormap* colormap_return,
    Atom property);
typedef Status (*tXGetTextProperty)(
    Display* display,
    Window window,
    XTextProperty* text_prop_return,
    Atom property);
typedef XVisualInfo* (*tXGetVisualInfo)(
    Display* display,
    long vinfo_mask,
    XVisualInfo* vinfo_template,
    int* nitems_return);
typedef Status (*tXGetWMClientMachine)(
    Display* display,
    Window w,
    XTextProperty* text_prop_return);
typedef XWMHints* (*tXGetWMHints)(Display* display, Window w);
typedef Status (*tXGetWMIconName)(
    Display* display,
    Window w,
    XTextProperty* text_prop_return);
typedef Status (*tXGetWMName)(
    Display* display,
    Window w,
    XTextProperty* text_prop_return);
typedef Status (*tXGetWMNormalHints)(
    Display* display,
    Window w,
    XSizeHints* hints_return,
    long* supplied_return);
typedef Status (*tXGetWMSizeHints)(
    Display* display,
    Window w,
    XSizeHints* hints_return,
    long* supplied_return,
    Atom property);
typedef Status (*tXGetZoomHints)(
    Display* display,
    Window w,
    XSizeHints* zhints_return);
typedef int (*tXIntersectRegion)(Region sra, Region srb, Region dr_return);
typedef void (*tXConvertCase)(KeySym sym, KeySym* lower, KeySym* upper);
typedef int (*tXLookupString)(
    XKeyEvent* event_struct,
    char* buffer_return,
    int bytes_buffer,
    KeySym* keysym_return,
    XComposeStatus* status_in_out);
typedef Status (*tXMatchVisualInfo)(
    Display* display,
    int screen,
    int depth,
    int class_,
    XVisualInfo* vinfo_return);
typedef int (*tXOffsetRegion)(Region r, int dx, int dy);
typedef Bool (*tXPointInRegion)(Region r, int x, int y);
typedef Region (*tXPolygonRegion)(XPoint* points, int n, int fill_rule);
typedef int (*tXRectInRegion)(
    Region r,
    int x,
    int y,
    unsigned int width,
    unsigned int height);
typedef int (*tXSaveContext)(
    Display* display,
    XID rid,
    XContext context,
    _Xconst char* data);
typedef int (*tXSetClassHint)(Display* display, Window w, XClassHint* class_hints);
typedef int (*tXSetIconSizes)(
    Display* display,
    Window w,
    XIconSize* size_list,
    int count);
typedef int (*tXSetNormalHints)(Display* display, Window w, XSizeHints* hints);
typedef void (*tXSetRGBColormaps)(
    Display* display,
    Window w,
    XStandardColormap* stdcmaps,
    int count,
    Atom property);
typedef int (*tXSetSizeHints)(
    Display* display,
    Window w,
    XSizeHints* hints,
    Atom property);
typedef int (*tXSetStandardProperties)(
    Display* display,
    Window w,
    _Xconst char* window_name,
    _Xconst char* icon_name,
    Pixmap icon_pixmap,
    char** argv,
    int argc,
    XSizeHints* hints);
typedef void (*tXSetTextProperty)(
    Display* display,
    Window w,
    XTextProperty* text_prop,
    Atom property);
typedef void (*tXSetWMClientMachine)(
    Display* display,
    Window w,
    XTextProperty* text_prop);
typedef int (*tXSetWMHints)(Display* display, Window w, XWMHints* wm_hints);
typedef void (*tXSetWMIconName)(Display* display, Window w, XTextProperty* text_prop);
typedef void (*tXSetWMName)(Display* display, Window w, XTextProperty* text_prop);
typedef void (*tXSetWMNormalHints)(Display* display, Window w, XSizeHints* hints);
typedef void (*tXSetWMProperties)(
    Display* display,
    Window w,
    XTextProperty* window_name,
    XTextProperty* icon_name,
    char** argv,
    int argc,
    XSizeHints* normal_hints,
    XWMHints* wm_hints,
    XClassHint* class_hints);
typedef void (*tXmbSetWMProperties)(
    Display* display,
    Window w,
    _Xconst char* window_name,
    _Xconst char* icon_name,
    char** argv,
    int argc,
    XSizeHints* normal_hints,
    XWMHints* wm_hints,
    XClassHint* class_hints);
typedef void (*tXutf8SetWMProperties)(
    Display* display,
    Window w,
    _Xconst char* window_name,
    _Xconst char* icon_name,
    char** argv,
    int argc,
    XSizeHints* normal_hints,
    XWMHints* wm_hints,
    XClassHint* class_hints);
typedef void (*tXSetWMSizeHints)(
    Display* display,
    Window w,
    XSizeHints* hints,
    Atom property);
typedef int (*tXSetRegion)(Display* display, GC gc, Region r);
typedef void (*tXSetStandardColormap)(
    Display* display,
    Window w,
    XStandardColormap* colormap,
    Atom property);
typedef int (*tXSetZoomHints)(Display* display, Window w, XSizeHints* zhints);
typedef int (*tXShrinkRegion)(Region r, int dx, int dy);
typedef Status (*tXStringListToTextProperty)(
    char** list,
    int count,
    XTextProperty* text_prop_return);
typedef int (*tXSubtractRegion)(Region sra, Region srb, Region dr_return);
typedef int (*tXmbTextListToTextProperty)(
    Display* display,
    char** list,
    int count,
    XICCEncodingStyle style,
    XTextProperty* text_prop_return);
typedef int (*tXwcTextListToTextProperty)(
    Display* display,
    wchar_t** list,
    int count,
    XICCEncodingStyle style,
    XTextProperty* text_prop_return);
typedef int (*tXutf8TextListToTextProperty)(
    Display* display,
    char** list,
    int count,
    XICCEncodingStyle style,
    XTextProperty* text_prop_return);
typedef void (*tXwcFreeStringList)(wchar_t** list);
typedef Status (*tXTextPropertyToStringList)(
    XTextProperty* text_prop,
    char*** list_return,
    int* count_return);
typedef int (*tXmbTextPropertyToTextList)(
    Display* display,
    const XTextProperty* text_prop,
    char*** list_return,
    int* count_return);
typedef int (*tXwcTextPropertyToTextList)(
    Display* display,
    const XTextProperty* text_prop,
    wchar_t*** list_return,
    int* count_return);
typedef int (*tXutf8TextPropertyToTextList)(
    Display* display,
    const XTextProperty* text_prop,
    char*** list_return,
    int* count_return);
typedef int (*tXUnionRectWithRegion)(
    XRectangle* rectangle,
    Region src_region,
    Region dest_region_return);
typedef int (*tXUnionRegion)(Region sra, Region srb, Region dr_return);
typedef int (*tXWMGeometry)(
    Display* display,
    int screen_number,
    _Xconst char* user_geometry,
    _Xconst char* default_geometry,
    unsigned int border_width,
    XSizeHints* hints,
    int* x_return,
    int* y_return,
    int* width_return,
    int* height_return,
    int* gravity_return);
typedef int (*tXXorRegion)(Region sra, Region srb, Region dr_return);

typedef Bool (*tXineramaQueryExtension)(
    Display* dpy,
    int* event_base,
    int* error_base);
typedef Status (*tXineramaQueryVersion)(
    Display* dpy,
    int* major_versionp,
    int* minor_versionp);
typedef Bool (*tXineramaIsActive)(Display* dpy);
typedef XineramaScreenInfo* (*tXineramaQueryScreens)(Display* dpy, int* number);

////////////////////////////////////////////////////////////////////////////////
// Functors declarations.

// Xlib.h
extern t_Xmblen _Xmblen_impl;
extern tXLoadQueryFont XLoadQueryFont_impl;
extern tXQueryFont XQueryFont_impl;
extern tXGetMotionEvents XGetMotionEvents_impl;
extern tXDeleteModifiermapEntry XDeleteModifiermapEntry_impl;
extern tXGetModifierMapping XGetModifierMapping_impl;
extern tXInsertModifiermapEntry XInsertModifiermapEntry_impl;
extern tXNewModifiermap XNewModifiermap_impl;
extern tXCreateImage XCreateImage_impl;
extern tXInitImage XInitImage_impl;
extern tXGetImage XGetImage_impl;
extern tXGetSubImage XGetSubImage_impl;
extern tXOpenDisplay XOpenDisplay_impl;
extern tXrmInitialize XrmInitialize_impl;
extern tXFetchBytes XFetchBytes_impl;
extern tXFetchBuffer XFetchBuffer_impl;
extern tXGetAtomName XGetAtomName_impl;
extern tXGetAtomNames XGetAtomNames_impl;
extern tXGetDefault XGetDefault_impl;
extern tXDisplayName XDisplayName_impl;
extern tXKeysymToString XKeysymToString_impl;
extern tXSynchronize XSynchronize_impl;
extern tXInternAtom XInternAtom_impl;
extern tXInternAtoms XInternAtoms_impl;
extern tXCopyColormapAndFree XCopyColormapAndFree_impl;
extern tXCreateColormap XCreateColormap_impl;
extern tXCreatePixmapCursor XCreatePixmapCursor_impl;
extern tXCreateGlyphCursor XCreateGlyphCursor_impl;
extern tXCreateFontCursor XCreateFontCursor_impl;
extern tXLoadFont XLoadFont_impl;
extern tXCreateGC XCreateGC_impl;
extern tXGContextFromGC XGContextFromGC_impl;
extern tXFlushGC XFlushGC_impl;
extern tXCreatePixmap XCreatePixmap_impl;
extern tXCreateBitmapFromData XCreateBitmapFromData_impl;
extern tXCreatePixmapFromBitmapData XCreatePixmapFromBitmapData_impl;
extern tXCreateSimpleWindow XCreateSimpleWindow_impl;
extern tXGetSelectionOwner XGetSelectionOwner_impl;
extern tXCreateWindow XCreateWindow_impl;
extern tXListInstalledColormaps XListInstalledColormaps_impl;
extern tXListFonts XListFonts_impl;
extern tXListFontsWithInfo XListFontsWithInfo_impl;
extern tXGetFontPath XGetFontPath_impl;
extern tXListExtensions XListExtensions_impl;
extern tXListProperties XListProperties_impl;
extern tXListHosts XListHosts_impl;
extern tXKeycodeToKeysym XKeycodeToKeysym_impl;
extern tXLookupKeysym XLookupKeysym_impl;
extern tXGetKeyboardMapping XGetKeyboardMapping_impl;
extern tXStringToKeysym XStringToKeysym_impl;
extern tXMaxRequestSize XMaxRequestSize_impl;
extern tXExtendedMaxRequestSize XExtendedMaxRequestSize_impl;
extern tXResourceManagerString XResourceManagerString_impl;
extern tXScreenResourceString XScreenResourceString_impl;
extern tXDisplayMotionBufferSize XDisplayMotionBufferSize_impl;
extern tXVisualIDFromVisual XVisualIDFromVisual_impl;
extern tXInitThreads XInitThreads_impl;
extern tXLockDisplay XLockDisplay_impl;
extern tXUnlockDisplay XUnlockDisplay_impl;
extern tXInitExtension XInitExtension_impl;
extern tXAddExtension XAddExtension_impl;
extern tXFindOnExtensionList XFindOnExtensionList_impl;
extern tXEHeadOfExtensionList XEHeadOfExtensionList_impl;
extern tXRootWindow XRootWindow_impl;
extern tXDefaultRootWindow XDefaultRootWindow_impl;
extern tXRootWindowOfScreen XRootWindowOfScreen_impl;
extern tXDefaultVisual XDefaultVisual_impl;
extern tXDefaultVisualOfScreen XDefaultVisualOfScreen_impl;
extern tXDefaultGC XDefaultGC_impl;
extern tXDefaultGCOfScreen XDefaultGCOfScreen_impl;
extern tXBlackPixel XBlackPixel_impl;
extern tXWhitePixel XWhitePixel_impl;
extern tXAllPlanes XAllPlanes_impl;
extern tXBlackPixelOfScreen XBlackPixelOfScreen_impl;
extern tXWhitePixelOfScreen XWhitePixelOfScreen_impl;
extern tXNextRequest XNextRequest_impl;
extern tXLastKnownRequestProcessed XLastKnownRequestProcessed_impl;
extern tXServerVendor XServerVendor_impl;
extern tXDisplayString XDisplayString_impl;
extern tXDefaultColormap XDefaultColormap_impl;
extern tXDefaultColormapOfScreen XDefaultColormapOfScreen_impl;
extern tXDisplayOfScreen XDisplayOfScreen_impl;
extern tXScreenOfDisplay XScreenOfDisplay_impl;
extern tXDefaultScreenOfDisplay XDefaultScreenOfDisplay_impl;
extern tXEventMaskOfScreen XEventMaskOfScreen_impl;
extern tXScreenNumberOfScreen XScreenNumberOfScreen_impl;
extern tXSetErrorHandler XSetErrorHandler_impl;
extern tXSetIOErrorHandler XSetIOErrorHandler_impl;
extern tXListPixmapFormats XListPixmapFormats_impl;
extern tXListDepths XListDepths_impl;
extern tXReconfigureWMWindow XReconfigureWMWindow_impl;
extern tXGetWMProtocols XGetWMProtocols_impl;
extern tXSetWMProtocols XSetWMProtocols_impl;
extern tXIconifyWindow XIconifyWindow_impl;
extern tXWithdrawWindow XWithdrawWindow_impl;
extern tXGetCommand XGetCommand_impl;
extern tXGetWMColormapWindows XGetWMColormapWindows_impl;
extern tXSetWMColormapWindows XSetWMColormapWindows_impl;
extern tXFreeStringList XFreeStringList_impl;
extern tXSetTransientForHint XSetTransientForHint_impl;
extern tXActivateScreenSaver XActivateScreenSaver_impl;
extern tXAddHost XAddHost_impl;
extern tXAddHosts XAddHosts_impl;
extern tXAddToExtensionList XAddToExtensionList_impl;
extern tXAddToSaveSet XAddToSaveSet_impl;
extern tXAllocColor XAllocColor_impl;
extern tXAllocColorCells XAllocColorCells_impl;
extern tXAllocColorPlanes XAllocColorPlanes_impl;
extern tXAllocNamedColor XAllocNamedColor_impl;
extern tXAllowEvents XAllowEvents_impl;
extern tXAutoRepeatOff XAutoRepeatOff_impl;
extern tXAutoRepeatOn XAutoRepeatOn_impl;
extern tXBell XBell_impl;
extern tXBitmapBitOrder XBitmapBitOrder_impl;
extern tXBitmapPad XBitmapPad_impl;
extern tXBitmapUnit XBitmapUnit_impl;
extern tXCellsOfScreen XCellsOfScreen_impl;
extern tXChangeActivePointerGrab XChangeActivePointerGrab_impl;
extern tXChangeGC XChangeGC_impl;
extern tXChangeKeyboardControl XChangeKeyboardControl_impl;
extern tXChangeKeyboardMapping XChangeKeyboardMapping_impl;
extern tXChangePointerControl XChangePointerControl_impl;
extern tXChangeProperty XChangeProperty_impl;
extern tXChangeSaveSet XChangeSaveSet_impl;
extern tXChangeWindowAttributes XChangeWindowAttributes_impl;
extern tXCheckIfEvent XCheckIfEvent_impl;
extern tXCheckMaskEvent XCheckMaskEvent_impl;
extern tXCheckTypedEvent XCheckTypedEvent_impl;
extern tXCheckTypedWindowEvent XCheckTypedWindowEvent_impl;
extern tXCheckWindowEvent XCheckWindowEvent_impl;
extern tXCirculateSubwindows XCirculateSubwindows_impl;
extern tXCirculateSubwindowsDown XCirculateSubwindowsDown_impl;
extern tXCirculateSubwindowsUp XCirculateSubwindowsUp_impl;
extern tXClearArea XClearArea_impl;
extern tXClearWindow XClearWindow_impl;
extern tXCloseDisplay XCloseDisplay_impl;
extern tXConfigureWindow XConfigureWindow_impl;
extern tXConnectionNumber XConnectionNumber_impl;
extern tXConvertSelection XConvertSelection_impl;
extern tXCopyArea XCopyArea_impl;
extern tXCopyGC XCopyGC_impl;
extern tXCopyPlane XCopyPlane_impl;
extern tXDefaultDepth XDefaultDepth_impl;
extern tXDefaultDepthOfScreen XDefaultDepthOfScreen_impl;
extern tXDefaultScreen XDefaultScreen_impl;
extern tXDefineCursor XDefineCursor_impl;
extern tXDeleteProperty XDeleteProperty_impl;
extern tXDestroyWindow XDestroyWindow_impl;
extern tXDestroySubwindows XDestroySubwindows_impl;
extern tXDoesBackingStore XDoesBackingStore_impl;
extern tXDoesSaveUnders XDoesSaveUnders_impl;
extern tXDisableAccessControl XDisableAccessControl_impl;
extern tXDisplayCells XDisplayCells_impl;
extern tXDisplayHeight XDisplayHeight_impl;
extern tXDisplayHeightMM XDisplayHeightMM_impl;
extern tXDisplayKeycodes XDisplayKeycodes_impl;
extern tXDisplayPlanes XDisplayPlanes_impl;
extern tXDisplayWidth XDisplayWidth_impl;
extern tXDisplayWidthMM XDisplayWidthMM_impl;
extern tXDrawArc XDrawArc_impl;
extern tXDrawArcs XDrawArcs_impl;
extern tXDrawImageString XDrawImageString_impl;
extern tXDrawImageString16 XDrawImageString16_impl;
extern tXDrawLine XDrawLine_impl;
extern tXDrawLines XDrawLines_impl;
extern tXDrawPoint XDrawPoint_impl;
extern tXDrawPoints XDrawPoints_impl;
extern tXDrawRectangle XDrawRectangle_impl;
extern tXDrawRectangles XDrawRectangles_impl;
extern tXDrawSegments XDrawSegments_impl;
extern tXDrawString XDrawString_impl;
extern tXDrawString16 XDrawString16_impl;
extern tXDrawText XDrawText_impl;
extern tXDrawText16 XDrawText16_impl;
extern tXEnableAccessControl XEnableAccessControl_impl;
extern tXEventsQueued XEventsQueued_impl;
extern tXFetchName XFetchName_impl;
extern tXFillArc XFillArc_impl;
extern tXFillArcs XFillArcs_impl;
extern tXFillPolygon XFillPolygon_impl;
extern tXFillRectangle XFillRectangle_impl;
extern tXFillRectangles XFillRectangles_impl;
extern tXFlush XFlush_impl;
extern tXForceScreenSaver XForceScreenSaver_impl;
extern tXFree XFree_impl;
extern tXFreeColormap XFreeColormap_impl;
extern tXFreeColors XFreeColors_impl;
extern tXFreeCursor XFreeCursor_impl;
extern tXFreeExtensionList XFreeExtensionList_impl;
extern tXFreeFont XFreeFont_impl;
extern tXFreeFontInfo XFreeFontInfo_impl;
extern tXFreeFontNames XFreeFontNames_impl;
extern tXFreeFontPath XFreeFontPath_impl;
extern tXFreeGC XFreeGC_impl;
extern tXFreeModifiermap XFreeModifiermap_impl;
extern tXFreePixmap XFreePixmap_impl;
extern tXGeometry XGeometry_impl;
extern tXGetErrorDatabaseText XGetErrorDatabaseText_impl;
extern tXGetErrorText XGetErrorText_impl;
extern tXGetFontProperty XGetFontProperty_impl;
extern tXGetGCValues XGetGCValues_impl;
extern tXGetGeometry XGetGeometry_impl;
extern tXGetIconName XGetIconName_impl;
extern tXGetInputFocus XGetInputFocus_impl;
extern tXGetKeyboardControl XGetKeyboardControl_impl;
extern tXGetPointerControl XGetPointerControl_impl;
extern tXGetPointerMapping XGetPointerMapping_impl;
extern tXGetScreenSaver XGetScreenSaver_impl;
extern tXGetTransientForHint XGetTransientForHint_impl;
extern tXGetWindowProperty XGetWindowProperty_impl;
extern tXGetWindowAttributes XGetWindowAttributes_impl;
extern tXGrabButton XGrabButton_impl;
extern tXGrabKey XGrabKey_impl;
extern tXGrabKeyboard XGrabKeyboard_impl;
extern tXGrabPointer XGrabPointer_impl;
extern tXGrabServer XGrabServer_impl;
extern tXHeightMMOfScreen XHeightMMOfScreen_impl;
extern tXHeightOfScreen XHeightOfScreen_impl;
extern tXIfEvent XIfEvent_impl;
extern tXImageByteOrder XImageByteOrder_impl;
extern tXInstallColormap XInstallColormap_impl;
extern tXKeysymToKeycode XKeysymToKeycode_impl;
extern tXKillClient XKillClient_impl;
extern tXLookupColor XLookupColor_impl;
extern tXLowerWindow XLowerWindow_impl;
extern tXMapRaised XMapRaised_impl;
extern tXMapSubwindows XMapSubwindows_impl;
extern tXMapWindow XMapWindow_impl;
extern tXMaskEvent XMaskEvent_impl;
extern tXMaxCmapsOfScreen XMaxCmapsOfScreen_impl;
extern tXMinCmapsOfScreen XMinCmapsOfScreen_impl;
extern tXMoveResizeWindow XMoveResizeWindow_impl;
extern tXMoveWindow XMoveWindow_impl;
extern tXNextEvent XNextEvent_impl;
extern tXNoOp XNoOp_impl;
extern tXParseColor XParseColor_impl;
extern tXParseGeometry XParseGeometry_impl;
extern tXPeekEvent XPeekEvent_impl;
extern tXPeekIfEvent XPeekIfEvent_impl;
extern tXPending XPending_impl;
extern tXPlanesOfScreen XPlanesOfScreen_impl;
extern tXProtocolRevision XProtocolRevision_impl;
extern tXProtocolVersion XProtocolVersion_impl;
extern tXPutBackEvent XPutBackEvent_impl;
extern tXPutImage XPutImage_impl;
extern tXQLength XQLength_impl;
extern tXQueryBestCursor XQueryBestCursor_impl;
extern tXQueryBestSize XQueryBestSize_impl;
extern tXQueryBestStipple XQueryBestStipple_impl;
extern tXQueryBestTile XQueryBestTile_impl;
extern tXQueryColor XQueryColor_impl;
extern tXQueryColors XQueryColors_impl;
extern tXQueryExtension XQueryExtension_impl;
extern tXQueryKeymap XQueryKeymap_impl;
extern tXQueryPointer XQueryPointer_impl;
extern tXQueryTextExtents XQueryTextExtents_impl;
extern tXQueryTextExtents16 XQueryTextExtents16_impl;
extern tXQueryTree XQueryTree_impl;
extern tXRaiseWindow XRaiseWindow_impl;
extern tXReadBitmapFile XReadBitmapFile_impl;
extern tXReadBitmapFileData XReadBitmapFileData_impl;
extern tXRebindKeysym XRebindKeysym_impl;
extern tXRecolorCursor XRecolorCursor_impl;
extern tXRefreshKeyboardMapping XRefreshKeyboardMapping_impl;
extern tXRemoveFromSaveSet XRemoveFromSaveSet_impl;
extern tXRemoveHost XRemoveHost_impl;
extern tXRemoveHosts XRemoveHosts_impl;
extern tXReparentWindow XReparentWindow_impl;
extern tXResetScreenSaver XResetScreenSaver_impl;
extern tXResizeWindow XResizeWindow_impl;
extern tXRestackWindows XRestackWindows_impl;
extern tXRotateBuffers XRotateBuffers_impl;
extern tXRotateWindowProperties XRotateWindowProperties_impl;
extern tXScreenCount XScreenCount_impl;
extern tXSelectInput XSelectInput_impl;
extern tXSendEvent XSendEvent_impl;
extern tXSetAccessControl XSetAccessControl_impl;
extern tXSetArcMode XSetArcMode_impl;
extern tXSetBackground XSetBackground_impl;
extern tXSetClipMask XSetClipMask_impl;
extern tXSetClipOrigin XSetClipOrigin_impl;
extern tXSetClipRectangles XSetClipRectangles_impl;
extern tXSetCloseDownMode XSetCloseDownMode_impl;
extern tXSetCommand XSetCommand_impl;
extern tXSetDashes XSetDashes_impl;
extern tXSetFillRule XSetFillRule_impl;
extern tXSetFillStyle XSetFillStyle_impl;
extern tXSetFont XSetFont_impl;
extern tXSetFontPath XSetFontPath_impl;
extern tXSetForeground XSetForeground_impl;
extern tXSetFunction XSetFunction_impl;
extern tXSetGraphicsExposures XSetGraphicsExposures_impl;
extern tXSetIconName XSetIconName_impl;
extern tXSetInputFocus XSetInputFocus_impl;
extern tXSetLineAttributes XSetLineAttributes_impl;
extern tXSetModifierMapping XSetModifierMapping_impl;
extern tXSetPlaneMask XSetPlaneMask_impl;
extern tXSetPointerMapping XSetPointerMapping_impl;
extern tXSetScreenSaver XSetScreenSaver_impl;
extern tXSetSelectionOwner XSetSelectionOwner_impl;
extern tXSetState XSetState_impl;
extern tXSetStipple XSetStipple_impl;
extern tXSetSubwindowMode XSetSubwindowMode_impl;
extern tXSetTSOrigin XSetTSOrigin_impl;
extern tXSetTile XSetTile_impl;
extern tXSetWindowBackground XSetWindowBackground_impl;
extern tXSetWindowBackgroundPixmap XSetWindowBackgroundPixmap_impl;
extern tXSetWindowBorder XSetWindowBorder_impl;
extern tXSetWindowBorderPixmap XSetWindowBorderPixmap_impl;
extern tXSetWindowBorderWidth XSetWindowBorderWidth_impl;
extern tXSetWindowColormap XSetWindowColormap_impl;
extern tXStoreBuffer XStoreBuffer_impl;
extern tXStoreBytes XStoreBytes_impl;
extern tXStoreColor XStoreColor_impl;
extern tXStoreColors XStoreColors_impl;
extern tXStoreName XStoreName_impl;
extern tXStoreNamedColor XStoreNamedColor_impl;
extern tXSync XSync_impl;
extern tXTextExtents XTextExtents_impl;
extern tXTextExtents16 XTextExtents16_impl;
extern tXTextWidth XTextWidth_impl;
extern tXTextWidth16 XTextWidth16_impl;
extern tXTranslateCoordinates XTranslateCoordinates_impl;
extern tXUndefineCursor XUndefineCursor_impl;
extern tXUngrabButton XUngrabButton_impl;
extern tXUngrabKey XUngrabKey_impl;
extern tXUngrabKeyboard XUngrabKeyboard_impl;
extern tXUngrabPointer XUngrabPointer_impl;
extern tXUngrabServer XUngrabServer_impl;
extern tXUninstallColormap XUninstallColormap_impl;
extern tXUnloadFont XUnloadFont_impl;
extern tXUnmapSubwindows XUnmapSubwindows_impl;
extern tXUnmapWindow XUnmapWindow_impl;
extern tXVendorRelease XVendorRelease_impl;
extern tXWarpPointer XWarpPointer_impl;
extern tXWidthMMOfScreen XWidthMMOfScreen_impl;
extern tXWidthOfScreen XWidthOfScreen_impl;
extern tXWindowEvent XWindowEvent_impl;
extern tXWriteBitmapFile XWriteBitmapFile_impl;
extern tXSupportsLocale XSupportsLocale_impl;
extern tXSetLocaleModifiers XSetLocaleModifiers_impl;
extern tXOpenOM XOpenOM_impl;
extern tXCloseOM XCloseOM_impl;
extern tXSetOMValues XSetOMValues_impl;
extern tXGetOMValues XGetOMValues_impl;
extern tXDisplayOfOM XDisplayOfOM_impl;
extern tXLocaleOfOM XLocaleOfOM_impl;
extern tXCreateOC XCreateOC_impl;
extern tXDestroyOC XDestroyOC_impl;
extern tXOMOfOC XOMOfOC_impl;
extern tXSetOCValues XSetOCValues_impl;
extern tXGetOCValues XGetOCValues_impl;
extern tXCreateFontSet XCreateFontSet_impl;
extern tXFreeFontSet XFreeFontSet_impl;
extern tXFontsOfFontSet XFontsOfFontSet_impl;
extern tXBaseFontNameListOfFontSet XBaseFontNameListOfFontSet_impl;
extern tXLocaleOfFontSet XLocaleOfFontSet_impl;
extern tXContextDependentDrawing XContextDependentDrawing_impl;
extern tXDirectionalDependentDrawing XDirectionalDependentDrawing_impl;
extern tXContextualDrawing XContextualDrawing_impl;
extern tXExtentsOfFontSet XExtentsOfFontSet_impl;
extern tXmbTextEscapement XmbTextEscapement_impl;
extern tXwcTextEscapement XwcTextEscapement_impl;
extern tXutf8TextEscapement Xutf8TextEscapement_impl;
extern tXmbTextExtents XmbTextExtents_impl;
extern tXwcTextExtents XwcTextExtents_impl;
extern tXutf8TextExtents Xutf8TextExtents_impl;
extern tXmbTextPerCharExtents XmbTextPerCharExtents_impl;
extern tXwcTextPerCharExtents XwcTextPerCharExtents_impl;
extern tXutf8TextPerCharExtents Xutf8TextPerCharExtents_impl;
extern tXmbDrawText XmbDrawText_impl;
extern tXwcDrawText XwcDrawText_impl;
extern tXutf8DrawText Xutf8DrawText_impl;
extern tXmbDrawString XmbDrawString_impl;
extern tXwcDrawString XwcDrawString_impl;
extern tXutf8DrawString Xutf8DrawString_impl;
extern tXmbDrawImageString XmbDrawImageString_impl;
extern tXwcDrawImageString XwcDrawImageString_impl;
extern tXutf8DrawImageString Xutf8DrawImageString_impl;
extern tXOpenIM XOpenIM_impl;
extern tXCloseIM XCloseIM_impl;
extern tXGetIMValues XGetIMValues_impl;
extern tXSetIMValues XSetIMValues_impl;
extern tXDisplayOfIM XDisplayOfIM_impl;
extern tXLocaleOfIM XLocaleOfIM_impl;
extern tXCreateIC XCreateIC_impl;
extern tXDestroyIC XDestroyIC_impl;
extern tXSetICFocus XSetICFocus_impl;
extern tXUnsetICFocus XUnsetICFocus_impl;
extern tXwcResetIC XwcResetIC_impl;
extern tXmbResetIC XmbResetIC_impl;
extern tXutf8ResetIC Xutf8ResetIC_impl;
extern tXSetICValues XSetICValues_impl;
extern tXGetICValues XGetICValues_impl;
extern tXIMOfIC XIMOfIC_impl;
extern tXFilterEvent XFilterEvent_impl;
extern tXmbLookupString XmbLookupString_impl;
extern tXwcLookupString XwcLookupString_impl;
extern tXutf8LookupString Xutf8LookupString_impl;
extern tXVaCreateNestedList XVaCreateNestedList_impl;
extern tXRegisterIMInstantiateCallback XRegisterIMInstantiateCallback_impl;
extern tXUnregisterIMInstantiateCallback XUnregisterIMInstantiateCallback_impl;
extern tXInternalConnectionNumbers XInternalConnectionNumbers_impl;
extern tXProcessInternalConnection XProcessInternalConnection_impl;
extern tXAddConnectionWatch XAddConnectionWatch_impl;
extern tXRemoveConnectionWatch XRemoveConnectionWatch_impl;
extern tXSetAuthorization XSetAuthorization_impl;
extern t_Xmbtowc _Xmbtowc_impl;
extern t_Xwctomb _Xwctomb_impl;
extern tXGetEventData XGetEventData_impl;
extern tXFreeEventData XFreeEventData_impl;

// Xlib-xcb.h
extern tXGetXCBConnection XGetXCBConnection_impl;
extern tXSetEventQueueOwner XSetEventQueueOwner_impl;

// Xlibint.h
extern t_XGetRequest _XGetRequest_impl;
extern t_XFlushGCCache _XFlushGCCache_impl;
extern t_XData32 _XData32_impl;
extern t_XRead32 _XRead32_impl;
extern t_XDeqAsyncHandler _XDeqAsyncHandler_impl;
extern tData Data_impl;
extern t_XError _XError_impl;
extern t_XIOError _XIOError_impl;
extern t_XEatData _XEatData_impl;
extern t_XEatDataWords _XEatDataWords_impl;
extern t_XAllocScratch _XAllocScratch_impl;
extern t_XAllocTemp _XAllocTemp_impl;
extern t_XFreeTemp _XFreeTemp_impl;
extern t_XVIDtoVisual _XVIDtoVisual_impl;
extern t_XSetLastRequestRead _XSetLastRequestRead_impl;
extern t_XGetHostname _XGetHostname_impl;
extern t_XScreenOfWindow _XScreenOfWindow_impl;
extern t_XAsyncErrorHandler _XAsyncErrorHandler_impl;
extern t_XGetAsyncReply _XGetAsyncReply_impl;
extern t_XGetAsyncData _XGetAsyncData_impl;
extern t_XFlush _XFlush_impl;
extern t_XEventsQueued _XEventsQueued_impl;
extern t_XReadEvents _XReadEvents_impl;
extern t_XRead _XRead_impl;
extern t_XReadPad _XReadPad_impl;
extern t_XSend _XSend_impl;
extern t_XReply _XReply_impl;
extern t_XEnq _XEnq_impl;
extern t_XDeq _XDeq_impl;
extern t_XUnknownWireEvent _XUnknownWireEvent_impl;
extern t_XUnknownWireEventCookie _XUnknownWireEventCookie_impl;
extern t_XUnknownCopyEventCookie _XUnknownCopyEventCookie_impl;
extern t_XUnknownNativeEvent _XUnknownNativeEvent_impl;
extern t_XWireToEvent _XWireToEvent_impl;
extern t_XDefaultWireError _XDefaultWireError_impl;
extern t_XPollfdCacheInit _XPollfdCacheInit_impl;
extern t_XPollfdCacheAdd _XPollfdCacheAdd_impl;
extern t_XPollfdCacheDel _XPollfdCacheDel_impl;
extern t_XAllocID _XAllocID_impl;
extern t_XAllocIDs _XAllocIDs_impl;
extern t_XFreeExtData _XFreeExtData_impl;
extern tXESetCreateGC XESetCreateGC_impl;
extern tXESetCopyGC XESetCopyGC_impl;
extern tXESetFlushGC XESetFlushGC_impl;
extern tXESetFreeGC XESetFreeGC_impl;
extern tXESetCreateFont XESetCreateFont_impl;
extern tXESetFreeFont XESetFreeFont_impl;
extern tXESetCloseDisplay XESetCloseDisplay_impl;
extern tXESetError XESetError_impl;
extern tXESetErrorString XESetErrorString_impl;
extern tXESetPrintErrorValues XESetPrintErrorValues_impl;
extern tXESetWireToEvent XESetWireToEvent_impl;
extern tXESetWireToEventCookie XESetWireToEventCookie_impl;
extern tXESetCopyEventCookie XESetCopyEventCookie_impl;
extern tXESetEventToWire XESetEventToWire_impl;
extern tXESetWireToError XESetWireToError_impl;
extern tXESetBeforeFlush XESetBeforeFlush_impl;
extern t_XRegisterInternalConnection _XRegisterInternalConnection_impl;
extern t_XUnregisterInternalConnection _XUnregisterInternalConnection_impl;
extern t_XProcessInternalConnection _XProcessInternalConnection_impl;
extern t__XOS2RedirRoot __XOS2RedirRoot_impl;
extern t_XTextHeight _XTextHeight_impl;
extern t_XTextHeight16 _XTextHeight16_impl;
extern t_XOpenFile _XOpenFile_impl;
extern t_XOpenFileMode _XOpenFileMode_impl;
extern t_XFopenFile _XFopenFile_impl;
extern t_XAccessFile _XAccessFile_impl;
extern t_XEventToWire _XEventToWire_impl;
extern t_XF86LoadQueryLocaleFont _XF86LoadQueryLocaleFont_impl;
extern t_XProcessWindowAttributes _XProcessWindowAttributes_impl;
extern t_XDefaultError _XDefaultError_impl;
extern t_XDefaultIOError _XDefaultIOError_impl;
extern t_XSetClipRectangles _XSetClipRectangles_impl;
extern t_XGetWindowAttributes _XGetWindowAttributes_impl;
extern t_XPutBackEvent _XPutBackEvent_impl;
extern t_XIsEventCookie _XIsEventCookie_impl;
extern t_XFreeEventCookies _XFreeEventCookies_impl;
extern t_XStoreEventCookie _XStoreEventCookie_impl;
extern t_XFetchEventCookie _XFetchEventCookie_impl;
extern t_XCopyEventCookie _XCopyEventCookie_impl;
extern txlocaledir xlocaledir_impl;

// XKBlib.h
extern tXkbIgnoreExtension XkbIgnoreExtension_impl;
extern tXkbOpenDisplay XkbOpenDisplay_impl;
extern tXkbQueryExtension XkbQueryExtension_impl;
extern tXkbUseExtension XkbUseExtension_impl;
extern tXkbLibraryVersion XkbLibraryVersion_impl;
extern tXkbSetXlibControls XkbSetXlibControls_impl;
extern tXkbGetXlibControls XkbGetXlibControls_impl;
extern tXkbXlibControlsImplemented XkbXlibControlsImplemented_impl;
extern tXkbSetAtomFuncs XkbSetAtomFuncs_impl;
extern tXkbKeycodeToKeysym XkbKeycodeToKeysym_impl;
extern tXkbKeysymToModifiers XkbKeysymToModifiers_impl;
extern tXkbLookupKeySym XkbLookupKeySym_impl;
extern tXkbLookupKeyBinding XkbLookupKeyBinding_impl;
extern tXkbTranslateKeyCode XkbTranslateKeyCode_impl;
extern tXkbTranslateKeySym XkbTranslateKeySym_impl;
extern tXkbSetAutoRepeatRate XkbSetAutoRepeatRate_impl;
extern tXkbGetAutoRepeatRate XkbGetAutoRepeatRate_impl;
extern tXkbChangeEnabledControls XkbChangeEnabledControls_impl;
extern tXkbDeviceBell XkbDeviceBell_impl;
extern tXkbForceDeviceBell XkbForceDeviceBell_impl;
extern tXkbDeviceBellEvent XkbDeviceBellEvent_impl;
extern tXkbBell XkbBell_impl;
extern tXkbForceBell XkbForceBell_impl;
extern tXkbBellEvent XkbBellEvent_impl;
extern tXkbSelectEvents XkbSelectEvents_impl;
extern tXkbSelectEventDetails XkbSelectEventDetails_impl;
extern tXkbNoteMapChanges XkbNoteMapChanges_impl;
extern tXkbNoteNameChanges XkbNoteNameChanges_impl;
extern tXkbGetIndicatorState XkbGetIndicatorState_impl;
extern tXkbGetDeviceIndicatorState XkbGetDeviceIndicatorState_impl;
extern tXkbGetIndicatorMap XkbGetIndicatorMap_impl;
extern tXkbSetIndicatorMap XkbSetIndicatorMap_impl;
extern tXkbGetNamedIndicator XkbGetNamedIndicator_impl;
extern tXkbGetNamedDeviceIndicator XkbGetNamedDeviceIndicator_impl;
extern tXkbSetNamedIndicator XkbSetNamedIndicator_impl;
extern tXkbSetNamedDeviceIndicator XkbSetNamedDeviceIndicator_impl;
extern tXkbLockModifiers XkbLockModifiers_impl;
extern tXkbLatchModifiers XkbLatchModifiers_impl;
extern tXkbLockGroup XkbLockGroup_impl;
extern tXkbLatchGroup XkbLatchGroup_impl;
extern tXkbSetServerInternalMods XkbSetServerInternalMods_impl;
extern tXkbSetIgnoreLockMods XkbSetIgnoreLockMods_impl;
extern tXkbVirtualModsToReal XkbVirtualModsToReal_impl;
extern tXkbComputeEffectiveMap XkbComputeEffectiveMap_impl;
extern tXkbInitCanonicalKeyTypes XkbInitCanonicalKeyTypes_impl;
extern tXkbAllocKeyboard XkbAllocKeyboard_impl;
extern tXkbFreeKeyboard XkbFreeKeyboard_impl;
extern tXkbAllocClientMap XkbAllocClientMap_impl;
extern tXkbAllocServerMap XkbAllocServerMap_impl;
extern tXkbFreeClientMap XkbFreeClientMap_impl;
extern tXkbFreeServerMap XkbFreeServerMap_impl;
extern tXkbAddKeyType XkbAddKeyType_impl;
extern tXkbAllocIndicatorMaps XkbAllocIndicatorMaps_impl;
extern tXkbFreeIndicatorMaps XkbFreeIndicatorMaps_impl;
extern tXkbGetMap XkbGetMap_impl;
extern tXkbGetUpdatedMap XkbGetUpdatedMap_impl;
extern tXkbGetMapChanges XkbGetMapChanges_impl;
extern tXkbRefreshKeyboardMapping XkbRefreshKeyboardMapping_impl;
extern tXkbGetKeyTypes XkbGetKeyTypes_impl;
extern tXkbGetKeySyms XkbGetKeySyms_impl;
extern tXkbGetKeyActions XkbGetKeyActions_impl;
extern tXkbGetKeyBehaviors XkbGetKeyBehaviors_impl;
extern tXkbGetVirtualMods XkbGetVirtualMods_impl;
extern tXkbGetKeyExplicitComponents XkbGetKeyExplicitComponents_impl;
extern tXkbGetKeyModifierMap XkbGetKeyModifierMap_impl;
extern tXkbGetKeyVirtualModMap XkbGetKeyVirtualModMap_impl;
extern tXkbAllocControls XkbAllocControls_impl;
extern tXkbFreeControls XkbFreeControls_impl;
extern tXkbGetControls XkbGetControls_impl;
extern tXkbSetControls XkbSetControls_impl;
extern tXkbNoteControlsChanges XkbNoteControlsChanges_impl;
extern tXkbAllocCompatMap XkbAllocCompatMap_impl;
extern tXkbFreeCompatMap XkbFreeCompatMap_impl;
extern tXkbGetCompatMap XkbGetCompatMap_impl;
extern tXkbSetCompatMap XkbSetCompatMap_impl;
extern tXkbAddSymInterpret XkbAddSymInterpret_impl;
extern tXkbAllocNames XkbAllocNames_impl;
extern tXkbGetNames XkbGetNames_impl;
extern tXkbSetNames XkbSetNames_impl;
extern tXkbChangeNames XkbChangeNames_impl;
extern tXkbFreeNames XkbFreeNames_impl;
extern tXkbGetState XkbGetState_impl;
extern tXkbSetMap XkbSetMap_impl;
extern tXkbChangeMap XkbChangeMap_impl;
extern tXkbSetDetectableAutoRepeat XkbSetDetectableAutoRepeat_impl;
extern tXkbGetDetectableAutoRepeat XkbGetDetectableAutoRepeat_impl;
extern tXkbSetAutoResetControls XkbSetAutoResetControls_impl;
extern tXkbGetAutoResetControls XkbGetAutoResetControls_impl;
extern tXkbSetPerClientControls XkbSetPerClientControls_impl;
extern tXkbGetPerClientControls XkbGetPerClientControls_impl;
extern tXkbCopyKeyType XkbCopyKeyType_impl;
extern tXkbCopyKeyTypes XkbCopyKeyTypes_impl;
extern tXkbResizeKeyType XkbResizeKeyType_impl;
extern tXkbResizeKeySyms XkbResizeKeySyms_impl;
extern tXkbResizeKeyActions XkbResizeKeyActions_impl;
extern tXkbChangeTypesOfKey XkbChangeTypesOfKey_impl;
extern tXkbChangeKeycodeRange XkbChangeKeycodeRange_impl;
extern tXkbListComponents XkbListComponents_impl;
extern tXkbFreeComponentList XkbFreeComponentList_impl;
extern tXkbGetKeyboard XkbGetKeyboard_impl;
extern tXkbGetKeyboardByName XkbGetKeyboardByName_impl;
extern tXkbKeyTypesForCoreSymbols XkbKeyTypesForCoreSymbols_impl;
extern tXkbApplyCompatMapToKey XkbApplyCompatMapToKey_impl;
extern tXkbUpdateMapFromCore XkbUpdateMapFromCore_impl;
extern tXkbAddDeviceLedInfo XkbAddDeviceLedInfo_impl;
extern tXkbResizeDeviceButtonActions XkbResizeDeviceButtonActions_impl;
extern tXkbAllocDeviceInfo XkbAllocDeviceInfo_impl;
extern tXkbFreeDeviceInfo XkbFreeDeviceInfo_impl;
extern tXkbNoteDeviceChanges XkbNoteDeviceChanges_impl;
extern tXkbGetDeviceInfo XkbGetDeviceInfo_impl;
extern tXkbGetDeviceInfoChanges XkbGetDeviceInfoChanges_impl;
extern tXkbGetDeviceButtonActions XkbGetDeviceButtonActions_impl;
extern tXkbGetDeviceLedInfo XkbGetDeviceLedInfo_impl;
extern tXkbSetDeviceInfo XkbSetDeviceInfo_impl;
extern tXkbChangeDeviceInfo XkbChangeDeviceInfo_impl;
extern tXkbSetDeviceLedInfo XkbSetDeviceLedInfo_impl;
extern tXkbSetDeviceButtonActions XkbSetDeviceButtonActions_impl;
extern tXkbToControl XkbToControl_impl;
extern tXkbSetDebuggingFlags XkbSetDebuggingFlags_impl;
extern tXkbApplyVirtualModChanges XkbApplyVirtualModChanges_impl;
extern tXkbUpdateActionVirtualMods XkbUpdateActionVirtualMods_impl;
extern tXkbUpdateKeyTypeVirtualMods XkbUpdateKeyTypeVirtualMods_impl;

// Xutil.h
extern tXDestroyImage XDestroyImage_impl;
extern tXGetPixel XGetPixel_impl;
extern tXPutPixel XPutPixel_impl;
extern tXSubImage XSubImage_impl;
extern tXAddPixel XAddPixel_impl;
extern tXAllocClassHint XAllocClassHint_impl;
extern tXAllocIconSize XAllocIconSize_impl;
extern tXAllocSizeHints XAllocSizeHints_impl;
extern tXAllocStandardColormap XAllocStandardColormap_impl;
extern tXAllocWMHints XAllocWMHints_impl;
extern tXClipBox XClipBox_impl;
extern tXCreateRegion XCreateRegion_impl;
extern tXDefaultString XDefaultString_impl;
extern tXDeleteContext XDeleteContext_impl;
extern tXDestroyRegion XDestroyRegion_impl;
extern tXEmptyRegion XEmptyRegion_impl;
extern tXEqualRegion XEqualRegion_impl;
extern tXFindContext XFindContext_impl;
extern tXGetClassHint XGetClassHint_impl;
extern tXGetIconSizes XGetIconSizes_impl;
extern tXGetNormalHints XGetNormalHints_impl;
extern tXGetRGBColormaps XGetRGBColormaps_impl;
extern tXGetSizeHints XGetSizeHints_impl;
extern tXGetStandardColormap XGetStandardColormap_impl;
extern tXGetTextProperty XGetTextProperty_impl;
extern tXGetVisualInfo XGetVisualInfo_impl;
extern tXGetWMClientMachine XGetWMClientMachine_impl;
extern tXGetWMHints XGetWMHints_impl;
extern tXGetWMIconName XGetWMIconName_impl;
extern tXGetWMName XGetWMName_impl;
extern tXGetWMNormalHints XGetWMNormalHints_impl;
extern tXGetWMSizeHints XGetWMSizeHints_impl;
extern tXGetZoomHints XGetZoomHints_impl;
extern tXIntersectRegion XIntersectRegion_impl;
extern tXConvertCase XConvertCase_impl;
extern tXLookupString XLookupString_impl;
extern tXMatchVisualInfo XMatchVisualInfo_impl;
extern tXOffsetRegion XOffsetRegion_impl;
extern tXPointInRegion XPointInRegion_impl;
extern tXPolygonRegion XPolygonRegion_impl;
extern tXRectInRegion XRectInRegion_impl;
extern tXSaveContext XSaveContext_impl;
extern tXSetClassHint XSetClassHint_impl;
extern tXSetIconSizes XSetIconSizes_impl;
extern tXSetNormalHints XSetNormalHints_impl;
extern tXSetRGBColormaps XSetRGBColormaps_impl;
extern tXSetSizeHints XSetSizeHints_impl;
extern tXSetStandardProperties XSetStandardProperties_impl;
extern tXSetTextProperty XSetTextProperty_impl;
extern tXSetWMClientMachine XSetWMClientMachine_impl;
extern tXSetWMHints XSetWMHints_impl;
extern tXSetWMIconName XSetWMIconName_impl;
extern tXSetWMName XSetWMName_impl;
extern tXSetWMNormalHints XSetWMNormalHints_impl;
extern tXSetWMProperties XSetWMProperties_impl;
extern tXmbSetWMProperties XmbSetWMProperties_impl;
extern tXutf8SetWMProperties Xutf8SetWMProperties_impl;
extern tXSetWMSizeHints XSetWMSizeHints_impl;
extern tXSetRegion XSetRegion_impl;
extern tXSetStandardColormap XSetStandardColormap_impl;
extern tXSetZoomHints XSetZoomHints_impl;
extern tXShrinkRegion XShrinkRegion_impl;
extern tXStringListToTextProperty XStringListToTextProperty_impl;
extern tXSubtractRegion XSubtractRegion_impl;
extern tXmbTextListToTextProperty XmbTextListToTextProperty_impl;
extern tXwcTextListToTextProperty XwcTextListToTextProperty_impl;
extern tXutf8TextListToTextProperty Xutf8TextListToTextProperty_impl;
extern tXwcFreeStringList XwcFreeStringList_impl;
extern tXTextPropertyToStringList XTextPropertyToStringList_impl;
extern tXmbTextPropertyToTextList XmbTextPropertyToTextList_impl;
extern tXwcTextPropertyToTextList XwcTextPropertyToTextList_impl;
extern tXutf8TextPropertyToTextList Xutf8TextPropertyToTextList_impl;
extern tXUnionRectWithRegion XUnionRectWithRegion_impl;
extern tXUnionRegion XUnionRegion_impl;
extern tXWMGeometry XWMGeometry_impl;
extern tXXorRegion XXorRegion_impl;

// Xinerama.h
extern tXineramaQueryExtension XineramaQueryExtension_impl;
extern tXineramaQueryVersion XineramaQueryVersion_impl;
extern tXineramaIsActive XineramaIsActive_impl;
extern tXineramaQueryScreens XineramaQueryScreens_impl;

////////////////////////////////////////////////////////////////////////////////
// Wrangler.

XewErrorCode xewX11Init(void);

// Check whether given function is available.
#define X11_HAS_SYMBOL(symbol) (XEW_HAS_SYMBOL_IMPL(symbol))

////////////////////////////////////////////////////////////////////////////////
// Variadic functions
//
// NOTE: Those can not be used as a fully transparent DL_PRELOAD replacement for
// the real ones, since we can not pass variadic arguments from one function to
// another.
// This means, to use those the application eeds to be actually complied using
// this header file.

#ifndef XEW_IMPL
// Xlib.h
#  define XSetOMValues XSetOMValues_impl
#  define XGetOMValues XGetOMValues_impl
#  define XCreateOC XCreateOC_impl
#  define XSetOCValues XSetOCValues_impl
#  define XGetOCValues XGetOCValues_impl
#  define XGetIMValues XGetIMValues_impl
#  define XSetIMValues XSetIMValues_impl
#  define XCreateIC XCreateIC_impl
#  define XSetICValues XSetICValues_impl
#  define XGetICValues XGetICValues_impl
#  define XVaCreateNestedList XVaCreateNestedList_impl

#endif  // XEW_IMPL

#ifdef __cplusplus
}
#endif

#endif  // X11_SYMBOLS_H_
