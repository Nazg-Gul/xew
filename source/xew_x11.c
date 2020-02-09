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

#include "xew_x11.h"

#include "xew_util.h"

// Per-platform list of libraries to be queried/
#ifdef _WIN32
static const char* x11_paths[] = {NULL};
static const char* x11_xcb_paths[] = {NULL};
static const char* x11_xinerama_paths[] = {NULL};
static const char* x11_cursor_paths[] = {NULL};
#elif defined(__APPLE__)
static const char* x11_paths[] = {NULL};
static const char* x11_xcb_paths[] = {NULL};
static const char* x11_xinerama_paths[] = {NULL};
static const char* x11_cursor_paths[] = {NULL};
#else
static const char* x11_paths[] = {"libX11.so.6",
                                  "libX11.so",
                                  NULL};
static const char* x11_xcb_paths[] = {"libX11-xcb.so.1",
                                      "libX11-xcb.so",
                                      NULL};
static const char* x11_xinerama_paths[] = {"libXinerama.so.1",
                                           "libXinerama.so",
                                           NULL};
static const char* x11_cursor_paths[] = {"libXcursor.so.1",
                                         "libXcursor.so",
                                         NULL};
#endif

static DynamicLibrary* x11_lib = NULL;
static DynamicLibrary* x11_xcb_lib = NULL;
static DynamicLibrary* x11_xinerama_lib = NULL;
static DynamicLibrary* x11_cursor_lib = NULL;

////////////////////////////////////////////////////////////////////////////////
// Functor definitions.

// Xlib.h
t_Xmblen _Xmblen_impl;
tXLoadQueryFont XLoadQueryFont_impl;
tXQueryFont XQueryFont_impl;
tXGetMotionEvents XGetMotionEvents_impl;
tXDeleteModifiermapEntry XDeleteModifiermapEntry_impl;
tXGetModifierMapping XGetModifierMapping_impl;
tXInsertModifiermapEntry XInsertModifiermapEntry_impl;
tXNewModifiermap XNewModifiermap_impl;
tXCreateImage XCreateImage_impl;
tXInitImage XInitImage_impl;
tXGetImage XGetImage_impl;
tXGetSubImage XGetSubImage_impl;
tXOpenDisplay XOpenDisplay_impl;
tXrmInitialize XrmInitialize_impl;
tXFetchBytes XFetchBytes_impl;
tXFetchBuffer XFetchBuffer_impl;
tXGetAtomName XGetAtomName_impl;
tXGetAtomNames XGetAtomNames_impl;
tXGetDefault XGetDefault_impl;
tXDisplayName XDisplayName_impl;
tXKeysymToString XKeysymToString_impl;
tXSynchronize XSynchronize_impl;
tXInternAtom XInternAtom_impl;
tXInternAtoms XInternAtoms_impl;
tXCopyColormapAndFree XCopyColormapAndFree_impl;
tXCreateColormap XCreateColormap_impl;
tXCreatePixmapCursor XCreatePixmapCursor_impl;
tXCreateGlyphCursor XCreateGlyphCursor_impl;
tXCreateFontCursor XCreateFontCursor_impl;
tXLoadFont XLoadFont_impl;
tXCreateGC XCreateGC_impl;
tXGContextFromGC XGContextFromGC_impl;
tXFlushGC XFlushGC_impl;
tXCreatePixmap XCreatePixmap_impl;
tXCreateBitmapFromData XCreateBitmapFromData_impl;
tXCreatePixmapFromBitmapData XCreatePixmapFromBitmapData_impl;
tXCreateSimpleWindow XCreateSimpleWindow_impl;
tXGetSelectionOwner XGetSelectionOwner_impl;
tXCreateWindow XCreateWindow_impl;
tXListInstalledColormaps XListInstalledColormaps_impl;
tXListFonts XListFonts_impl;
tXListFontsWithInfo XListFontsWithInfo_impl;
tXGetFontPath XGetFontPath_impl;
tXListExtensions XListExtensions_impl;
tXListProperties XListProperties_impl;
tXListHosts XListHosts_impl;
tXKeycodeToKeysym XKeycodeToKeysym_impl;
tXLookupKeysym XLookupKeysym_impl;
tXGetKeyboardMapping XGetKeyboardMapping_impl;
tXStringToKeysym XStringToKeysym_impl;
tXMaxRequestSize XMaxRequestSize_impl;
tXExtendedMaxRequestSize XExtendedMaxRequestSize_impl;
tXResourceManagerString XResourceManagerString_impl;
tXScreenResourceString XScreenResourceString_impl;
tXDisplayMotionBufferSize XDisplayMotionBufferSize_impl;
tXVisualIDFromVisual XVisualIDFromVisual_impl;
tXInitThreads XInitThreads_impl;
tXLockDisplay XLockDisplay_impl;
tXUnlockDisplay XUnlockDisplay_impl;
tXInitExtension XInitExtension_impl;
tXAddExtension XAddExtension_impl;
tXFindOnExtensionList XFindOnExtensionList_impl;
tXEHeadOfExtensionList XEHeadOfExtensionList_impl;
tXRootWindow XRootWindow_impl;
tXDefaultRootWindow XDefaultRootWindow_impl;
tXRootWindowOfScreen XRootWindowOfScreen_impl;
tXDefaultVisual XDefaultVisual_impl;
tXDefaultVisualOfScreen XDefaultVisualOfScreen_impl;
tXDefaultGC XDefaultGC_impl;
tXDefaultGCOfScreen XDefaultGCOfScreen_impl;
tXBlackPixel XBlackPixel_impl;
tXWhitePixel XWhitePixel_impl;
tXAllPlanes XAllPlanes_impl;
tXBlackPixelOfScreen XBlackPixelOfScreen_impl;
tXWhitePixelOfScreen XWhitePixelOfScreen_impl;
tXNextRequest XNextRequest_impl;
tXLastKnownRequestProcessed XLastKnownRequestProcessed_impl;
tXServerVendor XServerVendor_impl;
tXDisplayString XDisplayString_impl;
tXDefaultColormap XDefaultColormap_impl;
tXDefaultColormapOfScreen XDefaultColormapOfScreen_impl;
tXDisplayOfScreen XDisplayOfScreen_impl;
tXScreenOfDisplay XScreenOfDisplay_impl;
tXDefaultScreenOfDisplay XDefaultScreenOfDisplay_impl;
tXEventMaskOfScreen XEventMaskOfScreen_impl;
tXScreenNumberOfScreen XScreenNumberOfScreen_impl;
tXSetErrorHandler XSetErrorHandler_impl;
tXSetIOErrorHandler XSetIOErrorHandler_impl;
tXListPixmapFormats XListPixmapFormats_impl;
tXListDepths XListDepths_impl;
tXReconfigureWMWindow XReconfigureWMWindow_impl;
tXGetWMProtocols XGetWMProtocols_impl;
tXSetWMProtocols XSetWMProtocols_impl;
tXIconifyWindow XIconifyWindow_impl;
tXWithdrawWindow XWithdrawWindow_impl;
tXGetCommand XGetCommand_impl;
tXGetWMColormapWindows XGetWMColormapWindows_impl;
tXSetWMColormapWindows XSetWMColormapWindows_impl;
tXFreeStringList XFreeStringList_impl;
tXSetTransientForHint XSetTransientForHint_impl;
tXActivateScreenSaver XActivateScreenSaver_impl;
tXAddHost XAddHost_impl;
tXAddHosts XAddHosts_impl;
tXAddToExtensionList XAddToExtensionList_impl;
tXAddToSaveSet XAddToSaveSet_impl;
tXAllocColor XAllocColor_impl;
tXAllocColorCells XAllocColorCells_impl;
tXAllocColorPlanes XAllocColorPlanes_impl;
tXAllocNamedColor XAllocNamedColor_impl;
tXAllowEvents XAllowEvents_impl;
tXAutoRepeatOff XAutoRepeatOff_impl;
tXAutoRepeatOn XAutoRepeatOn_impl;
tXBell XBell_impl;
tXBitmapBitOrder XBitmapBitOrder_impl;
tXBitmapPad XBitmapPad_impl;
tXBitmapUnit XBitmapUnit_impl;
tXCellsOfScreen XCellsOfScreen_impl;
tXChangeActivePointerGrab XChangeActivePointerGrab_impl;
tXChangeGC XChangeGC_impl;
tXChangeKeyboardControl XChangeKeyboardControl_impl;
tXChangeKeyboardMapping XChangeKeyboardMapping_impl;
tXChangePointerControl XChangePointerControl_impl;
tXChangeProperty XChangeProperty_impl;
tXChangeSaveSet XChangeSaveSet_impl;
tXChangeWindowAttributes XChangeWindowAttributes_impl;
tXCheckIfEvent XCheckIfEvent_impl;
tXCheckMaskEvent XCheckMaskEvent_impl;
tXCheckTypedEvent XCheckTypedEvent_impl;
tXCheckTypedWindowEvent XCheckTypedWindowEvent_impl;
tXCheckWindowEvent XCheckWindowEvent_impl;
tXCirculateSubwindows XCirculateSubwindows_impl;
tXCirculateSubwindowsDown XCirculateSubwindowsDown_impl;
tXCirculateSubwindowsUp XCirculateSubwindowsUp_impl;
tXClearArea XClearArea_impl;
tXClearWindow XClearWindow_impl;
tXCloseDisplay XCloseDisplay_impl;
tXConfigureWindow XConfigureWindow_impl;
tXConnectionNumber XConnectionNumber_impl;
tXConvertSelection XConvertSelection_impl;
tXCopyArea XCopyArea_impl;
tXCopyGC XCopyGC_impl;
tXCopyPlane XCopyPlane_impl;
tXDefaultDepth XDefaultDepth_impl;
tXDefaultDepthOfScreen XDefaultDepthOfScreen_impl;
tXDefaultScreen XDefaultScreen_impl;
tXDefineCursor XDefineCursor_impl;
tXDeleteProperty XDeleteProperty_impl;
tXDestroyWindow XDestroyWindow_impl;
tXDestroySubwindows XDestroySubwindows_impl;
tXDoesBackingStore XDoesBackingStore_impl;
tXDoesSaveUnders XDoesSaveUnders_impl;
tXDisableAccessControl XDisableAccessControl_impl;
tXDisplayCells XDisplayCells_impl;
tXDisplayHeight XDisplayHeight_impl;
tXDisplayHeightMM XDisplayHeightMM_impl;
tXDisplayKeycodes XDisplayKeycodes_impl;
tXDisplayPlanes XDisplayPlanes_impl;
tXDisplayWidth XDisplayWidth_impl;
tXDisplayWidthMM XDisplayWidthMM_impl;
tXDrawArc XDrawArc_impl;
tXDrawArcs XDrawArcs_impl;
tXDrawImageString XDrawImageString_impl;
tXDrawImageString16 XDrawImageString16_impl;
tXDrawLine XDrawLine_impl;
tXDrawLines XDrawLines_impl;
tXDrawPoint XDrawPoint_impl;
tXDrawPoints XDrawPoints_impl;
tXDrawRectangle XDrawRectangle_impl;
tXDrawRectangles XDrawRectangles_impl;
tXDrawSegments XDrawSegments_impl;
tXDrawString XDrawString_impl;
tXDrawString16 XDrawString16_impl;
tXDrawText XDrawText_impl;
tXDrawText16 XDrawText16_impl;
tXEnableAccessControl XEnableAccessControl_impl;
tXEventsQueued XEventsQueued_impl;
tXFetchName XFetchName_impl;
tXFillArc XFillArc_impl;
tXFillArcs XFillArcs_impl;
tXFillPolygon XFillPolygon_impl;
tXFillRectangle XFillRectangle_impl;
tXFillRectangles XFillRectangles_impl;
tXFlush XFlush_impl;
tXForceScreenSaver XForceScreenSaver_impl;
tXFree XFree_impl;
tXFreeColormap XFreeColormap_impl;
tXFreeColors XFreeColors_impl;
tXFreeCursor XFreeCursor_impl;
tXFreeExtensionList XFreeExtensionList_impl;
tXFreeFont XFreeFont_impl;
tXFreeFontInfo XFreeFontInfo_impl;
tXFreeFontNames XFreeFontNames_impl;
tXFreeFontPath XFreeFontPath_impl;
tXFreeGC XFreeGC_impl;
tXFreeModifiermap XFreeModifiermap_impl;
tXFreePixmap XFreePixmap_impl;
tXGeometry XGeometry_impl;
tXGetErrorDatabaseText XGetErrorDatabaseText_impl;
tXGetErrorText XGetErrorText_impl;
tXGetFontProperty XGetFontProperty_impl;
tXGetGCValues XGetGCValues_impl;
tXGetGeometry XGetGeometry_impl;
tXGetIconName XGetIconName_impl;
tXGetInputFocus XGetInputFocus_impl;
tXGetKeyboardControl XGetKeyboardControl_impl;
tXGetPointerControl XGetPointerControl_impl;
tXGetPointerMapping XGetPointerMapping_impl;
tXGetScreenSaver XGetScreenSaver_impl;
tXGetTransientForHint XGetTransientForHint_impl;
tXGetWindowProperty XGetWindowProperty_impl;
tXGetWindowAttributes XGetWindowAttributes_impl;
tXGrabButton XGrabButton_impl;
tXGrabKey XGrabKey_impl;
tXGrabKeyboard XGrabKeyboard_impl;
tXGrabPointer XGrabPointer_impl;
tXGrabServer XGrabServer_impl;
tXHeightMMOfScreen XHeightMMOfScreen_impl;
tXHeightOfScreen XHeightOfScreen_impl;
tXIfEvent XIfEvent_impl;
tXImageByteOrder XImageByteOrder_impl;
tXInstallColormap XInstallColormap_impl;
tXKeysymToKeycode XKeysymToKeycode_impl;
tXKillClient XKillClient_impl;
tXLookupColor XLookupColor_impl;
tXLowerWindow XLowerWindow_impl;
tXMapRaised XMapRaised_impl;
tXMapSubwindows XMapSubwindows_impl;
tXMapWindow XMapWindow_impl;
tXMaskEvent XMaskEvent_impl;
tXMaxCmapsOfScreen XMaxCmapsOfScreen_impl;
tXMinCmapsOfScreen XMinCmapsOfScreen_impl;
tXMoveResizeWindow XMoveResizeWindow_impl;
tXMoveWindow XMoveWindow_impl;
tXNextEvent XNextEvent_impl;
tXNoOp XNoOp_impl;
tXParseColor XParseColor_impl;
tXParseGeometry XParseGeometry_impl;
tXPeekEvent XPeekEvent_impl;
tXPeekIfEvent XPeekIfEvent_impl;
tXPending XPending_impl;
tXPlanesOfScreen XPlanesOfScreen_impl;
tXProtocolRevision XProtocolRevision_impl;
tXProtocolVersion XProtocolVersion_impl;
tXPutBackEvent XPutBackEvent_impl;
tXPutImage XPutImage_impl;
tXQLength XQLength_impl;
tXQueryBestCursor XQueryBestCursor_impl;
tXQueryBestSize XQueryBestSize_impl;
tXQueryBestStipple XQueryBestStipple_impl;
tXQueryBestTile XQueryBestTile_impl;
tXQueryColor XQueryColor_impl;
tXQueryColors XQueryColors_impl;
tXQueryExtension XQueryExtension_impl;
tXQueryKeymap XQueryKeymap_impl;
tXQueryPointer XQueryPointer_impl;
tXQueryTextExtents XQueryTextExtents_impl;
tXQueryTextExtents16 XQueryTextExtents16_impl;
tXQueryTree XQueryTree_impl;
tXRaiseWindow XRaiseWindow_impl;
tXReadBitmapFile XReadBitmapFile_impl;
tXReadBitmapFileData XReadBitmapFileData_impl;
tXRebindKeysym XRebindKeysym_impl;
tXRecolorCursor XRecolorCursor_impl;
tXRefreshKeyboardMapping XRefreshKeyboardMapping_impl;
tXRemoveFromSaveSet XRemoveFromSaveSet_impl;
tXRemoveHost XRemoveHost_impl;
tXRemoveHosts XRemoveHosts_impl;
tXReparentWindow XReparentWindow_impl;
tXResetScreenSaver XResetScreenSaver_impl;
tXResizeWindow XResizeWindow_impl;
tXRestackWindows XRestackWindows_impl;
tXRotateBuffers XRotateBuffers_impl;
tXRotateWindowProperties XRotateWindowProperties_impl;
tXScreenCount XScreenCount_impl;
tXSelectInput XSelectInput_impl;
tXSendEvent XSendEvent_impl;
tXSetAccessControl XSetAccessControl_impl;
tXSetArcMode XSetArcMode_impl;
tXSetBackground XSetBackground_impl;
tXSetClipMask XSetClipMask_impl;
tXSetClipOrigin XSetClipOrigin_impl;
tXSetClipRectangles XSetClipRectangles_impl;
tXSetCloseDownMode XSetCloseDownMode_impl;
tXSetCommand XSetCommand_impl;
tXSetDashes XSetDashes_impl;
tXSetFillRule XSetFillRule_impl;
tXSetFillStyle XSetFillStyle_impl;
tXSetFont XSetFont_impl;
tXSetFontPath XSetFontPath_impl;
tXSetForeground XSetForeground_impl;
tXSetFunction XSetFunction_impl;
tXSetGraphicsExposures XSetGraphicsExposures_impl;
tXSetIconName XSetIconName_impl;
tXSetInputFocus XSetInputFocus_impl;
tXSetLineAttributes XSetLineAttributes_impl;
tXSetModifierMapping XSetModifierMapping_impl;
tXSetPlaneMask XSetPlaneMask_impl;
tXSetPointerMapping XSetPointerMapping_impl;
tXSetScreenSaver XSetScreenSaver_impl;
tXSetSelectionOwner XSetSelectionOwner_impl;
tXSetState XSetState_impl;
tXSetStipple XSetStipple_impl;
tXSetSubwindowMode XSetSubwindowMode_impl;
tXSetTSOrigin XSetTSOrigin_impl;
tXSetTile XSetTile_impl;
tXSetWindowBackground XSetWindowBackground_impl;
tXSetWindowBackgroundPixmap XSetWindowBackgroundPixmap_impl;
tXSetWindowBorder XSetWindowBorder_impl;
tXSetWindowBorderPixmap XSetWindowBorderPixmap_impl;
tXSetWindowBorderWidth XSetWindowBorderWidth_impl;
tXSetWindowColormap XSetWindowColormap_impl;
tXStoreBuffer XStoreBuffer_impl;
tXStoreBytes XStoreBytes_impl;
tXStoreColor XStoreColor_impl;
tXStoreColors XStoreColors_impl;
tXStoreName XStoreName_impl;
tXStoreNamedColor XStoreNamedColor_impl;
tXSync XSync_impl;
tXTextExtents XTextExtents_impl;
tXTextExtents16 XTextExtents16_impl;
tXTextWidth XTextWidth_impl;
tXTextWidth16 XTextWidth16_impl;
tXTranslateCoordinates XTranslateCoordinates_impl;
tXUndefineCursor XUndefineCursor_impl;
tXUngrabButton XUngrabButton_impl;
tXUngrabKey XUngrabKey_impl;
tXUngrabKeyboard XUngrabKeyboard_impl;
tXUngrabPointer XUngrabPointer_impl;
tXUngrabServer XUngrabServer_impl;
tXUninstallColormap XUninstallColormap_impl;
tXUnloadFont XUnloadFont_impl;
tXUnmapSubwindows XUnmapSubwindows_impl;
tXUnmapWindow XUnmapWindow_impl;
tXVendorRelease XVendorRelease_impl;
tXWarpPointer XWarpPointer_impl;
tXWidthMMOfScreen XWidthMMOfScreen_impl;
tXWidthOfScreen XWidthOfScreen_impl;
tXWindowEvent XWindowEvent_impl;
tXWriteBitmapFile XWriteBitmapFile_impl;
tXSupportsLocale XSupportsLocale_impl;
tXSetLocaleModifiers XSetLocaleModifiers_impl;
tXOpenOM XOpenOM_impl;
tXCloseOM XCloseOM_impl;
tXSetOMValues XSetOMValues_impl;
tXGetOMValues XGetOMValues_impl;
tXDisplayOfOM XDisplayOfOM_impl;
tXLocaleOfOM XLocaleOfOM_impl;
tXCreateOC XCreateOC_impl;
tXDestroyOC XDestroyOC_impl;
tXOMOfOC XOMOfOC_impl;
tXSetOCValues XSetOCValues_impl;
tXGetOCValues XGetOCValues_impl;
tXCreateFontSet XCreateFontSet_impl;
tXFreeFontSet XFreeFontSet_impl;
tXFontsOfFontSet XFontsOfFontSet_impl;
tXBaseFontNameListOfFontSet XBaseFontNameListOfFontSet_impl;
tXLocaleOfFontSet XLocaleOfFontSet_impl;
tXContextDependentDrawing XContextDependentDrawing_impl;
tXDirectionalDependentDrawing XDirectionalDependentDrawing_impl;
tXContextualDrawing XContextualDrawing_impl;
tXExtentsOfFontSet XExtentsOfFontSet_impl;
tXmbTextEscapement XmbTextEscapement_impl;
tXwcTextEscapement XwcTextEscapement_impl;
tXutf8TextEscapement Xutf8TextEscapement_impl;
tXmbTextExtents XmbTextExtents_impl;
tXwcTextExtents XwcTextExtents_impl;
tXutf8TextExtents Xutf8TextExtents_impl;
tXmbTextPerCharExtents XmbTextPerCharExtents_impl;
tXwcTextPerCharExtents XwcTextPerCharExtents_impl;
tXutf8TextPerCharExtents Xutf8TextPerCharExtents_impl;
tXmbDrawText XmbDrawText_impl;
tXwcDrawText XwcDrawText_impl;
tXutf8DrawText Xutf8DrawText_impl;
tXmbDrawString XmbDrawString_impl;
tXwcDrawString XwcDrawString_impl;
tXutf8DrawString Xutf8DrawString_impl;
tXmbDrawImageString XmbDrawImageString_impl;
tXwcDrawImageString XwcDrawImageString_impl;
tXutf8DrawImageString Xutf8DrawImageString_impl;
tXOpenIM XOpenIM_impl;
tXCloseIM XCloseIM_impl;
tXGetIMValues XGetIMValues_impl;
tXSetIMValues XSetIMValues_impl;
tXDisplayOfIM XDisplayOfIM_impl;
tXLocaleOfIM XLocaleOfIM_impl;
tXCreateIC XCreateIC_impl;
tXDestroyIC XDestroyIC_impl;
tXSetICFocus XSetICFocus_impl;
tXUnsetICFocus XUnsetICFocus_impl;
tXwcResetIC XwcResetIC_impl;
tXmbResetIC XmbResetIC_impl;
tXutf8ResetIC Xutf8ResetIC_impl;
tXSetICValues XSetICValues_impl;
tXGetICValues XGetICValues_impl;
tXIMOfIC XIMOfIC_impl;
tXFilterEvent XFilterEvent_impl;
tXmbLookupString XmbLookupString_impl;
tXwcLookupString XwcLookupString_impl;
tXutf8LookupString Xutf8LookupString_impl;
tXVaCreateNestedList XVaCreateNestedList_impl;
tXRegisterIMInstantiateCallback XRegisterIMInstantiateCallback_impl;
tXUnregisterIMInstantiateCallback XUnregisterIMInstantiateCallback_impl;
tXInternalConnectionNumbers XInternalConnectionNumbers_impl;
tXProcessInternalConnection XProcessInternalConnection_impl;
tXAddConnectionWatch XAddConnectionWatch_impl;
tXRemoveConnectionWatch XRemoveConnectionWatch_impl;
tXSetAuthorization XSetAuthorization_impl;
t_Xmbtowc _Xmbtowc_impl;
t_Xwctomb _Xwctomb_impl;
tXGetEventData XGetEventData_impl;
tXFreeEventData XFreeEventData_impl;

// Xlib-xcb.h
tXGetXCBConnection XGetXCBConnection_impl;
tXSetEventQueueOwner XSetEventQueueOwner_impl;

// Xlibint.h
t_XGetRequest _XGetRequest_impl;
t_XFlushGCCache _XFlushGCCache_impl;
t_XData32 _XData32_impl;
t_XRead32 _XRead32_impl;
t_XDeqAsyncHandler _XDeqAsyncHandler_impl;
tData Data_impl;
t_XError _XError_impl;
t_XIOError _XIOError_impl;
t_XEatData _XEatData_impl;
t_XEatDataWords _XEatDataWords_impl;
t_XAllocScratch _XAllocScratch_impl;
t_XAllocTemp _XAllocTemp_impl;
t_XFreeTemp _XFreeTemp_impl;
t_XVIDtoVisual _XVIDtoVisual_impl;
t_XSetLastRequestRead _XSetLastRequestRead_impl;
t_XGetHostname _XGetHostname_impl;
t_XScreenOfWindow _XScreenOfWindow_impl;
t_XAsyncErrorHandler _XAsyncErrorHandler_impl;
t_XGetAsyncReply _XGetAsyncReply_impl;
t_XGetAsyncData _XGetAsyncData_impl;
t_XFlush _XFlush_impl;
t_XEventsQueued _XEventsQueued_impl;
t_XReadEvents _XReadEvents_impl;
t_XRead _XRead_impl;
t_XReadPad _XReadPad_impl;
t_XSend _XSend_impl;
t_XReply _XReply_impl;
t_XEnq _XEnq_impl;
t_XDeq _XDeq_impl;
t_XUnknownWireEvent _XUnknownWireEvent_impl;
t_XUnknownWireEventCookie _XUnknownWireEventCookie_impl;
t_XUnknownCopyEventCookie _XUnknownCopyEventCookie_impl;
t_XUnknownNativeEvent _XUnknownNativeEvent_impl;
t_XWireToEvent _XWireToEvent_impl;
t_XDefaultWireError _XDefaultWireError_impl;
t_XPollfdCacheInit _XPollfdCacheInit_impl;
t_XPollfdCacheAdd _XPollfdCacheAdd_impl;
t_XPollfdCacheDel _XPollfdCacheDel_impl;
t_XAllocID _XAllocID_impl;
t_XAllocIDs _XAllocIDs_impl;
t_XFreeExtData _XFreeExtData_impl;
tXESetCreateGC XESetCreateGC_impl;
tXESetCopyGC XESetCopyGC_impl;
tXESetFlushGC XESetFlushGC_impl;
tXESetFreeGC XESetFreeGC_impl;
tXESetCreateFont XESetCreateFont_impl;
tXESetFreeFont XESetFreeFont_impl;
tXESetCloseDisplay XESetCloseDisplay_impl;
tXESetError XESetError_impl;
tXESetErrorString XESetErrorString_impl;
tXESetPrintErrorValues XESetPrintErrorValues_impl;
tXESetWireToEvent XESetWireToEvent_impl;
tXESetWireToEventCookie XESetWireToEventCookie_impl;
tXESetCopyEventCookie XESetCopyEventCookie_impl;
tXESetEventToWire XESetEventToWire_impl;
tXESetWireToError XESetWireToError_impl;
tXESetBeforeFlush XESetBeforeFlush_impl;
t_XRegisterInternalConnection _XRegisterInternalConnection_impl;
t_XUnregisterInternalConnection _XUnregisterInternalConnection_impl;
t_XProcessInternalConnection _XProcessInternalConnection_impl;
t__XOS2RedirRoot __XOS2RedirRoot_impl;
t_XTextHeight _XTextHeight_impl;
t_XTextHeight16 _XTextHeight16_impl;
t_XOpenFile _XOpenFile_impl;
t_XOpenFileMode _XOpenFileMode_impl;
t_XFopenFile _XFopenFile_impl;
t_XAccessFile _XAccessFile_impl;
t_XEventToWire _XEventToWire_impl;
t_XF86LoadQueryLocaleFont _XF86LoadQueryLocaleFont_impl;
t_XProcessWindowAttributes _XProcessWindowAttributes_impl;
t_XDefaultError _XDefaultError_impl;
t_XDefaultIOError _XDefaultIOError_impl;
t_XSetClipRectangles _XSetClipRectangles_impl;
t_XGetWindowAttributes _XGetWindowAttributes_impl;
t_XPutBackEvent _XPutBackEvent_impl;
t_XIsEventCookie _XIsEventCookie_impl;
t_XFreeEventCookies _XFreeEventCookies_impl;
t_XStoreEventCookie _XStoreEventCookie_impl;
t_XFetchEventCookie _XFetchEventCookie_impl;
t_XCopyEventCookie _XCopyEventCookie_impl;
txlocaledir xlocaledir_impl;

