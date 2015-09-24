//  Copyright (C) 2009 Mozilla Corporation
//  Copyright (C) 1998-2007 Marti Maria
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// The above copyright notice and this permission notice shall be included in 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h> 
#include "qcmsint.h"
#include "chain.h"
#include "matrix.h"
#include "transform_util.h"

#if defined(_M_IX86) || defined(__i386__) || defined(__i386) || defined(_M_AMD64) || defined(__x86_64__) || defined(__x86_64)
#define X86
#endif 

static struct matrix build_RGB_to_XYZ_transfer_matrix(qcms_CIE_xyY white, qcms_CIE_xyYTRIPLE primrs)
{
	struct matrix primaries;
	struct matrix primaries_invert;
	struct matrix result;
	struct vector white_point;
	struct vector coefs;

	double xn, yn;
	double xr, yr;
	double xg, yg;
	double xb, yb;

	xn = white.x;
	yn = white.y;

	if (yn == 0.0)
		return matrix_invalid();

	xr = primrs.red.x;
	yr = primrs.red.y;
	xg = primrs.green.x;
	yg = primrs.green.y;
	xb = primrs.blue.x;
	yb = primrs.blue.y;

	primaries.m[0][0] = xr;
	primaries.m[0][1] = xg;
	primaries.m[0][2] = xb;

	primaries.m[1][0] = yr;
	primaries.m[1][1] = yg;
	primaries.m[1][2] = yb;

	primaries.m[2][0] = 1 - xr - yr;
	primaries.m[2][1] = 1 - xg - yg;
	primaries.m[2][2] = 1 - xb - yb;
	primaries.invalid = false;

	white_point.v[0] = xn/yn;
	white_point.v[1] = 1.;
	white_point.v[2] = (1.0-xn-yn)/yn;

	primaries_invert = matrix_invert(primaries);

	coefs = matrix_eval(primaries_invert, white_point);

	result.m[0][0] = coefs.v[0]*xr;
	result.m[0][1] = coefs.v[1]*xg;
	result.m[0][2] = coefs.v[2]*xb;

	result.m[1][0] = coefs.v[0]*yr;
	result.m[1][1] = coefs.v[1]*yg;
	result.m[1][2] = coefs.v[2]*yb;

	result.m[2][0] = coefs.v[0]*(1.-xr-yr);
	result.m[2][1] = coefs.v[1]*(1.-xg-yg);
	result.m[2][2] = coefs.v[2]*(1.-xb-yb);
	result.invalid = primaries_invert.invalid;

	return result;
}

struct CIE_XYZ {
	double X;
	double Y;
	double Z;
};

static const struct CIE_XYZ D50_XYZ = {
	0.9642,
	1.0000,
	0.8249
};

static struct CIE_XYZ xyY2XYZ(qcms_CIE_xyY source)
{
	struct CIE_XYZ dest;
	dest.X = (source.x / source.y) * source.Y;
	dest.Y = source.Y;
	dest.Z = ((1 - source.x - source.y) / source.y) * source.Y;
	return dest;
}

static struct matrix
compute_chromatic_adaption(struct CIE_XYZ source_white_point,
                           struct CIE_XYZ dest_white_point,
                           struct matrix chad)
{
	struct matrix chad_inv;
	struct vector cone_source_XYZ, cone_source_rgb;
	struct vector cone_dest_XYZ, cone_dest_rgb;
	struct matrix cone, tmp;

	tmp = chad;
	chad_inv = matrix_invert(tmp);

	cone_source_XYZ.v[0] = source_white_point.X;
	cone_source_XYZ.v[1] = source_white_point.Y;
	cone_source_XYZ.v[2] = source_white_point.Z;

	cone_dest_XYZ.v[0] = dest_white_point.X;
	cone_dest_XYZ.v[1] = dest_white_point.Y;
	cone_dest_XYZ.v[2] = dest_white_point.Z;

	cone_source_rgb = matrix_eval(chad, cone_source_XYZ);
	cone_dest_rgb   = matrix_eval(chad, cone_dest_XYZ);

	cone.m[0][0] = cone_dest_rgb.v[0]/cone_source_rgb.v[0];
	cone.m[0][1] = 0;
	cone.m[0][2] = 0;
	cone.m[1][0] = 0;
	cone.m[1][1] = cone_dest_rgb.v[1]/cone_source_rgb.v[1];
	cone.m[1][2] = 0;
	cone.m[2][0] = 0;
	cone.m[2][1] = 0;
	cone.m[2][2] = cone_dest_rgb.v[2]/cone_source_rgb.v[2];
	cone.invalid = false;

	
	return matrix_multiply(chad_inv, matrix_multiply(cone, chad));
}

static struct matrix
adaption_matrix(struct CIE_XYZ source_illumination, struct CIE_XYZ target_illumination)
{
#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4305)
#endif
	struct matrix lam_rigg = {{ 
	                         {  0.8951,  0.2664, -0.1614 },
	                         { -0.7502,  1.7135,  0.0367 },
	                         {  0.0389, -0.0685,  1.0296 }
	                         }};
#if defined (_MSC_VER)
#pragma warning(pop)
#endif
	return compute_chromatic_adaption(source_illumination, target_illumination, lam_rigg);
}

static struct matrix adapt_matrix_to_D50(struct matrix r, qcms_CIE_xyY source_white_pt)
{
	struct CIE_XYZ Dn;
	struct matrix Bradford;

	if (source_white_pt.y == 0.0)
		return matrix_invalid();

	Dn = xyY2XYZ(source_white_pt);

	Bradford = adaption_matrix(Dn, D50_XYZ);
	return matrix_multiply(Bradford, r);
}

