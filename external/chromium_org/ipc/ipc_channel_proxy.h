// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_CHANNEL_PROXY_H_
#define IPC_IPC_CHANNEL_PROXY_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/lock.h"
#include "base/threading/non_thread_safe.h"
#include "ipc/ipc_channel.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_listener.h"
#include "ipc/ipc_sender.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace IPC {

class SendCallbackHelper;

class IPC_EXPORT ChannelProxy : public Sender, public base::NonThreadSafe {
 public:

  
  
  class IPC_EXPORT MessageFilter
      : public base::RefCountedThreadSafe<MessageFilter> {
   public:
    MessageFilter();

    
    
    
    virtual void OnFilterAdded(Channel* channel);

    
    
    
    virtual void OnFilterRemoved();

    
    
    virtual void OnChannelConnected(int32 peer_pid);

    
    
    virtual void OnChannelError();

    
    
    virtual void OnChannelClosing();

    
    
    virtual bool OnMessageReceived(const Message& message);

   protected:
    virtual ~MessageFilter();

   private:
    friend class base::RefCountedThreadSafe<MessageFilter>;
  };

  
  
  
  
  
  
  
  
  ChannelProxy(const IPC::ChannelHandle& channel_handle,
               Channel::Mode mode,
               Listener* listener,
               base::SingleThreadTaskRunner* ipc_task_runner);

  virtual ~ChannelProxy();

  
  
  
  
  void Init(const IPC::ChannelHandle& channel_handle, Channel::Mode mode,
            bool create_pipe_now);

  
  
  
  
  
  
  
  
  void Close();

  
  
  virtual bool Send(Message* message) OVERRIDE;

  
  
  
  
  
  
  
  
  
  void AddFilter(MessageFilter* filter);
  void RemoveFilter(MessageFilter* filter);

  
  void ClearIPCTaskRunner();

  
  
  base::ProcessId peer_pid() const { return context_->peer_pid_; }

#if defined(OS_POSIX) && !defined(OS_NACL)
  
  int GetClientFileDescriptor();
  int TakeClientFileDescriptor();
  bool GetPeerEuid(uid_t* peer_euid) const;
#endif  

 protected:
  class Context;
  
  
  ChannelProxy(Context* context);

  
  class Context : public base::RefCountedThreadSafe<Context>,
                  public Listener {
   public:
    Context(Listener* listener, base::SingleThreadTaskRunner* ipc_thread);
    void ClearIPCTaskRunner();
    base::SingleThreadTaskRunner* ipc_task_runner() const {
      return ipc_task_runner_.get();
    }
    const std::string& channel_id() const { return channel_id_; }

    
    void OnDispatchMessage(const Message& message);

   protected:
    friend class base::RefCountedThreadSafe<Context>;
    virtual ~Context();

    
    virtual bool OnMessageReceived(const Message& message) OVERRIDE;
    virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
    virtual void OnChannelError() OVERRIDE;

    
    bool OnMessageReceivedNoFilter(const Message& message);

    
    
    bool TryFilters(const Message& message);

    
    virtual void OnChannelOpened();
    virtual void OnChannelClosed();

    
    
    
    virtual void Clear();

   private:
    friend class ChannelProxy;
    friend class SendCallbackHelper;

    
    void CreateChannel(const IPC::ChannelHandle& channel_handle,
                       const Channel::Mode& mode);

    
    void OnSendMessage(scoped_ptr<Message> message_ptr);
    void OnAddFilter();
    void OnRemoveFilter(MessageFilter* filter);

    
    void AddFilter(MessageFilter* filter);
    void OnDispatchConnected();
    void OnDispatchError();

    scoped_refptr<base::SingleThreadTaskRunner> listener_task_runner_;
    Listener* listener_;

    
    std::vector<scoped_refptr<MessageFilter> > filters_;
    scoped_refptr<base::SingleThreadTaskRunner> ipc_task_runner_;
    scoped_ptr<Channel> channel_;
    std::string channel_id_;
    bool channel_connected_called_;

    
    
    std::vector<scoped_refptr<MessageFilter> > pending_filters_;
    
    base::Lock pending_filters_lock_;

    
    
    base::ProcessId peer_pid_;
  };

  Context* context() { return context_.get(); }

 private:
  friend class SendCallbackHelper;

  
  
  
  scoped_refptr<Context> context_;

  
  bool did_init_;
};

}  

#endif  
