// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_CONTENT_BROWSER_CLIENT_H_
#define CONTENT_PUBLIC_BROWSER_CONTENT_BROWSER_CLIENT_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/callback_forward.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/values.h"
#include "content/public/browser/certificate_request_result_type.h"
#include "content/public/browser/file_descriptor_info.h"
#include "content/public/common/content_client.h"
#include "content/public/common/socket_permission_request.h"
#include "content/public/common/window_container_type.h"
#include "net/base/mime_util.h"
#include "net/cookies/canonical_cookie.h"
#include "net/url_request/url_request_job_factory.h"
#include "third_party/WebKit/public/web/WebNotificationPresenter.h"
#include "ui/base/window_open_disposition.h"
#include "webkit/common/resource_type.h"

#if defined(OS_POSIX) && !defined(OS_MACOSX)
#include "base/posix/global_descriptors.h"
#endif

class CommandLine;
class GURL;
struct WebPreferences;

namespace blink {
struct WebWindowFeatures;
}

namespace base {
class DictionaryValue;
class FilePath;
}

namespace gfx {
class ImageSkia;
}

namespace net {
class CookieOptions;
class HttpNetworkSession;
class NetLog;
class SSLCertRequestInfo;
class SSLInfo;
class URLRequest;
class URLRequestContext;
class URLRequestContextGetter;
class X509Certificate;
}

namespace sandbox {
class TargetPolicy;
}

namespace ui {
class SelectFilePolicy;
}

namespace fileapi {
class ExternalMountPoints;
class FileSystemBackend;
}

namespace content {

class AccessTokenStore;
class BrowserChildProcessHost;
class BrowserContext;
class BrowserMainParts;
class BrowserPluginGuestDelegate;
class BrowserPpapiHost;
class BrowserURLHandler;
class LocationProvider;
class MediaObserver;
class QuotaPermissionContext;
class RenderProcessHost;
class RenderViewHost;
class RenderViewHostDelegateView;
class ResourceContext;
class SiteInstance;
class SpeechRecognitionManagerDelegate;
class VibrationProvider;
class WebContents;
class WebContentsViewDelegate;
class WebContentsViewPort;
struct MainFunctionParams;
struct Referrer;
struct ShowDesktopNotificationHostMsgParams;

typedef std::map<
  std::string, linked_ptr<net::URLRequestJobFactory::ProtocolHandler> >
    ProtocolHandlerMap;

class CONTENT_EXPORT ContentBrowserClient {
 public:
  virtual ~ContentBrowserClient() {}

  
  
  
  virtual BrowserMainParts* CreateBrowserMainParts(
      const MainFunctionParams& parameters);

  
  
  
  
  virtual WebContentsViewPort* OverrideCreateWebContentsView(
      WebContents* web_contents,
      RenderViewHostDelegateView** render_view_host_delegate_view);

  
  
  
  virtual WebContentsViewDelegate* GetWebContentsViewDelegate(
      WebContents* web_contents);

  
  
  
  
  
  
  
  
  
  
  virtual void GuestWebContentsCreated(
      SiteInstance* guest_site_instance,
      WebContents* guest_web_contents,
      WebContents* opener_web_contents,
      BrowserPluginGuestDelegate** guest_delegate,
      scoped_ptr<base::DictionaryValue> extra_params) {}

  
  
  
  
  
  virtual void GuestWebContentsAttached(
      WebContents* guest_web_contents,
      WebContents* embedder_web_contents,
      const base::DictionaryValue& extra_params) {}

  
  
  
  virtual void RenderProcessHostCreated(RenderProcessHost* host) {}

  
  virtual void BrowserChildProcessHostCreated(BrowserChildProcessHost* host) {}

  
  
  virtual GURL GetEffectiveURL(BrowserContext* browser_context,
                               const GURL& url);

  
  
  virtual bool ShouldUseProcessPerSite(BrowserContext* browser_context,
                                       const GURL& effective_url);

  
  
  
  
  virtual void GetAdditionalWebUISchemes(
      std::vector<std::string>* additional_schemes) {}

  
  
  
  virtual net::URLRequestContextGetter* CreateRequestContext(
      BrowserContext* browser_context,
      ProtocolHandlerMap* protocol_handlers);

  
  
  
  virtual net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
      BrowserContext* browser_context,
      const base::FilePath& partition_path,
      bool in_memory,
      ProtocolHandlerMap* protocol_handlers);

  
  
