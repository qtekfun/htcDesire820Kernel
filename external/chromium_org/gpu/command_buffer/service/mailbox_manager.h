// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_MAILBOX_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_MAILBOX_MANAGER_H_

#include <functional>
#include <map>

#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "gpu/command_buffer/common/constants.h"
#include "gpu/command_buffer/common/mailbox.h"
#include "gpu/gpu_export.h"

#ifndef GL_MAILBOX_SIZE_CHROMIUM
#define GL_MAILBOX_SIZE_CHROMIUM 64
#endif

typedef signed char GLbyte;

namespace gpu {
namespace gles2 {

class MailboxSynchronizer;
class Texture;
class TextureManager;

class GPU_EXPORT MailboxManager : public base::RefCounted<MailboxManager> {
 public:
  MailboxManager();

  
  void GenerateMailbox(Mailbox* mailbox);

  
  Texture* ConsumeTexture(unsigned target, const Mailbox& mailbox);

  
  void ProduceTexture(unsigned target,
                      const Mailbox& mailbox,
                      Texture* texture);

  
  bool UsesSync() { return sync_ != NULL; }

  
  
  void PushTextureUpdates();
  void PullTextureUpdates();

  
  void TextureDeleted(Texture* texture);

 private:
  friend class base::RefCounted<MailboxManager>;
  friend class MailboxSynchronizer;

  ~MailboxManager();

  struct TargetName {
    TargetName(unsigned target, const Mailbox& mailbox);
    unsigned target;
    Mailbox mailbox;
  };
  void InsertTexture(TargetName target_name, Texture* texture);

  static bool TargetNameLess(const TargetName& lhs, const TargetName& rhs);

  
  
  
  
  typedef std::multimap<Texture*, TargetName> TextureToMailboxMap;
  typedef std::map<
      TargetName,
      TextureToMailboxMap::iterator,
      std::pointer_to_binary_function<
          const TargetName&, const TargetName&, bool> > MailboxToTextureMap;

  MailboxToTextureMap mailbox_to_textures_;
  TextureToMailboxMap textures_to_mailboxes_;

  MailboxSynchronizer* sync_;

  DISALLOW_COPY_AND_ASSIGN(MailboxManager);
};
}  
}  

#endif  


