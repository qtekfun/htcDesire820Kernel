/* Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef _NP_EXTENSIONS_H_
#define _NP_EXTENSIONS_H_

#include "npapi.h"

#include <stddef.h>         

#define NPNVPepperExtensions ((NPNVariable) 4000)

#define NPPVPepperExtensions ((NPPVariable) 4001)

typedef void NPDeviceConfig;
typedef void NPDeviceContext;
typedef void NPUserData;

typedef int32_t NPDeviceID;


typedef enum {
  NPMouseButton_None    = -1,
  NPMouseButton_Left    = 0,
  NPMouseButton_Middle  = 1,
  NPMouseButton_Right   = 2
} NPMouseButtons;

typedef enum {
  NPEventType_Undefined   = -1,
  NPEventType_MouseDown   = 0,
  NPEventType_MouseUp     = 1,
  NPEventType_MouseMove   = 2,
  NPEventType_MouseEnter  = 3,
  NPEventType_MouseLeave  = 4,
  NPEventType_MouseWheel  = 5,
  NPEventType_RawKeyDown  = 6,
  NPEventType_KeyDown     = 7,
  NPEventType_KeyUp       = 8,
  NPEventType_Char        = 9,
  NPEventType_Minimize    = 10,
  NPEventType_Focus       = 11,
  NPEventType_Device      = 12
} NPEventTypes;

typedef enum {
  NPEventModifier_ShiftKey         = 1 << 0,
  NPEventModifier_ControlKey       = 1 << 1,
  NPEventModifier_AltKey           = 1 << 2,
  NPEventModifier_MetaKey          = 1 << 3,
  NPEventModifier_IsKeyPad         = 1 << 4,
  NPEventModifier_IsAutoRepeat     = 1 << 5,
  NPEventModifier_LeftButtonDown   = 1 << 6,
  NPEventModifier_MiddleButtonDown = 1 << 7,
  NPEventModifier_RightButtonDown  = 1 << 8
} NPEventModifiers;

typedef struct _NPKeyEvent
{
  uint32_t modifier;
  uint32_t normalizedKeyCode;
} NPKeyEvent;

typedef struct _NPCharacterEvent
{
  uint32_t modifier;
  uint16_t text[4];
  uint16_t unmodifiedText[4];
} NPCharacterEvent;

typedef struct _NPMouseEvent
{
  uint32_t modifier;
  int32_t button;
  int32_t x;
  int32_t y;
  int32_t clickCount;
} NPMouseEvent;

typedef struct _NPMouseWheelEvent
{
  uint32_t modifier;
  float deltaX;
  float deltaY;
  float wheelTicksX;
  float wheelTicksY;
  uint32_t scrollByPage;
} NPMouseWheelEvent;

typedef struct _NPDeviceEvent {
  uint32_t device_uid;
  uint32_t subtype;
  
} NPDeviceEvent;

typedef struct _NPMinimizeEvent {
  int32_t value;
} NPMinimizeEvent;

typedef struct _NPFocusEvent {
  int32_t value;
} NPFocusEvent;

typedef struct _NPPepperEvent
{
  uint32_t size;
  int32_t type;
  double timeStampSeconds;
  union {
    NPKeyEvent key;
    NPCharacterEvent character;
    NPMouseEvent mouse;
    NPMouseWheelEvent wheel;
    NPMinimizeEvent minimize;
    NPFocusEvent focus;
    NPDeviceEvent device;
  } u;
} NPPepperEvent;


#define NPPepper2DDevice 1

typedef struct _NPDeviceContext2DConfig {
} NPDeviceContext2DConfig;

typedef struct _NPDeviceContext2D
{
  
  void* reserved;

  void* region;

  int32_t stride;

  struct {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
  } dirty;
} NPDeviceContext2D;

typedef struct _NPDeviceBuffer {
  void* ptr;
  size_t size;
} NPDeviceBuffer;

typedef void (*NPDeviceFlushContextCallbackPtr)(
    NPP instance,
    NPDeviceContext* context,
    NPError err,
    NPUserData* userData);

typedef NPError (
    *NPDeviceQueryCapabilityPtr)(NPP instance,
    int32_t capability,
    int32_t *value);
typedef NPError (
    *NPDeviceQueryConfigPtr)(NPP instance,
    const NPDeviceConfig* request,
    NPDeviceConfig* obtain);
typedef NPError (*NPDeviceInitializeContextPtr)(
    NPP instance,
    const NPDeviceConfig* config,
    NPDeviceContext* context);
typedef NPError (*NPDeviceGetStateContextPtr) (
    NPP instance,
    NPDeviceContext* context,
    int32_t state,
    intptr_t* value);
typedef NPError (*NPDeviceSetStateContextPtr) (
    NPP instance,
    NPDeviceContext* context,
    int32_t state,
    intptr_t value);
typedef NPError (*NPDeviceFlushContextPtr)(
    NPP instance,
    NPDeviceContext* context,
    NPDeviceFlushContextCallbackPtr callback,
    void* userData);
typedef NPError (*NPDeviceDestroyContextPtr)(
    NPP instance,
    NPDeviceContext* context);
typedef NPError (*NPDeviceCreateBufferPtr)(
    NPP instance,
    NPDeviceContext* context,
    size_t size,
    int32_t* id);
typedef NPError (*NPDeviceDestroyBufferPtr)(
    NPP instance,
    NPDeviceContext* context,
    int32_t id);
typedef NPError (*NPDeviceMapBufferPtr)(
    NPP instance,
    NPDeviceContext* context,
    int32_t id,
    NPDeviceBuffer* buffer);


typedef struct NPDevice NPDevice;
typedef struct NPNExtensions NPNExtensions;

typedef struct NPNExtensions NPExtensions;



typedef enum {
  
  
  NPDeviceSynchronizationMode_Cached,

  
  
  NPDeviceSynchronizationMode_Immediate,

  
  
  NPDeviceSynchronizationMode_Flush
} NPDeviceSynchronizationMode;

typedef NPError (*NPDeviceGetNumConfigsPtr)(NPP instance,
                                            int32_t* numConfigs);

typedef NPError (*NPDeviceGetConfigAttribsPtr)(NPP instance,
                                               int32_t config,
                                               int32_t* attribList);

typedef NPError (*NPDeviceCreateContextPtr)(NPP instance,
                                            int32_t config,
                                            const int32_t* attribList,
                                            NPDeviceContext** context);


typedef void (*NPDeviceGenericCallbackPtr)(void);

typedef NPError (*NPDeviceRegisterCallbackPtr)(
    NPP instance,
    NPDeviceContext* context,
    int32_t callbackType,
    NPDeviceGenericCallbackPtr callback,
    void* callbackData);

typedef void (*NPDeviceSynchronizeContextCallbackPtr)(
    NPP instance,
    NPDeviceContext* context,
    NPError error,
    void* data);

typedef NPError (*NPDeviceSynchronizeContextPtr)(
    NPP instance,
    NPDeviceContext* context,
    NPDeviceSynchronizationMode mode,
    const int32_t* inputAttribList,
    int32_t* outputAttribList,
    NPDeviceSynchronizeContextCallbackPtr callback,
    void* callbackData);

enum {
  
  NPAttrib_End   = 0,

  
  
  NPAttrib_Error = 0x80000000
};

struct NPDevice {
  NPDeviceQueryCapabilityPtr queryCapability;
  NPDeviceQueryConfigPtr queryConfig;
  NPDeviceInitializeContextPtr initializeContext;
  NPDeviceSetStateContextPtr setStateContext;
  NPDeviceGetStateContextPtr getStateContext;
  NPDeviceFlushContextPtr flushContext;
  NPDeviceDestroyContextPtr destroyContext;
  NPDeviceCreateBufferPtr createBuffer;
  NPDeviceDestroyBufferPtr destroyBuffer;
  NPDeviceMapBufferPtr mapBuffer;

  
  NPDeviceGetNumConfigsPtr getNumConfigs;
  NPDeviceGetConfigAttribsPtr getConfigAttribs;
  NPDeviceCreateContextPtr createContext;
  NPDeviceRegisterCallbackPtr registerCallback;
  NPDeviceSynchronizeContextPtr synchronizeContext;
};

typedef NPDevice* (*NPAcquireDevicePtr)(
    NPP instance,
    NPDeviceID device);

typedef void (*NPNumberOfFindResultsChangedPtr)(
    NPP instance,
    int total,
    bool finalResult);

 
typedef void (*NPSelectedFindResultChangedPtr)(
    NPP instance,
    int index);

typedef int32_t NPWidgetID;

typedef enum {
  NPWidgetTypeScrollbar = 0
} NPWidgetType;

typedef struct _NPScrollbarCreateParams {
  bool vertical;
} NPScrollbarCreateParams;

typedef struct _NPRect32
{
  uint32_t top;
  uint32_t left;
  uint32_t bottom;
  uint32_t right;
} NPRect32;

typedef struct _NPScrollbarTickMarks {
  uint32_t count;
  NPRect32* tickmarks;
} NPScrollbarTickMarks;

typedef enum {
  NPWidgetPropertyLocation = 0,  
  NPWidgetPropertyDirtyRect = 1,  
  NPWidgetPropertyScrollbarThickness = 2,  
  NPWidgetPropertyScrollbarValue = 3,  
  NPWidgetPropertyScrollbarDocumentSize = 4,  
  
  NPWidgetPropertyScrollbarTickMarks = 5,
  
  NPWidgetPropertyScrollbarScrollByLine = 6,
  
  NPWidgetPropertyScrollbarScrollByPage = 7,
  
  NPWidgetPropertyScrollbarScrollByDocument = 8,
  
  NPWidgetPropertyScrollbarScrollByPixels = 9
} NPWidgetProperty;

typedef NPError (*NPCreateWidgetPtr) (
    NPP instance,
    NPWidgetType type,
    void* params,  
    NPWidgetID* id);

typedef NPError (*NPDestroyWidgetPtr) (
    NPP instance,
    NPWidgetID id);

typedef NPError (*NPPaintWidgetPtr) (
    NPP instance,
    NPWidgetID id,
    NPDeviceContext2D* context,
    NPRect* dirty);

typedef bool (*NPHandleWidgetEventPtr) (
    NPP instance,
    NPWidgetID id,
    NPPepperEvent* event);

typedef NPError (*NPGetWidgetPropertyPtr) (
    NPP instance,
    NPWidgetID id,
    NPWidgetProperty property,
    void* value);

typedef NPError (*NPSetWidgetPropertyPtr) (
    NPP instance,
    NPWidgetID id,
    NPWidgetProperty property,
    void* value);

typedef struct _NPWidgetExtensions {
  NPCreateWidgetPtr createWidget;
  NPDestroyWidgetPtr destroyWidget;
  NPPaintWidgetPtr paintWidget;
  NPHandleWidgetEventPtr handleWidgetEvent;
  NPGetWidgetPropertyPtr getWidgetProperty;
  NPSetWidgetPropertyPtr setWidgetProperty;
} NPWidgetExtensions;

typedef NPWidgetExtensions* (*NPGetWidgetExtensionsPtr)(
    NPP instance);


typedef enum {
  NPChooseFile_Open = 1,
  NPChooseFile_OpenMultiple = 2,
  NPChooseFile_Save = 3
} NPChooseFileMode;
typedef void (*NPChooseFileCallback)(const char** filePaths,
                                     uint32_t pathCount,
                                     void* userData);
typedef NPError (*NPChooseFilePtr)(
    NPP instance,
    const char* mimeTypes,
    NPChooseFileMode mode,
    NPChooseFileCallback callback,
    void* userData);

typedef enum {
  NPCursorTypePointer = 0,
  NPCursorTypeCross = 1,
  NPCursorTypeHand = 2,
  NPCursorTypeIBeam = 3,
  NPCursorTypeWait = 4,
  NPCursorTypeHelp = 5,
  NPCursorTypeEastResize = 6,
  NPCursorTypeNorthResize = 7,
  NPCursorTypeNorthEastResize = 8,
  NPCursorTypeNorthWestResize = 9,
  NPCursorTypeSouthResize = 10,
  NPCursorTypeSouthEastResize = 11,
  NPCursorTypeSouthWestResize = 12,
  NPCursorTypeWestResize = 13,
  NPCursorTypeNorthSouthResize = 14,
  NPCursorTypeEastWestResize = 15,
  NPCursorTypeNorthEastSouthWestResize = 16,
  NPCursorTypeNorthWestSouthEastResize = 17,
  NPCursorTypeColumnResize = 18,
  NPCursorTypeRowResize = 19,
  NPCursorTypeMiddlePanning = 20,
  NPCursorTypeEastPanning = 21,
  NPCursorTypeNorthPanning = 22,
  NPCursorTypeNorthEastPanning = 23,
  NPCursorTypeNorthWestPanning = 24,
  NPCursorTypeSouthPanning = 25,
  NPCursorTypeSouthEastPanning = 26,
  NPCursorTypeSouthWestPanning = 27,
  NPCursorTypeWestPanning = 28,
  NPCursorTypeMove = 29,
  NPCursorTypeVerticalText = 30,
  NPCursorTypeCell = 31,
  NPCursorTypeContextMenu = 32,
  NPCursorTypeAlias = 33,
  NPCursorTypeProgress = 34,
  NPCursorTypeNoDrop = 35,
  NPCursorTypeCopy = 36,
  NPCursorTypeNone = 37,
  NPCursorTypeNotAllowed = 38,
  NPCursorTypeZoomIn = 39,
  NPCursorTypeZoomOut = 40
} NPCursorType;

typedef NPError (*NPSetCursorPtr)(
    NPP instance,
    NPCursorType type);

typedef int NPFontID;

typedef enum {
  NPCharsetAnsi = 0,
  NPCharsetDefault = 1,
  NPCharsetSymbol = 2,
  NPCharsetMac = 77,
  NPCharsetShiftJIS = 128,
  NPCharsetHangul = 129,
  NPCharsetJohab = 130,
  NPCharsetGB2312 =134,
  NPCharsetChineseBIG5 = 136,
  NPCharsetGreek = 161,
  NPCharsetTurkish = 162,
  NPCharsetVietnamese = 163,
  NPCharsetHebrew = 177,
  NPCharsetArabic = 178,
  NPCharsetBaltic = 186,
  NPCharsetRussian = 204,
  NPCharsetThai = 222,
  NPCharsetEastEurope = 238,
  NPCharsetOEM = 255
} NPCharset;

typedef enum {
  NPPitchDefault,
  NPPitchFixed
} NPPitch;

typedef enum {
  NPFamilyDefault,
  NPFamilyRoman,
  NPFamilyScript
} NPFamily;

typedef struct _NPFontDescription {
  const char* face;
  int weight;
  bool italic;
  NPPitch pitch;
  NPFamily family;
  NPCharset charset;
} NPFontDescription;

typedef NPError (*NPMatchFontWithFallbackPtr) (
    NPP instance,
    const NPFontDescription* description,
    NPFontID* id);

typedef NPError (*GetFontTablePtr) (
    NPP instance,
    NPFontID id,
    uint32_t table,
    void* output,
    size_t* output_length);

typedef NPError (*NPDestroyFontPtr) (
    NPP instance,
    NPFontID id);

typedef struct _NPFontExtensions {
  NPMatchFontWithFallbackPtr matchFontWithFallback;
  GetFontTablePtr getFontTable;
  NPDestroyFontPtr destroyFont;
} NPFontExtensions;

typedef NPFontExtensions* (*NPGetFontExtensionsPtr)(
    NPP instance);

struct NPNExtensions {
  
  NPAcquireDevicePtr acquireDevice;
  
  NPNumberOfFindResultsChangedPtr numberOfFindResultsChanged;
  NPSelectedFindResultChangedPtr selectedFindResultChanged;
  
  NPChooseFilePtr chooseFile;
  
  NPGetWidgetExtensionsPtr getWidgetExtensions;
  
  NPSetCursorPtr setCursor;
  
  NPGetFontExtensionsPtr getFontExtensions;
};


#define NPPepper3DDevice 2

typedef struct _NPDeviceContext3DConfig {
  int32_t commandBufferSize;
} NPDeviceContext3DConfig;

typedef enum _NPDeviceContext3DError {
  
  NPDeviceContext3DError_NoError,

  
  NPDeviceContext3DError_InvalidSize,

  
  NPDeviceContext3DError_OutOfBounds,

  
  NPDeviceContext3DError_UnknownCommand,

  
  NPDeviceContext3DError_InvalidArguments,

  
  
  NPDeviceContext3DError_LostContext,

  
  NPDeviceContext3DError_GenericError
} NPDeviceContext3DError;

typedef struct _NPDeviceContext3D NPDeviceContext3D;

typedef void (*NPDeviceContext3DRepaintPtr)(NPP npp,
                                            NPDeviceContext3D* context);

typedef struct _NPDeviceContext3D
{
  void* reserved;

  
  
  
  
  
  
  
  
  bool waitForProgress;

  
  void* commandBuffer;
  int32_t commandBufferSize;

  
  int32_t getOffset;

  
  int32_t putOffset;

  
  int32_t token;

  
  
  
  NPDeviceContext3DRepaintPtr repaintCallback;

  
  NPDeviceContext3DError error;
} NPDeviceContext3D;



enum {
  NP3DCommandBufferId = 0
};

enum {
  
  
  NP3DAttrib_BufferSize        = 0x3020,
  NP3DAttrib_AlphaSize         = 0x3021,
  NP3DAttrib_BlueSize          = 0x3022,
  NP3DAttrib_GreenSize         = 0x3023,
  NP3DAttrib_RedSize           = 0x3024,
  NP3DAttrib_DepthSize         = 0x3025,
  NP3DAttrib_StencilSize       = 0x3026,
  NP3DAttrib_SurfaceType       = 0x3033,

  
  
  NP3DAttrib_SwapBehavior       = 0x3093,
  NP3DAttrib_MultisampleResolve = 0x3099,

  
  
  
  NP3DAttrib_CommandBufferSize  = 0x10000000,

  

  
  NP3DAttrib_PutOffset,

  
  NP3DAttrib_GetOffset,

  
  NP3DAttrib_Token
};

enum {
  
  
  
  NP3DCallback_Repaint = 1
};

enum {
  NP3DSurfaceType_MultisampleResolveBox = 0x0200,
  NP3DSurfaceType_SwapBehaviorPreserved = 0x0400
};

enum {
  NP3DSwapBehavior_Preserved            = 0x3094,
  NP3DSwapBehavior_Destroyed            = 0x3095
};

enum {
  NP3DMultisampleResolve_Default        = 0x309A,
  NP3DMultisampleResolve_Box            = 0x309B
};



#define NPPepperAudioDevice 3

typedef enum {
  NPAudioMinSampleFrameCount = 64,
  NPAudioMaxSampleFrameCount = 32768
} NPAudioSampleFrameCounts;

typedef enum {
  NPAudioSampleRate44100Hz = 44100,
  NPAudioSampleRate48000Hz = 48000,
  NPAudioSampleRate96000Hz = 96000
} NPAudioSampleRates;

typedef enum {
  NPAudioSampleTypeInt16   = 0,
  NPAudioSampleTypeFloat32 = 1
} NPAudioSampleTypes;

typedef enum {
  NPAudioChannelNone     = 0,
  NPAudioChannelMono     = 1,
  NPAudioChannelStereo   = 2,
  NPAudioChannelThree    = 3,
  NPAudioChannelFour     = 4,
  NPAudioChannelFive     = 5,
  NPAudioChannelFiveOne  = 6,
  NPAudioChannelSeven    = 7,
  NPAudioChannelSevenOne = 8
} NPAudioChannels;

typedef enum {
  NPAudioContextStateCallback = 0,
  NPAudioContextStateUnderrunCounter = 1
} NPAudioContextStates;

typedef enum {
  NPAudioCallbackStop = 0,
  NPAudioCallbackStart = 1
} NPAudioContextStateValues;

typedef enum {
  NPAudioCapabilitySampleRate              = 0,
  NPAudioCapabilitySampleType              = 1,
  NPAudioCapabilitySampleFrameCount        = 2,
  NPAudioCapabilitySampleFrameCount44100Hz = 3,
  NPAudioCapabilitySampleFrameCount48000Hz = 4,
  NPAudioCapabilitySampleFrameCount96000Hz = 5,
  NPAudioCapabilityOutputChannelMap        = 6,
  NPAudioCapabilityInputChannelMap         = 7
} NPAudioCapabilities;

typedef struct _NPDeviceContextAudio NPDeviceContextAudio;

typedef void (*NPAudioCallback)(NPDeviceContextAudio *context);

typedef struct _NPDeviceContextAudioConfig {
  int32_t sampleRate;
  int32_t sampleType;
  int32_t outputChannelMap;
  int32_t inputChannelMap;
  int32_t sampleFrameCount;
  uint32_t startThread;
  uint32_t flags;
  NPAudioCallback callback;
  void *userData;
} NPDeviceContextAudioConfig;

struct _NPDeviceContextAudio {
  NPDeviceContextAudioConfig config;
  void *outBuffer;
  void *inBuffer;
  void *reserved;
};


typedef struct _NPPrintPageNumberRange {
  int32_t firstPageNumber;
  int32_t lastPageNumber;
} NPPrintPageNumberRange;

typedef NPError (*NPPPrintBeginPtr) (
    NPP instance,
    NPRect* printableArea,
    int32_t printerDPI,
    int32_t* numPages);
typedef NPError (*NPPGetRasterDimensionsPtr) (
    NPP instance,
    int32_t pageNumber,
    int32_t* widthInPixels,
    int32_t* heightInPixels);
typedef NPError (*NPPPrintPageRasterPtr) (
    NPP instance,
    int32_t pageNumber,
    NPDeviceContext2D* printSurface);
typedef NPError (*NPPPrintEndPtr) (NPP instance);
typedef NPError (*NPPrintPagesAsPDFPtr)(NPP instance,
                                        NPPrintPageNumberRange* page_ranges,
                                        int32_t page_range_count,
                                        unsigned char** pdf_output,
                                        int32_t* output_size);



typedef struct _NPPPrintExtensions {
  NPPPrintBeginPtr printBegin;
  NPPGetRasterDimensionsPtr getRasterDimensions;
  NPPPrintPageRasterPtr printPageRaster;
  NPPPrintEndPtr printEnd;
  NPPrintPagesAsPDFPtr printPagesAsPDF;
} NPPPrintExtensions;

typedef NPPPrintExtensions* (*NPPGetPrintExtensionsPtr)(NPP instance);


typedef NPError (*NPPStartFindPtr) (
    NPP instance,
    const char* text,
    bool caseSensitive);

typedef NPError (*NPPSelectFindResultPtr) (
    NPP instance,
    bool forward);

typedef NPError (*NPPStopFindPtr) (
    NPP instance);

typedef struct _NPPFindExtensions {
  NPPStartFindPtr startFind;
  NPPSelectFindResultPtr selectFindResult;
  NPPStopFindPtr stopFind;
} NPPFindExtensions;

typedef NPPFindExtensions* (*NPPGetFindExtensionsPtr)(NPP instance);

typedef NPError (*NPPZoomPtr) (
    NPP instance,
    float factor,
    bool text_only);

typedef NPError (*NPPWidgetPropertyChangedPtr) (
    NPP instance,
    NPWidgetID id,
    NPWidgetProperty property);

typedef enum {
  NPSelectionTypeAny       = 0,
  NPSelectionTypePlainText = 1,
  NPSelectionTypeHTML      = 2
} NPSelectionType;

typedef NPError (*NPPGetSelectionPtr) (
    NPP instance,
    NPSelectionType* type,
    void** data);

typedef struct _NPPExtensions {
  NPPGetPrintExtensionsPtr getPrintExtensions;
  NPPGetFindExtensionsPtr getFindExtensions;
  NPPZoomPtr zoom;
  NPPWidgetPropertyChangedPtr widgetPropertyChanged;
  NPPGetSelectionPtr getSelection;
} NPPExtensions;

#endif  
