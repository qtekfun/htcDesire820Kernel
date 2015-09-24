// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_COMMON_CANCELABLE_REQUEST_H_
#define CHROME_BROWSER_COMMON_CANCELABLE_REQUEST_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/bind.h"
#include "base/callback.h"
#include "base/callback_internal.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/cancellation_flag.h"
#include "base/synchronization/lock.h"
#include "build/build_config.h"

class CancelableRequestBase;
class CancelableRequestConsumerBase;


class CancelableRequestProvider {
 public:
  
  typedef int Handle;

  CancelableRequestProvider();

  
  
  
  
  void CancelRequest(Handle handle);

 protected:
  virtual ~CancelableRequestProvider();

  
  
  
  Handle AddRequest(CancelableRequestBase* request,
                    CancelableRequestConsumerBase* consumer);

  
  
  
  void RequestCompleted(Handle handle);

 private:
  typedef std::map<Handle, scoped_refptr<CancelableRequestBase> >
      CancelableRequestMap;

  
  void CancelRequestLocked(const CancelableRequestMap::iterator& item);

  friend class CancelableRequestBase;

  base::Lock pending_request_lock_;

  
  CancelableRequestMap pending_requests_;

  
  int next_handle_;

  DISALLOW_COPY_AND_ASSIGN(CancelableRequestProvider);
};


class CancelableRequestConsumerBase {
 protected:
  friend class CancelableRequestBase;
  friend class CancelableRequestProvider;

  virtual ~CancelableRequestConsumerBase() {
  }

  
  
  virtual void OnRequestAdded(CancelableRequestProvider* provider,
                              CancelableRequestProvider::Handle handle) = 0;

  
  
  
  virtual void OnRequestRemoved(CancelableRequestProvider* provider,
                                CancelableRequestProvider::Handle handle) = 0;

  
  virtual void WillExecute(CancelableRequestProvider* provider,
                           CancelableRequestProvider::Handle handle) = 0;

  
  virtual void DidExecute(CancelableRequestProvider* provider,
                          CancelableRequestProvider::Handle handle) = 0;
};

template<class T>
class CancelableRequestConsumerTSimple
    : public CancelableRequestConsumerBase {
 public:
  CancelableRequestConsumerTSimple();

  
  
  
  
  virtual ~CancelableRequestConsumerTSimple();

  
  
  
  void SetClientData(CancelableRequestProvider* p,
                     CancelableRequestProvider::Handle h,
                     T client_data);

  
  
  
  T GetClientData(CancelableRequestProvider* p,
                  CancelableRequestProvider::Handle h);

  
  
  T GetClientDataForCurrentRequest();

  
  bool HasPendingRequests() const;

  
  size_t PendingRequestCount() const;

  
  void CancelAllRequests();

  
  void CancelAllRequestsForClientData(T client_data);

  
  
  
  bool GetFirstHandleForClientData(T client_data,
                                   CancelableRequestProvider::Handle* handle);

  
  void GetAllClientData(std::vector<T>* data);

 protected:
  struct PendingRequest {
    PendingRequest(CancelableRequestProvider* p,
                   CancelableRequestProvider::Handle h)
        : provider(p), handle(h) {
    }

    PendingRequest() : provider(NULL), handle(0) {}

    
    bool operator<(const PendingRequest& other) const {
      if (provider != other.provider)
        return provider < other.provider;
      return handle < other.handle;
    }

    bool is_valid() const { return provider != NULL; }

    CancelableRequestProvider* provider;
    CancelableRequestProvider::Handle handle;
  };
  typedef std::map<PendingRequest, T> PendingRequestList;

  virtual T get_initial_t() const;

  virtual void OnRequestAdded(CancelableRequestProvider* provider,
                              CancelableRequestProvider::Handle handle);

  virtual void OnRequestRemoved(CancelableRequestProvider* provider,
                                CancelableRequestProvider::Handle handle);

  virtual void WillExecute(CancelableRequestProvider* provider,
                           CancelableRequestProvider::Handle handle);

  virtual void DidExecute(CancelableRequestProvider* provider,
                          CancelableRequestProvider::Handle handle);

  
  PendingRequestList pending_requests_;

  
  
  PendingRequest current_request_;
};

template<class T>
CancelableRequestConsumerTSimple<T>::CancelableRequestConsumerTSimple() {
}

template<class T>
CancelableRequestConsumerTSimple<T>::~CancelableRequestConsumerTSimple() {
  CancelAllRequests();
}

