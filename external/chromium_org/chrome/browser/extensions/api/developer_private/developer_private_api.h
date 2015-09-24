// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DEVELOPER_PRIVATE_DEVELOPER_PRIVATE_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_DEVELOPER_PRIVATE_DEVELOPER_PRIVATE_API_H_

#include "base/platform_file.h"
#include "chrome/browser/extensions/api/developer_private/entry_picker.h"
#include "chrome/browser/extensions/api/file_system/file_system_api.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/extensions/extension_install_prompt.h"
#include "chrome/browser/extensions/extension_uninstall_dialog.h"
#include "chrome/browser/extensions/pack_extension_job.h"
#include "chrome/browser/extensions/requirements_checker.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/render_view_host.h"
#include "extensions/browser/event_router.h"
#include "ui/shell_dialogs/select_file_dialog.h"
#include "webkit/browser/fileapi/file_system_context.h"
#include "webkit/browser/fileapi/file_system_operation.h"

class ExtensionService;

namespace extensions {

class ExtensionSystem;
class ManagementPolicy;

namespace api {

class EntryPicker;
class EntryPickerClient;

namespace developer_private {

struct ItemInfo;
struct ItemInspectView;
struct ProjectInfo;

}

}  

}  

namespace developer = extensions::api::developer_private;

typedef std::vector<linked_ptr<developer::ItemInfo> > ItemInfoList;
typedef std::vector<linked_ptr<developer::ProjectInfo> > ProjectInfoList;
typedef std::vector<linked_ptr<developer::ItemInspectView> >
    ItemInspectViewList;

namespace extensions {

class DeveloperPrivateEventRouter : public content::NotificationObserver {
 public:
  explicit DeveloperPrivateEventRouter(Profile* profile);
  virtual ~DeveloperPrivateEventRouter();

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  content::NotificationRegistrar registrar_;

  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(DeveloperPrivateEventRouter);
};

class DeveloperPrivateAPI : public BrowserContextKeyedService,
                            public extensions::EventRouter::Observer {
 public:
  
  static DeveloperPrivateAPI* Get(Profile* profile);

  explicit DeveloperPrivateAPI(Profile* profile);
  virtual ~DeveloperPrivateAPI();

  void SetLastUnpackedDirectory(const base::FilePath& path);

  base::FilePath& GetLastUnpackedDirectory() {
    return last_unpacked_directory_;
  }

  
  virtual void Shutdown() OVERRIDE;

  
  virtual void OnListenerAdded(const extensions::EventListenerInfo& details)
      OVERRIDE;
  virtual void OnListenerRemoved(const extensions::EventListenerInfo& details)
      OVERRIDE;

 private:
  void RegisterNotifications();

  Profile* profile_;

  
  
  base::FilePath last_unpacked_directory_;

  
  scoped_ptr<DeveloperPrivateEventRouter> developer_private_event_router_;

  DISALLOW_COPY_AND_ASSIGN(DeveloperPrivateAPI);

};

namespace api {

class DeveloperPrivateAutoUpdateFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.autoUpdate",
                             DEVELOPERPRIVATE_AUTOUPDATE)

 protected:
  virtual ~DeveloperPrivateAutoUpdateFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class DeveloperPrivateGetItemsInfoFunction
    : public ChromeAsyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.getItemsInfo",
                             DEVELOPERPRIVATE_GETITEMSINFO)

 protected:
  virtual ~DeveloperPrivateGetItemsInfoFunction();

  
  virtual bool RunImpl() OVERRIDE;

 private:

  scoped_ptr<developer::ItemInfo> CreateItemInfo(
      const extensions::Extension& item,
      bool item_is_enabled);

  void GetIconsOnFileThread(
      ItemInfoList item_list,
      std::map<std::string, ExtensionResource> itemIdToIconResourceMap);

  
  void GetInspectablePagesForExtensionProcess(
      const Extension* extension,
      const std::set<content::RenderViewHost*>& views,
      ItemInspectViewList* result);

  ItemInspectViewList GetInspectablePagesForExtension(
      const extensions::Extension* extension,
      bool extension_is_enabled);

  void GetShellWindowPagesForExtensionProfile(
      const extensions::Extension* extension,
      ItemInspectViewList* result);

  linked_ptr<developer::ItemInspectView> constructInspectView(
      const GURL& url,
      int render_process_id,
      int render_view_id,
      bool incognito,
      bool generated_background_page);
};

class DeveloperPrivateInspectFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.inspect",
                             DEVELOPERPRIVATE_INSPECT)

 protected:
  virtual ~DeveloperPrivateInspectFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class DeveloperPrivateAllowFileAccessFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.allowFileAccess",
                             DEVELOPERPRIVATE_ALLOWFILEACCESS);

 protected:
  virtual ~DeveloperPrivateAllowFileAccessFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class DeveloperPrivateAllowIncognitoFunction
    : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.allowIncognito",
                             DEVELOPERPRIVATE_ALLOWINCOGNITO);

 protected:
  virtual ~DeveloperPrivateAllowIncognitoFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class DeveloperPrivateReloadFunction : public ChromeSyncExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.reload",
                             DEVELOPERPRIVATE_RELOAD);

 protected:
  virtual ~DeveloperPrivateReloadFunction();

  
  virtual bool RunImpl() OVERRIDE;
};

class DeveloperPrivateShowPermissionsDialogFunction
    : public ChromeSyncExtensionFunction,
      public ExtensionInstallPrompt::Delegate {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.showPermissionsDialog",
                             DEVELOPERPRIVATE_PERMISSIONS);

  DeveloperPrivateShowPermissionsDialogFunction();
 protected:
  virtual ~DeveloperPrivateShowPermissionsDialogFunction();

  
  virtual bool RunImpl() OVERRIDE;

  
  virtual void InstallUIProceed() OVERRIDE;
  virtual void InstallUIAbort(bool user_initiated) OVERRIDE;

  scoped_ptr<ExtensionInstallPrompt> prompt_;
  std::string extension_id_;

};

class DeveloperPrivateEnableFunction
    : public ChromeSyncExtensionFunction,
      public base::SupportsWeakPtr<DeveloperPrivateEnableFunction> {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.enable",
                             DEVELOPERPRIVATE_ENABLE);

  DeveloperPrivateEnableFunction();

 protected:
  virtual ~DeveloperPrivateEnableFunction();

  
  void OnRequirementsChecked(std::string extension_id,
                             std::vector<std::string> requirements_errors);
  
  virtual bool RunImpl() OVERRIDE;

 private:
  scoped_ptr<extensions::RequirementsChecker> requirements_checker_;
};

class DeveloperPrivateChooseEntryFunction : public ChromeAsyncExtensionFunction,
                                            public EntryPickerClient {
 protected:
  virtual ~DeveloperPrivateChooseEntryFunction();
  virtual bool RunImpl() OVERRIDE;
  bool ShowPicker(ui::SelectFileDialog::Type picker_type,
                  const base::FilePath& last_directory,
                  const base::string16& select_title,
                  const ui::SelectFileDialog::FileTypeInfo& info,
                  int file_type_index);

  
  virtual void FileSelected(const base::FilePath& path) = 0;
  virtual void FileSelectionCanceled() = 0;
};


class DeveloperPrivateLoadUnpackedFunction
    : public DeveloperPrivateChooseEntryFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.loadUnpacked",
                             DEVELOPERPRIVATE_LOADUNPACKED);

 protected:
  virtual ~DeveloperPrivateLoadUnpackedFunction();
  virtual bool RunImpl() OVERRIDE;

  
  virtual void FileSelected(const base::FilePath& path) OVERRIDE;
  virtual void FileSelectionCanceled() OVERRIDE;
};

class DeveloperPrivateChoosePathFunction
    : public DeveloperPrivateChooseEntryFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.choosePath",
                             DEVELOPERPRIVATE_CHOOSEPATH);

 protected:
  virtual ~DeveloperPrivateChoosePathFunction();
  virtual bool RunImpl() OVERRIDE;

  
  virtual void FileSelected(const base::FilePath& path) OVERRIDE;
  virtual void FileSelectionCanceled() OVERRIDE;
};

