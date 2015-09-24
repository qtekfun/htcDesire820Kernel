// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GOOGLE_APIS_DRIVE_TEST_UTIL_H_
#define GOOGLE_APIS_DRIVE_TEST_UTIL_H_

#include <string>
#include <utility>
#include <vector>

#include "base/bind.h"
#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/template_util.h"
#include "google_apis/drive/base_requests.h"
#include "google_apis/drive/gdata_errorcode.h"
#include "google_apis/drive/task_util.h"

class GURL;

namespace base {
class FilePath;
class RunLoop;
class Value;
}

namespace net {
namespace test_server {
class BasicHttpResponse;
class HttpResponse;
struct HttpRequest;
}
}

namespace google_apis {
namespace test_util {

void RunAndQuit(base::RunLoop* run_loop, const base::Closure& closure);

template<typename CallbackType>
CallbackType CreateQuitCallback(base::RunLoop* run_loop,
                                const CallbackType& callback) {
  return CreateComposedCallback(base::Bind(&RunAndQuit, run_loop), callback);
}

bool RemovePrefix(const std::string& input,
                  const std::string& prefix,
                  std::string* output);

base::FilePath GetTestFilePath(const std::string& relative_path);

GURL GetBaseUrlForTesting(int port);

bool WriteStringToFile(const base::FilePath& file_path,
                       const std::string& content);

bool CreateFileOfSpecifiedSize(const base::FilePath& temp_dir,
                               size_t size,
                               base::FilePath* path,
                               std::string* data);

scoped_ptr<base::Value> LoadJSONFile(const std::string& relative_path);

scoped_ptr<net::test_server::BasicHttpResponse> CreateHttpResponseFromFile(
    const base::FilePath& file_path);

scoped_ptr<net::test_server::HttpResponse> HandleDownloadFileRequest(
    const GURL& base_url,
    net::test_server::HttpRequest* out_request,
    const net::test_server::HttpRequest& request);

bool ParseContentRangeHeader(const std::string& value,
                             int64* start_position,
                             int64* end_position,
                             int64* length);

namespace internal {

template<typename T> struct IsMovable : base::false_type {};
template<typename T, typename D>
struct IsMovable<scoped_ptr<T, D> > : base::true_type {};

template<bool UseConstRef, typename T> struct InTypeHelper {
  typedef const T& InType;
};
template<typename T> struct InTypeHelper<false, T> {
  typedef T InType;
};

template<bool IsMovable, typename T> struct MoveHelper {
  static const T& Move(const T* in) { return *in; }
};
template<typename T> struct MoveHelper<true, T> {
  static T Move(T* in) { return in->Pass(); }
};

template<typename T>
struct CopyResultCallbackHelper
      
      
      
      
      
      
      
    : InTypeHelper<
          base::is_class<T>::value && !IsMovable<T>::value,  
          T>,
      MoveHelper<IsMovable<T>::value, T> {
};

template<typename T1>
void CopyResultCallback(
    T1* out,
    typename CopyResultCallbackHelper<T1>::InType in) {
  *out = CopyResultCallbackHelper<T1>::Move(&in);
}

template<typename T1, typename T2>
void CopyResultCallback(
    T1* out1,
    T2* out2,
    typename CopyResultCallbackHelper<T1>::InType in1,
    typename CopyResultCallbackHelper<T2>::InType in2) {
  *out1 = CopyResultCallbackHelper<T1>::Move(&in1);
  *out2 = CopyResultCallbackHelper<T2>::Move(&in2);
}

template<typename T1, typename T2, typename T3>
void CopyResultCallback(
    T1* out1,
    T2* out2,
    T3* out3,
    typename CopyResultCallbackHelper<T1>::InType in1,
    typename CopyResultCallbackHelper<T2>::InType in2,
    typename CopyResultCallbackHelper<T3>::InType in3) {
  *out1 = CopyResultCallbackHelper<T1>::Move(&in1);
  *out2 = CopyResultCallbackHelper<T2>::Move(&in2);
  *out3 = CopyResultCallbackHelper<T3>::Move(&in3);
}

template<typename T1, typename T2, typename T3, typename T4>
struct OutputParams {
  OutputParams(T1* out1, T2* out2, T3* out3, T4* out4)
      : out1(out1), out2(out2), out3(out3), out4(out4) {}
  T1* out1;
  T2* out2;
  T3* out3;
  T4* out4;
};

template<typename T1, typename T2, typename T3, typename T4>
void CopyResultCallback(
    const OutputParams<T1, T2, T3, T4>& output,
    typename CopyResultCallbackHelper<T1>::InType in1,
    typename CopyResultCallbackHelper<T2>::InType in2,
    typename CopyResultCallbackHelper<T3>::InType in3,
    typename CopyResultCallbackHelper<T4>::InType in4) {
  *output.out1 = CopyResultCallbackHelper<T1>::Move(&in1);
  *output.out2 = CopyResultCallbackHelper<T2>::Move(&in2);
  *output.out3 = CopyResultCallbackHelper<T3>::Move(&in3);
  *output.out4 = CopyResultCallbackHelper<T4>::Move(&in4);
}

}  

template<typename T1>
base::Callback<void(typename internal::CopyResultCallbackHelper<T1>::InType)>
CreateCopyResultCallback(T1* out1) {
  return base::Bind(&internal::CopyResultCallback<T1>, out1);
}

template<typename T1, typename T2>
base::Callback<void(typename internal::CopyResultCallbackHelper<T1>::InType,
                    typename internal::CopyResultCallbackHelper<T2>::InType)>
CreateCopyResultCallback(T1* out1, T2* out2) {
  return base::Bind(&internal::CopyResultCallback<T1, T2>, out1, out2);
}

template<typename T1, typename T2, typename T3>
base::Callback<void(typename internal::CopyResultCallbackHelper<T1>::InType,
                    typename internal::CopyResultCallbackHelper<T2>::InType,
                    typename internal::CopyResultCallbackHelper<T3>::InType)>
CreateCopyResultCallback(T1* out1, T2* out2, T3* out3) {
  return base::Bind(
      &internal::CopyResultCallback<T1, T2, T3>, out1, out2, out3);
}

template<typename T1, typename T2, typename T3, typename T4>
base::Callback<void(typename internal::CopyResultCallbackHelper<T1>::InType,
                    typename internal::CopyResultCallbackHelper<T2>::InType,
                    typename internal::CopyResultCallbackHelper<T3>::InType,
                    typename internal::CopyResultCallbackHelper<T4>::InType)>
CreateCopyResultCallback(T1* out1, T2* out2, T3* out3, T4* out4) {
  return base::Bind(
      &internal::CopyResultCallback<T1, T2, T3, T4>,
      internal::OutputParams<T1, T2, T3, T4>(out1, out2, out3, out4));
}

typedef std::pair<int64, int64> ProgressInfo;

void AppendProgressCallbackResult(std::vector<ProgressInfo>* progress_values,
                                  int64 progress,
                                  int64 total);

class TestGetContentCallback {
 public:
  TestGetContentCallback();
  ~TestGetContentCallback();

  const GetContentCallback& callback() const { return callback_; }
  const ScopedVector<std::string>& data() const { return data_; }
  ScopedVector<std::string>* mutable_data() { return &data_; }
  std::string GetConcatenatedData() const;

 private:
  void OnGetContent(google_apis::GDataErrorCode error,
                    scoped_ptr<std::string> data);

  const GetContentCallback callback_;
  ScopedVector<std::string> data_;

  DISALLOW_COPY_AND_ASSIGN(TestGetContentCallback);
};

}  
}  

#endif  
