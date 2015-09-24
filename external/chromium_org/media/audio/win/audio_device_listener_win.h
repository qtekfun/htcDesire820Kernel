// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MEDIA_AUDIO_WIN_AUDIO_DEVICE_LISTENER_WIN_H_
#define MEDIA_AUDIO_WIN_AUDIO_DEVICE_LISTENER_WIN_H_

#include <MMDeviceAPI.h>
#include <string>

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/threading/thread_checker.h"
#include "base/win/scoped_comptr.h"
#include "media/base/media_export.h"

using base::win::ScopedComPtr;

namespace media {

class MEDIA_EXPORT AudioDeviceListenerWin : public IMMNotificationClient {
 public:
  
  
  
  explicit AudioDeviceListenerWin(const base::Closure& listener_cb);
  virtual ~AudioDeviceListenerWin();

 private:
  friend class AudioDeviceListenerWinTest;

  
  STDMETHOD_(ULONG, AddRef)();
  STDMETHOD_(ULONG, Release)();
  STDMETHOD(QueryInterface)(REFIID iid, void** object);
  STDMETHOD(OnPropertyValueChanged)(LPCWSTR device_id, const PROPERTYKEY key);
  STDMETHOD(OnDeviceAdded)(LPCWSTR device_id);
  STDMETHOD(OnDeviceRemoved)(LPCWSTR device_id);
  STDMETHOD(OnDeviceStateChanged)(LPCWSTR device_id, DWORD new_state);
  STDMETHOD(OnDefaultDeviceChanged)(EDataFlow flow, ERole role,
                                    LPCWSTR new_default_device_id);

  base::Closure listener_cb_;
  ScopedComPtr<IMMDeviceEnumerator> device_enumerator_;
  std::string default_render_device_id_;

  
  base::ThreadChecker thread_checker_;

  DISALLOW_COPY_AND_ASSIGN(AudioDeviceListenerWin);
};

}  

#endif  
