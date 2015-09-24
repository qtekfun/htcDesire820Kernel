/*-
 * Copyright 2003-2005 Colin Percival
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions 
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#if 0
__FBSDID("$FreeBSD: src/usr.bin/bsdiff/bspatch/bspatch.c,v 1.1 2005/08/06 01:59:06 cperciva Exp $");
#endif

#include <sys/types.h>

#include <bzlib.h>
#include <err.h>
#include <fcntl.h>
#include <lzma.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>

#if defined(__APPLE__)
#include <libkern/OSByteOrder.h>
#define le64toh(x) OSSwapLittleToHostInt64(x)
#elif defined(__linux__)
#include <endian.h>
#elif defined(_WIN32) && (defined(_M_IX86) || defined(_M_X64))
#define le64toh(x) (x)
#else
#error Provide le64toh for this platform
#endif

#include "chrome/installer/mac/third_party/bsdiff/sha1_adapter.h"

static inline off_t offtin(u_char *buf)
{
	return le64toh(*((off_t*)buf));
}

static void sha1tostr(const u_char *sha1, char *sha1str)
{
	int i;
	for (i = 0; i < SHA1_DIGEST_LENGTH; ++i)
		sprintf(&sha1str[i * 2], "%02x", sha1[i]);
}


typedef struct {
	
	u_char in[BUFSIZ];
	u_char out[BUFSIZ];

	lzma_stream ls;
	FILE *f;

	u_char *read_out;
	size_t read_out_len;

	
	lzma_ret err;
	int eof;
} xzfile;

static xzfile *xzdopen(FILE *f, lzma_ret *err)
{
	xzfile *xzf;
	lzma_stream ls = LZMA_STREAM_INIT;
	uint64_t physmem, memlimit;

	if (!(xzf = malloc(sizeof(xzfile)))) {
		if (err) *err = LZMA_MEM_ERROR;
		return NULL;
	}

	xzf->ls = ls;
	xzf->f = f;

	xzf->read_out = xzf->out;
	xzf->read_out_len = 0;

	xzf->err = LZMA_OK;
	xzf->eof = 0;

	physmem = lzma_physmem();
	if (physmem == 0)
		physmem = 128 * 1024 * 1024;
	memlimit = 40 * physmem / 100;
	if (memlimit < 80 * 1024 * 1024) {
		memlimit = 80 * physmem / 100;
		if (memlimit > 80 * 1024 * 1024)
			memlimit = 80 * 1024 * 1024;
	}

	xzf->err = lzma_stream_decoder(&xzf->ls, memlimit,
	                               LZMA_TELL_NO_CHECK |
	                                   LZMA_TELL_UNSUPPORTED_CHECK);
	if (xzf->err != LZMA_OK) {
		if (err) *err = xzf->err;
		free(xzf);
		return NULL;
	}

	if (err) *err = xzf->err;
	return xzf;
}

static lzma_ret xzclose(xzfile *xzf)
{
	lzma_ret lzma_err = LZMA_OK;

	lzma_end(&xzf->ls);
	if (fclose(xzf->f) != 0)
		lzma_err = LZMA_STREAM_END;
	free(xzf);

	return lzma_err;
}

static size_t xzread(xzfile *xzf, u_char *buf, size_t len, lzma_ret *err)
{
	lzma_action action = LZMA_RUN;
	size_t copylen;
	size_t nread = 0;

	while (xzf->err == LZMA_OK && len > 0) {
		if (xzf->read_out_len == 0) {
			if (xzf->ls.avail_in == 0 && !xzf->eof) {
				
				xzf->ls.next_in = xzf->in;
				xzf->ls.avail_in = fread(xzf->in, 1, BUFSIZ,
				                         xzf->f);
				if (ferror(xzf->f)) {
					
					xzf->err = LZMA_STREAM_END;
					if (err) *err = xzf->err;
					return 0;
				} else if (feof(xzf->f)) {
					xzf->eof = 1;
				}
			}

			
			xzf->ls.next_out = xzf->out;
			xzf->ls.avail_out = BUFSIZ;

			
			if (xzf->ls.avail_in == 0) {
				xzf->err = LZMA_BUF_ERROR;
				if (err) *err = xzf->err;
				return 0;
			}

			if (xzf->eof)
				action = LZMA_FINISH;

			
			xzf->err = lzma_code(&xzf->ls, action);
			if (xzf->err == LZMA_STREAM_END) {
				xzf->eof = 1;
				xzf->err = LZMA_OK;
			} else if (xzf->err != LZMA_OK) {
				if (err) *err = xzf->err;
				return 0;
			}

			xzf->read_out = xzf->out;
			xzf->read_out_len = BUFSIZ - xzf->ls.avail_out;
		}

		copylen = xzf->read_out_len;
		if (copylen > len)
			copylen = len;
		memcpy(buf, xzf->read_out, copylen);
		nread += copylen;
		buf += copylen;
		len -= copylen;
		xzf->read_out += copylen;
		xzf->read_out_len -= copylen;
	}

	if (err) *err = xzf->err;
	return nread;
}


typedef struct {
	FILE *f;              
	union {
		BZFILE *bz2;  
		gzFile gz;    
		xzfile *xz;   
	} u;
	const char *tag;
	unsigned char method;
} cfile;

static void cfopen(cfile *cf, const char *path, off_t off,
                   const char *tag, unsigned char method)
{
	int fd;
	int bz2_err, gz_err;
	lzma_ret lzma_err;

	if (method == 1 || method == 2 || method == 4) {
		if ((cf->f = fopen(path, "rb")) == NULL)
			err(1, "fdopen(%s)", tag);
		if ((fseeko(cf->f, off, SEEK_SET)) != 0)
			err(1, "fseeko(%s, %lld)", tag, off);
		if (method == 2) {
			if ((cf->u.bz2 = BZ2_bzReadOpen(&bz2_err, cf->f, 0, 0,
			                                NULL, 0)) == NULL)
				errx(1, "BZ2_bzReadOpen(%s): %d", tag, bz2_err);
		} else if (method == 4) {
			if ((cf->u.xz = xzdopen(cf->f, &lzma_err)) == NULL)
				errx(1, "xzdopen(%s): %d", tag, lzma_err);
			cf->f = NULL;
		}
	} else if (method == 3) {
		if ((fd = open(path, O_RDONLY)) < 0)
			err(1, "open(%s)", tag);
		if (lseek(fd, off, SEEK_SET) != off)
			err(1, "lseek(%s, %lld)", tag, off);
		if ((cf->u.gz = gzdopen(fd, "rb")) == NULL)
			errx(1, "gzdopen(%s)", tag);
	} else {
		errx(1, "cfopen(%s): unknown method %d", tag, method);
	}

	cf->tag = tag;
	cf->method = method;
}

static void cfclose(cfile *cf)
{
	int bz2_err, gz_err;
	lzma_ret lzma_err;

	if (cf->method == 1 || cf->method == 2) {
		if (cf->method == 2) {
			bz2_err = BZ_OK;
			BZ2_bzReadClose(&bz2_err, cf->u.bz2);
			if (bz2_err != BZ_OK)
				errx(1, "BZ2_bzReadClose(%s): %d\n",
				     cf->tag, bz2_err);
		}
		if (fclose(cf->f) != 0)
			err(1, "fclose(%s)", cf->tag);
	} else if (cf->method == 3) {
		if ((gz_err = gzclose(cf->u.gz)) != Z_OK)
			errx(1, "gzclose(%s): %d", cf->tag, gz_err);
	} else if (cf->method == 4) {
		if ((lzma_err = xzclose(cf->u.xz)) != LZMA_OK)
			errx(1, "xzclose(%s): %d", cf->tag, lzma_err);
	} else {
		errx(1, "cfclose(%s): unknown method %d", cf->tag, cf->method);
	}
}

static void cfread(cfile *cf, u_char *buf, size_t len)
{
	size_t nread;
	int bz2_err, gz_err;
	lzma_ret lzma_err;

	if (cf->method == 1) {
		if ((nread = fread(buf, 1, len, cf->f)) != len) {
			if (!ferror(cf->f))
				errx(1, "fread(%s, %zd): short read %zd",
				     cf->tag, len, nread);
			err(1, "fread(%s, %zd)", cf->tag, len);
		}
	} else if (cf->method == 2) {
		bz2_err = BZ_OK;
		if ((nread = BZ2_bzRead(&bz2_err, cf->u.bz2, buf, len)) !=
		    len) {
			if (bz2_err == BZ_OK)
				errx(1, "BZ2_bzRead(%s, %zd): short read %zd",
				     cf->tag, len, nread);
			errx(1, "BZ2_bzRead(%s, %zd): %d",
			     cf->tag, len, bz2_err);
		}
	} else if (cf->method == 3) {
		if ((nread = gzread(cf->u.gz, buf, len)) != len) {
			gz_err = Z_OK;
			gzerror(cf->u.gz, &gz_err);
			if (gz_err == Z_OK)
				errx(1, "gzread(%s, %zd): short read %zd",
				     cf->tag, len, nread);
			errx(1, "gzread(%s, %zd): %d", cf->tag, len, gz_err);
		}
	} else if (cf->method == 4) {
		if ((nread = xzread(cf->u.xz, buf, len, &lzma_err)) != len) {
			if (lzma_err == LZMA_OK)
				errx(1, "xzread(%s, %zd): short read %zd",
				     cf->tag, len, nread);
			errx(1, "xzread(%s, %zd): %d", cf->tag, len, lzma_err);
		}
	} else {
		errx(1, "cfread(%s, %zd): unknown method %d",
		     cf->tag, len, cf->method);
	}
}

int main(int argc,char * argv[])
{
	FILE * f;
	cfile cf, df, ef;
	int fd;
	off_t expect_oldsize, oldsize, newsize, patchsize;
	off_t zctrllen, zdatalen, zextralen;
	u_char header[96], buf[8];
	u_char *old, *new;
	off_t oldpos,newpos;
	off_t ctrl[3];
	off_t i;
	u_char sha1[SHA1_DIGEST_LENGTH];
	char sha1str[SHA1_DIGEST_LENGTH * 2 + 1];
	char expected_sha1str[SHA1_DIGEST_LENGTH * 2 + 1];

	if(argc!=4) errx(1,"usage: %s oldfile newfile patchfile",argv[0]);

	
	if ((f = fopen(argv[3], "rb")) == NULL)
		err(1, "fopen(%s)", argv[3]);


	
	if (fread(header, 1, sizeof(header), f) < sizeof(header)) {
		if (feof(f))
			errx(1, "corrupt patch (header size)");
		err(1, "fread(%s)", argv[3]);
	}

	
	if (memcmp(header, "BSDIFF4G", 8) != 0)
		errx(1, "corrupt patch (magic)");

	
	zctrllen = offtin(header + 8);
	zdatalen = offtin(header + 16);
	zextralen = offtin(header + 24);
	expect_oldsize = offtin(header + 32);
	newsize = offtin(header + 40);
	if (zctrllen < 0 || zdatalen < 0 || zextralen < 0)
		errx(1, "corrupt patch (stream sizes)");
	if (expect_oldsize < 0 || newsize < 0)
		errx(1, "corrupt patch (file sizes)");

	if (fseeko(f, 0, SEEK_END) != 0 || (patchsize = ftello(f)) < 0)
		err(1, "fseeko/ftello(%s)", argv[3]);
	if (patchsize != sizeof(header) + zctrllen + zdatalen + zextralen)
		errx(1, "corrupt patch (patch size)");

	cfopen(&cf, argv[3], sizeof(header), "control", header[88]);
	cfopen(&df, argv[3], sizeof(header) + zctrllen, "diff", header[89]);
	cfopen(&ef, argv[3], sizeof(header) + zctrllen + zdatalen, "extra",
	       header[90]);

	if (fclose(f))
		err(1, "fclose(%s)", argv[3]);

	if(((fd=open(argv[1],O_RDONLY,0))<0) ||
		((oldsize=lseek(fd,0,SEEK_END))==-1) ||
		((old=malloc(oldsize+1))==NULL) ||
		(lseek(fd,0,SEEK_SET)!=0) ||
		(read(fd,old,oldsize)!=oldsize) ||
		(close(fd)==-1)) err(1,"%s",argv[1]);
	if (expect_oldsize != oldsize)
		errx(1, "old size mismatch: %lld != %lld",
		     oldsize, expect_oldsize);
	SHA1(old, oldsize, sha1);
	if (memcmp(sha1, header + 48, sizeof(sha1)) != 0) {
		sha1tostr(sha1, sha1str);
		sha1tostr(header + 48, expected_sha1str);
		errx(1, "old hash mismatch: %s != %s",
	             sha1str, expected_sha1str);
	}
	if((new=malloc(newsize+1))==NULL) err(1,NULL);

	oldpos=0;newpos=0;
	while(newpos<newsize) {
		
		for(i=0;i<=2;i++) {
			cfread(&cf, buf, 8);
			ctrl[i]=offtin(buf);
		};

		
		if(newpos+ctrl[0]>newsize)
			errx(1,"corrupt patch (diff): overrun");

		
		cfread(&df, new + newpos, ctrl[0]);

		
		for(i=0;i<ctrl[0];i++)
			if((oldpos+i>=0) && (oldpos+i<oldsize))
				new[newpos+i]+=old[oldpos+i];

		
		newpos+=ctrl[0];
		oldpos+=ctrl[0];

		
		if(newpos+ctrl[1]>newsize)
			errx(1,"corrupt patch (extra): overrun");

		
		cfread(&ef, new + newpos, ctrl[1]);

		
		newpos+=ctrl[1];
		oldpos+=ctrl[2];
	};

	
	cfclose(&cf);
	cfclose(&df);
	cfclose(&ef);

	SHA1(new, newsize, sha1);
	if (memcmp(sha1, header + 68, sizeof(sha1)) != 0) {
		sha1tostr(sha1, sha1str);
		sha1tostr(header + 68, expected_sha1str);
		errx(1, "new hash mismatch: %s != %s",
		     sha1str, expected_sha1str);
	}

	
	if(((fd=open(argv[2],O_CREAT|O_TRUNC|O_WRONLY,0644))<0) ||
		(write(fd,new,newsize)!=newsize) || (close(fd)==-1))
		err(1,"open/write/close(%s)",argv[2]);

	free(new);
	free(old);

	return 0;
}
