// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "ipc/ipc_message_macros.h"
#include "ui/gfx/range/range.h"
#include "ui/gfx/rect.h"

#if defined(OS_MACOSX)
#include "content/common/mac/attributed_string_coder.h"
#endif

#define IPC_MESSAGE_START TextInputClientMsgStart
#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT


IPC_MESSAGE_ROUTED1(TextInputClientMsg_CharacterIndexForPoint,
                    gfx::Point)

IPC_MESSAGE_ROUTED1(TextInputClientMsg_FirstRectForCharacterRange,
                    gfx::Range)

IPC_MESSAGE_ROUTED1(TextInputClientMsg_StringForRange,
                    gfx::Range)



IPC_MESSAGE_ROUTED1(TextInputClientReplyMsg_GotCharacterIndexForPoint,
                    size_t )

IPC_MESSAGE_ROUTED1(TextInputClientReplyMsg_GotFirstRectForRange,
                    gfx::Rect )

#if defined(OS_MACOSX)
IPC_MESSAGE_ROUTED1(TextInputClientReplyMsg_GotStringForRange,
                    mac::AttributedStringCoder::EncodedString)
#endif  

