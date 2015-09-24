// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_WEBUI_OPTIONS_EXTENSION_SETTINGS_HANDLER_H_
#define CHROME_BROWSER_UI_WEBUI_OPTIONS_EXTENSION_SETTINGS_HANDLER_H_
#pragma once

#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/extension_uninstall_dialog.h"
#include "chrome/browser/extensions/pack_extension_job.h"
#include "chrome/browser/ui/shell_dialogs.h"
#include "chrome/browser/ui/webui/chrome_url_data_manager.h"
#include "chrome/common/extensions/extension_resource.h"
#include "content/browser/webui/web_ui.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"
#include "googleurl/src/gurl.h"

class DictionaryValue;
class Extension;
class ExtensionService;
class FilePath;
class ListValue;
class PrefService;
class RenderProcessHost;
class UserScript;

struct ExtensionPage {
  ExtensionPage(const GURL& url, int render_process_id, int render_view_id,
                bool incognito)
    : url(url),
      render_process_id(render_process_id),
      render_view_id(render_view_id),
      incognito(incognito) {}
  GURL url;
  int render_process_id;
  int render_view_id;
  bool incognito;
};

class ExtensionsUIHTMLSource : public ChromeURLDataManager::DataSource {
 public:
  ExtensionsUIHTMLSource();

  
  
  virtual void StartDataRequest(const std::string& path,
                                bool is_incognito,
                                int request_id);
  virtual std::string GetMimeType(const std::string&) const;

 private:
  ~ExtensionsUIHTMLSource() {}

  DISALLOW_COPY_AND_ASSIGN(ExtensionsUIHTMLSource);
};

class ExtensionsDOMHandler : public WebUIMessageHandler,
                             public NotificationObserver,
                             public PackExtensionJob::Client,
                             public SelectFileDialog::Listener,
                             public ExtensionUninstallDialog::Delegate {
 public:

  
  
  
  
  
  
  
  
  class IconLoader : public base::RefCountedThreadSafe<IconLoader> {
   public:
    explicit IconLoader(ExtensionsDOMHandler* handler);

    
    
    void LoadIcons(std::vector<ExtensionResource>* icons,
                   DictionaryValue* json);

    
    
    void Cancel();

   private:
    
    
    void LoadIconsOnFileThread(std::vector<ExtensionResource>* icons,
                               DictionaryValue* json);

    
    void ReportResultOnUIThread(DictionaryValue* json);

    
    ExtensionsDOMHandler* handler_;
  };

  explicit ExtensionsDOMHandler(ExtensionService* extension_service);
  virtual ~ExtensionsDOMHandler();

  
  virtual void RegisterMessages();

  
  
  static DictionaryValue* CreateExtensionDetailValue(
      ExtensionService* service,
      const Extension* extension,
      const std::vector<ExtensionPage>& pages,
      bool enabled,
      bool terminated);

  
  static DictionaryValue* CreateContentScriptDetailValue(
      const UserScript& script,
      const FilePath& extension_path);

  
  virtual void OnPackSuccess(const FilePath& crx_file,
                             const FilePath& key_file);

  virtual void OnPackFailure(const std::string& error);

  
  virtual void ExtensionDialogAccepted();
  virtual void ExtensionDialogCanceled();

 private:
  
  void HandleRequestExtensionsData(const ListValue* args);

  
  void HandleToggleDeveloperMode(const ListValue* args);

  
  void HandleInspectMessage(const ListValue* args);

  
  void HandleReloadMessage(const ListValue* args);

  
  void HandleEnableMessage(const ListValue* args);

  
  void HandleEnableIncognitoMessage(const ListValue* args);

  
  void HandleAllowFileAccessMessage(const ListValue* args);

  
  void HandleUninstallMessage(const ListValue* args);

  
  void HandleOptionsMessage(const ListValue* args);

  
  void HandleShowButtonMessage(const ListValue* args);

  
  void HandleLoadMessage(const ListValue* args);

  
  void HandlePackMessage(const ListValue* args);

  
  void HandleAutoUpdateMessage(const ListValue* args);

  
  void ShowAlert(const std::string& message);

  
  void HandleSelectFilePathMessage(const ListValue* args);

  
  const Extension* GetExtension(const ListValue* args);

  
  void MaybeUpdateAfterNotification();

  
  virtual void FileSelected(const FilePath& path,
                            int index, void* params);
  virtual void MultiFilesSelected(
      const std::vector<FilePath>& files, void* params);
  virtual void FileSelectionCanceled(void* params) {}

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  std::vector<ExtensionPage> GetActivePagesForExtension(
      const Extension* extension);
  void GetActivePagesForExtensionProcess(
      RenderProcessHost* process,
      const Extension* extension,
      std::vector<ExtensionPage> *result);

  
  
  ExtensionResource PickExtensionIcon(const Extension* extension);

  
  
  
  void LoadExtensionIcons(std::vector<ExtensionResource>* icons,
                          DictionaryValue* json_data);

  
  
  void OnIconsLoaded(DictionaryValue* json_data);

  
  
  ExtensionUninstallDialog* GetExtensionUninstallDialog();

  
  scoped_refptr<ExtensionService> extensions_service_;

  
  scoped_refptr<SelectFileDialog> load_extension_dialog_;

  
  scoped_refptr<PackExtensionJob> pack_job_;

  
  scoped_refptr<IconLoader> icon_loader_;

  
  scoped_ptr<ExtensionUninstallDialog> extension_uninstall_dialog_;

  
  std::string extension_id_prompting_;

  
  
  NotificationRegistrar registrar_;

  
  
  
  bool ignore_notifications_;

  
  
  
  
  
  RenderViewHost* deleting_rvh_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionsDOMHandler);
};

class ExtensionsUI : public WebUI {
 public:
  explicit ExtensionsUI(TabContents* contents);

  static RefCountedMemory* GetFaviconResourceBytes();

  static void RegisterUserPrefs(PrefService* prefs);

 private:
  DISALLOW_COPY_AND_ASSIGN(ExtensionsUI);
};

#endif  
