// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MIDI_MIDI_MANAGER_H_
#define MEDIA_MIDI_MIDI_MANAGER_H_

#include <set>
#include <vector>

#include "base/basictypes.h"
#include "base/synchronization/lock.h"
#include "media/base/media_export.h"
#include "media/midi/midi_port_info.h"

namespace media {

class MEDIA_EXPORT MIDIManagerClient {
 public:
   virtual ~MIDIManagerClient() {}

  
  
  
  
  
  
  virtual void ReceiveMIDIData(uint32 port_index,
                               const uint8* data,
                               size_t length,
                               double timestamp) = 0;

  
  
  
  
  virtual void AccumulateMIDIBytesSent(size_t n) = 0;
};

class MEDIA_EXPORT MIDIManager {
 public:
  static MIDIManager* Create();

  MIDIManager();
  virtual ~MIDIManager();

  
  
  
  
  bool StartSession(MIDIManagerClient* client);

  
  void EndSession(MIDIManagerClient* client);

  
  
  
  
  
  
  
  
  virtual void DispatchSendMIDIData(MIDIManagerClient* client,
                                    uint32 port_index,
                                    const std::vector<uint8>& data,
                                    double timestamp) = 0;

  
  
  
  const MIDIPortInfoList& input_ports() { return input_ports_; }

  
  
  
  const MIDIPortInfoList& output_ports() { return output_ports_; }

 protected:
  
  virtual bool Initialize() = 0;

  void AddInputPort(const MIDIPortInfo& info);
  void AddOutputPort(const MIDIPortInfo& info);

  
  void ReceiveMIDIData(uint32 port_index,
                       const uint8* data,
                       size_t length,
                       double timestamp);

  bool initialized_;

  
  typedef std::set<MIDIManagerClient*> ClientList;
  ClientList clients_;

  
  base::Lock clients_lock_;

  MIDIPortInfoList input_ports_;
  MIDIPortInfoList output_ports_;

  DISALLOW_COPY_AND_ASSIGN(MIDIManager);
};

}  

#endif  
