/*
** 2001 September 15
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Utility functions used throughout sqlite.
**
** This file contains functions for allocating memory, comparing
** strings, and stuff like that.
**
*/
#include "sqliteInt.h"
#include <stdarg.h>
#ifdef SQLITE_HAVE_ISNAN
# include <math.h>
#endif

#ifdef SQLITE_COVERAGE_TEST
void sqlite3Coverage(int x){
  static unsigned dummy = 0;
  dummy += (unsigned)x;
}
#endif

#ifndef SQLITE_OMIT_FLOATING_POINT
int sqlite3IsNaN(double x){
  int rc;   
#if !defined(SQLITE_HAVE_ISNAN)
#ifdef __FAST_MATH__
# error SQLite will not work correctly with the -ffast-math option of GCC.
#endif
  volatile double y = x;
  volatile double z = y;
  rc = (y!=z);
#else  
  rc = isnan(x);
#endif 
  testcase( rc );
  return rc;
}
#endif 

int sqlite3Strlen30(const char *z){
  const char *z2 = z;
  if( z==0 ) return 0;
  while( *z2 ){ z2++; }
  return 0x3fffffff & (int)(z2 - z);
}

void sqlite3Error(sqlite3 *db, int err_code, const char *zFormat, ...){
  if( db && (db->pErr || (db->pErr = sqlite3ValueNew(db))!=0) ){
    db->errCode = err_code;
    if( zFormat ){
      char *z;
      va_list ap;
      va_start(ap, zFormat);
      z = sqlite3VMPrintf(db, zFormat, ap);
      va_end(ap);
      sqlite3ValueSetStr(db->pErr, -1, z, SQLITE_UTF8, SQLITE_DYNAMIC);
    }else{
      sqlite3ValueSetStr(db->pErr, 0, 0, SQLITE_UTF8, SQLITE_STATIC);
    }
  }
}

void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...){
  char *zMsg;
  va_list ap;
  sqlite3 *db = pParse->db;
  va_start(ap, zFormat);
  zMsg = sqlite3VMPrintf(db, zFormat, ap);
  va_end(ap);
  if( db->suppressErr ){
    sqlite3DbFree(db, zMsg);
  }else{
    pParse->nErr++;
    sqlite3DbFree(db, pParse->zErrMsg);
    pParse->zErrMsg = zMsg;
    pParse->rc = SQLITE_ERROR;
  }
}

int sqlite3Dequote(char *z){
  char quote;
  int i, j;
  if( z==0 ) return -1;
  quote = z[0];
  switch( quote ){
    case '\'':  break;
    case '"':   break;
    case '`':   break;                
    case '[':   quote = ']';  break;  
    default:    return -1;
  }
  for(i=1, j=0; ALWAYS(z[i]); i++){
    if( z[i]==quote ){
      if( z[i+1]==quote ){
        z[j++] = quote;
        i++;
      }else{
        break;
      }
    }else{
      z[j++] = z[i];
    }
  }
  z[j] = 0;
  return j;
}

#define UpperToLower sqlite3UpperToLower

int sqlite3StrICmp(const char *zLeft, const char *zRight){
  register unsigned char *a, *b;
  a = (unsigned char *)zLeft;
  b = (unsigned char *)zRight;
  while( *a!=0 && UpperToLower[*a]==UpperToLower[*b]){ a++; b++; }
  return UpperToLower[*a] - UpperToLower[*b];
}
int sqlite3_strnicmp(const char *zLeft, const char *zRight, int N){
  register unsigned char *a, *b;
  a = (unsigned char *)zLeft;
  b = (unsigned char *)zRight;
  while( N-- > 0 && *a!=0 && UpperToLower[*a]==UpperToLower[*b]){ a++; b++; }
  return N<0 ? 0 : UpperToLower[*a] - UpperToLower[*b];
}

