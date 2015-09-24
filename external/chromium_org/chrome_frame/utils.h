// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_UTILS_H_
#define CHROME_FRAME_UTILS_H_

#include <OAidl.h>
#include <objidl.h>
#include <windows.h>
#include <wininet.h>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/logging.h"
#include "base/metrics/histogram.h"
#include "base/strings/string16.h"
#include "base/win/scoped_comptr.h"
#include "ui/gfx/rect.h"
#include "url/gurl.h"

class RegistryListPreferencesHolder;
interface IBrowserService;
interface IWebBrowser2;
struct ContextMenuModel;

namespace base {
class FilePath;
}

extern const char kGCFProtocol[];

extern const wchar_t kAllowUnsafeURLs[];
extern const wchar_t kChromeContentPrefix[];
extern const wchar_t kChromeFrameAccessibleMode[];
extern const wchar_t kChromeFrameAttachTabPattern[];
extern const wchar_t kChromeFrameConfigKey[];
extern const wchar_t kChromeFrameHeadlessMode[];
extern const wchar_t kChromeFrameUnpinnedMode[];
extern const wchar_t kChromeMimeType[];
extern const wchar_t kChromeProtocolPrefix[];
extern const wchar_t kEnableBuggyBhoIntercept[];
extern const wchar_t kEnableGCFRendererByDefault[];
extern const wchar_t kExcludeUAFromDomainList[];
extern const wchar_t kIexploreProfileName[];
extern const wchar_t kRenderInGCFUrlList[];
extern const wchar_t kRenderInHostUrlList[];
extern const wchar_t kRundllProfileName[];
extern const wchar_t kUseBackgroundThreadForSubResources[];

HRESULT UtilRegisterTypeLib(HINSTANCE tlb_instance,
                            LPCOLESTR index,
                            bool for_current_user_only);

HRESULT UtilUnRegisterTypeLib(HINSTANCE tlb_instance,
                              LPCOLESTR index,
                              bool for_current_user_only);

HRESULT UtilRegisterTypeLib(LPCWSTR typelib_path, bool for_current_user_only);

HRESULT UtilUnRegisterTypeLib(LPCWSTR typelib_path, bool for_current_user_only);

HRESULT UtilRegisterTypeLib(ITypeLib* typelib,
                            LPCWSTR typelib_path,
                            LPCWSTR help_dir,
                            bool for_current_user_only);

HRESULT UtilUnRegisterTypeLib(ITypeLib* typelib,
                              bool for_current_user_only);

bool UtilRemovePersistentNPAPIMarker();

HRESULT UtilGetXUACompatContentValue(const std::wstring& html_string,
                                     std::wstring* content_value);

std::wstring GetResourceString(int resource_id);

void DisplayVersionMismatchWarning(HWND parent,
                                   const std::string& server_version);

template < class BaseAtlModule >
class AtlPerUserModule : public BaseAtlModule {
 public:
  HRESULT RegisterServer(BOOL reg_typelib = FALSE,
                         const CLSID* clsid = NULL) throw() {
    HRESULT hr = BaseAtlModule::RegisterServer(FALSE, clsid);
    if (FAILED(hr)) {
      return hr;
    }
    if (reg_typelib)  {
      hr = UtilRegisterTypeLib(_AtlComModule.m_hInstTypeLib, NULL, false);
    }
    return hr;
  }

  HRESULT UnregisterServer(BOOL unreg_typelib,
                           const CLSID* clsid = NULL) throw() {
    HRESULT hr = BaseAtlModule::UnregisterServer(FALSE, clsid);
    if (FAILED(hr)) {
      return hr;
    }
    if (unreg_typelib)  {
      hr = UtilUnRegisterTypeLib(_AtlComModule.m_hInstTypeLib, NULL, false);
    }
    return hr;
  }
};

std::string CreateJavascript(const std::string& function_name,
                             const std::string args);

class AddRefModule {
 public:
  AddRefModule();
  ~AddRefModule();
};

std::wstring GetHostProcessName(bool include_extension);

typedef enum BrowserType {
  BROWSER_INVALID = -1,
  BROWSER_UNKNOWN,
  BROWSER_IE,
};

BrowserType GetBrowserType();

typedef enum IEVersion {
  IE_INVALID,
  NON_IE,
  IE_UNSUPPORTED,
  IE_6,
  IE_7,
  IE_8,
  IE_9,
  IE_10,
  IE_11,
};

