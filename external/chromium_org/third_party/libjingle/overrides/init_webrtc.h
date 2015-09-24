// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_LIBJINGLE_OVERRIDES_INIT_WEBRTC_H_
#define THIRD_PARTY_LIBJINGLE_OVERRIDES_INIT_WEBRTC_H_

#include "allocator_shim/allocator_stub.h"
#include "base/logging.h"
#include "third_party/webrtc/system_wrappers/interface/event_tracer.h"

class CommandLine;

namespace cricket {
class MediaEngineInterface;
class WebRtcVideoDecoderFactory;
class WebRtcVideoEncoderFactory;
}  

namespace webrtc {
class AudioDeviceModule;
}  

typedef cricket::MediaEngineInterface* (*CreateWebRtcMediaEngineFunction)(
    webrtc::AudioDeviceModule* adm,
    webrtc::AudioDeviceModule* adm_sc,
    cricket::WebRtcVideoEncoderFactory* encoder_factory,
    cricket::WebRtcVideoDecoderFactory* decoder_factory);

typedef void (*DestroyWebRtcMediaEngineFunction)(
    cricket::MediaEngineInterface* media_engine);

typedef void (*InitDiagnosticLoggingDelegateFunctionFunction)(
    void (*DelegateFunction)(const std::string&));

typedef bool (*InitializeModuleFunction)(
    const CommandLine& command_line,
#if !defined(OS_MACOSX) && !defined(OS_ANDROID)
    AllocateFunction alloc,
    DellocateFunction dealloc,
#endif
    logging::LogMessageHandlerFunction log_handler,
    webrtc::GetCategoryEnabledPtr trace_get_category_enabled,
    webrtc::AddTraceEventPtr trace_add_trace_event,
    CreateWebRtcMediaEngineFunction* create_media_engine,
    DestroyWebRtcMediaEngineFunction* destroy_media_engine,
    InitDiagnosticLoggingDelegateFunctionFunction* init_diagnostic_logging);

#if !defined(LIBPEERCONNECTION_IMPLEMENTATION)
bool InitializeWebRtcModule();
#endif

#endif 
