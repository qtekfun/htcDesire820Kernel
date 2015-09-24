// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_PERMISSIONS_API_PERMISSION_H_
#define EXTENSIONS_COMMON_PERMISSIONS_API_PERMISSION_H_

#include <map>
#include <set>
#include <string>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/pickle.h"
#include "base/values.h"
#include "extensions/common/permissions/permission_message.h"

namespace IPC {
class Message;
}

namespace extensions {

class APIPermissionInfo;
class ChromeAPIPermissions;

class APIPermission {
 public:
  enum ID {
    
    kInvalid = -2,
    kUnknown = -1,

    
    kActiveTab,
    kActivityLogPrivate,
    kAdView,
    kAlarms,
    kAlwaysOnTopWindows,
    kAppCurrentWindowInternal,
    kAppRuntime,
    kAppWindow,
    kAudio,
    kAudioCapture,
    kAutoTestPrivate,
    kBackground,
    kBluetooth,
    kBookmark,
    kBookmarkManagerPrivate,
    kBrailleDisplayPrivate,
    kBrowsingData,
    kCast,
    kCastStreaming,
    kChromeosInfoPrivate,
    kClipboardRead,
    kClipboardWrite,
    kCloudPrintPrivate,
    kCommandLinePrivate,
    kContentSettings,
    kContextMenus,
    kCookie,
    kDiagnostics,
    kDial,
    kDebugger,
    kDeclarative,
    kDeclarativeContent,
    kDeclarativeWebRequest,
    kDesktopCapture,
    kDeveloperPrivate,
    kDevtools,
    kDns,
    kDownloads,
    kDownloadsInternal,
    kDownloadsOpen,
    kDownloadsShelf,
    kEchoPrivate,
    kEnterprisePlatformKeysPrivate,
    kExperimental,
    kFeedbackPrivate,
    kFileBrowserHandler,
    kFileBrowserHandlerInternal,
    kFileBrowserPrivate,
    kFileSystem,
    kFileSystemDirectory,
    kFileSystemProvider,
    kFileSystemRetainEntries,
    kFileSystemWrite,
    kFileSystemWriteDirectory,
    kFontSettings,
    kFullscreen,
    kGcm,
    kGeolocation,
    kHistory,
    kHomepage,
    kIdentity,
    kIdentityPrivate,
    kIdltest,
    kIdle,
    kInfobars,
    kInput,
    kInputMethodPrivate,
    kLocation,
    kLogPrivate,
    kManagement,
    kMediaGalleries,
    kMediaGalleriesPrivate,
    kMediaPlayerPrivate,
    kMetricsPrivate,
    kMDns,
    kMusicManagerPrivate,
    kNativeMessaging,
    kNetworkingPrivate,
    kNotification,
    kOverrideEscFullscreen,
    kPageCapture,
    kPointerLock,
    kPlugin,
    kPower,
    kPreferencesPrivate,
    kPrincipalsPrivate,
    kPrivacy,
    kProcesses,
    kProxy,
    kPushMessaging,
    kImageWriterPrivate,
    kRtcPrivate,
    kSearchProvider,
    kSerial,
    kSessions,
    kSignedInDevices,
    kSocket,
    kStartupPages,
    kStorage,
    kStreamsPrivate,
    kSyncFileSystem,
    kSystemPrivate,
    kSystemIndicator,
    kSystemDisplay,
    kSystemStorage,
    kTab,
    kTabCapture,
    kTabCaptureForTab,
    kTerminalPrivate,
    kTopSites,
    kTts,
    kTtsEngine,
    kUnlimitedStorage,
    kUsb,
    kUsbDevice,
    kVideoCapture,
    kVirtualKeyboardPrivate,
    kWallpaper,
    kWallpaperPrivate,
    kWebConnectable,  
    kWebNavigation,
    kWebRequest,
    kWebRequestBlocking,
    kWebRequestInternal,
    kWebrtcAudioPrivate,
    kWebrtcLoggingPrivate,
    kWebstorePrivate,
    kWebView,
    kScreenlockPrivate,
    kSystemCpu,
    kSystemMemory,
    kSystemNetwork,
    kSystemInfoCpu,
    kSystemInfoMemory,
    kFirstRunPrivate,
    kEnumBoundary
  };

  struct CheckParam {
  };

  explicit APIPermission(const APIPermissionInfo* info);

  virtual ~APIPermission();

  
  ID id() const;

  
  const char* name() const;

  
  const APIPermissionInfo* info() const {
    return info_;
  }

  
  virtual bool HasMessages() const = 0;

  
  virtual PermissionMessages GetMessages() const = 0;

  
  virtual bool Check(const CheckParam* param) const = 0;

  
  virtual bool Contains(const APIPermission* rhs) const = 0;

  
  virtual bool Equal(const APIPermission* rhs) const = 0;

  
  virtual bool FromValue(const base::Value* value) = 0;

  
  virtual scoped_ptr<base::Value> ToValue() const = 0;

  
  virtual APIPermission* Clone() const = 0;

  
  virtual APIPermission* Diff(const APIPermission* rhs) const = 0;

  
  virtual APIPermission* Union(const APIPermission* rhs) const = 0;

  
  virtual APIPermission* Intersect(const APIPermission* rhs) const = 0;

  
  
  virtual void Write(IPC::Message* m) const = 0;

  
  virtual bool Read(const IPC::Message* m, PickleIterator* iter) = 0;

  
  virtual void Log(std::string* log) const = 0;

 protected:
  
  
  PermissionMessage GetMessage_() const;

 private:
  const APIPermissionInfo* const info_;
};


class APIPermissionInfo {
 public:
  enum Flag {
    kFlagNone = 0,

    
    kFlagImpliesFullAccess = 1 << 0,

    
    kFlagImpliesFullURLAccess = 1 << 1,

    
    kFlagCannotBeOptional = 1 << 3,

    
    
    kFlagInternal = 1 << 4,
  };

  typedef APIPermission* (*APIPermissionConstructor)(const APIPermissionInfo*);

  typedef std::set<APIPermission::ID> IDSet;

  ~APIPermissionInfo();

  
  APIPermission* CreateAPIPermission() const;

  int flags() const { return flags_; }

  APIPermission::ID id() const { return id_; }

  
  PermissionMessage::ID message_id() const {
    return message_id_;
  }

  
  const char* name() const { return name_; }

  
  bool implies_full_access() const {
    return (flags_ & kFlagImpliesFullAccess) != 0;
  }

  
  bool implies_full_url_access() const {
    return (flags_ & kFlagImpliesFullURLAccess) != 0;
  }

  
  
  bool supports_optional() const {
    return (flags_ & kFlagCannotBeOptional) == 0;
  }

  
  
  bool is_internal() const {
    return (flags_ & kFlagInternal) != 0;
  }

 private:
  
  
  friend class ChromeAPIPermissions;
  
  
  friend class APIPermission;

  explicit APIPermissionInfo(
      APIPermission::ID id,
      const char* name,
      int l10n_message_id,
      PermissionMessage::ID message_id,
      int flags,
      APIPermissionConstructor api_permission_constructor);

  
  
  PermissionMessage GetMessage_() const;

  const APIPermission::ID id_;
  const char* const name_;
  const int flags_;
  const int l10n_message_id_;
  const PermissionMessage::ID message_id_;
  const APIPermissionConstructor api_permission_constructor_;
};

}  

#endif  
