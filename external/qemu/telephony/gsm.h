/* Copyright (C) 2007-2008 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#ifndef _android_gsm_h
#define _android_gsm_h


typedef unsigned char  byte_t;
typedef byte_t*        bytes_t;
typedef const byte_t*  cbytes_t;


extern byte_t   gsm_int_to_bcdi( int  value );

extern int      gsm_int_from_bcdi( byte_t  value );


extern int      gsm_hex_to_bytes  ( cbytes_t  hex, int  hexlen, bytes_t  dst );

extern void     gsm_hex_to_bytes0 ( cbytes_t  hex, int  hexlen, bytes_t  dst );

extern void     gsm_hex_from_bytes( char*  hex, cbytes_t  src, int  srclen );

extern int      gsm_hexchar_to_int( char  c );

extern int      gsm_hexchar_to_int0( char  c );

extern int      gsm_hex2_to_byte( const char*  hex );

extern int      gsm_hex2_to_byte0( const char*  hex );

extern int      gsm_hex4_to_short( const char*  hex );

extern int      gsm_hex4_to_short0( const char*  hex );

extern void     gsm_hex_from_byte( char*  hex, int  val );

extern void     gsm_hex_from_short( char*  hex, int  val );


extern int      utf8_check( cbytes_t  utf8, int  utf8len );

extern int      utf8_check_gsm7( cbytes_t  utf8, int  utf8len );

extern cbytes_t utf8_skip_gsm7( cbytes_t  utf8, cbytes_t  utf8end, int  gsm7len );

extern int      utf8_to_gsm7( cbytes_t  utf8, int  utf8len, bytes_t  dst, int  offset );

extern int      utf8_to_gsm8( cbytes_t  utf8, int  utf8len, bytes_t  dst );

extern int      utf8_from_gsm7( cbytes_t  src, int  offset, int  count, bytes_t  utf8 );

extern int      utf8_from_gsm8( cbytes_t  src, int  count, bytes_t  utf8 );



extern int      ucs2_check_gsm7( cbytes_t  ucs2, int  ucs2len );

extern int      ucs2_to_gsm7( cbytes_t  ucs2, int  ucs2len, bytes_t  dst, int  offset );

extern int      ucs2_to_gsm8( cbytes_t  ucs2, int  ucs2len, bytes_t  dst );

extern int      ucs2_from_gsm7( bytes_t   ucs2, cbytes_t  src, int  offset, int  count );

extern int      ucs2_from_gsm8( bytes_t   ucs2, cbytes_t  src, int  count );



extern int      ucs2_to_utf8( cbytes_t  ucs2, int  ucs2len, bytes_t  utf8 );

extern int      utf8_to_ucs2( cbytes_t  utf8, int  utf8len, bytes_t  ucs2 );

extern cbytes_t  utf8_skip( cbytes_t   utf8, cbytes_t   utf8end, int  count);


extern int  gsm_bcdnum_to_ascii  ( cbytes_t  bcd, int  num_digits, bytes_t  dst );

/* convert an ASCII dial-number into a bcd-coded string, returns the number of 4-bit nibbles written, */
extern int  gsm_bcdnum_from_ascii( cbytes_t  ascii, int  asciilen, bytes_t  dst );

#define  SIM_ADN_MAX_ALPHA        20  
#define  SIM_ADN_MAX_NUMBER       20  

typedef struct {
    byte_t  alpha [ SIM_ADN_MAX_ALPHA*3+1 ];  
    char    number[ SIM_ADN_MAX_NUMBER+1 ];   
}
SimAdnRec, *SimAdn;

typedef struct {
    SimAdnRec       adn;
    byte_t          ext_record;  
}
SimAdnRecordRec, *SimAdnRecord;

extern int  sim_adn_record_from_bytes( SimAdnRecord  rec, cbytes_t  data, int  datalen );
extern int  sim_adn_record_to_bytes  ( SimAdnRecord  rec, bytes_t   data, int  datalen );


typedef struct {
    bytes_t         data;
    int             max;
    int             pos;
    int             error;
    unsigned char   data0[16];
} GsmRopeRec, *GsmRope;

extern void      gsm_rope_init( GsmRope  rope );
extern void      gsm_rope_init_alloc( GsmRope  rope, int  alloc );
extern int       gsm_rope_done( GsmRope  rope );
extern bytes_t   gsm_rope_done_acquire( GsmRope  rope, int  *psize );
extern void      gsm_rope_add_c( GsmRope  rope, char  c );
extern void      gsm_rope_add( GsmRope  rope, const void*  str, int  len );
extern void*     gsm_rope_reserve( GsmRope  rope, int  len );

#define  PHONE_PREFIX "1555521"

#endif 
