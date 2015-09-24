// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PLUGINS_PLUGIN_INSTALLER_OBSERVER_H_
#define CHROME_BROWSER_PLUGINS_PLUGIN_INSTALLER_OBSERVER_H_

#include <string>

class PluginInstaller;

class PluginInstallerObserver {
 public:
  explicit PluginInstallerObserver(PluginInstaller* installer);
  virtual ~PluginInstallerObserver();

 protected:
  PluginInstaller* installer() const { return installer_; }

 private:
  friend class PluginInstaller;

  virtual void DownloadStarted();
  virtual void DownloadFinished();
  virtual void DownloadError(const std::string& message);
  virtual void DownloadCancelled();

  
  PluginInstaller* installer_;
};

class WeakPluginInstallerObserver : public PluginInstallerObserver {
 public:
  explicit WeakPluginInstallerObserver(PluginInstaller* installer);
  virtual ~WeakPluginInstallerObserver();

 private:
  friend class PluginInstaller;

  virtual void OnlyWeakObserversLeft();
};

#endif  