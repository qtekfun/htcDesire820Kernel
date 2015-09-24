// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ENCODINGS_PUBLIC_ENCODINGS_H_
#define ENCODINGS_PUBLIC_ENCODINGS_H_



#undef UNICODE


#ifndef SWIG

#include "encodings/proto/encodings.pb.h"


#else

#include "i18n/encodings/internal/encodings_proto_wrapper.h"

#endif

const int kNumEncodings = NUM_ENCODINGS;

#define LATIN1           ISO_8859_1
#define LATIN2           ISO_8859_2
#define LATIN3           ISO_8859_3
#define LATIN4           ISO_8859_4
#define CYRILLIC         ISO_8859_5
#define ARABIC_ENCODING  ISO_8859_6     
#define GREEK_ENCODING   ISO_8859_7     
#define HEBREW_ENCODING  ISO_8859_8     
#define LATIN5           ISO_8859_9
#define LATIN6           ISO_8859_10
#define KOREAN_HANGUL    KOREAN_EUC_KR

Encoding default_encoding();





bool IsValidEncoding(Encoding enc);

bool IsEncEncCompatible(const Encoding from, const Encoding to);


bool IsSupersetOfAscii7Bit(Encoding e);


bool Is8BitEncoding(Encoding e);

bool IsCJKEncoding(Encoding e);

bool IsHebrewEncoding(Encoding e);

bool IsRightToLeftEncoding(Encoding enc);

bool IsLogicalRightToLeftEncoding(Encoding enc);

bool IsVisualRightToLeftEncoding(Encoding enc);

bool IsIso2022Encoding(Encoding enc);

bool IsIso2022JpOrVariant(Encoding enc);

bool IsShiftJisOrVariant(Encoding enc);

bool IsJapaneseCellPhoneCarrierSpecificEncoding(Encoding enc);




const char* EncodingName(Encoding enc);

const char* MimeEncodingName(Encoding enc);


const int kMaxEncodingNameSize = 50;

const char* default_encoding_name();

const char* invalid_encoding_name();

bool EncodingFromName(const char* enc_name, Encoding *encoding);


Encoding EncodingNameAliasToEncoding(const char *enc_name);



Encoding PreferredWebOutputEncoding(Encoding enc);


void InitEncodings();

#endif  
