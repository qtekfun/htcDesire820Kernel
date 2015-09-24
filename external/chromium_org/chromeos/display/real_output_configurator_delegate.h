// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DISPLAY_REAL_OUTPUT_CONFIGURATOR_DELEGATE_H_
#define CHROMEOS_DISPLAY_REAL_OUTPUT_CONFIGURATOR_DELEGATE_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chromeos/display/output_configurator.h"

typedef XID Window;

struct _XDisplay;
typedef struct _XDisplay Display;
struct _XRROutputInfo;
typedef _XRROutputInfo XRROutputInfo;
struct _XRRScreenResources;
typedef _XRRScreenResources XRRScreenResources;

namespace chromeos {

class RealOutputConfiguratorDelegate : public OutputConfigurator::Delegate {
 public:
  RealOutputConfiguratorDelegate();
  virtual ~RealOutputConfiguratorDelegate();

  
  virtual void InitXRandRExtension(int* event_base) OVERRIDE;
  virtual void UpdateXRandRConfiguration(
      const base::NativeEvent& event) OVERRIDE;
  virtual void GrabServer() OVERRIDE;
  virtual void UngrabServer() OVERRIDE;
  virtual void SyncWithServer() OVERRIDE;
  virtual void SetBackgroundColor(uint32 color_argb) OVERRIDE;
  virtual void ForceDPMSOn() OVERRIDE;
  virtual std::vector<OutputConfigurator::OutputSnapshot> GetOutputs() OVERRIDE;
  virtual void AddOutputMode(RROutput output, RRMode mode) OVERRIDE;
  virtual bool ConfigureCrtc(
      RRCrtc crtc,
      RRMode mode,
      RROutput output,
      int x,
      int y) OVERRIDE;
  virtual void CreateFrameBuffer(
      int width,
      int height,
      const std::vector<OutputConfigurator::OutputSnapshot>& outputs) OVERRIDE;
  virtual void ConfigureCTM(
      int touch_device_id,
      const OutputConfigurator::CoordinateTransformation& ctm) OVERRIDE;
  virtual void SendProjectingStateToPowerManager(bool projecting) OVERRIDE;
  virtual bool GetHDCPState(RROutput id, HDCPState* state) OVERRIDE;
  virtual bool SetHDCPState(RROutput id, HDCPState state) OVERRIDE;

 private:
  
  
  bool InitModeInfo(RRMode mode, OutputConfigurator::ModeInfo* mode_info);

  
  
  
  OutputConfigurator::OutputSnapshot InitOutputSnapshot(
      RROutput id,
      XRROutputInfo* info,
      RRCrtc* last_used_crtc,
      int index);

  
  
  
  void DestroyUnusedCrtcs(
      const std::vector<OutputConfigurator::OutputSnapshot>& outputs);

  
  bool IsOutputAspectPreservingScaling(RROutput id);

  
  
  
  
  
  void GetTouchscreens(
      std::vector<OutputConfigurator::OutputSnapshot>* outputs);

  Display* display_;
  Window window_;

  
  XRRScreenResources* screen_;

  DISALLOW_COPY_AND_ASSIGN(RealOutputConfiguratorDelegate);
};

}  

#endif  