int sqlite3AtoF(const char *z, double *pResult, int length, u8 enc){
#ifndef SQLITE_OMIT_FLOATING_POINT
  int incr = (enc==SQLITE_UTF8?1:2);
  const char *zEnd = z + length;
  
  int sign = 1;    
  i64 s = 0;       
  int d = 0;       
  int esign = 1;   
  int e = 0;       
  int eValid = 1;  
  double result;
  int nDigits = 0;

  *pResult = 0.0;   

  if( enc==SQLITE_UTF16BE ) z++;

  
  while( z<zEnd && sqlite3Isspace(*z) ) z+=incr;
  if( z>=zEnd ) return 0;

  
  if( *z=='-' ){
    sign = -1;
    z+=incr;
  }else if( *z=='+' ){
    z+=incr;
  }

  
  while( z<zEnd && z[0]=='0' ) z+=incr, nDigits++;

  
  while( z<zEnd && sqlite3Isdigit(*z) && s<((LARGEST_INT64-9)/10) ){
    s = s*10 + (*z - '0');
    z+=incr, nDigits++;
  }

  while( z<zEnd && sqlite3Isdigit(*z) ) z+=incr, nDigits++, d++;
  if( z>=zEnd ) goto do_atof_calc;

  
  if( *z=='.' ){
    z+=incr;
    while( z<zEnd && sqlite3Isdigit(*z) && s<((LARGEST_INT64-9)/10) ){
      s = s*10 + (*z - '0');
      z+=incr, nDigits++, d--;
    }
    
    while( z<zEnd && sqlite3Isdigit(*z) ) z+=incr, nDigits++;
  }
  if( z>=zEnd ) goto do_atof_calc;

  
  if( *z=='e' || *z=='E' ){
    z+=incr;
    eValid = 0;
    if( z>=zEnd ) goto do_atof_calc;
    
    if( *z=='-' ){
      esign = -1;
      z+=incr;
    }else if( *z=='+' ){
      z+=incr;
    }
    
    while( z<zEnd && sqlite3Isdigit(*z) ){
      e = e*10 + (*z - '0');
      z+=incr;
      eValid = 1;
    }
  }

  
  if( nDigits && eValid ){
    while( z<zEnd && sqlite3Isspace(*z) ) z+=incr;
  }

do_atof_calc:
  
  e = (e*esign) + d;
  if( e<0 ) {
    esign = -1;
    e *= -1;
  } else {
    esign = 1;
  }

  
  if( !s ) {
    result = (sign<0 && nDigits) ? -(double)0 : (double)0;
  } else {
    
    if( esign>0 ){
      while( s<(LARGEST_INT64/10) && e>0 ) e--,s*=10;
    }else{
      while( !(s%10) && e>0 ) e--,s/=10;
    }

    
    s = sign<0 ? -s : s;

    if( e ){
      double scale = 1.0;
      
      if( e>307 && e<342 ){
        while( e%308 ) { scale *= 1.0e+1; e -= 1; }
        if( esign<0 ){
          result = s / scale;
          result /= 1.0e+308;
        }else{
          result = s * scale;
          result *= 1.0e+308;
        }
      }else{
        while( e%22 ) { scale *= 1.0e+1; e -= 1; }
        while( e>0 ) { scale *= 1.0e+22; e -= 22; }
        if( esign<0 ){
          result = s / scale;
        }else{
          result = s * scale;
        }
      }
    } else {
      result = (double)s;
    }
  }

  
  *pResult = result;

  
  return z>=zEnd && nDigits>0 && eValid;
#else
  return !sqlite3Atoi64(z, pResult, length, enc);
#endif 
}

static int compare2pow63(const char *zNum, int incr){
  int c = 0;
  int i;
                    
  const char *pow63 = "922337203685477580";
  for(i=0; c==0 && i<18; i++){
    c = (zNum[i*incr]-pow63[i])*10;
  }
  if( c==0 ){
    c = zNum[18*incr] - '8';
    testcase( c==(-1) );
    testcase( c==0 );
    testcase( c==(+1) );
  }
  return c;
}


int sqlite3Atoi64(const char *zNum, i64 *pNum, int length, u8 enc){
  int incr = (enc==SQLITE_UTF8?1:2);
  u64 u = 0;
  int neg = 0; 
  int i;
  int c = 0;
  const char *zStart;
  const char *zEnd = zNum + length;
  if( enc==SQLITE_UTF16BE ) zNum++;
  while( zNum<zEnd && sqlite3Isspace(*zNum) ) zNum+=incr;
  if( zNum<zEnd ){
    if( *zNum=='-' ){
      neg = 1;
      zNum+=incr;
    }else if( *zNum=='+' ){
      zNum+=incr;
    }
  }
  zStart = zNum;
  while( zNum<zEnd && zNum[0]=='0' ){ zNum+=incr; } 
  for(i=0; &zNum[i]<zEnd && (c=zNum[i])>='0' && c<='9'; i+=incr){
    u = u*10 + c - '0';
  }
  if( u>LARGEST_INT64 ){
    *pNum = SMALLEST_INT64;
  }else if( neg ){
    *pNum = -(i64)u;
  }else{
    *pNum = (i64)u;
  }
  testcase( i==18 );
  testcase( i==19 );
  testcase( i==20 );
  if( (c!=0 && &zNum[i]<zEnd) || (i==0 && zStart==zNum) || i>19*incr ){
    return 1;
  }else if( i<19*incr ){
    
    assert( u<=LARGEST_INT64 );
    return 0;
  }else{
    
    c = compare2pow63(zNum, incr);
    if( c<0 ){
      
      assert( u<=LARGEST_INT64 );
      return 0;
    }else if( c>0 ){
      
      return 1;
    }else{
      assert( u-1==LARGEST_INT64 );
      assert( (*pNum)==SMALLEST_INT64 );
      return neg ? 0 : 2;
    }
  }
}