qcms_bool set_rgb_colorants(qcms_profile *profile, qcms_CIE_xyY white_point, qcms_CIE_xyYTRIPLE primaries)
{
	struct matrix colorants;
	colorants = build_RGB_to_XYZ_transfer_matrix(white_point, primaries);
	colorants = adapt_matrix_to_D50(colorants, white_point);

	if (colorants.invalid)
		return false;

	
	profile->redColorant.X = double_to_s15Fixed16Number(colorants.m[0][0]);
	profile->redColorant.Y = double_to_s15Fixed16Number(colorants.m[1][0]);
	profile->redColorant.Z = double_to_s15Fixed16Number(colorants.m[2][0]);

	profile->greenColorant.X = double_to_s15Fixed16Number(colorants.m[0][1]);
	profile->greenColorant.Y = double_to_s15Fixed16Number(colorants.m[1][1]);
	profile->greenColorant.Z = double_to_s15Fixed16Number(colorants.m[2][1]);

	profile->blueColorant.X = double_to_s15Fixed16Number(colorants.m[0][2]);
	profile->blueColorant.Y = double_to_s15Fixed16Number(colorants.m[1][2]);
	profile->blueColorant.Z = double_to_s15Fixed16Number(colorants.m[2][2]);

	return true;
}

#if 0
static void qcms_transform_data_rgb_out_pow(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	int i;
	float (*mat)[4] = transform->matrix;
	for (i=0; i<length; i++) {
		unsigned char device_r = *src++;
		unsigned char device_g = *src++;
		unsigned char device_b = *src++;

		float linear_r = transform->input_gamma_table_r[device_r];
		float linear_g = transform->input_gamma_table_g[device_g];
		float linear_b = transform->input_gamma_table_b[device_b];

		float out_linear_r = mat[0][0]*linear_r + mat[1][0]*linear_g + mat[2][0]*linear_b;
		float out_linear_g = mat[0][1]*linear_r + mat[1][1]*linear_g + mat[2][1]*linear_b;
		float out_linear_b = mat[0][2]*linear_r + mat[1][2]*linear_g + mat[2][2]*linear_b;

		float out_device_r = pow(out_linear_r, transform->out_gamma_r);
		float out_device_g = pow(out_linear_g, transform->out_gamma_g);
		float out_device_b = pow(out_linear_b, transform->out_gamma_b);

		dest[r_out] = clamp_u8(out_device_r*255);
		dest[1]     = clamp_u8(out_device_g*255);
		dest[b_out] = clamp_u8(out_device_b*255);
		dest += 3;
	}
}
#endif

static void qcms_transform_data_gray_out_lut(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	for (i = 0; i < length; i++) {
		float out_device_r, out_device_g, out_device_b;
		unsigned char device = *src++;

		float linear = transform->input_gamma_table_gray[device];

		out_device_r = lut_interp_linear(linear, transform->output_gamma_lut_r, transform->output_gamma_lut_r_length);
		out_device_g = lut_interp_linear(linear, transform->output_gamma_lut_g, transform->output_gamma_lut_g_length);
		out_device_b = lut_interp_linear(linear, transform->output_gamma_lut_b, transform->output_gamma_lut_b_length);

		dest[r_out] = clamp_u8(out_device_r*255);
		dest[1]     = clamp_u8(out_device_g*255);
		dest[b_out] = clamp_u8(out_device_b*255);
		dest += 3;
	}
}


static void qcms_transform_data_graya_out_lut(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	for (i = 0; i < length; i++) {
		float out_device_r, out_device_g, out_device_b;
		unsigned char device = *src++;
		unsigned char alpha = *src++;

		float linear = transform->input_gamma_table_gray[device];

		out_device_r = lut_interp_linear(linear, transform->output_gamma_lut_r, transform->output_gamma_lut_r_length);
		out_device_g = lut_interp_linear(linear, transform->output_gamma_lut_g, transform->output_gamma_lut_g_length);
		out_device_b = lut_interp_linear(linear, transform->output_gamma_lut_b, transform->output_gamma_lut_b_length);

		dest[r_out] = clamp_u8(out_device_r*255);
		dest[1]     = clamp_u8(out_device_g*255);
		dest[b_out] = clamp_u8(out_device_b*255);
		dest[3]     = alpha;
		dest += 4;
	}
}


static void qcms_transform_data_gray_out_precache(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	for (i = 0; i < length; i++) {
		unsigned char device = *src++;
		uint16_t gray;

		float linear = transform->input_gamma_table_gray[device];

		
		gray = linear * PRECACHE_OUTPUT_MAX;

		dest[r_out] = transform->output_table_r->data[gray];
		dest[1]     = transform->output_table_g->data[gray];
		dest[b_out] = transform->output_table_b->data[gray];
		dest += 3;
	}
}


static void qcms_transform_data_graya_out_precache(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	for (i = 0; i < length; i++) {
		unsigned char device = *src++;
		unsigned char alpha = *src++;
		uint16_t gray;

		float linear = transform->input_gamma_table_gray[device];

		
		gray = linear * PRECACHE_OUTPUT_MAX;

		dest[r_out] = transform->output_table_r->data[gray];
		dest[1]     = transform->output_table_g->data[gray];
		dest[b_out] = transform->output_table_b->data[gray];
		dest[3]     = alpha;
		dest += 4;
	}
}

static void qcms_transform_data_rgb_out_lut_precache(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	float (*mat)[4] = transform->matrix;
	for (i = 0; i < length; i++) {
		unsigned char device_r = *src++;
		unsigned char device_g = *src++;
		unsigned char device_b = *src++;
		uint16_t r, g, b;

		float linear_r = transform->input_gamma_table_r[device_r];
		float linear_g = transform->input_gamma_table_g[device_g];
		float linear_b = transform->input_gamma_table_b[device_b];

		float out_linear_r = mat[0][0]*linear_r + mat[1][0]*linear_g + mat[2][0]*linear_b;
		float out_linear_g = mat[0][1]*linear_r + mat[1][1]*linear_g + mat[2][1]*linear_b;
		float out_linear_b = mat[0][2]*linear_r + mat[1][2]*linear_g + mat[2][2]*linear_b;

		out_linear_r = clamp_float(out_linear_r);
		out_linear_g = clamp_float(out_linear_g);
		out_linear_b = clamp_float(out_linear_b);

		
		r = out_linear_r * PRECACHE_OUTPUT_MAX;
		g = out_linear_g * PRECACHE_OUTPUT_MAX;
		b = out_linear_b * PRECACHE_OUTPUT_MAX;

		dest[r_out] = transform->output_table_r->data[r];
		dest[1]     = transform->output_table_g->data[g];
		dest[b_out] = transform->output_table_b->data[b];
		dest += 3;
	}
}

