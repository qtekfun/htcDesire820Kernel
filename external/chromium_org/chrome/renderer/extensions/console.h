// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_RENDERER_EXTENSIONS_CONSOLE_H_
#define CHROME_RENDERER_EXTENSIONS_CONSOLE_H_

#include <string>

#include "content/public/common/console_message_level.h"
#include "v8/include/v8.h"

namespace content {
class RenderView;
}

namespace extensions {

namespace console {

void Debug(content::RenderView* render_view, const std::string& message);
void Log(content::RenderView* render_view, const std::string& message);
void Warn(content::RenderView* render_view, const std::string& message);
void Error(content::RenderView* render_view, const std::string& message);

void Fatal(content::RenderView* render_view, const std::string& message);

void AddMessage(content::RenderView* render_view,
                content::ConsoleMessageLevel level,
                const std::string& message);

void Debug(v8::Handle<v8::Context> context, const std::string& message);
void Log(v8::Handle<v8::Context> context, const std::string& message);
void Warn(v8::Handle<v8::Context> context, const std::string& message);
void Error(v8::Handle<v8::Context> context, const std::string& message);

void Fatal(v8::Handle<v8::Context> context, const std::string& message);

void AddMessage(v8::Handle<v8::Context> context,
                content::ConsoleMessageLevel level,
                const std::string& message);

v8::Local<v8::Object> AsV8Object();

}  

}  

#endif  