template<class T>
void CancelableRequestConsumerTSimple<T>::SetClientData(
    CancelableRequestProvider* p,
    CancelableRequestProvider::Handle h,
    T client_data) {
  PendingRequest request(p, h);
  DCHECK(pending_requests_.find(request) != pending_requests_.end());
  pending_requests_[request] = client_data;
}

template<class T>
T CancelableRequestConsumerTSimple<T>::GetClientData(
    CancelableRequestProvider* p,
    CancelableRequestProvider::Handle h) {
  PendingRequest request(p, h);
  DCHECK(pending_requests_.find(request) != pending_requests_.end());
  return pending_requests_[request];
}

template<class T>
T CancelableRequestConsumerTSimple<T>::GetClientDataForCurrentRequest() {
  DCHECK(current_request_.is_valid());
  return GetClientData(current_request_.provider, current_request_.handle);
}

template<class T>
bool CancelableRequestConsumerTSimple<T>::HasPendingRequests() const {
  return !pending_requests_.empty();
}

template<class T>
size_t CancelableRequestConsumerTSimple<T>::PendingRequestCount() const {
  return pending_requests_.size();
}

template<class T>
void CancelableRequestConsumerTSimple<T>::CancelAllRequests() {
  
  
  
  PendingRequestList copied_requests(pending_requests_);
  for (typename PendingRequestList::iterator i = copied_requests.begin();
       i != copied_requests.end(); ++i) {
    i->first.provider->CancelRequest(i->first.handle);
  }
  copied_requests.clear();

  
  DCHECK(pending_requests_.empty());
}

template<class T>
void CancelableRequestConsumerTSimple<T>::CancelAllRequestsForClientData(
    T client_data) {
  PendingRequestList copied_requests(pending_requests_);
  for (typename PendingRequestList::const_iterator i = copied_requests.begin();
      i != copied_requests.end(); ++i) {
    if (i->second == client_data)
      i->first.provider->CancelRequest(i->first.handle);
  }
  copied_requests.clear();
}

template<class T>
bool CancelableRequestConsumerTSimple<T>::GetFirstHandleForClientData(
    T client_data,
    CancelableRequestProvider::Handle* handle) {
  for (typename PendingRequestList::const_iterator i =
           pending_requests_.begin(); i != pending_requests_.end(); ++i) {
    if (i->second == client_data) {
      *handle = i->first.handle;
      return true;
    }
  }
  *handle = 0;
  return false;
}

template<class T>
void CancelableRequestConsumerTSimple<T>::GetAllClientData(
    std::vector<T>* data) {
  DCHECK(data);
  for (typename PendingRequestList::iterator i = pending_requests_.begin();
       i != pending_requests_.end(); ++i)
    data->push_back(i->second);
}

template<class T>
T CancelableRequestConsumerTSimple<T>::get_initial_t() const {
  return T();
}

template<class T>
void CancelableRequestConsumerTSimple<T>::OnRequestAdded(
    CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle) {
  
  
  
#ifndef NDEBUG
  if (!pending_requests_.empty())
    DCHECK(pending_requests_.begin()->first.provider == provider);
#endif
  DCHECK(pending_requests_.find(PendingRequest(provider, handle)) ==
         pending_requests_.end());
  pending_requests_[PendingRequest(provider, handle)] = get_initial_t();
}

template<class T>
void CancelableRequestConsumerTSimple<T>::OnRequestRemoved(
    CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle) {
  typename PendingRequestList::iterator i =
      pending_requests_.find(PendingRequest(provider, handle));
  if (i == pending_requests_.end()) {
    NOTREACHED() << "Got a complete notification for a nonexistent request";
    return;
  }

  pending_requests_.erase(i);
}

template<class T>
void CancelableRequestConsumerTSimple<T>::WillExecute(
    CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle) {
  current_request_ = PendingRequest(provider, handle);
}

template<class T>
void CancelableRequestConsumerTSimple<T>::DidExecute(
    CancelableRequestProvider* provider,
    CancelableRequestProvider::Handle handle) {
  current_request_ = PendingRequest();
}

template<class T, T initial_t>
class CancelableRequestConsumerT
    : public CancelableRequestConsumerTSimple<T> {
 public:
  CancelableRequestConsumerT();
  virtual ~CancelableRequestConsumerT();

 protected:
  virtual T get_initial_t() const;
};

template<class T, T initial_t>
CancelableRequestConsumerT<T, initial_t>::CancelableRequestConsumerT() {
}

template<class T, T initial_t>
CancelableRequestConsumerT<T, initial_t>::~CancelableRequestConsumerT() {
}

