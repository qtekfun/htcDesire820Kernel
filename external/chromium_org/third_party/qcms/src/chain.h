//  Copyright (C) 2009 Mozilla Foundation
//  Copyright (C) 1998-2007 Marti Maria
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// The above copyright notice and this permission notice shall be included in 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 

#ifndef _QCMS_CHAIN_H
#define _QCMS_CHAIN_H

float* qcms_chain_transform(qcms_profile *in, qcms_profile *out, float *src, float *dest, size_t lutSize);

#endif