static void qcms_transform_data_rgba_out_lut_precache(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	float (*mat)[4] = transform->matrix;
	for (i = 0; i < length; i++) {
		unsigned char device_r = *src++;
		unsigned char device_g = *src++;
		unsigned char device_b = *src++;
		unsigned char alpha = *src++;
		uint16_t r, g, b;

		float linear_r = transform->input_gamma_table_r[device_r];
		float linear_g = transform->input_gamma_table_g[device_g];
		float linear_b = transform->input_gamma_table_b[device_b];

		float out_linear_r = mat[0][0]*linear_r + mat[1][0]*linear_g + mat[2][0]*linear_b;
		float out_linear_g = mat[0][1]*linear_r + mat[1][1]*linear_g + mat[2][1]*linear_b;
		float out_linear_b = mat[0][2]*linear_r + mat[1][2]*linear_g + mat[2][2]*linear_b;

		out_linear_r = clamp_float(out_linear_r);
		out_linear_g = clamp_float(out_linear_g);
		out_linear_b = clamp_float(out_linear_b);

		
		r = out_linear_r * PRECACHE_OUTPUT_MAX;
		g = out_linear_g * PRECACHE_OUTPUT_MAX;
		b = out_linear_b * PRECACHE_OUTPUT_MAX;

		dest[r_out] = transform->output_table_r->data[r];
		dest[1]     = transform->output_table_g->data[g];
		dest[b_out] = transform->output_table_b->data[b];
		dest[3]     = alpha;
		dest += 4;
	}
}


static void qcms_transform_data_tetra_clut_rgba(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	int xy_len = 1;
	int x_len = transform->grid_size;
	int len = x_len * x_len;
	float* r_table = transform->r_clut;
	float* g_table = transform->g_clut;
	float* b_table = transform->b_clut;
	float c0_r, c1_r, c2_r, c3_r;
	float c0_g, c1_g, c2_g, c3_g;
	float c0_b, c1_b, c2_b, c3_b;
	float clut_r, clut_g, clut_b;
	for (i = 0; i < length; i++) {
		unsigned char in_r = *src++;
		unsigned char in_g = *src++;
		unsigned char in_b = *src++;
		unsigned char in_a = *src++;
		float linear_r = in_r/255.0f, linear_g=in_g/255.0f, linear_b = in_b/255.0f;

		int x = floor(linear_r * (transform->grid_size-1));
		int y = floor(linear_g * (transform->grid_size-1));
		int z = floor(linear_b * (transform->grid_size-1));
		int x_n = ceil(linear_r * (transform->grid_size-1));
		int y_n = ceil(linear_g * (transform->grid_size-1));
		int z_n = ceil(linear_b * (transform->grid_size-1));
		float rx = linear_r * (transform->grid_size-1) - x; 
		float ry = linear_g * (transform->grid_size-1) - y;
		float rz = linear_b * (transform->grid_size-1) - z; 

		c0_r = CLU(r_table, x, y, z);
		c0_g = CLU(g_table, x, y, z);
		c0_b = CLU(b_table, x, y, z);

		if( rx >= ry ) {
			if (ry >= rz) { 
				c1_r = CLU(r_table, x_n, y, z) - c0_r;
				c2_r = CLU(r_table, x_n, y_n, z) - CLU(r_table, x_n, y, z);
				c3_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x_n, y_n, z);
				c1_g = CLU(g_table, x_n, y, z) - c0_g;
				c2_g = CLU(g_table, x_n, y_n, z) - CLU(g_table, x_n, y, z);
				c3_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x_n, y_n, z);
				c1_b = CLU(b_table, x_n, y, z) - c0_b;
				c2_b = CLU(b_table, x_n, y_n, z) - CLU(b_table, x_n, y, z);
				c3_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x_n, y_n, z);
			} else { 
				if (rx >= rz) { 
					c1_r = CLU(r_table, x_n, y, z) - c0_r;
					c2_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x_n, y, z_n);
					c3_r = CLU(r_table, x_n, y, z_n) - CLU(r_table, x_n, y, z);
					c1_g = CLU(g_table, x_n, y, z) - c0_g;
					c2_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x_n, y, z_n);
					c3_g = CLU(g_table, x_n, y, z_n) - CLU(g_table, x_n, y, z);
					c1_b = CLU(b_table, x_n, y, z) - c0_b;
					c2_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x_n, y, z_n);
					c3_b = CLU(b_table, x_n, y, z_n) - CLU(b_table, x_n, y, z);
				} else { 
					c1_r = CLU(r_table, x_n, y, z_n) - CLU(r_table, x, y, z_n);
					c2_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x_n, y, z_n);
					c3_r = CLU(r_table, x, y, z_n) - c0_r;
					c1_g = CLU(g_table, x_n, y, z_n) - CLU(g_table, x, y, z_n);
					c2_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x_n, y, z_n);
					c3_g = CLU(g_table, x, y, z_n) - c0_g;
					c1_b = CLU(b_table, x_n, y, z_n) - CLU(b_table, x, y, z_n);
					c2_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x_n, y, z_n);
					c3_b = CLU(b_table, x, y, z_n) - c0_b;
				}
			}
		} else {
			if (rx >= rz) { 
				c1_r = CLU(r_table, x_n, y_n, z) - CLU(r_table, x, y_n, z);
				c2_r = CLU(r_table, x, y_n, z) - c0_r;
				c3_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x_n, y_n, z);
				c1_g = CLU(g_table, x_n, y_n, z) - CLU(g_table, x, y_n, z);
				c2_g = CLU(g_table, x, y_n, z) - c0_g;
				c3_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x_n, y_n, z);
				c1_b = CLU(b_table, x_n, y_n, z) - CLU(b_table, x, y_n, z);
				c2_b = CLU(b_table, x, y_n, z) - c0_b;
				c3_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x_n, y_n, z);
			} else {
				if (ry >= rz) { 
					c1_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x, y_n, z_n);
					c2_r = CLU(r_table, x, y_n, z) - c0_r;
					c3_r = CLU(r_table, x, y_n, z_n) - CLU(r_table, x, y_n, z);
					c1_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x, y_n, z_n);
					c2_g = CLU(g_table, x, y_n, z) - c0_g;
					c3_g = CLU(g_table, x, y_n, z_n) - CLU(g_table, x, y_n, z);
					c1_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x, y_n, z_n);
					c2_b = CLU(b_table, x, y_n, z) - c0_b;
					c3_b = CLU(b_table, x, y_n, z_n) - CLU(b_table, x, y_n, z);
				} else { 
					c1_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x, y_n, z_n);
					c2_r = CLU(r_table, x, y_n, z_n) - CLU(r_table, x, y, z_n);
					c3_r = CLU(r_table, x, y, z_n) - c0_r;
					c1_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x, y_n, z_n);
					c2_g = CLU(g_table, x, y_n, z_n) - CLU(g_table, x, y, z_n);
					c3_g = CLU(g_table, x, y, z_n) - c0_g;
					c1_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x, y_n, z_n);
					c2_b = CLU(b_table, x, y_n, z_n) - CLU(b_table, x, y, z_n);
					c3_b = CLU(b_table, x, y, z_n) - c0_b;
				}
			}
		}
				
		clut_r = c0_r + c1_r*rx + c2_r*ry + c3_r*rz;
		clut_g = c0_g + c1_g*rx + c2_g*ry + c3_g*rz;
		clut_b = c0_b + c1_b*rx + c2_b*ry + c3_b*rz;

		dest[r_out] = clamp_u8(clut_r*255.0f);
		dest[1]     = clamp_u8(clut_g*255.0f);
		dest[b_out] = clamp_u8(clut_b*255.0f);
		dest[3]     = in_a;
		dest += 4;
	}
}