  virtual bool IsHandledURL(const GURL& url);

  
  
  
  
  virtual bool CanCommitURL(RenderProcessHost* process_host, const GURL& url);

  
  
  virtual bool ShouldAllowOpenURL(SiteInstance* site_instance, const GURL& url);

  
  
  virtual bool IsSuitableHost(RenderProcessHost* process_host,
                              const GURL& site_url);

  
  
  
  virtual bool ShouldTryToUseExistingProcessHost(
      BrowserContext* browser_context, const GURL& url);

  
  virtual void SiteInstanceGotProcess(SiteInstance* site_instance) {}

  
  virtual void SiteInstanceDeleting(SiteInstance* site_instance) {}

  
  
  
  
  
  virtual bool ShouldSwapBrowsingInstancesForNavigation(
      SiteInstance* site_instance,
      const GURL& current_url,
      const GURL& new_url);

  
  
  
  virtual bool ShouldSwapProcessesForRedirect(ResourceContext* resource_context,
                                              const GURL& current_url,
                                              const GURL& new_url);

  
  
  virtual bool ShouldAssignSiteForURL(const GURL& url);

  
  virtual std::string GetCanonicalEncodingNameByAliasName(
      const std::string& alias_name);

  
  
  virtual void AppendExtraCommandLineSwitches(CommandLine* command_line,
                                              int child_process_id) {}

  
  
  virtual std::string GetApplicationLocale();

  
  
  virtual std::string GetAcceptLangs(BrowserContext* context);

  
  virtual gfx::ImageSkia* GetDefaultFavicon();

  
  
  virtual bool AllowAppCache(const GURL& manifest_url,
                             const GURL& first_party,
                             ResourceContext* context);

  
  
  virtual bool AllowGetCookie(const GURL& url,
                              const GURL& first_party,
                              const net::CookieList& cookie_list,
                              ResourceContext* context,
                              int render_process_id,
                              int render_view_id);

  
  
  virtual bool AllowSetCookie(const GURL& url,
                              const GURL& first_party,
                              const std::string& cookie_line,
                              ResourceContext* context,
                              int render_process_id,
                              int render_view_id,
                              net::CookieOptions* options);

  
  virtual bool AllowSaveLocalState(ResourceContext* context);

  
  
  
  
  virtual bool AllowWorkerDatabase(
      const GURL& url,
      const base::string16& name,
      const base::string16& display_name,
      unsigned long estimated_size,
      ResourceContext* context,
      const std::vector<std::pair<int, int> >& render_views);

  
  
  
  virtual bool AllowWorkerFileSystem(
      const GURL& url,
      ResourceContext* context,
      const std::vector<std::pair<int, int> >& render_views);

  
  
  
  virtual bool AllowWorkerIndexedDB(
      const GURL& url,
      const base::string16& name,
      ResourceContext* context,
      const std::vector<std::pair<int, int> >& render_views);

  
  
  
  
  virtual net::URLRequestContext* OverrideRequestContextForURL(
      const GURL& url, ResourceContext* context);

  
  
  virtual std::string GetStoragePartitionIdForSite(
      content::BrowserContext* browser_context,
      const GURL& site);

  
  
  
  virtual bool IsValidStoragePartitionId(BrowserContext* browser_context,
                                         const std::string& partition_id);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void GetStoragePartitionConfigForSite(
      content::BrowserContext* browser_context,
      const GURL& site,
      bool can_be_default,
      std::string* partition_domain,
      std::string* partition_name,
      bool* in_memory);

  
  virtual QuotaPermissionContext* CreateQuotaPermissionContext();

  
  
  
  
  
  
  virtual void AllowCertificateError(
      int render_process_id,
      int render_view_id,
      int cert_error,
      const net::SSLInfo& ssl_info,
      const GURL& request_url,
      ResourceType::Type resource_type,
      bool overridable,
      bool strict_enforcement,
      const base::Callback<void(bool)>& callback,
      CertificateRequestResultType* result) {}

  
  
  virtual void SelectClientCertificate(
      int render_process_id,
      int render_view_id,
      const net::HttpNetworkSession* network_session,
      net::SSLCertRequestInfo* cert_request_info,
      const base::Callback<void(net::X509Certificate*)>& callback) {}

  
  
  
  
  virtual void AddCertificate(
      net::URLRequest* request,
      net::CertificateMimeType cert_type,
      const void* cert_data,
      size_t cert_size,
      int render_process_id,
      int render_view_id) {}

  
  
