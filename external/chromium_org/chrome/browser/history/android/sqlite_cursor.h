// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_ANDROID_SQLITE_CURSOR_H_
#define CHROME_BROWSER_HISTORY_ANDROID_SQLITE_CURSOR_H_

#include <jni.h>
#include <vector>

#include "base/android/scoped_java_ref.h"
#include "base/basictypes.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/favicon/favicon_service.h"
#include "chrome/browser/history/android/android_history_provider_service.h"
#include "chrome/browser/history/history_types.h"
#include "chrome/common/cancelable_task_tracker.h"

class SQLiteCursor {
 public:
  
  enum JavaColumnType {
    BLOB = 2004,
    LONG_VAR_CHAR = -1,
    NULL_TYPE = 0,
    NUMERIC = 2,
    DOUBLE = 8,
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  class TestObserver {
   public:
    TestObserver();

    
    virtual void OnPostMoveToTask() = 0;
    
    virtual void OnGetMoveToResult() = 0;
    
    virtual void OnPostGetFaviconTask() = 0;
    
    virtual void OnGetFaviconResult() = 0;

   protected:
    virtual ~TestObserver();
  };

  
  
  static base::android::ScopedJavaLocalRef<jobject> NewJavaSqliteCursor(
      JNIEnv* env,
      const std::vector<std::string>& column_names,
      history::AndroidStatement* statement,
      AndroidHistoryProviderService* service,
      FaviconService* favicon_service);

  static bool RegisterSqliteCursor(JNIEnv* env);

  

  
  jint GetCount(JNIEnv* env, jobject obj);

  
  base::android::ScopedJavaLocalRef<jobjectArray> GetColumnNames(
      JNIEnv* env,
      jobject obj);

  
  base::android::ScopedJavaLocalRef<jstring> GetString(JNIEnv* env,
                                                       jobject obj,
                                                       jint column);

  
  jlong GetLong(JNIEnv* env, jobject obj, jint column);

  
  jint GetInt(JNIEnv* env, jobject obj, jint column);

  
  jdouble GetDouble(JNIEnv* env, jobject obj, jint column);

  
  base::android::ScopedJavaLocalRef<jbyteArray> GetBlob(JNIEnv* env,
                                                        jobject obj,
                                                        jint column);

  
  jboolean IsNull(JNIEnv* env, jobject obj, jint column);

  
  
  
  jint MoveTo(JNIEnv* env, jobject obj, jint pos);

  
  jint GetColumnType(JNIEnv* env, jobject obj, jint column);

  
  void Destroy(JNIEnv* env, jobject obj);

 private:
  FRIEND_TEST_ALL_PREFIXES(SQLiteCursorTest, Run);

  
  
  
  
  SQLiteCursor(const std::vector<std::string>& column_names,
               history::AndroidStatement* statement,
               AndroidHistoryProviderService* service,
               FaviconService* favicon_service);

  virtual ~SQLiteCursor();

  
  
  void DestroyOnUIThread();

  
  void set_test_observer(TestObserver* test_observer) {
    test_observer_ = test_observer;
  }

  
  bool GetFavicon(chrome::FaviconID id,
                  std::vector<unsigned char>* image_data);

  void GetFaviconForIDInUIThread(
      chrome::FaviconID id,
      const FaviconService::FaviconRawCallback& callback);

  
  void OnFaviconData(const chrome::FaviconBitmapResult& bitmap_result);

  
  void OnMoved(AndroidHistoryProviderService::Handle handle, int pos);

  JavaColumnType GetColumnTypeInternal(int column);

  
  void RunMoveStatementOnUIThread(int pos);

  
  int position_;

  base::WaitableEvent event_;

  
  history::AndroidStatement* statement_;

  
  const std::vector<std::string> column_names_;

  AndroidHistoryProviderService* service_;

  FaviconService* favicon_service_;

  
  scoped_ptr<CancelableRequestConsumer> consumer_;
  scoped_ptr<CancelableTaskTracker> tracker_;

  
  int count_;

  
  chrome::FaviconBitmapResult favicon_bitmap_result_;

  TestObserver* test_observer_;

  DISALLOW_COPY_AND_ASSIGN(SQLiteCursor);
};

#endif  
