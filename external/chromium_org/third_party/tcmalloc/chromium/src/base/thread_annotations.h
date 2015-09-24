// Copyright (c) 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_THREAD_ANNOTATIONS_H_
#define BASE_THREAD_ANNOTATIONS_H_


#if defined(__GNUC__) \
  && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4)) \
  && defined(__SUPPORT_TS_ANNOTATION__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   
#endif


#define GUARDED_BY(x)          THREAD_ANNOTATION_ATTRIBUTE__(guarded_by(x))
#define GUARDED_VAR            THREAD_ANNOTATION_ATTRIBUTE__(guarded)

#define PT_GUARDED_BY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(point_to_guarded_by(x))
#define PT_GUARDED_VAR \
  THREAD_ANNOTATION_ATTRIBUTE__(point_to_guarded)

#define ACQUIRED_AFTER(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquired_after(x))
#define ACQUIRED_BEFORE(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(acquired_before(x))


#define EXCLUSIVE_LOCKS_REQUIRED(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(exclusive_locks_required(x))

#define SHARED_LOCKS_REQUIRED(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(shared_locks_required(x))

#define LOCKS_EXCLUDED(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(locks_excluded(x))

#define LOCK_RETURNED(x)       THREAD_ANNOTATION_ATTRIBUTE__(lock_returned(x))

#define LOCKABLE               THREAD_ANNOTATION_ATTRIBUTE__(lockable)

#define SCOPED_LOCKABLE        THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)

#define EXCLUSIVE_LOCK_FUNCTION(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(exclusive_lock(x))

#define SHARED_LOCK_FUNCTION(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(shared_lock(x))

#define EXCLUSIVE_TRYLOCK_FUNCTION(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(exclusive_trylock(x))

#define SHARED_TRYLOCK_FUNCTION(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(shared_trylock(x))

#define UNLOCK_FUNCTION(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(unlock(x))

#define NO_THREAD_SAFETY_ANALYSIS \
  THREAD_ANNOTATION_ATTRIBUTE__(no_thread_safety_analysis)

#endif  
