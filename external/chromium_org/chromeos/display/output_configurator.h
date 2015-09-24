// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DISPLAY_OUTPUT_CONFIGURATOR_H_
#define CHROMEOS_DISPLAY_OUTPUT_CONFIGURATOR_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/event_types.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "chromeos/chromeos_export.h"
#include "third_party/cros_system_api/dbus/service_constants.h"

typedef unsigned long XID;
typedef XID RROutput;
typedef XID RRCrtc;
typedef XID RRMode;

namespace chromeos {

enum OutputState {
  STATE_INVALID,
  STATE_HEADLESS,
  STATE_SINGLE,
  STATE_DUAL_MIRROR,
  STATE_DUAL_EXTENDED,
};

enum OutputType {
  OUTPUT_TYPE_NONE = 0,
  OUTPUT_TYPE_UNKNOWN = 1 << 0,
  OUTPUT_TYPE_INTERNAL = 1 << 1,
  OUTPUT_TYPE_VGA = 1 << 2,
  OUTPUT_TYPE_HDMI = 1 << 3,
  OUTPUT_TYPE_DVI = 1 << 4,
  OUTPUT_TYPE_DISPLAYPORT = 1 << 5,
  OUTPUT_TYPE_NETWORK = 1 << 6,
};

enum OutputProtectionMethod {
  OUTPUT_PROTECTION_METHOD_NONE = 0,
  OUTPUT_PROTECTION_METHOD_HDCP = 1 << 0,
};

enum HDCPState {
  HDCP_STATE_UNDESIRED,
  HDCP_STATE_DESIRED,
  HDCP_STATE_ENABLED
};

class CHROMEOS_EXPORT OutputConfigurator
    : public base::MessageLoop::Dispatcher,
      public base::MessagePumpObserver {
 public:
  typedef uint64_t OutputProtectionClientId;
  static const OutputProtectionClientId kInvalidClientId = 0;

  struct ModeInfo {
    ModeInfo();
    ModeInfo(int width, int height, bool interlaced, float refresh_rate);

    int width;
    int height;
    bool interlaced;
    float refresh_rate;
  };

  typedef std::map<RRMode, ModeInfo> ModeInfoMap;

  struct CoordinateTransformation {
    
    CoordinateTransformation();

    float x_scale;
    float x_offset;
    float y_scale;
    float y_offset;
  };

  
  struct OutputSnapshot {
    OutputSnapshot();
    ~OutputSnapshot();

    RROutput output;

    
    
    RRCrtc crtc;

    
    RRMode current_mode;

    
    RRMode native_mode;

    
    RRMode mirror_mode;

    
    RRMode selected_mode;

    
    int x;
    int y;

    
    uint64 width_mm;
    uint64 height_mm;

    
    bool is_internal;
    bool is_aspect_preserving_scaling;

    
    OutputType type;

    
    ModeInfoMap mode_infos;

    
    int touch_device_id;

    CoordinateTransformation transform;

    
    int64 display_id;

    bool has_display_id;

    
    
    int index;
  };

  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    
    virtual void OnDisplayModeChanged(
        const std::vector<OutputSnapshot>& outputs) {}

    
    
    virtual void OnDisplayModeChangeFailed(OutputState failed_new_state) {}
  };

  
  
  class StateController {
   public:
    virtual ~StateController() {}

    
    virtual OutputState GetStateForDisplayIds(
        const std::vector<int64>& display_ids) const = 0;

    
    
    virtual bool GetResolutionForDisplayId(int64 display_id,
                                           int* width,
                                           int* height) const = 0;
  };

  
  class SoftwareMirroringController {
   public:
    virtual ~SoftwareMirroringController() {}

    
    virtual void SetSoftwareMirroring(bool enabled) = 0;
  };

  
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    
    virtual void InitXRandRExtension(int* event_base) = 0;

    
    
    virtual void UpdateXRandRConfiguration(const base::NativeEvent& event) = 0;

    
    
    
    virtual void GrabServer() = 0;

    
    virtual void UngrabServer() = 0;

    
    virtual void SyncWithServer() = 0;

    
    virtual void SetBackgroundColor(uint32 color_argb) = 0;

    
    virtual void ForceDPMSOn() = 0;

    
    
    
    
    virtual std::vector<OutputSnapshot> GetOutputs() = 0;

    
    virtual void AddOutputMode(RROutput output, RRMode mode) = 0;

    
    
    virtual bool ConfigureCrtc(RRCrtc crtc,
                               RRMode mode,
                               RROutput output,
                               int x,
                               int y) = 0;

    
    
    virtual void CreateFrameBuffer(
        int width,
        int height,
        const std::vector<OutputConfigurator::OutputSnapshot>& outputs) = 0;

    
    
    
    
    
    virtual void ConfigureCTM(int touch_device_id,
                              const CoordinateTransformation& ctm) = 0;

    
    
