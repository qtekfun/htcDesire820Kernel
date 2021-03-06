// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef EXPERIMENTAL_USERS_DSITES_LANGDET_UTF8SCANNOTJUSTLETTERSPECIAL_H__
#define EXPERIMENTAL_USERS_DSITES_LANGDET_UTF8SCANNOTJUSTLETTERSPECIAL_H__

#include "encodings/compact_lang_det/win/cld_utf8statetable.h"

#define X__ (kExitIllegalStructure)
#define RJ_ (kExitReject)
#define S1_ (kExitReplace1)
#define S2_ (kExitReplace2)
#define S3_ (kExitReplace3)
#define S21 (kExitReplace21)
#define S31 (kExitReplace31)
#define S32 (kExitReplace32)
#define T1_ (kExitReplaceOffset1)
#define T2_ (kExitReplaceOffset2)
#define S11 (kExitReplace1S0)
#define SP_ (kExitSpecial)
#define D__ (kExitDoAgain)
#define RJA (kExitRejectAlt)


static const unsigned int utf8scannotjustletterspecial_STATE0 = 0;		
static const unsigned int utf8scannotjustletterspecial_STATE0_SIZE = 64;	
static const unsigned int utf8scannotjustletterspecial_TOTAL_SIZE = 11520;
static const unsigned int utf8scannotjustletterspecial_MAX_EXPAND_X4 = 0;
static const unsigned int utf8scannotjustletterspecial_SHIFT = 6;
static const unsigned int utf8scannotjustletterspecial_BYTES = 1;
static const unsigned int utf8scannotjustletterspecial_LOSUB = 0x27272727;
static const unsigned int utf8scannotjustletterspecial_HIADD = 0x44444444;

