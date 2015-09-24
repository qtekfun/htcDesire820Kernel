// Copyright (C) 2013 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef I18N_ADDRESSINPUT_CALLBACK_H_
#define I18N_ADDRESSINPUT_CALLBACK_H_

#include <cassert>
#include <cstddef>

namespace i18n {
namespace addressinput {

template <typename Key, typename Data>
class Callback {
 public:
  virtual ~Callback() {}

  virtual void operator()(bool success,
                          const Key& key,
                          const Data& data) const = 0;
};

namespace {

template <typename Observer, typename Key, typename Data>
class CallbackImpl : public Callback<Key, Data> {
 public:
  typedef void (Observer::*ObserveEvent)(bool, const Key&, const Data&);

  CallbackImpl(Observer* observer, ObserveEvent observe_event)
      : observer_(observer),
        observe_event_(observe_event) {
    assert(observer_ != NULL);
    assert(observe_event_ != NULL);
  }

  virtual ~CallbackImpl() {}

  virtual void operator()(bool success,
                          const Key& key,
                          const Data& data) const {
    (observer_->*observe_event_)(success, key, data);
  }

 private:
  Observer* observer_;
  ObserveEvent observe_event_;
};

}  

template <typename Observer, typename Key, typename Data>
Callback<Key, Data>* BuildCallback(
    Observer* observer,
    void (Observer::*observe_event)(bool, const Key&, const Data&)) {
  return new CallbackImpl<Observer, Key, Data>(observer, observe_event);
}

}  
}  

#endif  
