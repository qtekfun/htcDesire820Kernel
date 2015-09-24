// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_ANDROID_H_
#define CHROME_BROWSER_SYNC_PROFILE_SYNC_SERVICE_ANDROID_H_

#include <jni.h>
#include <map>

#include "base/android/jni_helper.h"
#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/time/time.h"
#include "chrome/browser/sync/profile_sync_service_observer.h"
#include "chrome/browser/sync/sync_prefs.h"
#include "google/cacheinvalidation/include/types.h"
#include "google_apis/gaia/google_service_auth_error.h"

class Profile;
class ProfileSyncService;

class ProfileSyncServiceAndroid : public ProfileSyncServiceObserver {
 public:

  ProfileSyncServiceAndroid(JNIEnv* env, jobject obj);

  
  void Init();

  
  
  
  void NudgeSyncer(JNIEnv* env,
                   jobject obj,
                   jint objectSource,
                   jstring objectId,
                   jlong version,
                   jstring payload);

  
  
  void NudgeSyncerForAllTypes(JNIEnv* env, jobject obj);

  
  void EnableSync(JNIEnv* env, jobject obj);

  
  void DisableSync(JNIEnv* env, jobject obj);

  
  void SignInSync(JNIEnv* env, jobject obj);

  
  void SignOutSync(JNIEnv* env, jobject obj);

  
  base::android::ScopedJavaLocalRef<jstring> QuerySyncStatusSummary(
      JNIEnv* env, jobject obj);

  
  
  
  
  jboolean SetSyncSessionsId(JNIEnv* env, jobject obj, jstring tag);

  
  jboolean IsSyncInitialized(JNIEnv* env, jobject obj);

  
  jboolean IsFirstSetupInProgress(JNIEnv* env, jobject obj);

  
  jboolean IsEncryptEverythingEnabled(JNIEnv* env, jobject obj);

  
  
  
  jboolean IsPassphraseRequired(JNIEnv* env, jobject obj);

  
  
  jboolean IsPassphraseRequiredForDecryption(JNIEnv* env, jobject obj);

  
  
  jboolean IsPassphraseRequiredForExternalType(JNIEnv* env, jobject obj);

  
  
  jboolean IsUsingSecondaryPassphrase(JNIEnv* env, jobject obj);

  
  
  
  jboolean SetDecryptionPassphrase(JNIEnv* env,
                                   jobject obj,
                                   jstring passphrase);

  
  
  
  void SetEncryptionPassphrase(JNIEnv* env,
                               jobject obj,
                               jstring passphrase,
                               jboolean is_gaia);

  
  
  jboolean IsCryptographerReady(JNIEnv* env, jobject);

  
  
  
  jint GetPassphraseType(JNIEnv* env, jobject obj);

  
  jboolean HasExplicitPassphraseTime(JNIEnv* env, jobject);

  base::android::ScopedJavaLocalRef<jstring>
      GetSyncEnterGooglePassphraseBodyWithDateText(
          JNIEnv* env, jobject);

  base::android::ScopedJavaLocalRef<jstring>
      GetSyncEnterCustomPassphraseBodyWithDateText(
          JNIEnv* env, jobject);

  base::android::ScopedJavaLocalRef<jstring>
      GetCurrentSignedInAccountText(
          JNIEnv* env, jobject);

  base::android::ScopedJavaLocalRef<jstring>
      GetSyncEnterCustomPassphraseBodyText(
          JNIEnv* env, jobject);

  
  jboolean IsSyncKeystoreMigrationDone(JNIEnv* env, jobject obj);

  
  
  
  
  jlong GetEnabledDataTypes(JNIEnv* env, jobject obj);

  
  
  
  
  
  
  void SetPreferredDataTypes(JNIEnv* env,
                             jobject obj,
                             jboolean sync_everything,
                             jlong model_type_selection);

  
  
  void SetSetupInProgress(JNIEnv* env, jobject obj, jboolean in_progress);

  
  void SetSyncSetupCompleted(JNIEnv* env, jobject obj);

  
  jboolean HasSyncSetupCompleted(JNIEnv* env, jobject obj);

  
  jboolean IsStartSuppressed(JNIEnv* env, jobject obj);

  
  jboolean HasKeepEverythingSynced(JNIEnv* env, jobject obj);

  
  
  
  void EnableEncryptEverything(JNIEnv* env, jobject obj);

  
  jboolean HasUnrecoverableError(JNIEnv* env, jobject obj);

  
  base::android::ScopedJavaLocalRef<jstring> GetAboutInfoForTest(JNIEnv* env,
                                                                 jobject obj);

  
  
  jint GetAuthError(JNIEnv* env, jobject obj);

  
  virtual void OnStateChanged() OVERRIDE;

  
  
  jlong GetLastSyncedTimeForTest(JNIEnv* env, jobject obj);

  static ProfileSyncServiceAndroid* GetProfileSyncServiceAndroid();

  
  static bool Register(JNIEnv* env);

 private:
  typedef std::map<invalidation::ObjectId,
                   int64,
                   syncer::ObjectIdLessThan> ObjectIdVersionMap;

  virtual ~ProfileSyncServiceAndroid();
  
  void RemoveObserver();
  
  
  
  void SendNudgeNotification(int object_source,
                             const std::string& str_object_id,
                             int64 version,
                             const std::string& payload);

  Profile* profile_;
  ProfileSyncService* sync_service_;
  
  
  scoped_ptr<browser_sync::SyncPrefs> sync_prefs_;

  
  JavaObjectWeakGlobalRef weak_java_profile_sync_service_;

  
  
  
  ObjectIdVersionMap max_invalidation_versions_;

  DISALLOW_COPY_AND_ASSIGN(ProfileSyncServiceAndroid);
};

#endif  