// XKBlib.h
tXkbIgnoreExtension XkbIgnoreExtension_impl;
tXkbOpenDisplay XkbOpenDisplay_impl;
tXkbQueryExtension XkbQueryExtension_impl;
tXkbUseExtension XkbUseExtension_impl;
tXkbLibraryVersion XkbLibraryVersion_impl;
tXkbSetXlibControls XkbSetXlibControls_impl;
tXkbGetXlibControls XkbGetXlibControls_impl;
tXkbXlibControlsImplemented XkbXlibControlsImplemented_impl;
tXkbSetAtomFuncs XkbSetAtomFuncs_impl;
tXkbKeycodeToKeysym XkbKeycodeToKeysym_impl;
tXkbKeysymToModifiers XkbKeysymToModifiers_impl;
tXkbLookupKeySym XkbLookupKeySym_impl;
tXkbLookupKeyBinding XkbLookupKeyBinding_impl;
tXkbTranslateKeyCode XkbTranslateKeyCode_impl;
tXkbTranslateKeySym XkbTranslateKeySym_impl;
tXkbSetAutoRepeatRate XkbSetAutoRepeatRate_impl;
tXkbGetAutoRepeatRate XkbGetAutoRepeatRate_impl;
tXkbChangeEnabledControls XkbChangeEnabledControls_impl;
tXkbDeviceBell XkbDeviceBell_impl;
tXkbForceDeviceBell XkbForceDeviceBell_impl;
tXkbDeviceBellEvent XkbDeviceBellEvent_impl;
tXkbBell XkbBell_impl;
tXkbForceBell XkbForceBell_impl;
tXkbBellEvent XkbBellEvent_impl;
tXkbSelectEvents XkbSelectEvents_impl;
tXkbSelectEventDetails XkbSelectEventDetails_impl;
tXkbNoteMapChanges XkbNoteMapChanges_impl;
tXkbNoteNameChanges XkbNoteNameChanges_impl;
tXkbGetIndicatorState XkbGetIndicatorState_impl;
tXkbGetDeviceIndicatorState XkbGetDeviceIndicatorState_impl;
tXkbGetIndicatorMap XkbGetIndicatorMap_impl;
tXkbSetIndicatorMap XkbSetIndicatorMap_impl;
tXkbGetNamedIndicator XkbGetNamedIndicator_impl;
tXkbGetNamedDeviceIndicator XkbGetNamedDeviceIndicator_impl;
tXkbSetNamedIndicator XkbSetNamedIndicator_impl;
tXkbSetNamedDeviceIndicator XkbSetNamedDeviceIndicator_impl;
tXkbLockModifiers XkbLockModifiers_impl;
tXkbLatchModifiers XkbLatchModifiers_impl;
tXkbLockGroup XkbLockGroup_impl;
tXkbLatchGroup XkbLatchGroup_impl;
tXkbSetServerInternalMods XkbSetServerInternalMods_impl;
tXkbSetIgnoreLockMods XkbSetIgnoreLockMods_impl;
tXkbVirtualModsToReal XkbVirtualModsToReal_impl;
tXkbComputeEffectiveMap XkbComputeEffectiveMap_impl;
tXkbInitCanonicalKeyTypes XkbInitCanonicalKeyTypes_impl;
tXkbAllocKeyboard XkbAllocKeyboard_impl;
tXkbFreeKeyboard XkbFreeKeyboard_impl;
tXkbAllocClientMap XkbAllocClientMap_impl;
tXkbAllocServerMap XkbAllocServerMap_impl;
tXkbFreeClientMap XkbFreeClientMap_impl;
tXkbFreeServerMap XkbFreeServerMap_impl;
tXkbAddKeyType XkbAddKeyType_impl;
tXkbAllocIndicatorMaps XkbAllocIndicatorMaps_impl;
tXkbFreeIndicatorMaps XkbFreeIndicatorMaps_impl;
tXkbGetMap XkbGetMap_impl;
tXkbGetUpdatedMap XkbGetUpdatedMap_impl;
tXkbGetMapChanges XkbGetMapChanges_impl;
tXkbRefreshKeyboardMapping XkbRefreshKeyboardMapping_impl;
tXkbGetKeyTypes XkbGetKeyTypes_impl;
tXkbGetKeySyms XkbGetKeySyms_impl;
tXkbGetKeyActions XkbGetKeyActions_impl;
tXkbGetKeyBehaviors XkbGetKeyBehaviors_impl;
tXkbGetVirtualMods XkbGetVirtualMods_impl;
tXkbGetKeyExplicitComponents XkbGetKeyExplicitComponents_impl;
tXkbGetKeyModifierMap XkbGetKeyModifierMap_impl;
tXkbGetKeyVirtualModMap XkbGetKeyVirtualModMap_impl;
tXkbAllocControls XkbAllocControls_impl;
tXkbFreeControls XkbFreeControls_impl;
tXkbGetControls XkbGetControls_impl;
tXkbSetControls XkbSetControls_impl;
tXkbNoteControlsChanges XkbNoteControlsChanges_impl;
tXkbAllocCompatMap XkbAllocCompatMap_impl;
tXkbFreeCompatMap XkbFreeCompatMap_impl;
tXkbGetCompatMap XkbGetCompatMap_impl;
tXkbSetCompatMap XkbSetCompatMap_impl;
tXkbAddSymInterpret XkbAddSymInterpret_impl;
tXkbAllocNames XkbAllocNames_impl;
tXkbGetNames XkbGetNames_impl;
tXkbSetNames XkbSetNames_impl;
tXkbChangeNames XkbChangeNames_impl;
tXkbFreeNames XkbFreeNames_impl;
tXkbGetState XkbGetState_impl;
tXkbSetMap XkbSetMap_impl;
tXkbChangeMap XkbChangeMap_impl;
tXkbSetDetectableAutoRepeat XkbSetDetectableAutoRepeat_impl;
tXkbGetDetectableAutoRepeat XkbGetDetectableAutoRepeat_impl;
tXkbSetAutoResetControls XkbSetAutoResetControls_impl;
tXkbGetAutoResetControls XkbGetAutoResetControls_impl;
tXkbSetPerClientControls XkbSetPerClientControls_impl;
tXkbGetPerClientControls XkbGetPerClientControls_impl;
tXkbCopyKeyType XkbCopyKeyType_impl;
tXkbCopyKeyTypes XkbCopyKeyTypes_impl;
tXkbResizeKeyType XkbResizeKeyType_impl;
tXkbResizeKeySyms XkbResizeKeySyms_impl;
tXkbResizeKeyActions XkbResizeKeyActions_impl;
tXkbChangeTypesOfKey XkbChangeTypesOfKey_impl;
tXkbChangeKeycodeRange XkbChangeKeycodeRange_impl;
tXkbListComponents XkbListComponents_impl;
tXkbFreeComponentList XkbFreeComponentList_impl;
tXkbGetKeyboard XkbGetKeyboard_impl;
tXkbGetKeyboardByName XkbGetKeyboardByName_impl;
tXkbKeyTypesForCoreSymbols XkbKeyTypesForCoreSymbols_impl;
tXkbApplyCompatMapToKey XkbApplyCompatMapToKey_impl;
tXkbUpdateMapFromCore XkbUpdateMapFromCore_impl;
tXkbAddDeviceLedInfo XkbAddDeviceLedInfo_impl;
tXkbResizeDeviceButtonActions XkbResizeDeviceButtonActions_impl;
tXkbAllocDeviceInfo XkbAllocDeviceInfo_impl;
tXkbFreeDeviceInfo XkbFreeDeviceInfo_impl;
tXkbNoteDeviceChanges XkbNoteDeviceChanges_impl;
tXkbGetDeviceInfo XkbGetDeviceInfo_impl;
tXkbGetDeviceInfoChanges XkbGetDeviceInfoChanges_impl;
tXkbGetDeviceButtonActions XkbGetDeviceButtonActions_impl;
tXkbGetDeviceLedInfo XkbGetDeviceLedInfo_impl;
tXkbSetDeviceInfo XkbSetDeviceInfo_impl;
tXkbChangeDeviceInfo XkbChangeDeviceInfo_impl;
tXkbSetDeviceLedInfo XkbSetDeviceLedInfo_impl;
tXkbSetDeviceButtonActions XkbSetDeviceButtonActions_impl;
tXkbToControl XkbToControl_impl;
tXkbSetDebuggingFlags XkbSetDebuggingFlags_impl;
tXkbApplyVirtualModChanges XkbApplyVirtualModChanges_impl;
tXkbUpdateActionVirtualMods XkbUpdateActionVirtualMods_impl;
tXkbUpdateKeyTypeVirtualMods XkbUpdateKeyTypeVirtualMods_impl;

// Xutil.h
tXDestroyImage XDestroyImage_impl;
tXGetPixel XGetPixel_impl;
tXPutPixel XPutPixel_impl;
tXSubImage XSubImage_impl;
tXAddPixel XAddPixel_impl;
tXAllocClassHint XAllocClassHint_impl;
tXAllocIconSize XAllocIconSize_impl;
tXAllocSizeHints XAllocSizeHints_impl;
tXAllocStandardColormap XAllocStandardColormap_impl;
tXAllocWMHints XAllocWMHints_impl;
tXClipBox XClipBox_impl;
tXCreateRegion XCreateRegion_impl;
tXDefaultString XDefaultString_impl;
tXDeleteContext XDeleteContext_impl;
tXDestroyRegion XDestroyRegion_impl;
tXEmptyRegion XEmptyRegion_impl;
tXEqualRegion XEqualRegion_impl;
tXFindContext XFindContext_impl;
tXGetClassHint XGetClassHint_impl;
tXGetIconSizes XGetIconSizes_impl;
tXGetNormalHints XGetNormalHints_impl;
tXGetRGBColormaps XGetRGBColormaps_impl;
tXGetSizeHints XGetSizeHints_impl;
tXGetStandardColormap XGetStandardColormap_impl;
tXGetTextProperty XGetTextProperty_impl;
tXGetVisualInfo XGetVisualInfo_impl;
tXGetWMClientMachine XGetWMClientMachine_impl;
tXGetWMHints XGetWMHints_impl;
tXGetWMIconName XGetWMIconName_impl;
tXGetWMName XGetWMName_impl;
tXGetWMNormalHints XGetWMNormalHints_impl;
tXGetWMSizeHints XGetWMSizeHints_impl;
tXGetZoomHints XGetZoomHints_impl;
tXIntersectRegion XIntersectRegion_impl;
tXConvertCase XConvertCase_impl;
tXLookupString XLookupString_impl;
tXMatchVisualInfo XMatchVisualInfo_impl;
tXOffsetRegion XOffsetRegion_impl;
tXPointInRegion XPointInRegion_impl;
tXPolygonRegion XPolygonRegion_impl;
tXRectInRegion XRectInRegion_impl;
tXSaveContext XSaveContext_impl;
tXSetClassHint XSetClassHint_impl;
tXSetIconSizes XSetIconSizes_impl;
tXSetNormalHints XSetNormalHints_impl;
tXSetRGBColormaps XSetRGBColormaps_impl;
tXSetSizeHints XSetSizeHints_impl;
tXSetStandardProperties XSetStandardProperties_impl;
tXSetTextProperty XSetTextProperty_impl;
tXSetWMClientMachine XSetWMClientMachine_impl;
tXSetWMHints XSetWMHints_impl;
tXSetWMIconName XSetWMIconName_impl;
tXSetWMName XSetWMName_impl;
tXSetWMNormalHints XSetWMNormalHints_impl;
tXSetWMProperties XSetWMProperties_impl;
tXmbSetWMProperties XmbSetWMProperties_impl;
tXutf8SetWMProperties Xutf8SetWMProperties_impl;
tXSetWMSizeHints XSetWMSizeHints_impl;
tXSetRegion XSetRegion_impl;
tXSetStandardColormap XSetStandardColormap_impl;
tXSetZoomHints XSetZoomHints_impl;
tXShrinkRegion XShrinkRegion_impl;
tXStringListToTextProperty XStringListToTextProperty_impl;
tXSubtractRegion XSubtractRegion_impl;
tXmbTextListToTextProperty XmbTextListToTextProperty_impl;
tXwcTextListToTextProperty XwcTextListToTextProperty_impl;
tXutf8TextListToTextProperty Xutf8TextListToTextProperty_impl;
tXwcFreeStringList XwcFreeStringList_impl;
tXTextPropertyToStringList XTextPropertyToStringList_impl;
tXmbTextPropertyToTextList XmbTextPropertyToTextList_impl;
tXwcTextPropertyToTextList XwcTextPropertyToTextList_impl;
tXutf8TextPropertyToTextList Xutf8TextPropertyToTextList_impl;
tXUnionRectWithRegion XUnionRectWithRegion_impl;
tXUnionRegion XUnionRegion_impl;
tXWMGeometry XWMGeometry_impl;
tXXorRegion XXorRegion_impl;

// Xcursor.h
tXcursorImageCreate XcursorImageCreate_impl;
tXcursorImageDestroy XcursorImageDestroy_impl;
tXcursorImagesCreate XcursorImagesCreate_impl;
tXcursorImagesDestroy XcursorImagesDestroy_impl;
tXcursorImagesSetName XcursorImagesSetName_impl;
tXcursorCursorsCreate XcursorCursorsCreate_impl;
tXcursorCursorsDestroy XcursorCursorsDestroy_impl;
tXcursorAnimateCreate XcursorAnimateCreate_impl;
tXcursorAnimateDestroy XcursorAnimateDestroy_impl;
tXcursorAnimateNext XcursorAnimateNext_impl;
tXcursorCommentCreate XcursorCommentCreate_impl;
tXcursorCommentDestroy XcursorCommentDestroy_impl;
tXcursorCommentsCreate XcursorCommentsCreate_impl;
tXcursorCommentsDestroy XcursorCommentsDestroy_impl;
tXcursorXcFileLoadImage XcursorXcFileLoadImage_impl;
tXcursorXcFileLoadImages XcursorXcFileLoadImages_impl;
tXcursorXcFileLoadAllImages XcursorXcFileLoadAllImages_impl;
tXcursorXcFileLoad XcursorXcFileLoad_impl;
tXcursorXcFileSave XcursorXcFileSave_impl;
tXcursorFileLoadImage XcursorFileLoadImage_impl;
tXcursorFileLoadImages XcursorFileLoadImages_impl;
tXcursorFileLoadAllImages XcursorFileLoadAllImages_impl;
tXcursorFileLoad XcursorFileLoad_impl;
tXcursorFileSaveImages XcursorFileSaveImages_impl;
tXcursorFileSave XcursorFileSave_impl;
tXcursorFilenameLoadImage XcursorFilenameLoadImage_impl;
tXcursorFilenameLoadImages XcursorFilenameLoadImages_impl;
tXcursorFilenameLoadAllImages XcursorFilenameLoadAllImages_impl;
tXcursorFilenameLoad XcursorFilenameLoad_impl;
tXcursorFilenameSaveImages XcursorFilenameSaveImages_impl;
tXcursorFilenameSave XcursorFilenameSave_impl;
tXcursorLibraryLoadImage XcursorLibraryLoadImage_impl;
tXcursorLibraryLoadImages XcursorLibraryLoadImages_impl;
tXcursorLibraryPath XcursorLibraryPath_impl;
tXcursorLibraryShape XcursorLibraryShape_impl;
tXcursorImageLoadCursor XcursorImageLoadCursor_impl;
tXcursorImagesLoadCursors XcursorImagesLoadCursors_impl;
tXcursorImagesLoadCursor XcursorImagesLoadCursor_impl;
tXcursorFilenameLoadCursor XcursorFilenameLoadCursor_impl;
tXcursorFilenameLoadCursors XcursorFilenameLoadCursors_impl;
tXcursorLibraryLoadCursor XcursorLibraryLoadCursor_impl;
tXcursorLibraryLoadCursors XcursorLibraryLoadCursors_impl;
tXcursorShapeLoadImage XcursorShapeLoadImage_impl;
tXcursorShapeLoadImages XcursorShapeLoadImages_impl;
tXcursorShapeLoadCursor XcursorShapeLoadCursor_impl;
tXcursorShapeLoadCursors XcursorShapeLoadCursors_impl;
tXcursorTryShapeCursor XcursorTryShapeCursor_impl;
tXcursorNoticeCreateBitmap XcursorNoticeCreateBitmap_impl;
tXcursorNoticePutBitmap XcursorNoticePutBitmap_impl;
tXcursorTryShapeBitmapCursor XcursorTryShapeBitmapCursor_impl;
tXcursorImageHash XcursorImageHash_impl;
tXcursorSupportsARGB XcursorSupportsARGB_impl;
tXcursorSupportsAnim XcursorSupportsAnim_impl;
tXcursorSetDefaultSize XcursorSetDefaultSize_impl;
tXcursorGetDefaultSize XcursorGetDefaultSize_impl;
tXcursorSetTheme XcursorSetTheme_impl;
tXcursorGetTheme XcursorGetTheme_impl;
tXcursorGetThemeCore XcursorGetThemeCore_impl;
tXcursorSetThemeCore XcursorSetThemeCore_impl;

// Xinerama.h
tXineramaQueryExtension XineramaQueryExtension_impl;
tXineramaQueryVersion XineramaQueryVersion_impl;
tXineramaIsActive XineramaIsActive_impl;
tXineramaQueryScreens XineramaQueryScreens_impl;


////////////////////////////////////////////////////////////////////////////////
// Function wrappers.

// Xlib.h
int _Xmblen(char* str, int len) {
  return _Xmblen_impl(str, len);
}

XFontStruct* XLoadQueryFont(Display* display, _Xconst char* name) {
  return XLoadQueryFont_impl(display, name);
}

XFontStruct* XQueryFont(Display* display, XID font_ID) {
  return XQueryFont_impl(display, font_ID);
}

XTimeCoord* XGetMotionEvents(Display* display, Window w, Time start, Time stop, int* nevents_return) {
  return XGetMotionEvents_impl(display, w, start, stop, nevents_return);
}

XModifierKeymap* XDeleteModifiermapEntry(
    XModifierKeymap* modmap,
#if NeedWidePrototypes
    unsigned int keycode_entry,
#else
    KeyCode keycode_entry,
#endif
    int modifier) {
  return XDeleteModifiermapEntry_impl(modmap, keycode_entry, modifier);
}

XModifierKeymap* XGetModifierMapping(Display* display) {
  return XGetModifierMapping_impl(display);
}

XModifierKeymap* XInsertModifiermapEntry(
    XModifierKeymap* modmap,
#if NeedWidePrototypes
    unsigned int keycode_entry,
#else
    KeyCode keycode_entry,
#endif
    int modifier) {
  return XInsertModifiermapEntry_impl(modmap, keycode_entry, modifier);
}

XModifierKeymap* XNewModifiermap(int max_keys_per_mod) {
  return XNewModifiermap_impl(max_keys_per_mod);
}

XImage* XCreateImage(
    Display* display,
    Visual* visual,
    unsigned int depth,
    int format,
    int offset,
    char* data,
    unsigned int width,
    unsigned int height,
    int bitmap_pad,
    int bytes_per_line) {
  return XCreateImage_impl(display, visual, depth, format, offset, data, width, height, bitmap_pad, bytes_per_line);
}

Status XInitImage(XImage* image) {
  return XInitImage_impl(image);
}

XImage* XGetImage(
    Display* display,
    Drawable d,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    unsigned long plane_mask,
    int format) {
  return XGetImage_impl(display, d, x, y, width, height, plane_mask, format);
}

XImage* XGetSubImage(
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
    int dest_y) {
  return XGetSubImage_impl(display, d, x, y, width, height, plane_mask, format, dest_image, dest_x, dest_y);
}

Display* XOpenDisplay(_Xconst char* display_name) {
  return XOpenDisplay_impl(display_name);
}

void XrmInitialize(void) {
  return XrmInitialize_impl();
}

char* XFetchBytes(Display* display, int* nbytes_return) {
  return XFetchBytes_impl(display, nbytes_return);
}

char* XFetchBuffer(Display* display, int* nbytes_return, int buffer) {
  return XFetchBuffer_impl(display, nbytes_return, buffer);
}

char* XGetAtomName(Display* display, Atom atom) {
  return XGetAtomName_impl(display, atom);
}

Status XGetAtomNames(Display* dpy, Atom* atoms, int count, char** names_return) {
  return XGetAtomNames_impl(dpy, atoms, count, names_return);
}

char* XGetDefault(Display* display, _Xconst char* program, _Xconst char* option) {
  return XGetDefault_impl(display, program, option);
}

char* XDisplayName(_Xconst char* string) {
  return XDisplayName_impl(string);
}

char* XKeysymToString(KeySym keysym) {
  return XKeysymToString_impl(keysym);
}

int (* XSynchronize(
    Display* display,
    Bool onoff))(Display* /* display */) {
  return XSynchronize_impl(display, onoff);
}

Atom XInternAtom(Display* display, _Xconst char* atom_name, Bool only_if_exists) {
  return XInternAtom_impl(display, atom_name, only_if_exists);
}

Status XInternAtoms(Display* dpy, char** names, int count, Bool onlyIfExists, Atom* atoms_return) {
  return XInternAtoms_impl(dpy, names, count, onlyIfExists, atoms_return);
}

Colormap XCopyColormapAndFree(Display* display, Colormap colormap) {
  return XCopyColormapAndFree_impl(display, colormap);
}

Colormap XCreateColormap(Display* display, Window w, Visual* visual, int alloc) {
  return XCreateColormap_impl(display, w, visual, alloc);
}

Cursor XCreatePixmapCursor(
    Display* display,
    Pixmap source,
    Pixmap mask,
    XColor* foreground_color,
    XColor* background_color,
    unsigned int x,
    unsigned int y) {
  return XCreatePixmapCursor_impl(display, source, mask, foreground_color, background_color, x, y);
}

Cursor XCreateGlyphCursor(
    Display* display,
    Font source_font,
    Font mask_font,
    unsigned int source_char,
    unsigned int mask_char,
    XColor _Xconst* foreground_color,
    XColor _Xconst* background_color) {
  return XCreateGlyphCursor_impl(display, source_font, mask_font, source_char, mask_char, foreground_color, background_color);
}

Cursor XCreateFontCursor(Display* display, unsigned int shape) {
  return XCreateFontCursor_impl(display, shape);
}

Font XLoadFont(Display* display, _Xconst char* name) {
  return XLoadFont_impl(display, name);
}

GC XCreateGC(Display* display, Drawable d, unsigned long valuemask, XGCValues* values) {
  return XCreateGC_impl(display, d, valuemask, values);
}

GContext XGContextFromGC(GC gc) {
  return XGContextFromGC_impl(gc);
}

void XFlushGC(Display* display, GC gc) {
  return XFlushGC_impl(display, gc);
}

Pixmap XCreatePixmap(Display* display, Drawable d, unsigned int width, unsigned int height, unsigned int depth) {
  return XCreatePixmap_impl(display, d, width, height, depth);
}

Pixmap XCreateBitmapFromData(Display* display, Drawable d, _Xconst char* data, unsigned int width, unsigned int height) {
  return XCreateBitmapFromData_impl(display, d, data, width, height);
}

Pixmap XCreatePixmapFromBitmapData(
    Display* display,
    Drawable d,
    char* data,
    unsigned int width,
    unsigned int height,
    unsigned long fg,
    unsigned long bg,
    unsigned int depth) {
  return XCreatePixmapFromBitmapData_impl(display, d, data, width, height, fg, bg, depth);
}

Window XCreateSimpleWindow(
    Display* display,
    Window parent,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    unsigned int border_width,
    unsigned long border,
    unsigned long background) {
  return XCreateSimpleWindow_impl(display, parent, x, y, width, height, border_width, border, background);
}

Window XGetSelectionOwner(Display* display, Atom selection) {
  return XGetSelectionOwner_impl(display, selection);
}

Window XCreateWindow(
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
    XSetWindowAttributes* attributes) {
  return XCreateWindow_impl(display, parent, x, y, width, height, border_width, depth, class_, visual, valuemask, attributes);
}

Colormap* XListInstalledColormaps(Display* display, Window w, int* num_return) {
  return XListInstalledColormaps_impl(display, w, num_return);
}

char** XListFonts(Display* display, _Xconst char* pattern, int maxnames, int* actual_count_return) {
  return XListFonts_impl(display, pattern, maxnames, actual_count_return);
}

char** XListFontsWithInfo(
    Display* display,
    _Xconst char* pattern,
    int maxnames,
    int* count_return,
    XFontStruct** info_return) {
  return XListFontsWithInfo_impl(display, pattern, maxnames, count_return, info_return);
}

char** XGetFontPath(Display* display, int* npaths_return) {
  return XGetFontPath_impl(display, npaths_return);
}

char** XListExtensions(Display* display, int* nextensions_return) {
  return XListExtensions_impl(display, nextensions_return);
}

Atom* XListProperties(Display* display, Window w, int* num_prop_return) {
  return XListProperties_impl(display, w, num_prop_return);
}

XHostAddress* XListHosts(Display* display, int* nhosts_return, Bool* state_return) {
  return XListHosts_impl(display, nhosts_return, state_return);
}

KeySym XKeycodeToKeysym(
    Display* display,
#if NeedWidePrototypes
    unsigned int keycode,
#else
    KeyCode keycode,
#endif
    int index) {
  return XKeycodeToKeysym_impl(display, keycode, index);
}

KeySym XLookupKeysym(XKeyEvent* key_event, int index) {
  return XLookupKeysym_impl(key_event, index);
}

