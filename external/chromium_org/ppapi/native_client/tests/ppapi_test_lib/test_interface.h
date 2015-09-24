// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NATIVE_CLIENT_TESTS_PPAPI_TEST_PPB_TEMPLATE_TEST_INTERFACE_H
#define NATIVE_CLIENT_TESTS_PPAPI_TEST_PPB_TEMPLATE_TEST_INTERFACE_H

#include <stdio.h>
#include <limits>

#include <sstream>

#include "native_client/src/include/nacl_string.h"

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_image_data.h"

struct PP_Rect;
struct PP_Size;


void SetupTests();
void SetupPluginInterfaces();


typedef void (*TestFunction)();
void RegisterTest(nacl::string test_name, TestFunction test_function);

void RegisterPluginInterface(const char* interface_name,
                             const void* ppp_interface);

PP_CompletionCallback MakeTestableCompletionCallback(
    const char* callback_name,  
    PP_CompletionCallback_Func func,
    void* user_data);
PP_CompletionCallback MakeTestableCompletionCallback(
    const char* callback_name,  
    PP_CompletionCallback_Func func);

void PostTestMessage(nacl::string test_name, nacl::string message);

PP_Var PP_MakeString(const char* s);

nacl::string StringifyVar(const PP_Var& var);

#define EXPECT(expr) do { \
  if (!(expr)) { \
    char error[1024]; \
    snprintf(error, sizeof(error), \
             "ERROR at %s:%d: %s\n", __FILE__, __LINE__, #expr); \
    fprintf(stderr, "%s", error); \
    PostTestMessage(__FUNCTION__, error); \
  } \
} while (0)

#define EXPECT_VAR_INT(var, val) \
  EXPECT(var.type == PP_VARTYPE_INT32 && var.value.as_int == val)

#define EXPECT_VAR_STRING(var, val) \
  do { \
    EXPECT(var.type == PP_VARTYPE_STRING); \
    uint32_t dummy_size; \
    const char* expected = PPBVar()->VarToUtf8(var, &dummy_size); \
    EXPECT(0 == strcmp(expected, val)); \
  } while (0)

#define EXPECT_VAR_BOOL(var, val) \
  EXPECT(var.type == PP_VARTYPE_BOOL && var.value.as_bool == val)

#define TEST_PASSED PostTestMessage(__FUNCTION__, "PASSED");
#define TEST_FAILED EXPECT(false)

template<typename T> nacl::string toString(T v) {
  std::stringstream s;
  s << v;
  return s.str();
}

#define LOG_TO_BROWSER(message) PostTestMessage("@", message)

#define CRASH *(volatile int *) 0 = 0;

const int kManyResources = 1000;


const PP_Instance kInvalidInstance = 0;
const PP_Module kInvalidModule = 0;
const PP_Resource kInvalidResource = 0;

const PP_Instance kNotAnInstance = 0xFFFFF0;
const PP_Resource kNotAResource = 0xAAAAA0;

const PP_Point kOrigin = PP_MakePoint(0, 0);

PP_Instance pp_instance();
PP_Module pp_module();

PP_Bool DidCreateDefault(PP_Instance instance,
                         uint32_t argc, const char* argn[], const char* argv[]);
void DidDestroyDefault(PP_Instance instance);
void DidChangeViewDefault(PP_Instance instance, PP_Resource view);
void DidChangeFocusDefault(PP_Instance instance, PP_Bool has_focus);
PP_Bool HandleDocumentLoadDefault(PP_Instance instance, PP_Resource url_loader);


bool IsSizeInRange(PP_Size size, PP_Size min_size, PP_Size max_size);
bool IsSizeEqual(PP_Size size, PP_Size expected);
bool IsRectEqual(PP_Rect position, PP_Rect expected);


struct ColorPremul { uint32_t A, R, G, B; };  
const ColorPremul kSheerRed = { 0x88, 0x88, 0x00, 0x00 };
const ColorPremul kSheerBlue = { 0x88, 0x00, 0x00, 0x88 };
const ColorPremul kSheerGray = { 0x77, 0x55, 0x55, 0x55 };
const ColorPremul kOpaqueGreen = { 0xFF, 0x00, 0xFF, 0x00 };
const ColorPremul kOpaqueBlack = { 0xFF, 0x00, 0x00, 0x00 };
const ColorPremul kOpaqueWhite = { 0xFF, 0xFF, 0xFF, 0xFF };
const ColorPremul kOpaqueYellow = { 0xFF, 0xFF, 0xFF, 0x00 };
const int kBytesPerPixel = sizeof(uint32_t);  

uint32_t FormatColor(PP_ImageDataFormat format, ColorPremul color);

PP_Resource CreateImageData(PP_Size size, ColorPremul pixel_color, void** bmp);


bool IsImageRectOnScreen(PP_Resource graphics2d,
                         PP_Point origin,
                         PP_Size size,
                         ColorPremul color);

#endif  
