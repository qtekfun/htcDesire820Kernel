// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_AUDIO_CRAS_AUDIO_HANDLER_H_
#define CHROMEOS_AUDIO_CRAS_AUDIO_HANDLER_H_

#include <queue>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chromeos/audio/audio_device.h"
#include "chromeos/audio/audio_pref_observer.h"
#include "chromeos/dbus/audio_node.h"
#include "chromeos/dbus/cras_audio_client.h"
#include "chromeos/dbus/session_manager_client.h"
#include "chromeos/dbus/volume_state.h"

class PrefRegistrySimple;
class PrefService;

namespace chromeos {

class AudioDevicesPrefHandler;

class CHROMEOS_EXPORT CrasAudioHandler : public CrasAudioClient::Observer,
                                         public AudioPrefObserver,
                                         public SessionManagerClient::Observer {
 public:
  typedef std::priority_queue<AudioDevice,
                              std::vector<AudioDevice>,
                              AudioDeviceCompare> AudioDevicePriorityQueue;

  class AudioObserver {
   public:
    
    virtual void OnOutputVolumeChanged();

    
    virtual void OnOutputMuteChanged();

    
    virtual void OnInputGainChanged();

    
    virtual void OnInputMuteChanged();

    
    virtual void OnAudioNodesChanged();

    
    virtual void OnActiveOutputNodeChanged();

    
    virtual void OnActiveInputNodeChanged();

   protected:
    AudioObserver();
    virtual ~AudioObserver();
    DISALLOW_COPY_AND_ASSIGN(AudioObserver);
  };

  
  static void Initialize(
      scoped_refptr<AudioDevicesPrefHandler> audio_pref_handler);

  
  static void InitializeForTesting();

  
  static void Shutdown();

  
  static bool IsInitialized();

  
  static CrasAudioHandler* Get();

  
  virtual void AddAudioObserver(AudioObserver* observer);

  
  virtual void RemoveAudioObserver(AudioObserver* observer);

  
  virtual bool IsOutputMuted();

  
  virtual bool IsOutputMutedForDevice(uint64 device_id);

  
  virtual bool IsInputMuted();

  
  virtual bool IsInputMutedForDevice(uint64 device_id);

  
  virtual bool IsOutputVolumeBelowDefaultMuteLvel();

  
  
  virtual int GetOutputVolumePercent();

  
  virtual int GetOutputVolumePercentForDevice(uint64 device_id);

  
  
  virtual int GetInputGainPercent();

  
  virtual int GetInputGainPercentForDevice(uint64 device_id);

  
  virtual uint64 GetActiveOutputNode() const;

  
  virtual uint64 GetActiveInputNode() const;

  
  virtual void GetAudioDevices(AudioDeviceList* device_list) const;

  virtual bool GetActiveOutputDevice(AudioDevice* device) const;

  
  virtual bool has_alternative_input() const;
  virtual bool has_alternative_output() const;

  
  virtual void SetOutputVolumePercent(int volume_percent);

  
  virtual void SetInputGainPercent(int gain_percent);

  
  virtual void AdjustOutputVolumeByPercent(int adjust_by_percent);

  
  virtual void AdjustOutputVolumeToAudibleLevel();

  
  virtual void SetOutputMute(bool mute_on);

  
  virtual void SetInputMute(bool mute_on);

  
  virtual void SwitchToDevice(const AudioDevice& device);

  
  virtual void SetVolumeGainPercentForDevice(uint64 device_id, int value);

  
  virtual void SetMuteForDevice(uint64 device_id, bool mute_on);

  
  virtual void LogErrors();

 protected:
  explicit CrasAudioHandler(
      scoped_refptr<AudioDevicesPrefHandler> audio_pref_handler);
  virtual ~CrasAudioHandler();

 private:
  
  virtual void AudioClientRestarted() OVERRIDE;
  virtual void NodesChanged() OVERRIDE;
  virtual void ActiveOutputNodeChanged(uint64 node_id) OVERRIDE;
  virtual void ActiveInputNodeChanged(uint64 node_id) OVERRIDE;

  
  virtual void OnAudioPolicyPrefChanged() OVERRIDE;

  
  virtual void EmitLoginPromptVisibleCalled() OVERRIDE;

  
  void SetActiveOutputNode(uint64 node_id);
  void SetActiveInputNode(uint64 node_id);

  
  
  void SetupAudioInputState();
  void SetupAudioOutputState();

  const AudioDevice* GetDeviceFromId(uint64 device_id) const;

  
  
  void InitializeAudioState();

  
  
  void ApplyAudioPolicy();

  
  void SetOutputNodeVolume(uint64 node_id, int volume);

  
  
  bool SetOutputMuteInternal(bool mute_on);

  
  void SetInputNodeGain(uint64 node_id, int gain);

  
  
  bool SetInputMuteInternal(bool mute_on);

  
  void GetNodes();

  
  
  void UpdateDevicesAndSwitchActive(const AudioNodeList& nodes);

  
  
  bool ChangeActiveDevice(const AudioDevice& new_active_device,
                          uint64* current_active_node_id);

  
  
  bool NonActiveDeviceUnplugged(size_t old_devices_size,
                                size_t new_device_size,
                                uint64 current_active_node);

  
  
  bool HasDeviceChange(const AudioNodeList& new_nodes, bool is_input);

  
  void HandleGetNodes(const chromeos::AudioNodeList& node_list, bool success);

  
  void HandleGetNodesError(const std::string& error_name,
                           const std::string& error_msg);

  
  bool FoundNewDevice(const AudioDevice& device);

  
  AudioDevice GetSanitizedAudioDevice(const AudioNode& node);

  scoped_refptr<AudioDevicesPrefHandler> audio_pref_handler_;
  base::WeakPtrFactory<CrasAudioHandler> weak_ptr_factory_;
  ObserverList<AudioObserver> observers_;

  
  AudioDeviceMap audio_devices_;

  AudioDevicePriorityQueue input_devices_pq_;
  AudioDevicePriorityQueue output_devices_pq_;

  bool output_mute_on_;
  bool input_mute_on_;
  int output_volume_;
  int input_gain_;
  uint64 active_output_node_id_;
  uint64 active_input_node_id_;
  bool has_alternative_input_;
  bool has_alternative_output_;

  bool output_mute_locked_;
  bool input_mute_locked_;

  
  bool log_errors_;

  DISALLOW_COPY_AND_ASSIGN(CrasAudioHandler);
};

}  

#endif  
