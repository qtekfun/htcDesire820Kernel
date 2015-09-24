// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_INSTANCE_H_
#define PPAPI_CPP_INSTANCE_H_


#include <map>
#include <string>

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_console.h"
#include "ppapi/cpp/instance_handle.h"
#include "ppapi/cpp/view.h"

#ifdef PostMessage
#undef PostMessage
#endif

struct PP_InputEvent;

namespace pp {

class Graphics2D;
class Graphics3D;
class InputEvent;
class InstanceHandle;
class Rect;
class URLLoader;
class Var;

class Instance {
 public:
  
  
  
  
  
  
  
  
  explicit Instance(PP_Instance instance);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual ~Instance();

  
  
  
  
  PP_Instance pp_instance() const { return pp_instance_; }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

  
  

  
  
  
  
  
  
  
  
  
  virtual void DidChangeView(const View& view);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void DidChangeView(const Rect& position, const Rect& clip);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void DidChangeFocus(bool has_focus);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool HandleInputEvent(const pp::InputEvent& event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool HandleDocumentLoad(const URLLoader& url_loader);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void HandleMessage(const Var& message);

  

  
  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  bool BindGraphics(const Graphics2D& graphics);

  
  
  
  
  
  
  
  
  
  bool BindGraphics(const Graphics3D& graphics);

  
  
  
  
  
  
  
  bool IsFullFrame();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t RequestInputEvents(uint32_t event_classes);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  int32_t RequestFilteringInputEvents(uint32_t event_classes);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void ClearInputEventRequest(uint32_t event_classes);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void PostMessage(const Var& message);

  

  
  

  
  
  
  
  void LogToConsole(PP_LogLevel level, const Var& value);

  
  
  
  
  
  
  
  
  void LogToConsoleWithSource(PP_LogLevel level,
                              const Var& source,
                              const Var& value);

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void AddPerInstanceObject(const std::string& interface_name, void* object);

  
  
  
  
  
  
  
  void RemovePerInstanceObject(const std::string& interface_name, void* object);

  
  
  
  static void RemovePerInstanceObject(const InstanceHandle& instance,
                                      const std::string& interface_name,
                                      void* object);

  
  
  
  
  
  
  
  
  
  static void* GetPerInstanceObject(PP_Instance instance,
                                    const std::string& interface_name);

 private:
  PP_Instance pp_instance_;

  typedef std::map<std::string, void*> InterfaceNameToObjectMap;
  InterfaceNameToObjectMap interface_name_to_objects_;
};

}  

#endif  
