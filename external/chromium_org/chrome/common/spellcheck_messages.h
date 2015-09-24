// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "chrome/common/spellcheck_marker.h"
#include "chrome/common/spellcheck_result.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_platform_file.h"


#define IPC_MESSAGE_START SpellCheckMsgStart

IPC_ENUM_TRAITS(SpellCheckResult::Decoration)

IPC_STRUCT_TRAITS_BEGIN(SpellCheckResult)
  IPC_STRUCT_TRAITS_MEMBER(decoration)
  IPC_STRUCT_TRAITS_MEMBER(location)
  IPC_STRUCT_TRAITS_MEMBER(length)
  IPC_STRUCT_TRAITS_MEMBER(replacement)
  IPC_STRUCT_TRAITS_MEMBER(hash)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(SpellCheckMarker)
  IPC_STRUCT_TRAITS_MEMBER(hash)
  IPC_STRUCT_TRAITS_MEMBER(offset)
IPC_STRUCT_TRAITS_END()


IPC_MESSAGE_CONTROL1(SpellCheckMsg_EnableSpellCheck,
                     bool)

IPC_MESSAGE_CONTROL4(SpellCheckMsg_Init,
                     IPC::PlatformFileForTransit ,
                     std::set<std::string> ,
                     std::string ,
                     bool )

IPC_MESSAGE_CONTROL2(SpellCheckMsg_CustomDictionaryChanged,
                     std::vector<std::string> ,
                     std::vector<std::string> )

IPC_MESSAGE_CONTROL1(SpellCheckMsg_EnableAutoSpellCorrect,
                     bool )

IPC_MESSAGE_CONTROL0(SpellCheckMsg_RequestDocumentMarkers)

IPC_MESSAGE_CONTROL1(SpellCheckHostMsg_RespondDocumentMarkers,
                     std::vector<uint32> )

#if !defined(OS_MACOSX)
IPC_MESSAGE_ROUTED4(SpellCheckMsg_RespondSpellingService,
                    int             ,
                    bool            ,
                    base::string16  ,
                    std::vector<SpellCheckResult>)
#endif

#if defined(OS_MACOSX)
IPC_MESSAGE_ROUTED0(SpellCheckMsg_AdvanceToNextMisspelling)

IPC_MESSAGE_ROUTED2(SpellCheckMsg_RespondTextCheck,
                    int        ,
                    std::vector<SpellCheckResult>)

IPC_MESSAGE_ROUTED1(SpellCheckMsg_ToggleSpellPanel,
                    bool)
#endif


IPC_MESSAGE_CONTROL0(SpellCheckHostMsg_RequestDictionary)

IPC_MESSAGE_ROUTED2(SpellCheckHostMsg_NotifyChecked,
                    base::string16 ,
                    bool )

#if !defined(OS_MACOSX)
IPC_MESSAGE_CONTROL4(SpellCheckHostMsg_CallSpellingService,
                     int ,
                     int ,
                     base::string16 ,
                     std::vector<SpellCheckMarker> )
#endif

#if defined(OS_MACOSX)
IPC_MESSAGE_ROUTED1(SpellCheckHostMsg_ShowSpellingPanel,
                    bool )

IPC_MESSAGE_ROUTED1(SpellCheckHostMsg_UpdateSpellingPanelWithMisspelledWord,
                    base::string16 )

IPC_SYNC_MESSAGE_CONTROL2_1(SpellCheckHostMsg_CheckSpelling,
                            base::string16 ,
                            int ,
                            bool )

IPC_SYNC_MESSAGE_CONTROL1_1(SpellCheckHostMsg_FillSuggestionList,
                            base::string16 ,
                            std::vector<base::string16> )

IPC_MESSAGE_CONTROL4(SpellCheckHostMsg_RequestTextCheck,
                     int ,
                     int ,
                     base::string16 ,
                     std::vector<SpellCheckMarker> )

IPC_MESSAGE_ROUTED2(SpellCheckHostMsg_ToggleSpellCheck,
                    bool ,
                    bool )
#endif  
