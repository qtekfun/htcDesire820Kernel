// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_TRAY_SYSTEM_TRAY_DELEGATE_H_
#define ASH_SYSTEM_TRAY_SYSTEM_TRAY_DELEGATE_H_

#include <string>
#include <vector>

#include "ash/ash_export.h"
#include "ash/system/user/login_status.h"
#include "base/files/file_path.h"
#include "base/i18n/time_formatting.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/native_widget_types.h"

namespace base {
class TimeDelta;
class TimeTicks;
}

namespace ash {

struct ASH_EXPORT NetworkIconInfo {
  NetworkIconInfo();
  ~NetworkIconInfo();

  bool highlight() const { return connected || connecting; }

  bool connecting;
  bool connected;
  bool tray_icon_visible;
  gfx::ImageSkia image;
  base::string16 name;
  base::string16 description;
  std::string service_path;
  bool is_cellular;
};

struct ASH_EXPORT BluetoothDeviceInfo {
  BluetoothDeviceInfo();
  ~BluetoothDeviceInfo();

  std::string address;
  base::string16 display_name;
  bool connected;
  bool connecting;
  bool paired;
};

typedef std::vector<BluetoothDeviceInfo> BluetoothDeviceList;

struct ASH_EXPORT DriveOperationStatus {
  enum OperationType {
    OPERATION_UPLOAD,
    OPERATION_DOWNLOAD
  };

  enum OperationState {
    OPERATION_NOT_STARTED,
    OPERATION_IN_PROGRESS,
    OPERATION_COMPLETED,
    OPERATION_FAILED,
  };

  DriveOperationStatus();
  ~DriveOperationStatus();

  
  int32 id;

  
  base::FilePath file_path;
  
  double progress;
  OperationType type;
  OperationState state;
};

typedef std::vector<DriveOperationStatus> DriveOperationStatusList;


struct ASH_EXPORT IMEPropertyInfo {
  IMEPropertyInfo();
  ~IMEPropertyInfo();

  bool selected;
  std::string key;
  base::string16 name;
};

typedef std::vector<IMEPropertyInfo> IMEPropertyInfoList;

struct ASH_EXPORT IMEInfo {
  IMEInfo();
  ~IMEInfo();

  bool selected;
  bool third_party;
  std::string id;
  base::string16 name;
  base::string16 medium_name;
  base::string16 short_name;
};

typedef std::vector<IMEInfo> IMEInfoList;

class VolumeControlDelegate;

class ASH_EXPORT SystemTrayDelegate {
 public:
  virtual ~SystemTrayDelegate() {}

  
  virtual void Initialize() = 0;

  
  virtual void Shutdown() = 0;

  
  virtual bool GetTrayVisibilityOnStartup() = 0;

  
  virtual user::LoginStatus GetUserLoginStatus() const = 0;
  virtual bool IsOobeCompleted() const = 0;

  
  virtual void ChangeProfilePicture() = 0;

  
  virtual const std::string GetEnterpriseDomain() const = 0;

  
  virtual const base::string16 GetEnterpriseMessage() const = 0;

  
  
  virtual const std::string GetLocallyManagedUserManager() const = 0;

  
  virtual const base::string16 GetLocallyManagedUserManagerName() const = 0;

  
  virtual const base::string16 GetLocallyManagedUserMessage() const = 0;

  
  virtual bool SystemShouldUpgrade() const = 0;

  
  virtual base::HourClockType GetHourClockType() const = 0;

  
  virtual void ShowSettings() = 0;

  
  virtual bool ShouldShowSettings() = 0;

  
  virtual void ShowDateSettings() = 0;

  
  
  virtual void ShowNetworkSettings(const std::string& service_path) = 0;

  
  virtual void ShowBluetoothSettings() = 0;

  
  virtual void ShowDisplaySettings() = 0;

  
  virtual void ShowChromeSlow() = 0;

  
  
  virtual bool ShouldShowDisplayNotification() = 0;

  
  virtual void ShowDriveSettings() = 0;

  
  virtual void ShowIMESettings() = 0;

  
  virtual void ShowHelp() = 0;

  
  virtual void ShowAccessibilityHelp() = 0;

  
  virtual void ShowAccessibilitySettings() = 0;

  
  virtual void ShowPublicAccountInfo() = 0;

  
  virtual void ShowEnterpriseInfo() = 0;

  
  virtual void ShowLocallyManagedUserInfo() = 0;

  
  virtual void ShowUserLogin() = 0;

  
  virtual void ShowSpringChargerReplacementDialog() = 0;

  
  virtual bool HasUserConfirmedSafeSpringCharger() = 0;

  
  virtual void ShutDown() = 0;

  
  virtual void SignOut() = 0;

  
  virtual void RequestLockScreen() = 0;

  
  virtual void RequestRestartForUpdate() = 0;

  
  virtual void GetAvailableBluetoothDevices(BluetoothDeviceList* devices) = 0;

  
  virtual void BluetoothStartDiscovering() = 0;

  
  virtual void BluetoothStopDiscovering() = 0;

  
  virtual void ConnectToBluetoothDevice(const std::string& address) = 0;

  
  virtual bool IsBluetoothDiscovering() = 0;

  
  virtual void GetCurrentIME(IMEInfo* info) = 0;

  
  virtual void GetAvailableIMEList(IMEInfoList* list) = 0;

  
  virtual void GetCurrentIMEProperties(IMEPropertyInfoList* list) = 0;

  
  virtual void SwitchIME(const std::string& ime_id) = 0;

  
  virtual void ActivateIMEProperty(const std::string& key) = 0;

  
  virtual void CancelDriveOperation(int32 operation_id) = 0;

  
  virtual void GetDriveOperationStatusList(
      DriveOperationStatusList* list) = 0;

  
  
  
  
  virtual void ShowNetworkConfigure(const std::string& network_id,
                                    gfx::NativeWindow parent_window) = 0;

  
  
  
  virtual bool EnrollNetwork(const std::string& network_id,
                             gfx::NativeWindow parent_window) = 0;

  
  virtual void ManageBluetoothDevices() = 0;

  
  virtual void ToggleBluetooth() = 0;

  
  virtual void ShowMobileSimDialog() = 0;

  
  virtual void ShowMobileSetupDialog(const std::string& service_path) = 0;

  
  
  virtual void ShowOtherNetworkDialog(const std::string& type) = 0;

  
  virtual bool GetBluetoothAvailable() = 0;

  
  virtual bool GetBluetoothEnabled() = 0;

  
  virtual void ChangeProxySettings() = 0;

  
  virtual VolumeControlDelegate* GetVolumeControlDelegate() const = 0;

  
  virtual void SetVolumeControlDelegate(
      scoped_ptr<VolumeControlDelegate> delegate) = 0;

  
  virtual bool GetSessionStartTime(base::TimeTicks* session_start_time) = 0;

  
  virtual bool GetSessionLengthLimit(base::TimeDelta* session_length_limit) = 0;

  
  virtual int GetSystemTrayMenuWidth() = 0;
};

}  

#endif  
