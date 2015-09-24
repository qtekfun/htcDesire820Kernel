//  Copyright (C) 2009 Mozilla Foundation
//  Copyright (C) 1998-2007 Marti Maria
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// The above copyright notice and this permission notice shall be included in 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 

#ifndef _QCMS_MATRIX_H
#define _QCMS_MATRIX_H

struct vector {
        float v[3];
};

struct vector matrix_eval(struct matrix mat, struct vector v);
float matrix_det(struct matrix mat);
struct matrix matrix_identity(void);
struct matrix matrix_multiply(struct matrix a, struct matrix b);
struct matrix matrix_invert(struct matrix mat);

struct matrix matrix_invalid(void);

#endif