template<class T, T initial_t>
T CancelableRequestConsumerT<T, initial_t>::get_initial_t() const {
  return initial_t;
}

typedef CancelableRequestConsumerT<int, 0> CancelableRequestConsumer;

#if !defined(COMPILER_MSVC)
extern template class CancelableRequestConsumerTSimple<int>;

extern template class CancelableRequestConsumerT<int, 0>;
#endif


class CancelableRequestBase
    : public base::RefCountedThreadSafe<CancelableRequestBase> {
 public:
  friend class CancelableRequestProvider;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  CancelableRequestBase();

  CancelableRequestConsumerBase* consumer() const {
    return consumer_;
  }

  CancelableRequestProvider::Handle handle() const {
    return handle_;
  }

  
  
  
  void set_canceled() {
    canceled_.Set();
  }
  bool canceled() {
    return canceled_.IsSet();
  }

 protected:
  friend class base::RefCountedThreadSafe<CancelableRequestBase>;
  virtual ~CancelableRequestBase();

  
  
  void Init(CancelableRequestProvider* provider,
            CancelableRequestProvider::Handle handle,
            CancelableRequestConsumerBase* consumer);

  
  
  void DoForward(const base::Closure& forwarded_call, bool force_async);

  
  
  void NotifyCompleted() const {
    provider_->RequestCompleted(handle());
    consumer_->DidExecute(provider_, handle_);
  }

  
  void WillExecute() {
    consumer_->WillExecute(provider_, handle_);
  }

  
  
  base::MessageLoop* callback_thread_;

  
  
  CancelableRequestProvider* provider_;

  
  
  CancelableRequestConsumerBase* consumer_;

  
  
  
  CancelableRequestProvider::Handle handle_;

  
  
  base::CancellationFlag canceled_;

 private:
  
  
  
  void ExecuteCallback(const base::Closure& forwarded_call);

  DISALLOW_COPY_AND_ASSIGN(CancelableRequestBase);
};

template<typename CB>
class CancelableRequest : public CancelableRequestBase {
 public:
  
  
  typedef CB CallbackType;          
  typedef typename CB::TupleType TupleType;  

  
  
  
  explicit CancelableRequest(CallbackType* callback)
      : CancelableRequestBase(),
        callback_(callback) {
    DCHECK(callback) << "We should always have a callback";
  }

  
  
  
  
  
  
  
  
  
  void ForwardResult(const TupleType& param) {
    DCHECK(callback_.get());
    if (!canceled()) {
      if (callback_thread_ == base::MessageLoop::current()) {
        
        ExecuteCallback(param);
      } else {
        callback_thread_->PostTask(
            FROM_HERE,
            base::Bind(&CancelableRequest<CB>::ExecuteCallback, this, param));
      }
    }
  }

  
  void ForwardResultAsync(const TupleType& param) {
    DCHECK(callback_.get());
    if (!canceled()) {
      callback_thread_->PostTask(
          FROM_HERE,
          base::Bind(&CancelableRequest<CB>::ExecuteCallback, this, param));
    }
  }

 protected:
  virtual ~CancelableRequest() {}

 private:
  
  
  void ExecuteCallback(const TupleType& param) {
    if (!canceled_.IsSet()) {
      WillExecute();

      
      callback_->RunWithParams(param);
    }

    
    
    
    if (!canceled_.IsSet())
      NotifyCompleted();
  }

  
  
  scoped_ptr<CallbackType> callback_;
};

template<>
class CancelableRequest<base::Closure> : public CancelableRequestBase {
 public:
  typedef base::Closure CallbackType;

  explicit CancelableRequest(const CallbackType& callback)
      : CancelableRequestBase(),
        callback_(callback) {
    DCHECK(!callback.is_null()) << "Callback must be initialized.";
  }

  void ForwardResult(void) {
    if (canceled()) return;
    DoForward(callback_, false);
  }

  void ForwardResultAsync() {
    if (canceled()) return;
    DoForward(callback_, true);
  }

 protected:
  virtual ~CancelableRequest() {}

 private:
  CallbackType callback_;
};

template<typename A1>
class CancelableRequest<base::Callback<void(A1)> >
    : public CancelableRequestBase {
 public:
  typedef base::Callback<void(A1)> CallbackType;

  explicit CancelableRequest(const CallbackType& callback)
      : CancelableRequestBase(),
        callback_(callback) {
    DCHECK(!callback.is_null()) << "Callback must be initialized.";
  }

  void ForwardResult(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1), false);
  }

  void ForwardResultAsync(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1), true);
  }

 protected:
  virtual ~CancelableRequest() {}

 private:
  CallbackType callback_;
};