static void qcms_transform_data_tetra_clut(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	int xy_len = 1;
	int x_len = transform->grid_size;
	int len = x_len * x_len;
	float* r_table = transform->r_clut;
	float* g_table = transform->g_clut;
	float* b_table = transform->b_clut;
	float c0_r, c1_r, c2_r, c3_r;
	float c0_g, c1_g, c2_g, c3_g;
	float c0_b, c1_b, c2_b, c3_b;
	float clut_r, clut_g, clut_b;
	for (i = 0; i < length; i++) {
		unsigned char in_r = *src++;
		unsigned char in_g = *src++;
		unsigned char in_b = *src++;
		float linear_r = in_r/255.0f, linear_g=in_g/255.0f, linear_b = in_b/255.0f;

		int x = floor(linear_r * (transform->grid_size-1));
		int y = floor(linear_g * (transform->grid_size-1));
		int z = floor(linear_b * (transform->grid_size-1));
		int x_n = ceil(linear_r * (transform->grid_size-1));
		int y_n = ceil(linear_g * (transform->grid_size-1));
		int z_n = ceil(linear_b * (transform->grid_size-1));
		float rx = linear_r * (transform->grid_size-1) - x; 
		float ry = linear_g * (transform->grid_size-1) - y;
		float rz = linear_b * (transform->grid_size-1) - z; 

		c0_r = CLU(r_table, x, y, z);
		c0_g = CLU(g_table, x, y, z);
		c0_b = CLU(b_table, x, y, z);

		if( rx >= ry ) {
			if (ry >= rz) { 
				c1_r = CLU(r_table, x_n, y, z) - c0_r;
				c2_r = CLU(r_table, x_n, y_n, z) - CLU(r_table, x_n, y, z);
				c3_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x_n, y_n, z);
				c1_g = CLU(g_table, x_n, y, z) - c0_g;
				c2_g = CLU(g_table, x_n, y_n, z) - CLU(g_table, x_n, y, z);
				c3_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x_n, y_n, z);
				c1_b = CLU(b_table, x_n, y, z) - c0_b;
				c2_b = CLU(b_table, x_n, y_n, z) - CLU(b_table, x_n, y, z);
				c3_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x_n, y_n, z);
			} else { 
				if (rx >= rz) { 
					c1_r = CLU(r_table, x_n, y, z) - c0_r;
					c2_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x_n, y, z_n);
					c3_r = CLU(r_table, x_n, y, z_n) - CLU(r_table, x_n, y, z);
					c1_g = CLU(g_table, x_n, y, z) - c0_g;
					c2_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x_n, y, z_n);
					c3_g = CLU(g_table, x_n, y, z_n) - CLU(g_table, x_n, y, z);
					c1_b = CLU(b_table, x_n, y, z) - c0_b;
					c2_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x_n, y, z_n);
					c3_b = CLU(b_table, x_n, y, z_n) - CLU(b_table, x_n, y, z);
				} else { 
					c1_r = CLU(r_table, x_n, y, z_n) - CLU(r_table, x, y, z_n);
					c2_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x_n, y, z_n);
					c3_r = CLU(r_table, x, y, z_n) - c0_r;
					c1_g = CLU(g_table, x_n, y, z_n) - CLU(g_table, x, y, z_n);
					c2_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x_n, y, z_n);
					c3_g = CLU(g_table, x, y, z_n) - c0_g;
					c1_b = CLU(b_table, x_n, y, z_n) - CLU(b_table, x, y, z_n);
					c2_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x_n, y, z_n);
					c3_b = CLU(b_table, x, y, z_n) - c0_b;
				}
			}
		} else {
			if (rx >= rz) { 
				c1_r = CLU(r_table, x_n, y_n, z) - CLU(r_table, x, y_n, z);
				c2_r = CLU(r_table, x, y_n, z) - c0_r;
				c3_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x_n, y_n, z);
				c1_g = CLU(g_table, x_n, y_n, z) - CLU(g_table, x, y_n, z);
				c2_g = CLU(g_table, x, y_n, z) - c0_g;
				c3_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x_n, y_n, z);
				c1_b = CLU(b_table, x_n, y_n, z) - CLU(b_table, x, y_n, z);
				c2_b = CLU(b_table, x, y_n, z) - c0_b;
				c3_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x_n, y_n, z);
			} else {
				if (ry >= rz) { 
					c1_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x, y_n, z_n);
					c2_r = CLU(r_table, x, y_n, z) - c0_r;
					c3_r = CLU(r_table, x, y_n, z_n) - CLU(r_table, x, y_n, z);
					c1_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x, y_n, z_n);
					c2_g = CLU(g_table, x, y_n, z) - c0_g;
					c3_g = CLU(g_table, x, y_n, z_n) - CLU(g_table, x, y_n, z);
					c1_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x, y_n, z_n);
					c2_b = CLU(b_table, x, y_n, z) - c0_b;
					c3_b = CLU(b_table, x, y_n, z_n) - CLU(b_table, x, y_n, z);
				} else { 
					c1_r = CLU(r_table, x_n, y_n, z_n) - CLU(r_table, x, y_n, z_n);
					c2_r = CLU(r_table, x, y_n, z_n) - CLU(r_table, x, y, z_n);
					c3_r = CLU(r_table, x, y, z_n) - c0_r;
					c1_g = CLU(g_table, x_n, y_n, z_n) - CLU(g_table, x, y_n, z_n);
					c2_g = CLU(g_table, x, y_n, z_n) - CLU(g_table, x, y, z_n);
					c3_g = CLU(g_table, x, y, z_n) - c0_g;
					c1_b = CLU(b_table, x_n, y_n, z_n) - CLU(b_table, x, y_n, z_n);
					c2_b = CLU(b_table, x, y_n, z_n) - CLU(b_table, x, y, z_n);
					c3_b = CLU(b_table, x, y, z_n) - c0_b;
				}
			}
		}
				
		clut_r = c0_r + c1_r*rx + c2_r*ry + c3_r*rz;
		clut_g = c0_g + c1_g*rx + c2_g*ry + c3_g*rz;
		clut_b = c0_b + c1_b*rx + c2_b*ry + c3_b*rz;

		dest[r_out] = clamp_u8(clut_r*255.0f);
		dest[1]     = clamp_u8(clut_g*255.0f);
		dest[b_out] = clamp_u8(clut_b*255.0f);
		dest += 3;
	}
}