KeySym* XGetKeyboardMapping(
    Display* display,
#if NeedWidePrototypes
    unsigned int first_keycode,
#else
    KeyCode first_keycode,
#endif
    int keycode_count,
    int* keysyms_per_keycode_return) {
  return XGetKeyboardMapping_impl(display, first_keycode, keycode_count, keysyms_per_keycode_return);
}

KeySym XStringToKeysym(_Xconst char* string) {
  return XStringToKeysym_impl(string);
}

long XMaxRequestSize(Display* display) {
  return XMaxRequestSize_impl(display);
}

long XExtendedMaxRequestSize(Display* display) {
  return XExtendedMaxRequestSize_impl(display);
}

char* XResourceManagerString(Display* display) {
  return XResourceManagerString_impl(display);
}

char* XScreenResourceString(Screen* screen) {
  return XScreenResourceString_impl(screen);
}

unsigned long XDisplayMotionBufferSize(Display* display) {
  return XDisplayMotionBufferSize_impl(display);
}

VisualID XVisualIDFromVisual(Visual* visual) {
  return XVisualIDFromVisual_impl(visual);
}

Status XInitThreads(void) {
  return XInitThreads_impl();
}

void XLockDisplay(Display* display) {
  return XLockDisplay_impl(display);
}

void XUnlockDisplay(Display* display) {
  return XUnlockDisplay_impl(display);
}

XExtCodes* XInitExtension(Display* display, _Xconst char* name) {
  return XInitExtension_impl(display, name);
}

XExtCodes* XAddExtension(Display* display) {
  return XAddExtension_impl(display);
}

XExtData* XFindOnExtensionList(XExtData** structure, int number) {
  return XFindOnExtensionList_impl(structure, number);
}

XExtData** XEHeadOfExtensionList(XEDataObject object) {
  return XEHeadOfExtensionList_impl(object);
}

Window XRootWindow(Display* display, int screen_number) {
  return XRootWindow_impl(display, screen_number);
}

Window XDefaultRootWindow(Display* display) {
  return XDefaultRootWindow_impl(display);
}

Window XRootWindowOfScreen(Screen* screen) {
  return XRootWindowOfScreen_impl(screen);
}

Visual* XDefaultVisual(Display* display, int screen_number) {
  return XDefaultVisual_impl(display, screen_number);
}

Visual* XDefaultVisualOfScreen(Screen* screen) {
  return XDefaultVisualOfScreen_impl(screen);
}

GC XDefaultGC(Display* display, int screen_number) {
  return XDefaultGC_impl(display, screen_number);
}

GC XDefaultGCOfScreen(Screen* screen) {
  return XDefaultGCOfScreen_impl(screen);
}

unsigned long XBlackPixel(Display* display, int screen_number) {
  return XBlackPixel_impl(display, screen_number);
}

unsigned long XWhitePixel(Display* display, int screen_number) {
  return XWhitePixel_impl(display, screen_number);
}

unsigned long XAllPlanes(void) {
  return XAllPlanes_impl();
}

unsigned long XBlackPixelOfScreen(Screen* screen) {
  return XBlackPixelOfScreen_impl(screen);
}

unsigned long XWhitePixelOfScreen(Screen* screen) {
  return XWhitePixelOfScreen_impl(screen);
}

unsigned long XNextRequest(Display* display) {
  return XNextRequest_impl(display);
}

unsigned long XLastKnownRequestProcessed(Display* display) {
  return XLastKnownRequestProcessed_impl(display);
}

char* XServerVendor(Display* display) {
  return XServerVendor_impl(display);
}

char* XDisplayString(Display* display) {
  return XDisplayString_impl(display);
}

Colormap XDefaultColormap(Display* display, int screen_number) {
  return XDefaultColormap_impl(display, screen_number);
}

Colormap XDefaultColormapOfScreen(Screen* screen) {
  return XDefaultColormapOfScreen_impl(screen);
}

Display* XDisplayOfScreen(Screen* screen) {
  return XDisplayOfScreen_impl(screen);
}

Screen* XScreenOfDisplay(Display* display, int screen_number) {
  return XScreenOfDisplay_impl(display, screen_number);
}

Screen* XDefaultScreenOfDisplay(Display* display) {
  return XDefaultScreenOfDisplay_impl(display);
}

long XEventMaskOfScreen(Screen* screen) {
  return XEventMaskOfScreen_impl(screen);
}

int XScreenNumberOfScreen(Screen* screen) {
  return XScreenNumberOfScreen_impl(screen);
}

XErrorHandler XSetErrorHandler(XErrorHandler handler) {
  return XSetErrorHandler_impl(handler);
}

XIOErrorHandler XSetIOErrorHandler(XIOErrorHandler handler) {
  return XSetIOErrorHandler_impl(handler);
}

XPixmapFormatValues* XListPixmapFormats(Display* display, int* count_return) {
  return XListPixmapFormats_impl(display, count_return);
}

int* XListDepths(Display* display, int screen_number, int* count_return) {
  return XListDepths_impl(display, screen_number, count_return);
}

Status XReconfigureWMWindow(Display* display, Window w, int screen_number, unsigned int mask, XWindowChanges* changes) {
  return XReconfigureWMWindow_impl(display, w, screen_number, mask, changes);
}

Status XGetWMProtocols(Display* display, Window w, Atom** protocols_return, int* count_return) {
  return XGetWMProtocols_impl(display, w, protocols_return, count_return);
}

Status XSetWMProtocols(Display* display, Window w, Atom* protocols, int count) {
  return XSetWMProtocols_impl(display, w, protocols, count);
}

Status XIconifyWindow(Display* display, Window w, int screen_number) {
  return XIconifyWindow_impl(display, w, screen_number);
}

Status XWithdrawWindow(Display* display, Window w, int screen_number) {
  return XWithdrawWindow_impl(display, w, screen_number);
}

Status XGetCommand(Display* display, Window w, char*** argv_return, int* argc_return) {
  return XGetCommand_impl(display, w, argv_return, argc_return);
}

Status XGetWMColormapWindows(Display* display, Window w, Window** windows_return, int* count_return) {
  return XGetWMColormapWindows_impl(display, w, windows_return, count_return);
}

Status XSetWMColormapWindows(Display* display, Window w, Window* colormap_windows, int count) {
  return XSetWMColormapWindows_impl(display, w, colormap_windows, count);
}

void XFreeStringList(char** list) {
  return XFreeStringList_impl(list);
}

int XSetTransientForHint(Display* display, Window w, Window prop_window) {
  return XSetTransientForHint_impl(display, w, prop_window);
}

int XActivateScreenSaver(Display* display) {
  return XActivateScreenSaver_impl(display);
}

int XAddHost(Display* display, XHostAddress* host) {
  return XAddHost_impl(display, host);
}

int XAddHosts(Display* display, XHostAddress* hosts, int num_hosts) {
  return XAddHosts_impl(display, hosts, num_hosts);
}

int XAddToExtensionList(struct _XExtData** structure, XExtData* ext_data) {
  return XAddToExtensionList_impl(structure, ext_data);
}

int XAddToSaveSet(Display* display, Window w) {
  return XAddToSaveSet_impl(display, w);
}

Status XAllocColor(Display* display, Colormap colormap, XColor* screen_in_out) {
  return XAllocColor_impl(display, colormap, screen_in_out);
}

Status XAllocColorCells(
    Display* display,
    Colormap colormap,
    Bool contig,
    unsigned long* plane_masks_return,
    unsigned int nplanes,
    unsigned long* pixels_return,
    unsigned int npixels) {
  return XAllocColorCells_impl(display, colormap, contig, plane_masks_return, nplanes, pixels_return, npixels);
}

Status XAllocColorPlanes(
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
    unsigned long* bmask_return) {
  return XAllocColorPlanes_impl(display, colormap, contig, pixels_return, ncolors, nreds, ngreens, nblues, rmask_return, gmask_return, bmask_return);
}

Status XAllocNamedColor(
    Display* display,
    Colormap colormap,
    _Xconst char* color_name,
    XColor* screen_def_return,
    XColor* exact_def_return) {
  return XAllocNamedColor_impl(display, colormap, color_name, screen_def_return, exact_def_return);
}

int XAllowEvents(Display* display, int event_mode, Time time) {
  return XAllowEvents_impl(display, event_mode, time);
}

int XAutoRepeatOff(Display* display) {
  return XAutoRepeatOff_impl(display);
}

int XAutoRepeatOn(Display* display) {
  return XAutoRepeatOn_impl(display);
}

int XBell(Display* display, int percent) {
  return XBell_impl(display, percent);
}

int XBitmapBitOrder(Display* display) {
  return XBitmapBitOrder_impl(display);
}

int XBitmapPad(Display* display) {
  return XBitmapPad_impl(display);
}

int XBitmapUnit(Display* display) {
  return XBitmapUnit_impl(display);
}

int XCellsOfScreen(Screen* screen) {
  return XCellsOfScreen_impl(screen);
}

int XChangeActivePointerGrab(Display* display, unsigned int event_mask, Cursor cursor, Time time) {
  return XChangeActivePointerGrab_impl(display, event_mask, cursor, time);
}

int XChangeGC(Display* display, GC gc, unsigned long valuemask, XGCValues* values) {
  return XChangeGC_impl(display, gc, valuemask, values);
}

int XChangeKeyboardControl(Display* display, unsigned long value_mask, XKeyboardControl* values) {
  return XChangeKeyboardControl_impl(display, value_mask, values);
}

int XChangeKeyboardMapping(
    Display* display,
    int first_keycode,
    int keysyms_per_keycode,
    KeySym* keysyms,
    int num_codes) {
  return XChangeKeyboardMapping_impl(display, first_keycode, keysyms_per_keycode, keysyms, num_codes);
}

int XChangePointerControl(
    Display* display,
    Bool do_accel,
    Bool do_threshold,
    int accel_numerator,
    int accel_denominator,
    int threshold) {
  return XChangePointerControl_impl(display, do_accel, do_threshold, accel_numerator, accel_denominator, threshold);
}

int XChangeProperty(
    Display* display,
    Window w,
    Atom property,
    Atom type,
    int format,
    int mode,
    _Xconst unsigned char* data,
    int nelements) {
  return XChangeProperty_impl(display, w, property, type, format, mode, data, nelements);
}

int XChangeSaveSet(Display* display, Window w, int change_mode) {
  return XChangeSaveSet_impl(display, w, change_mode);
}

int XChangeWindowAttributes(Display* display, Window w, unsigned long valuemask, XSetWindowAttributes* attributes) {
  return XChangeWindowAttributes_impl(display, w, valuemask, attributes);
}

Bool XCheckIfEvent(
    Display* display,
    XEvent* event_return,
    Bool (*predicate)(Display* /* display */, XEvent* /* event */, XPointer /* arg */),
    XPointer arg) {
  return XCheckIfEvent_impl(display, event_return, predicate, arg);
}

Bool XCheckMaskEvent(Display* display, long event_mask, XEvent* event_return) {
  return XCheckMaskEvent_impl(display, event_mask, event_return);
}

Bool XCheckTypedEvent(Display* display, int event_type, XEvent* event_return) {
  return XCheckTypedEvent_impl(display, event_type, event_return);
}

Bool XCheckTypedWindowEvent(Display* display, Window w, int event_type, XEvent* event_return) {
  return XCheckTypedWindowEvent_impl(display, w, event_type, event_return);
}

Bool XCheckWindowEvent(Display* display, Window w, long event_mask, XEvent* event_return) {
  return XCheckWindowEvent_impl(display, w, event_mask, event_return);
}

int XCirculateSubwindows(Display* display, Window w, int direction) {
  return XCirculateSubwindows_impl(display, w, direction);
}

int XCirculateSubwindowsDown(Display* display, Window w) {
  return XCirculateSubwindowsDown_impl(display, w);
}

int XCirculateSubwindowsUp(Display* display, Window w) {
  return XCirculateSubwindowsUp_impl(display, w);
}

int XClearArea(
    Display* display,
    Window w,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    Bool exposures) {
  return XClearArea_impl(display, w, x, y, width, height, exposures);
}

int XClearWindow(Display* display, Window w) {
  return XClearWindow_impl(display, w);
}

int XCloseDisplay(Display* display) {
  return XCloseDisplay_impl(display);
}

int XConfigureWindow(Display* display, Window w, unsigned int value_mask, XWindowChanges* values) {
  return XConfigureWindow_impl(display, w, value_mask, values);
}

int XConnectionNumber(Display* display) {
  return XConnectionNumber_impl(display);
}

int XConvertSelection(Display* display, Atom selection, Atom target, Atom property, Window requestor, Time time) {
  return XConvertSelection_impl(display, selection, target, property, requestor, time);
}

int XCopyArea(
    Display* display,
    Drawable src,
    Drawable dest,
    GC gc,
    int src_x,
    int src_y,
    unsigned int width,
    unsigned int height,
    int dest_x,
    int dest_y) {
  return XCopyArea_impl(display, src, dest, gc, src_x, src_y, width, height, dest_x, dest_y);
}

int XCopyGC(Display* display, GC src, unsigned long valuemask, GC dest) {
  return XCopyGC_impl(display, src, valuemask, dest);
}

int XCopyPlane(
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
    unsigned long plane) {
  return XCopyPlane_impl(display, src, dest, gc, src_x, src_y, width, height, dest_x, dest_y, plane);
}

int XDefaultDepth(Display* display, int screen_number) {
  return XDefaultDepth_impl(display, screen_number);
}

int XDefaultDepthOfScreen(Screen* screen) {
  return XDefaultDepthOfScreen_impl(screen);
}

int XDefaultScreen(Display* display) {
  return XDefaultScreen_impl(display);
}

int XDefineCursor(Display* display, Window w, Cursor cursor) {
  return XDefineCursor_impl(display, w, cursor);
}

int XDeleteProperty(Display* display, Window w, Atom property) {
  return XDeleteProperty_impl(display, w, property);
}

int XDestroyWindow(Display* display, Window w) {
  return XDestroyWindow_impl(display, w);
}

int XDestroySubwindows(Display* display, Window w) {
  return XDestroySubwindows_impl(display, w);
}

int XDoesBackingStore(Screen* screen) {
  return XDoesBackingStore_impl(screen);
}

Bool XDoesSaveUnders(Screen* screen) {
  return XDoesSaveUnders_impl(screen);
}

int XDisableAccessControl(Display* display) {
  return XDisableAccessControl_impl(display);
}

int XDisplayCells(Display* display, int screen_number) {
  return XDisplayCells_impl(display, screen_number);
}

int XDisplayHeight(Display* display, int screen_number) {
  return XDisplayHeight_impl(display, screen_number);
}

int XDisplayHeightMM(Display* display, int screen_number) {
  return XDisplayHeightMM_impl(display, screen_number);
}

int XDisplayKeycodes(Display* display, int* min_keycodes_return, int* max_keycodes_return) {
  return XDisplayKeycodes_impl(display, min_keycodes_return, max_keycodes_return);
}

int XDisplayPlanes(Display* display, int screen_number) {
  return XDisplayPlanes_impl(display, screen_number);
}

int XDisplayWidth(Display* display, int screen_number) {
  return XDisplayWidth_impl(display, screen_number);
}

int XDisplayWidthMM(Display* display, int screen_number) {
  return XDisplayWidthMM_impl(display, screen_number);
}

int XDrawArc(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    int angle1,
    int angle2) {
  return XDrawArc_impl(display, d, gc, x, y, width, height, angle1, angle2);
}

int XDrawArcs(Display* display, Drawable d, GC gc, XArc* arcs, int narcs) {
  return XDrawArcs_impl(display, d, gc, arcs, narcs);
}

int XDrawImageString(Display* display, Drawable d, GC gc, int x, int y, _Xconst char* string, int length) {
  return XDrawImageString_impl(display, d, gc, x, y, string, length);
}

int XDrawImageString16(Display* display, Drawable d, GC gc, int x, int y, _Xconst XChar2b* string, int length) {
  return XDrawImageString16_impl(display, d, gc, x, y, string, length);
}

int XDrawLine(Display* display, Drawable d, GC gc, int x1, int y1, int x2, int y2) {
  return XDrawLine_impl(display, d, gc, x1, y1, x2, y2);
}

int XDrawLines(Display* display, Drawable d, GC gc, XPoint* points, int npoints, int mode) {
  return XDrawLines_impl(display, d, gc, points, npoints, mode);
}

int XDrawPoint(Display* display, Drawable d, GC gc, int x, int y) {
  return XDrawPoint_impl(display, d, gc, x, y);
}

int XDrawPoints(Display* display, Drawable d, GC gc, XPoint* points, int npoints, int mode) {
  return XDrawPoints_impl(display, d, gc, points, npoints, mode);
}

int XDrawRectangle(Display* display, Drawable d, GC gc, int x, int y, unsigned int width, unsigned int height) {
  return XDrawRectangle_impl(display, d, gc, x, y, width, height);
}

int XDrawRectangles(Display* display, Drawable d, GC gc, XRectangle* rectangles, int nrectangles) {
  return XDrawRectangles_impl(display, d, gc, rectangles, nrectangles);
}

int XDrawSegments(Display* display, Drawable d, GC gc, XSegment* segments, int nsegments) {
  return XDrawSegments_impl(display, d, gc, segments, nsegments);
}

int XDrawString(Display* display, Drawable d, GC gc, int x, int y, _Xconst char* string, int length) {
  return XDrawString_impl(display, d, gc, x, y, string, length);
}

int XDrawString16(Display* display, Drawable d, GC gc, int x, int y, _Xconst XChar2b* string, int length) {
  return XDrawString16_impl(display, d, gc, x, y, string, length);
}

int XDrawText(Display* display, Drawable d, GC gc, int x, int y, XTextItem* items, int nitems) {
  return XDrawText_impl(display, d, gc, x, y, items, nitems);
}

int XDrawText16(Display* display, Drawable d, GC gc, int x, int y, XTextItem16* items, int nitems) {
  return XDrawText16_impl(display, d, gc, x, y, items, nitems);
}

int XEnableAccessControl(Display* display) {
  return XEnableAccessControl_impl(display);
}

int XEventsQueued(Display* display, int mode) {
  return XEventsQueued_impl(display, mode);
}

Status XFetchName(Display* display, Window w, char** window_name_return) {
  return XFetchName_impl(display, w, window_name_return);
}

int XFillArc(
    Display* display,
    Drawable d,
    GC gc,
    int x,
    int y,
    unsigned int width,
    unsigned int height,
    int angle1,
    int angle2) {
  return XFillArc_impl(display, d, gc, x, y, width, height, angle1, angle2);
}

int XFillArcs(Display* display, Drawable d, GC gc, XArc* arcs, int narcs) {
  return XFillArcs_impl(display, d, gc, arcs, narcs);
}

int XFillPolygon(Display* display, Drawable d, GC gc, XPoint* points, int npoints, int shape, int mode) {
  return XFillPolygon_impl(display, d, gc, points, npoints, shape, mode);
}

int XFillRectangle(Display* display, Drawable d, GC gc, int x, int y, unsigned int width, unsigned int height) {
  return XFillRectangle_impl(display, d, gc, x, y, width, height);
}

int XFillRectangles(Display* display, Drawable d, GC gc, XRectangle* rectangles, int nrectangles) {
  return XFillRectangles_impl(display, d, gc, rectangles, nrectangles);
}

int XFlush(Display* display) {
  return XFlush_impl(display);
}

int XForceScreenSaver(Display* display, int mode) {
  return XForceScreenSaver_impl(display, mode);
}

int XFree(void* data) {
  return XFree_impl(data);
}

int XFreeColormap(Display* display, Colormap colormap) {
  return XFreeColormap_impl(display, colormap);
}

int XFreeColors(
    Display* display,
    Colormap colormap,
    unsigned long* pixels,
    int npixels,
    unsigned long planes) {
  return XFreeColors_impl(display, colormap, pixels, npixels, planes);
}

int XFreeCursor(Display* display, Cursor cursor) {
  return XFreeCursor_impl(display, cursor);
}

int XFreeExtensionList(char** list) {
  return XFreeExtensionList_impl(list);
}

int XFreeFont(Display* display, XFontStruct* font_struct) {
  return XFreeFont_impl(display, font_struct);
}

int XFreeFontInfo(char** names, XFontStruct* free_info, int actual_count) {
  return XFreeFontInfo_impl(names, free_info, actual_count);
}

int XFreeFontNames(char** list) {
  return XFreeFontNames_impl(list);
}

int XFreeFontPath(char** list) {
  return XFreeFontPath_impl(list);
}

int XFreeGC(Display* display, GC gc) {
  return XFreeGC_impl(display, gc);
}

int XFreeModifiermap(XModifierKeymap* modmap) {
  return XFreeModifiermap_impl(modmap);
}

int XFreePixmap(Display* display, Pixmap pixmap) {
  return XFreePixmap_impl(display, pixmap);
}

int XGeometry(
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
    int* height_return) {
  return XGeometry_impl(display, screen, position, default_position, bwidth, fwidth, fheight, xadder, yadder, x_return, y_return, width_return, height_return);
}

int XGetErrorDatabaseText(
    Display* display,
    _Xconst char* name,
    _Xconst char* message,
    _Xconst char* default_string,
    char* buffer_return,
    int length) {
  return XGetErrorDatabaseText_impl(display, name, message, default_string, buffer_return, length);
}

int XGetErrorText(Display* display, int code, char* buffer_return, int length) {
  return XGetErrorText_impl(display, code, buffer_return, length);
}

Bool XGetFontProperty(XFontStruct* font_struct, Atom atom, unsigned long* value_return) {
  return XGetFontProperty_impl(font_struct, atom, value_return);
}

Status XGetGCValues(Display* display, GC gc, unsigned long valuemask, XGCValues* values_return) {
  return XGetGCValues_impl(display, gc, valuemask, values_return);
}

Status XGetGeometry(
    Display* display,
    Drawable d,
    Window* root_return,
    int* x_return,
    int* y_return,
    unsigned int* width_return,
    unsigned int* height_return,
    unsigned int* border_width_return,
    unsigned int* depth_return) {
  return XGetGeometry_impl(display, d, root_return, x_return, y_return, width_return, height_return, border_width_return, depth_return);
}

Status XGetIconName(Display* display, Window w, char** icon_name_return) {
  return XGetIconName_impl(display, w, icon_name_return);
}

int XGetInputFocus(Display* display, Window* focus_return, int* revert_to_return) {
  return XGetInputFocus_impl(display, focus_return, revert_to_return);
}

int XGetKeyboardControl(Display* display, XKeyboardState* values_return) {
  return XGetKeyboardControl_impl(display, values_return);
}

int XGetPointerControl(
    Display* display,
    int* accel_numerator_return,
    int* accel_denominator_return,
    int* threshold_return) {
  return XGetPointerControl_impl(display, accel_numerator_return, accel_denominator_return, threshold_return);
}

int XGetPointerMapping(Display* display, unsigned char* map_return, int nmap) {
  return XGetPointerMapping_impl(display, map_return, nmap);
}

int XGetScreenSaver(
    Display* display,
    int* timeout_return,
    int* interval_return,
    int* prefer_blanking_return,
    int* allow_exposures_return) {
  return XGetScreenSaver_impl(display, timeout_return, interval_return, prefer_blanking_return, allow_exposures_return);
}

Status XGetTransientForHint(Display* display, Window w, Window* prop_window_return) {
  return XGetTransientForHint_impl(display, w, prop_window_return);
}

int XGetWindowProperty(
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
    unsigned char** prop_return) {
  return XGetWindowProperty_impl(display, w, property, long_offset, long_length, delete_, req_type, actual_type_return, actual_format_return, nitems_return, bytes_after_return, prop_return);
}

Status XGetWindowAttributes(Display* display, Window w, XWindowAttributes* window_attributes_return) {
  return XGetWindowAttributes_impl(display, w, window_attributes_return);
}

int XGrabButton(
    Display* display,
    unsigned int button,
    unsigned int modifiers,
    Window grab_window,
    Bool owner_events,
    unsigned int event_mask,
    int pointer_mode,
    int keyboard_mode,
    Window confine_to,
    Cursor cursor) {
  return XGrabButton_impl(display, button, modifiers, grab_window, owner_events, event_mask, pointer_mode, keyboard_mode, confine_to, cursor);
}

int XGrabKey(
    Display* display,
    int keycode,
    unsigned int modifiers,
    Window grab_window,
    Bool owner_events,
    int pointer_mode,
    int keyboard_mode) {
  return XGrabKey_impl(display, keycode, modifiers, grab_window, owner_events, pointer_mode, keyboard_mode);
}

int XGrabKeyboard(
    Display* display,
    Window grab_window,
    Bool owner_events,
    int pointer_mode,
    int keyboard_mode,
    Time time) {
  return XGrabKeyboard_impl(display, grab_window, owner_events, pointer_mode, keyboard_mode, time);
}

int XGrabPointer(
    Display* display,
    Window grab_window,
    Bool owner_events,
    unsigned int event_mask,
    int pointer_mode,
    int keyboard_mode,
    Window confine_to,
    Cursor cursor,
    Time time) {
  return XGrabPointer_impl(display, grab_window, owner_events, event_mask, pointer_mode, keyboard_mode, confine_to, cursor, time);
}

int XGrabServer(Display* display) {
  return XGrabServer_impl(display);
}

int XHeightMMOfScreen(Screen* screen) {
  return XHeightMMOfScreen_impl(screen);
}

int XHeightOfScreen(Screen* screen) {
  return XHeightOfScreen_impl(screen);
}

int XIfEvent(
    Display* display,
    XEvent* event_return,
    Bool (*predicate)(Display* /* display */, XEvent* /* event */, XPointer /* arg */),
    XPointer arg) {
  return XIfEvent_impl(display, event_return, predicate, arg);
}

int XImageByteOrder(Display* display) {
  return XImageByteOrder_impl(display);
}

int XInstallColormap(Display* display, Colormap colormap) {
  return XInstallColormap_impl(display, colormap);
}

KeyCode XKeysymToKeycode(Display* display, KeySym keysym) {
  return XKeysymToKeycode_impl(display, keysym);
}

int XKillClient(Display* display, XID resource) {
  return XKillClient_impl(display, resource);
}

Status XLookupColor(
    Display* display,
    Colormap colormap,
    _Xconst char* color_name,
    XColor* exact_def_return,
    XColor* screen_def_return) {
  return XLookupColor_impl(display, colormap, color_name, exact_def_return, screen_def_return);
}

int XLowerWindow(Display* display, Window w) {
  return XLowerWindow_impl(display, w);
}

int XMapRaised(Display* display, Window w) {
  return XMapRaised_impl(display, w);
}

int XMapSubwindows(Display* display, Window w) {
  return XMapSubwindows_impl(display, w);
}

int XMapWindow(Display* display, Window w) {
  return XMapWindow_impl(display, w);
}

int XMaskEvent(Display* display, long event_mask, XEvent* event_return) {
  return XMaskEvent_impl(display, event_mask, event_return);
}

int XMaxCmapsOfScreen(Screen* screen) {
  return XMaxCmapsOfScreen_impl(screen);
}

int XMinCmapsOfScreen(Screen* screen) {
  return XMinCmapsOfScreen_impl(screen);
}

int XMoveResizeWindow(Display* display, Window w, int x, int y, unsigned int width, unsigned int height) {
  return XMoveResizeWindow_impl(display, w, x, y, width, height);
}

