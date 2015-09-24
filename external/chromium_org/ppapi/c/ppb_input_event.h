/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PPB_INPUT_EVENT_H_
#define PPAPI_C_PPB_INPUT_EVENT_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/pp_touch_point.h"
#include "ppapi/c/pp_var.h"

#define PPB_INPUT_EVENT_INTERFACE_1_0 "PPB_InputEvent;1.0"
#define PPB_INPUT_EVENT_INTERFACE PPB_INPUT_EVENT_INTERFACE_1_0

#define PPB_MOUSE_INPUT_EVENT_INTERFACE_1_0 "PPB_MouseInputEvent;1.0"
#define PPB_MOUSE_INPUT_EVENT_INTERFACE_1_1 "PPB_MouseInputEvent;1.1"
#define PPB_MOUSE_INPUT_EVENT_INTERFACE PPB_MOUSE_INPUT_EVENT_INTERFACE_1_1

#define PPB_WHEEL_INPUT_EVENT_INTERFACE_1_0 "PPB_WheelInputEvent;1.0"
#define PPB_WHEEL_INPUT_EVENT_INTERFACE PPB_WHEEL_INPUT_EVENT_INTERFACE_1_0

#define PPB_KEYBOARD_INPUT_EVENT_INTERFACE_1_0 "PPB_KeyboardInputEvent;1.0"
#define PPB_KEYBOARD_INPUT_EVENT_INTERFACE \
    PPB_KEYBOARD_INPUT_EVENT_INTERFACE_1_0

#define PPB_TOUCH_INPUT_EVENT_INTERFACE_1_0 "PPB_TouchInputEvent;1.0"
#define PPB_TOUCH_INPUT_EVENT_INTERFACE PPB_TOUCH_INPUT_EVENT_INTERFACE_1_0

#define PPB_IME_INPUT_EVENT_INTERFACE_1_0 "PPB_IMEInputEvent;1.0"
#define PPB_IME_INPUT_EVENT_INTERFACE PPB_IME_INPUT_EVENT_INTERFACE_1_0



typedef enum {
  PP_INPUTEVENT_TYPE_UNDEFINED = -1,
  PP_INPUTEVENT_TYPE_MOUSEDOWN = 0,
  PP_INPUTEVENT_TYPE_MOUSEUP = 1,
  PP_INPUTEVENT_TYPE_MOUSEMOVE = 2,
  PP_INPUTEVENT_TYPE_MOUSEENTER = 3,
  PP_INPUTEVENT_TYPE_MOUSELEAVE = 4,
  PP_INPUTEVENT_TYPE_WHEEL = 5,
  PP_INPUTEVENT_TYPE_RAWKEYDOWN = 6,
  PP_INPUTEVENT_TYPE_KEYDOWN = 7,
  PP_INPUTEVENT_TYPE_KEYUP = 8,
  PP_INPUTEVENT_TYPE_CHAR = 9,
  PP_INPUTEVENT_TYPE_CONTEXTMENU = 10,
  PP_INPUTEVENT_TYPE_IME_COMPOSITION_START = 11,
  PP_INPUTEVENT_TYPE_IME_COMPOSITION_UPDATE = 12,
  PP_INPUTEVENT_TYPE_IME_COMPOSITION_END = 13,
  PP_INPUTEVENT_TYPE_IME_TEXT = 14,
  PP_INPUTEVENT_TYPE_TOUCHSTART = 15,
  PP_INPUTEVENT_TYPE_TOUCHMOVE = 16,
  PP_INPUTEVENT_TYPE_TOUCHEND = 17,
  PP_INPUTEVENT_TYPE_TOUCHCANCEL = 18
} PP_InputEvent_Type;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_InputEvent_Type, 4);

