// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_MINI_INSTALLER_CONFIGURATION_H_
#define CHROME_INSTALLER_MINI_INSTALLER_CONFIGURATION_H_

namespace mini_installer {

class Configuration {
 public:
  enum Operation {
    INSTALL_PRODUCT,
    CLEANUP,
  };

  Configuration();
  ~Configuration();

  
  bool Initialize();

  
  Operation operation() const { return operation_; }

  
  
  const wchar_t* program() const;

  
  
  int argument_count() const { return argument_count_; }

  
  const wchar_t* command_line() const { return command_line_; }

  
  
  const wchar_t* chrome_app_guid() const { return chrome_app_guid_; }

  
  bool has_chrome() const { return has_chrome_; }

  
  bool has_chrome_frame() const { return has_chrome_frame_; }

  
  bool has_app_host() const { return has_app_host_; }

  
  bool is_multi_install() const { return is_multi_install_; }

  
  bool is_system_level() const { return is_system_level_; }

 protected:
  void Clear();
  bool InitializeFromCommandLine(const wchar_t* command_line);

  wchar_t** args_;
  const wchar_t* chrome_app_guid_;
  const wchar_t* command_line_;
  int argument_count_;
  Operation operation_;
  bool has_chrome_;
  bool has_chrome_frame_;
  bool has_app_host_;
  bool is_multi_install_;
  bool is_system_level_;

 private:
  Configuration(const Configuration&);
  Configuration& operator=(const Configuration&);
};

}  

#endif  
