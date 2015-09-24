// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <vector>

#include "base/basictypes.h"
#include "ipc/ipc_message_macros.h"
#include "ui/events/event_constants.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/metro_viewer/ime_types.h"

#define IPC_MESSAGE_START MetroViewerMsgStart

IPC_ENUM_TRAITS(ui::EventType)
IPC_ENUM_TRAITS(ui::EventFlags)


IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_SetTargetSurface,
                     gfx::NativeViewId )
IPC_MESSAGE_CONTROL3(MetroViewerHostMsg_MouseMoved,
                     int32,       
                     int32,       
                     int32        )
IPC_MESSAGE_CONTROL5(MetroViewerHostMsg_MouseButton,
                     int32,           
                     int32,           
                     int32,           
                     ui::EventType,   
                     ui::EventFlags   )
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_KeyDown,
                     uint32,       
                     uint32,       
                     uint32,       
                     uint32        );
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_KeyUp,
                     uint32,       
                     uint32,       
                     uint32,       
                     uint32        );
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_Character,
                     uint32,       
                     uint32,       
                     uint32,       
                     uint32        );
IPC_MESSAGE_CONTROL0(MetroViewerHostMsg_WindowActivated);

IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_TouchDown,
                     int32,           
                     int32,           
                     uint64,          
                     uint32)          
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_TouchUp,
                     int32,           
                     int32,           
                     uint64,          
                     uint32)          
IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_TouchMoved,
                     int32,           
                     int32,           
                     uint64,          
                     uint32)          

IPC_MESSAGE_CONTROL3(MetroViewerHostMsg_FileSaveAsDone,
                     bool,           
                     base::FilePath, 
                     int)            

IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_FileOpenDone,
                     bool,           
                     base::FilePath) 

IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_MultiFileOpenDone,
                     bool,                    
                     std::vector<base::FilePath>)   

IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_SelectFolderDone,
                     bool,           
                     base::FilePath) 


IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_ActivateDesktop,
                     base::FilePath ,
                     bool           );

IPC_MESSAGE_CONTROL0(MetroViewerHostMsg_MetroExit);

IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_OpenURLOnDesktop,
                     base::FilePath,  
                     string16         );

IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_SetCursor,
                     int64         );

IPC_STRUCT_BEGIN(MetroViewerHostMsg_SaveAsDialogParams)

  
  IPC_STRUCT_MEMBER(string16, title)

  
  IPC_STRUCT_MEMBER(base::FilePath, suggested_name)

  
  IPC_STRUCT_MEMBER(string16, filter)

  
  IPC_STRUCT_MEMBER(uint32, filter_index)

  
  IPC_STRUCT_MEMBER(string16, default_extension)

IPC_STRUCT_END()

IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_DisplayFileSaveAs,
                     MetroViewerHostMsg_SaveAsDialogParams)

IPC_MESSAGE_CONTROL4(MetroViewerHostMsg_DisplayFileOpen,
                     string16,       
                     string16,       
                     base::FilePath, 
                     bool)           

IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_DisplaySelectFolder,
                     string16)   

IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_SetCursorPos,
                     int,  
                     int)  

IPC_MESSAGE_CONTROL0(MetroViewerHostMsg_SetCursorPosAck)

IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_OpenURL,
                     string16)  

IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_SearchRequest,
                     string16)  

IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_WindowSizeChanged,
                     uint32,   
                     uint32)   

IPC_STRUCT_TRAITS_BEGIN(metro_viewer::UnderlineInfo)
  IPC_STRUCT_TRAITS_MEMBER(start_offset)
  IPC_STRUCT_TRAITS_MEMBER(end_offset)
  IPC_STRUCT_TRAITS_MEMBER(thick)
IPC_STRUCT_TRAITS_END()

IPC_MESSAGE_CONTROL4(
    MetroViewerHostMsg_ImeCompositionChanged,
    string16,                                  
    int32,                                     
    int32,                                     
    std::vector<metro_viewer::UnderlineInfo>)  

IPC_MESSAGE_CONTROL1(
    MetroViewerHostMsg_ImeCandidatePopupChanged,
    bool)                                      

IPC_MESSAGE_CONTROL1(MetroViewerHostMsg_ImeTextCommitted,
                     string16)                 

IPC_MESSAGE_CONTROL2(MetroViewerHostMsg_ImeInputSourceChanged,
                     uint16,                   
                     bool)                     

IPC_MESSAGE_CONTROL0(MetroViewerHostMsg_ImeCancelComposition)

IPC_STRUCT_TRAITS_BEGIN(metro_viewer::CharacterBounds)
  IPC_STRUCT_TRAITS_MEMBER(left)
  IPC_STRUCT_TRAITS_MEMBER(top)
  IPC_STRUCT_TRAITS_MEMBER(right)
  IPC_STRUCT_TRAITS_MEMBER(bottom)
IPC_STRUCT_TRAITS_END()

IPC_MESSAGE_CONTROL2(
    MetroViewerHostMsg_ImeTextInputClientUpdated,
    std::vector<int32>,                           
    std::vector<metro_viewer::CharacterBounds>)   