static void qcms_transform_data_rgb_out_lut(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	float (*mat)[4] = transform->matrix;
	for (i = 0; i < length; i++) {
		unsigned char device_r = *src++;
		unsigned char device_g = *src++;
		unsigned char device_b = *src++;
		float out_device_r, out_device_g, out_device_b;

		float linear_r = transform->input_gamma_table_r[device_r];
		float linear_g = transform->input_gamma_table_g[device_g];
		float linear_b = transform->input_gamma_table_b[device_b];

		float out_linear_r = mat[0][0]*linear_r + mat[1][0]*linear_g + mat[2][0]*linear_b;
		float out_linear_g = mat[0][1]*linear_r + mat[1][1]*linear_g + mat[2][1]*linear_b;
		float out_linear_b = mat[0][2]*linear_r + mat[1][2]*linear_g + mat[2][2]*linear_b;

		out_linear_r = clamp_float(out_linear_r);
		out_linear_g = clamp_float(out_linear_g);
		out_linear_b = clamp_float(out_linear_b);

		out_device_r = lut_interp_linear(out_linear_r, 
				transform->output_gamma_lut_r, transform->output_gamma_lut_r_length);
		out_device_g = lut_interp_linear(out_linear_g, 
				transform->output_gamma_lut_g, transform->output_gamma_lut_g_length);
		out_device_b = lut_interp_linear(out_linear_b, 
				transform->output_gamma_lut_b, transform->output_gamma_lut_b_length);

		dest[r_out] = clamp_u8(out_device_r*255);
		dest[1]     = clamp_u8(out_device_g*255);
		dest[b_out] = clamp_u8(out_device_b*255);
		dest += 3;
	}
}

static void qcms_transform_data_rgba_out_lut(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	unsigned int i;
	float (*mat)[4] = transform->matrix;
	for (i = 0; i < length; i++) {
		unsigned char device_r = *src++;
		unsigned char device_g = *src++;
		unsigned char device_b = *src++;
		unsigned char alpha = *src++;
		float out_device_r, out_device_g, out_device_b;

		float linear_r = transform->input_gamma_table_r[device_r];
		float linear_g = transform->input_gamma_table_g[device_g];
		float linear_b = transform->input_gamma_table_b[device_b];

		float out_linear_r = mat[0][0]*linear_r + mat[1][0]*linear_g + mat[2][0]*linear_b;
		float out_linear_g = mat[0][1]*linear_r + mat[1][1]*linear_g + mat[2][1]*linear_b;
		float out_linear_b = mat[0][2]*linear_r + mat[1][2]*linear_g + mat[2][2]*linear_b;

		out_linear_r = clamp_float(out_linear_r);
		out_linear_g = clamp_float(out_linear_g);
		out_linear_b = clamp_float(out_linear_b);

		out_device_r = lut_interp_linear(out_linear_r, 
				transform->output_gamma_lut_r, transform->output_gamma_lut_r_length);
		out_device_g = lut_interp_linear(out_linear_g, 
				transform->output_gamma_lut_g, transform->output_gamma_lut_g_length);
		out_device_b = lut_interp_linear(out_linear_b, 
				transform->output_gamma_lut_b, transform->output_gamma_lut_b_length);

		dest[r_out] = clamp_u8(out_device_r*255);
		dest[1]     = clamp_u8(out_device_g*255);
		dest[b_out] = clamp_u8(out_device_b*255);
		dest[3]     = alpha;
		dest += 4;
	}
}