int XMoveWindow(Display* display, Window w, int x, int y) {
  return XMoveWindow_impl(display, w, x, y);
}

int XNextEvent(Display* display, XEvent* event_return) {
  return XNextEvent_impl(display, event_return);
}

int XNoOp(Display* display) {
  return XNoOp_impl(display);
}

Status XParseColor(Display* display, Colormap colormap, _Xconst char* spec, XColor* exact_def_return) {
  return XParseColor_impl(display, colormap, spec, exact_def_return);
}

int XParseGeometry(
    _Xconst char* parsestring,
    int* x_return,
    int* y_return,
    unsigned int* width_return,
    unsigned int* height_return) {
  return XParseGeometry_impl(parsestring, x_return, y_return, width_return, height_return);
}

int XPeekEvent(Display* display, XEvent* event_return) {
  return XPeekEvent_impl(display, event_return);
}

int XPeekIfEvent(
    Display* display,
    XEvent* event_return,
    Bool (*predicate)(Display* /* display */, XEvent* /* event */, XPointer /* arg */),
    XPointer arg) {
  return XPeekIfEvent_impl(display, event_return, predicate, arg);
}

int XPending(Display* display) {
  return XPending_impl(display);
}

int XPlanesOfScreen(Screen* screen) {
  return XPlanesOfScreen_impl(screen);
}

int XProtocolRevision(Display* display) {
  return XProtocolRevision_impl(display);
}

int XProtocolVersion(Display* display) {
  return XProtocolVersion_impl(display);
}

int XPutBackEvent(Display* display, XEvent* event) {
  return XPutBackEvent_impl(display, event);
}

int XPutImage(
    Display* display,
    Drawable d,
    GC gc,
    XImage* image,
    int src_x,
    int src_y,
    int dest_x,
    int dest_y,
    unsigned int width,
    unsigned int height) {
  return XPutImage_impl(display, d, gc, image, src_x, src_y, dest_x, dest_y, width, height);
}

int XQLength(Display* display) {
  return XQLength_impl(display);
}

Status XQueryBestCursor(
    Display* display,
    Drawable d,
    unsigned int width,
    unsigned int height,
    unsigned int* width_return,
    unsigned int* height_return) {
  return XQueryBestCursor_impl(display, d, width, height, width_return, height_return);
}

Status XQueryBestSize(
    Display* display,
    int class_,
    Drawable which_screen,
    unsigned int width,
    unsigned int height,
    unsigned int* width_return,
    unsigned int* height_return) {
  return XQueryBestSize_impl(display, class_, which_screen, width, height, width_return, height_return);
}

Status XQueryBestStipple(
    Display* display,
    Drawable which_screen,
    unsigned int width,
    unsigned int height,
    unsigned int* width_return,
    unsigned int* height_return) {
  return XQueryBestStipple_impl(display, which_screen, width, height, width_return, height_return);
}

Status XQueryBestTile(
    Display* display,
    Drawable which_screen,
    unsigned int width,
    unsigned int height,
    unsigned int* width_return,
    unsigned int* height_return) {
  return XQueryBestTile_impl(display, which_screen, width, height, width_return, height_return);
}

int XQueryColor(Display* display, Colormap colormap, XColor* def_in_out) {
  return XQueryColor_impl(display, colormap, def_in_out);
}

int XQueryColors(Display* display, Colormap colormap, XColor* defs_in_out, int ncolors) {
  return XQueryColors_impl(display, colormap, defs_in_out, ncolors);
}

Bool XQueryExtension(
    Display* display,
    _Xconst char* name,
    int* major_opcode_return,
    int* first_event_return,
    int* first_error_return) {
  return XQueryExtension_impl(display, name, major_opcode_return, first_event_return, first_error_return);
}

int XQueryKeymap(Display* display, char keys_return[32]) {
  return XQueryKeymap_impl(display, keys_return);
}

Bool XQueryPointer(
    Display* display,
    Window w,
    Window* root_return,
    Window* child_return,
    int* root_x_return,
    int* root_y_return,
    int* win_x_return,
    int* win_y_return,
    unsigned int* mask_return) {
  return XQueryPointer_impl(display, w, root_return, child_return, root_x_return, root_y_return, win_x_return, win_y_return, mask_return);
}

int XQueryTextExtents(
    Display* display,
    XID font_ID,
    _Xconst char* string,
    int nchars,
    int* direction_return,
    int* font_ascent_return,
    int* font_descent_return,
    XCharStruct* overall_return) {
  return XQueryTextExtents_impl(display, font_ID, string, nchars, direction_return, font_ascent_return, font_descent_return, overall_return);
}

int XQueryTextExtents16(
    Display* display,
    XID font_ID,
    _Xconst XChar2b* string,
    int nchars,
    int* direction_return,
    int* font_ascent_return,
    int* font_descent_return,
    XCharStruct* overall_return) {
  return XQueryTextExtents16_impl(display, font_ID, string, nchars, direction_return, font_ascent_return, font_descent_return, overall_return);
}

Status XQueryTree(
    Display* display,
    Window w,
    Window* root_return,
    Window* parent_return,
    Window** children_return,
    unsigned int* nchildren_return) {
  return XQueryTree_impl(display, w, root_return, parent_return, children_return, nchildren_return);
}

int XRaiseWindow(Display* display, Window w) {
  return XRaiseWindow_impl(display, w);
}

int XReadBitmapFile(
    Display* display,
    Drawable d,
    _Xconst char* filename,
    unsigned int* width_return,
    unsigned int* height_return,
    Pixmap* bitmap_return,
    int* x_hot_return,
    int* y_hot_return) {
  return XReadBitmapFile_impl(display, d, filename, width_return, height_return, bitmap_return, x_hot_return, y_hot_return);
}

int XReadBitmapFileData(
    _Xconst char* filename,
    unsigned int* width_return,
    unsigned int* height_return,
    unsigned char** data_return,
    int* x_hot_return,
    int* y_hot_return) {
  return XReadBitmapFileData_impl(filename, width_return, height_return, data_return, x_hot_return, y_hot_return);
}

int XRebindKeysym(
    Display* display,
    KeySym keysym,
    KeySym* list,
    int mod_count,
    _Xconst unsigned char* string,
    int bytes_string) {
  return XRebindKeysym_impl(display, keysym, list, mod_count, string, bytes_string);
}

int XRecolorCursor(Display* display, Cursor cursor, XColor* foreground_color, XColor* background_color) {
  return XRecolorCursor_impl(display, cursor, foreground_color, background_color);
}

int XRefreshKeyboardMapping(XMappingEvent* event_map) {
  return XRefreshKeyboardMapping_impl(event_map);
}

int XRemoveFromSaveSet(Display* display, Window w) {
  return XRemoveFromSaveSet_impl(display, w);
}

int XRemoveHost(Display* display, XHostAddress* host) {
  return XRemoveHost_impl(display, host);
}

int XRemoveHosts(Display* display, XHostAddress* hosts, int num_hosts) {
  return XRemoveHosts_impl(display, hosts, num_hosts);
}

int XReparentWindow(Display* display, Window w, Window parent, int x, int y) {
  return XReparentWindow_impl(display, w, parent, x, y);
}

int XResetScreenSaver(Display* display) {
  return XResetScreenSaver_impl(display);
}

int XResizeWindow(Display* display, Window w, unsigned int width, unsigned int height) {
  return XResizeWindow_impl(display, w, width, height);
}

int XRestackWindows(Display* display, Window* windows, int nwindows) {
  return XRestackWindows_impl(display, windows, nwindows);
}

int XRotateBuffers(Display* display, int rotate) {
  return XRotateBuffers_impl(display, rotate);
}

int XRotateWindowProperties(Display* display, Window w, Atom* properties, int num_prop, int npositions) {
  return XRotateWindowProperties_impl(display, w, properties, num_prop, npositions);
}

int XScreenCount(Display* display) {
  return XScreenCount_impl(display);
}

int XSelectInput(Display* display, Window w, long event_mask) {
  return XSelectInput_impl(display, w, event_mask);
}

Status XSendEvent(Display* display, Window w, Bool propagate, long event_mask, XEvent* event_send) {
  return XSendEvent_impl(display, w, propagate, event_mask, event_send);
}

int XSetAccessControl(Display* display, int mode) {
  return XSetAccessControl_impl(display, mode);
}

int XSetArcMode(Display* display, GC gc, int arc_mode) {
  return XSetArcMode_impl(display, gc, arc_mode);
}

int XSetBackground(Display* display, GC gc, unsigned long background) {
  return XSetBackground_impl(display, gc, background);
}

int XSetClipMask(Display* display, GC gc, Pixmap pixmap) {
  return XSetClipMask_impl(display, gc, pixmap);
}

int XSetClipOrigin(Display* display, GC gc, int clip_x_origin, int clip_y_origin) {
  return XSetClipOrigin_impl(display, gc, clip_x_origin, clip_y_origin);
}

int XSetClipRectangles(
    Display* display,
    GC gc,
    int clip_x_origin,
    int clip_y_origin,
    XRectangle* rectangles,
    int n,
    int ordering) {
  return XSetClipRectangles_impl(display, gc, clip_x_origin, clip_y_origin, rectangles, n, ordering);
}

int XSetCloseDownMode(Display* display, int close_mode) {
  return XSetCloseDownMode_impl(display, close_mode);
}

int XSetCommand(Display* display, Window w, char** argv, int argc) {
  return XSetCommand_impl(display, w, argv, argc);
}

int XSetDashes(Display* display, GC gc, int dash_offset, _Xconst char* dash_list, int n) {
  return XSetDashes_impl(display, gc, dash_offset, dash_list, n);
}

int XSetFillRule(Display* display, GC gc, int fill_rule) {
  return XSetFillRule_impl(display, gc, fill_rule);
}

int XSetFillStyle(Display* display, GC gc, int fill_style) {
  return XSetFillStyle_impl(display, gc, fill_style);
}

int XSetFont(Display* display, GC gc, Font font) {
  return XSetFont_impl(display, gc, font);
}

int XSetFontPath(Display* display, char** directories, int ndirs) {
  return XSetFontPath_impl(display, directories, ndirs);
}

int XSetForeground(Display* display, GC gc, unsigned long foreground) {
  return XSetForeground_impl(display, gc, foreground);
}

int XSetFunction(Display* display, GC gc, int function) {
  return XSetFunction_impl(display, gc, function);
}

int XSetGraphicsExposures(Display* display, GC gc, Bool graphics_exposures) {
  return XSetGraphicsExposures_impl(display, gc, graphics_exposures);
}

int XSetIconName(Display* display, Window w, _Xconst char* icon_name) {
  return XSetIconName_impl(display, w, icon_name);
}

int XSetInputFocus(Display* display, Window focus, int revert_to, Time time) {
  return XSetInputFocus_impl(display, focus, revert_to, time);
}

int XSetLineAttributes(
    Display* display,
    GC gc,
    unsigned int line_width,
    int line_style,
    int cap_style,
    int join_style) {
  return XSetLineAttributes_impl(display, gc, line_width, line_style, cap_style, join_style);
}

int XSetModifierMapping(Display* display, XModifierKeymap* modmap) {
  return XSetModifierMapping_impl(display, modmap);
}

int XSetPlaneMask(Display* display, GC gc, unsigned long plane_mask) {
  return XSetPlaneMask_impl(display, gc, plane_mask);
}

int XSetPointerMapping(Display* display, _Xconst unsigned char* map, int nmap) {
  return XSetPointerMapping_impl(display, map, nmap);
}

int XSetScreenSaver(Display* display, int timeout, int interval, int prefer_blanking, int allow_exposures) {
  return XSetScreenSaver_impl(display, timeout, interval, prefer_blanking, allow_exposures);
}

int XSetSelectionOwner(Display* display, Atom selection, Window owner, Time time) {
  return XSetSelectionOwner_impl(display, selection, owner, time);
}

int XSetState(
    Display* display,
    GC gc,
    unsigned long foreground,
    unsigned long background,
    int function,
    unsigned long plane_mask) {
  return XSetState_impl(display, gc, foreground, background, function, plane_mask);
}

int XSetStipple(Display* display, GC gc, Pixmap stipple) {
  return XSetStipple_impl(display, gc, stipple);
}

int XSetSubwindowMode(Display* display, GC gc, int subwindow_mode) {
  return XSetSubwindowMode_impl(display, gc, subwindow_mode);
}

int XSetTSOrigin(Display* display, GC gc, int ts_x_origin, int ts_y_origin) {
  return XSetTSOrigin_impl(display, gc, ts_x_origin, ts_y_origin);
}

int XSetTile(Display* display, GC gc, Pixmap tile) {
  return XSetTile_impl(display, gc, tile);
}

int XSetWindowBackground(Display* display, Window w, unsigned long background_pixel) {
  return XSetWindowBackground_impl(display, w, background_pixel);
}

int XSetWindowBackgroundPixmap(Display* display, Window w, Pixmap background_pixmap) {
  return XSetWindowBackgroundPixmap_impl(display, w, background_pixmap);
}

int XSetWindowBorder(Display* display, Window w, unsigned long border_pixel) {
  return XSetWindowBorder_impl(display, w, border_pixel);
}

int XSetWindowBorderPixmap(Display* display, Window w, Pixmap border_pixmap) {
  return XSetWindowBorderPixmap_impl(display, w, border_pixmap);
}

int XSetWindowBorderWidth(Display* display, Window w, unsigned int width) {
  return XSetWindowBorderWidth_impl(display, w, width);
}

int XSetWindowColormap(Display* display, Window w, Colormap colormap) {
  return XSetWindowColormap_impl(display, w, colormap);
}

int XStoreBuffer(Display* display, _Xconst char* bytes, int nbytes, int buffer) {
  return XStoreBuffer_impl(display, bytes, nbytes, buffer);
}

int XStoreBytes(Display* display, _Xconst char* bytes, int nbytes) {
  return XStoreBytes_impl(display, bytes, nbytes);
}

int XStoreColor(Display* display, Colormap colormap, XColor* color) {
  return XStoreColor_impl(display, colormap, color);
}

int XStoreColors(Display* display, Colormap colormap, XColor* color, int ncolors) {
  return XStoreColors_impl(display, colormap, color, ncolors);
}

int XStoreName(Display* display, Window w, _Xconst char* window_name) {
  return XStoreName_impl(display, w, window_name);
}

int XStoreNamedColor(Display* display, Colormap colormap, _Xconst char* color, unsigned long pixel, int flags) {
  return XStoreNamedColor_impl(display, colormap, color, pixel, flags);
}

int XSync(Display* display, Bool discard) {
  return XSync_impl(display, discard);
}

int XTextExtents(
    XFontStruct* font_struct,
    _Xconst char* string,
    int nchars,
    int* direction_return,
    int* font_ascent_return,
    int* font_descent_return,
    XCharStruct* overall_return) {
  return XTextExtents_impl(font_struct, string, nchars, direction_return, font_ascent_return, font_descent_return, overall_return);
}

int XTextExtents16(
    XFontStruct* font_struct,
    _Xconst XChar2b* string,
    int nchars,
    int* direction_return,
    int* font_ascent_return,
    int* font_descent_return,
    XCharStruct* overall_return) {
  return XTextExtents16_impl(font_struct, string, nchars, direction_return, font_ascent_return, font_descent_return, overall_return);
}

int XTextWidth(XFontStruct* font_struct, _Xconst char* string, int count) {
  return XTextWidth_impl(font_struct, string, count);
}

int XTextWidth16(XFontStruct* font_struct, _Xconst XChar2b* string, int count) {
  return XTextWidth16_impl(font_struct, string, count);
}

Bool XTranslateCoordinates(
    Display* display,
    Window src_w,
    Window dest_w,
    int src_x,
    int src_y,
    int* dest_x_return,
    int* dest_y_return,
    Window* child_return) {
  return XTranslateCoordinates_impl(display, src_w, dest_w, src_x, src_y, dest_x_return, dest_y_return, child_return);
}

int XUndefineCursor(Display* display, Window w) {
  return XUndefineCursor_impl(display, w);
}

int XUngrabButton(Display* display, unsigned int button, unsigned int modifiers, Window grab_window) {
  return XUngrabButton_impl(display, button, modifiers, grab_window);
}

int XUngrabKey(Display* display, int keycode, unsigned int modifiers, Window grab_window) {
  return XUngrabKey_impl(display, keycode, modifiers, grab_window);
}

int XUngrabKeyboard(Display* display, Time time) {
  return XUngrabKeyboard_impl(display, time);
}

int XUngrabPointer(Display* display, Time time) {
  return XUngrabPointer_impl(display, time);
}

int XUngrabServer(Display* display) {
  return XUngrabServer_impl(display);
}

int XUninstallColormap(Display* display, Colormap colormap) {
  return XUninstallColormap_impl(display, colormap);
}

int XUnloadFont(Display* display, Font font) {
  return XUnloadFont_impl(display, font);
}

int XUnmapSubwindows(Display* display, Window w) {
  return XUnmapSubwindows_impl(display, w);
}

int XUnmapWindow(Display* display, Window w) {
  return XUnmapWindow_impl(display, w);
}

int XVendorRelease(Display* display) {
  return XVendorRelease_impl(display);
}

int XWarpPointer(
    Display* display,
    Window src_w,
    Window dest_w,
    int src_x,
    int src_y,
    unsigned int src_width,
    unsigned int src_height,
    int dest_x,
    int dest_y) {
  return XWarpPointer_impl(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x, dest_y);
}

int XWidthMMOfScreen(Screen* screen) {
  return XWidthMMOfScreen_impl(screen);
}

int XWidthOfScreen(Screen* screen) {
  return XWidthOfScreen_impl(screen);
}

int XWindowEvent(Display* display, Window w, long event_mask, XEvent* event_return) {
  return XWindowEvent_impl(display, w, event_mask, event_return);
}

int XWriteBitmapFile(
    Display* display,
    _Xconst char* filename,
    Pixmap bitmap,
    unsigned int width,
    unsigned int height,
    int x_hot,
    int y_hot) {
  return XWriteBitmapFile_impl(display, filename, bitmap, width, height, x_hot, y_hot);
}

Bool XSupportsLocale(void) {
  return XSupportsLocale_impl();
}

char* XSetLocaleModifiers(const char* modifier_list) {
  return XSetLocaleModifiers_impl(modifier_list);
}

XOM XOpenOM(
    Display* display,
    struct _XrmHashBucketRec* rdb,
    _Xconst char* res_name,
    _Xconst char* res_class) {
  return XOpenOM_impl(display, rdb, res_name, res_class);
}

Status XCloseOM(XOM om) {
  return XCloseOM_impl(om);
}

Display* XDisplayOfOM(XOM om) {
  return XDisplayOfOM_impl(om);
}

char* XLocaleOfOM(XOM om) {
  return XLocaleOfOM_impl(om);
}

void XDestroyOC(XOC oc) {
  return XDestroyOC_impl(oc);
}

XOM XOMOfOC(XOC oc) {
  return XOMOfOC_impl(oc);
}

XFontSet XCreateFontSet(
    Display* display,
    _Xconst char* base_font_name_list,
    char*** missing_charset_list,
    int* missing_charset_count,
    char** def_string) {
  return XCreateFontSet_impl(display, base_font_name_list, missing_charset_list, missing_charset_count, def_string);
}

void XFreeFontSet(Display* display, XFontSet font_set) {
  return XFreeFontSet_impl(display, font_set);
}

int XFontsOfFontSet(XFontSet font_set, XFontStruct*** font_struct_list, char*** font_name_list) {
  return XFontsOfFontSet_impl(font_set, font_struct_list, font_name_list);
}

char* XBaseFontNameListOfFontSet(XFontSet font_set) {
  return XBaseFontNameListOfFontSet_impl(font_set);
}

char* XLocaleOfFontSet(XFontSet font_set) {
  return XLocaleOfFontSet_impl(font_set);
}

Bool XContextDependentDrawing(XFontSet font_set) {
  return XContextDependentDrawing_impl(font_set);
}

Bool XDirectionalDependentDrawing(XFontSet font_set) {
  return XDirectionalDependentDrawing_impl(font_set);
}

Bool XContextualDrawing(XFontSet font_set) {
  return XContextualDrawing_impl(font_set);
}

XFontSetExtents* XExtentsOfFontSet(XFontSet font_set) {
  return XExtentsOfFontSet_impl(font_set);
}

int XmbTextEscapement(XFontSet font_set, _Xconst char* text, int bytes_text) {
  return XmbTextEscapement_impl(font_set, text, bytes_text);
}

int XwcTextEscapement(XFontSet font_set, _Xconst wchar_t* text, int num_wchars) {
  return XwcTextEscapement_impl(font_set, text, num_wchars);
}

int Xutf8TextEscapement(XFontSet font_set, _Xconst char* text, int bytes_text) {
  return Xutf8TextEscapement_impl(font_set, text, bytes_text);
}

int XmbTextExtents(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return) {
  return XmbTextExtents_impl(font_set, text, bytes_text, overall_ink_return, overall_logical_return);
}

int XwcTextExtents(
    XFontSet font_set,
    _Xconst wchar_t* text,
    int num_wchars,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return) {
  return XwcTextExtents_impl(font_set, text, num_wchars, overall_ink_return, overall_logical_return);
}

int Xutf8TextExtents(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return) {
  return Xutf8TextExtents_impl(font_set, text, bytes_text, overall_ink_return, overall_logical_return);
}

Status XmbTextPerCharExtents(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text,
    XRectangle* ink_extents_buffer,
    XRectangle* logical_extents_buffer,
    int buffer_size,
    int* num_chars,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return) {
  return XmbTextPerCharExtents_impl(font_set, text, bytes_text, ink_extents_buffer, logical_extents_buffer, buffer_size, num_chars, overall_ink_return, overall_logical_return);
}

Status XwcTextPerCharExtents(
    XFontSet font_set,
    _Xconst wchar_t* text,
    int num_wchars,
    XRectangle* ink_extents_buffer,
    XRectangle* logical_extents_buffer,
    int buffer_size,
    int* num_chars,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return) {
  return XwcTextPerCharExtents_impl(font_set, text, num_wchars, ink_extents_buffer, logical_extents_buffer, buffer_size, num_chars, overall_ink_return, overall_logical_return);
}

Status Xutf8TextPerCharExtents(
    XFontSet font_set,
    _Xconst char* text,
    int bytes_text,
    XRectangle* ink_extents_buffer,
    XRectangle* logical_extents_buffer,
    int buffer_size,
    int* num_chars,
    XRectangle* overall_ink_return,
    XRectangle* overall_logical_return) {
  return Xutf8TextPerCharExtents_impl(font_set, text, bytes_text, ink_extents_buffer, logical_extents_buffer, buffer_size, num_chars, overall_ink_return, overall_logical_return);
}

void XmbDrawText(Display* display, Drawable d, GC gc, int x, int y, XmbTextItem* text_items, int nitems) {
  return XmbDrawText_impl(display, d, gc, x, y, text_items, nitems);
}

void XwcDrawText(Display* display, Drawable d, GC gc, int x, int y, XwcTextItem* text_items, int nitems) {
  return XwcDrawText_impl(display, d, gc, x, y, text_items, nitems);
}

void Xutf8DrawText(Display* display, Drawable d, GC gc, int x, int y, XmbTextItem* text_items, int nitems) {
  return Xutf8DrawText_impl(display, d, gc, x, y, text_items, nitems);
}

void XmbDrawString(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst char* text,
    int bytes_text) {
  return XmbDrawString_impl(display, d, font_set, gc, x, y, text, bytes_text);
}

void XwcDrawString(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst wchar_t* text,
    int num_wchars) {
  return XwcDrawString_impl(display, d, font_set, gc, x, y, text, num_wchars);
}

void Xutf8DrawString(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst char* text,
    int bytes_text) {
  return Xutf8DrawString_impl(display, d, font_set, gc, x, y, text, bytes_text);
}

void XmbDrawImageString(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst char* text,
    int bytes_text) {
  return XmbDrawImageString_impl(display, d, font_set, gc, x, y, text, bytes_text);
}

void XwcDrawImageString(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst wchar_t* text,
    int num_wchars) {
  return XwcDrawImageString_impl(display, d, font_set, gc, x, y, text, num_wchars);
}

void Xutf8DrawImageString(
    Display* display,
    Drawable d,
    XFontSet font_set,
    GC gc,
    int x,
    int y,
    _Xconst char* text,
    int bytes_text) {
  return Xutf8DrawImageString_impl(display, d, font_set, gc, x, y, text, bytes_text);
}

XIM XOpenIM(Display* dpy, struct _XrmHashBucketRec* rdb, char* res_name, char* res_class) {
  return XOpenIM_impl(dpy, rdb, res_name, res_class);
}

Status XCloseIM(XIM im) {
  return XCloseIM_impl(im);
}

Display* XDisplayOfIM(XIM im) {
  return XDisplayOfIM_impl(im);
}

char* XLocaleOfIM(XIM im) {
  return XLocaleOfIM_impl(im);
}

void XDestroyIC(XIC ic) {
  return XDestroyIC_impl(ic);
}

void XSetICFocus(XIC ic) {
  return XSetICFocus_impl(ic);
}

void XUnsetICFocus(XIC ic) {
  return XUnsetICFocus_impl(ic);
}

wchar_t* XwcResetIC(XIC ic) {
  return XwcResetIC_impl(ic);
}

char* XmbResetIC(XIC ic) {
  return XmbResetIC_impl(ic);
}

char* Xutf8ResetIC(XIC ic) {
  return Xutf8ResetIC_impl(ic);
}

XIM XIMOfIC(XIC ic) {
  return XIMOfIC_impl(ic);
}

Bool XFilterEvent(XEvent* event, Window window) {
  return XFilterEvent_impl(event, window);
}

int XmbLookupString(
    XIC ic,
    XKeyPressedEvent* event,
    char* buffer_return,
    int bytes_buffer,
    KeySym* keysym_return,
    Status* status_return) {
  return XmbLookupString_impl(ic, event, buffer_return, bytes_buffer, keysym_return, status_return);
}

int XwcLookupString(
    XIC ic,
    XKeyPressedEvent* event,
    wchar_t* buffer_return,
    int wchars_buffer,
    KeySym* keysym_return,
    Status* status_return) {
  return XwcLookupString_impl(ic, event, buffer_return, wchars_buffer, keysym_return, status_return);
}

int Xutf8LookupString(
    XIC ic,
    XKeyPressedEvent* event,
    char* buffer_return,
    int bytes_buffer,
    KeySym* keysym_return,
    Status* status_return) {
  return Xutf8LookupString_impl(ic, event, buffer_return, bytes_buffer, keysym_return, status_return);
}

Bool XRegisterIMInstantiateCallback(
    Display* dpy,
    struct _XrmHashBucketRec* rdb,
    char* res_name,
    char* res_class,
    XIDProc callback,
    XPointer client_data) {
  return XRegisterIMInstantiateCallback_impl(dpy, rdb, res_name, res_class, callback, client_data);
}