template<typename A1, typename A2>
class CancelableRequest<base::Callback<void(A1,A2)> >
    : public CancelableRequestBase {
 public:
  typedef base::Callback<void(A1,A2)> CallbackType;

  explicit CancelableRequest(const CallbackType& callback)
      : CancelableRequestBase(),
        callback_(callback) {
    DCHECK(!callback.is_null()) << "Callback must be initialized.";
  }

  void ForwardResult(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2), false);
  }

  void ForwardResultAsync(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2), true);
  }

 protected:
  virtual ~CancelableRequest() {}

 private:
  CallbackType callback_;
};

template<typename A1, typename A2, typename A3>
class CancelableRequest<base::Callback<void(A1,A2,A3)> >
    : public CancelableRequestBase {
 public:
  typedef base::Callback<void(A1,A2,A3)> CallbackType;

  explicit CancelableRequest(const CallbackType& callback)
      : CancelableRequestBase(),
        callback_(callback) {
    DCHECK(!callback.is_null()) << "Callback must be initialized.";
  }

  void ForwardResult(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2, a3), false);
  }

  void ForwardResultAsync(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2, a3), true);
  }

 protected:
  virtual ~CancelableRequest() {}

 private:
  CallbackType callback_;
};

template<typename A1, typename A2, typename A3, typename A4>
class CancelableRequest<base::Callback<void(A1, A2, A3, A4)> >
    : public CancelableRequestBase {
 public:
  typedef base::Callback<void(A1, A2, A3, A4)> CallbackType;

  explicit CancelableRequest(const CallbackType& callback)
      : CancelableRequestBase(),
        callback_(callback) {
    DCHECK(!callback.is_null()) << "Callback must be initialized.";
  }

  void ForwardResult(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2, a3, a4), false);
  }

  void ForwardResultAsync(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2, a3, a4), true);
  }

 protected:
  virtual ~CancelableRequest() {}

 private:
  CallbackType callback_;
};

template<typename A1, typename A2, typename A3, typename A4, typename A5>
class CancelableRequest<base::Callback<void(A1, A2, A3, A4, A5)> >
    : public CancelableRequestBase {
 public:
  typedef base::Callback<void(A1, A2, A3, A4, A5)> CallbackType;

  explicit CancelableRequest(const CallbackType& callback)
      : CancelableRequestBase(),
        callback_(callback) {
    DCHECK(!callback.is_null()) << "Callback must be initialized.";
  }

  void ForwardResult(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2, a3, a4, a5), false);
  }

  void ForwardResultAsync(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2, a3, a4, a5), true);
  }

 protected:
  virtual ~CancelableRequest() {}

 private:
  CallbackType callback_;
};

template<typename A1, typename A2, typename A3, typename A4, typename A5,
         typename A6>
class CancelableRequest<base::Callback<void(A1, A2, A3, A4, A5, A6)> >
    : public CancelableRequestBase {
 public:
  typedef base::Callback<void(A1, A2, A3, A4, A5, A6)> CallbackType;

  explicit CancelableRequest(const CallbackType& callback)
      : CancelableRequestBase(),
        callback_(callback) {
    DCHECK(!callback.is_null()) << "Callback must be initialized.";
  }

  void ForwardResult(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5,
      typename base::internal::CallbackParamTraits<A6>::ForwardType a6) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2, a3, a4, a5, a6), false);
  }

  void ForwardResultAsync(
      typename base::internal::CallbackParamTraits<A1>::ForwardType a1,
      typename base::internal::CallbackParamTraits<A2>::ForwardType a2,
      typename base::internal::CallbackParamTraits<A3>::ForwardType a3,
      typename base::internal::CallbackParamTraits<A4>::ForwardType a4,
      typename base::internal::CallbackParamTraits<A5>::ForwardType a5,
      typename base::internal::CallbackParamTraits<A6>::ForwardType a6) {
    if (canceled()) return;
    DoForward(base::Bind(callback_, a1, a2, a3, a4, a5, a6), true);
  }

 protected:
  virtual ~CancelableRequest() {}

 private:
  CallbackType callback_;
};

template<typename CB, typename Type>
class CancelableRequest1 : public CancelableRequest<CB> {
 public:
  explicit CancelableRequest1(
      const typename CancelableRequest<CB>::CallbackType& callback)
      : CancelableRequest<CB>(callback), value() {
  }

  
  Type value;

 protected:
  virtual ~CancelableRequest1() {}
};

#endif  