int sqlite3GetInt32(const char *zNum, int *pValue){
  sqlite_int64 v = 0;
  int i, c;
  int neg = 0;
  if( zNum[0]=='-' ){
    neg = 1;
    zNum++;
  }else if( zNum[0]=='+' ){
    zNum++;
  }
  while( zNum[0]=='0' ) zNum++;
  for(i=0; i<11 && (c = zNum[i] - '0')>=0 && c<=9; i++){
    v = v*10 + c;
  }

  testcase( i==10 );
  if( i>10 ){
    return 0;
  }
  testcase( v-neg==2147483647 );
  if( v-neg>2147483647 ){
    return 0;
  }
  if( neg ){
    v = -v;
  }
  *pValue = (int)v;
  return 1;
}

int sqlite3Atoi(const char *z){
  int x = 0;
  if( z ) sqlite3GetInt32(z, &x);
  return x;
}


/*
** Write a 64-bit variable-length integer to memory starting at p[0].
** The length of data write will be between 1 and 9 bytes.  The number
** of bytes written is returned.
**
** A variable-length integer consists of the lower 7 bits of each byte
** for all bytes that have the 8th bit set and one byte with the 8th
** bit clear.  Except, if we get to the 9th byte, it stores the full
** 8 bits and is the last byte.
*/
int sqlite3PutVarint(unsigned char *p, u64 v){
  int i, j, n;
  u8 buf[10];
  if( v & (((u64)0xff000000)<<32) ){
    p[8] = (u8)v;
    v >>= 8;
    for(i=7; i>=0; i--){
      p[i] = (u8)((v & 0x7f) | 0x80);
      v >>= 7;
    }
    return 9;
  }    
  n = 0;
  do{
    buf[n++] = (u8)((v & 0x7f) | 0x80);
    v >>= 7;
  }while( v!=0 );
  buf[0] &= 0x7f;
  assert( n<=9 );
  for(i=0, j=n-1; j>=0; j--, i++){
    p[i] = buf[j];
  }
  return n;
}

int sqlite3PutVarint32(unsigned char *p, u32 v){
#ifndef putVarint32
  if( (v & ~0x7f)==0 ){
    p[0] = v;
    return 1;
  }
#endif
  if( (v & ~0x3fff)==0 ){
    p[0] = (u8)((v>>7) | 0x80);
    p[1] = (u8)(v & 0x7f);
    return 2;
  }
  return sqlite3PutVarint(p, v);
}

#define SLOT_2_0     0x001fc07f
#define SLOT_4_2_0   0xf01fc07f