enum RendererType {
  RENDERER_TYPE_UNDETERMINED = 0,
  RENDERER_TYPE_CHROME_MIN,
  
  
  RENDERER_TYPE_CHROME_GCF_PROTOCOL = RENDERER_TYPE_CHROME_MIN,
  RENDERER_TYPE_CHROME_HTTP_EQUIV,
  RENDERER_TYPE_CHROME_RESPONSE_HEADER,
  RENDERER_TYPE_CHROME_DEFAULT_RENDERER,
  RENDERER_TYPE_CHROME_OPT_IN_URL,
  RENDERER_TYPE_CHROME_WIDGET,
  
  RENDERER_TYPE_CHROME_MAX = RENDERER_TYPE_CHROME_WIDGET,
  RENDERER_TYPE_OTHER,
};

bool IsChrome(RendererType renderer_type);

#define UMA_LAUNCH_TYPE_COUNT(sample) \
  UMA_HISTOGRAM_CUSTOM_COUNTS("ChromeFrame.LaunchType", sample, \
  RENDERER_TYPE_CHROME_MIN, RENDERER_TYPE_CHROME_MAX, \
  RENDERER_TYPE_CHROME_MAX + 1 - RENDERER_TYPE_CHROME_MIN)

IEVersion GetIEVersion();

uint32 GetIEMajorVersion();

base::FilePath GetIETemporaryFilesFolder();

bool GetModuleVersion(HMODULE module, uint32* high, uint32* low);

bool IsIEInPrivate();

HRESULT DoFileDownloadInIE(const wchar_t* url);

HMENU BuildContextMenu(const ContextMenuModel& menu_model);

std::string ResolveURL(const std::string& document,
                       const std::string& relative);

bool HaveSameOrigin(const std::string& url1, const std::string& url2);

bool GetConfigBool(bool default_value, const wchar_t* value_name);

int GetConfigInt(int default_value, const wchar_t* value_name);

int64 GetConfigInt64(int64 default_value, const wchar_t* value_name);

bool SetConfigInt(const wchar_t* value_name, int value);

bool SetConfigBool(const wchar_t* value_name, bool value);

bool SetConfigInt64(const wchar_t* value_name, int64 value);

bool DeleteConfigValue(const wchar_t* value_name);

bool IsHeadlessMode();

bool IsAccessibleMode();

bool IsUnpinnedMode();

bool IsGcfDefaultRenderer();

bool SkipMetadataCheck();

RendererType RendererTypeForUrl(const std::wstring& url);

bool ShouldRemoveUAForUrl(const string16& url);

RegistryListPreferencesHolder& GetRendererTypePreferencesHolderForTesting();
RegistryListPreferencesHolder& GetUserAgentPreferencesHolderForTesting();

template <typename T>
HRESULT DoQueryService(const IID& service_id, IUnknown* unk, T** service) {
  DCHECK(service);
  if (!unk)
    return E_INVALIDARG;

  base::win::ScopedComPtr<IServiceProvider> service_provider;
  HRESULT hr = service_provider.QueryFrom(unk);
  if (service_provider)
    hr = service_provider->QueryService(service_id, service);

  DCHECK(FAILED(hr) || *service);
  return hr;
}

HRESULT NavigateBrowserToMoniker(IUnknown* browser, IMoniker* moniker,
                                 const wchar_t* headers, IBindCtx* bind_ctx,
                                 const wchar_t* fragment, IStream* post_data,
                                 VARIANT* flags);

void MarkBrowserOnThreadForCFNavigation(IBrowserService* browser);

bool CheckForCFNavigation(IBrowserService* browser, bool clear_flag);

bool IsValidUrlScheme(const GURL& url, bool is_privileged);

std::string GetRawHttpHeaders(IWinInetHttpInfo* info);

bool IsSubFrameRequest(IUnknown* service_provider);

template <class T>
STDMETHODIMP CheckOutgoingInterface(void* obj, REFIID iid, void** ret,
                                    DWORD cookie) {
  T* instance = reinterpret_cast<T*>(obj);
  HRESULT hr = E_NOINTERFACE;
  IUnknown* delegate = instance ? instance->delegate() : NULL;
  if (delegate) {
    hr = delegate->QueryInterface(iid, ret);
#if !defined(NDEBUG)
    if (SUCCEEDED(hr)) {
      wchar_t iid_string[64] = {0};
      StringFromGUID2(iid, iid_string, arraysize(iid_string));
      DVLOG(1) << __FUNCTION__ << " Giving out wrapped interface: "
               << iid_string;
    }
#endif
  }

  return hr;
}