Bool XUnregisterIMInstantiateCallback(
    Display* dpy,
    struct _XrmHashBucketRec* rdb,
    char* res_name,
    char* res_class,
    XIDProc callback,
    XPointer client_data) {
  return XUnregisterIMInstantiateCallback_impl(dpy, rdb, res_name, res_class, callback, client_data);
}

Status XInternalConnectionNumbers(Display* dpy, int** fd_return, int* count_return) {
  return XInternalConnectionNumbers_impl(dpy, fd_return, count_return);
}

void XProcessInternalConnection(Display* dpy, int fd) {
  return XProcessInternalConnection_impl(dpy, fd);
}

Status XAddConnectionWatch(Display* dpy, XConnectionWatchProc callback, XPointer client_data) {
  return XAddConnectionWatch_impl(dpy, callback, client_data);
}

void XRemoveConnectionWatch(Display* dpy, XConnectionWatchProc callback, XPointer client_data) {
  return XRemoveConnectionWatch_impl(dpy, callback, client_data);
}

void XSetAuthorization(char* name, int namelen, char* data, int datalen) {
  return XSetAuthorization_impl(name, namelen, data, datalen);
}

int _Xmbtowc(wchar_t* wstr, char* str, int len) {
  return _Xmbtowc_impl(wstr, str, len);
}

int _Xwctomb(char* str, wchar_t wc) {
  return _Xwctomb_impl(str, wc);
}

Bool XGetEventData(Display* dpy, XGenericEventCookie* cookie) {
  return XGetEventData_impl(dpy, cookie);
}

void XFreeEventData(Display* dpy, XGenericEventCookie* cookie) {
  return XFreeEventData_impl(dpy, cookie);
}

// Xlib-xcb.h
xcb_connection_t* XGetXCBConnection(Display* dpy) {
  return XGetXCBConnection_impl(dpy);
}

void XSetEventQueueOwner(Display* dpy, enum XEventQueueOwner owner) {
  return XSetEventQueueOwner_impl(dpy, owner);
}

// Xlibint.h
void* _XGetRequest(Display* dpy, CARD8 type, size_t len) {
  return _XGetRequest_impl(dpy, type, len);
}

void _XFlushGCCache(Display* dpy, GC gc) {
  return _XFlushGCCache_impl(dpy, gc);
}

int _XData32(Display* dpy, register _Xconst long* data, unsigned len) {
  return _XData32_impl(dpy, data, len);
}

void _XRead32(Display* dpy, register long* data, long len) {
  return _XRead32_impl(dpy, data, len);
}

void _XDeqAsyncHandler(Display* dpy, _XAsyncHandler* handler) {
  return _XDeqAsyncHandler_impl(dpy, handler);
}

#ifdef DataRoutineIsProcedure
void Data(Display* dpy, char* data, long len) {
  return Data_impl(dpy, data, len);
}
#endif

int _XError(Display* dpy, xError* rep) {
  return _XError_impl(dpy, rep);
}

int _XIOError(Display* dpy) {
  _XIOError_impl(dpy);
  abort();
}

void _XEatData(Display* dpy, unsigned long n) {
  return _XEatData_impl(dpy, n);
}

void _XEatDataWords(Display* dpy, unsigned long n) {
  return _XEatDataWords_impl(dpy, n);
}

char* _XAllocScratch(Display* dpy, unsigned long nbytes) {
  return _XAllocScratch_impl(dpy, nbytes);
}

char* _XAllocTemp(Display* dpy, unsigned long nbytes) {
  return _XAllocTemp_impl(dpy, nbytes);
}

void _XFreeTemp(Display* dpy, char* buf, unsigned long nbytes) {
  return _XFreeTemp_impl(dpy, buf, nbytes);
}

Visual* _XVIDtoVisual(Display* dpy, VisualID id) {
  return _XVIDtoVisual_impl(dpy, id);
}

unsigned long _XSetLastRequestRead(Display* dpy, xGenericReply* rep) {
  return _XSetLastRequestRead_impl(dpy, rep);
}

int _XGetHostname(char* buf, int maxlen) {
  return _XGetHostname_impl(buf, maxlen);
}

Screen* _XScreenOfWindow(Display* dpy, Window w) {
  return _XScreenOfWindow_impl(dpy, w);
}

Bool _XAsyncErrorHandler(Display* dpy, xReply* rep, char* buf, int len, XPointer data) {
  return _XAsyncErrorHandler_impl(dpy, rep, buf, len, data);
}

char* _XGetAsyncReply(Display* dpy, char* replbuf, xReply* rep, char* buf, int len, int extra, Bool discard) {
  return _XGetAsyncReply_impl(dpy, replbuf, rep, buf, len, extra, discard);
}

void _XGetAsyncData(Display* dpy, char* data, char* buf, int len, int skip, int datalen, int discardtotal) {
  return _XGetAsyncData_impl(dpy, data, buf, len, skip, datalen, discardtotal);
}

void _XFlush(Display* dpy) {
  return _XFlush_impl(dpy);
}

int _XEventsQueued(Display* dpy, int mode) {
  return _XEventsQueued_impl(dpy, mode);
}

void _XReadEvents(Display* dpy) {
  return _XReadEvents_impl(dpy);
}

int _XRead(Display* dpy, char* data, long size) {
  return _XRead_impl(dpy, data, size);
}

void _XReadPad(Display* dpy, char* data, long size) {
  return _XReadPad_impl(dpy, data, size);
}

void _XSend(Display* dpy, _Xconst char* data, long size) {
  return _XSend_impl(dpy, data, size);
}

Status _XReply(Display* dpy, xReply* rep, int extra, Bool discard) {
  return _XReply_impl(dpy, rep, extra, discard);
}

void _XEnq(Display* dpy, xEvent* event) {
  return _XEnq_impl(dpy, event);
}

void _XDeq(Display* dpy, _XQEvent* prev, _XQEvent* qelt) {
  return _XDeq_impl(dpy, prev, qelt);
}

Bool _XUnknownWireEvent(Display* dpy, XEvent* re, xEvent* event) {
  return _XUnknownWireEvent_impl(dpy, re, event);
}

Bool _XUnknownWireEventCookie(Display* dpy, XGenericEventCookie* re, xEvent* event) {
  return _XUnknownWireEventCookie_impl(dpy, re, event);
}

Bool _XUnknownCopyEventCookie(Display* dpy, XGenericEventCookie* in, XGenericEventCookie* out) {
  return _XUnknownCopyEventCookie_impl(dpy, in, out);
}

Status _XUnknownNativeEvent(Display* dpy, XEvent* re, xEvent* event) {
  return _XUnknownNativeEvent_impl(dpy, re, event);
}

Bool _XWireToEvent(Display* dpy, XEvent* re, xEvent* event) {
  return _XWireToEvent_impl(dpy, re, event);
}

Bool _XDefaultWireError(Display* display, XErrorEvent* he, xError* we) {
  return _XDefaultWireError_impl(display, he, we);
}

Bool _XPollfdCacheInit(Display* dpy) {
  return _XPollfdCacheInit_impl(dpy);
}

void _XPollfdCacheAdd(Display* dpy, int fd) {
  return _XPollfdCacheAdd_impl(dpy, fd);
}

void _XPollfdCacheDel(Display* dpy, int fd) {
  return _XPollfdCacheDel_impl(dpy, fd);
}

XID _XAllocID(Display* dpy) {
  return _XAllocID_impl(dpy);
}

void _XAllocIDs(Display* dpy, XID* ids, int count) {
  return _XAllocIDs_impl(dpy, ids, count);
}

int _XFreeExtData(XExtData* extension) {
  return _XFreeExtData_impl(extension);
}

int (* XESetCreateGC(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, GC /* gc */, XExtCodes* /* codes */)))(Display*, GC, XExtCodes*) {
  return XESetCreateGC_impl(display, extension, proc);
}

int (* XESetCopyGC(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, GC /* gc */, XExtCodes* /* codes */)))(Display*, GC, XExtCodes*) {
  return XESetCopyGC_impl(display, extension, proc);
}

int (* XESetFlushGC(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, GC /* gc */, XExtCodes* /* codes */)))(Display*, GC, XExtCodes*) {
  return XESetFlushGC_impl(display, extension, proc);
}

int (* XESetFreeGC(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, GC /* gc */, XExtCodes* /* codes */)))(Display*, GC, XExtCodes*) {
  return XESetFreeGC_impl(display, extension, proc);
}

int (* XESetCreateFont(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, XFontStruct* /* fs */, XExtCodes* /* codes */)))(Display*, XFontStruct*, XExtCodes*) {
  return XESetCreateFont_impl(display, extension, proc);
}

int (* XESetFreeFont(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, XFontStruct* /* fs */, XExtCodes* /* codes */)))(Display*, XFontStruct*, XExtCodes*) {
  return XESetFreeFont_impl(display, extension, proc);
}

int (* XESetCloseDisplay(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, XExtCodes* /* codes */)))(Display*, XExtCodes*) {
  return XESetCloseDisplay_impl(display, extension, proc);
}

int (* XESetError(
    Display* display,
    int extension,
    int (*proc)(Display* /* display */, xError* /* err */, XExtCodes* /* codes */, int* /* ret_code */)))(Display*, xError*, XExtCodes*, int*) {
  return XESetError_impl(display, extension, proc);
}

char* (* XESetErrorString(
    Display* display,
    int extension,
    char* (*proc)(Display* /* display */, int /* code */, XExtCodes* /* codes */, char* /* buffer */, int /* nbytes */)))(Display*, int, XExtCodes*, char*, int) {
  return XESetErrorString_impl(display, extension, proc);
}

void (* XESetPrintErrorValues(
    Display* display,
    int extension,
    void (*proc)(Display* /* display */, XErrorEvent* /* ev */, void* /* fp */)))(Display*, XErrorEvent*, void*) {
  return XESetPrintErrorValues_impl(display, extension, proc);
}

Bool (* XESetWireToEvent(
    Display* display,
    int event_number,
    Bool (*proc)(Display* /* display */, XEvent* /* re */, xEvent* /* event */)))(Display*, XEvent*, xEvent*) {
  return XESetWireToEvent_impl(display, event_number, proc);
}

Bool (* XESetWireToEventCookie(
    Display* display,
    int extension,
    Bool (*proc)(Display* /* display */, XGenericEventCookie* /* re */, xEvent* /* event */)))(Display*, XGenericEventCookie*, xEvent*) {
  return XESetWireToEventCookie_impl(display, extension, proc);
}

Bool (* XESetCopyEventCookie(
    Display* display,
    int extension,
    Bool (*proc)(Display* /* display */, XGenericEventCookie* /* in */, XGenericEventCookie* /* out */)))(Display*, XGenericEventCookie*, XGenericEventCookie*) {
  return XESetCopyEventCookie_impl(display, extension, proc);
}

Status (* XESetEventToWire(
    Display* display,
    int event_number,
    Status (*proc)(Display* /* display */, XEvent* /* re */, xEvent* /* event */)))(Display*, XEvent*, xEvent*) {
  return XESetEventToWire_impl(display, event_number, proc);
}

Bool (* XESetWireToError(
    Display* display,
    int error_number,
    Bool (*proc)(Display* /* display */, XErrorEvent* /* he */, xError* /* we */)))(Display*, XErrorEvent*, xError*) {
  return XESetWireToError_impl(display, error_number, proc);
}

void (* XESetBeforeFlush(
    Display* display,
    int error_number,
    void (*proc)(Display* /* display */, XExtCodes* /* codes */, _Xconst char* /* data */, long /* len */)))(Display*, XExtCodes*, _Xconst char*, long) {
  return XESetBeforeFlush_impl(display, error_number, proc);
}

Status _XRegisterInternalConnection(Display* dpy, int fd, _XInternalConnectionProc callback, XPointer call_data) {
  return _XRegisterInternalConnection_impl(dpy, fd, callback, call_data);
}

void _XUnregisterInternalConnection(Display* dpy, int fd) {
  return _XUnregisterInternalConnection_impl(dpy, fd);
}

void _XProcessInternalConnection(Display* dpy, struct _XConnectionInfo* conn_info) {
  return _XProcessInternalConnection_impl(dpy, conn_info);
}

#if defined(__UNIXOS2__)
char* __XOS2RedirRoot(char* arg) {
  return __XOS2RedirRoot_impl(arg);
}
#endif

int _XTextHeight(XFontStruct* font_struct, _Xconst char* string, int count) {
  return _XTextHeight_impl(font_struct, string, count);
}

int _XTextHeight16(XFontStruct* font_struct, _Xconst XChar2b* string, int count) {
  return _XTextHeight16_impl(font_struct, string, count);
}

#if defined(WIN32)
int _XOpenFile(_Xconst char* path, int flags) {
  return _XOpenFile_impl(path, flags);
}
#endif

#if defined(WIN32)
int _XOpenFileMode(_Xconst char* path, int flags, mode_t mode) {
  return _XOpenFileMode_impl(path, flags, mode);
}
#endif

#if defined(WIN32)
void* _XFopenFile(_Xconst char* path, _Xconst char* mode) {
  return _XFopenFile_impl(path, mode);
}
#endif

#if defined(WIN32)
int _XAccessFile(_Xconst char* path) {
  return _XAccessFile_impl(path);
}
#endif

Status _XEventToWire(Display* dpy, XEvent* re, xEvent* event) {
  return _XEventToWire_impl(dpy, re, event);
}

int _XF86LoadQueryLocaleFont(Display* dpy, _Xconst char* name, XFontStruct** xfp, Font* fidp) {
  return _XF86LoadQueryLocaleFont_impl(dpy, name, xfp, fidp);
}

void _XProcessWindowAttributes(
    register Display* dpy,
    xChangeWindowAttributesReq* req,
    register unsigned long valuemask,
    register XSetWindowAttributes* attributes) {
  return _XProcessWindowAttributes_impl(dpy, req, valuemask, attributes);
}

int _XDefaultError(Display* dpy, XErrorEvent* event) {
  return _XDefaultError_impl(dpy, event);
}

int _XDefaultIOError(Display* dpy) {
  return _XDefaultIOError_impl(dpy);
}

void _XSetClipRectangles(
    register Display* dpy,
    GC gc,
    int clip_x_origin,
    int clip_y_origin,
    XRectangle* rectangles,
    int n,
    int ordering) {
  return _XSetClipRectangles_impl(dpy, gc, clip_x_origin, clip_y_origin, rectangles, n, ordering);
}

Status _XGetWindowAttributes(register Display* dpy, Window w, XWindowAttributes* attr) {
  return _XGetWindowAttributes_impl(dpy, w, attr);
}

int _XPutBackEvent(register Display* dpy, register XEvent* event) {
  return _XPutBackEvent_impl(dpy, event);
}

Bool _XIsEventCookie(Display* dpy, XEvent* ev) {
  return _XIsEventCookie_impl(dpy, ev);
}

void _XFreeEventCookies(Display* dpy) {
  return _XFreeEventCookies_impl(dpy);
}

void _XStoreEventCookie(Display* dpy, XEvent* ev) {
  return _XStoreEventCookie_impl(dpy, ev);
}

Bool _XFetchEventCookie(Display* dpy, XGenericEventCookie* ev) {
  return _XFetchEventCookie_impl(dpy, ev);
}

Bool _XCopyEventCookie(Display* dpy, XGenericEventCookie* in, XGenericEventCookie* out) {
  return _XCopyEventCookie_impl(dpy, in, out);
}

void xlocaledir(char* buf, int buf_len) {
  return xlocaledir_impl(buf, buf_len);
}

// XKBlib.h
Bool XkbIgnoreExtension(Bool ignore) {
  return XkbIgnoreExtension_impl(ignore);
}

Display* XkbOpenDisplay(char* name, int* ev_rtrn, int* err_rtrn, int* major_rtrn, int* minor_rtrn, int* reason) {
  return XkbOpenDisplay_impl(name, ev_rtrn, err_rtrn, major_rtrn, minor_rtrn, reason);
}

Bool XkbQueryExtension(
    Display* dpy,
    int* opcodeReturn,
    int* eventBaseReturn,
    int* errorBaseReturn,
    int* majorRtrn,
    int* minorRtrn) {
  return XkbQueryExtension_impl(dpy, opcodeReturn, eventBaseReturn, errorBaseReturn, majorRtrn, minorRtrn);
}

Bool XkbUseExtension(Display* dpy, int* major_rtrn, int* minor_rtrn) {
  return XkbUseExtension_impl(dpy, major_rtrn, minor_rtrn);
}

Bool XkbLibraryVersion(int* libMajorRtrn, int* libMinorRtrn) {
  return XkbLibraryVersion_impl(libMajorRtrn, libMinorRtrn);
}

unsigned int XkbSetXlibControls(Display* dpy, unsigned int affect, unsigned int values) {
  return XkbSetXlibControls_impl(dpy, affect, values);
}

unsigned int XkbGetXlibControls(Display* dpy) {
  return XkbGetXlibControls_impl(dpy);
}

unsigned int XkbXlibControlsImplemented(void) {
  return XkbXlibControlsImplemented_impl();
}

void XkbSetAtomFuncs(XkbInternAtomFunc getAtom, XkbGetAtomNameFunc getName) {
  return XkbSetAtomFuncs_impl(getAtom, getName);
}

KeySym XkbKeycodeToKeysym(
    Display* dpy,
#if NeedWidePrototypes
    unsigned int kc,
#else
    KeyCode kc,
#endif
    int group,
    int level) {
  return XkbKeycodeToKeysym_impl(dpy, kc, group, level);
}

unsigned int XkbKeysymToModifiers(Display* dpy, KeySym ks) {
  return XkbKeysymToModifiers_impl(dpy, ks);
}

Bool XkbLookupKeySym(
    Display* dpy,
#if NeedWidePrototypes
    unsigned int keycode,
#else
    KeyCode keycode,
#endif
    unsigned int modifiers,
    unsigned int* modifiers_return,
    KeySym* keysym_return) {
  return XkbLookupKeySym_impl(dpy, keycode, modifiers, modifiers_return, keysym_return);
}

int XkbLookupKeyBinding(Display* dpy, KeySym sym_rtrn, unsigned int mods, char* buffer, int nbytes, int* extra_rtrn) {
  return XkbLookupKeyBinding_impl(dpy, sym_rtrn, mods, buffer, nbytes, extra_rtrn);
}

Bool XkbTranslateKeyCode(
    XkbDescPtr xkb,
#if NeedWidePrototypes
    unsigned int keycode,
#else
    KeyCode keycode,
#endif
    unsigned int modifiers,
    unsigned int* modifiers_return,
    KeySym* keysym_return) {
  return XkbTranslateKeyCode_impl(xkb, keycode, modifiers, modifiers_return, keysym_return);
}

int XkbTranslateKeySym(
    Display* dpy,
    register KeySym* sym_return,
    unsigned int modifiers,
    char* buffer,
    int nbytes,
    int* extra_rtrn) {
  return XkbTranslateKeySym_impl(dpy, sym_return, modifiers, buffer, nbytes, extra_rtrn);
}

Bool XkbSetAutoRepeatRate(Display* dpy, unsigned int deviceSpec, unsigned int delay, unsigned int interval) {
  return XkbSetAutoRepeatRate_impl(dpy, deviceSpec, delay, interval);
}

Bool XkbGetAutoRepeatRate(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int* delayRtrn,
    unsigned int* intervalRtrn) {
  return XkbGetAutoRepeatRate_impl(dpy, deviceSpec, delayRtrn, intervalRtrn);
}

Bool XkbChangeEnabledControls(Display* dpy, unsigned int deviceSpec, unsigned int affect, unsigned int values) {
  return XkbChangeEnabledControls_impl(dpy, deviceSpec, affect, values);
}

Bool XkbDeviceBell(Display* dpy, Window win, int deviceSpec, int bellClass, int bellID, int percent, Atom name) {
  return XkbDeviceBell_impl(dpy, win, deviceSpec, bellClass, bellID, percent, name);
}

Bool XkbForceDeviceBell(Display* dpy, int deviceSpec, int bellClass, int bellID, int percent) {
  return XkbForceDeviceBell_impl(dpy, deviceSpec, bellClass, bellID, percent);
}

Bool XkbDeviceBellEvent(Display* dpy, Window win, int deviceSpec, int bellClass, int bellID, int percent, Atom name) {
  return XkbDeviceBellEvent_impl(dpy, win, deviceSpec, bellClass, bellID, percent, name);
}

Bool XkbBell(Display* dpy, Window win, int percent, Atom name) {
  return XkbBell_impl(dpy, win, percent, name);
}

Bool XkbForceBell(Display* dpy, int percent) {
  return XkbForceBell_impl(dpy, percent);
}

Bool XkbBellEvent(Display* dpy, Window win, int percent, Atom name) {
  return XkbBellEvent_impl(dpy, win, percent, name);
}

Bool XkbSelectEvents(Display* dpy, unsigned int deviceID, unsigned int affect, unsigned int values) {
  return XkbSelectEvents_impl(dpy, deviceID, affect, values);
}

Bool XkbSelectEventDetails(
    Display* dpy,
    unsigned int deviceID,
    unsigned int eventType,
    unsigned long affect,
    unsigned long details) {
  return XkbSelectEventDetails_impl(dpy, deviceID, eventType, affect, details);
}

void XkbNoteMapChanges(XkbMapChangesPtr old, XkbMapNotifyEvent* new_, unsigned int wanted) {
  return XkbNoteMapChanges_impl(old, new_, wanted);
}

void XkbNoteNameChanges(XkbNameChangesPtr old, XkbNamesNotifyEvent* new_, unsigned int wanted) {
  return XkbNoteNameChanges_impl(old, new_, wanted);
}

Status XkbGetIndicatorState(Display* dpy, unsigned int deviceSpec, unsigned int* pStateRtrn) {
  return XkbGetIndicatorState_impl(dpy, deviceSpec, pStateRtrn);
}

Status XkbGetDeviceIndicatorState(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int ledClass,
    unsigned int ledID,
    unsigned int* pStateRtrn) {
  return XkbGetDeviceIndicatorState_impl(dpy, deviceSpec, ledClass, ledID, pStateRtrn);
}

Status XkbGetIndicatorMap(Display* dpy, unsigned long which, XkbDescPtr desc) {
  return XkbGetIndicatorMap_impl(dpy, which, desc);
}

Bool XkbSetIndicatorMap(Display* dpy, unsigned long which, XkbDescPtr desc) {
  return XkbSetIndicatorMap_impl(dpy, which, desc);
}

Bool XkbGetNamedIndicator(
    Display* dpy,
    Atom name,
    int* pNdxRtrn,
    Bool* pStateRtrn,
    XkbIndicatorMapPtr pMapRtrn,
    Bool* pRealRtrn) {
  return XkbGetNamedIndicator_impl(dpy, name, pNdxRtrn, pStateRtrn, pMapRtrn, pRealRtrn);
}

Bool XkbGetNamedDeviceIndicator(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int ledClass,
    unsigned int ledID,
    Atom name,
    int* pNdxRtrn,
    Bool* pStateRtrn,
    XkbIndicatorMapPtr pMapRtrn,
    Bool* pRealRtrn) {
  return XkbGetNamedDeviceIndicator_impl(dpy, deviceSpec, ledClass, ledID, name, pNdxRtrn, pStateRtrn, pMapRtrn, pRealRtrn);
}

Bool XkbSetNamedIndicator(
    Display* dpy,
    Atom name,
    Bool changeState,
    Bool state,
    Bool createNewMap,
    XkbIndicatorMapPtr pMap) {
  return XkbSetNamedIndicator_impl(dpy, name, changeState, state, createNewMap, pMap);
}

Bool XkbSetNamedDeviceIndicator(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int ledClass,
    unsigned int ledID,
    Atom name,
    Bool changeState,
    Bool state,
    Bool createNewMap,
    XkbIndicatorMapPtr pMap) {
  return XkbSetNamedDeviceIndicator_impl(dpy, deviceSpec, ledClass, ledID, name, changeState, state, createNewMap, pMap);
}

Bool XkbLockModifiers(Display* dpy, unsigned int deviceSpec, unsigned int affect, unsigned int values) {
  return XkbLockModifiers_impl(dpy, deviceSpec, affect, values);
}

Bool XkbLatchModifiers(Display* dpy, unsigned int deviceSpec, unsigned int affect, unsigned int values) {
  return XkbLatchModifiers_impl(dpy, deviceSpec, affect, values);
}

Bool XkbLockGroup(Display* dpy, unsigned int deviceSpec, unsigned int group) {
  return XkbLockGroup_impl(dpy, deviceSpec, group);
}

Bool XkbLatchGroup(Display* dpy, unsigned int deviceSpec, unsigned int group) {
  return XkbLatchGroup_impl(dpy, deviceSpec, group);
}

Bool XkbSetServerInternalMods(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int affectReal,
    unsigned int realValues,
    unsigned int affectVirtual,
    unsigned int virtualValues) {
  return XkbSetServerInternalMods_impl(dpy, deviceSpec, affectReal, realValues, affectVirtual, virtualValues);
}

Bool XkbSetIgnoreLockMods(
    Display* dpy,
    unsigned int deviceSpec,
    unsigned int affectReal,
    unsigned int realValues,
    unsigned int affectVirtual,
    unsigned int virtualValues) {
  return XkbSetIgnoreLockMods_impl(dpy, deviceSpec, affectReal, realValues, affectVirtual, virtualValues);
}

Bool XkbVirtualModsToReal(XkbDescPtr xkb, unsigned int virtual_mask, unsigned int* mask_rtrn) {
  return XkbVirtualModsToReal_impl(xkb, virtual_mask, mask_rtrn);
}

Bool XkbComputeEffectiveMap(XkbDescPtr xkb, XkbKeyTypePtr type, unsigned char* map_rtrn) {
  return XkbComputeEffectiveMap_impl(xkb, type, map_rtrn);
}

Status XkbInitCanonicalKeyTypes(XkbDescPtr xkb, unsigned int which, int keypadVMod) {
  return XkbInitCanonicalKeyTypes_impl(xkb, which, keypadVMod);
}

XkbDescPtr XkbAllocKeyboard(void) {
  return XkbAllocKeyboard_impl();
}

void XkbFreeKeyboard(XkbDescPtr xkb, unsigned int which, Bool freeDesc) {
  return XkbFreeKeyboard_impl(xkb, which, freeDesc);
}

Status XkbAllocClientMap(XkbDescPtr xkb, unsigned int which, unsigned int nTypes) {
  return XkbAllocClientMap_impl(xkb, which, nTypes);
}

Status XkbAllocServerMap(XkbDescPtr xkb, unsigned int which, unsigned int nActions) {
  return XkbAllocServerMap_impl(xkb, which, nActions);
}

void XkbFreeClientMap(XkbDescPtr xkb, unsigned int what, Bool freeMap) {
  return XkbFreeClientMap_impl(xkb, what, freeMap);
}

void XkbFreeServerMap(XkbDescPtr xkb, unsigned int what, Bool freeMap) {
  return XkbFreeServerMap_impl(xkb, what, freeMap);
}

XkbKeyTypePtr XkbAddKeyType(XkbDescPtr xkb, Atom name, int map_count, Bool want_preserve, int num_lvls) {
  return XkbAddKeyType_impl(xkb, name, map_count, want_preserve, num_lvls);
}

