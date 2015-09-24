#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


static int pagesize = 1024;     
static int fd = -1;             
static int mxFrame = 0;         
static int perLine = 16;        

typedef long long int i64;      


static int decodeVarint(const unsigned char *z, i64 *pVal){
  i64 v = 0;
  int i;
  for(i=0; i<8; i++){
    v = (v<<7) + (z[i]&0x7f);
    if( (z[i]&0x80)==0 ){ *pVal = v; return i+1; }
  }
  v = (v<<8) + (z[i]&0xff);
  *pVal = v;
  return 9;
}

static void out_of_memory(void){
  fprintf(stderr,"Out of memory...\n");
  exit(1);
}

static unsigned char *getContent(int ofst, int nByte){
  unsigned char *aData;
  aData = malloc(nByte);
  if( aData==0 ) out_of_memory();
  lseek(fd, ofst, SEEK_SET);
  read(fd, aData, nByte);
  return aData;
}

static void print_byte_range(
  int ofst,              
  int nByte,             
  unsigned char *aData,  
  int printOfst          
){
  int i, j;
  const char *zOfstFmt;

  if( ((printOfst+nByte)&~0xfff)==0 ){
    zOfstFmt = " %03x: ";
  }else if( ((printOfst+nByte)&~0xffff)==0 ){
    zOfstFmt = " %04x: ";
  }else if( ((printOfst+nByte)&~0xfffff)==0 ){
    zOfstFmt = " %05x: ";
  }else if( ((printOfst+nByte)&~0xffffff)==0 ){
    zOfstFmt = " %06x: ";
  }else{
    zOfstFmt = " %08x: ";
  }

  for(i=0; i<nByte; i += perLine){
    fprintf(stdout, zOfstFmt, i+printOfst);
    for(j=0; j<perLine; j++){
      if( i+j>nByte ){
        fprintf(stdout, "   ");
      }else{
        fprintf(stdout,"%02x ", aData[i+j]);
      }
    }
    for(j=0; j<perLine; j++){
      if( i+j>nByte ){
        fprintf(stdout, " ");
      }else{
        fprintf(stdout,"%c", isprint(aData[i+j]) ? aData[i+j] : '.');
      }
    }
    fprintf(stdout,"\n");
  }
}

static void print_decode_line(
  unsigned char *aData,      
  int ofst, int nByte,       
  int asHex,                 
  const char *zMsg           
){
  int i, j;
  int val = aData[ofst];
  char zBuf[100];
  sprintf(zBuf, " %03x: %02x", ofst, aData[ofst]);
  i = strlen(zBuf);
  for(j=1; j<4; j++){
    if( j>=nByte ){
      sprintf(&zBuf[i], "   ");
    }else{
      sprintf(&zBuf[i], " %02x", aData[ofst+j]);
      val = val*256 + aData[ofst+j];
    }
    i += strlen(&zBuf[i]);
  }
  if( asHex ){
    sprintf(&zBuf[i], "  0x%08x", val);
  }else{
    sprintf(&zBuf[i], "   %9d", val);
  }
  printf("%s  %s\n", zBuf, zMsg);
}

static void print_frame(int iFrame){
  int iStart;
  unsigned char *aData;
  iStart = 32 + (iFrame-1)*(pagesize+24);
  fprintf(stdout, "Frame %d:   (offsets 0x%x..0x%x)\n",
          iFrame, iStart, iStart+pagesize+24);
  aData = getContent(iStart, pagesize+24);
  print_decode_line(aData, 0, 4, 0, "Page number");
  print_decode_line(aData, 4, 4, 0, "DB size, or 0 for non-commit");
  print_decode_line(aData, 8, 4, 1, "Salt-1");
  print_decode_line(aData,12, 4, 1, "Salt-2");
  print_decode_line(aData,16, 4, 1, "Checksum-1");
  print_decode_line(aData,20, 4, 1, "Checksum-2");
  print_byte_range(iStart+24, pagesize, aData+24, 0);
  free(aData);
}

static unsigned int getInt32(const unsigned char *a){
  unsigned int x = (a[0]<<24) + (a[1]<<16) + (a[2]<<8) + a[3];
  return x;
}

static void print_oneline_frame(int iFrame){
  int iStart;
  unsigned char *aData;
  iStart = 32 + (iFrame-1)*(pagesize+24);
  aData = getContent(iStart, 24);
  fprintf(stdout, "Frame %4d: %6d %6d 0x%08x 0x%08x 0x%08x 0x%08x\n",
          iFrame, 
          getInt32(aData),
          getInt32(aData+4),
          getInt32(aData+8),
          getInt32(aData+12),
          getInt32(aData+16),
          getInt32(aData+20)
  );
  free(aData);
}

static void print_wal_header(void){
  unsigned char *aData;
  aData = getContent(0, 32);
  printf("WAL Header:\n");
  print_decode_line(aData, 0, 4,1,"Magic.  0x377f0682 (le) or 0x377f0683 (be)");
  print_decode_line(aData, 4, 4, 0, "File format");
  print_decode_line(aData, 8, 4, 0, "Database page size");
  print_decode_line(aData, 12,4, 0, "Checkpoint sequence number");
  print_decode_line(aData, 16,4, 1, "Salt-1");
  print_decode_line(aData, 20,4, 1, "Salt-2");
  print_decode_line(aData, 24,4, 1, "Checksum-1");
  print_decode_line(aData, 28,4, 1, "Checksum-2");
  free(aData);
}

