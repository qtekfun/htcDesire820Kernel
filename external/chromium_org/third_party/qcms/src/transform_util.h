//  Copyright (C) 2009 Mozilla Foundation
//  Copyright (C) 1998-2007 Marti Maria
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// The above copyright notice and this permission notice shall be included in 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 

#ifndef _QCMS_TRANSFORM_UTIL_H
#define _QCMS_TRANSFORM_UTIL_H

#include <stdlib.h>

#define CLU(table,x,y,z) table[(x*len + y*x_len + z*xy_len)*3]

typedef uint16_t uint16_fract_t;

float lut_interp_linear(double value, uint16_t *table, size_t length);
float lut_interp_linear_float(float value, float *table, size_t length);
uint16_t lut_interp_linear16(uint16_t input_value, uint16_t *table, size_t length);


static inline float lerp(float a, float b, float t)
{
        return a*(1.f-t) + b*t;
}

unsigned char clamp_u8(float v);
float clamp_float(float a);

float u8Fixed8Number_to_float(uint16_t x);


float *build_input_gamma_table(struct curveType *TRC);
struct matrix build_colorant_matrix(qcms_profile *p);
void build_output_lut(struct curveType *trc,
                      uint16_t **output_gamma_lut, size_t *output_gamma_lut_length);

struct matrix matrix_invert(struct matrix mat);
qcms_bool compute_precache(struct curveType *trc, uint8_t *output);


#endif
