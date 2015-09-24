// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebTextCheckingResult.h"

#define IPC_MESSAGE_START SpellCheckMsgStart

IPC_ENUM_TRAITS(WebKit::WebTextCheckingResult::Error)

IPC_STRUCT_TRAITS_BEGIN(WebKit::WebTextCheckingResult)
  IPC_STRUCT_TRAITS_MEMBER(error)
  IPC_STRUCT_TRAITS_MEMBER(position)
  IPC_STRUCT_TRAITS_MEMBER(length)
IPC_STRUCT_TRAITS_END()



IPC_MESSAGE_ROUTED0(SpellCheckMsg_ToggleSpellCheck)

IPC_MESSAGE_ROUTED1(SpellCheckMsg_ToggleSpellPanel,
                    bool)

IPC_MESSAGE_ROUTED3(SpellCheckMsg_RespondTextCheck,
                    int        ,
                    int        ,
                    std::vector<WebKit::WebTextCheckingResult>)

IPC_MESSAGE_ROUTED0(SpellCheckMsg_AdvanceToNextMisspelling)

IPC_MESSAGE_CONTROL4(SpellCheckMsg_Init,
                     IPC::PlatformFileForTransit ,
                     std::vector<std::string> ,
                     std::string ,
                     bool )

IPC_MESSAGE_CONTROL1(SpellCheckMsg_WordAdded,
                     std::string )

IPC_MESSAGE_CONTROL1(SpellCheckMsg_EnableAutoSpellCorrect,
                     bool )



IPC_SYNC_MESSAGE_ROUTED0_1(SpellCheckHostMsg_GetDocumentTag,
                           int )

IPC_MESSAGE_ROUTED1(SpellCheckHostMsg_DocumentWithTagClosed,
                    int )

IPC_MESSAGE_ROUTED1(SpellCheckHostMsg_ShowSpellingPanel,
                    bool )

IPC_MESSAGE_ROUTED1(SpellCheckHostMsg_UpdateSpellingPanelWithMisspelledWord,
                    string16 )

IPC_MESSAGE_CONTROL0(SpellCheckHostMsg_RequestDictionary)

IPC_SYNC_MESSAGE_CONTROL2_1(SpellCheckHostMsg_PlatformCheckSpelling,
                            string16 ,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(SpellCheckHostMsg_PlatformFillSuggestionList,
                            string16 ,
                            std::vector<string16> )

IPC_MESSAGE_CONTROL4(SpellCheckHostMsg_PlatformRequestTextCheck,
                     int ,
                     int ,
                     int ,
                     string16 )
