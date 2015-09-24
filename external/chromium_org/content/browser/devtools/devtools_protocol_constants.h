// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_PROTOCOL_CONSTANTSH_
#define CONTENT_BROWSER_DEVTOOLS_DEVTOOLS_PROTOCOL_CONSTANTSH_


namespace content {
namespace devtools {

extern const char kResult[];

namespace DOM {
  extern const char kName[];

  namespace Rect {
    extern const char kParamHeight[];
    extern const char kParamWidth[];
    extern const char kParamX[];
    extern const char kParamY[];
  } 

  namespace setFileInputFiles {
    extern const char kName[];
    extern const char kParamFiles[];
    extern const char kParamNodeId[];
  } 
} 

namespace Input {
  extern const char kName[];

  namespace dispatchGestureEvent {
    extern const char kName[];
    extern const char kParamDeltaX[];
    extern const char kParamDeltaY[];
    extern const char kParamPinchScale[];
    extern const char kParamTimestamp[];
    extern const char kParamType[];
    extern const char kParamX[];
    extern const char kParamY[];

    namespace Type {
      extern const char kEnumPinchBegin[];
      extern const char kEnumPinchEnd[];
      extern const char kEnumPinchUpdate[];
      extern const char kEnumScrollBegin[];
      extern const char kEnumScrollEnd[];
      extern const char kEnumScrollUpdate[];
      extern const char kEnumTap[];
      extern const char kEnumTapDown[];
    } 
  } 

  namespace dispatchMouseEvent {
    extern const char kName[];
    extern const char kParamButton[];
    extern const char kParamClickCount[];
    extern const char kParamDeviceSpace[];
    extern const char kParamModifiers[];
    extern const char kParamTimestamp[];
    extern const char kParamType[];
    extern const char kParamX[];
    extern const char kParamY[];

    namespace Button {
      extern const char kEnumLeft[];
      extern const char kEnumMiddle[];
      extern const char kEnumNone[];
      extern const char kEnumRight[];
    } 

    namespace Type {
      extern const char kEnumMouseMoved[];
      extern const char kEnumMousePressed[];
      extern const char kEnumMouseReleased[];
    } 
  } 
} 

namespace Inspector {
  extern const char kName[];

  namespace detached {
    extern const char kName[];
    extern const char kParamReason[];
  } 

  namespace targetCrashed {
    extern const char kName[];
  } 
} 

namespace Page {
  extern const char kName[];

  namespace NavigationEntry {
    extern const char kParamId[];
    extern const char kParamTitle[];
    extern const char kParamUrl[];
  } 

  namespace Quota {
    extern const char kParamPersistent[];
    extern const char kParamTemporary[];
  } 

  namespace ScreencastFrameMetadata {
    extern const char kParamDeviceScaleFactor[];
    extern const char kParamOffsetBottom[];
    extern const char kParamOffsetTop[];
    extern const char kParamPageScaleFactor[];
    extern const char kParamPageScaleFactorMax[];
    extern const char kParamPageScaleFactorMin[];
    extern const char kParamViewport[];
  } 

  namespace Usage {
    extern const char kParamPersistent[];
    extern const char kParamSyncable[];
    extern const char kParamTemporary[];
  } 

  namespace UsageItem {
    extern const char kParamId[];
    extern const char kParamValue[];

    namespace Id {
      extern const char kEnumAppcache[];
      extern const char kEnumDatabase[];
      extern const char kEnumFilesystem[];
      extern const char kEnumIndexeddatabase[];
    } 
  } 

  namespace canScreencast {
    extern const char kName[];
    extern const char kResponseResult[];
  } 

  namespace captureScreenshot {
    extern const char kName[];
    extern const char kParamFormat[];
    extern const char kParamMaxHeight[];
    extern const char kParamMaxWidth[];
    extern const char kParamQuality[];
    extern const char kResponseData[];
    extern const char kResponseMetadata[];

    namespace Format {
      extern const char kEnumJpeg[];
      extern const char kEnumPng[];
    } 
  } 

  namespace disable {
    extern const char kName[];
  } 

  namespace getNavigationHistory {
    extern const char kName[];
    extern const char kResponseCurrentIndex[];
    extern const char kResponseEntries[];
  } 

  namespace handleJavaScriptDialog {
    extern const char kName[];
    extern const char kParamAccept[];
    extern const char kParamPromptText[];
  } 

  namespace navigate {
    extern const char kName[];
    extern const char kParamUrl[];
  } 

  namespace navigateToHistoryEntry {
    extern const char kName[];
    extern const char kParamEntryId[];
  } 

  namespace queryUsageAndQuota {
    extern const char kName[];
    extern const char kParamSecurityOrigin[];
    extern const char kResponseQuota[];
    extern const char kResponseUsage[];
  } 

  namespace reload {
    extern const char kName[];
    extern const char kParamIgnoreCache[];
    extern const char kParamScriptPreprocessor[];
    extern const char kParamScriptToEvaluateOnLoad[];
  } 

  namespace screencastFrame {
    extern const char kName[];
    extern const char kParamData[];
    extern const char kParamMetadata[];
  } 

  namespace screencastVisibilityChanged {
    extern const char kName[];
    extern const char kParamVisible[];
  } 

  namespace startScreencast {
    extern const char kName[];
    extern const char kParamFormat[];
    extern const char kParamMaxHeight[];
    extern const char kParamMaxWidth[];
    extern const char kParamQuality[];

    namespace Format {
      extern const char kEnumJpeg[];
      extern const char kEnumPng[];
    } 
  } 

  namespace stopScreencast {
    extern const char kName[];
  } 
} 

namespace SystemInfo {
  extern const char kName[];

  namespace GPUDevice {
    extern const char kParamDeviceId[];
    extern const char kParamDeviceString[];
    extern const char kParamVendorId[];
    extern const char kParamVendorString[];
  } 

  namespace GPUInfo {
    extern const char kParamAuxAttributes[];
    extern const char kParamDevices[];
    extern const char kParamFeatureStatus[];
  } 

  namespace SystemInfo {
    extern const char kParamGpu[];
    extern const char kParamModelName[];
  } 

  namespace getInfo {
    extern const char kName[];
    extern const char kResponseInfo[];
  } 
} 

namespace Tracing {
  extern const char kName[];

  namespace dataCollected {
    extern const char kName[];
    extern const char kParamValue[];
  } 

  namespace end {
    extern const char kName[];
  } 

  namespace start {
    extern const char kName[];
    extern const char kParamCategories[];
    extern const char kParamOptions[];
  } 

  namespace tracingComplete {
    extern const char kName[];
  } 
} 

namespace Worker {
  extern const char kName[];

  namespace disconnectFromWorker {
    extern const char kName[];
    extern const char kParamWorkerId[];
  } 

  namespace disconnectedFromWorker {
    extern const char kName[];
  } 
} 


}  
}  

#endif  
