/*
 * OMX offloading remote processor driver
 *
 * Copyright(c) 2011 Texas Instruments. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name Texas Instruments nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RPMSG_OMX_H
#define RPMSG_OMX_H

#include <linux/ioctl.h>

#define OMX_IOC_MAGIC	'X'

#define OMX_IOCCONNECT		_IOW(OMX_IOC_MAGIC, 1, char *)
#define OMX_IOCIONREGISTER	_IOWR(OMX_IOC_MAGIC, 2, struct ion_fd_data)
#define OMX_IOCIONUNREGISTER	_IOWR(OMX_IOC_MAGIC, 3, struct ion_fd_data)

#define OMX_IOC_MAXNR	(3)

#ifdef __KERNEL__

enum omx_msg_types {
	OMX_CONN_REQ = 0,
	OMX_CONN_RSP = 1,
	OMX_DISCONNECT = 4,
	OMX_RAW_MSG = 5,
};

enum omx_error_codes {
	OMX_SUCCESS = 0,
	OMX_NOTSUPP = 1,
	OMX_NOMEM = 2,
};

enum omx_state {
	OMX_UNCONNECTED,
	OMX_CONNECTED,
	OMX_FAIL,
};

struct omx_msg_hdr {
	u32 type;
	u32 flags;
	u32 len;
	char data[0];
} __packed;

struct omx_conn_rsp {
	u32 status;
	u32 addr;
} __packed;

struct omx_disc_req {
	u32 addr;
} __packed;


#endif 

struct omx_conn_req {
	char name[48];
} __packed;

struct omx_packet {
	uint16_t      desc;	
	uint16_t      msg_id;	
	uint32_t      flags;	
	uint32_t      fxn_idx;	
	int32_t       result;	
	uint32_t      data_size;
	uint32_t      data[0];	
};

#endif 