#if 0
static void qcms_transform_data_rgb_out_linear(qcms_transform *transform, unsigned char *src, unsigned char *dest, size_t length, qcms_format_type output_format)
{
	const int r_out = output_format.r;
	const int b_out = output_format.b;

	int i;
	float (*mat)[4] = transform->matrix;
	for (i = 0; i < length; i++) {
		unsigned char device_r = *src++;
		unsigned char device_g = *src++;
		unsigned char device_b = *src++;

		float linear_r = transform->input_gamma_table_r[device_r];
		float linear_g = transform->input_gamma_table_g[device_g];
		float linear_b = transform->input_gamma_table_b[device_b];

		float out_linear_r = mat[0][0]*linear_r + mat[1][0]*linear_g + mat[2][0]*linear_b;
		float out_linear_g = mat[0][1]*linear_r + mat[1][1]*linear_g + mat[2][1]*linear_b;
		float out_linear_b = mat[0][2]*linear_r + mat[1][2]*linear_g + mat[2][2]*linear_b;

		dest[r_out] = clamp_u8(out_linear_r*255);
		dest[1]     = clamp_u8(out_linear_g*255);
		dest[b_out] = clamp_u8(out_linear_b*255);
		dest += 3;
	}
}
#endif

static struct precache_output *precache_reference(struct precache_output *p)
{
	qcms_atomic_increment(p->ref_count);
	return p;
}

static struct precache_output *precache_create()
{
	struct precache_output *p = malloc(sizeof(struct precache_output));
	if (p)
		p->ref_count = 1;
	return p;
}

void precache_release(struct precache_output *p)
{
	if (qcms_atomic_decrement(p->ref_count) == 0) {
		free(p);
	}
}

#ifdef HAVE_POSIX_MEMALIGN
static qcms_transform *transform_alloc(void)
{
	qcms_transform *t;
	if (!posix_memalign(&t, 16, sizeof(*t))) {
		return t;
	} else {
		return NULL;
	}
}
static void transform_free(qcms_transform *t)
{
	free(t);
}
#else
static qcms_transform *transform_alloc(void)
{
	
	char *original_block = calloc(sizeof(qcms_transform) + sizeof(void*) + 16, 1);
	
	void *transform_start = original_block + sizeof(void*);
	
	qcms_transform *transform_aligned = (qcms_transform*)(((uintptr_t)transform_start + 15) & ~0xf);

	
	void **(original_block_ptr) = (void**)transform_aligned;
	if (!original_block)
		return NULL;
	original_block_ptr--;
	*original_block_ptr = original_block;

	return transform_aligned;
}
static void transform_free(qcms_transform *t)
{
	
	void **p = (void**)t;
	p--;
	free(*p);
}
#endif

void qcms_transform_release(qcms_transform *t)
{

	if (t->output_table_r)
		precache_release(t->output_table_r);
	if (t->output_table_g)
		precache_release(t->output_table_g);
	if (t->output_table_b)
		precache_release(t->output_table_b);

	free(t->input_gamma_table_r);
	if (t->input_gamma_table_g != t->input_gamma_table_r)
		free(t->input_gamma_table_g);
	if (t->input_gamma_table_g != t->input_gamma_table_r &&
	    t->input_gamma_table_g != t->input_gamma_table_b)
		free(t->input_gamma_table_b);

	free(t->input_gamma_table_gray);

	free(t->output_gamma_lut_r);
	free(t->output_gamma_lut_g);
	free(t->output_gamma_lut_b);

	transform_free(t);
}

#ifdef X86
 
