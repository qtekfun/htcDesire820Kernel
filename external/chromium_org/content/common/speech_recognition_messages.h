// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <string>

#include "content/public/common/speech_recognition_error.h"
#include "content/public/common/speech_recognition_grammar.h"
#include "content/public/common/speech_recognition_result.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "ui/gfx/rect.h"

#define IPC_MESSAGE_START SpeechRecognitionMsgStart

IPC_ENUM_TRAITS(content::SpeechAudioErrorDetails)
IPC_ENUM_TRAITS(content::SpeechRecognitionErrorCode)

IPC_STRUCT_TRAITS_BEGIN(content::SpeechRecognitionError)
  IPC_STRUCT_TRAITS_MEMBER(code)
  IPC_STRUCT_TRAITS_MEMBER(details)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::SpeechRecognitionHypothesis)
  IPC_STRUCT_TRAITS_MEMBER(utterance)
  IPC_STRUCT_TRAITS_MEMBER(confidence)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::SpeechRecognitionResult)
  IPC_STRUCT_TRAITS_MEMBER(is_provisional)
  IPC_STRUCT_TRAITS_MEMBER(hypotheses)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(content::SpeechRecognitionGrammar)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(weight)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_BEGIN(InputTagSpeechHostMsg_StartRecognition_Params)
  
  IPC_STRUCT_MEMBER(int, render_view_id)
  
  IPC_STRUCT_MEMBER(int, request_id)
  
  IPC_STRUCT_MEMBER(gfx::Rect, element_rect)
  
  IPC_STRUCT_MEMBER(std::string, language)
  
  IPC_STRUCT_MEMBER(std::string, grammar)
  
  IPC_STRUCT_MEMBER(std::string, origin_url)
IPC_STRUCT_END()


IPC_MESSAGE_CONTROL1(InputTagSpeechHostMsg_StartRecognition,
                     InputTagSpeechHostMsg_StartRecognition_Params)

IPC_MESSAGE_CONTROL2(InputTagSpeechHostMsg_CancelRecognition,
                     int ,
                     int )

IPC_MESSAGE_CONTROL2(InputTagSpeechHostMsg_StopRecording,
                     int ,
                     int )


IPC_MESSAGE_ROUTED2(InputTagSpeechMsg_SetRecognitionResults,
                    int ,
                    content::SpeechRecognitionResults )

IPC_MESSAGE_ROUTED1(InputTagSpeechMsg_RecordingComplete,
                    int )

IPC_MESSAGE_ROUTED1(InputTagSpeechMsg_RecognitionComplete,
                    int )

IPC_MESSAGE_ROUTED0(InputTagSpeechMsg_ToggleSpeechInput)




IPC_STRUCT_BEGIN(SpeechRecognitionHostMsg_StartRequest_Params)
  
  IPC_STRUCT_MEMBER(int, render_view_id)
  
  IPC_STRUCT_MEMBER(int, request_id)
  
  IPC_STRUCT_MEMBER(std::string, language)
  
  IPC_STRUCT_MEMBER(content::SpeechRecognitionGrammarArray, grammars)
  
  IPC_STRUCT_MEMBER(std::string, origin_url)
  
  IPC_STRUCT_MEMBER(uint32, max_hypotheses)
  
  IPC_STRUCT_MEMBER(bool, continuous)
  
  IPC_STRUCT_MEMBER(bool, interim_results)
IPC_STRUCT_END()


IPC_MESSAGE_CONTROL1(SpeechRecognitionHostMsg_StartRequest,
                     SpeechRecognitionHostMsg_StartRequest_Params)

IPC_MESSAGE_CONTROL2(SpeechRecognitionHostMsg_AbortRequest,
                     int ,
                     int )

IPC_MESSAGE_CONTROL2(SpeechRecognitionHostMsg_StopCaptureRequest,
                     int ,
                     int )


IPC_MESSAGE_ROUTED2(SpeechRecognitionMsg_ResultRetrieved,
                    int ,
                    content::SpeechRecognitionResults )

IPC_MESSAGE_ROUTED2(SpeechRecognitionMsg_ErrorOccurred,
                    int ,
                    content::SpeechRecognitionError )

IPC_MESSAGE_ROUTED1(SpeechRecognitionMsg_Started, int )

IPC_MESSAGE_ROUTED1(SpeechRecognitionMsg_AudioStarted, int )

IPC_MESSAGE_ROUTED1(SpeechRecognitionMsg_SoundStarted, int )

IPC_MESSAGE_ROUTED1(SpeechRecognitionMsg_SoundEnded, int )

IPC_MESSAGE_ROUTED1(SpeechRecognitionMsg_AudioEnded, int )

IPC_MESSAGE_ROUTED1(SpeechRecognitionMsg_Ended, int )