class DeveloperPrivatePackDirectoryFunction
    : public ChromeAsyncExtensionFunction,
      public extensions::PackExtensionJob::Client {

 public:
  DECLARE_EXTENSION_FUNCTION("developerPrivate.packDirectory",
                             DEVELOPERPRIVATE_PACKDIRECTORY);

  DeveloperPrivatePackDirectoryFunction();

  
  virtual void OnPackSuccess(const base::FilePath& crx_file,
                             const base::FilePath& key_file) OVERRIDE;
  virtual void OnPackFailure(
      const std::string& error,
      extensions::ExtensionCreator::ErrorType error_type) OVERRIDE;

 protected:
  virtual ~DeveloperPrivatePackDirectoryFunction();
  virtual bool RunImpl() OVERRIDE;

 private:
  scoped_refptr<extensions::PackExtensionJob> pack_job_;
  std::string item_path_str_;
  std::string key_path_str_;
};

class DeveloperPrivateGetStringsFunction : public ChromeSyncExtensionFunction {
  public:
   DECLARE_EXTENSION_FUNCTION("developerPrivate.getStrings",
                              DEVELOPERPRIVATE_GETSTRINGS);

  protected:
   virtual ~DeveloperPrivateGetStringsFunction();

   
   virtual bool RunImpl() OVERRIDE;
};

class DeveloperPrivateIsProfileManagedFunction
    : public ChromeSyncExtensionFunction {
  public:
   DECLARE_EXTENSION_FUNCTION("developerPrivate.isProfileManaged",
                              DEVELOPERPRIVATE_ISPROFILEMANAGED);

  protected:
   virtual ~DeveloperPrivateIsProfileManagedFunction();

   
   virtual bool RunImpl() OVERRIDE;
};

class DeveloperPrivateExportSyncfsFolderToLocalfsFunction
    : public ChromeAsyncExtensionFunction {
  public:
   DECLARE_EXTENSION_FUNCTION("developerPrivate.exportSyncfsFolderToLocalfs",
                              DEVELOPERPRIVATE_LOADUNPACKEDCROS);

   DeveloperPrivateExportSyncfsFolderToLocalfsFunction();

  protected:
   virtual ~DeveloperPrivateExportSyncfsFolderToLocalfsFunction();

   
   virtual bool RunImpl() OVERRIDE;

   void ClearPrexistingDirectoryContent(const base::FilePath& project_path);

   void ReadSyncFileSystemDirectory(const base::FilePath& project_path,
                                    const base::FilePath& destination_path);

   void ReadSyncFileSystemDirectoryCb(
       const base::FilePath& project_path,
       const base::FilePath& destination_path,
       base::PlatformFileError result,
       const fileapi::FileSystemOperation::FileEntryList& file_list,
       bool has_more);

   void SnapshotFileCallback(
       const base::FilePath& target_path,
       base::PlatformFileError result,
       const base::PlatformFileInfo& file_info,
       const base::FilePath& platform_path,
       const scoped_refptr<webkit_blob::ShareableFileReference>& file_ref);

   void CopyFile(const base::FilePath& src_path,
                 const base::FilePath& dest_path);

   scoped_refptr<fileapi::FileSystemContext> context_;

  private:
   int pendingCopyOperationsCount_;

   
   
   bool success_;
};

class DeveloperPrivateLoadProjectFunction
    : public ChromeAsyncExtensionFunction {
  public:
   DECLARE_EXTENSION_FUNCTION("developerPrivate.loadProject",
                              DEVELOPERPRIVATE_LOADPROJECT);

   DeveloperPrivateLoadProjectFunction();

  protected:
   virtual ~DeveloperPrivateLoadProjectFunction();

   
   virtual bool RunImpl() OVERRIDE;

   void GetUnpackedExtension(const base::FilePath& path,
                             const ExtensionSet* extensions);
};

}  

}  

#endif  
