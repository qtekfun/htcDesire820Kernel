// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ANDROID_PROVIDER_CHROME_BROWSER_PROVIDER_H_
#define CHROME_BROWSER_ANDROID_PROVIDER_CHROME_BROWSER_PROVIDER_H_

#include "base/android/jni_helper.h"
#include "base/android/scoped_java_ref.h"
#include "base/memory/scoped_ptr.h"
#include "base/synchronization/waitable_event.h"
#include "chrome/browser/bookmarks/base_bookmark_model_observer.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/history/android/android_history_types.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

class AndroidHistoryProviderService;
class FaviconService;
class Profile;

namespace history {
class TopSites;
}

namespace sql {
class Statement;
}

class ChromeBrowserProvider : public BaseBookmarkModelObserver,
                              public content::NotificationObserver {
 public:
  ChromeBrowserProvider(JNIEnv* env, jobject obj);
  void Destroy(JNIEnv*, jobject);

  
  static bool RegisterChromeBrowserProvider(JNIEnv* env);

  
  
  
  
  jlong AddBookmark(JNIEnv* env, jobject,
                    jstring jurl,
                    jstring jtitle,
                    jboolean is_folder,
                    jlong parent_id);

  
  jint RemoveBookmark(JNIEnv*, jobject, jlong id);

  
  
  
  
  
  jint UpdateBookmark(JNIEnv* env,
                      jobject,
                      jlong id,
                      jstring url,
                      jstring title,
                      jlong parent_id);

  
  
  jlong AddBookmarkFromAPI(JNIEnv* env,
                           jobject obj,
                           jstring url,
                           jobject created,
                           jobject isBookmark,
                           jobject date,
                           jbyteArray favicon,
                           jstring title,
                           jobject visits,
                           jlong parent_id);

  base::android::ScopedJavaLocalRef<jobject> QueryBookmarkFromAPI(
      JNIEnv* env,
      jobject obj,
      jobjectArray projection,
      jstring selections,
      jobjectArray selection_args,
      jstring sort_order);

  jint UpdateBookmarkFromAPI(JNIEnv* env,
                             jobject obj,
                             jstring url,
                             jobject created,
                             jobject isBookmark,
                             jobject date,
                             jbyteArray favicon,
                             jstring title,
                             jobject visits,
                             jlong parent_id,
                             jstring selections,
                             jobjectArray selection_args);

  jint RemoveBookmarkFromAPI(JNIEnv* env,
                             jobject obj,
                             jstring selections,
                             jobjectArray selection_args);

  jint RemoveHistoryFromAPI(JNIEnv* env,
                            jobject obj,
                            jstring selections,
                            jobjectArray selection_args);

  jlong AddSearchTermFromAPI(JNIEnv* env,
                             jobject obj,
                             jstring search_term,
                             jobject date);

  base::android::ScopedJavaLocalRef<jobject> QuerySearchTermFromAPI(
      JNIEnv* env,
      jobject obj,
      jobjectArray projection,
      jstring selections,
      jobjectArray selection_args,
      jstring sort_order);

  jint RemoveSearchTermFromAPI(JNIEnv* env,
                               jobject obj,
                               jstring selections,
                               jobjectArray selection_args);

  jint UpdateSearchTermFromAPI(JNIEnv* env,
                               jobject obj,
                               jstring search_term,
                               jobject date,
                               jstring selections,
                               jobjectArray selection_args);

  
  jlong CreateBookmarksFolderOnce(JNIEnv* env,
                                  jobject obj,
                                  jstring title,
                                  jlong parent_id);

  void RemoveAllBookmarks(JNIEnv* env, jobject obj);

  base::android::ScopedJavaLocalRef<jobject> GetAllBookmarkFolders(JNIEnv* env,
                                                                   jobject obj);

  base::android::ScopedJavaLocalRef<jobject> GetBookmarkNode(
      JNIEnv* env,
      jobject obj,
      jlong id,
      jboolean get_parent,
      jboolean get_children);

  base::android::ScopedJavaLocalRef<jobject> GetMobileBookmarksFolder(
      JNIEnv* env,
      jobject obj);

  jboolean IsBookmarkInMobileBookmarksBranch(JNIEnv* env,
                                             jobject obj,
                                             jlong id);

  jboolean BookmarkNodeExists(JNIEnv* env,
                              jobject obj,
                              jlong id);

  base::android::ScopedJavaLocalRef<jbyteArray> GetFaviconOrTouchIcon(
      JNIEnv* env,
      jobject obj,
      jstring url);

  base::android::ScopedJavaLocalRef<jbyteArray> GetThumbnail(JNIEnv* env,
                                                             jobject obj,
                                                             jstring url);

 private:
  virtual ~ChromeBrowserProvider();

  
  virtual void BookmarkModelChanged() OVERRIDE;
  virtual void ExtensiveBookmarkChangesBeginning(BookmarkModel* model) OVERRIDE;
  virtual void ExtensiveBookmarkChangesEnded(BookmarkModel* model) OVERRIDE;

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  JavaObjectWeakGlobalRef weak_java_provider_;

  Profile* profile_;
  BookmarkModel* bookmark_model_;
  history::TopSites* top_sites_;

  scoped_ptr<AndroidHistoryProviderService> service_;
  scoped_ptr<FaviconService> favicon_service_;

  
  
  
  
  
  CancelableRequestConsumer android_history_consumer_;
  CancelableRequestConsumer favicon_consumer_;

  CancelableTaskTracker cancelable_task_tracker_;

  
  content::NotificationRegistrar notification_registrar_;

  bool handling_extensive_changes_;

  DISALLOW_COPY_AND_ASSIGN(ChromeBrowserProvider);
};

#endif  