typedef enum {
  PP_INPUTEVENT_MODIFIER_SHIFTKEY = 1 << 0,
  PP_INPUTEVENT_MODIFIER_CONTROLKEY = 1 << 1,
  PP_INPUTEVENT_MODIFIER_ALTKEY = 1 << 2,
  PP_INPUTEVENT_MODIFIER_METAKEY = 1 << 3,
  PP_INPUTEVENT_MODIFIER_ISKEYPAD = 1 << 4,
  PP_INPUTEVENT_MODIFIER_ISAUTOREPEAT = 1 << 5,
  PP_INPUTEVENT_MODIFIER_LEFTBUTTONDOWN = 1 << 6,
  PP_INPUTEVENT_MODIFIER_MIDDLEBUTTONDOWN = 1 << 7,
  PP_INPUTEVENT_MODIFIER_RIGHTBUTTONDOWN = 1 << 8,
  PP_INPUTEVENT_MODIFIER_CAPSLOCKKEY = 1 << 9,
  PP_INPUTEVENT_MODIFIER_NUMLOCKKEY = 1 << 10,
  PP_INPUTEVENT_MODIFIER_ISLEFT = 1 << 11,
  PP_INPUTEVENT_MODIFIER_ISRIGHT = 1 << 12
} PP_InputEvent_Modifier;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_InputEvent_Modifier, 4);

typedef enum {
  PP_INPUTEVENT_MOUSEBUTTON_NONE = -1,
  PP_INPUTEVENT_MOUSEBUTTON_LEFT = 0,
  PP_INPUTEVENT_MOUSEBUTTON_MIDDLE = 1,
  PP_INPUTEVENT_MOUSEBUTTON_RIGHT = 2
} PP_InputEvent_MouseButton;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_InputEvent_MouseButton, 4);

typedef enum {
  PP_INPUTEVENT_CLASS_MOUSE = 1 << 0,
  PP_INPUTEVENT_CLASS_KEYBOARD = 1 << 1,
  PP_INPUTEVENT_CLASS_WHEEL = 1 << 2,
  PP_INPUTEVENT_CLASS_TOUCH = 1 << 3,
  PP_INPUTEVENT_CLASS_IME = 1 << 4
} PP_InputEvent_Class;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_InputEvent_Class, 4);

struct PPB_InputEvent_1_0 {
  int32_t (*RequestInputEvents)(PP_Instance instance, uint32_t event_classes);
  int32_t (*RequestFilteringInputEvents)(PP_Instance instance,
                                         uint32_t event_classes);
  void (*ClearInputEventRequest)(PP_Instance instance, uint32_t event_classes);
  PP_Bool (*IsInputEvent)(PP_Resource resource);
  PP_InputEvent_Type (*GetType)(PP_Resource event);
  PP_TimeTicks (*GetTimeStamp)(PP_Resource event);
  uint32_t (*GetModifiers)(PP_Resource event);
};

typedef struct PPB_InputEvent_1_0 PPB_InputEvent;

struct PPB_MouseInputEvent_1_1 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_InputEvent_Type type,
                        PP_TimeTicks time_stamp,
                        uint32_t modifiers,
                        PP_InputEvent_MouseButton mouse_button,
                        const struct PP_Point* mouse_position,
                        int32_t click_count,
                        const struct PP_Point* mouse_movement);
  PP_Bool (*IsMouseInputEvent)(PP_Resource resource);
  PP_InputEvent_MouseButton (*GetButton)(PP_Resource mouse_event);
  struct PP_Point (*GetPosition)(PP_Resource mouse_event);
  int32_t (*GetClickCount)(PP_Resource mouse_event);
  struct PP_Point (*GetMovement)(PP_Resource mouse_event);
};

typedef struct PPB_MouseInputEvent_1_1 PPB_MouseInputEvent;

