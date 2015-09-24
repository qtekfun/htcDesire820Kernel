// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_COMMON_CONTENT_CLIENT_H_
#define CONTENT_PUBLIC_COMMON_CONTENT_CLIENT_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "base/strings/string_piece.h"
#include "build/build_config.h"
#include "content/common/content_export.h"
#include "ui/base/layout.h"

class CommandLine;
class GURL;

namespace base {
class RefCountedStaticMemory;
}

namespace IPC {
class Message;
}

namespace gfx {
class Image;
}

namespace gpu {
struct GPUInfo;
}

namespace sandbox {
class TargetPolicy;
}

namespace content {

class ContentBrowserClient;
class ContentClient;
class ContentPluginClient;
class ContentRendererClient;
class ContentUtilityClient;
struct PepperPluginInfo;

CONTENT_EXPORT void SetContentClient(ContentClient* client);

#if defined(CONTENT_IMPLEMENTATION)
ContentClient* GetContentClient();
#endif

CONTENT_EXPORT ContentBrowserClient* SetBrowserClientForTesting(
    ContentBrowserClient* b);
CONTENT_EXPORT ContentRendererClient* SetRendererClientForTesting(
    ContentRendererClient* r);
CONTENT_EXPORT ContentUtilityClient* SetUtilityClientForTesting(
    ContentUtilityClient* u);

CONTENT_EXPORT const std::string& GetUserAgent(const GURL& url);

class CONTENT_EXPORT ContentClient {
 public:
  ContentClient();
  virtual ~ContentClient();

  ContentBrowserClient* browser() { return browser_; }
  ContentPluginClient* plugin() { return plugin_; }
  ContentRendererClient* renderer() { return renderer_; }
  ContentUtilityClient* utility() { return utility_; }

  
  virtual void SetActiveURL(const GURL& url) {}

  
  virtual void SetGpuInfo(const gpu::GPUInfo& gpu_info) {}

  
  virtual void AddPepperPlugins(
      std::vector<content::PepperPluginInfo>* plugins) {}

  
  
  virtual void AddAdditionalSchemes(
      std::vector<std::string>* standard_schemes,
      std::vector<std::string>* savable_schemes) {}

  
  virtual bool CanSendWhileSwappedOut(const IPC::Message* message);

  
  
  virtual bool CanHandleWhileSwappedOut(const IPC::Message& message);

  
  
  
  virtual std::string GetProduct() const;

  
  virtual std::string GetUserAgent() const;

  
  virtual base::string16 GetLocalizedString(int message_id) const;

  
  virtual base::StringPiece GetDataResource(
      int resource_id,
      ui::ScaleFactor scale_factor) const;

  
  virtual base::RefCountedStaticMemory* GetDataResourceBytes(
      int resource_id) const;

  
  virtual gfx::Image& GetNativeImageNamed(int resource_id) const;

  
  
  virtual std::string GetProcessTypeNameInEnglish(int type);

#if defined(OS_MACOSX) && !defined(OS_IOS)
  
  
  
  
  
  
  
  virtual bool GetSandboxProfileForSandboxType(
      int sandbox_type,
      int* sandbox_profile_resource_id) const;

  
  
  virtual std::string GetCarbonInterposePath() const;
#endif

 private:
  friend class ContentClientInitializer;  
  friend class InternalTestInitializer;

  
  ContentBrowserClient* browser_;
  
  ContentPluginClient* plugin_;
  
  ContentRendererClient* renderer_;
  
  ContentUtilityClient* utility_;
};

}  

#endif  