template <class T>
STDMETHODIMP QueryInterfaceIfDelegateSupports(void* obj, REFIID iid,
                                              void** ret, DWORD cookie) {
  HRESULT hr = E_NOINTERFACE;
  T* instance = reinterpret_cast<T*>(obj);
  IUnknown* delegate = instance ? instance->delegate() : NULL;
  if (delegate) {
    base::win::ScopedComPtr<IUnknown> original;
    hr = delegate->QueryInterface(iid,
                                  reinterpret_cast<void**>(original.Receive()));
    if (original) {
      IUnknown* supported_interface = reinterpret_cast<IUnknown*>(
          reinterpret_cast<DWORD_PTR>(obj) + cookie);
      supported_interface->AddRef();
      *ret = supported_interface;
      hr = S_OK;
    }
  }

  return hr;
}

#define COM_INTERFACE_ENTRY_IF_DELEGATE_SUPPORTS(x) \
    COM_INTERFACE_ENTRY_FUNC(_ATL_IIDOF(x), \
        offsetofclass(x, _ComMapClass), \
        QueryInterfaceIfDelegateSupports<_ComMapClass>)

#define COM_INTERFACE_BLIND_DELEGATE() \
    COM_INTERFACE_ENTRY_FUNC_BLIND(0, CheckOutgoingInterface<_ComMapClass>)

std::wstring GuidToString(const GUID& guid);

std::wstring GetActualUrlFromMoniker(IMoniker* moniker,
                                     IBindCtx* bind_context,
                                     const std::wstring& bho_url);

bool IsTopLevelWindow(HWND window);

HRESULT RewindStream(IStream* stream);

#define WM_FIRE_PRIVACY_CHANGE_NOTIFICATION (WM_APP + 1)

#define WM_DOWNLOAD_IN_HOST (WM_APP + 2)

struct DownloadInHostParams {
  base::win::ScopedComPtr<IBindCtx> bind_ctx;
  base::win::ScopedComPtr<IMoniker> moniker;
  base::win::ScopedComPtr<IStream> post_data;
  std::string request_headers;
};

int32 MapCookieStateToCookieAction(InternetCookieState cookie_state);

GURL GetUrlWithoutFragment(const wchar_t* url);

bool CompareUrlsWithoutFragment(const wchar_t* url1, const wchar_t* url2);

std::string FindReferrerFromHeaders(const wchar_t* headers,
                                     const wchar_t* additional_headers);

std::string GetHttpHeadersFromBinding(IBinding* binding);

int GetHttpResponseStatusFromBinding(IBinding* binding);

CLIPFORMAT GetTextHtmlClipboardFormat();

bool IsTextHtmlMimeType(const wchar_t* mime_type);

bool IsTextHtmlClipFormat(CLIPFORMAT cf);

bool IsSystemProcess();

namespace utils {
class DeleteObject {
 public:
  template <typename T>
  void operator()(T* obj) {
    delete obj;
  }
};
}

std::string BindStatus2Str(ULONG bind_status);
std::string PiFlags2Str(DWORD flags);
std::string Bscf2Str(DWORD flags);

HRESULT ReadStream(IStream* stream, size_t size, std::string* data);

class ChromeFrameUrl {
 public:
  ChromeFrameUrl();

  
  bool Parse(const std::wstring& url);

  bool is_chrome_protocol() const {
    return is_chrome_protocol_;
  }

  bool attach_to_external_tab() const {
    return attach_to_external_tab_;
  }

  uint64 cookie() const {
    return cookie_;
  }

  int disposition() const {
    return disposition_;
  }

  const gfx::Rect& dimensions() const {
    return dimensions_;
  }

  const GURL& gurl() const {
    return parsed_url_;
  }

  const std::string& profile_name() const {
    return profile_name_;
  }

 private:
  
  
  bool ParseAttachExternalTabUrl();

  
  void Reset();

  bool attach_to_external_tab_;
  bool is_chrome_protocol_;
  uint64 cookie_;
  gfx::Rect dimensions_;
  int disposition_;

  GURL parsed_url_;
  std::string profile_name_;
};

class NavigationConstraints;
bool CanNavigate(const GURL& url,
                 NavigationConstraints* navigation_constraints);

void WaitWithMessageLoop(HANDLE* handles, int count, DWORD timeout);

void EnumerateKeyValues(HKEY parent_key, const wchar_t* sub_key_name,
                        std::vector<std::wstring>* values);

bool CheckXUaCompatibleDirective(const std::string& directive,
                                 int ie_major_version);

std::wstring GetCurrentModuleVersion();

bool IsChromeFrameDocument(IWebBrowser2* web_browser);

bool IncreaseWinInetConnections(DWORD connections);

void GetChromeFrameProfilePath(const string16& profile_name,
                               base::FilePath* profile_path);

#endif  