Status XkbAllocIndicatorMaps(XkbDescPtr xkb) {
  return XkbAllocIndicatorMaps_impl(xkb);
}

void XkbFreeIndicatorMaps(XkbDescPtr xkb) {
  return XkbFreeIndicatorMaps_impl(xkb);
}

XkbDescPtr XkbGetMap(Display* dpy, unsigned int which, unsigned int deviceSpec) {
  return XkbGetMap_impl(dpy, which, deviceSpec);
}

Status XkbGetUpdatedMap(Display* dpy, unsigned int which, XkbDescPtr desc) {
  return XkbGetUpdatedMap_impl(dpy, which, desc);
}

Status XkbGetMapChanges(Display* dpy, XkbDescPtr xkb, XkbMapChangesPtr changes) {
  return XkbGetMapChanges_impl(dpy, xkb, changes);
}

Status XkbRefreshKeyboardMapping(XkbMapNotifyEvent* event) {
  return XkbRefreshKeyboardMapping_impl(event);
}

Status XkbGetKeyTypes(Display* dpy, unsigned int first, unsigned int num, XkbDescPtr xkb) {
  return XkbGetKeyTypes_impl(dpy, first, num, xkb);
}

Status XkbGetKeySyms(Display* dpy, unsigned int first, unsigned int num, XkbDescPtr xkb) {
  return XkbGetKeySyms_impl(dpy, first, num, xkb);
}

Status XkbGetKeyActions(Display* dpy, unsigned int first, unsigned int num, XkbDescPtr xkb) {
  return XkbGetKeyActions_impl(dpy, first, num, xkb);
}

Status XkbGetKeyBehaviors(Display* dpy, unsigned int firstKey, unsigned int nKeys, XkbDescPtr desc) {
  return XkbGetKeyBehaviors_impl(dpy, firstKey, nKeys, desc);
}

Status XkbGetVirtualMods(Display* dpy, unsigned int which, XkbDescPtr desc) {
  return XkbGetVirtualMods_impl(dpy, which, desc);
}

Status XkbGetKeyExplicitComponents(Display* dpy, unsigned int firstKey, unsigned int nKeys, XkbDescPtr desc) {
  return XkbGetKeyExplicitComponents_impl(dpy, firstKey, nKeys, desc);
}

Status XkbGetKeyModifierMap(Display* dpy, unsigned int firstKey, unsigned int nKeys, XkbDescPtr desc) {
  return XkbGetKeyModifierMap_impl(dpy, firstKey, nKeys, desc);
}

Status XkbGetKeyVirtualModMap(Display* dpy, unsigned int first, unsigned int num, XkbDescPtr xkb) {
  return XkbGetKeyVirtualModMap_impl(dpy, first, num, xkb);
}

Status XkbAllocControls(XkbDescPtr xkb, unsigned int which) {
  return XkbAllocControls_impl(xkb, which);
}

void XkbFreeControls(XkbDescPtr xkb, unsigned int which, Bool freeMap) {
  return XkbFreeControls_impl(xkb, which, freeMap);
}

Status XkbGetControls(Display* dpy, unsigned long which, XkbDescPtr desc) {
  return XkbGetControls_impl(dpy, which, desc);
}

Bool XkbSetControls(Display* dpy, unsigned long which, XkbDescPtr desc) {
  return XkbSetControls_impl(dpy, which, desc);
}

void XkbNoteControlsChanges(XkbControlsChangesPtr old, XkbControlsNotifyEvent* new_, unsigned int wanted) {
  return XkbNoteControlsChanges_impl(old, new_, wanted);
}

Status XkbAllocCompatMap(XkbDescPtr xkb, unsigned int which, unsigned int nInterpret) {
  return XkbAllocCompatMap_impl(xkb, which, nInterpret);
}

void XkbFreeCompatMap(XkbDescPtr xkb, unsigned int which, Bool freeMap) {
  return XkbFreeCompatMap_impl(xkb, which, freeMap);
}

Status XkbGetCompatMap(Display* dpy, unsigned int which, XkbDescPtr xkb) {
  return XkbGetCompatMap_impl(dpy, which, xkb);
}

Bool XkbSetCompatMap(Display* dpy, unsigned int which, XkbDescPtr xkb, Bool updateActions) {
  return XkbSetCompatMap_impl(dpy, which, xkb, updateActions);
}

XkbSymInterpretPtr XkbAddSymInterpret(XkbDescPtr xkb, XkbSymInterpretPtr si, Bool updateMap, XkbChangesPtr changes) {
  return XkbAddSymInterpret_impl(xkb, si, updateMap, changes);
}

Status XkbAllocNames(XkbDescPtr xkb, unsigned int which, int nTotalRG, int nTotalAliases) {
  return XkbAllocNames_impl(xkb, which, nTotalRG, nTotalAliases);
}

Status XkbGetNames(Display* dpy, unsigned int which, XkbDescPtr desc) {
  return XkbGetNames_impl(dpy, which, desc);
}

Bool XkbSetNames(
    Display* dpy,
    unsigned int which,
    unsigned int firstType,
    unsigned int nTypes,
    XkbDescPtr desc) {
  return XkbSetNames_impl(dpy, which, firstType, nTypes, desc);
}

Bool XkbChangeNames(Display* dpy, XkbDescPtr xkb, XkbNameChangesPtr changes) {
  return XkbChangeNames_impl(dpy, xkb, changes);
}

void XkbFreeNames(XkbDescPtr xkb, unsigned int which, Bool freeMap) {
  return XkbFreeNames_impl(xkb, which, freeMap);
}

Status XkbGetState(Display* dpy, unsigned int deviceSpec, XkbStatePtr rtrnState) {
  return XkbGetState_impl(dpy, deviceSpec, rtrnState);
}

Bool XkbSetMap(Display* dpy, unsigned int which, XkbDescPtr desc) {
  return XkbSetMap_impl(dpy, which, desc);
}

Bool XkbChangeMap(Display* dpy, XkbDescPtr desc, XkbMapChangesPtr changes) {
  return XkbChangeMap_impl(dpy, desc, changes);
}

Bool XkbSetDetectableAutoRepeat(Display* dpy, Bool detectable, Bool* supported) {
  return XkbSetDetectableAutoRepeat_impl(dpy, detectable, supported);
}

Bool XkbGetDetectableAutoRepeat(Display* dpy, Bool* supported) {
  return XkbGetDetectableAutoRepeat_impl(dpy, supported);
}

Bool XkbSetAutoResetControls(Display* dpy, unsigned int changes, unsigned int* auto_ctrls, unsigned int* auto_values) {
  return XkbSetAutoResetControls_impl(dpy, changes, auto_ctrls, auto_values);
}

Bool XkbGetAutoResetControls(Display* dpy, unsigned int* auto_ctrls, unsigned int* auto_ctrl_values) {
  return XkbGetAutoResetControls_impl(dpy, auto_ctrls, auto_ctrl_values);
}

Bool XkbSetPerClientControls(Display* dpy, unsigned int change, unsigned int* values) {
  return XkbSetPerClientControls_impl(dpy, change, values);
}

Bool XkbGetPerClientControls(Display* dpy, unsigned int* ctrls) {
  return XkbGetPerClientControls_impl(dpy, ctrls);
}

Status XkbCopyKeyType(XkbKeyTypePtr from, XkbKeyTypePtr into) {
  return XkbCopyKeyType_impl(from, into);
}

Status XkbCopyKeyTypes(XkbKeyTypePtr from, XkbKeyTypePtr into, int num_types) {
  return XkbCopyKeyTypes_impl(from, into, num_types);
}

Status XkbResizeKeyType(XkbDescPtr xkb, int type_ndx, int map_count, Bool want_preserve, int new_num_lvls) {
  return XkbResizeKeyType_impl(xkb, type_ndx, map_count, want_preserve, new_num_lvls);
}

KeySym* XkbResizeKeySyms(XkbDescPtr desc, int forKey, int symsNeeded) {
  return XkbResizeKeySyms_impl(desc, forKey, symsNeeded);
}

XkbAction* XkbResizeKeyActions(XkbDescPtr desc, int forKey, int actsNeeded) {
  return XkbResizeKeyActions_impl(desc, forKey, actsNeeded);
}

Status XkbChangeTypesOfKey(
    XkbDescPtr xkb,
    int key,
    int num_groups,
    unsigned int groups,
    int* newTypes,
    XkbMapChangesPtr pChanges) {
  return XkbChangeTypesOfKey_impl(xkb, key, num_groups, groups, newTypes, pChanges);
}

Status XkbChangeKeycodeRange(XkbDescPtr xkb, int minKC, int maxKC, XkbChangesPtr changes) {
  return XkbChangeKeycodeRange_impl(xkb, minKC, maxKC, changes);
}

XkbComponentListPtr XkbListComponents(Display* dpy, unsigned int deviceSpec, XkbComponentNamesPtr ptrns, int* max_inout) {
  return XkbListComponents_impl(dpy, deviceSpec, ptrns, max_inout);
}

void XkbFreeComponentList(XkbComponentListPtr list) {
  return XkbFreeComponentList_impl(list);
}

XkbDescPtr XkbGetKeyboard(Display* dpy, unsigned int which, unsigned int deviceSpec) {
  return XkbGetKeyboard_impl(dpy, which, deviceSpec);
}

XkbDescPtr XkbGetKeyboardByName(
    Display* dpy,
    unsigned int deviceSpec,
    XkbComponentNamesPtr names,
    unsigned int want,
    unsigned int need,
    Bool load) {
  return XkbGetKeyboardByName_impl(dpy, deviceSpec, names, want, need, load);
}

int XkbKeyTypesForCoreSymbols(
    XkbDescPtr xkb,
    int map_width,
    KeySym* core_syms,
    unsigned int protected_,
    int* types_inout,
    KeySym* xkb_syms_rtrn) {
  return XkbKeyTypesForCoreSymbols_impl(xkb, map_width, core_syms, protected_, types_inout, xkb_syms_rtrn);
}

Bool XkbApplyCompatMapToKey(
    XkbDescPtr xkb,
#if NeedWidePrototypes
    unsigned int key,
#else
    KeyCode key,
#endif
    XkbChangesPtr changes) {
  return XkbApplyCompatMapToKey_impl(xkb, key, changes);
}

Bool XkbUpdateMapFromCore(
    XkbDescPtr xkb,
#if NeedWidePrototypes
    unsigned int first_key,
#else
    KeyCode first_key,
#endif
    int num_keys,
    int map_width,
    KeySym* core_keysyms,
    XkbChangesPtr changes) {
  return XkbUpdateMapFromCore_impl(xkb, first_key, num_keys, map_width, core_keysyms, changes);
}

XkbDeviceLedInfoPtr XkbAddDeviceLedInfo(XkbDeviceInfoPtr devi, unsigned int ledClass, unsigned int ledId) {
  return XkbAddDeviceLedInfo_impl(devi, ledClass, ledId);
}

Status XkbResizeDeviceButtonActions(XkbDeviceInfoPtr devi, unsigned int newTotal) {
  return XkbResizeDeviceButtonActions_impl(devi, newTotal);
}

XkbDeviceInfoPtr XkbAllocDeviceInfo(unsigned int deviceSpec, unsigned int nButtons, unsigned int szLeds) {
  return XkbAllocDeviceInfo_impl(deviceSpec, nButtons, szLeds);
}

void XkbFreeDeviceInfo(XkbDeviceInfoPtr devi, unsigned int which, Bool freeDevI) {
  return XkbFreeDeviceInfo_impl(devi, which, freeDevI);
}

void XkbNoteDeviceChanges(XkbDeviceChangesPtr old, XkbExtensionDeviceNotifyEvent* new_, unsigned int wanted) {
  return XkbNoteDeviceChanges_impl(old, new_, wanted);
}

XkbDeviceInfoPtr XkbGetDeviceInfo(
    Display* dpy,
    unsigned int which,
    unsigned int deviceSpec,
    unsigned int ledClass,
    unsigned int ledID) {
  return XkbGetDeviceInfo_impl(dpy, which, deviceSpec, ledClass, ledID);
}

Status XkbGetDeviceInfoChanges(Display* dpy, XkbDeviceInfoPtr devi, XkbDeviceChangesPtr changes) {
  return XkbGetDeviceInfoChanges_impl(dpy, devi, changes);
}

Status XkbGetDeviceButtonActions(Display* dpy, XkbDeviceInfoPtr devi, Bool all, unsigned int first, unsigned int nBtns) {
  return XkbGetDeviceButtonActions_impl(dpy, devi, all, first, nBtns);
}

Status XkbGetDeviceLedInfo(
    Display* dpy,
    XkbDeviceInfoPtr devi,
    unsigned int ledClass,
    unsigned int ledId,
    unsigned int which) {
  return XkbGetDeviceLedInfo_impl(dpy, devi, ledClass, ledId, which);
}

Bool XkbSetDeviceInfo(Display* dpy, unsigned int which, XkbDeviceInfoPtr devi) {
  return XkbSetDeviceInfo_impl(dpy, which, devi);
}

Bool XkbChangeDeviceInfo(Display* dpy, XkbDeviceInfoPtr desc, XkbDeviceChangesPtr changes) {
  return XkbChangeDeviceInfo_impl(dpy, desc, changes);
}

Bool XkbSetDeviceLedInfo(
    Display* dpy,
    XkbDeviceInfoPtr devi,
    unsigned int ledClass,
    unsigned int ledID,
    unsigned int which) {
  return XkbSetDeviceLedInfo_impl(dpy, devi, ledClass, ledID, which);
}

Bool XkbSetDeviceButtonActions(Display* dpy, XkbDeviceInfoPtr devi, unsigned int first, unsigned int nBtns) {
  return XkbSetDeviceButtonActions_impl(dpy, devi, first, nBtns);
}

char XkbToControl(char c) {
  return XkbToControl_impl(c);
}

Bool XkbSetDebuggingFlags(
    Display* dpy,
    unsigned int mask,
    unsigned int flags,
    char* msg,
    unsigned int ctrls_mask,
    unsigned int ctrls,
    unsigned int* rtrn_flags,
    unsigned int* rtrn_ctrls) {
  return XkbSetDebuggingFlags_impl(dpy, mask, flags, msg, ctrls_mask, ctrls, rtrn_flags, rtrn_ctrls);
}

Bool XkbApplyVirtualModChanges(XkbDescPtr xkb, unsigned int changed, XkbChangesPtr changes) {
  return XkbApplyVirtualModChanges_impl(xkb, changed, changes);
}

Bool XkbUpdateActionVirtualMods(XkbDescPtr xkb, XkbAction* act, unsigned int changed) {
  return XkbUpdateActionVirtualMods_impl(xkb, act, changed);
}

void XkbUpdateKeyTypeVirtualMods(XkbDescPtr xkb, XkbKeyTypePtr type, unsigned int changed, XkbChangesPtr changes) {
  return XkbUpdateKeyTypeVirtualMods_impl(xkb, type, changed, changes);
}

// Xutil.h
#ifdef XUTIL_DEFINE_FUNCTIONS
int XDestroyImage(XImage* ximage) {
  return XDestroyImage_impl(ximage);
}
#endif

#ifdef XUTIL_DEFINE_FUNCTIONS
unsigned long XGetPixel(XImage* ximage, int x, int y) {
  return XGetPixel_impl(ximage, x, y);
}
#endif

#ifdef XUTIL_DEFINE_FUNCTIONS
int XPutPixel(XImage* ximage, int x, int y, unsigned long pixel) {
  return XPutPixel_impl(ximage, x, y, pixel);
}
#endif

#ifdef XUTIL_DEFINE_FUNCTIONS
XImage* XSubImage(XImage* ximage, int x, int y, unsigned int width, unsigned int height) {
  return XSubImage_impl(ximage, x, y, width, height);
}
#endif

#ifdef XUTIL_DEFINE_FUNCTIONS
int XAddPixel(XImage* ximage, long value) {
  return XAddPixel_impl(ximage, value);
}
#endif

XClassHint* XAllocClassHint(void) {
  return XAllocClassHint_impl();
}

XIconSize* XAllocIconSize(void) {
  return XAllocIconSize_impl();
}

XSizeHints* XAllocSizeHints(void) {
  return XAllocSizeHints_impl();
}

XStandardColormap* XAllocStandardColormap(void) {
  return XAllocStandardColormap_impl();
}

XWMHints* XAllocWMHints(void) {
  return XAllocWMHints_impl();
}

int XClipBox(Region r, XRectangle* rect_return) {
  return XClipBox_impl(r, rect_return);
}

Region XCreateRegion(void) {
  return XCreateRegion_impl();
}

const char* XDefaultString(void) {
  return XDefaultString_impl();
}

int XDeleteContext(Display* display, XID rid, XContext context) {
  return XDeleteContext_impl(display, rid, context);
}

int XDestroyRegion(Region r) {
  return XDestroyRegion_impl(r);
}

int XEmptyRegion(Region r) {
  return XEmptyRegion_impl(r);
}

int XEqualRegion(Region r1, Region r2) {
  return XEqualRegion_impl(r1, r2);
}

int XFindContext(Display* display, XID rid, XContext context, XPointer* data_return) {
  return XFindContext_impl(display, rid, context, data_return);
}

Status XGetClassHint(Display* display, Window w, XClassHint* class_hints_return) {
  return XGetClassHint_impl(display, w, class_hints_return);
}

Status XGetIconSizes(Display* display, Window w, XIconSize** size_list_return, int* count_return) {
  return XGetIconSizes_impl(display, w, size_list_return, count_return);
}

Status XGetNormalHints(Display* display, Window w, XSizeHints* hints_return) {
  return XGetNormalHints_impl(display, w, hints_return);
}

Status XGetRGBColormaps(
    Display* display,
    Window w,
    XStandardColormap** stdcmap_return,
    int* count_return,
    Atom property) {
  return XGetRGBColormaps_impl(display, w, stdcmap_return, count_return, property);
}

Status XGetSizeHints(Display* display, Window w, XSizeHints* hints_return, Atom property) {
  return XGetSizeHints_impl(display, w, hints_return, property);
}

Status XGetStandardColormap(Display* display, Window w, XStandardColormap* colormap_return, Atom property) {
  return XGetStandardColormap_impl(display, w, colormap_return, property);
}

Status XGetTextProperty(Display* display, Window window, XTextProperty* text_prop_return, Atom property) {
  return XGetTextProperty_impl(display, window, text_prop_return, property);
}

XVisualInfo* XGetVisualInfo(Display* display, long vinfo_mask, XVisualInfo* vinfo_template, int* nitems_return) {
  return XGetVisualInfo_impl(display, vinfo_mask, vinfo_template, nitems_return);
}

Status XGetWMClientMachine(Display* display, Window w, XTextProperty* text_prop_return) {
  return XGetWMClientMachine_impl(display, w, text_prop_return);
}

XWMHints* XGetWMHints(Display* display, Window w) {
  return XGetWMHints_impl(display, w);
}

Status XGetWMIconName(Display* display, Window w, XTextProperty* text_prop_return) {
  return XGetWMIconName_impl(display, w, text_prop_return);
}

Status XGetWMName(Display* display, Window w, XTextProperty* text_prop_return) {
  return XGetWMName_impl(display, w, text_prop_return);
}

Status XGetWMNormalHints(Display* display, Window w, XSizeHints* hints_return, long* supplied_return) {
  return XGetWMNormalHints_impl(display, w, hints_return, supplied_return);
}

Status XGetWMSizeHints(Display* display, Window w, XSizeHints* hints_return, long* supplied_return, Atom property) {
  return XGetWMSizeHints_impl(display, w, hints_return, supplied_return, property);
}

Status XGetZoomHints(Display* display, Window w, XSizeHints* zhints_return) {
  return XGetZoomHints_impl(display, w, zhints_return);
}

int XIntersectRegion(Region sra, Region srb, Region dr_return) {
  return XIntersectRegion_impl(sra, srb, dr_return);
}

void XConvertCase(KeySym sym, KeySym* lower, KeySym* upper) {
  return XConvertCase_impl(sym, lower, upper);
}

int XLookupString(
    XKeyEvent* event_struct,
    char* buffer_return,
    int bytes_buffer,
    KeySym* keysym_return,
    XComposeStatus* status_in_out) {
  return XLookupString_impl(event_struct, buffer_return, bytes_buffer, keysym_return, status_in_out);
}

Status XMatchVisualInfo(Display* display, int screen, int depth, int class_, XVisualInfo* vinfo_return) {
  return XMatchVisualInfo_impl(display, screen, depth, class_, vinfo_return);
}

int XOffsetRegion(Region r, int dx, int dy) {
  return XOffsetRegion_impl(r, dx, dy);
}

Bool XPointInRegion(Region r, int x, int y) {
  return XPointInRegion_impl(r, x, y);
}

Region XPolygonRegion(XPoint* points, int n, int fill_rule) {
  return XPolygonRegion_impl(points, n, fill_rule);
}

int XRectInRegion(Region r, int x, int y, unsigned int width, unsigned int height) {
  return XRectInRegion_impl(r, x, y, width, height);
}

int XSaveContext(Display* display, XID rid, XContext context, _Xconst char* data) {
  return XSaveContext_impl(display, rid, context, data);
}

int XSetClassHint(Display* display, Window w, XClassHint* class_hints) {
  return XSetClassHint_impl(display, w, class_hints);
}

int XSetIconSizes(Display* display, Window w, XIconSize* size_list, int count) {
  return XSetIconSizes_impl(display, w, size_list, count);
}

int XSetNormalHints(Display* display, Window w, XSizeHints* hints) {
  return XSetNormalHints_impl(display, w, hints);
}

void XSetRGBColormaps(Display* display, Window w, XStandardColormap* stdcmaps, int count, Atom property) {
  return XSetRGBColormaps_impl(display, w, stdcmaps, count, property);
}

int XSetSizeHints(Display* display, Window w, XSizeHints* hints, Atom property) {
  return XSetSizeHints_impl(display, w, hints, property);
}

int XSetStandardProperties(
    Display* display,
    Window w,
    _Xconst char* window_name,
    _Xconst char* icon_name,
    Pixmap icon_pixmap,
    char** argv,
    int argc,
    XSizeHints* hints) {
  return XSetStandardProperties_impl(display, w, window_name, icon_name, icon_pixmap, argv, argc, hints);
}

void XSetTextProperty(Display* display, Window w, XTextProperty* text_prop, Atom property) {
  return XSetTextProperty_impl(display, w, text_prop, property);
}

void XSetWMClientMachine(Display* display, Window w, XTextProperty* text_prop) {
  return XSetWMClientMachine_impl(display, w, text_prop);
}

int XSetWMHints(Display* display, Window w, XWMHints* wm_hints) {
  return XSetWMHints_impl(display, w, wm_hints);
}

void XSetWMIconName(Display* display, Window w, XTextProperty* text_prop) {
  return XSetWMIconName_impl(display, w, text_prop);
}

void XSetWMName(Display* display, Window w, XTextProperty* text_prop) {
  return XSetWMName_impl(display, w, text_prop);
}

void XSetWMNormalHints(Display* display, Window w, XSizeHints* hints) {
  return XSetWMNormalHints_impl(display, w, hints);
}

void XSetWMProperties(
    Display* display,
    Window w,
    XTextProperty* window_name,
    XTextProperty* icon_name,
    char** argv,
    int argc,
    XSizeHints* normal_hints,
    XWMHints* wm_hints,
    XClassHint* class_hints) {
  return XSetWMProperties_impl(display, w, window_name, icon_name, argv, argc, normal_hints, wm_hints, class_hints);
}

void XmbSetWMProperties(
    Display* display,
    Window w,
    _Xconst char* window_name,
    _Xconst char* icon_name,
    char** argv,
    int argc,
    XSizeHints* normal_hints,
    XWMHints* wm_hints,
    XClassHint* class_hints) {
  return XmbSetWMProperties_impl(display, w, window_name, icon_name, argv, argc, normal_hints, wm_hints, class_hints);
}

void Xutf8SetWMProperties(
    Display* display,
    Window w,
    _Xconst char* window_name,
    _Xconst char* icon_name,
    char** argv,
    int argc,
    XSizeHints* normal_hints,
    XWMHints* wm_hints,
    XClassHint* class_hints) {
  return Xutf8SetWMProperties_impl(display, w, window_name, icon_name, argv, argc, normal_hints, wm_hints, class_hints);
}

void XSetWMSizeHints(Display* display, Window w, XSizeHints* hints, Atom property) {
  return XSetWMSizeHints_impl(display, w, hints, property);
}

int XSetRegion(Display* display, GC gc, Region r) {
  return XSetRegion_impl(display, gc, r);
}

void XSetStandardColormap(Display* display, Window w, XStandardColormap* colormap, Atom property) {
  return XSetStandardColormap_impl(display, w, colormap, property);
}

int XSetZoomHints(Display* display, Window w, XSizeHints* zhints) {
  return XSetZoomHints_impl(display, w, zhints);
}

int XShrinkRegion(Region r, int dx, int dy) {
  return XShrinkRegion_impl(r, dx, dy);
}

Status XStringListToTextProperty(char** list, int count, XTextProperty* text_prop_return) {
  return XStringListToTextProperty_impl(list, count, text_prop_return);
}

int XSubtractRegion(Region sra, Region srb, Region dr_return) {
  return XSubtractRegion_impl(sra, srb, dr_return);
}

int XmbTextListToTextProperty(
    Display* display,
    char** list,
    int count,
    XICCEncodingStyle style,
    XTextProperty* text_prop_return) {
  return XmbTextListToTextProperty_impl(display, list, count, style, text_prop_return);
}

int XwcTextListToTextProperty(
    Display* display,
    wchar_t** list,
    int count,
    XICCEncodingStyle style,
    XTextProperty* text_prop_return) {
  return XwcTextListToTextProperty_impl(display, list, count, style, text_prop_return);
}

int Xutf8TextListToTextProperty(
    Display* display,
    char** list,
    int count,
    XICCEncodingStyle style,
    XTextProperty* text_prop_return) {
  return Xutf8TextListToTextProperty_impl(display, list, count, style, text_prop_return);
}

void XwcFreeStringList(wchar_t** list) {
  return XwcFreeStringList_impl(list);
}

Status XTextPropertyToStringList(XTextProperty* text_prop, char*** list_return, int* count_return) {
  return XTextPropertyToStringList_impl(text_prop, list_return, count_return);
}

int XmbTextPropertyToTextList(
    Display* display,
    const XTextProperty* text_prop,
    char*** list_return,
    int* count_return) {
  return XmbTextPropertyToTextList_impl(display, text_prop, list_return, count_return);
}

int XwcTextPropertyToTextList(
    Display* display,
    const XTextProperty* text_prop,
    wchar_t*** list_return,
    int* count_return) {
  return XwcTextPropertyToTextList_impl(display, text_prop, list_return, count_return);
}

int Xutf8TextPropertyToTextList(
    Display* display,
    const XTextProperty* text_prop,
    char*** list_return,
    int* count_return) {
  return Xutf8TextPropertyToTextList_impl(display, text_prop, list_return, count_return);
}

int XUnionRectWithRegion(XRectangle* rectangle, Region src_region, Region dest_region_return) {
  return XUnionRectWithRegion_impl(rectangle, src_region, dest_region_return);
}

