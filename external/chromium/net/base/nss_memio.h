// Copyright (c) 2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// Written in NSPR style to also be suitable for adding to the NSS demo suite

#ifndef __MEMIO_H
#define __MEMIO_H
#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "prio.h"

struct memio_Private;
typedef struct memio_Private memio_Private;


PRFileDesc *memio_CreateIOLayer(int bufsize);

void memio_SetPeerName(PRFileDesc *fd, const PRNetAddr *peername);

memio_Private *memio_GetSecret(PRFileDesc *fd);

int memio_GetReadParams(memio_Private *secret, char **buf);

void memio_PutReadResult(memio_Private *secret, int bytes_read);

void memio_GetWriteParams(memio_Private *secret,
                          const char **buf1, unsigned int *len1,
                          const char **buf2, unsigned int *len2);

/* Tell memio how many bytes were sent to the network.
 * If bytes_written is < 0, it is treated as an NSPR error code.
 * See nspr/pr/src/md/unix/unix_errors.c for how to
 * map from Unix errors to NSPR error codes.
 * On EWOULDBLOCK or the equivalent, don't call this function.
 */
void memio_PutWriteResult(memio_Private *secret, int bytes_written);


#ifdef __cplusplus
}
#endif

#endif