struct PPB_MouseInputEvent_1_0 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_InputEvent_Type type,
                        PP_TimeTicks time_stamp,
                        uint32_t modifiers,
                        PP_InputEvent_MouseButton mouse_button,
                        const struct PP_Point* mouse_position,
                        int32_t click_count);
  PP_Bool (*IsMouseInputEvent)(PP_Resource resource);
  PP_InputEvent_MouseButton (*GetButton)(PP_Resource mouse_event);
  struct PP_Point (*GetPosition)(PP_Resource mouse_event);
  int32_t (*GetClickCount)(PP_Resource mouse_event);
};

struct PPB_WheelInputEvent_1_0 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_TimeTicks time_stamp,
                        uint32_t modifiers,
                        const struct PP_FloatPoint* wheel_delta,
                        const struct PP_FloatPoint* wheel_ticks,
                        PP_Bool scroll_by_page);
  PP_Bool (*IsWheelInputEvent)(PP_Resource resource);
  struct PP_FloatPoint (*GetDelta)(PP_Resource wheel_event);
  struct PP_FloatPoint (*GetTicks)(PP_Resource wheel_event);
  PP_Bool (*GetScrollByPage)(PP_Resource wheel_event);
};

typedef struct PPB_WheelInputEvent_1_0 PPB_WheelInputEvent;

struct PPB_KeyboardInputEvent_1_0 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_InputEvent_Type type,
                        PP_TimeTicks time_stamp,
                        uint32_t modifiers,
                        uint32_t key_code,
                        struct PP_Var character_text);
  PP_Bool (*IsKeyboardInputEvent)(PP_Resource resource);
  uint32_t (*GetKeyCode)(PP_Resource key_event);
  struct PP_Var (*GetCharacterText)(PP_Resource character_event);
};

typedef struct PPB_KeyboardInputEvent_1_0 PPB_KeyboardInputEvent;

typedef enum {
  PP_TOUCHLIST_TYPE_TOUCHES = 0,
  PP_TOUCHLIST_TYPE_CHANGEDTOUCHES = 1,
  PP_TOUCHLIST_TYPE_TARGETTOUCHES = 2
} PP_TouchListType;
PP_COMPILE_ASSERT_SIZE_IN_BYTES(PP_TouchListType, 4);

struct PPB_TouchInputEvent_1_0 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_InputEvent_Type type,
                        PP_TimeTicks time_stamp,
                        uint32_t modifiers);
  void (*AddTouchPoint)(PP_Resource touch_event,
                        PP_TouchListType list,
                        const struct PP_TouchPoint* point);
  PP_Bool (*IsTouchInputEvent)(PP_Resource resource);
  uint32_t (*GetTouchCount)(PP_Resource resource, PP_TouchListType list);
  struct PP_TouchPoint (*GetTouchByIndex)(PP_Resource resource,
                                          PP_TouchListType list,
                                          uint32_t index);
  struct PP_TouchPoint (*GetTouchById)(PP_Resource resource,
                                       PP_TouchListType list,
                                       uint32_t touch_id);
};

typedef struct PPB_TouchInputEvent_1_0 PPB_TouchInputEvent;

struct PPB_IMEInputEvent_1_0 {
  PP_Resource (*Create)(PP_Instance instance,
                        PP_InputEvent_Type type,
                        PP_TimeTicks time_stamp,
                        struct PP_Var text,
                        uint32_t segment_number,
                        const uint32_t segment_offsets[],
                        int32_t target_segment,
                        uint32_t selection_start,
                        uint32_t selection_end);
  PP_Bool (*IsIMEInputEvent)(PP_Resource resource);
  struct PP_Var (*GetText)(PP_Resource ime_event);
  uint32_t (*GetSegmentNumber)(PP_Resource ime_event);
  uint32_t (*GetSegmentOffset)(PP_Resource ime_event, uint32_t index);
  int32_t (*GetTargetSegment)(PP_Resource ime_event);
  void (*GetSelection)(PP_Resource ime_event, uint32_t* start, uint32_t* end);
};

typedef struct PPB_IMEInputEvent_1_0 PPB_IMEInputEvent;

#endif  