int XUnionRegion(Region sra, Region srb, Region dr_return) {
  return XUnionRegion_impl(sra, srb, dr_return);
}

int XWMGeometry(
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
    int* gravity_return) {
  return XWMGeometry_impl(display, screen_number, user_geometry, default_geometry, border_width, hints, x_return, y_return, width_return, height_return, gravity_return);
}

int XXorRegion(Region sra, Region srb, Region dr_return) {
  return XXorRegion_impl(sra, srb, dr_return);
}

// Xcursor.h
XcursorImage* XcursorImageCreate(int width, int height) {
  return XcursorImageCreate_impl(width, height);
}

void XcursorImageDestroy(XcursorImage* image) {
  return XcursorImageDestroy_impl(image);
}

XcursorImages* XcursorImagesCreate(int size) {
  return XcursorImagesCreate_impl(size);
}

void XcursorImagesDestroy(XcursorImages* images) {
  return XcursorImagesDestroy_impl(images);
}

void XcursorImagesSetName(XcursorImages* images, const char* name) {
  return XcursorImagesSetName_impl(images, name);
}

XcursorCursors* XcursorCursorsCreate(Display* dpy, int size) {
  return XcursorCursorsCreate_impl(dpy, size);
}

void XcursorCursorsDestroy(XcursorCursors* cursors) {
  return XcursorCursorsDestroy_impl(cursors);
}

XcursorAnimate* XcursorAnimateCreate(XcursorCursors* cursors) {
  return XcursorAnimateCreate_impl(cursors);
}

void XcursorAnimateDestroy(XcursorAnimate* animate) {
  return XcursorAnimateDestroy_impl(animate);
}

Cursor XcursorAnimateNext(XcursorAnimate* animate) {
  return XcursorAnimateNext_impl(animate);
}

XcursorComment* XcursorCommentCreate(XcursorUInt comment_type, int length) {
  return XcursorCommentCreate_impl(comment_type, length);
}

void XcursorCommentDestroy(XcursorComment* comment) {
  return XcursorCommentDestroy_impl(comment);
}

XcursorComments* XcursorCommentsCreate(int size) {
  return XcursorCommentsCreate_impl(size);
}

void XcursorCommentsDestroy(XcursorComments* comments) {
  return XcursorCommentsDestroy_impl(comments);
}

XcursorImage* XcursorXcFileLoadImage(XcursorFile* file, int size) {
  return XcursorXcFileLoadImage_impl(file, size);
}

XcursorImages* XcursorXcFileLoadImages(XcursorFile* file, int size) {
  return XcursorXcFileLoadImages_impl(file, size);
}

XcursorImages* XcursorXcFileLoadAllImages(XcursorFile* file) {
  return XcursorXcFileLoadAllImages_impl(file);
}

XcursorBool XcursorXcFileLoad(XcursorFile* file, XcursorComments** commentsp, XcursorImages** imagesp) {
  return XcursorXcFileLoad_impl(file, commentsp, imagesp);
}

XcursorBool XcursorXcFileSave(XcursorFile* file, const XcursorComments* comments, const XcursorImages* images) {
  return XcursorXcFileSave_impl(file, comments, images);
}

XcursorImage* XcursorFileLoadImage(FILE* file, int size) {
  return XcursorFileLoadImage_impl(file, size);
}

XcursorImages* XcursorFileLoadImages(FILE* file, int size) {
  return XcursorFileLoadImages_impl(file, size);
}

XcursorImages* XcursorFileLoadAllImages(FILE* file) {
  return XcursorFileLoadAllImages_impl(file);
}

XcursorBool XcursorFileLoad(FILE* file, XcursorComments** commentsp, XcursorImages** imagesp) {
  return XcursorFileLoad_impl(file, commentsp, imagesp);
}

XcursorBool XcursorFileSaveImages(FILE* file, const XcursorImages* images) {
  return XcursorFileSaveImages_impl(file, images);
}

XcursorBool XcursorFileSave(FILE* file, const XcursorComments* comments, const XcursorImages* images) {
  return XcursorFileSave_impl(file, comments, images);
}

XcursorImage* XcursorFilenameLoadImage(const char* filename, int size) {
  return XcursorFilenameLoadImage_impl(filename, size);
}

XcursorImages* XcursorFilenameLoadImages(const char* filename, int size) {
  return XcursorFilenameLoadImages_impl(filename, size);
}

XcursorImages* XcursorFilenameLoadAllImages(const char* filename) {
  return XcursorFilenameLoadAllImages_impl(filename);
}

XcursorBool XcursorFilenameLoad(const char* file, XcursorComments** commentsp, XcursorImages** imagesp) {
  return XcursorFilenameLoad_impl(file, commentsp, imagesp);
}

XcursorBool XcursorFilenameSaveImages(const char* filename, const XcursorImages* images) {
  return XcursorFilenameSaveImages_impl(filename, images);
}

XcursorBool XcursorFilenameSave(const char* file, const XcursorComments* comments, const XcursorImages* images) {
  return XcursorFilenameSave_impl(file, comments, images);
}

XcursorImage* XcursorLibraryLoadImage(const char* library, const char* theme, int size) {
  return XcursorLibraryLoadImage_impl(library, theme, size);
}

XcursorImages* XcursorLibraryLoadImages(const char* library, const char* theme, int size) {
  return XcursorLibraryLoadImages_impl(library, theme, size);
}

const char* XcursorLibraryPath(void) {
  return XcursorLibraryPath_impl();
}

int XcursorLibraryShape(const char* library) {
  return XcursorLibraryShape_impl(library);
}

Cursor XcursorImageLoadCursor(Display* dpy, const XcursorImage* image) {
  return XcursorImageLoadCursor_impl(dpy, image);
}

XcursorCursors* XcursorImagesLoadCursors(Display* dpy, const XcursorImages* images) {
  return XcursorImagesLoadCursors_impl(dpy, images);
}

Cursor XcursorImagesLoadCursor(Display* dpy, const XcursorImages* images) {
  return XcursorImagesLoadCursor_impl(dpy, images);
}

Cursor XcursorFilenameLoadCursor(Display* dpy, const char* file) {
  return XcursorFilenameLoadCursor_impl(dpy, file);
}

XcursorCursors* XcursorFilenameLoadCursors(Display* dpy, const char* file) {
  return XcursorFilenameLoadCursors_impl(dpy, file);
}

Cursor XcursorLibraryLoadCursor(Display* dpy, const char* file) {
  return XcursorLibraryLoadCursor_impl(dpy, file);
}

XcursorCursors* XcursorLibraryLoadCursors(Display* dpy, const char* file) {
  return XcursorLibraryLoadCursors_impl(dpy, file);
}

XcursorImage* XcursorShapeLoadImage(unsigned int shape, const char* theme, int size) {
  return XcursorShapeLoadImage_impl(shape, theme, size);
}

XcursorImages* XcursorShapeLoadImages(unsigned int shape, const char* theme, int size) {
  return XcursorShapeLoadImages_impl(shape, theme, size);
}

Cursor XcursorShapeLoadCursor(Display* dpy, unsigned int shape) {
  return XcursorShapeLoadCursor_impl(dpy, shape);
}

XcursorCursors* XcursorShapeLoadCursors(Display* dpy, unsigned int shape) {
  return XcursorShapeLoadCursors_impl(dpy, shape);
}

Cursor XcursorTryShapeCursor(
    Display* dpy,
    Font source_font,
    Font mask_font,
    unsigned int source_char,
    unsigned int mask_char,
    XColor _Xconst* foreground,
    XColor _Xconst* background) {
  return XcursorTryShapeCursor_impl(dpy, source_font, mask_font, source_char, mask_char, foreground, background);
}

void XcursorNoticeCreateBitmap(Display* dpy, Pixmap pid, unsigned int width, unsigned int height) {
  return XcursorNoticeCreateBitmap_impl(dpy, pid, width, height);
}

void XcursorNoticePutBitmap(Display* dpy, Drawable draw, XImage* image) {
  return XcursorNoticePutBitmap_impl(dpy, draw, image);
}

Cursor XcursorTryShapeBitmapCursor(
    Display* dpy,
    Pixmap source,
    Pixmap mask,
    XColor* foreground,
    XColor* background,
    unsigned int x,
    unsigned int y) {
  return XcursorTryShapeBitmapCursor_impl(dpy, source, mask, foreground, background, x, y);
}

void XcursorImageHash(XImage* image, unsigned char hash [XCURSOR_BITMAP_HASH_SIZE]) {
  return XcursorImageHash_impl(image, hash);
}

XcursorBool XcursorSupportsARGB(Display* dpy) {
  return XcursorSupportsARGB_impl(dpy);
}

XcursorBool XcursorSupportsAnim(Display* dpy) {
  return XcursorSupportsAnim_impl(dpy);
}

XcursorBool XcursorSetDefaultSize(Display* dpy, int size) {
  return XcursorSetDefaultSize_impl(dpy, size);
}

int XcursorGetDefaultSize(Display* dpy) {
  return XcursorGetDefaultSize_impl(dpy);
}

XcursorBool XcursorSetTheme(Display* dpy, const char* theme) {
  return XcursorSetTheme_impl(dpy, theme);
}

char* XcursorGetTheme(Display* dpy) {
  return XcursorGetTheme_impl(dpy);
}

XcursorBool XcursorGetThemeCore(Display* dpy) {
  return XcursorGetThemeCore_impl(dpy);
}

XcursorBool XcursorSetThemeCore(Display* dpy, XcursorBool theme_core) {
  return XcursorSetThemeCore_impl(dpy, theme_core);
}

// Xinerama.h
Bool XineramaQueryExtension(Display* dpy, int* event_base, int* error_base) {
  return XineramaQueryExtension_impl(dpy, event_base, error_base);
}

Status XineramaQueryVersion(Display* dpy, int* major_versionp, int* minor_versionp) {
  return XineramaQueryVersion_impl(dpy, major_versionp, minor_versionp);
}

Bool XineramaIsActive(Display* dpy) {
  return XineramaIsActive_impl(dpy);
}

XineramaScreenInfo* XineramaQueryScreens(Display* dpy, int* number) {
  return XineramaQueryScreens_impl(dpy, number);
}



////////////////////////////////////////////////////////////////////////////////
// Main wrangling logic.

