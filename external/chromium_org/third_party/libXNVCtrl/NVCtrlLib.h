/*
 * Copyright (c) 2008 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __NVCTRLLIB_H
#define __NVCTRLLIB_H

#include "NVCtrl.h"

#if defined __cplusplus
extern "C" {
#endif 


Bool XNVCTRLQueryExtension (
    Display *dpy,
    int *event_basep,
    int *error_basep
);



Bool XNVCTRLQueryVersion (
    Display *dpy,
    int *major,
    int *minor
);



Bool XNVCTRLIsNvScreen (
    Display *dpy,
    int screen
);



Bool XNVCTRLQueryTargetCount (
    Display *dpy,
    int target_type,
    int *value
);



void XNVCTRLSetAttribute (
    Display *dpy,
    int screen,
    unsigned int display_mask,
    unsigned int attribute,
    int value
);



void XNVCTRLSetTargetAttribute (
    Display *dpy,
    int target_type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    int value
);



Bool XNVCTRLSetAttributeAndGetStatus (
    Display *dpy,
    int screen,
    unsigned int display_mask,
    unsigned int attribute,
    int value
);



Bool XNVCTRLSetTargetAttributeAndGetStatus (
    Display *dpy,
    int target_type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    int value
);



Bool XNVCTRLQueryAttribute (
    Display *dpy,
    int screen,
    unsigned int display_mask,
    unsigned int attribute,
    int *value
);



Bool XNVCTRLQueryTargetAttribute (
    Display *dpy,
    int target_Type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    int *value
);



Bool XNVCTRLQueryTargetAttribute64 (
    Display *dpy,
    int target_Type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    int64_t *value
);



Bool XNVCTRLQueryStringAttribute (
    Display *dpy,
    int screen,
    unsigned int display_mask,
    unsigned int attribute,
    char **ptr
);



Bool XNVCTRLQueryTargetStringAttribute (
    Display *dpy,
    int target_type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    char **ptr
);


 
Bool XNVCTRLSetStringAttribute (
    Display *dpy,
    int screen,
    unsigned int display_mask,
    unsigned int attribute,
    char *ptr
);


 
Bool XNVCTRLSetTargetStringAttribute (
    Display *dpy,
    int target_type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    char *ptr
);



Bool XNVCTRLQueryValidAttributeValues (
    Display *dpy,
    int screen,
    unsigned int display_mask,
    unsigned int attribute,                                 
    NVCTRLAttributeValidValuesRec *values
);




Bool XNVCTRLQueryValidTargetAttributeValues (
    Display *dpy,
    int target_type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,                                 
    NVCTRLAttributeValidValuesRec *values
);



 Bool XNVCTRLQueryValidTargetStringAttributeValues (
    Display *dpy,
    int target_type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    NVCTRLAttributeValidValuesRec *values
);



Bool XNVCTRLQueryAttributePermissions (
    Display *dpy,
    unsigned int attribute,
    NVCTRLAttributePermissionsRec *permissions
);



 Bool XNVCTRLQueryStringAttributePermissions (
    Display *dpy,
    unsigned int attribute,
    NVCTRLAttributePermissionsRec *permissions
);



 Bool XNVCTRLQueryBinaryDataAttributePermissions (
    Display *dpy,
    unsigned int attribute,
    NVCTRLAttributePermissionsRec *permissions
);



 Bool XNVCTRLQueryStringOperationAttributePermissions (
    Display *dpy,
    unsigned int attribute,
    NVCTRLAttributePermissionsRec *permissions
);



void XNVCTRLSetGvoColorConversion (
    Display *dpy,
    int screen,
    float colorMatrix[3][3],
    float colorOffset[3],
    float colorScale[3]
);




Bool XNVCTRLQueryGvoColorConversion (
    Display *dpy,
    int screen,
    float colorMatrix[3][3],
    float colorOffset[3],
    float colorScale[3]
);



Bool XNVCTRLQueryBinaryData (
    Display *dpy,
    int screen,
    unsigned int display_mask,
    unsigned int attribute,
    unsigned char **ptr,
    int *len
);



Bool XNVCTRLQueryTargetBinaryData (
    Display *dpy,
    int target_type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    unsigned char **ptr,
    int *len
);



Bool XNVCTRLStringOperation (
    Display *dpy,
    int target_type,
    int target_id,
    unsigned int display_mask,
    unsigned int attribute,
    char *pIn,
    char **ppOut
);




Bool XNVCtrlSelectNotify (
    Display *dpy,
    int screen,
    int type,
    Bool onoff
);



Bool XNVCtrlSelectTargetNotify (
    Display *dpy,
    int target_type,
    int target_id,
    int notify_type,
    Bool onoff
);



typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;  
    Display *display;
    Time time;
    int screen;
    unsigned int display_mask;
    unsigned int attribute;
    int value;
} XNVCtrlAttributeChangedEvent;

typedef union {
    int type;
    XNVCtrlAttributeChangedEvent attribute_changed;
    long pad[24];
} XNVCtrlEvent;



typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;  
    Display *display;
    Time time;
    int target_type;
    int target_id;
    unsigned int display_mask;
    unsigned int attribute;
    int value;
} XNVCtrlAttributeChangedEventTarget;

typedef union {
    int type;
    XNVCtrlAttributeChangedEventTarget attribute_changed;
    long pad[24];
} XNVCtrlEventTarget;



typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;  
    Display *display;
    Time time;
    int target_type;
    int target_id;
    unsigned int display_mask;
    unsigned int attribute;
    int value;
    Bool availability;
} XNVCtrlAttributeChangedEventTargetAvailability;

typedef union {
    int type;
    XNVCtrlAttributeChangedEventTargetAvailability attribute_changed;
    long pad[24];
} XNVCtrlEventTargetAvailability;



typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;  
    Display *display;
    Time time;
    int target_type;
    int target_id;
    unsigned int display_mask;
    unsigned int attribute;
} XNVCtrlStringAttributeChangedEventTarget;

typedef union {
    int type;
    XNVCtrlStringAttributeChangedEventTarget attribute_changed;
    long pad[24];
} XNVCtrlStringEventTarget;




typedef struct {
    int type;
    unsigned long serial;
    Bool send_event;  
    Display *display;
    Time time;
    int target_type;
    int target_id;
    unsigned int display_mask;
    unsigned int attribute;
} XNVCtrlBinaryAttributeChangedEventTarget;

typedef union {
    int type;
    XNVCtrlBinaryAttributeChangedEventTarget attribute_changed;
    long pad[24];
} XNVCtrlBinaryEventTarget;

#if defined __cplusplus
} 
#endif 

#endif 
