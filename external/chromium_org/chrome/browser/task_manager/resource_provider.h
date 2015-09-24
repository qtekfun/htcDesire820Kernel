// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TASK_MANAGER_RESOURCE_PROVIDER_H_
#define CHROME_BROWSER_TASK_MANAGER_RESOURCE_PROVIDER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/process/process_handle.h"
#include "base/strings/string16.h"
#include "third_party/WebKit/public/web/WebCache.h"

class PrefRegistrySimple;
class TaskManagerModel;

namespace content {
class WebContents;
}

namespace extensions {
class Extension;
}

namespace gfx {
class ImageSkia;
}

namespace task_manager {

#define TASKMANAGER_RESOURCE_TYPE_LIST(def) \
    def(BROWSER)          \
    def(RENDERER)         \
    def(EXTENSION)        \
    def(NOTIFICATION)     \
    def(GUEST)            \
    def(PLUGIN)           \
    def(WORKER)           \
    def(NACL)             \
    def(UTILITY)          \
    def(ZYGOTE)           \
    def(SANDBOX_HELPER)   \
    def(GPU)             

#define TASKMANAGER_RESOURCE_TYPE_LIST_ENUM(a)   a,
#define TASKMANAGER_RESOURCE_TYPE_LIST_AS_STRING(a)   case a: return #a;

class Resource {
 public:
  virtual ~Resource() {}

  enum Type {
    UNKNOWN = 0,
    TASKMANAGER_RESOURCE_TYPE_LIST(TASKMANAGER_RESOURCE_TYPE_LIST_ENUM)
  };

  virtual base::string16 GetTitle() const = 0;
  virtual base::string16 GetProfileName() const = 0;
  virtual gfx::ImageSkia GetIcon() const = 0;
  virtual base::ProcessHandle GetProcess() const = 0;
  virtual int GetUniqueChildProcessId() const = 0;
  virtual Type GetType() const = 0;
  virtual int GetRoutingID() const;

  virtual bool ReportsCacheStats() const;
  virtual blink::WebCache::ResourceTypeStats GetWebCoreCacheStats() const;

  virtual bool ReportsFPS() const;
  virtual float GetFPS() const;

  virtual bool ReportsSqliteMemoryUsed() const;
  virtual size_t SqliteMemoryUsedBytes() const;

  
  virtual const extensions::Extension* GetExtension() const;

  virtual bool ReportsV8MemoryStats() const;
  virtual size_t GetV8MemoryAllocated() const;
  virtual size_t GetV8MemoryUsed() const;

  
  virtual bool CanInspect() const;

  
  virtual void Inspect() const {}

  
  
  
  virtual content::WebContents* GetWebContents() const;

  
  
  
  
  
  virtual bool SupportNetworkUsage() const = 0;

  
  
  virtual void SetSupportNetworkUsage() = 0;

  
  
  virtual void Refresh() {}

  virtual void NotifyResourceTypeStats(
      const blink::WebCache::ResourceTypeStats& stats) {}
  virtual void NotifyFPS(float fps) {}
  virtual void NotifyV8HeapStats(size_t v8_memory_allocated,
                                 size_t v8_memory_used) {}

  
  
  virtual bool IsBackground() const;

  static const char* GetResourceTypeAsString(const Type type) {
    switch (type) {
      TASKMANAGER_RESOURCE_TYPE_LIST(TASKMANAGER_RESOURCE_TYPE_LIST_AS_STRING)
      default: return "UNKNOWN";
    }
  }

  
  
  int get_unique_id() { return unique_id_; }

 protected:
  Resource() : unique_id_(0) {}

 private:
  friend class ::TaskManagerModel;
  int unique_id_;

  DISALLOW_COPY_AND_ASSIGN(Resource);
};

#undef TASKMANAGER_RESOURCE_TYPE_LIST
#undef TASKMANAGER_RESOURCE_TYPE_LIST_ENUM
#undef TASKMANAGER_RESOURCE_TYPE_LIST_AS_STRING

class ResourceProvider : public base::RefCountedThreadSafe<ResourceProvider> {
 public:
  
  
  virtual Resource* GetResource(int process_id,
                                int render_process_host_id,
                                int routing_id) = 0;
  virtual void StartUpdating() = 0;
  virtual void StopUpdating() = 0;

 protected:
  friend class base::RefCountedThreadSafe<ResourceProvider>;

  virtual ~ResourceProvider() {}
};

}  

#endif  