#if defined(_M_IX86) && defined(_MSC_VER)
#define HAS_CPUID
static void cpuid(uint32_t fxn, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
       uint32_t a_, b_, c_, d_;
       __asm {
              xchg   ebx, esi
              mov    eax, fxn
              cpuid
              mov    a_, eax
              mov    b_, ebx
              mov    c_, ecx
              mov    d_, edx
              xchg   ebx, esi
       }
       *a = a_;
       *b = b_;
       *c = c_;
       *d = d_;
}
#elif (defined(__GNUC__) || defined(__SUNPRO_C)) && (defined(__i386__) || defined(__i386))
#define HAS_CPUID
static void cpuid(uint32_t fxn, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {

	uint32_t a_, b_, c_, d_;
       __asm__ __volatile__ ("xchgl %%ebx, %%esi; cpuid; xchgl %%ebx, %%esi;" 
                             : "=a" (a_), "=S" (b_), "=c" (c_), "=d" (d_) : "a" (fxn));
	   *a = a_;
	   *b = b_;
	   *c = c_;
	   *d = d_;
}
#endif


#define SSE1_EDX_MASK (1UL << 25)
#define SSE2_EDX_MASK (1UL << 26)
#define SSE3_ECX_MASK (1UL <<  0)

static int sse_version_available(void)
{
#if defined(__x86_64__) || defined(__x86_64) || defined(_M_AMD64)
	return 2;
#elif defined(HAS_CPUID)
	static int sse_version = -1;
	uint32_t a, b, c, d;
	uint32_t function = 0x00000001;

	if (sse_version == -1) {
		sse_version = 0;
		cpuid(function, &a, &b, &c, &d);
		if (c & SSE3_ECX_MASK)
			sse_version = 3;
		else if (d & SSE2_EDX_MASK)
			sse_version = 2;
		else if (d & SSE1_EDX_MASK)
			sse_version = 1;
	}

	return sse_version;
#else
	return 0;
#endif
}
#endif

static const struct matrix bradford_matrix = {{	{ 0.8951f, 0.2664f,-0.1614f},
						{-0.7502f, 1.7135f, 0.0367f},
						{ 0.0389f,-0.0685f, 1.0296f}}, 
						false};

static const struct matrix bradford_matrix_inv = {{ { 0.9869929f,-0.1470543f, 0.1599627f},
						    { 0.4323053f, 0.5183603f, 0.0492912f},
						    {-0.0085287f, 0.0400428f, 0.9684867f}}, 
						    false};

struct matrix compute_whitepoint_adaption(float X, float Y, float Z) {
	float p = (0.96422f*bradford_matrix.m[0][0] + 1.000f*bradford_matrix.m[1][0] + 0.82521f*bradford_matrix.m[2][0]) /
		  (X*bradford_matrix.m[0][0]      + Y*bradford_matrix.m[1][0]      + Z*bradford_matrix.m[2][0]     );
	float y = (0.96422f*bradford_matrix.m[0][1] + 1.000f*bradford_matrix.m[1][1] + 0.82521f*bradford_matrix.m[2][1]) /
		  (X*bradford_matrix.m[0][1]      + Y*bradford_matrix.m[1][1]      + Z*bradford_matrix.m[2][1]     );
	float b = (0.96422f*bradford_matrix.m[0][2] + 1.000f*bradford_matrix.m[1][2] + 0.82521f*bradford_matrix.m[2][2]) /
		  (X*bradford_matrix.m[0][2]      + Y*bradford_matrix.m[1][2]      + Z*bradford_matrix.m[2][2]     );
	struct matrix white_adaption = {{ {p,0,0}, {0,y,0}, {0,0,b}}, false};
	return matrix_multiply( bradford_matrix_inv, matrix_multiply(white_adaption, bradford_matrix) );
}

void qcms_profile_precache_output_transform(qcms_profile *profile)
{
	
	if (profile->color_space != RGB_SIGNATURE)
		return;

	if (qcms_supports_iccv4) {
		
		if (profile->B2A0)
			return;

		
		if (profile->mBA)
			return;
	}

	
	if (!profile->redTRC || !profile->greenTRC || !profile->blueTRC)
		return;

	if (!profile->output_table_r) {
		profile->output_table_r = precache_create();
		if (profile->output_table_r &&
				!compute_precache(profile->redTRC, profile->output_table_r->data)) {
			precache_release(profile->output_table_r);
			profile->output_table_r = NULL;
		}
	}
	if (!profile->output_table_g) {
		profile->output_table_g = precache_create();
		if (profile->output_table_g &&
				!compute_precache(profile->greenTRC, profile->output_table_g->data)) {
			precache_release(profile->output_table_g);
			profile->output_table_g = NULL;
		}
	}
	if (!profile->output_table_b) {
		profile->output_table_b = precache_create();
		if (profile->output_table_b &&
				!compute_precache(profile->blueTRC, profile->output_table_b->data)) {
			precache_release(profile->output_table_b);
			profile->output_table_b = NULL;
		}
	}
}

qcms_transform* qcms_transform_precacheLUT_float(qcms_transform *transform, qcms_profile *in, qcms_profile *out, 
                                                 int samples, qcms_data_type in_type)
{
	
	uint16_t x,y,z;
	uint32_t l;
	uint32_t lutSize = 3 * samples * samples * samples;
	float* src = NULL;
	float* dest = NULL;
	float* lut = NULL;

	src = malloc(lutSize*sizeof(float));
	dest = malloc(lutSize*sizeof(float));

	if (src && dest) {
		
		l = 0;
		for (x = 0; x < samples; x++) {
			for (y = 0; y < samples; y++) {
				for (z = 0; z < samples; z++) {
					src[l++] = x / (float)(samples-1);
					src[l++] = y / (float)(samples-1);
					src[l++] = z / (float)(samples-1);
				}
			}
		}

		lut = qcms_chain_transform(in, out, src, dest, lutSize);
		if (lut) {
			transform->r_clut = &lut[0];
			transform->g_clut = &lut[1];
			transform->b_clut = &lut[2];
			transform->grid_size = samples;
			if (in_type == QCMS_DATA_RGBA_8) {
				transform->transform_fn = qcms_transform_data_tetra_clut_rgba;
			} else {
				transform->transform_fn = qcms_transform_data_tetra_clut;
			}
		}
	}


	
	if (src && lut != src) {
		free(src);
	} else if (dest && lut != src) {
		free(dest);
	}

	if (lut == NULL) {
		return NULL;
	}
	return transform;
}

#define NO_MEM_TRANSFORM NULL

qcms_transform* qcms_transform_create(
		qcms_profile *in, qcms_data_type in_type,
		qcms_profile *out, qcms_data_type out_type,
		qcms_intent intent)
{
	bool precache = false;

        qcms_transform *transform = transform_alloc();
        if (!transform) {
		return NULL;
	}
	if (out_type != QCMS_DATA_RGB_8 &&
                out_type != QCMS_DATA_RGBA_8) {
            assert(0 && "output type");
	    transform_free(transform);
            return NULL;
        }

	if (out->output_table_r &&
			out->output_table_g &&
			out->output_table_b) {
		precache = true;
	}

	if (qcms_supports_iccv4 && (in->A2B0 || out->B2A0 || in->mAB || out->mAB)) {
		
		
		
		
		
		qcms_transform *result = qcms_transform_precacheLUT_float(transform, in, out, 33, in_type);
		if (!result) {
            		assert(0 && "precacheLUT failed");
			transform_free(transform);
			return NULL;
		}
		return result;
	}

	if (precache) {
		transform->output_table_r = precache_reference(out->output_table_r);
		transform->output_table_g = precache_reference(out->output_table_g);
		transform->output_table_b = precache_reference(out->output_table_b);
	} else {
		if (!out->redTRC || !out->greenTRC || !out->blueTRC) {
			qcms_transform_release(transform);
			return NO_MEM_TRANSFORM;
		}
		build_output_lut(out->redTRC, &transform->output_gamma_lut_r, &transform->output_gamma_lut_r_length);
		build_output_lut(out->greenTRC, &transform->output_gamma_lut_g, &transform->output_gamma_lut_g_length);
		build_output_lut(out->blueTRC, &transform->output_gamma_lut_b, &transform->output_gamma_lut_b_length);
		if (!transform->output_gamma_lut_r || !transform->output_gamma_lut_g || !transform->output_gamma_lut_b) {
			qcms_transform_release(transform);
			return NO_MEM_TRANSFORM;
		}
	}

        if (in->color_space == RGB_SIGNATURE) {
		struct matrix in_matrix, out_matrix, result;

		if (in_type != QCMS_DATA_RGB_8 &&
                    in_type != QCMS_DATA_RGBA_8){
                	assert(0 && "input type");
			transform_free(transform);
                	return NULL;
            	}
		if (precache) {
#if defined(SSE2_ENABLE) && defined(X86)
		    if (sse_version_available() >= 2) {
			    if (in_type == QCMS_DATA_RGB_8)
				    transform->transform_fn = qcms_transform_data_rgb_out_lut_sse2;
			    else
				    transform->transform_fn = qcms_transform_data_rgba_out_lut_sse2;

#if defined(SSE2_ENABLE) && !(defined(_MSC_VER) && defined(_M_AMD64))
		    } else
		    if (sse_version_available() >= 1) {
			    if (in_type == QCMS_DATA_RGB_8)
				    transform->transform_fn = qcms_transform_data_rgb_out_lut_sse1;
			    else
				    transform->transform_fn = qcms_transform_data_rgba_out_lut_sse1;
#endif
		    } else
#endif
			{
				if (in_type == QCMS_DATA_RGB_8)
					transform->transform_fn = qcms_transform_data_rgb_out_lut_precache;
				else
					transform->transform_fn = qcms_transform_data_rgba_out_lut_precache;
			}
		} else {
			if (in_type == QCMS_DATA_RGB_8)
				transform->transform_fn = qcms_transform_data_rgb_out_lut;
			else
				transform->transform_fn = qcms_transform_data_rgba_out_lut;
		}

		
		transform->input_gamma_table_r = build_input_gamma_table(in->redTRC);
		transform->input_gamma_table_g = build_input_gamma_table(in->greenTRC);
		transform->input_gamma_table_b = build_input_gamma_table(in->blueTRC);
		if (!transform->input_gamma_table_r || !transform->input_gamma_table_g || !transform->input_gamma_table_b) {
			qcms_transform_release(transform);
			return NO_MEM_TRANSFORM;
		}


		
		in_matrix = build_colorant_matrix(in);
		out_matrix = build_colorant_matrix(out);
		out_matrix = matrix_invert(out_matrix);
		if (out_matrix.invalid) {
			qcms_transform_release(transform);
			return NULL;
		}
		result = matrix_multiply(out_matrix, in_matrix);

		transform->matrix[0][0] = result.m[0][0];
		transform->matrix[1][0] = result.m[0][1];
		transform->matrix[2][0] = result.m[0][2];
		transform->matrix[0][1] = result.m[1][0];
		transform->matrix[1][1] = result.m[1][1];
		transform->matrix[2][1] = result.m[1][2];
		transform->matrix[0][2] = result.m[2][0];
		transform->matrix[1][2] = result.m[2][1];
		transform->matrix[2][2] = result.m[2][2];

	} else if (in->color_space == GRAY_SIGNATURE) {
		if (in_type != QCMS_DATA_GRAY_8 &&
				in_type != QCMS_DATA_GRAYA_8){
			assert(0 && "input type");
			transform_free(transform);
			return NULL;
		}

		transform->input_gamma_table_gray = build_input_gamma_table(in->grayTRC);
		if (!transform->input_gamma_table_gray) {
			qcms_transform_release(transform);
			return NO_MEM_TRANSFORM;
		}

		if (precache) {
			if (in_type == QCMS_DATA_GRAY_8) {
				transform->transform_fn = qcms_transform_data_gray_out_precache;
			} else {
				transform->transform_fn = qcms_transform_data_graya_out_precache;
			}
		} else {
			if (in_type == QCMS_DATA_GRAY_8) {
				transform->transform_fn = qcms_transform_data_gray_out_lut;
			} else {
				transform->transform_fn = qcms_transform_data_graya_out_lut;
			}
		}
	} else {
		assert(0 && "unexpected colorspace");
		transform_free(transform);
		return NULL;
	}
	return transform;
}

#if defined(__has_attribute)
#define HAS_FORCE_ALIGN_ARG_POINTER __has_attribute(__force_align_arg_pointer__)
#elif defined(__GNUC__) && !defined(__x86_64__) && !defined(__amd64__) && !defined(__arm__) && !defined(__mips__)
#define HAS_FORCE_ALIGN_ARG_POINTER 1
#else
#define HAS_FORCE_ALIGN_ARG_POINTER 0
#endif

#if HAS_FORCE_ALIGN_ARG_POINTER
__attribute__((__force_align_arg_pointer__))
#endif
void qcms_transform_data(qcms_transform *transform, void *src, void *dest, size_t length)
{
	static const struct _qcms_format_type output_rgbx = { 0, 2 };

	transform->transform_fn(transform, src, dest, length, output_rgbx);
}

void qcms_transform_data_type(qcms_transform *transform, void *src, void *dest, size_t length, qcms_output_type type)
{
	static const struct _qcms_format_type output_rgbx = { 0, 2 };
	static const struct _qcms_format_type output_bgrx = { 2, 0 };

	transform->transform_fn(transform, src, dest, length, type == QCMS_OUTPUT_BGRX ? output_bgrx : output_rgbx);
}

qcms_bool qcms_supports_iccv4;
void qcms_enable_iccv4()
{
	qcms_supports_iccv4 = true;
}