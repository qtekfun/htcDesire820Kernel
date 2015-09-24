// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_DBUS_CRAS_AUDIO_CLIENT_H_
#define CHROMEOS_DBUS_CRAS_AUDIO_CLIENT_H_

#include "base/callback.h"
#include "base/observer_list.h"
#include "chromeos/chromeos_export.h"
#include "chromeos/dbus/audio_node.h"
#include "chromeos/dbus/dbus_client.h"
#include "chromeos/dbus/volume_state.h"

namespace chromeos {

class CHROMEOS_EXPORT CrasAudioClient : public DBusClient {
 public:
  
  class Observer {
   public:
    
    
    virtual void AudioClientRestarted();

    
    virtual void OutputMuteChanged(bool mute_on);

    
    virtual void InputMuteChanged(bool mute_on);

    
    virtual void NodesChanged();

    
    virtual void ActiveOutputNodeChanged(uint64 node_id);

    
    virtual void ActiveInputNodeChanged(uint64 node_id);

   protected:
    virtual ~Observer();
  };

  virtual ~CrasAudioClient();

  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  
  virtual bool HasObserver(Observer* observer) = 0;

  
  
  
  
  typedef base::Callback<void(const VolumeState&, bool)> GetVolumeStateCallback;

  
  
  
  typedef base::Callback<void(const AudioNodeList&, bool)> GetNodesCallback;

  
  
  
  typedef base::Callback<void(const std::string&,
                              const std::string&)> ErrorCallback;

  
  virtual void GetVolumeState(const GetVolumeStateCallback& callback) = 0;

  
  virtual void GetNodes(const GetNodesCallback& callback,
                        const ErrorCallback& error_callback) = 0;

  
  
  virtual void SetOutputNodeVolume(uint64 node_id, int32 volume) = 0;

  
  virtual void SetOutputUserMute(bool mute_on) = 0;

  
  
  virtual void SetInputNodeGain(uint64 node_id, int32 gain) = 0;

  
  virtual void SetInputMute(bool mute_on) = 0;

  
  virtual void SetActiveOutputNode(uint64 node_id) = 0;

  
  virtual void SetActiveInputNode(uint64 node_id) = 0;

  
  static CrasAudioClient* Create();

 protected:
  
  CrasAudioClient();

 private:

  DISALLOW_COPY_AND_ASSIGN(CrasAudioClient);
};

}  

#endif  
