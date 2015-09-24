/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_ASYNC_IO_COMMON_H_
#define ANDROID_ASYNC_IO_COMMON_H_


typedef enum AsyncIOState {
    
    ASIO_STATE_QUEUED,
    ASIO_STATE_STARTED,
    ASIO_STATE_CONTINUES,
    
    ASIO_STATE_RETRYING,
    
    ASIO_STATE_SUCCEEDED,
    
    ASIO_STATE_FAILED,
    
    ASIO_STATE_TIMED_OUT,
    ASIO_STATE_CANCELLED,
    ASIO_STATE_FINISHED,
} AsyncIOState;

typedef enum AsyncIOAction {
    
    ASIO_ACTION_DONE,
    
    ASIO_ACTION_ABORT,
    
    ASIO_ACTION_RETRY,
} AsyncIOAction;

#endif  
