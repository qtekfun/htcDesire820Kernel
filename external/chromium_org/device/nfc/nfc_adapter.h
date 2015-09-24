// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEVICE_NFC_NFC_ADAPTER_H_
#define DEVICE_NFC_NFC_ADAPTER_H_

#include <map>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"

namespace device {

class NfcPeer;
class NfcTag;

class NfcAdapter : public base::RefCounted<NfcAdapter> {
 public:
  
  class Observer {
   public:
    virtual ~Observer() {}

    
    
    
    
    virtual void AdapterPresentChanged(NfcAdapter* adapter, bool present) {}

    
    
    
    virtual void AdapterPoweredChanged(NfcAdapter* adapter, bool powered) {}

    
    
    
    
    
    virtual void AdapterPollingChanged(NfcAdapter* adapter, bool polling) {}

    
    
    
    
    
    
    
    
    virtual void TagFound(NfcAdapter* adapter, NfcTag* tag) {}

    
    
    virtual void TagLost(NfcAdapter* adapter, NfcTag* tag) {}

    
    
    
    
    
    
    
    
    
    virtual void PeerFound(NfcAdapter* adaper, NfcPeer* peer) {}

    
    
    virtual void PeerLost(NfcAdapter* adapter, NfcPeer* peer) {}
  };

  
  typedef base::Closure ErrorCallback;

  
  typedef std::vector<NfcPeer*> PeerList;
  typedef std::vector<NfcTag*> TagList;

  
  
  
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  
  
  
  
  virtual bool IsPresent() const = 0;

  
  virtual bool IsPowered() const = 0;

  
  virtual bool IsPolling() const = 0;

  
  virtual bool IsInitialized() const = 0;

  
  
  
  
  
  virtual void SetPowered(bool powered,
                          const base::Closure& callback,
                          const ErrorCallback& error_callback) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void StartPolling(const base::Closure& callback,
                            const ErrorCallback& error_callback) = 0;

  
  
  
  
  
  
  virtual void StopPolling(const base::Closure& callback,
                           const ErrorCallback& error_callback) = 0;

  
  
  
  virtual void GetPeers(PeerList* peer_list) const;

  
  
  
  virtual void GetTags(TagList* tag_list) const;

  
  
  
  virtual NfcPeer* GetPeer(const std::string& identifier) const;

  
  
  
  virtual NfcTag* GetTag(const std::string& identifier) const;

 protected:
  friend class base::RefCounted<NfcAdapter>;

  
  
  NfcAdapter();
  virtual ~NfcAdapter();

  
  
  
  
  typedef std::map<const std::string, NfcPeer*> PeersMap;
  typedef std::map<const std::string, NfcTag*> TagsMap;

  PeersMap peers_;
  TagsMap tags_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NfcAdapter);
};

}  

#endif  
