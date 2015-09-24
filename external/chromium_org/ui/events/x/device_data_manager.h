// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_EVENTS_X_DEVICE_DATA_MANAGER_H_
#define UI_EVENTS_X_DEVICE_DATA_MANAGER_H_

#ifndef Bool
#define Bool int
#endif

#include <X11/extensions/XInput2.h>

#include <bitset>
#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/event_types.h"
#include "ui/events/event_constants.h"
#include "ui/events/events_base_export.h"
#include "ui/gfx/x/x11_atom_cache.h"

template <typename T> struct DefaultSingletonTraits;

typedef union _XEvent XEvent;

namespace ui {

enum GestureMetricsType {
  kGestureMetricsTypeNoisyGround = 0,
  kGestureMetricsTypeUnknown,
};

class EVENTS_BASE_EXPORT DeviceDataManager {
 public:
  
  
  
  
  
  enum DataType {
    
    
    DT_CMT_SCROLL_X = 0,  
    DT_CMT_SCROLL_Y,      
    DT_CMT_ORDINAL_X,     
                          
    DT_CMT_ORDINAL_Y,     
                          
    DT_CMT_START_TIME,    
    DT_CMT_END_TIME,      
    DT_CMT_FLING_X,       
    DT_CMT_FLING_Y,       
    DT_CMT_FLING_STATE,   
                          
    DT_CMT_METRICS_TYPE,  
                          
                          
    DT_CMT_METRICS_DATA1, 
    DT_CMT_METRICS_DATA2, 
    DT_CMT_FINGER_COUNT,  
                          
                          

    
    

    
    
    DT_TOUCH_MAJOR,       
    DT_TOUCH_MINOR,       
    DT_TOUCH_ORIENTATION, 
                          
    DT_TOUCH_PRESSURE,    

    
    
    
    
    DT_TOUCH_TRACKING_ID, 

    
    DT_TOUCH_RAW_TIMESTAMP,

    

    DT_LAST_ENTRY         
  };

  
  typedef std::map<int, double> EventData;

  
  static bool IsCMTDataType(const int type);
  static bool IsTouchDataType(const int type);

  
  static DeviceDataManager* GetInstance();

  
  bool natural_scroll_enabled() const { return natural_scroll_enabled_; }
  void set_natural_scroll_enabled(bool enabled) {
    natural_scroll_enabled_ = enabled;
  }

  
  bool IsXInput2Available() const;

  
  float GetNaturalScrollFactor(int sourceid) const;

  
  void UpdateDeviceList(Display* display);

  
  
  
  
  bool GetSlotNumber(const XIDeviceEvent* xiev, int* slot);

  
  
  
  void GetEventRawData(const XEvent& xev, EventData* data);

  
  
  
  
  bool GetEventData(const XEvent& xev, const DataType type, double* value);

  
  
  
  bool IsXIDeviceEvent(const base::NativeEvent& native_event) const;

  
  bool IsTouchpadXInputEvent(const base::NativeEvent& native_event) const;

  
  
  
  bool IsCMTDeviceEvent(const base::NativeEvent& native_event) const;

  
  
  bool IsCMTGestureEvent(const base::NativeEvent& native_event) const;

  
  bool IsScrollEvent(const base::NativeEvent& native_event) const;
  bool IsFlingEvent(const base::NativeEvent& native_event) const;
  bool IsCMTMetricsEvent(const base::NativeEvent& native_event) const;

  
  bool HasGestureTimes(const base::NativeEvent& native_event) const;

  
  
  
  void GetScrollOffsets(const base::NativeEvent& native_event,
                        float* x_offset,
                        float* y_offset,
                        float* x_offset_ordinal,
                        float* y_offset_ordinal,
                        int* finger_count);

  
  
  void GetFlingData(const base::NativeEvent& native_event,
                    float* vx,
                    float* vy,
                    float* vx_ordinal,
                    float* vy_ordinal,
                    bool* is_cancel);

  
  
  void GetMetricsData(const base::NativeEvent& native_event,
                      GestureMetricsType* type,
                      float* data1,
                      float* data2);

  
  int GetMappedButton(int button);

  
  
  void UpdateButtonMap();

  
  
  void GetGestureTimes(const base::NativeEvent& native_event,
                       double* start_time,
                       double* end_time);

  
  
  
  
  bool NormalizeData(unsigned int deviceid,
                     const DataType type,
                     double* value);

  
  // and written into min & max, false if the range is not available.
  bool GetDataRange(unsigned int deviceid,
                    const DataType type,
                    double* min,
                    double* max);

  
  
  
  
  void SetDeviceListForTest(const std::vector<unsigned int>& touchscreen,
                            const std::vector<unsigned int>& cmt_devices);

  void SetValuatorDataForTest(XIDeviceEvent* xievent,
                              DataType type,
                              double value);

 private:
  
  friend struct DefaultSingletonTraits<DeviceDataManager>;

  DeviceDataManager();
  ~DeviceDataManager();

  
  bool InitializeXInputInternal();

  
  bool HasEventData(const XIDeviceEvent* xiev, const DataType type) const;

  void InitializeValuatorsForTest(int deviceid,
                                  int start_valuator,
                                  int end_valuator,
                                  double min_value,
                                  double max_value);

  static const int kMaxDeviceNum = 128;
  static const int kMaxXIEventType = XI_LASTEVENT + 1;
  static const int kMaxSlotNum = 10;
  bool natural_scroll_enabled_;

  
  int xi_opcode_;

  
  std::bitset<kMaxXIEventType> xi_device_event_types_;

  
  
  std::bitset<kMaxDeviceNum> cmt_devices_;
  std::bitset<kMaxDeviceNum> touchpads_;

  
  int valuator_count_[kMaxDeviceNum];

  
  
  std::vector<int> valuator_lookup_[kMaxDeviceNum];

  
  
  std::vector<int> data_type_lookup_[kMaxDeviceNum];

  
  
  std::vector<double> valuator_min_[kMaxDeviceNum];
  std::vector<double> valuator_max_[kMaxDeviceNum];

  
  
  
  
  
  
  
  std::vector<double> last_seen_valuator_[kMaxDeviceNum][kMaxSlotNum];

  
  X11AtomCache atom_cache_;

  unsigned char button_map_[256];
  int button_map_count_;

  DISALLOW_COPY_AND_ASSIGN(DeviceDataManager);
};

}  

#endif  
