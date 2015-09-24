/*
 * Copyright (C) 2010 The Android Open Source Project
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
#ifndef ANDROID_ASYNC_UTILS_H
#define ANDROID_ASYNC_UTILS_H

#include "android/looper.h"
#include "sockets.h"

typedef enum {
    ASYNC_COMPLETE = 0,   
    ASYNC_ERROR,          
    ASYNC_NEED_MORE       
} AsyncStatus;


typedef struct {
    uint8_t*  buffer;
    size_t    buffsize;
    size_t    pos;
    LoopIo*   io;
} AsyncReader;

void asyncReader_init(AsyncReader* ar,
                      void*        buffer,
                      size_t       buffsize,
                      LoopIo*      io);

AsyncStatus  asyncReader_read(AsyncReader*  ar);


typedef struct {
    const uint8_t* buffer;
    size_t         buffsize;
    size_t         pos;
    LoopIo*        io;
} AsyncWriter;

void asyncWriter_init(AsyncWriter*  aw,
                      const void*   buffer,
                      size_t        buffsize,
                      LoopIo*       io);

AsyncStatus asyncWriter_write(AsyncWriter* aw);



typedef struct {
    uint8_t*  buffer;
    size_t    buffsize;
    size_t    pos;
    LoopIo*   io;
    char      eol;
} AsyncLineReader;

void asyncLineReader_init(AsyncLineReader* alr,
                          void*            buffer,
                          size_t           buffsize,
                          LoopIo*          io);

AINLINED void
asyncLineReader_setEOL(AsyncLineReader* alr, char eol)
{
    alr->eol = eol;
}

AsyncStatus asyncLineReader_read(AsyncLineReader* alr);

const char* asyncLineReader_getLineRaw(AsyncLineReader* alr, int *pLength);

const char* asyncLineReader_getLine(AsyncLineReader* alr);


typedef struct {
    int     error;
    int     state;
    LoopIo* io;
} AsyncConnector;

AsyncStatus
asyncConnector_init(AsyncConnector*    ac,
                    const SockAddress* address,
                    LoopIo*            io);

AsyncStatus
asyncConnector_run(AsyncConnector* ac);

int
asyncConnector_stop(AsyncConnector* ac);

#endif 
