// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_FILE_SYSTEM_BROWSER_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_PEPPER_PEPPER_FILE_SYSTEM_BROWSER_HOST_H_

#include <queue>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/memory/weak_ptr.h"
#include "content/browser/renderer_host/pepper/quota_reservation.h"
#include "content/common/content_export.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/private/ppb_isolated_file_system_private.h"
#include "ppapi/host/host_message_context.h"
#include "ppapi/host/resource_host.h"
#include "url/gurl.h"
#include "webkit/browser/fileapi/file_system_context.h"

namespace content {

class BrowserPpapiHost;
class PepperFileIOHost;

class CONTENT_EXPORT PepperFileSystemBrowserHost
    : public ppapi::host::ResourceHost,
      public base::SupportsWeakPtr<PepperFileSystemBrowserHost> {
 public:
  
  
  PepperFileSystemBrowserHost(BrowserPpapiHost* host,
                              PP_Instance instance,
                              PP_Resource resource,
                              PP_FileSystemType type);
  virtual ~PepperFileSystemBrowserHost();

  
  
  
  
  void OpenExisting(const GURL& root_url, const base::Closure& callback);

  
  virtual int32_t OnResourceMessageReceived(
      const IPC::Message& msg,
      ppapi::host::HostMessageContext* context) OVERRIDE;
  virtual bool IsFileSystemHost() OVERRIDE;

  
  PP_FileSystemType GetType() const { return type_; }
  bool IsOpened() const { return opened_; }
  GURL GetRootUrl() const { return root_url_; }
  scoped_refptr<fileapi::FileSystemContext> GetFileSystemContext() const {
    return file_system_context_;
  }

  
  
  fileapi::FileSystemOperationRunner* GetFileSystemOperationRunner() const {
    return file_system_operation_runner_.get();
  }
  bool ChecksQuota() const { return quota_reservation_ != NULL; }
  
  
  typedef base::Callback<void(int64_t)> OpenQuotaFileCallback;
  void OpenQuotaFile(PepperFileIOHost* file_io_host,
                     const fileapi::FileSystemURL& url,
                     const OpenQuotaFileCallback& callback);
  
  
  void CloseQuotaFile(PepperFileIOHost* file_io_host);
  
  
  
  
  
  typedef base::Callback<void(int32_t)> RequestQuotaCallback;
  int32_t RequestQuota(int32_t amount,
                       const RequestQuotaCallback& callback);
 private:
  friend class PepperFileSystemBrowserHostTest;

  struct QuotaRequest {
    QuotaRequest(int32_t amount, const RequestQuotaCallback& callback);
    ~QuotaRequest();

    int32_t amount;
    RequestQuotaCallback callback;
  };

  void OpenExistingFileSystem(
      const base::Closure& callback,
      scoped_refptr<fileapi::FileSystemContext> file_system_context);
  void OpenFileSystem(
      ppapi::host::ReplyMessageContext reply_context,
      fileapi::FileSystemType file_system_type,
      scoped_refptr<fileapi::FileSystemContext> file_system_context);
  void OpenFileSystemComplete(
      ppapi::host::ReplyMessageContext reply_context,
      const GURL& root,
      const std::string& name,
      base::PlatformFileError error);
  void OpenIsolatedFileSystem(
      ppapi::host::ReplyMessageContext reply_context,
      const std::string& fsid,
      PP_IsolatedFileSystemType_Private type,
      scoped_refptr<fileapi::FileSystemContext> file_system_context);
  void OpenPluginPrivateFileSystem(
      ppapi::host::ReplyMessageContext reply_context,
      const std::string& fsid,
      scoped_refptr<fileapi::FileSystemContext> file_system_context);
  void OpenPluginPrivateFileSystemComplete(
      ppapi::host::ReplyMessageContext reply_context,
      const std::string& fsid,
      base::PlatformFileError error);

  int32_t OnHostMsgOpen(ppapi::host::HostMessageContext* context,
                        int64_t expected_size);
  int32_t OnHostMsgInitIsolatedFileSystem(
      ppapi::host::HostMessageContext* context,
      const std::string& fsid,
      PP_IsolatedFileSystemType_Private type);

  void SendReplyForFileSystem(
      ppapi::host::ReplyMessageContext reply_context,
      int32_t pp_error);
  void SendReplyForIsolatedFileSystem(
      ppapi::host::ReplyMessageContext reply_context,
      const std::string& fsid,
      int32_t error);

  void SetFileSystemContext(
      scoped_refptr<fileapi::FileSystemContext> file_system_context);

  bool ShouldCreateQuotaReservation() const;
  void CreateQuotaReservation(const base::Closure& callback);
  void GotQuotaReservation(
      const base::Closure& callback,
      scoped_refptr<QuotaReservation> quota_reservation);

  void ReserveQuota(int32_t amount);
  void GotReservedQuota(int64_t amount,
                        const QuotaReservation::OffsetMap& max_written_offsets);

  std::string GetPluginMimeType() const;

  
  std::string GeneratePluginId(const std::string& mime_type) const;

  BrowserPpapiHost* browser_ppapi_host_;

  PP_FileSystemType type_;
  bool called_open_;  
  bool opened_;  
  GURL root_url_;
  scoped_refptr<fileapi::FileSystemContext> file_system_context_;

  scoped_ptr<fileapi::FileSystemOperationRunner> file_system_operation_runner_;

  
  
  
  
  typedef std::map<int32_t, PepperFileIOHost*> FileMap;
  FileMap files_;
  std::queue<QuotaRequest> pending_quota_requests_;
  int64_t reserved_quota_;
  bool reserving_quota_;
  
  scoped_refptr<QuotaReservation> quota_reservation_;

  std::string fsid_;  

  base::WeakPtrFactory<PepperFileSystemBrowserHost> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(PepperFileSystemBrowserHost);
};

}  

#endif  
