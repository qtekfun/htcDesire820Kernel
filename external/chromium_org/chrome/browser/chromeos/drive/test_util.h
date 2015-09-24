// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_DRIVE_TEST_UTIL_H_
#define CHROME_BROWSER_CHROMEOS_DRIVE_TEST_UTIL_H_

#include <string>

#include "google_apis/drive/test_util.h"
#include "net/base/completion_callback.h"
#include "net/base/io_buffer.h"
#include "net/base/network_change_notifier.h"
#include "net/base/test_completion_callback.h"
#include "third_party/cros_system_api/constants/cryptohome.h"

class PrefRegistrySimple;

namespace net {
class IOBuffer;
}  

namespace drive {

namespace test_util {

const int64 kLotsOfSpace = cryptohome::kMinFreeSpaceInBytes * 10;

void RunBlockingPoolTask();

struct DestroyHelperForTests {
  template<typename T>
  void operator()(T* object) const {
    if (object) {
      object->Destroy();
      test_util::RunBlockingPoolTask();  
    }
  }
};

template<typename Reader>
int ReadAllData(Reader* reader, std::string* content) {
  const int kBufferSize = 10;
  scoped_refptr<net::IOBuffer> buffer(new net::IOBuffer(kBufferSize));
  while (true) {
    net::TestCompletionCallback callback;
    int result = reader->Read(buffer.get(), kBufferSize, callback.callback());
    result = callback.GetResult(result);
    if (result <= 0) {
      
      return result;
    }
    content->append(buffer->data(), result);
  }
}

void RegisterDrivePrefs(PrefRegistrySimple* pref_registry);

class FakeNetworkChangeNotifier : public net::NetworkChangeNotifier {
 public:
  FakeNetworkChangeNotifier();

  void SetConnectionType(ConnectionType type);

  
  virtual ConnectionType GetCurrentConnectionType() const OVERRIDE;

 private:
  net::NetworkChangeNotifier::ConnectionType type_;
};

}  
}  

#endif  