static int describeCell(unsigned char cType, unsigned char *a, char **pzDesc){
  int i;
  int nDesc = 0;
  int n = 0;
  int leftChild;
  i64 nPayload;
  i64 rowid;
  static char zDesc[100];
  i = 0;
  if( cType<=5 ){
    leftChild = ((a[0]*256 + a[1])*256 + a[2])*256 + a[3];
    a += 4;
    n += 4;
    sprintf(zDesc, "left-child: %d ", leftChild);
    nDesc = strlen(zDesc);
  }
  if( cType!=5 ){
    i = decodeVarint(a, &nPayload);
    a += i;
    n += i;
    sprintf(&zDesc[nDesc], "sz: %lld ", nPayload);
    nDesc += strlen(&zDesc[nDesc]);
  }
  if( cType==5 || cType==13 ){
    i = decodeVarint(a, &rowid);
    a += i;
    n += i;
    sprintf(&zDesc[nDesc], "rowid: %lld ", rowid);
    nDesc += strlen(&zDesc[nDesc]);
  }
  *pzDesc = zDesc;
  return n;
}

static void decode_btree_page(unsigned char *a, int pgno, int hdrSize){
  const char *zType = "unknown";
  int nCell;
  int i;
  int iCellPtr;
  switch( a[0] ){
    case 2:  zType = "index interior node";  break;
    case 5:  zType = "table interior node";  break;
    case 10: zType = "index leaf";           break;
    case 13: zType = "table leaf";           break;
  }
  printf("Decode of btree page %d:\n", pgno);
  print_decode_line(a, 0, 1, 0, zType);
  print_decode_line(a, 1, 2, 0, "Offset to first freeblock");
  print_decode_line(a, 3, 2, 0, "Number of cells on this page");
  nCell = a[3]*256 + a[4];
  print_decode_line(a, 5, 2, 0, "Offset to cell content area");
  print_decode_line(a, 7, 1, 0, "Fragmented byte count");
  if( a[0]==2 || a[0]==5 ){
    print_decode_line(a, 8, 4, 0, "Right child");
    iCellPtr = 12;
  }else{
    iCellPtr = 8;
  }
  for(i=0; i<nCell; i++){
    int cofst = iCellPtr + i*2;
    char *zDesc;
    cofst = a[cofst]*256 + a[cofst+1];
    describeCell(a[0], &a[cofst-hdrSize], &zDesc);
    printf(" %03x: cell[%d] %s\n", cofst, i, zDesc);
  }
}

int main(int argc, char **argv){
  struct stat sbuf;
  unsigned char zPgSz[2];
  if( argc<2 ){
    fprintf(stderr,"Usage: %s FILENAME ?PAGE? ...\n", argv[0]);
    exit(1);
  }
  fd = open(argv[1], O_RDONLY);
  if( fd<0 ){
    fprintf(stderr,"%s: can't open %s\n", argv[0], argv[1]);
    exit(1);
  }
  zPgSz[0] = 0;
  zPgSz[1] = 0;
  lseek(fd, 10, SEEK_SET);
  read(fd, zPgSz, 2);
  pagesize = zPgSz[0]*256 + zPgSz[1];
  if( pagesize==0 ) pagesize = 1024;
  printf("Pagesize: %d\n", pagesize);
  fstat(fd, &sbuf);
  if( sbuf.st_size<32 ){
    printf("file too small to be a WAL\n");
    return 0;
  }
  mxFrame = (sbuf.st_size - 32)/(pagesize + 24);
  printf("Available pages: 1..%d\n", mxFrame);
  if( argc==2 ){
    int i;
    print_wal_header();
    for(i=1; i<=mxFrame; i++) print_oneline_frame(i);
  }else{
    int i;
    for(i=2; i<argc; i++){
      int iStart, iEnd;
      char *zLeft;
      if( strcmp(argv[i], "header")==0 ){
        print_wal_header();
        continue;
      }
      if( !isdigit(argv[i][0]) ){
        fprintf(stderr, "%s: unknown option: [%s]\n", argv[0], argv[i]);
        continue;
      }
      iStart = strtol(argv[i], &zLeft, 0);
      if( zLeft && strcmp(zLeft,"..end")==0 ){
        iEnd = mxFrame;
      }else if( zLeft && zLeft[0]=='.' && zLeft[1]=='.' ){
        iEnd = strtol(&zLeft[2], 0, 0);
#if 0
      }else if( zLeft && zLeft[0]=='b' ){
        int ofst, nByte, hdrSize;
        unsigned char *a;
        if( iStart==1 ){
          ofst = hdrSize = 100;
          nByte = pagesize-100;
        }else{
          hdrSize = 0;
          ofst = (iStart-1)*pagesize;
          nByte = pagesize;
        }
        a = getContent(ofst, nByte);
        decode_btree_page(a, iStart, hdrSize);
        free(a);
        continue;
#endif
      }else{
        iEnd = iStart;
      }
      if( iStart<1 || iEnd<iStart || iEnd>mxFrame ){
        fprintf(stderr,
          "Page argument should be LOWER?..UPPER?.  Range 1 to %d\n",
          mxFrame);
        exit(1);
      }
      while( iStart<=iEnd ){
        print_frame(iStart);
        iStart++;
      }
    }
  }
  close(fd);
  return 0;
}