static void atExitHandler(void) {
  if (x11_lib != NULL) {
    // Currently we ignore errors.
    // TODO(sergey): It might be a good idea to print errors to stderr at least?
    dynamic_library_close(x11_lib);
    x11_lib = NULL;
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
  x11_lib = xew_dynamic_library_open_find(x11_paths);
  if (x11_lib == NULL) {
    return XEW_ERROR_OPEN_FAILED;
  }
  x11_xcb_lib = xew_dynamic_library_open_find(x11_xcb_paths);
  if (x11_xcb_lib == NULL) {
    return XEW_ERROR_OPEN_FAILED;
  }
  x11_xinerama_lib = xew_dynamic_library_open_find(x11_xinerama_paths);
  x11_cursor_lib = xew_dynamic_library_open_find(x11_cursor_paths);
  return XEW_SUCCESS;
}

static void fetchPointersFromLibrary(void) {
  // Xlib.h
  _LIBRARY_FIND_IMPL(x11_lib, _Xmblen);
  _LIBRARY_FIND_IMPL(x11_lib, XLoadQueryFont);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryFont);
  _LIBRARY_FIND_IMPL(x11_lib, XGetMotionEvents);
  _LIBRARY_FIND_IMPL(x11_lib, XDeleteModifiermapEntry);
  _LIBRARY_FIND_IMPL(x11_lib, XGetModifierMapping);
  _LIBRARY_FIND_IMPL(x11_lib, XInsertModifiermapEntry);
  _LIBRARY_FIND_IMPL(x11_lib, XNewModifiermap);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateImage);
  _LIBRARY_FIND_IMPL(x11_lib, XInitImage);
  _LIBRARY_FIND_IMPL(x11_lib, XGetImage);
  _LIBRARY_FIND_IMPL(x11_lib, XGetSubImage);
  _LIBRARY_FIND_IMPL(x11_lib, XOpenDisplay);
  _LIBRARY_FIND_IMPL(x11_lib, XrmInitialize);
  _LIBRARY_FIND_IMPL(x11_lib, XFetchBytes);
  _LIBRARY_FIND_IMPL(x11_lib, XFetchBuffer);
  _LIBRARY_FIND_IMPL(x11_lib, XGetAtomName);
  _LIBRARY_FIND_IMPL(x11_lib, XGetAtomNames);
  _LIBRARY_FIND_IMPL(x11_lib, XGetDefault);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayName);
  _LIBRARY_FIND_IMPL(x11_lib, XKeysymToString);
  _LIBRARY_FIND_IMPL(x11_lib, XSynchronize);
  _LIBRARY_FIND_IMPL(x11_lib, XInternAtom);
  _LIBRARY_FIND_IMPL(x11_lib, XInternAtoms);
  _LIBRARY_FIND_IMPL(x11_lib, XCopyColormapAndFree);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XCreatePixmapCursor);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateGlyphCursor);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateFontCursor);
  _LIBRARY_FIND_IMPL(x11_lib, XLoadFont);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateGC);
  _LIBRARY_FIND_IMPL(x11_lib, XGContextFromGC);
  _LIBRARY_FIND_IMPL(x11_lib, XFlushGC);
  _LIBRARY_FIND_IMPL(x11_lib, XCreatePixmap);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateBitmapFromData);
  _LIBRARY_FIND_IMPL(x11_lib, XCreatePixmapFromBitmapData);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateSimpleWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XGetSelectionOwner);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XListInstalledColormaps);
  _LIBRARY_FIND_IMPL(x11_lib, XListFonts);
  _LIBRARY_FIND_IMPL(x11_lib, XListFontsWithInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XGetFontPath);
  _LIBRARY_FIND_IMPL(x11_lib, XListExtensions);
  _LIBRARY_FIND_IMPL(x11_lib, XListProperties);
  _LIBRARY_FIND_IMPL(x11_lib, XListHosts);
  _LIBRARY_FIND_IMPL(x11_lib, XKeycodeToKeysym);
  _LIBRARY_FIND_IMPL(x11_lib, XLookupKeysym);
  _LIBRARY_FIND_IMPL(x11_lib, XGetKeyboardMapping);
  _LIBRARY_FIND_IMPL(x11_lib, XStringToKeysym);
  _LIBRARY_FIND_IMPL(x11_lib, XMaxRequestSize);
  _LIBRARY_FIND_IMPL(x11_lib, XExtendedMaxRequestSize);
  _LIBRARY_FIND_IMPL(x11_lib, XResourceManagerString);
  _LIBRARY_FIND_IMPL(x11_lib, XScreenResourceString);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayMotionBufferSize);
  _LIBRARY_FIND_IMPL(x11_lib, XVisualIDFromVisual);
  _LIBRARY_FIND_IMPL(x11_lib, XInitThreads);
  _LIBRARY_FIND_IMPL(x11_lib, XLockDisplay);
  _LIBRARY_FIND_IMPL(x11_lib, XUnlockDisplay);
  _LIBRARY_FIND_IMPL(x11_lib, XInitExtension);
  _LIBRARY_FIND_IMPL(x11_lib, XAddExtension);
  _LIBRARY_FIND_IMPL(x11_lib, XFindOnExtensionList);
  _LIBRARY_FIND_IMPL(x11_lib, XEHeadOfExtensionList);
  _LIBRARY_FIND_IMPL(x11_lib, XRootWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultRootWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XRootWindowOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultVisual);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultVisualOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultGC);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultGCOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XBlackPixel);
  _LIBRARY_FIND_IMPL(x11_lib, XWhitePixel);
  _LIBRARY_FIND_IMPL(x11_lib, XAllPlanes);
  _LIBRARY_FIND_IMPL(x11_lib, XBlackPixelOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XWhitePixelOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XNextRequest);
  _LIBRARY_FIND_IMPL(x11_lib, XLastKnownRequestProcessed);
  _LIBRARY_FIND_IMPL(x11_lib, XServerVendor);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayString);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultColormapOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XScreenOfDisplay);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultScreenOfDisplay);
  _LIBRARY_FIND_IMPL(x11_lib, XEventMaskOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XScreenNumberOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XSetErrorHandler);
  _LIBRARY_FIND_IMPL(x11_lib, XSetIOErrorHandler);
  _LIBRARY_FIND_IMPL(x11_lib, XListPixmapFormats);
  _LIBRARY_FIND_IMPL(x11_lib, XListDepths);
  _LIBRARY_FIND_IMPL(x11_lib, XReconfigureWMWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWMProtocols);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMProtocols);
  _LIBRARY_FIND_IMPL(x11_lib, XIconifyWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XWithdrawWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XGetCommand);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWMColormapWindows);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMColormapWindows);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeStringList);
  _LIBRARY_FIND_IMPL(x11_lib, XSetTransientForHint);
  _LIBRARY_FIND_IMPL(x11_lib, XActivateScreenSaver);
  _LIBRARY_FIND_IMPL(x11_lib, XAddHost);
  _LIBRARY_FIND_IMPL(x11_lib, XAddHosts);
  _LIBRARY_FIND_IMPL(x11_lib, XAddToExtensionList);
  _LIBRARY_FIND_IMPL(x11_lib, XAddToSaveSet);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocColor);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocColorCells);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocColorPlanes);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocNamedColor);
  _LIBRARY_FIND_IMPL(x11_lib, XAllowEvents);
  _LIBRARY_FIND_IMPL(x11_lib, XAutoRepeatOff);
  _LIBRARY_FIND_IMPL(x11_lib, XAutoRepeatOn);
  _LIBRARY_FIND_IMPL(x11_lib, XBell);
  _LIBRARY_FIND_IMPL(x11_lib, XBitmapBitOrder);
  _LIBRARY_FIND_IMPL(x11_lib, XBitmapPad);
  _LIBRARY_FIND_IMPL(x11_lib, XBitmapUnit);
  _LIBRARY_FIND_IMPL(x11_lib, XCellsOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XChangeActivePointerGrab);
  _LIBRARY_FIND_IMPL(x11_lib, XChangeGC);
  _LIBRARY_FIND_IMPL(x11_lib, XChangeKeyboardControl);
  _LIBRARY_FIND_IMPL(x11_lib, XChangeKeyboardMapping);
  _LIBRARY_FIND_IMPL(x11_lib, XChangePointerControl);
  _LIBRARY_FIND_IMPL(x11_lib, XChangeProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XChangeSaveSet);
  _LIBRARY_FIND_IMPL(x11_lib, XChangeWindowAttributes);
  _LIBRARY_FIND_IMPL(x11_lib, XCheckIfEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XCheckMaskEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XCheckTypedEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XCheckTypedWindowEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XCheckWindowEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XCirculateSubwindows);
  _LIBRARY_FIND_IMPL(x11_lib, XCirculateSubwindowsDown);
  _LIBRARY_FIND_IMPL(x11_lib, XCirculateSubwindowsUp);
  _LIBRARY_FIND_IMPL(x11_lib, XClearArea);
  _LIBRARY_FIND_IMPL(x11_lib, XClearWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XCloseDisplay);
  _LIBRARY_FIND_IMPL(x11_lib, XConfigureWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XConnectionNumber);
  _LIBRARY_FIND_IMPL(x11_lib, XConvertSelection);
  _LIBRARY_FIND_IMPL(x11_lib, XCopyArea);
  _LIBRARY_FIND_IMPL(x11_lib, XCopyGC);
  _LIBRARY_FIND_IMPL(x11_lib, XCopyPlane);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultDepth);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultDepthOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XDefineCursor);
  _LIBRARY_FIND_IMPL(x11_lib, XDeleteProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XDestroyWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XDestroySubwindows);
  _LIBRARY_FIND_IMPL(x11_lib, XDoesBackingStore);
  _LIBRARY_FIND_IMPL(x11_lib, XDoesSaveUnders);
  _LIBRARY_FIND_IMPL(x11_lib, XDisableAccessControl);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayCells);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayHeight);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayHeightMM);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayKeycodes);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayPlanes);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayWidth);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayWidthMM);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawArc);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawArcs);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawImageString);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawImageString16);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawLine);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawLines);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawPoint);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawPoints);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawRectangle);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawRectangles);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawSegments);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawString);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawString16);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawText);
  _LIBRARY_FIND_IMPL(x11_lib, XDrawText16);
  _LIBRARY_FIND_IMPL(x11_lib, XEnableAccessControl);
  _LIBRARY_FIND_IMPL(x11_lib, XEventsQueued);
  _LIBRARY_FIND_IMPL(x11_lib, XFetchName);
  _LIBRARY_FIND_IMPL(x11_lib, XFillArc);
  _LIBRARY_FIND_IMPL(x11_lib, XFillArcs);
  _LIBRARY_FIND_IMPL(x11_lib, XFillPolygon);
  _LIBRARY_FIND_IMPL(x11_lib, XFillRectangle);
  _LIBRARY_FIND_IMPL(x11_lib, XFillRectangles);
  _LIBRARY_FIND_IMPL(x11_lib, XFlush);
  _LIBRARY_FIND_IMPL(x11_lib, XForceScreenSaver);
  _LIBRARY_FIND_IMPL(x11_lib, XFree);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeColors);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeCursor);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeExtensionList);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeFont);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeFontInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeFontNames);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeFontPath);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeGC);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeModifiermap);
  _LIBRARY_FIND_IMPL(x11_lib, XFreePixmap);
  _LIBRARY_FIND_IMPL(x11_lib, XGeometry);
  _LIBRARY_FIND_IMPL(x11_lib, XGetErrorDatabaseText);
  _LIBRARY_FIND_IMPL(x11_lib, XGetErrorText);
  _LIBRARY_FIND_IMPL(x11_lib, XGetFontProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XGetGCValues);
  _LIBRARY_FIND_IMPL(x11_lib, XGetGeometry);
  _LIBRARY_FIND_IMPL(x11_lib, XGetIconName);
  _LIBRARY_FIND_IMPL(x11_lib, XGetInputFocus);
  _LIBRARY_FIND_IMPL(x11_lib, XGetKeyboardControl);
  _LIBRARY_FIND_IMPL(x11_lib, XGetPointerControl);
  _LIBRARY_FIND_IMPL(x11_lib, XGetPointerMapping);
  _LIBRARY_FIND_IMPL(x11_lib, XGetScreenSaver);
  _LIBRARY_FIND_IMPL(x11_lib, XGetTransientForHint);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWindowProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWindowAttributes);
  _LIBRARY_FIND_IMPL(x11_lib, XGrabButton);
  _LIBRARY_FIND_IMPL(x11_lib, XGrabKey);
  _LIBRARY_FIND_IMPL(x11_lib, XGrabKeyboard);
  _LIBRARY_FIND_IMPL(x11_lib, XGrabPointer);
  _LIBRARY_FIND_IMPL(x11_lib, XGrabServer);
  _LIBRARY_FIND_IMPL(x11_lib, XHeightMMOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XHeightOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XIfEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XImageByteOrder);
  _LIBRARY_FIND_IMPL(x11_lib, XInstallColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XKeysymToKeycode);
  _LIBRARY_FIND_IMPL(x11_lib, XKillClient);
  _LIBRARY_FIND_IMPL(x11_lib, XLookupColor);
  _LIBRARY_FIND_IMPL(x11_lib, XLowerWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XMapRaised);
  _LIBRARY_FIND_IMPL(x11_lib, XMapSubwindows);
  _LIBRARY_FIND_IMPL(x11_lib, XMapWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XMaskEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XMaxCmapsOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XMinCmapsOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XMoveResizeWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XMoveWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XNextEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XNoOp);
  _LIBRARY_FIND_IMPL(x11_lib, XParseColor);
  _LIBRARY_FIND_IMPL(x11_lib, XParseGeometry);
  _LIBRARY_FIND_IMPL(x11_lib, XPeekEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XPeekIfEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XPending);
  _LIBRARY_FIND_IMPL(x11_lib, XPlanesOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XProtocolRevision);
  _LIBRARY_FIND_IMPL(x11_lib, XProtocolVersion);
  _LIBRARY_FIND_IMPL(x11_lib, XPutBackEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XPutImage);
  _LIBRARY_FIND_IMPL(x11_lib, XQLength);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryBestCursor);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryBestSize);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryBestStipple);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryBestTile);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryColor);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryColors);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryExtension);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryKeymap);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryPointer);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryTextExtents);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryTextExtents16);
  _LIBRARY_FIND_IMPL(x11_lib, XQueryTree);
  _LIBRARY_FIND_IMPL(x11_lib, XRaiseWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XReadBitmapFile);
  _LIBRARY_FIND_IMPL(x11_lib, XReadBitmapFileData);
  _LIBRARY_FIND_IMPL(x11_lib, XRebindKeysym);
  _LIBRARY_FIND_IMPL(x11_lib, XRecolorCursor);
  _LIBRARY_FIND_IMPL(x11_lib, XRefreshKeyboardMapping);
  _LIBRARY_FIND_IMPL(x11_lib, XRemoveFromSaveSet);
  _LIBRARY_FIND_IMPL(x11_lib, XRemoveHost);
  _LIBRARY_FIND_IMPL(x11_lib, XRemoveHosts);
  _LIBRARY_FIND_IMPL(x11_lib, XReparentWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XResetScreenSaver);
  _LIBRARY_FIND_IMPL(x11_lib, XResizeWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XRestackWindows);
  _LIBRARY_FIND_IMPL(x11_lib, XRotateBuffers);
  _LIBRARY_FIND_IMPL(x11_lib, XRotateWindowProperties);
  _LIBRARY_FIND_IMPL(x11_lib, XScreenCount);
  _LIBRARY_FIND_IMPL(x11_lib, XSelectInput);
  _LIBRARY_FIND_IMPL(x11_lib, XSendEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XSetAccessControl);
  _LIBRARY_FIND_IMPL(x11_lib, XSetArcMode);
  _LIBRARY_FIND_IMPL(x11_lib, XSetBackground);
  _LIBRARY_FIND_IMPL(x11_lib, XSetClipMask);
  _LIBRARY_FIND_IMPL(x11_lib, XSetClipOrigin);
  _LIBRARY_FIND_IMPL(x11_lib, XSetClipRectangles);
  _LIBRARY_FIND_IMPL(x11_lib, XSetCloseDownMode);
  _LIBRARY_FIND_IMPL(x11_lib, XSetCommand);
  _LIBRARY_FIND_IMPL(x11_lib, XSetDashes);
  _LIBRARY_FIND_IMPL(x11_lib, XSetFillRule);
  _LIBRARY_FIND_IMPL(x11_lib, XSetFillStyle);
  _LIBRARY_FIND_IMPL(x11_lib, XSetFont);
  _LIBRARY_FIND_IMPL(x11_lib, XSetFontPath);
  _LIBRARY_FIND_IMPL(x11_lib, XSetForeground);
  _LIBRARY_FIND_IMPL(x11_lib, XSetFunction);
  _LIBRARY_FIND_IMPL(x11_lib, XSetGraphicsExposures);
  _LIBRARY_FIND_IMPL(x11_lib, XSetIconName);
  _LIBRARY_FIND_IMPL(x11_lib, XSetInputFocus);
  _LIBRARY_FIND_IMPL(x11_lib, XSetLineAttributes);
  _LIBRARY_FIND_IMPL(x11_lib, XSetModifierMapping);
  _LIBRARY_FIND_IMPL(x11_lib, XSetPlaneMask);
  _LIBRARY_FIND_IMPL(x11_lib, XSetPointerMapping);
  _LIBRARY_FIND_IMPL(x11_lib, XSetScreenSaver);
  _LIBRARY_FIND_IMPL(x11_lib, XSetSelectionOwner);
  _LIBRARY_FIND_IMPL(x11_lib, XSetState);
  _LIBRARY_FIND_IMPL(x11_lib, XSetStipple);
  _LIBRARY_FIND_IMPL(x11_lib, XSetSubwindowMode);
  _LIBRARY_FIND_IMPL(x11_lib, XSetTSOrigin);
  _LIBRARY_FIND_IMPL(x11_lib, XSetTile);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWindowBackground);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWindowBackgroundPixmap);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWindowBorder);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWindowBorderPixmap);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWindowBorderWidth);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWindowColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XStoreBuffer);
  _LIBRARY_FIND_IMPL(x11_lib, XStoreBytes);
  _LIBRARY_FIND_IMPL(x11_lib, XStoreColor);
  _LIBRARY_FIND_IMPL(x11_lib, XStoreColors);
  _LIBRARY_FIND_IMPL(x11_lib, XStoreName);
  _LIBRARY_FIND_IMPL(x11_lib, XStoreNamedColor);
  _LIBRARY_FIND_IMPL(x11_lib, XSync);
  _LIBRARY_FIND_IMPL(x11_lib, XTextExtents);
  _LIBRARY_FIND_IMPL(x11_lib, XTextExtents16);
  _LIBRARY_FIND_IMPL(x11_lib, XTextWidth);
  _LIBRARY_FIND_IMPL(x11_lib, XTextWidth16);
  _LIBRARY_FIND_IMPL(x11_lib, XTranslateCoordinates);
  _LIBRARY_FIND_IMPL(x11_lib, XUndefineCursor);
  _LIBRARY_FIND_IMPL(x11_lib, XUngrabButton);
  _LIBRARY_FIND_IMPL(x11_lib, XUngrabKey);
  _LIBRARY_FIND_IMPL(x11_lib, XUngrabKeyboard);
  _LIBRARY_FIND_IMPL(x11_lib, XUngrabPointer);
  _LIBRARY_FIND_IMPL(x11_lib, XUngrabServer);
  _LIBRARY_FIND_IMPL(x11_lib, XUninstallColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XUnloadFont);
  _LIBRARY_FIND_IMPL(x11_lib, XUnmapSubwindows);
  _LIBRARY_FIND_IMPL(x11_lib, XUnmapWindow);
  _LIBRARY_FIND_IMPL(x11_lib, XVendorRelease);
  _LIBRARY_FIND_IMPL(x11_lib, XWarpPointer);
  _LIBRARY_FIND_IMPL(x11_lib, XWidthMMOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XWidthOfScreen);
  _LIBRARY_FIND_IMPL(x11_lib, XWindowEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XWriteBitmapFile);
  _LIBRARY_FIND_IMPL(x11_lib, XSupportsLocale);
  _LIBRARY_FIND_IMPL(x11_lib, XSetLocaleModifiers);
  _LIBRARY_FIND_IMPL(x11_lib, XOpenOM);
  _LIBRARY_FIND_IMPL(x11_lib, XCloseOM);
  _LIBRARY_FIND_IMPL(x11_lib, XSetOMValues);
  _LIBRARY_FIND_IMPL(x11_lib, XGetOMValues);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayOfOM);
  _LIBRARY_FIND_IMPL(x11_lib, XLocaleOfOM);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateOC);
  _LIBRARY_FIND_IMPL(x11_lib, XDestroyOC);
  _LIBRARY_FIND_IMPL(x11_lib, XOMOfOC);
  _LIBRARY_FIND_IMPL(x11_lib, XSetOCValues);
  _LIBRARY_FIND_IMPL(x11_lib, XGetOCValues);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateFontSet);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeFontSet);
  _LIBRARY_FIND_IMPL(x11_lib, XFontsOfFontSet);
  _LIBRARY_FIND_IMPL(x11_lib, XBaseFontNameListOfFontSet);
  _LIBRARY_FIND_IMPL(x11_lib, XLocaleOfFontSet);
  _LIBRARY_FIND_IMPL(x11_lib, XContextDependentDrawing);
  _LIBRARY_FIND_IMPL(x11_lib, XDirectionalDependentDrawing);
  _LIBRARY_FIND_IMPL(x11_lib, XContextualDrawing);
  _LIBRARY_FIND_IMPL(x11_lib, XExtentsOfFontSet);
  _LIBRARY_FIND_IMPL(x11_lib, XmbTextEscapement);
  _LIBRARY_FIND_IMPL(x11_lib, XwcTextEscapement);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8TextEscapement);
  _LIBRARY_FIND_IMPL(x11_lib, XmbTextExtents);
  _LIBRARY_FIND_IMPL(x11_lib, XwcTextExtents);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8TextExtents);
  _LIBRARY_FIND_IMPL(x11_lib, XmbTextPerCharExtents);
  _LIBRARY_FIND_IMPL(x11_lib, XwcTextPerCharExtents);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8TextPerCharExtents);
  _LIBRARY_FIND_IMPL(x11_lib, XmbDrawText);
  _LIBRARY_FIND_IMPL(x11_lib, XwcDrawText);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8DrawText);
  _LIBRARY_FIND_IMPL(x11_lib, XmbDrawString);
  _LIBRARY_FIND_IMPL(x11_lib, XwcDrawString);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8DrawString);
  _LIBRARY_FIND_IMPL(x11_lib, XmbDrawImageString);
  _LIBRARY_FIND_IMPL(x11_lib, XwcDrawImageString);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8DrawImageString);
  _LIBRARY_FIND_IMPL(x11_lib, XOpenIM);
  _LIBRARY_FIND_IMPL(x11_lib, XCloseIM);
  _LIBRARY_FIND_IMPL(x11_lib, XGetIMValues);
  _LIBRARY_FIND_IMPL(x11_lib, XSetIMValues);
  _LIBRARY_FIND_IMPL(x11_lib, XDisplayOfIM);
  _LIBRARY_FIND_IMPL(x11_lib, XLocaleOfIM);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateIC);
  _LIBRARY_FIND_IMPL(x11_lib, XDestroyIC);
  _LIBRARY_FIND_IMPL(x11_lib, XSetICFocus);
  _LIBRARY_FIND_IMPL(x11_lib, XUnsetICFocus);
  _LIBRARY_FIND_IMPL(x11_lib, XwcResetIC);
  _LIBRARY_FIND_IMPL(x11_lib, XmbResetIC);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8ResetIC);
  _LIBRARY_FIND_IMPL(x11_lib, XSetICValues);
  _LIBRARY_FIND_IMPL(x11_lib, XGetICValues);
  _LIBRARY_FIND_IMPL(x11_lib, XIMOfIC);
  _LIBRARY_FIND_IMPL(x11_lib, XFilterEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XmbLookupString);
  _LIBRARY_FIND_IMPL(x11_lib, XwcLookupString);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8LookupString);
  _LIBRARY_FIND_IMPL(x11_lib, XVaCreateNestedList);
  _LIBRARY_FIND_IMPL(x11_lib, XRegisterIMInstantiateCallback);
  _LIBRARY_FIND_IMPL(x11_lib, XUnregisterIMInstantiateCallback);
  _LIBRARY_FIND_IMPL(x11_lib, XInternalConnectionNumbers);
  _LIBRARY_FIND_IMPL(x11_lib, XProcessInternalConnection);
  _LIBRARY_FIND_IMPL(x11_lib, XAddConnectionWatch);
  _LIBRARY_FIND_IMPL(x11_lib, XRemoveConnectionWatch);
  _LIBRARY_FIND_IMPL(x11_lib, XSetAuthorization);
  _LIBRARY_FIND_IMPL(x11_lib, _Xmbtowc);
  _LIBRARY_FIND_IMPL(x11_lib, _Xwctomb);
  _LIBRARY_FIND_IMPL(x11_lib, XGetEventData);
  _LIBRARY_FIND_IMPL(x11_lib, XFreeEventData);
  // Xlib-xcb.h
  _LIBRARY_FIND_IMPL(x11_xcb_lib, XGetXCBConnection);
  _LIBRARY_FIND_IMPL(x11_xcb_lib, XSetEventQueueOwner);
  // Xlibint.h
  _LIBRARY_FIND_IMPL(x11_lib, _XGetRequest);
  _LIBRARY_FIND_IMPL(x11_lib, _XFlushGCCache);
  _LIBRARY_FIND_IMPL(x11_lib, _XData32);
  _LIBRARY_FIND_IMPL(x11_lib, _XRead32);
  _LIBRARY_FIND_IMPL(x11_lib, _XDeqAsyncHandler);
  _LIBRARY_FIND_IMPL(x11_lib, Data);
  _LIBRARY_FIND_IMPL(x11_lib, _XError);
  _LIBRARY_FIND_IMPL(x11_lib, _XIOError);
  _LIBRARY_FIND_IMPL(x11_lib, _XEatData);
  _LIBRARY_FIND_IMPL(x11_lib, _XEatDataWords);
  _LIBRARY_FIND_IMPL(x11_lib, _XAllocScratch);
  _LIBRARY_FIND_IMPL(x11_lib, _XAllocTemp);
  _LIBRARY_FIND_IMPL(x11_lib, _XFreeTemp);
  _LIBRARY_FIND_IMPL(x11_lib, _XVIDtoVisual);
  _LIBRARY_FIND_IMPL(x11_lib, _XSetLastRequestRead);
  _LIBRARY_FIND_IMPL(x11_lib, _XGetHostname);
  _LIBRARY_FIND_IMPL(x11_lib, _XScreenOfWindow);
  _LIBRARY_FIND_IMPL(x11_lib, _XAsyncErrorHandler);
  _LIBRARY_FIND_IMPL(x11_lib, _XGetAsyncReply);
  _LIBRARY_FIND_IMPL(x11_lib, _XGetAsyncData);
  _LIBRARY_FIND_IMPL(x11_lib, _XFlush);
  _LIBRARY_FIND_IMPL(x11_lib, _XEventsQueued);
  _LIBRARY_FIND_IMPL(x11_lib, _XReadEvents);
  _LIBRARY_FIND_IMPL(x11_lib, _XRead);
  _LIBRARY_FIND_IMPL(x11_lib, _XReadPad);
  _LIBRARY_FIND_IMPL(x11_lib, _XSend);
  _LIBRARY_FIND_IMPL(x11_lib, _XReply);
  _LIBRARY_FIND_IMPL(x11_lib, _XEnq);
  _LIBRARY_FIND_IMPL(x11_lib, _XDeq);
  _LIBRARY_FIND_IMPL(x11_lib, _XUnknownWireEvent);
  _LIBRARY_FIND_IMPL(x11_lib, _XUnknownWireEventCookie);
  _LIBRARY_FIND_IMPL(x11_lib, _XUnknownCopyEventCookie);
  _LIBRARY_FIND_IMPL(x11_lib, _XUnknownNativeEvent);
  _LIBRARY_FIND_IMPL(x11_lib, _XWireToEvent);
  _LIBRARY_FIND_IMPL(x11_lib, _XDefaultWireError);
  _LIBRARY_FIND_IMPL(x11_lib, _XPollfdCacheInit);
  _LIBRARY_FIND_IMPL(x11_lib, _XPollfdCacheAdd);
  _LIBRARY_FIND_IMPL(x11_lib, _XPollfdCacheDel);
  _LIBRARY_FIND_IMPL(x11_lib, _XAllocID);
  _LIBRARY_FIND_IMPL(x11_lib, _XAllocIDs);
  _LIBRARY_FIND_IMPL(x11_lib, _XFreeExtData);
  _LIBRARY_FIND_IMPL(x11_lib, XESetCreateGC);
  _LIBRARY_FIND_IMPL(x11_lib, XESetCopyGC);
  _LIBRARY_FIND_IMPL(x11_lib, XESetFlushGC);
  _LIBRARY_FIND_IMPL(x11_lib, XESetFreeGC);
  _LIBRARY_FIND_IMPL(x11_lib, XESetCreateFont);
  _LIBRARY_FIND_IMPL(x11_lib, XESetFreeFont);
  _LIBRARY_FIND_IMPL(x11_lib, XESetCloseDisplay);
  _LIBRARY_FIND_IMPL(x11_lib, XESetError);
  _LIBRARY_FIND_IMPL(x11_lib, XESetErrorString);
  _LIBRARY_FIND_IMPL(x11_lib, XESetPrintErrorValues);
  _LIBRARY_FIND_IMPL(x11_lib, XESetWireToEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XESetWireToEventCookie);
  _LIBRARY_FIND_IMPL(x11_lib, XESetCopyEventCookie);
  _LIBRARY_FIND_IMPL(x11_lib, XESetEventToWire);
  _LIBRARY_FIND_IMPL(x11_lib, XESetWireToError);
  _LIBRARY_FIND_IMPL(x11_lib, XESetBeforeFlush);
  _LIBRARY_FIND_IMPL(x11_lib, _XRegisterInternalConnection);
  _LIBRARY_FIND_IMPL(x11_lib, _XUnregisterInternalConnection);
  _LIBRARY_FIND_IMPL(x11_lib, _XProcessInternalConnection);
  _LIBRARY_FIND_IMPL(x11_lib, __XOS2RedirRoot);
  _LIBRARY_FIND_IMPL(x11_lib, _XTextHeight);
  _LIBRARY_FIND_IMPL(x11_lib, _XTextHeight16);
  _LIBRARY_FIND_IMPL(x11_lib, _XOpenFile);
  _LIBRARY_FIND_IMPL(x11_lib, _XOpenFileMode);
  _LIBRARY_FIND_IMPL(x11_lib, _XFopenFile);
  _LIBRARY_FIND_IMPL(x11_lib, _XAccessFile);
  _LIBRARY_FIND_IMPL(x11_lib, _XEventToWire);
  _LIBRARY_FIND_IMPL(x11_lib, _XF86LoadQueryLocaleFont);
  _LIBRARY_FIND_IMPL(x11_lib, _XProcessWindowAttributes);
  _LIBRARY_FIND_IMPL(x11_lib, _XDefaultError);
  _LIBRARY_FIND_IMPL(x11_lib, _XDefaultIOError);
  _LIBRARY_FIND_IMPL(x11_lib, _XSetClipRectangles);
  _LIBRARY_FIND_IMPL(x11_lib, _XGetWindowAttributes);
  _LIBRARY_FIND_IMPL(x11_lib, _XPutBackEvent);
  _LIBRARY_FIND_IMPL(x11_lib, _XIsEventCookie);
  _LIBRARY_FIND_IMPL(x11_lib, _XFreeEventCookies);
  _LIBRARY_FIND_IMPL(x11_lib, _XStoreEventCookie);
  _LIBRARY_FIND_IMPL(x11_lib, _XFetchEventCookie);
  _LIBRARY_FIND_IMPL(x11_lib, _XCopyEventCookie);
  _LIBRARY_FIND_IMPL(x11_lib, xlocaledir);
  // XKBlib.h
  _LIBRARY_FIND_IMPL(x11_lib, XkbIgnoreExtension);
  _LIBRARY_FIND_IMPL(x11_lib, XkbOpenDisplay);
  _LIBRARY_FIND_IMPL(x11_lib, XkbQueryExtension);
  _LIBRARY_FIND_IMPL(x11_lib, XkbUseExtension);
  _LIBRARY_FIND_IMPL(x11_lib, XkbLibraryVersion);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetXlibControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetXlibControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbXlibControlsImplemented);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetAtomFuncs);
  _LIBRARY_FIND_IMPL(x11_lib, XkbKeycodeToKeysym);
  _LIBRARY_FIND_IMPL(x11_lib, XkbKeysymToModifiers);
  _LIBRARY_FIND_IMPL(x11_lib, XkbLookupKeySym);
  _LIBRARY_FIND_IMPL(x11_lib, XkbLookupKeyBinding);
  _LIBRARY_FIND_IMPL(x11_lib, XkbTranslateKeyCode);
  _LIBRARY_FIND_IMPL(x11_lib, XkbTranslateKeySym);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetAutoRepeatRate);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetAutoRepeatRate);
  _LIBRARY_FIND_IMPL(x11_lib, XkbChangeEnabledControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbDeviceBell);
  _LIBRARY_FIND_IMPL(x11_lib, XkbForceDeviceBell);
  _LIBRARY_FIND_IMPL(x11_lib, XkbDeviceBellEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XkbBell);
  _LIBRARY_FIND_IMPL(x11_lib, XkbForceBell);
  _LIBRARY_FIND_IMPL(x11_lib, XkbBellEvent);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSelectEvents);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSelectEventDetails);
  _LIBRARY_FIND_IMPL(x11_lib, XkbNoteMapChanges);
  _LIBRARY_FIND_IMPL(x11_lib, XkbNoteNameChanges);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetIndicatorState);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetDeviceIndicatorState);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetIndicatorMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetIndicatorMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetNamedIndicator);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetNamedDeviceIndicator);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetNamedIndicator);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetNamedDeviceIndicator);
  _LIBRARY_FIND_IMPL(x11_lib, XkbLockModifiers);
  _LIBRARY_FIND_IMPL(x11_lib, XkbLatchModifiers);
  _LIBRARY_FIND_IMPL(x11_lib, XkbLockGroup);
  _LIBRARY_FIND_IMPL(x11_lib, XkbLatchGroup);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetServerInternalMods);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetIgnoreLockMods);
  _LIBRARY_FIND_IMPL(x11_lib, XkbVirtualModsToReal);
  _LIBRARY_FIND_IMPL(x11_lib, XkbComputeEffectiveMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbInitCanonicalKeyTypes);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAllocKeyboard);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeKeyboard);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAllocClientMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAllocServerMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeClientMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeServerMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAddKeyType);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAllocIndicatorMaps);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeIndicatorMaps);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetUpdatedMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetMapChanges);
  _LIBRARY_FIND_IMPL(x11_lib, XkbRefreshKeyboardMapping);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeyTypes);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeySyms);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeyActions);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeyBehaviors);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetVirtualMods);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeyExplicitComponents);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeyModifierMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeyVirtualModMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAllocControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbNoteControlsChanges);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAllocCompatMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeCompatMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetCompatMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetCompatMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAddSymInterpret);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAllocNames);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetNames);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetNames);
  _LIBRARY_FIND_IMPL(x11_lib, XkbChangeNames);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeNames);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetState);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbChangeMap);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetDetectableAutoRepeat);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetDetectableAutoRepeat);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetAutoResetControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetAutoResetControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetPerClientControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetPerClientControls);
  _LIBRARY_FIND_IMPL(x11_lib, XkbCopyKeyType);
  _LIBRARY_FIND_IMPL(x11_lib, XkbCopyKeyTypes);
  _LIBRARY_FIND_IMPL(x11_lib, XkbResizeKeyType);
  _LIBRARY_FIND_IMPL(x11_lib, XkbResizeKeySyms);
  _LIBRARY_FIND_IMPL(x11_lib, XkbResizeKeyActions);
  _LIBRARY_FIND_IMPL(x11_lib, XkbChangeTypesOfKey);
  _LIBRARY_FIND_IMPL(x11_lib, XkbChangeKeycodeRange);
  _LIBRARY_FIND_IMPL(x11_lib, XkbListComponents);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeComponentList);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeyboard);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetKeyboardByName);
  _LIBRARY_FIND_IMPL(x11_lib, XkbKeyTypesForCoreSymbols);
  _LIBRARY_FIND_IMPL(x11_lib, XkbApplyCompatMapToKey);
  _LIBRARY_FIND_IMPL(x11_lib, XkbUpdateMapFromCore);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAddDeviceLedInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XkbResizeDeviceButtonActions);
  _LIBRARY_FIND_IMPL(x11_lib, XkbAllocDeviceInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XkbFreeDeviceInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XkbNoteDeviceChanges);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetDeviceInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetDeviceInfoChanges);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetDeviceButtonActions);
  _LIBRARY_FIND_IMPL(x11_lib, XkbGetDeviceLedInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetDeviceInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XkbChangeDeviceInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetDeviceLedInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetDeviceButtonActions);
  _LIBRARY_FIND_IMPL(x11_lib, XkbToControl);
  _LIBRARY_FIND_IMPL(x11_lib, XkbSetDebuggingFlags);
  _LIBRARY_FIND_IMPL(x11_lib, XkbApplyVirtualModChanges);
  _LIBRARY_FIND_IMPL(x11_lib, XkbUpdateActionVirtualMods);
  _LIBRARY_FIND_IMPL(x11_lib, XkbUpdateKeyTypeVirtualMods);
  // Xutil.h
  _LIBRARY_FIND_IMPL(x11_lib, XDestroyImage);
  _LIBRARY_FIND_IMPL(x11_lib, XGetPixel);
  _LIBRARY_FIND_IMPL(x11_lib, XPutPixel);
  _LIBRARY_FIND_IMPL(x11_lib, XSubImage);
  _LIBRARY_FIND_IMPL(x11_lib, XAddPixel);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocClassHint);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocIconSize);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocSizeHints);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocStandardColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XAllocWMHints);
  _LIBRARY_FIND_IMPL(x11_lib, XClipBox);
  _LIBRARY_FIND_IMPL(x11_lib, XCreateRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XDefaultString);
  _LIBRARY_FIND_IMPL(x11_lib, XDeleteContext);
  _LIBRARY_FIND_IMPL(x11_lib, XDestroyRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XEmptyRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XEqualRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XFindContext);
  _LIBRARY_FIND_IMPL(x11_lib, XGetClassHint);
  _LIBRARY_FIND_IMPL(x11_lib, XGetIconSizes);
  _LIBRARY_FIND_IMPL(x11_lib, XGetNormalHints);
  _LIBRARY_FIND_IMPL(x11_lib, XGetRGBColormaps);
  _LIBRARY_FIND_IMPL(x11_lib, XGetSizeHints);
  _LIBRARY_FIND_IMPL(x11_lib, XGetStandardColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XGetTextProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XGetVisualInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWMClientMachine);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWMHints);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWMIconName);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWMName);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWMNormalHints);
  _LIBRARY_FIND_IMPL(x11_lib, XGetWMSizeHints);
  _LIBRARY_FIND_IMPL(x11_lib, XGetZoomHints);
  _LIBRARY_FIND_IMPL(x11_lib, XIntersectRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XConvertCase);
  _LIBRARY_FIND_IMPL(x11_lib, XLookupString);
  _LIBRARY_FIND_IMPL(x11_lib, XMatchVisualInfo);
  _LIBRARY_FIND_IMPL(x11_lib, XOffsetRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XPointInRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XPolygonRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XRectInRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XSaveContext);
  _LIBRARY_FIND_IMPL(x11_lib, XSetClassHint);
  _LIBRARY_FIND_IMPL(x11_lib, XSetIconSizes);
  _LIBRARY_FIND_IMPL(x11_lib, XSetNormalHints);
  _LIBRARY_FIND_IMPL(x11_lib, XSetRGBColormaps);
  _LIBRARY_FIND_IMPL(x11_lib, XSetSizeHints);
  _LIBRARY_FIND_IMPL(x11_lib, XSetStandardProperties);
  _LIBRARY_FIND_IMPL(x11_lib, XSetTextProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMClientMachine);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMHints);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMIconName);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMName);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMNormalHints);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMProperties);
  _LIBRARY_FIND_IMPL(x11_lib, XmbSetWMProperties);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8SetWMProperties);
  _LIBRARY_FIND_IMPL(x11_lib, XSetWMSizeHints);
  _LIBRARY_FIND_IMPL(x11_lib, XSetRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XSetStandardColormap);
  _LIBRARY_FIND_IMPL(x11_lib, XSetZoomHints);
  _LIBRARY_FIND_IMPL(x11_lib, XShrinkRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XStringListToTextProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XSubtractRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XmbTextListToTextProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XwcTextListToTextProperty);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8TextListToTextProperty);
  _LIBRARY_FIND_IMPL(x11_lib, XwcFreeStringList);
  _LIBRARY_FIND_IMPL(x11_lib, XTextPropertyToStringList);
  _LIBRARY_FIND_IMPL(x11_lib, XmbTextPropertyToTextList);
  _LIBRARY_FIND_IMPL(x11_lib, XwcTextPropertyToTextList);
  _LIBRARY_FIND_IMPL(x11_lib, Xutf8TextPropertyToTextList);
  _LIBRARY_FIND_IMPL(x11_lib, XUnionRectWithRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XUnionRegion);
  _LIBRARY_FIND_IMPL(x11_lib, XWMGeometry);
  _LIBRARY_FIND_IMPL(x11_lib, XXorRegion);
  // Xcursor.h
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImageCreate);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImageDestroy);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImagesCreate);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImagesDestroy);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImagesSetName);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorCursorsCreate);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorCursorsDestroy);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorAnimateCreate);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorAnimateDestroy);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorAnimateNext);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorCommentCreate);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorCommentDestroy);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorCommentsCreate);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorCommentsDestroy);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorXcFileLoadImage);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorXcFileLoadImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorXcFileLoadAllImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorXcFileLoad);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorXcFileSave);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFileLoadImage);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFileLoadImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFileLoadAllImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFileLoad);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFileSaveImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFileSave);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFilenameLoadImage);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFilenameLoadImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFilenameLoadAllImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFilenameLoad);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFilenameSaveImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFilenameSave);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorLibraryLoadImage);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorLibraryLoadImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorLibraryPath);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorLibraryShape);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImageLoadCursor);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImagesLoadCursors);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImagesLoadCursor);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFilenameLoadCursor);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorFilenameLoadCursors);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorLibraryLoadCursor);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorLibraryLoadCursors);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorShapeLoadImage);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorShapeLoadImages);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorShapeLoadCursor);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorShapeLoadCursors);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorTryShapeCursor);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorNoticeCreateBitmap);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorNoticePutBitmap);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorTryShapeBitmapCursor);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorImageHash);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorSupportsARGB);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorSupportsAnim);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorSetDefaultSize);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorGetDefaultSize);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorSetTheme);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorGetTheme);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorGetThemeCore);
  _LIBRARY_FIND_IMPL(x11_cursor_lib, XcursorSetThemeCore);
  // Xinerama.h
  _LIBRARY_FIND_IMPL(x11_xinerama_lib, XineramaQueryExtension);
  _LIBRARY_FIND_IMPL(x11_xinerama_lib, XineramaQueryVersion);
  _LIBRARY_FIND_IMPL(x11_xinerama_lib, XineramaIsActive);
  _LIBRARY_FIND_IMPL(x11_xinerama_lib, XineramaQueryScreens);
}

XewErrorCode xewX11Init(void) {
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