  virtual MediaObserver* GetMediaObserver();

  
  virtual void RequestDesktopNotificationPermission(
      const GURL& source_origin,
      int callback_context,
      int render_process_id,
      int render_view_id) {}

  
  
  virtual blink::WebNotificationPresenter::Permission
      CheckDesktopNotificationPermission(
          const GURL& source_url,
          ResourceContext* context,
          int render_process_id);

  
  
  virtual void ShowDesktopNotification(
      const ShowDesktopNotificationHostMsgParams& params,
      int render_process_id,
      int render_view_id,
      bool worker) {}

  
  virtual void CancelDesktopNotification(
      int render_process_id,
      int render_view_id,
      int notification_id) {}

  
  
  
  
  virtual bool CanCreateWindow(const GURL& opener_url,
                               const GURL& opener_top_level_frame_url,
                               const GURL& source_origin,
                               WindowContainerType container_type,
                               const GURL& target_url,
                               const content::Referrer& referrer,
                               WindowOpenDisposition disposition,
                               const blink::WebWindowFeatures& features,
                               bool user_gesture,
                               bool opener_suppressed,
                               content::ResourceContext* context,
                               int render_process_id,
                               bool is_guest,
                               int opener_id,
                               bool* no_javascript_access);

  
  
  
  virtual std::string GetWorkerProcessTitle(const GURL& url,
                                            ResourceContext* context);

  
  
  virtual void ResourceDispatcherHostCreated() {}

  
  
  virtual SpeechRecognitionManagerDelegate*
      GetSpeechRecognitionManagerDelegate();

  
  virtual net::NetLog* GetNetLog();

  
  virtual AccessTokenStore* CreateAccessTokenStore();

  
  virtual bool IsFastShutdownPossible();

  
  
  
  virtual void OverrideWebkitPrefs(RenderViewHost* render_view_host,
                                   const GURL& url,
                                   WebPreferences* prefs) {}

  
  virtual void UpdateInspectorSetting(RenderViewHost* rvh,
                                      const std::string& key,
                                      const std::string& value) {}

  
  
  virtual void BrowserURLHandlerCreated(BrowserURLHandler* handler) {}

  
  virtual void ClearCache(RenderViewHost* rvh) {}

  
  virtual void ClearCookies(RenderViewHost* rvh) {}

  
  
  virtual base::FilePath GetDefaultDownloadDirectory();

  
  
  virtual std::string GetDefaultDownloadName();

  
  
  
  virtual void DidCreatePpapiPlugin(BrowserPpapiHost* browser_host) {}

  
  virtual content::BrowserPpapiHost* GetExternalBrowserPpapiHost(
      int plugin_child_id);

  
  
  virtual bool SupportsBrowserPlugin(BrowserContext* browser_context,
                                     const GURL& site_url);

  
  
  
  
  
  
  virtual bool AllowPepperSocketAPI(BrowserContext* browser_context,
                                    const GURL& url,
                                    bool private_api,
                                    const SocketPermissionRequest* params);

  
  virtual ui::SelectFilePolicy* CreateSelectFilePolicy(
      WebContents* web_contents);

  
  
  virtual void GetAdditionalAllowedSchemesForFileSystem(
      std::vector<std::string>* additional_schemes) {}

  
  
  
  
  virtual void GetAdditionalFileSystemBackends(
      BrowserContext* browser_context,
      const base::FilePath& storage_partition_path,
      ScopedVector<fileapi::FileSystemBackend>* additional_backends) {}

  
  
  
  
  
  virtual LocationProvider* OverrideSystemLocationProvider();

  
  
  
  
  
  virtual VibrationProvider* OverrideVibrationProvider();

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  
  
  virtual void GetAdditionalMappedFilesForChildProcess(
      const CommandLine& command_line,
      int child_process_id,
      std::vector<FileDescriptorInfo>* mappings) {}
#endif

#if defined(OS_WIN)
  
  virtual const wchar_t* GetResourceDllName();

  
  
  
  virtual void PreSpawnRenderer(sandbox::TargetPolicy* policy,
                                bool* success) {}
#endif

  
  
  virtual bool IsPluginAllowedToCallRequestOSFileHandle(
      content::BrowserContext* browser_context,
      const GURL& url);

  
  virtual bool IsPluginAllowedToUseDevChannelAPIs();
};

}  

#endif  
