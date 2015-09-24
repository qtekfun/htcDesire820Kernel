/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */



#if !defined(_ISAKMP_H_)
#define _ISAKMP_H_

typedef u_char cookie_t[8];
typedef u_char msgid_t[4];

typedef struct { 
	cookie_t i_ck;
	cookie_t r_ck;
} isakmp_index;

#define INITIATOR       1
#define RESPONDER       2

#define PORT_ISAKMP 500

#define GENERATE  1
#define VALIDATE  0

#define ISAKMP_PH1      0x0010
#define ISAKMP_PH2      0x0020
#define ISAKMP_EXPIRED  0x0100

#define ISAKMP_NGP_0    0x0000
#define ISAKMP_NGP_1    0x0001
#define ISAKMP_NGP_2    0x0002
#define ISAKMP_NGP_3    0x0003
#define ISAKMP_NGP_4    0x0004

#define ISAKMP_PH1_N    (ISAKMP_PH1 | ISAKMP_NGP_0)  
#define ISAKMP_PH1_1    (ISAKMP_PH1 | ISAKMP_NGP_1)
#define ISAKMP_PH1_2    (ISAKMP_PH1 | ISAKMP_NGP_2)
#define ISAKMP_PH1_3    (ISAKMP_PH1 | ISAKMP_NGP_3)
#define ISAKMP_PH2_N    (ISAKMP_PH2 | ISAKMP_NGP_0)
#define ISAKMP_PH2_1    (ISAKMP_PH2 | ISAKMP_NGP_1)
#define ISAKMP_PH2_2    (ISAKMP_PH2 | ISAKMP_NGP_2)
#define ISAKMP_PH2_3    (ISAKMP_PH2 | ISAKMP_NGP_3)

#define ISAKMP_TIMER_DEFAULT     10 
#define ISAKMP_TRY_DEFAULT        3 

struct isakmp {
	cookie_t i_ck;		
	cookie_t r_ck;		
	u_int8_t np;		
	u_int8_t vers;
#define ISAKMP_VERS_MAJOR	0xf0
#define ISAKMP_VERS_MAJOR_SHIFT	4
#define ISAKMP_VERS_MINOR	0x0f
#define ISAKMP_VERS_MINOR_SHIFT	0
	u_int8_t etype;		
	u_int8_t flags;		
	msgid_t msgid;
	u_int32_t len;		
};

#define ISAKMP_NPTYPE_NONE   0 
#define ISAKMP_NPTYPE_SA     1 
#define ISAKMP_NPTYPE_P      2 
#define ISAKMP_NPTYPE_T      3 
#define ISAKMP_NPTYPE_KE     4 
#define ISAKMP_NPTYPE_ID     5 
#define ISAKMP_NPTYPE_CERT   6 
#define ISAKMP_NPTYPE_CR     7 
#define ISAKMP_NPTYPE_HASH   8 
#define ISAKMP_NPTYPE_SIG    9 
#define ISAKMP_NPTYPE_NONCE 10 
#define ISAKMP_NPTYPE_N     11 
#define ISAKMP_NPTYPE_D     12 
#define ISAKMP_NPTYPE_VID   13 

#define ISAKMP_MAJOR_VERSION  1
#define ISAKMP_MINOR_VERSION  0

#define ISAKMP_ETYPE_NONE   0 
#define ISAKMP_ETYPE_BASE   1 
#define ISAKMP_ETYPE_IDENT  2 
#define ISAKMP_ETYPE_AUTH   3 
#define ISAKMP_ETYPE_AGG    4 
#define ISAKMP_ETYPE_INF    5 

#define ISAKMP_FLAG_E 0x01 
#define ISAKMP_FLAG_C 0x02 

struct isakmp_gen {
	u_int8_t  np;       
	u_int8_t  reserved; 
	u_int16_t len;      
};

struct isakmp_data {
	u_int16_t type;     
	u_int16_t lorv;     
	                  
	
};
#define ISAKMP_GEN_TLV 0x0000
#define ISAKMP_GEN_TV  0x8000
	
#define ISAKMP_GEN_MASK 0x8000

	
struct isakmp_pl_sa {
	struct isakmp_gen h;
	u_int32_t doi; 
	u_int32_t sit; 
};

struct isakmp_pl_p {
	struct isakmp_gen h;
	u_int8_t p_no;      
	u_int8_t prot_id;   
	u_int8_t spi_size;  
	u_int8_t num_t;     
	
};

