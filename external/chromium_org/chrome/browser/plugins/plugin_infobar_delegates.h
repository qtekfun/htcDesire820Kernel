// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGINS_PLUGIN_INFOBAR_DELEGATES_H_
#define CHROME_BROWSER_PLUGINS_PLUGIN_INFOBAR_DELEGATES_H_

#include "base/callback.h"
#include "chrome/browser/infobars/confirm_infobar_delegate.h"
#include "url/gurl.h"

#if defined(ENABLE_PLUGIN_INSTALLATION)
#include "chrome/browser/plugins/plugin_installer_observer.h"
#endif

class InfoBarService;
class HostContentSettingsMap;
class PluginMetadata;

namespace content {
class WebContents;
}

class PluginInfoBarDelegate : public ConfirmInfoBarDelegate {
 protected:
  explicit PluginInfoBarDelegate(const std::string& identifier);
  virtual ~PluginInfoBarDelegate();

  
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;

  virtual std::string GetLearnMoreURL() const = 0;

  void LoadBlockedPlugins();

 private:
  
  virtual int GetIconID() const OVERRIDE;
  virtual base::string16 GetLinkText() const OVERRIDE;

  std::string identifier_;

  DISALLOW_COPY_AND_ASSIGN(PluginInfoBarDelegate);
};

class UnauthorizedPluginInfoBarDelegate : public PluginInfoBarDelegate {
 public:
  
  
  static void Create(InfoBarService* infobar_service,
                     HostContentSettingsMap* content_settings,
                     const base::string16& name,
                     const std::string& identifier);

 private:
  UnauthorizedPluginInfoBarDelegate(HostContentSettingsMap* content_settings,
                                    const base::string16& name,
                                    const std::string& identifier);
  virtual ~UnauthorizedPluginInfoBarDelegate();

  
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual void InfoBarDismissed() OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;
  virtual std::string GetLearnMoreURL() const OVERRIDE;

  HostContentSettingsMap* content_settings_;
  base::string16 name_;

  DISALLOW_COPY_AND_ASSIGN(UnauthorizedPluginInfoBarDelegate);
};

#if defined(ENABLE_PLUGIN_INSTALLATION)
class OutdatedPluginInfoBarDelegate : public PluginInfoBarDelegate,
                                      public WeakPluginInstallerObserver {
 public:
  
  
  static void Create(InfoBarService* infobar_service,
                     PluginInstaller* installer,
                     scoped_ptr<PluginMetadata> metadata);

 private:
  OutdatedPluginInfoBarDelegate(PluginInstaller* installer,
                                scoped_ptr<PluginMetadata> metadata,
                                const base::string16& message);
  virtual ~OutdatedPluginInfoBarDelegate();

  
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual bool Cancel() OVERRIDE;
  virtual void InfoBarDismissed() OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;
  virtual std::string GetLearnMoreURL() const OVERRIDE;

  
  virtual void DownloadStarted() OVERRIDE;
  virtual void DownloadError(const std::string& message) OVERRIDE;
  virtual void DownloadCancelled() OVERRIDE;
  virtual void DownloadFinished() OVERRIDE;

  
  virtual void OnlyWeakObserversLeft() OVERRIDE;

  
  
  void ReplaceWithInfoBar(const base::string16& message);

  scoped_ptr<PluginMetadata> plugin_metadata_;

  base::string16 message_;

  DISALLOW_COPY_AND_ASSIGN(OutdatedPluginInfoBarDelegate);
};

class PluginInstallerInfoBarDelegate : public ConfirmInfoBarDelegate,
                                       public WeakPluginInstallerObserver {
 public:
  typedef base::Callback<void(const PluginMetadata*)> InstallCallback;

  
  
  
  
  static void Create(InfoBarService* infobar_service,
                     PluginInstaller* installer,
                     scoped_ptr<PluginMetadata> plugin_metadata,
                     const InstallCallback& callback);

  
  
  static void Replace(InfoBar* infobar,
                      PluginInstaller* installer,
                      scoped_ptr<PluginMetadata> plugin_metadata,
                      bool new_install,
                      const base::string16& message);

 private:
  PluginInstallerInfoBarDelegate(PluginInstaller* installer,
                                 scoped_ptr<PluginMetadata> metadata,
                                 const InstallCallback& callback,
                                 bool new_install,
                                 const base::string16& message);
  virtual ~PluginInstallerInfoBarDelegate();

  
  virtual int GetIconID() const OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual int GetButtons() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual base::string16 GetLinkText() const OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;

  
  virtual void DownloadStarted() OVERRIDE;
  virtual void DownloadError(const std::string& message) OVERRIDE;
  virtual void DownloadCancelled() OVERRIDE;
  virtual void DownloadFinished() OVERRIDE;

  
  virtual void OnlyWeakObserversLeft() OVERRIDE;

  
  
  void ReplaceWithInfoBar(const base::string16& message);

  scoped_ptr<PluginMetadata> plugin_metadata_;

  InstallCallback callback_;

  
  bool new_install_;

  base::string16 message_;

  DISALLOW_COPY_AND_ASSIGN(PluginInstallerInfoBarDelegate);
};
#endif  

#if defined(OS_WIN)
class PluginMetroModeInfoBarDelegate : public ConfirmInfoBarDelegate {
 public:
  
  
  
  enum Mode {
    MISSING_PLUGIN,
    DESKTOP_MODE_REQUIRED,
  };

  
  
  static void Create(InfoBarService* infobar_service,
                     Mode mode,
                     const base::string16& name);

 private:
  PluginMetroModeInfoBarDelegate(Mode mode, const base::string16& name);
  virtual ~PluginMetroModeInfoBarDelegate();

  
  virtual int GetIconID() const OVERRIDE;
  virtual base::string16 GetMessageText() const OVERRIDE;
  virtual int GetButtons() const OVERRIDE;
  virtual base::string16 GetButtonLabel(InfoBarButton button) const OVERRIDE;
  virtual bool Accept() OVERRIDE;
  virtual base::string16 GetLinkText() const OVERRIDE;
  virtual bool LinkClicked(WindowOpenDisposition disposition) OVERRIDE;

  const Mode mode_;
  const base::string16 name_;

  DISALLOW_COPY_AND_ASSIGN(PluginMetroModeInfoBarDelegate);
};
#endif  

#endif  