    virtual void SendProjectingStateToPowerManager(bool projecting) = 0;

    
    virtual bool GetHDCPState(RROutput id, HDCPState* state) = 0;

    
    virtual bool SetHDCPState(RROutput id, HDCPState state) = 0;
  };

  
  class TestApi {
   public:
    TestApi(OutputConfigurator* configurator, int xrandr_event_base)
        : configurator_(configurator),
          xrandr_event_base_(xrandr_event_base) {}
    ~TestApi() {}

    const std::vector<OutputSnapshot>& cached_outputs() const {
      return configurator_->cached_outputs_;
    }

    
    void SendScreenChangeEvent();

    
    void SendOutputChangeEvent(RROutput output,
                               RRCrtc crtc,
                               RRMode mode,
                               bool connected);

    
    
    bool TriggerConfigureTimeout();

   private:
    OutputConfigurator* configurator_;  

    int xrandr_event_base_;

    DISALLOW_COPY_AND_ASSIGN(TestApi);
  };

  
  static const int kSetDisplayPowerNoFlags                     = 0;
  
  static const int kSetDisplayPowerForceProbe                  = 1 << 0;
  
  
  static const int kSetDisplayPowerOnlyIfSingleInternalDisplay = 1 << 1;

  
  
  
  
  
  
  
  static const int kVerticalGap = 60;

  
  
  static const ModeInfo* GetModeInfo(const OutputSnapshot& output,
                                     RRMode mode);

  
  
  static RRMode FindOutputModeMatchingSize(const OutputSnapshot& output,
                                           int width,
                                           int height);

  OutputConfigurator();
  virtual ~OutputConfigurator();

  OutputState output_state() const { return output_state_; }
  DisplayPowerState power_state() const { return power_state_; }

  void set_state_controller(StateController* controller) {
    state_controller_ = controller;
  }
  void set_mirroring_controller(SoftwareMirroringController* controller) {
    mirroring_controller_ = controller;
  }

  
  
  void SetDelegateForTesting(scoped_ptr<Delegate> delegate);

  
  void SetInitialDisplayPower(DisplayPowerState power_state);

  
  
  void Init(bool is_panel_fitting_enabled);

  
  
  
  
  void Start(uint32 background_color_argb);

  
  void Stop();

  
  
  
  
  bool SetDisplayPower(DisplayPowerState power_state, int flags);

  
  
  
  bool SetDisplayMode(OutputState new_state);

  
  
  
  
  
  virtual bool Dispatch(const base::NativeEvent& event) OVERRIDE;

  
  virtual base::EventStatus WillProcessEvent(
      const base::NativeEvent& event) OVERRIDE;
  virtual void DidProcessEvent(const base::NativeEvent& event) OVERRIDE;

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  
  
  void SuspendDisplays();

  
  
  void ResumeDisplays();

  const std::map<int, float>& GetMirroredDisplayAreaRatioMap() {
    return mirrored_display_area_ratio_map_;
  }

  
  
  void ScheduleConfigureOutputs();

  
  
  OutputProtectionClientId RegisterOutputProtectionClient();

  
  void UnregisterOutputProtectionClient(OutputProtectionClientId client_id);

  
  
  
  
  
  bool QueryOutputProtectionStatus(
      OutputProtectionClientId client_id,
      int64 display_id,
      uint32_t* link_mask,
      uint32_t* protection_mask);

  
  
  
  
  bool EnableOutputProtection(
      OutputProtectionClientId client_id,
      int64 display_id,
      uint32_t desired_protection_mask);

 private:
  
  typedef std::map<int64, uint32_t> DisplayProtections;
  
  typedef std::map<OutputProtectionClientId,
                   DisplayProtections> ProtectionRequests;

  
  
  
  
  void UpdateCachedOutputs();

  
  
  
  
  
  
  
  
  
  
  
  bool FindMirrorMode(OutputSnapshot* internal_output,
                      OutputSnapshot* external_output,
                      bool try_panel_fitting,
                      bool preserve_aspect);

  
  void ConfigureOutputs();

  
  void NotifyObservers(bool success, OutputState attempted_state);

  
  
  
  
  
  
  bool EnterStateOrFallBackToSoftwareMirroring(
      OutputState output_state,
      DisplayPowerState power_state);

  
  
  
  bool EnterState(OutputState output_state, DisplayPowerState power_state);

  
  
  OutputState ChooseOutputState(DisplayPowerState power_state) const;

  
  
  
  CoordinateTransformation GetMirrorModeCTM(
      const OutputConfigurator::OutputSnapshot& output);

  
  
  
  
  CoordinateTransformation GetExtendedModeCTM(
      const OutputConfigurator::OutputSnapshot& output,
      int framebuffer_width,
      int frame_buffer_height);

  
  
  float GetMirroredDisplayAreaRatio(
      const OutputConfigurator::OutputSnapshot& output);

  
  bool ApplyProtections(const DisplayProtections& requests);

  StateController* state_controller_;
  SoftwareMirroringController* mirroring_controller_;
  scoped_ptr<Delegate> delegate_;

  
  bool is_panel_fitting_enabled_;

  
  
  
  
  
  
  std::map<int, float> mirrored_display_area_ratio_map_;

  
  
  
  
  
  bool configure_display_;

  
  
  int xrandr_event_base_;

  
  OutputState output_state_;

  
  DisplayPowerState power_state_;

  
  
  std::vector<OutputSnapshot> cached_outputs_;

  ObserverList<Observer> observers_;

  
  
  scoped_ptr<base::OneShotTimer<OutputConfigurator> > configure_timer_;

  
  OutputProtectionClientId next_output_protection_client_id_;

  
  ProtectionRequests client_protection_requests_;

  DISALLOW_COPY_AND_ASSIGN(OutputConfigurator);
};

typedef std::vector<OutputConfigurator::OutputSnapshot> OutputSnapshotList;

}  

#endif  