struct isakmp_pl_t {
	struct isakmp_gen h;
	u_int8_t  t_no;     
	u_int8_t  t_id;     
	u_int16_t reserved; 
	
};

struct isakmp_pl_ke {
	struct isakmp_gen h;
	
};

	
struct isakmp_pl_id {
	struct isakmp_gen h;
	union {
		u_int8_t  id_type;   
		u_int32_t doi_data;  
	} d;
	
};

struct isakmp_pl_cert {
	struct isakmp_gen h;
	u_int8_t encode; 
	char   cert;   
};

#define ISAKMP_CERT_NONE   0
#define ISAKMP_CERT_PKCS   1
#define ISAKMP_CERT_PGP    2
#define ISAKMP_CERT_DNS    3
#define ISAKMP_CERT_SIGN   4
#define ISAKMP_CERT_KE     5
#define ISAKMP_CERT_KT     6
#define ISAKMP_CERT_CRL    7
#define ISAKMP_CERT_ARL    8
#define ISAKMP_CERT_SPKI   9

struct isakmp_pl_cr {
	struct isakmp_gen h;
	u_int8_t num_cert; 
	
	
};

	
struct isakmp_pl_hash {
	struct isakmp_gen h;
	
};

	
struct isakmp_pl_sig {
	struct isakmp_gen h;
	
};

	
struct isakmp_pl_nonce {
	struct isakmp_gen h;
	
};

struct isakmp_pl_n {
	struct isakmp_gen h;
	u_int32_t doi;      
	u_int8_t  prot_id;  
	u_int8_t  spi_size; 
	u_int16_t type;     
	
	
};

#define ISAKMP_NTYPE_INVALID_PAYLOAD_TYPE           1
#define ISAKMP_NTYPE_DOI_NOT_SUPPORTED              2
#define ISAKMP_NTYPE_SITUATION_NOT_SUPPORTED        3
#define ISAKMP_NTYPE_INVALID_COOKIE                 4
#define ISAKMP_NTYPE_INVALID_MAJOR_VERSION          5
#define ISAKMP_NTYPE_INVALID_MINOR_VERSION          6
#define ISAKMP_NTYPE_INVALID_EXCHANGE_TYPE          7
#define ISAKMP_NTYPE_INVALID_FLAGS                  8
#define ISAKMP_NTYPE_INVALID_MESSAGE_ID             9
#define ISAKMP_NTYPE_INVALID_PROTOCOL_ID            10
#define ISAKMP_NTYPE_INVALID_SPI                    11
#define ISAKMP_NTYPE_INVALID_TRANSFORM_ID           12
#define ISAKMP_NTYPE_ATTRIBUTES_NOT_SUPPORTED       13
#define ISAKMP_NTYPE_NO_PROPOSAL_CHOSEN             14
#define ISAKMP_NTYPE_BAD_PROPOSAL_SYNTAX            15
#define ISAKMP_NTYPE_PAYLOAD_MALFORMED              16
#define ISAKMP_NTYPE_INVALID_KEY_INFORMATION        17
#define ISAKMP_NTYPE_INVALID_ID_INFORMATION         18
#define ISAKMP_NTYPE_INVALID_CERT_ENCODING          19
#define ISAKMP_NTYPE_INVALID_CERTIFICATE            20
#define ISAKMP_NTYPE_BAD_CERT_REQUEST_SYNTAX        21
#define ISAKMP_NTYPE_INVALID_CERT_AUTHORITY         22
#define ISAKMP_NTYPE_INVALID_HASH_INFORMATION       23
#define ISAKMP_NTYPE_AUTHENTICATION_FAILED          24
#define ISAKMP_NTYPE_INVALID_SIGNATURE              25
#define ISAKMP_NTYPE_ADDRESS_NOTIFICATION           26
#define ISAKMP_NTYPE_CONNECTED                   16384
#define ISAKMP_LOG_RETRY_LIMIT_REACHED           65530

struct isakmp_pl_d {
	struct isakmp_gen h;
	u_int32_t doi;      
	u_int8_t  prot_id;  
	u_int8_t  spi_size; 
	u_int16_t num_spi;  
	
};


struct isakmp_ph1tab {
	struct isakmp_ph1 *head;
	struct isakmp_ph1 *tail;
	int len;
};

struct isakmp_ph2tab {
	struct isakmp_ph2 *head;
	struct isakmp_ph2 *tail;
	int len;
};

#define EXCHANGE_PROXY   1
#define EXCHANGE_MYSELF  0

#define PFS_NEED	1
#define PFS_NONEED	0

#endif 