static const uint8 utf8scannotjustletterspecial[] = {
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,RJ_,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,RJ_,  0,RJ_,  0,

  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,

X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,
X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,
X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,
X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,

X__,X__,  6,  7,  8,  8,  8,  8,   8,  8,  8,  9,  8, 10, 11, 12,
  8,  8, 13,  8, 14, 15, 16, 17,  18, 19,  8, 20, 21, 22, 23, 24,
 25, 54, 92,106,113,115,115,115, 115,116,118,115,115,123,  2,125,
142,  4,  4,175,  5,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,

  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,

  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,
X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,
X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,
X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,RJ_,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,RJ_,  0,  0,   0,  0,RJ_,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,  0,  0,  0,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,

  0,  0,  0,  0,  0,  0,RJ_,  0, RJ_,RJ_,RJ_,  0,RJ_,  0,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,   0,RJ_,  0,  0,  0,  0,  0,  0,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,

  0,RJ_,RJ_,  0,RJ_,RJ_,  0,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,RJ_,RJ_,RJ_,  0,  0,RJ_,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,  0,RJ_,  0,  0,  0,  0,  0,

X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,
X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,
  3,  3,  3,  3, 26, 27, 28, 29,  30, 31, 32, 33, 34, 35, 36, 37,
 38, 39, 40, 41, 42, 43, 44, 45,  46, 47, 48, 49, 50, 51, 52, 53,

  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,

  0,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,RJ_,
RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,  0,RJ_,  0,  0,  0,RJ_,RJ_, RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,RJ_, RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,RJ_,   0,  0,  0,  0,RJ_,RJ_,  0,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,RJ_,
RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,  0,RJ_,RJ_,  0, RJ_,RJ_,  0,  0,RJ_,  0,RJ_,RJ_,

RJ_,RJ_,RJ_,  0,  0,  0,  0,RJ_, RJ_,  0,  0,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,RJ_,RJ_,RJ_,RJ_,  0,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,
RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_, RJ_,RJ_,  0,RJ_,RJ_,RJ_,  0,  0,
RJ_,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,RJ_,
RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,  0,  0,  0,RJ_, RJ_,  0,  0,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,RJ_,RJ_,   0,  0,  0,  0,RJ_,RJ_,  0,RJ_,
RJ_,RJ_,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,RJ_,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,RJ_,RJ_,  0,RJ_,  0,RJ_,RJ_,
  0,  0,  0,RJ_,RJ_,  0,  0,  0, RJ_,RJ_,RJ_,  0,  0,  0,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,RJ_,RJ_,

RJ_,RJ_,RJ_,  0,  0,  0,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,RJ_,RJ_,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,RJ_,RJ_,  0,   0,  0,  0,  0,  0,  0,RJ_,  0,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,  0,  0,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,   0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,RJ_,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,   0,  0,RJ_,  0,  0,  0,  0,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,  0,RJ_,  0,  0,RJ_, RJ_,  0,RJ_,  0,  0,RJ_,  0,  0,
  0,  0,  0,  0,RJ_,RJ_,RJ_,RJ_,   0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,RJ_,RJ_,RJ_,  0,RJ_,  0,RJ_,   0,  0,RJ_,RJ_,  0,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,RJ_,RJ_,RJ_,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, RJ_,RJ_,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,RJ_,  0,RJ_,   0,RJ_,  0,  0,  0,  0,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,

  0,  0,  0,  0,  0,  0,RJ_,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

 55, 56, 57, 58,  8, 59, 60, 61,   8, 62, 63, 64, 65, 66, 67, 68,
 69,  8,  8,  8,  8,  8,  8,  8,   8, 70, 71, 72, 73, 74, 75, 76,
 77, 78, 79,  3, 80, 81, 82, 83,  84,  3,  3,  3,  8, 85,  3,  3,
  3,  3,  3,  3,  8,  8,  8, 86,   8,  8, 87, 61, 88, 89, 90, 91,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,   0,RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,  0,  0,  0,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,RJ_,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,  0,  0,  0,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,

RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,RJ_,   0,  0,  0,  0,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,RJ_,  0,RJ_,  0,RJ_,  0,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,  0,

  0,  0,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,  0,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,
  0,  0,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,

  3, 93, 94, 95, 96, 97, 98,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
 99,100,  8,101,102,103,104,105,   3,  3,  3,  3,  3,  3,  3,  3,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,RJ_,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,RJ_,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,RJ_,  0,  0,  0,  0,RJ_,   0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,RJ_,  0,  0,   0,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,RJ_,  0,RJ_,  0, RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_,RJ_,

  0,  0,  0,  0,  0,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,
  0,  0,  0,  0,RJ_,RJ_,RJ_,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,  0,  0,  0,  0,  0,  0,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

107, 69,108,109,110,  8,111,112,   3,  3,  3,  3,  3,  3,  3,  3,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,

  0,  0,  0,  0,  0,RJ_,RJ_,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,  0,  0,RJ_,RJ_,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,   0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,

  0,  0,  0,  0,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,114,  3,   8,  8,  8,  8,  8,  8,  8,  8,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,

  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,117,  3,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,

  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,119,  3,  3,  3,  3,  3,   3,  3,  3,  3,120,  3,  3,  3,
121,122,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,124,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  8,  8,  8,  8, 126,127,  8,128,129,130, 23,131,
  8,  8,  8,  8,132, 21,133,134, 135,136,  8,137,138,139,140,141,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,   0,  0,  0,  0,  0,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,  0,

RJ_,RJ_,  0,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,  0,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,

  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,

  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,   0,  0,RJ_,RJ_,RJ_,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

X__,X__,X__,X__,X__,X__,X__,X__, X__,X__,X__,X__,X__,X__,X__,X__,
143,  2,155,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,157,  2,  2,
115,115,115,115,115,115,115,115, 115,115,172,  2,  2,  2,  2,174,
  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,

144,145,  8,146,  3,  3,  3,  3,   3,  3,  3,  3,147,148,149,150,
  8,  8,151,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
152,  3,  3,  3,153,  3,  3,  3, 154,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,RJ_,RJ_,  0,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_, RJ_,  0,  0,  0,RJ_,  0,  0,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,  0,   0,  0,  0,  0,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_,   0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0, RJ_,RJ_,RJ_,  0,  0,  0,  0,RJ_,

  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,156,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  3,  3,  3,  3,  3,158,159,  3,   3,160,  3,  3,  3,  3,  3,  3,
  8,161,162,163,164,165,  8,  8,   8,  8,166,167,168,169,170,171,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,

  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,RJ_,RJ_,RJ_, RJ_,RJ_,  0,  0,  0,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,  0,  0,  0,  0,  0,   0,  0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,RJ_,RJ_,RJ_,RJ_,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  0,  0,RJ_,RJ_,RJ_,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
  0,  0,RJ_,  0,  0,RJ_,RJ_,  0,   0,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,RJ_,  0,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_, RJ_,RJ_,RJ_,  0,  0,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,  0,

RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,  0,   0,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,  0, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,  0,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,  0,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,

RJ_,RJ_,RJ_,  0,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,  8,  8,  8,  8,  8,
  8,  8,  8,  8,  8,  8,  8,  8,   8,  8,  8,173,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  8,  8,  8,  8,  8,  8,  8,  8, 151,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,

  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,
176,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,
  2,  2,  2,  2,  2,  2,  2,  2,   2,  2,  2,  2,  2,  2,  2,  2,

  3,  3,  3,  3,  8,  8,  8,177,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  3,  3,  3,  3,  3,   3,  3,  3,  3,  3,  3,  3,  3,

RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_, RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,RJ_,
  0,  0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,  0,

};

static const RemapEntry utf8scannotjustletterspecial_remap_base[] = {
{0,0,0} };

static const unsigned char utf8scannotjustletterspecial_remap_string[] = {
0 };

static const unsigned char utf8scannotjustletterspecial_fast[256] = {
0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
0,0,0,0,0,0,1,0, 0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0, 0,0,0,0,1,0,1,0,

0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1, 1,1,1,0,0,0,0,0,
0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1, 1,1,1,0,0,0,0,0,

1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,

1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,

};

static const UTF8ScanObj utf8scannotjustletterspecial_obj = {
  utf8scannotjustletterspecial_STATE0,
  utf8scannotjustletterspecial_STATE0_SIZE,
  utf8scannotjustletterspecial_TOTAL_SIZE,
  utf8scannotjustletterspecial_MAX_EXPAND_X4,
  utf8scannotjustletterspecial_SHIFT,
  utf8scannotjustletterspecial_BYTES,
  utf8scannotjustletterspecial_LOSUB,
  utf8scannotjustletterspecial_HIADD,
  utf8scannotjustletterspecial,
  utf8scannotjustletterspecial_remap_base,
  utf8scannotjustletterspecial_remap_string,
  utf8scannotjustletterspecial_fast
};


#undef X__
#undef RJ_
#undef S1_
#undef S2_
#undef S3_
#undef S21
#undef S31
#undef S32
#undef T1_
#undef T2_
#undef S11
#undef SP_
#undef D__
#undef RJA


#endif  