u8 sqlite3GetVarint(const unsigned char *p, u64 *v){
  u32 a,b,s;

  a = *p;
  
  if (!(a&0x80))
  {
    *v = a;
    return 1;
  }

  p++;
  b = *p;
  
  if (!(b&0x80))
  {
    a &= 0x7f;
    a = a<<7;
    a |= b;
    *v = a;
    return 2;
  }

  
  assert( SLOT_2_0 == ((0x7f<<14) | (0x7f)) );
  assert( SLOT_4_2_0 == ((0xfU<<28) | (0x7f<<14) | (0x7f)) );

  p++;
  a = a<<14;
  a |= *p;
  
  if (!(a&0x80))
  {
    a &= SLOT_2_0;
    b &= 0x7f;
    b = b<<7;
    a |= b;
    *v = a;
    return 3;
  }

  
  a &= SLOT_2_0;
  p++;
  b = b<<14;
  b |= *p;
  
  if (!(b&0x80))
  {
    b &= SLOT_2_0;
    
    
    a = a<<7;
    a |= b;
    *v = a;
    return 4;
  }

  
  
  
  
  
  b &= SLOT_2_0;
  s = a;
  

  p++;
  a = a<<14;
  a |= *p;
  
  if (!(a&0x80))
  {
    
    
    
    b = b<<7;
    a |= b;
    s = s>>18;
    *v = ((u64)s)<<32 | a;
    return 5;
  }

  
  s = s<<7;
  s |= b;
  

  p++;
  b = b<<14;
  b |= *p;
  
  if (!(b&0x80))
  {
    
    
    a &= SLOT_2_0;
    a = a<<7;
    a |= b;
    s = s>>18;
    *v = ((u64)s)<<32 | a;
    return 6;
  }

  p++;
  a = a<<14;
  a |= *p;
  
  if (!(a&0x80))
  {
    a &= SLOT_4_2_0;
    b &= SLOT_2_0;
    b = b<<7;
    a |= b;
    s = s>>11;
    *v = ((u64)s)<<32 | a;
    return 7;
  }

  
  a &= SLOT_2_0;
  p++;
  b = b<<14;
  b |= *p;
  
  if (!(b&0x80))
  {
    b &= SLOT_4_2_0;
    
    
    a = a<<7;
    a |= b;
    s = s>>4;
    *v = ((u64)s)<<32 | a;
    return 8;
  }

  p++;
  a = a<<15;
  a |= *p;
  

  
  
  b &= SLOT_2_0;
  b = b<<8;
  a |= b;

  s = s<<4;
  b = p[-4];
  b &= 0x7f;
  b = b>>3;
  s |= b;

  *v = ((u64)s)<<32 | a;

  return 9;
}

u8 sqlite3GetVarint32(const unsigned char *p, u32 *v){
  u32 a,b;

  a = *p;
  
#ifndef getVarint32
  if (!(a&0x80))
  {
    
    *v = a;
    return 1;
  }
#endif

  
  p++;
  b = *p;
  
  if (!(b&0x80))
  {
    
    a &= 0x7f;
    a = a<<7;
    *v = a | b;
    return 2;
  }

  
  p++;
  a = a<<14;
  a |= *p;
  
  if (!(a&0x80))
  {
    
    a &= (0x7f<<14)|(0x7f);
    b &= 0x7f;
    b = b<<7;
    *v = a | b;
    return 3;
  }

#if 1
  {
    u64 v64;
    u8 n;

    p -= 2;
    n = sqlite3GetVarint(p, &v64);
    assert( n>3 && n<=9 );
    if( (v64 & SQLITE_MAX_U32)!=v64 ){
      *v = 0xffffffff;
    }else{
      *v = (u32)v64;
    }
    return n;
  }

#else
  p++;
  b = b<<14;
  b |= *p;
  
  if (!(b&0x80))
  {
    
    b &= (0x7f<<14)|(0x7f);
    a &= (0x7f<<14)|(0x7f);
    a = a<<7;
    *v = a | b;
    return 4;
  }

  p++;
  a = a<<14;
  a |= *p;
  
  if (!(a&0x80))
  {
    
    a &= SLOT_4_2_0;
    b &= SLOT_4_2_0;
    b = b<<7;
    *v = a | b;
    return 5;
  }

  {
    u64 v64;
    u8 n;

    p -= 4;
    n = sqlite3GetVarint(p, &v64);
    assert( n>5 && n<=9 );
    *v = (u32)v64;
    return n;
  }
#endif
}

int sqlite3VarintLen(u64 v){
  int i = 0;
  do{
    i++;
    v >>= 7;
  }while( v!=0 && ALWAYS(i<9) );
  return i;
}


u32 sqlite3Get4byte(const u8 *p){
  return (p[0]<<24) | (p[1]<<16) | (p[2]<<8) | p[3];
}
void sqlite3Put4byte(unsigned char *p, u32 v){
  p[0] = (u8)(v>>24);
  p[1] = (u8)(v>>16);
  p[2] = (u8)(v>>8);
  p[3] = (u8)v;
}



#if !defined(SQLITE_OMIT_BLOB_LITERAL) || defined(SQLITE_HAS_CODEC)
static u8 hexToInt(int h){
  assert( (h>='0' && h<='9') ||  (h>='a' && h<='f') ||  (h>='A' && h<='F') );
#ifdef SQLITE_ASCII
  h += 9*(1&(h>>6));
#endif
#ifdef SQLITE_EBCDIC
  h += 9*(1&~(h>>4));
#endif
  return (u8)(h & 0xf);
}
#endif 

