// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT



#ifndef GOOGLE_MAYBE_THREADS_H_
#define GOOGLE_MAYBE_THREADS_H_

#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif

int perftools_pthread_key_create(pthread_key_t *key,
                                 void (*destr_function) (void *));
void *perftools_pthread_getspecific(pthread_key_t key);
int perftools_pthread_setspecific(pthread_key_t key, void *val);
int perftools_pthread_once(pthread_once_t *ctl,
                           void  (*init_routine) (void));

#endif  
