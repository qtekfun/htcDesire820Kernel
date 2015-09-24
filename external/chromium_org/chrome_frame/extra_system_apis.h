// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_EXTRA_SYSTEM_APIS_H_
#define CHROME_FRAME_EXTRA_SYSTEM_APIS_H_

#include <mshtml.h>
#include <shdeprecated.h>

class __declspec(uuid("54A8F188-9EBD-4795-AD16-9B4945119636"))
IWebBrowserEventsService : public IUnknown {
 public:
  STDMETHOD(FireBeforeNavigate2Event)(VARIANT_BOOL* cancel) = 0;
  STDMETHOD(FireNavigateComplete2Event)(VOID) = 0;
  STDMETHOD(FireDownloadBeginEvent)(VOID) = 0;
  STDMETHOD(FireDownloadCompleteEvent)(VOID) = 0;
  STDMETHOD(FireDocumentCompleteEvent)(VOID) = 0;
};

class __declspec(uuid("{87CC5D04-EAFA-4833-9820-8F986530CC00}"))
IWebBrowserEventsUrlService : public IUnknown {
 public:
  STDMETHOD(GetUrlForEvents)(BSTR* url) = 0;
};

class __declspec(uuid("{3050F804-98B5-11CF-BB82-00AA00BDCE0B}"))
IWebBrowserPriv : public IUnknown {
 public:
  STDMETHOD(NavigateWithBindCtx)(VARIANT* uri, VARIANT* flags,
                                 VARIANT* target_frame, VARIANT* post_data,
                                 VARIANT* headers, IBindCtx* bind_ctx,
                                 LPOLESTR url_fragment);
  STDMETHOD(OnClose)();
};

class IWebBrowserPriv2Common : public IUnknown {
 public:
  STDMETHOD(NavigateWithBindCtx2)(IUri* uri, VARIANT* flags,
                                  VARIANT* target_frame, VARIANT* post_data,
                                  VARIANT* headers, IBindCtx* bind_ctx,
                                  LPOLESTR url_fragment);
};

class IWebBrowserPriv2CommonIE9 : public IUnknown {
 public:
  STDMETHOD(NavigateWithBindCtx2)(IUri* uri, VARIANT* flags,
                                  VARIANT* target_frame, VARIANT* post_data,
                                  VARIANT* headers, IBindCtx* bind_ctx,
                                  LPOLESTR url_fragment, DWORD unused1);
};

interface __declspec(uuid("3050f801-98b5-11cf-bb82-00aa00bdce0b"))
IDocObjectService : public IUnknown {
  STDMETHOD(FireBeforeNavigate2)(IDispatch* dispatch,
      LPCTSTR url, DWORD flags, LPCTSTR frame_name, BYTE* post_data,
      DWORD post_data_len, LPCTSTR headers, BOOL play_nav_sound,
      BOOL* cancel) = 0;
  STDMETHOD(FireNavigateComplete2)(IHTMLWindow2* html_window2,
      DWORD flags) = 0;
  STDMETHOD(FireDownloadBegin)() = 0;
  STDMETHOD(FireDownloadComplete)() = 0;
  STDMETHOD(FireDocumentComplete)(IHTMLWindow2* html_window2, DWORD flags) = 0;
  STDMETHOD(UpdateDesktopComponent)(IHTMLWindow2* html_window2) = 0;
  STDMETHOD(GetPendingUrl)(BSTR* pending_url) = 0;
  STDMETHOD(ActiveElementChanged)(IHTMLElement* html_element) = 0;
  STDMETHOD(GetUrlSearchComponent)(BSTR* search) = 0;
  STDMETHOD(IsErrorUrl)(LPCTSTR url, BOOL* is_error) = 0;
};

interface __declspec(uuid("f62d9369-75ef-4578-8856-232802c76468"))
ITridentService2 : public IUnknown {
  STDMETHOD(FireBeforeNavigate2)(IDispatch* dispatch,
      LPCTSTR url, DWORD flags, LPCTSTR frame_name, BYTE* post_data,
      DWORD post_data_len, LPCTSTR headers, BOOL play_nav_sound,
      BOOL* cancel) = 0;
  STDMETHOD(FireNavigateComplete2)(IHTMLWindow2*, uint32);
  STDMETHOD(FireDownloadBegin)(VOID);
  STDMETHOD(FireDownloadComplete)(VOID);
  STDMETHOD(FireDocumentComplete)(IHTMLWindow2*, uint32);
  STDMETHOD(UpdateDesktopComponent)(IHTMLWindow2*);
  STDMETHOD(GetPendingUrl)(uint16**);
  STDMETHOD(ActiveElementChanged)(IHTMLElement*);
  STDMETHOD(GetUrlSearchComponent)(uint16**);
  STDMETHOD(IsErrorUrl)(uint16 const*, int32*);
  STDMETHOD(AttachMyPics)(VOID *, VOID**);
  STDMETHOD(ReleaseMyPics)(VOID*);
  STDMETHOD(IsGalleryMeta)(int32, VOID*);
  STDMETHOD(EmailPicture)(uint16*);
  STDMETHOD(FireNavigateError)(IHTMLWindow2*,
                               uint16*,
                               uint16*,
                               uint32, int*);
  STDMETHOD(FirePrintTemplateEvent)(IHTMLWindow2*, int32);
  STDMETHOD(FireUpdatePageStatus)(IHTMLWindow2*, uint32, int32);
  STDMETHOD(FirePrivacyImpactedStateChange)(int32 privacy_violated);
  STDMETHOD(InitAutoImageResize)(VOID);
  STDMETHOD(UnInitAutoImageResize)(VOID);
};

#define TLEF_RELATIVE_INCLUDE_CURRENT (0x01)  
#define TLEF_RELATIVE_BACK (0x10)             
#define TLEF_RELATIVE_FORE (0x20)             

#endif  