#if !defined(SQLITE_OMIT_BLOB_LITERAL) || defined(SQLITE_HAS_CODEC)
void *sqlite3HexToBlob(sqlite3 *db, const char *z, int n){
  char *zBlob;
  int i;

  zBlob = (char *)sqlite3DbMallocRaw(db, n/2 + 1);
  n--;
  if( zBlob ){
    for(i=0; i<n; i+=2){
      zBlob[i/2] = (hexToInt(z[i])<<4) | hexToInt(z[i+1]);
    }
    zBlob[i/2] = 0;
  }
  return zBlob;
}
#endif 

static void logBadConnection(const char *zType){
  sqlite3_log(SQLITE_MISUSE, 
     "API call with %s database connection pointer",
     zType
  );
}

int sqlite3SafetyCheckOk(sqlite3 *db){
  u32 magic;
  if( db==0 ){
    logBadConnection("NULL");
    return 0;
  }
  magic = db->magic;
  if( magic!=SQLITE_MAGIC_OPEN ){
    if( sqlite3SafetyCheckSickOrOk(db) ){
      testcase( sqlite3GlobalConfig.xLog!=0 );
      logBadConnection("unopened");
    }
    return 0;
  }else{
    return 1;
  }
}
int sqlite3SafetyCheckSickOrOk(sqlite3 *db){
  u32 magic;
  magic = db->magic;
  if( magic!=SQLITE_MAGIC_SICK &&
      magic!=SQLITE_MAGIC_OPEN &&
      magic!=SQLITE_MAGIC_BUSY ){
    testcase( sqlite3GlobalConfig.xLog!=0 );
    logBadConnection("invalid");
    return 0;
  }else{
    return 1;
  }
}

int sqlite3AddInt64(i64 *pA, i64 iB){
  i64 iA = *pA;
  testcase( iA==0 ); testcase( iA==1 );
  testcase( iB==-1 ); testcase( iB==0 );
  if( iB>=0 ){
    testcase( iA>0 && LARGEST_INT64 - iA == iB );
    testcase( iA>0 && LARGEST_INT64 - iA == iB - 1 );
    if( iA>0 && LARGEST_INT64 - iA < iB ) return 1;
    *pA += iB;
  }else{
    testcase( iA<0 && -(iA + LARGEST_INT64) == iB + 1 );
    testcase( iA<0 && -(iA + LARGEST_INT64) == iB + 2 );
    if( iA<0 && -(iA + LARGEST_INT64) > iB + 1 ) return 1;
    *pA += iB;
  }
  return 0; 
}
int sqlite3SubInt64(i64 *pA, i64 iB){
  testcase( iB==SMALLEST_INT64+1 );
  if( iB==SMALLEST_INT64 ){
    testcase( (*pA)==(-1) ); testcase( (*pA)==0 );
    if( (*pA)>=0 ) return 1;
    *pA -= iB;
    return 0;
  }else{
    return sqlite3AddInt64(pA, -iB);
  }
}
#define TWOPOWER32 (((i64)1)<<32)
#define TWOPOWER31 (((i64)1)<<31)
int sqlite3MulInt64(i64 *pA, i64 iB){
  i64 iA = *pA;
  i64 iA1, iA0, iB1, iB0, r;

  iA1 = iA/TWOPOWER32;
  iA0 = iA % TWOPOWER32;
  iB1 = iB/TWOPOWER32;
  iB0 = iB % TWOPOWER32;
  if( iA1*iB1 != 0 ) return 1;
  assert( iA1*iB0==0 || iA0*iB1==0 );
  r = iA1*iB0 + iA0*iB1;
  testcase( r==(-TWOPOWER31)-1 );
  testcase( r==(-TWOPOWER31) );
  testcase( r==TWOPOWER31 );
  testcase( r==TWOPOWER31-1 );
  if( r<(-TWOPOWER31) || r>=TWOPOWER31 ) return 1;
  r *= TWOPOWER32;
  if( sqlite3AddInt64(&r, iA0*iB0) ) return 1;
  *pA = r;
  return 0;
}

int sqlite3AbsInt32(int x){
  if( x>=0 ) return x;
  if( x==(int)0x80000000 ) return 0x7fffffff;
  return -x;
}
