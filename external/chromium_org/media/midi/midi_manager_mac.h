// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_MIDI_MIDI_MANAGER_MAC_H_
#define MEDIA_MIDI_MIDI_MANAGER_MAC_H_

#include <CoreMIDI/MIDIServices.h>
#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/threading/thread.h"
#include "media/midi/midi_manager.h"
#include "media/midi/midi_port_info.h"

namespace media {

class MEDIA_EXPORT MIDIManagerMac : public MIDIManager {
 public:
  MIDIManagerMac();
  virtual ~MIDIManagerMac();

  
  virtual bool Initialize() OVERRIDE;
  virtual void DispatchSendMIDIData(MIDIManagerClient* client,
                                    uint32 port_index,
                                    const std::vector<uint8>& data,
                                    double timestamp) OVERRIDE;

 private:
  
  
  
  static void ReadMIDIDispatch(
      const MIDIPacketList *pktlist,
      void *read_proc_refcon,
      void *src_conn_refcon);
  virtual void ReadMIDI(MIDIEndpointRef source, const MIDIPacketList *pktlist);

  
  void SendMIDIData(MIDIManagerClient* client,
                    uint32 port_index,
                    const std::vector<uint8>& data,
                    double timestamp);

  
  static media::MIDIPortInfo GetPortInfoFromEndpoint(MIDIEndpointRef endpoint);
  static double MIDITimeStampToSeconds(MIDITimeStamp timestamp);
  static MIDITimeStamp SecondsToMIDITimeStamp(double seconds);

  
  MIDIClientRef midi_client_;
  MIDIPortRef coremidi_input_;
  MIDIPortRef coremidi_output_;

  enum{ kMaxPacketListSize = 512 };
  char midi_buffer_[kMaxPacketListSize];
  MIDIPacketList* packet_list_;
  MIDIPacket* midi_packet_;

  typedef std::map<MIDIEndpointRef, uint32> SourceMap;

  
  SourceMap source_map_;

  
  std::vector<MIDIEndpointRef> destinations_;

  
  base::Thread send_thread_;

  DISALLOW_COPY_AND_ASSIGN(MIDIManagerMac);
};

}  

#endif  
