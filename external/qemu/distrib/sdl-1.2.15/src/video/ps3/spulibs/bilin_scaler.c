/*
 * SDL - Simple DirectMedia Layer
 * CELL BE Support for PS3 Framebuffer
 * Copyright (C) 2008, 2009 International Business Machines Corporation
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 * USA
 *
 *  Martin Lowinski  <lowinski [at] de [dot] ibm [ibm] com>
 *  Dirk Herrendoerfer <d.herrendoerfer [at] de [dot] ibm [dot] com>
 *  SPE code based on research by:
 *  Rene Becker
 *  Thimo Emmerich
 */

#include "spu_common.h"

#include <spu_intrinsics.h>
#include <spu_mfcio.h>


#ifdef DEBUG
#define deprintf(fmt, args... ) \
	fprintf( stdout, fmt, ##args ); \
	fflush( stdout );
#else
#define deprintf( fmt, args... )
#endif

struct scale_parms_t parms __attribute__((aligned(128)));

unsigned char y_plane[2][(MAX_HDTV_WIDTH+128)*4] __attribute__((aligned(128)));
unsigned char v_plane[2][(MAX_HDTV_WIDTH+128)*2] __attribute__((aligned(128)));
unsigned char u_plane[2][(MAX_HDTV_WIDTH+128)*2] __attribute__((aligned(128)));

unsigned char scaled_y_plane[2][MAX_HDTV_WIDTH*2] __attribute__((aligned(128)));
unsigned char scaled_v_plane[2][MAX_HDTV_WIDTH/2] __attribute__((aligned(128)));
unsigned char scaled_u_plane[2][MAX_HDTV_WIDTH/2] __attribute__((aligned(128)));

static const vector float vec_255 = { 255.0f, 255.0f, 255.0f, 255.0f };
static const vector float vec_0_1 = { 0.1f, 0.1f, 0.1f, 0.1f };

void bilinear_scale_line_w8(unsigned char* src, unsigned char* dst_, unsigned int dst_width, vector float vf_x_scale, vector float vf_NSweight, unsigned int src_linestride);
void bilinear_scale_line_w16(unsigned char* src, unsigned char* dst_, unsigned int dst_width, vector float vf_x_scale, vector float vf_NSweight, unsigned int src_linestride);

void scale_srcw16_dstw16();
void scale_srcw16_dstw32();
void scale_srcw32_dstw16();
void scale_srcw32_dstw32();

int main( unsigned long long spe_id __attribute__((unused)), unsigned long long argp )
{
	deprintf("[SPU] bilin_scaler_spu is up... (on SPE #%llu)\n", spe_id);
	
	spu_mfcdma32(&parms, (unsigned int)argp, sizeof(struct scale_parms_t), TAG_INIT, MFC_GET_CMD);
	DMA_WAIT_TAG(TAG_INIT);

	deprintf("[SPU] Scale %ux%u to %ux%u\n", parms.src_pixel_width, parms.src_pixel_height,
			parms.dst_pixel_width, parms.dst_pixel_height);

	if(parms.src_pixel_width & 0x1f) {
		if(parms.dst_pixel_width & 0x1F) {
			deprintf("[SPU] Using scale_srcw16_dstw16\n");
			scale_srcw16_dstw16();
		} else {
			deprintf("[SPU] Using scale_srcw16_dstw32\n");
			scale_srcw16_dstw32();
		}
	} else {
		if(parms.dst_pixel_width & 0x1F) {
			deprintf("[SPU] Using scale_srcw32_dstw16\n");
			scale_srcw32_dstw16();
		} else {
			deprintf("[SPU] Using scale_srcw32_dstw32\n");
			scale_srcw32_dstw32();
		}
	}
	deprintf("[SPU] bilin_scaler_spu... done!\n");

	return 0;
}


inline static vector unsigned int vfloat_to_vuint(vector float vec_s) {
	vector unsigned int select_1 = spu_cmpgt(vec_0_1, vec_s);
	vec_s = spu_sel(vec_s, vec_0_1, select_1);

	vector unsigned int select_2 = spu_cmpgt(vec_s, vec_255);
	vec_s = spu_sel(vec_s, vec_255, select_2);
	return spu_convtu(vec_s,0);
}


void scale_srcw16_dstw16() {
	
	unsigned char* dst_addr = (unsigned char *)parms.dstBuffer;

	unsigned int src_width = parms.src_pixel_width;
	unsigned int src_height = parms.src_pixel_height;
	unsigned int dst_width = parms.dst_pixel_width;
	unsigned int dst_height = parms.dst_pixel_height;

	
	unsigned int src_linestride_y = src_width;
	unsigned int src_dbl_linestride_y = src_width<<1;
	unsigned int src_linestride_vu = src_width>>1;
	unsigned int src_dbl_linestride_vu = src_width;

	
	unsigned int scaled_src_linestride_y = dst_width;

	
	unsigned char* src_addr_y = parms.y_plane;
	unsigned char* src_addr_v = parms.v_plane;
	unsigned char* src_addr_u = parms.u_plane;

	
	unsigned char* precalc_src_addr_v = src_addr_v;
	unsigned char* precalc_src_addr_u = src_addr_u;

	unsigned int dst_picture_size = dst_width*dst_height;

	
	unsigned int dst_dbl_linestride_y = dst_width<<1;
	unsigned int dst_dbl_linestride_vu = dst_width>>1;

	
	unsigned char* dst_addr_main_memory_y = dst_addr;
	unsigned char* dst_addr_main_memory_v = dst_addr + dst_picture_size;
	unsigned char* dst_addr_main_memory_u = dst_addr_main_memory_v +(dst_picture_size>>2);

	
	vector float vf_x_scale = spu_splats( (float)src_width/(float)dst_width );
	float y_scale = (float)src_height/(float)dst_height;

	
	
	unsigned int curr_src_idx = 0;
	unsigned int curr_dst_idx = 0;
	unsigned int next_src_idx, next_dst_idx;

	
	unsigned int curr_interpl_y_upper = 0;
	unsigned int next_interpl_y_upper;
	unsigned int curr_interpl_y_lower, next_interpl_y_lower;
	
	unsigned int curr_interpl_vu = 0;
	unsigned int next_interpl_vu;

	
	vector float vf_curr_NSweight_y_upper = { 0.0f, 0.0f, 0.0f, 0.0f };
	vector float vf_next_NSweight_y_upper;
	vector float vf_curr_NSweight_y_lower, vf_next_NSweight_y_lower;
	vector float vf_curr_NSweight_vu = { 0.0f, 0.0f, 0.0f, 0.0f };
	vector float vf_next_NSweight_vu;

	
	float curr_src_y_upper = 0.0f, next_src_y_upper;
	float curr_src_y_lower, next_src_y_lower;
	float curr_src_vu = 0.0f, next_src_vu;

	
	unsigned int dst_y=0, dst_vu=0;

	
	unsigned int curr_lsoff_v = 0, next_lsoff_v;
	unsigned int curr_lsoff_u = 0, next_lsoff_u;

	
	curr_src_y_lower = ((float)curr_interpl_y_upper+1)*y_scale;
	curr_interpl_y_lower = (unsigned int)curr_src_y_lower;
	
	vf_curr_NSweight_y_lower = spu_splats( curr_src_y_lower-(float)curr_interpl_y_lower );


	
	
	mfc_get( y_plane[curr_src_idx], (unsigned int) src_addr_y, src_dbl_linestride_y, RETR_BUF, 0, 0 );
	mfc_get( y_plane[curr_src_idx]+src_dbl_linestride_y,
			(unsigned int) src_addr_y+(curr_interpl_y_lower*src_linestride_y),
			src_dbl_linestride_y,
			RETR_BUF,
			0, 0 );
	mfc_get( v_plane[curr_src_idx], (unsigned int) src_addr_v, src_dbl_linestride_vu, RETR_BUF, 0, 0 );
	mfc_get( u_plane[curr_src_idx], (unsigned int) src_addr_u, src_dbl_linestride_vu, RETR_BUF, 0, 0 );

	for( dst_vu=0; dst_vu<(dst_height>>1)-1; dst_vu++ ) {
		dst_y = dst_vu<<1;

		
		next_src_vu = ((float)dst_vu+1)*y_scale;
		next_src_y_upper = ((float)dst_y+2)*y_scale;
		next_src_y_lower = ((float)dst_y+3)*y_scale;

		next_interpl_vu = (unsigned int) next_src_vu;
		next_interpl_y_upper = (unsigned int) next_src_y_upper;
		next_interpl_y_lower = (unsigned int) next_src_y_lower;

		
		vf_next_NSweight_vu = spu_splats( next_src_vu-(float)next_interpl_vu );
		vf_next_NSweight_y_upper = spu_splats( next_src_y_upper-(float)next_interpl_y_upper );
		vf_next_NSweight_y_lower = spu_splats( next_src_y_lower-(float)next_interpl_y_lower );

		
		next_src_idx = curr_src_idx^1;
		next_dst_idx = curr_dst_idx^1;

		
		mfc_get( y_plane[next_src_idx],
				(unsigned int) src_addr_y+(next_interpl_y_upper*src_linestride_y),
				src_dbl_linestride_y,
				RETR_BUF+next_src_idx,
				0, 0 );
		mfc_get( y_plane[next_src_idx]+src_dbl_linestride_y,
				(unsigned int) src_addr_y+(next_interpl_y_lower*src_linestride_y),
				src_dbl_linestride_y,
				RETR_BUF+next_src_idx,
				0, 0 );

		
		precalc_src_addr_v = src_addr_v+(next_interpl_vu*src_linestride_vu);
		next_lsoff_v = ((unsigned int)precalc_src_addr_v)&0x0F;
		mfc_get( v_plane[next_src_idx],
				((unsigned int) precalc_src_addr_v)&0xFFFFFFF0,
				src_dbl_linestride_vu+(next_lsoff_v<<1),
				RETR_BUF+next_src_idx,
				0, 0 );
		
		precalc_src_addr_u = src_addr_u+(next_interpl_vu*src_linestride_vu);
		next_lsoff_u = ((unsigned int)precalc_src_addr_u)&0x0F;
		mfc_get( u_plane[next_src_idx],
				((unsigned int) precalc_src_addr_u)&0xFFFFFFF0,
				src_dbl_linestride_vu+(next_lsoff_v<<1),
				RETR_BUF+next_src_idx,
				0, 0 );

		DMA_WAIT_TAG( (RETR_BUF+curr_src_idx) );

		
		
		bilinear_scale_line_w16( y_plane[curr_src_idx],
				scaled_y_plane[curr_src_idx],
				dst_width,
				vf_x_scale,
				vf_curr_NSweight_y_upper,
				src_linestride_y );
		
		bilinear_scale_line_w16( y_plane[curr_src_idx]+src_dbl_linestride_y,
				scaled_y_plane[curr_src_idx]+scaled_src_linestride_y,
				dst_width,
				vf_x_scale,
				vf_curr_NSweight_y_lower,
				src_linestride_y );
		
		bilinear_scale_line_w8( v_plane[curr_src_idx]+curr_lsoff_v,
				scaled_v_plane[curr_src_idx],
				dst_width>>1,
				vf_x_scale,
				vf_curr_NSweight_vu,
				src_linestride_vu );
		
		bilinear_scale_line_w8( u_plane[curr_src_idx]+curr_lsoff_u,
				scaled_u_plane[curr_src_idx],
				dst_width>>1,
				vf_x_scale,
				vf_curr_NSweight_vu,
				src_linestride_vu );


		
		
		DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );

		
		
		
		
		
		mfc_put(	scaled_y_plane[curr_src_idx],					
				(unsigned int)dst_addr_main_memory_y + (dst_vu*dst_dbl_linestride_y),	
				dst_dbl_linestride_y,						
				STR_BUF+curr_dst_idx,						
				0, 0 );

		mfc_put(	scaled_v_plane[curr_src_idx],					
				(unsigned int)dst_addr_main_memory_v + (dst_vu*dst_dbl_linestride_vu),	
				dst_dbl_linestride_vu,						
				STR_BUF+curr_dst_idx,						
				0, 0 );

		mfc_put(	scaled_u_plane[curr_src_idx],					
				(unsigned int)dst_addr_main_memory_u + (dst_vu*dst_dbl_linestride_vu),	
				dst_dbl_linestride_vu,						
				STR_BUF+curr_dst_idx,						
				0, 0 );
		


		
		curr_src_idx = next_src_idx;
		curr_dst_idx = next_dst_idx;

		curr_interpl_y_upper = next_interpl_y_upper;
		curr_interpl_y_lower = next_interpl_y_lower;
		curr_interpl_vu = next_interpl_vu;

		vf_curr_NSweight_y_upper = vf_curr_NSweight_y_upper;
		vf_curr_NSweight_y_lower = vf_curr_NSweight_y_lower;
		vf_curr_NSweight_vu = vf_next_NSweight_vu;

		curr_src_y_upper = next_src_y_upper;
		curr_src_y_lower = next_src_y_lower;
		curr_src_vu = next_src_vu;

		curr_lsoff_v = next_lsoff_v;
		curr_lsoff_u = next_lsoff_u;
	}



	DMA_WAIT_TAG( (RETR_BUF+curr_src_idx) );

	
	
	bilinear_scale_line_w16( y_plane[curr_src_idx],
			scaled_y_plane[curr_src_idx],
			dst_width,
			vf_x_scale,
			vf_curr_NSweight_y_upper,
			src_linestride_y );
	
	bilinear_scale_line_w16( y_plane[curr_src_idx]+src_dbl_linestride_y,
			scaled_y_plane[curr_src_idx]+scaled_src_linestride_y,
			dst_width,
			vf_x_scale,
			vf_curr_NSweight_y_lower,
			src_linestride_y );
	
	bilinear_scale_line_w8( v_plane[curr_src_idx]+curr_lsoff_v,
			scaled_v_plane[curr_src_idx],
			dst_width>>1,
			vf_x_scale,
			vf_curr_NSweight_vu,
			src_linestride_vu );
	
	bilinear_scale_line_w8( u_plane[curr_src_idx]+curr_lsoff_u,
			scaled_u_plane[curr_src_idx],
			dst_width>>1,
			vf_x_scale,
			vf_curr_NSweight_vu,
			src_linestride_vu );


	
	
	DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );

	
	
	
	
	
	mfc_put(	scaled_y_plane[curr_src_idx],					
			(unsigned int)dst_addr_main_memory_y + (dst_vu*dst_dbl_linestride_y),	
			dst_dbl_linestride_y,						
			STR_BUF+curr_dst_idx,						
			0, 0 );

	mfc_put(	scaled_v_plane[curr_src_idx],					
			(unsigned int)dst_addr_main_memory_v + (dst_vu*dst_dbl_linestride_vu),	
			dst_dbl_linestride_vu,						
			STR_BUF+curr_dst_idx,						
			0, 0 );

	mfc_put(	scaled_u_plane[curr_src_idx],					
			(unsigned int)dst_addr_main_memory_u + (dst_vu*dst_dbl_linestride_vu),	
			dst_dbl_linestride_vu,						
			STR_BUF+curr_dst_idx,						
			0, 0 );

	
	DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );
	
}


void scale_srcw16_dstw32() {
	
	unsigned char* dst_addr = (unsigned char *)parms.dstBuffer;

	unsigned int src_width = parms.src_pixel_width;
	unsigned int src_height = parms.src_pixel_height;
	unsigned int dst_width = parms.dst_pixel_width;
	unsigned int dst_height = parms.dst_pixel_height;

	
	unsigned int src_linestride_y = src_width;
	unsigned int src_dbl_linestride_y = src_width<<1;
	unsigned int src_linestride_vu = src_width>>1;
	unsigned int src_dbl_linestride_vu = src_width;
	
	unsigned int scaled_src_linestride_y = dst_width;

	
	unsigned char* src_addr_y = parms.y_plane;
	unsigned char* src_addr_v = parms.v_plane;
	unsigned char* src_addr_u = parms.u_plane;

	unsigned int dst_picture_size = dst_width*dst_height;

	
	unsigned int dst_dbl_linestride_y = dst_width<<1;
	unsigned int dst_dbl_linestride_vu = dst_width>>1;

	
	unsigned char* dst_addr_main_memory_y = dst_addr;
	unsigned char* dst_addr_main_memory_v = dst_addr + dst_picture_size;
	unsigned char* dst_addr_main_memory_u = dst_addr_main_memory_v +(dst_picture_size>>2);


	
	unsigned char* precalc_src_addr_v = src_addr_v;
	unsigned char* precalc_src_addr_u = src_addr_u;

	
	vector float vf_x_scale = spu_splats( (float)src_width/(float)dst_width );
	float y_scale = (float)src_height/(float)dst_height;

	
	
	unsigned int curr_src_idx = 0;
	unsigned int curr_dst_idx = 0;
	unsigned int next_src_idx, next_dst_idx;

	
	unsigned int curr_interpl_y_upper = 0;
	unsigned int next_interpl_y_upper;
	unsigned int curr_interpl_y_lower, next_interpl_y_lower;
	
	unsigned int curr_interpl_vu = 0;
	unsigned int next_interpl_vu;

	
	vector float vf_curr_NSweight_y_upper = { 0.0f, 0.0f, 0.0f, 0.0f };
	vector float vf_next_NSweight_y_upper;
	vector float vf_curr_NSweight_y_lower, vf_next_NSweight_y_lower;
	vector float vf_curr_NSweight_vu = { 0.0f, 0.0f, 0.0f, 0.0f };
	vector float vf_next_NSweight_vu;

	
	float curr_src_y_upper = 0.0f, next_src_y_upper;
	float curr_src_y_lower, next_src_y_lower;
	float curr_src_vu = 0.0f, next_src_vu;

	
	unsigned int dst_y=0, dst_vu=0;

	
	unsigned int curr_lsoff_v = 0, next_lsoff_v;
	unsigned int curr_lsoff_u = 0, next_lsoff_u;

	
	curr_src_y_lower = ((float)curr_interpl_y_upper+1)*y_scale;
	curr_interpl_y_lower = (unsigned int)curr_src_y_lower;
	
	vf_curr_NSweight_y_lower = spu_splats( curr_src_y_lower-(float)curr_interpl_y_lower );


	
	
	mfc_get( y_plane[curr_src_idx], (unsigned int) src_addr_y, src_dbl_linestride_y, RETR_BUF, 0, 0 );
	mfc_get( y_plane[curr_src_idx]+src_dbl_linestride_y,
			(unsigned int) src_addr_y+(curr_interpl_y_lower*src_linestride_y),
			src_dbl_linestride_y,
			RETR_BUF,
			0, 0 );
	mfc_get( v_plane[curr_src_idx], (unsigned int) src_addr_v, src_dbl_linestride_vu, RETR_BUF, 0, 0 );
	mfc_get( u_plane[curr_src_idx], (unsigned int) src_addr_u, src_dbl_linestride_vu, RETR_BUF, 0, 0 );

	
	
	
	
	for( dst_vu=0; dst_vu<(dst_height>>1)-1; dst_vu++ ) {
		dst_y = dst_vu<<1;

		
		next_src_vu = ((float)dst_vu+1)*y_scale;
		next_src_y_upper = ((float)dst_y+2)*y_scale;
		next_src_y_lower = ((float)dst_y+3)*y_scale;

		next_interpl_vu = (unsigned int) next_src_vu;
		next_interpl_y_upper = (unsigned int) next_src_y_upper;
		next_interpl_y_lower = (unsigned int) next_src_y_lower;

		
		vf_next_NSweight_vu = spu_splats( next_src_vu-(float)next_interpl_vu );
		vf_next_NSweight_y_upper = spu_splats( next_src_y_upper-(float)next_interpl_y_upper );
		vf_next_NSweight_y_lower = spu_splats( next_src_y_lower-(float)next_interpl_y_lower );

		
		next_src_idx = curr_src_idx^1;
		next_dst_idx = curr_dst_idx^1;

		
		mfc_get( y_plane[next_src_idx],
				(unsigned int) src_addr_y+(next_interpl_y_upper*src_linestride_y),
				src_dbl_linestride_y,
				RETR_BUF+next_src_idx,
				0, 0 );
		mfc_get( y_plane[next_src_idx]+src_dbl_linestride_y,
				(unsigned int) src_addr_y+(next_interpl_y_lower*src_linestride_y),
				src_dbl_linestride_y,
				RETR_BUF+next_src_idx,
				0, 0 );

		
		precalc_src_addr_v = src_addr_v+(next_interpl_vu*src_linestride_vu);
		next_lsoff_v = ((unsigned int)precalc_src_addr_v)&0x0F;
		mfc_get( v_plane[next_src_idx],
				((unsigned int) precalc_src_addr_v)&0xFFFFFFF0,
				src_dbl_linestride_vu+(next_lsoff_v<<1),
				RETR_BUF+next_src_idx,
				0, 0 );
		
		precalc_src_addr_u = src_addr_u+(next_interpl_vu*src_linestride_vu);
		next_lsoff_u = ((unsigned int)precalc_src_addr_u)&0x0F;
		mfc_get( u_plane[next_src_idx],
				((unsigned int) precalc_src_addr_u)&0xFFFFFFF0,
				src_dbl_linestride_vu+(next_lsoff_v<<1),
				RETR_BUF+next_src_idx,
				0, 0 );

		DMA_WAIT_TAG( (RETR_BUF+curr_src_idx) );

		
		
		bilinear_scale_line_w16( y_plane[curr_src_idx],
				scaled_y_plane[curr_src_idx],
				dst_width,
				vf_x_scale,
				vf_curr_NSweight_y_upper,
				src_linestride_y );
		
		bilinear_scale_line_w16( y_plane[curr_src_idx]+src_dbl_linestride_y,
				scaled_y_plane[curr_src_idx]+scaled_src_linestride_y,
				dst_width,
				vf_x_scale,
				vf_curr_NSweight_y_lower,
				src_linestride_y );
		
		bilinear_scale_line_w8( v_plane[curr_src_idx]+curr_lsoff_v,
				scaled_v_plane[curr_src_idx],
				dst_width>>1,
				vf_x_scale,
				vf_curr_NSweight_vu,
				src_linestride_vu );
		
		bilinear_scale_line_w8( u_plane[curr_src_idx]+curr_lsoff_u,
				scaled_u_plane[curr_src_idx],
				dst_width>>1,
				vf_x_scale,
				vf_curr_NSweight_vu,
				src_linestride_vu );

		
		DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );

		
		
		
		
		

		mfc_put(	scaled_y_plane[curr_src_idx],							
				(unsigned int)  dst_addr_main_memory_y + (dst_vu*dst_dbl_linestride_y),	
				dst_dbl_linestride_y,								
				STR_BUF+curr_dst_idx,								
				0, 0 );

		mfc_put(	scaled_v_plane[curr_src_idx],							
				(unsigned int) dst_addr_main_memory_v + (dst_vu*dst_dbl_linestride_vu),	
				dst_dbl_linestride_vu,								
				STR_BUF+curr_dst_idx,								
				0, 0 );

		mfc_put(	scaled_u_plane[curr_src_idx],							
				(unsigned int)  dst_addr_main_memory_u + (dst_vu*dst_dbl_linestride_vu),	
				dst_dbl_linestride_vu,								
				STR_BUF+curr_dst_idx,								
				0, 0 );
		


		
		curr_src_idx = next_src_idx;
		curr_dst_idx = next_dst_idx;

		curr_interpl_y_upper = next_interpl_y_upper;
		curr_interpl_y_lower = next_interpl_y_lower;
		curr_interpl_vu = next_interpl_vu;

		vf_curr_NSweight_y_upper = vf_curr_NSweight_y_upper;
		vf_curr_NSweight_y_lower = vf_curr_NSweight_y_lower;
		vf_curr_NSweight_vu = vf_next_NSweight_vu;

		curr_src_y_upper = next_src_y_upper;
		curr_src_y_lower = next_src_y_lower;
		curr_src_vu = next_src_vu;

		curr_lsoff_v = next_lsoff_v;
		curr_lsoff_u = next_lsoff_u;
	}



	DMA_WAIT_TAG( (RETR_BUF+curr_src_idx) );

	
	
	bilinear_scale_line_w16( y_plane[curr_src_idx],
			scaled_y_plane[curr_src_idx],
			dst_width,
			vf_x_scale,
			vf_curr_NSweight_y_upper,
			src_linestride_y );
	
	bilinear_scale_line_w16( y_plane[curr_src_idx]+src_dbl_linestride_y,
			scaled_y_plane[curr_src_idx]+scaled_src_linestride_y,
			dst_width,
			vf_x_scale,
			vf_curr_NSweight_y_lower,
			src_linestride_y );
	
	bilinear_scale_line_w8( v_plane[curr_src_idx]+curr_lsoff_v,
			scaled_v_plane[curr_src_idx],
			dst_width>>1,
			vf_x_scale,
			vf_curr_NSweight_vu,
			src_linestride_vu );
	
	bilinear_scale_line_w8( u_plane[curr_src_idx]+curr_lsoff_u,
			scaled_u_plane[curr_src_idx],
			dst_width>>1,
			vf_x_scale,
			vf_curr_NSweight_vu,
			src_linestride_vu );

	
	DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );

	
	
	
	
	

	mfc_put(	scaled_y_plane[curr_src_idx],							
			(unsigned int)  dst_addr_main_memory_y + (dst_vu*dst_dbl_linestride_y),	
			dst_dbl_linestride_y,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	mfc_put(	scaled_v_plane[curr_src_idx],							
			(unsigned int) dst_addr_main_memory_v + (dst_vu*dst_dbl_linestride_vu),	
			dst_dbl_linestride_vu,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	mfc_put(	scaled_u_plane[curr_src_idx],							
			(unsigned int)  dst_addr_main_memory_u + (dst_vu*dst_dbl_linestride_vu),	
			dst_dbl_linestride_vu,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	
	DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );
	
}


void scale_srcw32_dstw16() {
	
	unsigned char* dst_addr = (unsigned char *)parms.dstBuffer;

	unsigned int src_width = parms.src_pixel_width;
	unsigned int src_height = parms.src_pixel_height;
	unsigned int dst_width = parms.dst_pixel_width;
	unsigned int dst_height = parms.dst_pixel_height;

	
	unsigned int src_linestride_y = src_width;
	unsigned int src_dbl_linestride_y = src_width<<1;
	unsigned int src_linestride_vu = src_width>>1;
	unsigned int src_dbl_linestride_vu = src_width;
	
	unsigned int scaled_src_linestride_y = dst_width;

	
	unsigned char* src_addr_y = parms.y_plane;
	unsigned char* src_addr_v = parms.v_plane;
	unsigned char* src_addr_u = parms.u_plane;

	unsigned int dst_picture_size = dst_width*dst_height;

	
	unsigned int dst_dbl_linestride_y = dst_width<<1;
	unsigned int dst_dbl_linestride_vu = dst_width>>1;

	
	unsigned char* dst_addr_main_memory_y = dst_addr;
	unsigned char* dst_addr_main_memory_v = dst_addr + dst_picture_size;
	unsigned char* dst_addr_main_memory_u = dst_addr_main_memory_v +(dst_picture_size>>2);

	
	vector float vf_x_scale = spu_splats( (float)src_width/(float)dst_width );
	float y_scale = (float)src_height/(float)dst_height;

	
	
	unsigned int curr_src_idx = 0;
	unsigned int curr_dst_idx = 0;
	unsigned int next_src_idx, next_dst_idx;

	
	unsigned int curr_interpl_y_upper = 0;
	unsigned int next_interpl_y_upper;
	unsigned int curr_interpl_y_lower, next_interpl_y_lower;
	
	unsigned int curr_interpl_vu = 0;
	unsigned int next_interpl_vu;

	
	vector float vf_curr_NSweight_y_upper = { 0.0f, 0.0f, 0.0f, 0.0f };
	vector float vf_next_NSweight_y_upper;
	vector float vf_curr_NSweight_y_lower, vf_next_NSweight_y_lower;
	vector float vf_curr_NSweight_vu = { 0.0f, 0.0f, 0.0f, 0.0f };
	vector float vf_next_NSweight_vu;

	
	float curr_src_y_upper = 0.0f, next_src_y_upper;
	float curr_src_y_lower, next_src_y_lower;
	float curr_src_vu = 0.0f, next_src_vu;

	
	unsigned int dst_y=0, dst_vu=0;

	
	curr_src_y_lower = ((float)curr_interpl_y_upper+1)*y_scale;
	curr_interpl_y_lower = (unsigned int)curr_src_y_lower;
	
	vf_curr_NSweight_y_lower = spu_splats( curr_src_y_lower-(float)curr_interpl_y_lower );


	
	
	mfc_get( y_plane[curr_src_idx], (unsigned int) src_addr_y, src_dbl_linestride_y, RETR_BUF, 0, 0 );
	mfc_get( y_plane[curr_src_idx]+src_dbl_linestride_y,
			(unsigned int) src_addr_y+(curr_interpl_y_lower*src_linestride_y),
			src_dbl_linestride_y,
			RETR_BUF,
			0, 0 );
	mfc_get( v_plane[curr_src_idx], (unsigned int) src_addr_v, src_dbl_linestride_vu, RETR_BUF, 0, 0 );
	mfc_get( u_plane[curr_src_idx], (unsigned int) src_addr_u, src_dbl_linestride_vu, RETR_BUF, 0, 0 );

	
	
	
	
	for( dst_vu=0; dst_vu<(dst_height>>1)-1; dst_vu++ ) {
		dst_y = dst_vu<<1;

		
		next_src_vu = ((float)dst_vu+1)*y_scale;
		next_src_y_upper = ((float)dst_y+2)*y_scale;
		next_src_y_lower = ((float)dst_y+3)*y_scale;

		next_interpl_vu = (unsigned int) next_src_vu;
		next_interpl_y_upper = (unsigned int) next_src_y_upper;
		next_interpl_y_lower = (unsigned int) next_src_y_lower;

		
		vf_next_NSweight_vu = spu_splats( next_src_vu-(float)next_interpl_vu );
		vf_next_NSweight_y_upper = spu_splats( next_src_y_upper-(float)next_interpl_y_upper );
		vf_next_NSweight_y_lower = spu_splats( next_src_y_lower-(float)next_interpl_y_lower );

		
		next_src_idx = curr_src_idx^1;
		next_dst_idx = curr_dst_idx^1;

		
		mfc_get( y_plane[next_src_idx],
				(unsigned int) src_addr_y+(next_interpl_y_upper*src_linestride_y),
				src_dbl_linestride_y,
				RETR_BUF+next_src_idx,
				0, 0 );
		mfc_get( y_plane[next_src_idx]+src_dbl_linestride_y,
				(unsigned int) src_addr_y+(next_interpl_y_lower*src_linestride_y),
				src_dbl_linestride_y,
				RETR_BUF+next_src_idx,
				0, 0 );

		
		mfc_get( v_plane[next_src_idx],
				(unsigned int) src_addr_v+(next_interpl_vu*src_linestride_vu),
				src_dbl_linestride_vu,
				RETR_BUF+next_src_idx,
				0, 0 );
		
		mfc_get( u_plane[next_src_idx],
				(unsigned int) src_addr_u+(next_interpl_vu*src_linestride_vu),
				src_dbl_linestride_vu,
				RETR_BUF+next_src_idx,
				0, 0 );

		DMA_WAIT_TAG( (RETR_BUF+curr_src_idx) );

		
		
		bilinear_scale_line_w16( y_plane[curr_src_idx],
				scaled_y_plane[curr_src_idx],
				dst_width,
				vf_x_scale,
				vf_curr_NSweight_y_upper,
				src_linestride_y );
		
		bilinear_scale_line_w16( y_plane[curr_src_idx]+src_dbl_linestride_y,
				scaled_y_plane[curr_src_idx]+scaled_src_linestride_y,
				dst_width,
				vf_x_scale,
				vf_curr_NSweight_y_lower,
				src_linestride_y );
		
		bilinear_scale_line_w16( v_plane[curr_src_idx],
				scaled_v_plane[curr_src_idx],
				dst_width>>1,
				vf_x_scale,
				vf_curr_NSweight_vu,
				src_linestride_vu );
		
		bilinear_scale_line_w16( u_plane[curr_src_idx],
				scaled_u_plane[curr_src_idx],
				dst_width>>1,
				vf_x_scale,
				vf_curr_NSweight_vu,
				src_linestride_vu );

		
		DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );

		
		
		
		
		

		mfc_put(	scaled_y_plane[curr_src_idx],							
				(unsigned int)  dst_addr_main_memory_y + (dst_vu*dst_dbl_linestride_y),	
				dst_dbl_linestride_y,								
				STR_BUF+curr_dst_idx,								
				0, 0 );

		mfc_put(	scaled_v_plane[curr_src_idx],							
				(unsigned int) dst_addr_main_memory_v + (dst_vu*dst_dbl_linestride_vu),	
				dst_dbl_linestride_vu,								
				STR_BUF+curr_dst_idx,								
				0, 0 );

		mfc_put(	scaled_u_plane[curr_src_idx],							
				(unsigned int)  dst_addr_main_memory_u + (dst_vu*dst_dbl_linestride_vu), 
				dst_dbl_linestride_vu,								
				STR_BUF+curr_dst_idx,								
				0, 0 );
		


		
		curr_src_idx = next_src_idx;
		curr_dst_idx = next_dst_idx;

		curr_interpl_y_upper = next_interpl_y_upper;
		curr_interpl_y_lower = next_interpl_y_lower;
		curr_interpl_vu = next_interpl_vu;

		vf_curr_NSweight_y_upper = vf_curr_NSweight_y_upper;
		vf_curr_NSweight_y_lower = vf_curr_NSweight_y_lower;
		vf_curr_NSweight_vu = vf_next_NSweight_vu;

		curr_src_y_upper = next_src_y_upper;
		curr_src_y_lower = next_src_y_lower;
		curr_src_vu = next_src_vu;
	}



	DMA_WAIT_TAG( (RETR_BUF+curr_src_idx) );

	
	
	bilinear_scale_line_w16( y_plane[curr_src_idx],
			scaled_y_plane[curr_src_idx],
			dst_width,
			vf_x_scale,
			vf_curr_NSweight_y_upper,
			src_linestride_y );
	
	bilinear_scale_line_w16( y_plane[curr_src_idx]+src_dbl_linestride_y,
			scaled_y_plane[curr_src_idx]+scaled_src_linestride_y,
			dst_width,
			vf_x_scale,
			vf_curr_NSweight_y_lower,
			src_linestride_y );
	
	bilinear_scale_line_w16( v_plane[curr_src_idx],
			scaled_v_plane[curr_src_idx],
			dst_width>>1,
			vf_x_scale,
			vf_curr_NSweight_vu,
			src_linestride_vu );
	
	bilinear_scale_line_w16( u_plane[curr_src_idx],
			scaled_u_plane[curr_src_idx],
			dst_width>>1,
			vf_x_scale,
			vf_curr_NSweight_vu,
			src_linestride_vu );


	
	DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );

	
	
	
	
	

	mfc_put(	scaled_y_plane[curr_src_idx],							
			(unsigned int)  dst_addr_main_memory_y + (dst_vu*dst_dbl_linestride_y),	
			dst_dbl_linestride_y,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	mfc_put(	scaled_v_plane[curr_src_idx],							
			(unsigned int) dst_addr_main_memory_v + (dst_vu*dst_dbl_linestride_vu),	
			dst_dbl_linestride_vu,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	mfc_put(	scaled_u_plane[curr_src_idx],							
			(unsigned int)  dst_addr_main_memory_u + (dst_vu*dst_dbl_linestride_vu), 
			dst_dbl_linestride_vu,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	
	DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );
	
}


void scale_srcw32_dstw32() {
	
	unsigned char* dst_addr = (unsigned char *)parms.dstBuffer;

	unsigned int src_width = parms.src_pixel_width;
	unsigned int src_height = parms.src_pixel_height;
	unsigned int dst_width = parms.dst_pixel_width;
	unsigned int dst_height = parms.dst_pixel_height;

	
	unsigned int src_linestride_y = src_width;
	unsigned int src_dbl_linestride_y = src_width<<1;
	unsigned int src_linestride_vu = src_width>>1;
	unsigned int src_dbl_linestride_vu = src_width;

	
	unsigned int scaled_src_linestride_y = dst_width;

	
	unsigned char* src_addr_y = parms.y_plane;
	unsigned char* src_addr_v = parms.v_plane;
	unsigned char* src_addr_u = parms.u_plane;

	unsigned int dst_picture_size = dst_width*dst_height;

	
	unsigned int dst_dbl_linestride_y = dst_width<<1;
	unsigned int dst_dbl_linestride_vu = dst_width>>1;

	
	unsigned char* dst_addr_main_memory_y = dst_addr;
	unsigned char* dst_addr_main_memory_v = dst_addr + dst_picture_size;
	unsigned char* dst_addr_main_memory_u = dst_addr_main_memory_v +(dst_picture_size>>2);

	
	vector float vf_x_scale = spu_splats( (float)src_width/(float)dst_width );
	float y_scale = (float)src_height/(float)dst_height;

	
	
	unsigned int curr_src_idx = 0;
	unsigned int curr_dst_idx = 0;
	unsigned int next_src_idx, next_dst_idx;

	
	unsigned int curr_interpl_y_upper = 0;
	unsigned int next_interpl_y_upper;
	unsigned int curr_interpl_y_lower, next_interpl_y_lower;
	
	unsigned int curr_interpl_vu = 0;
	unsigned int next_interpl_vu;

	
	vector float vf_curr_NSweight_y_upper = { 0.0f, 0.0f, 0.0f, 0.0f };
	vector float vf_next_NSweight_y_upper;
	vector float vf_curr_NSweight_y_lower, vf_next_NSweight_y_lower;
	vector float vf_curr_NSweight_vu = { 0.0f, 0.0f, 0.0f, 0.0f };
	vector float vf_next_NSweight_vu;

	
	float curr_src_y_upper = 0.0f, next_src_y_upper;
	float curr_src_y_lower, next_src_y_lower;
	float curr_src_vu = 0.0f, next_src_vu;

	
	unsigned int dst_y=0, dst_vu=0;

	
	curr_src_y_lower = ((float)curr_interpl_y_upper+1)*y_scale;
	curr_interpl_y_lower = (unsigned int)curr_src_y_lower;
	
	vf_curr_NSweight_y_lower = spu_splats( curr_src_y_lower-(float)curr_interpl_y_lower );


	
	
	mfc_get( y_plane[curr_src_idx], (unsigned int) src_addr_y, src_dbl_linestride_y, RETR_BUF, 0, 0 );
	mfc_get( y_plane[curr_src_idx]+src_dbl_linestride_y,
			(unsigned int) src_addr_y+(curr_interpl_y_lower*src_linestride_y),
			src_dbl_linestride_y,
			RETR_BUF,
			0, 0 );
	mfc_get( v_plane[curr_src_idx], (unsigned int) src_addr_v, src_dbl_linestride_vu, RETR_BUF, 0, 0 );
	mfc_get( u_plane[curr_src_idx], (unsigned int) src_addr_u, src_dbl_linestride_vu, RETR_BUF, 0, 0 );

	
	
	
	
	for( dst_vu=0; dst_vu<(dst_height>>1)-1; dst_vu++ ) {
		dst_y = dst_vu<<1;

		
		next_src_vu = ((float)dst_vu+1)*y_scale;
		next_src_y_upper = ((float)dst_y+2)*y_scale;
		next_src_y_lower = ((float)dst_y+3)*y_scale;

		next_interpl_vu = (unsigned int) next_src_vu;
		next_interpl_y_upper = (unsigned int) next_src_y_upper;
		next_interpl_y_lower = (unsigned int) next_src_y_lower;

		
		vf_next_NSweight_vu = spu_splats( next_src_vu-(float)next_interpl_vu );
		vf_next_NSweight_y_upper = spu_splats( next_src_y_upper-(float)next_interpl_y_upper );
		vf_next_NSweight_y_lower = spu_splats( next_src_y_lower-(float)next_interpl_y_lower );

		
		next_src_idx = curr_src_idx^1;
		next_dst_idx = curr_dst_idx^1;

		
		mfc_get( y_plane[next_src_idx],
				(unsigned int) src_addr_y+(next_interpl_y_upper*src_linestride_y),
				src_dbl_linestride_y,
				RETR_BUF+next_src_idx,
				0, 0 );
		mfc_get( y_plane[next_src_idx]+src_dbl_linestride_y,
				(unsigned int) src_addr_y+(next_interpl_y_lower*src_linestride_y),
				src_dbl_linestride_y,
				RETR_BUF+next_src_idx,
				0, 0 );

		
		mfc_get( v_plane[next_src_idx],
				(unsigned int) src_addr_v+(next_interpl_vu*src_linestride_vu),
				src_dbl_linestride_vu,
				RETR_BUF+next_src_idx,
				0, 0 );
		
		mfc_get( u_plane[next_src_idx],
				(unsigned int) src_addr_u+(next_interpl_vu*src_linestride_vu),
				src_dbl_linestride_vu,
				RETR_BUF+next_src_idx,
				0, 0 );

		DMA_WAIT_TAG( (RETR_BUF+curr_src_idx) );

		
		
		bilinear_scale_line_w16( y_plane[curr_src_idx],
				scaled_y_plane[curr_src_idx],
				dst_width,
				vf_x_scale,
				vf_curr_NSweight_y_upper,
				src_linestride_y );
		
		bilinear_scale_line_w16( y_plane[curr_src_idx]+src_dbl_linestride_y,
				scaled_y_plane[curr_src_idx]+scaled_src_linestride_y,
				dst_width,
				vf_x_scale,
				vf_curr_NSweight_y_lower,
				src_linestride_y );
		
		bilinear_scale_line_w16( v_plane[curr_src_idx],
				scaled_v_plane[curr_src_idx],
				dst_width>>1,
				vf_x_scale,
				vf_curr_NSweight_vu,
				src_linestride_vu );
		
		bilinear_scale_line_w16( u_plane[curr_src_idx],
				scaled_u_plane[curr_src_idx],
				dst_width>>1,
				vf_x_scale,
				vf_curr_NSweight_vu,
				src_linestride_vu );



		
		
		DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );

		
		
		
		
		

		mfc_put(	scaled_y_plane[curr_src_idx],							
				(unsigned int) dst_addr_main_memory_y + (dst_vu*dst_dbl_linestride_y),	
				dst_dbl_linestride_y,								
				STR_BUF+curr_dst_idx,								
				0, 0 );

		mfc_put(	scaled_v_plane[curr_src_idx],							
				(unsigned int) dst_addr_main_memory_v + (dst_vu*dst_dbl_linestride_vu),	
				dst_dbl_linestride_vu,								
				STR_BUF+curr_dst_idx,								
				0, 0 );

		mfc_put(	scaled_u_plane[curr_src_idx],							
				(unsigned int) dst_addr_main_memory_u + (dst_vu*dst_dbl_linestride_vu),	
				dst_dbl_linestride_vu,								
				STR_BUF+curr_dst_idx,								
				0, 0 );
		


		
		curr_src_idx = next_src_idx;
		curr_dst_idx = next_dst_idx;

		curr_interpl_y_upper = next_interpl_y_upper;
		curr_interpl_y_lower = next_interpl_y_lower;
		curr_interpl_vu = next_interpl_vu;

		vf_curr_NSweight_y_upper = vf_curr_NSweight_y_upper;
		vf_curr_NSweight_y_lower = vf_curr_NSweight_y_lower;
		vf_curr_NSweight_vu = vf_next_NSweight_vu;

		curr_src_y_upper = next_src_y_upper;
		curr_src_y_lower = next_src_y_lower;
		curr_src_vu = next_src_vu;
	}



	DMA_WAIT_TAG( (RETR_BUF+curr_src_idx) );

	
	
	bilinear_scale_line_w16( y_plane[curr_src_idx],
			scaled_y_plane[curr_src_idx],
			dst_width,
			vf_x_scale,
			vf_curr_NSweight_y_upper,
			src_linestride_y );
	
	bilinear_scale_line_w16( y_plane[curr_src_idx]+src_dbl_linestride_y,
			scaled_y_plane[curr_src_idx]+scaled_src_linestride_y,
			dst_width,
			vf_x_scale,
			vf_curr_NSweight_y_lower,
			src_linestride_y );
	
	bilinear_scale_line_w16( v_plane[curr_src_idx],
			scaled_v_plane[curr_src_idx],
			dst_width>>1,
			vf_x_scale,
			vf_curr_NSweight_vu,
			src_linestride_vu );
	
	bilinear_scale_line_w16( u_plane[curr_src_idx],
			scaled_u_plane[curr_src_idx],
			dst_width>>1,
			vf_x_scale,
			vf_curr_NSweight_vu,
			src_linestride_vu );


	
	
	DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );

	
	
	
	
	

	mfc_put(	scaled_y_plane[curr_src_idx],							
			(unsigned int)  dst_addr_main_memory_y + (dst_vu*dst_dbl_linestride_y),	
			dst_dbl_linestride_y,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	mfc_put(	scaled_v_plane[curr_src_idx],							
			(unsigned int) dst_addr_main_memory_v + (dst_vu*dst_dbl_linestride_vu),	
			dst_dbl_linestride_vu,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	mfc_put(	scaled_u_plane[curr_src_idx],							
			(unsigned int)  dst_addr_main_memory_u + (dst_vu*dst_dbl_linestride_vu), 
			dst_dbl_linestride_vu,								
			STR_BUF+curr_dst_idx,								
			0, 0 );

	
	DMA_WAIT_TAG( (STR_BUF+curr_dst_idx) );
	
}


/*
 * bilinear_scale_line_w8()
 *
 * processes a line of yuv-input, width has to be a multiple of 8
 * scaled yuv-output is written to local store buffer
 *
 * @param src buffer for 2 lines input
 * @param dst_ buffer for 1 line output
 * @param dst_width the width of the destination line
 * @param vf_x_scale a float vector, at each entry is the x_scale-factor
 * @param vf_NSweight a float vector, at each position is the weight NORTH/SOUTH for the current line
 * @param src_linestride the stride of the srcline
 */
void bilinear_scale_line_w8( unsigned char* src, unsigned char* dst_, unsigned int dst_width, vector float vf_x_scale, vector float vf_NSweight, unsigned int src_linestride ) {

	unsigned char* dst = dst_;

	unsigned int dst_x;
	for( dst_x=0; dst_x<dst_width; dst_x+=8) {
		
		
		vector unsigned int vui_dst_x_tmp = spu_splats( dst_x );
		
		
		vector unsigned int vui_inc_dst_x_lower_range = { 0, 1, 2, 3 };
		vector unsigned int vui_inc_dst_x_upper_range = { 4, 5, 6, 7 };
		vector unsigned int vui_dst_x_lower_range = spu_add( vui_dst_x_tmp, vui_inc_dst_x_lower_range );
		vector unsigned int vui_dst_x_upper_range = spu_add( vui_dst_x_tmp, vui_inc_dst_x_upper_range );

		
		vector float vf_dst_x_lower_range = spu_convtf( vui_dst_x_lower_range, 0 );
		vector float vf_dst_x_upper_range = spu_convtf( vui_dst_x_upper_range, 0 );
		vector float vf_src_x_lower_range = spu_mul( vf_dst_x_lower_range, vf_x_scale );
		vector float vf_src_x_upper_range = spu_mul( vf_dst_x_upper_range, vf_x_scale );
		vector unsigned int vui_interpl_x_lower_range = spu_convtu( vf_src_x_lower_range, 0 );
		vector unsigned int vui_interpl_x_upper_range = spu_convtu( vf_src_x_upper_range, 0 );
		vector float vf_interpl_x_lower_range = spu_convtf( vui_interpl_x_lower_range, 0 );
		vector float vf_interpl_x_upper_range = spu_convtf( vui_interpl_x_upper_range, 0 );
		vector float vf_EWweight_lower_range = spu_sub( vf_src_x_lower_range, vf_interpl_x_lower_range );
		vector float vf_EWweight_upper_range = spu_sub( vf_src_x_upper_range, vf_interpl_x_upper_range );

		
		
		
		vector unsigned int vui_off_pixelNW_lower_range = vui_interpl_x_lower_range;
		vector unsigned int vui_off_pixelNW_upper_range = vui_interpl_x_upper_range;

		
		vector unsigned int vui_add_1 = { 1, 1, 1, 1 };
		vector unsigned int vui_off_pixelNE_lower_range = spu_add( vui_off_pixelNW_lower_range, vui_add_1 );
		vector unsigned int vui_off_pixelNE_upper_range = spu_add( vui_off_pixelNW_upper_range, vui_add_1 );

		
		vector unsigned int vui_srclinestride = spu_splats( src_linestride );
		vector unsigned int vui_off_pixelSW_lower_range = spu_add( vui_srclinestride, vui_off_pixelNW_lower_range );
		vector unsigned int vui_off_pixelSW_upper_range = spu_add( vui_srclinestride, vui_off_pixelNW_upper_range );

		
		vector unsigned int vui_off_pixelSE_lower_range = spu_add( vui_srclinestride, vui_off_pixelNE_lower_range );
		vector unsigned int vui_off_pixelSE_upper_range = spu_add( vui_srclinestride, vui_off_pixelNE_upper_range );

		
		vector unsigned int vui_src_ls = spu_splats( (unsigned int) src );
		vector unsigned int vui_addr_pixelNW_lower_range = spu_add( vui_src_ls, vui_off_pixelNW_lower_range );
		vector unsigned int vui_addr_pixelNW_upper_range = spu_add( vui_src_ls, vui_off_pixelNW_upper_range );
		vector unsigned int vui_addr_pixelNE_lower_range = spu_add( vui_src_ls, vui_off_pixelNE_lower_range );
		vector unsigned int vui_addr_pixelNE_upper_range = spu_add( vui_src_ls, vui_off_pixelNE_upper_range );

		vector unsigned int vui_addr_pixelSW_lower_range = spu_add( vui_src_ls, vui_off_pixelSW_lower_range );
		vector unsigned int vui_addr_pixelSW_upper_range = spu_add( vui_src_ls, vui_off_pixelSW_upper_range );
		vector unsigned int vui_addr_pixelSE_lower_range = spu_add( vui_src_ls, vui_off_pixelSE_lower_range );
		vector unsigned int vui_addr_pixelSE_upper_range = spu_add( vui_src_ls, vui_off_pixelSE_upper_range );

		
		
		
		
		vector unsigned char null_vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		vector unsigned char vuc_pixel_NW_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_lower_range, 0 )), null_vector, 3 );
		vuc_pixel_NW_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_lower_range, 1 )),
				vuc_pixel_NW_lower_range, 7 );
		vuc_pixel_NW_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_lower_range, 2 )),
				vuc_pixel_NW_lower_range, 11 );
		vuc_pixel_NW_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_lower_range, 3 )),
				vuc_pixel_NW_lower_range, 15 );

		vector unsigned char vuc_pixel_NW_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_upper_range, 0 )), null_vector, 3 );
		vuc_pixel_NW_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_upper_range, 1 )),
				vuc_pixel_NW_upper_range, 7 );
		vuc_pixel_NW_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_upper_range, 2 )),
				vuc_pixel_NW_upper_range, 11 );
		vuc_pixel_NW_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_upper_range, 3 )),
				vuc_pixel_NW_upper_range, 15 );

		
		vector unsigned char vuc_pixel_NE_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_lower_range, 0 )), null_vector, 3 );
		vuc_pixel_NE_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_lower_range, 1 )),
				vuc_pixel_NE_lower_range, 7 );
		vuc_pixel_NE_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_lower_range, 2 )),
				vuc_pixel_NE_lower_range, 11 );
		vuc_pixel_NE_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_lower_range, 3 )),
				vuc_pixel_NE_lower_range, 15 );

		vector unsigned char vuc_pixel_NE_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_upper_range, 0 )), null_vector, 3 );
		vuc_pixel_NE_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_upper_range, 1 )),
				vuc_pixel_NE_upper_range, 7 );
		vuc_pixel_NE_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_upper_range, 2 )),
				vuc_pixel_NE_upper_range, 11 );
		vuc_pixel_NE_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_upper_range, 3 )),
				vuc_pixel_NE_upper_range, 15 );


		
		vector unsigned char vuc_pixel_SW_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_lower_range, 0 )), null_vector, 3 );
		vuc_pixel_SW_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_lower_range, 1 )),
				vuc_pixel_SW_lower_range, 7 );
		vuc_pixel_SW_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_lower_range, 2 )),
				vuc_pixel_SW_lower_range, 11 );
		vuc_pixel_SW_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_lower_range, 3 )),
				vuc_pixel_SW_lower_range, 15 );

		vector unsigned char vuc_pixel_SW_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_upper_range, 0 )), null_vector, 3 );
		vuc_pixel_SW_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_upper_range, 1 )),
				vuc_pixel_SW_upper_range, 7 );
		vuc_pixel_SW_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_upper_range, 2 )),
				vuc_pixel_SW_upper_range, 11 );
		vuc_pixel_SW_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_upper_range, 3 )),
				vuc_pixel_SW_upper_range, 15 );

		
		vector unsigned char vuc_pixel_SE_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_lower_range, 0 )), null_vector, 3 );
		vuc_pixel_SE_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_lower_range, 1 )),
				vuc_pixel_SE_lower_range, 7 );
		vuc_pixel_SE_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_lower_range, 2 )),
				vuc_pixel_SE_lower_range, 11 );
		vuc_pixel_SE_lower_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_lower_range, 3 )),
				vuc_pixel_SE_lower_range, 15 );

		vector unsigned char vuc_pixel_SE_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_upper_range, 0 )), null_vector, 3 );
		vuc_pixel_SE_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_upper_range, 1 )),
				vuc_pixel_SE_upper_range, 7 );
		vuc_pixel_SE_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_upper_range, 2 )),
				vuc_pixel_SE_upper_range, 11 );
		vuc_pixel_SE_upper_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_upper_range, 3 )),
				vuc_pixel_SE_upper_range, 15 );


		
		vector float vf_pixel_NW_lower_range = spu_convtf( (vector unsigned int) vuc_pixel_NW_lower_range, 0 );
		vector float vf_pixel_NW_upper_range = spu_convtf( (vector unsigned int) vuc_pixel_NW_upper_range, 0 );

		vector float vf_pixel_SW_lower_range = spu_convtf( (vector unsigned int) vuc_pixel_SW_lower_range, 0 );
		vector float vf_pixel_SW_upper_range = spu_convtf( (vector unsigned int) vuc_pixel_SW_upper_range, 0 );

		vector float vf_pixel_NE_lower_range = spu_convtf( (vector unsigned int) vuc_pixel_NE_lower_range, 0 );
		vector float vf_pixel_NE_upper_range = spu_convtf( (vector unsigned int) vuc_pixel_NE_upper_range, 0 );

		vector float vf_pixel_SE_lower_range = spu_convtf( (vector unsigned int) vuc_pixel_SE_lower_range, 0 );
		vector float vf_pixel_SE_upper_range = spu_convtf( (vector unsigned int) vuc_pixel_SE_upper_range, 0 );



		
		
		
		
		vector float vf_EWtop_lower_range_tmp = spu_sub( vf_pixel_NE_lower_range, vf_pixel_NW_lower_range );
		vector float vf_EWtop_lower_range = spu_madd( vf_EWweight_lower_range,
								vf_EWtop_lower_range_tmp,
								vf_pixel_NW_lower_range );

		
		vector float vf_EWtop_upper_range_tmp = spu_sub( vf_pixel_NE_upper_range, vf_pixel_NW_upper_range );
		vector float vf_EWtop_upper_range = spu_madd( vf_EWweight_upper_range,
								vf_EWtop_upper_range_tmp,
								vf_pixel_NW_upper_range );



		
		
		
		
		vector float vf_EWbottom_lower_range_tmp = spu_sub( vf_pixel_SE_lower_range, vf_pixel_SW_lower_range );
		vector float vf_EWbottom_lower_range = spu_madd( vf_EWweight_lower_range,
								vf_EWbottom_lower_range_tmp,
								vf_pixel_SW_lower_range );

		
		vector float vf_EWbottom_upper_range_tmp = spu_sub( vf_pixel_SE_upper_range, vf_pixel_SW_upper_range );
		vector float vf_EWbottom_upper_range = spu_madd( vf_EWweight_upper_range,
								vf_EWbottom_upper_range_tmp,
								vf_pixel_SW_upper_range );



		
		
		
		
		vector float vf_result_lower_range_tmp = spu_sub( vf_EWbottom_lower_range, vf_EWtop_lower_range );
		vector float vf_result_lower_range = spu_madd( vf_NSweight,
								vf_result_lower_range_tmp,
								vf_EWtop_lower_range );

		
		vector float vf_result_upper_range_tmp = spu_sub( vf_EWbottom_upper_range, vf_EWtop_upper_range );
		vector float vf_result_upper_range = spu_madd( vf_NSweight,
								vf_result_upper_range_tmp,
								vf_EWtop_upper_range );


		
		vector unsigned int vui_result_lower_range = vfloat_to_vuint( vf_result_lower_range );
		vector unsigned int vui_result_upper_range = vfloat_to_vuint( vf_result_upper_range );

		
		vector unsigned char vuc_mask_merge_result = { 0x03, 0x07, 0x0B, 0x0F,
							       0x13, 0x17, 0x1B, 0x1F,
							       0x00, 0x00, 0x00, 0x00,
							       0x00, 0x00, 0x00, 0x00 };

		vector unsigned char vuc_result = spu_shuffle( (vector unsigned char) vui_result_lower_range,
								(vector unsigned char) vui_result_upper_range,
								vuc_mask_merge_result );

		
		vector unsigned char vuc_mask_out = { 0x00, 0x00, 0x00, 0x00,
						      0x00, 0x00, 0x00, 0x00,
						      0xFF, 0xFF, 0xFF, 0xFF,
						      0xFF, 0xFF, 0xFF, 0xFF };


		
		vector unsigned char vuc_orig = *((vector unsigned char*)dst);

		
		vuc_orig = spu_and( vuc_orig,
				spu_rlqwbyte( vuc_mask_out, ((unsigned int)dst)&0x0F) );

		
		vuc_result = spu_rlqwbyte( vuc_result, ((unsigned int)dst)&0x0F );

		
		*((vector unsigned char*)dst) = spu_or( vuc_result,
							vuc_orig );
		dst += 8;
	}
}


/*
 * bilinear_scale_line_w16()
 *
 * processes a line of yuv-input, width has to be a multiple of 16
 * scaled yuv-output is written to local store buffer
 *
 * @param src buffer for 2 lines input
 * @param dst_ buffer for 1 line output
 * @param dst_width the width of the destination line
 * @param vf_x_scale a float vector, at each entry is the x_scale-factor
 * @param vf_NSweight a float vector, at each position is the weight NORTH/SOUTH for the current line
 * @param src_linestride the stride of the srcline
 */
void bilinear_scale_line_w16( unsigned char* src, unsigned char* dst_, unsigned int dst_width, vector float vf_x_scale, vector float vf_NSweight, unsigned int src_linestride ) {

	unsigned char* dst = dst_;

	unsigned int dst_x;
	for( dst_x=0; dst_x<dst_width; dst_x+=16) {
		
		
		vector unsigned int vui_dst_x_tmp = spu_splats( dst_x );
		
		
		
		
		
		vector unsigned int vui_inc_dst_x_first_range = { 0, 1, 2, 3 };
		vector unsigned int vui_inc_dst_x_second_range = { 4, 5, 6, 7 };
		vector unsigned int vui_inc_dst_x_third_range = { 8, 9, 10, 11 };
		vector unsigned int vui_inc_dst_x_fourth_range = { 12, 13, 14, 15 };
		vector unsigned int vui_dst_x_first_range = spu_add( vui_dst_x_tmp, vui_inc_dst_x_first_range );
		vector unsigned int vui_dst_x_second_range = spu_add( vui_dst_x_tmp, vui_inc_dst_x_second_range );
		vector unsigned int vui_dst_x_third_range = spu_add( vui_dst_x_tmp, vui_inc_dst_x_third_range );
		vector unsigned int vui_dst_x_fourth_range = spu_add( vui_dst_x_tmp, vui_inc_dst_x_fourth_range );

		
		vector float vf_dst_x_first_range = spu_convtf( vui_dst_x_first_range, 0 );
		vector float vf_dst_x_second_range = spu_convtf( vui_dst_x_second_range, 0 );
		vector float vf_dst_x_third_range = spu_convtf( vui_dst_x_third_range, 0 );
		vector float vf_dst_x_fourth_range = spu_convtf( vui_dst_x_fourth_range, 0 );
		vector float vf_src_x_first_range = spu_mul( vf_dst_x_first_range, vf_x_scale );
		vector float vf_src_x_second_range = spu_mul( vf_dst_x_second_range, vf_x_scale );
		vector float vf_src_x_third_range = spu_mul( vf_dst_x_third_range, vf_x_scale );
		vector float vf_src_x_fourth_range = spu_mul( vf_dst_x_fourth_range, vf_x_scale );
		vector unsigned int vui_interpl_x_first_range = spu_convtu( vf_src_x_first_range, 0 );
		vector unsigned int vui_interpl_x_second_range = spu_convtu( vf_src_x_second_range, 0 );
		vector unsigned int vui_interpl_x_third_range = spu_convtu( vf_src_x_third_range, 0 );
		vector unsigned int vui_interpl_x_fourth_range = spu_convtu( vf_src_x_fourth_range, 0 );
		vector float vf_interpl_x_first_range = spu_convtf( vui_interpl_x_first_range, 0 );
		vector float vf_interpl_x_second_range = spu_convtf( vui_interpl_x_second_range, 0 );
		vector float vf_interpl_x_third_range = spu_convtf( vui_interpl_x_third_range, 0 );
		vector float vf_interpl_x_fourth_range = spu_convtf( vui_interpl_x_fourth_range, 0 );
		vector float vf_EWweight_first_range = spu_sub( vf_src_x_first_range, vf_interpl_x_first_range );
		vector float vf_EWweight_second_range = spu_sub( vf_src_x_second_range, vf_interpl_x_second_range );
		vector float vf_EWweight_third_range = spu_sub( vf_src_x_third_range, vf_interpl_x_third_range );
		vector float vf_EWweight_fourth_range = spu_sub( vf_src_x_fourth_range, vf_interpl_x_fourth_range );

		
		
		
		vector unsigned int vui_off_pixelNW_first_range = vui_interpl_x_first_range;
		vector unsigned int vui_off_pixelNW_second_range = vui_interpl_x_second_range;
		vector unsigned int vui_off_pixelNW_third_range = vui_interpl_x_third_range;
		vector unsigned int vui_off_pixelNW_fourth_range = vui_interpl_x_fourth_range;

		
		vector unsigned int vui_add_1 = { 1, 1, 1, 1 };
		vector unsigned int vui_off_pixelNE_first_range = spu_add( vui_off_pixelNW_first_range, vui_add_1 );
		vector unsigned int vui_off_pixelNE_second_range = spu_add( vui_off_pixelNW_second_range, vui_add_1 );
		vector unsigned int vui_off_pixelNE_third_range = spu_add( vui_off_pixelNW_third_range, vui_add_1 );
		vector unsigned int vui_off_pixelNE_fourth_range = spu_add( vui_off_pixelNW_fourth_range, vui_add_1 );

		
		vector unsigned int vui_srclinestride = spu_splats( src_linestride );
		vector unsigned int vui_off_pixelSW_first_range = spu_add( vui_srclinestride, vui_off_pixelNW_first_range );
		vector unsigned int vui_off_pixelSW_second_range = spu_add( vui_srclinestride, vui_off_pixelNW_second_range );
		vector unsigned int vui_off_pixelSW_third_range = spu_add( vui_srclinestride, vui_off_pixelNW_third_range );
		vector unsigned int vui_off_pixelSW_fourth_range = spu_add( vui_srclinestride, vui_off_pixelNW_fourth_range );

		
		vector unsigned int vui_off_pixelSE_first_range = spu_add( vui_srclinestride, vui_off_pixelNE_first_range );
		vector unsigned int vui_off_pixelSE_second_range = spu_add( vui_srclinestride, vui_off_pixelNE_second_range );
		vector unsigned int vui_off_pixelSE_third_range = spu_add( vui_srclinestride, vui_off_pixelNE_third_range );
		vector unsigned int vui_off_pixelSE_fourth_range = spu_add( vui_srclinestride, vui_off_pixelNE_fourth_range );

		
		vector unsigned int vui_src_ls = spu_splats( (unsigned int) src );
		vector unsigned int vui_addr_pixelNW_first_range = spu_add( vui_src_ls, vui_off_pixelNW_first_range );
		vector unsigned int vui_addr_pixelNW_second_range = spu_add( vui_src_ls, vui_off_pixelNW_second_range );
		vector unsigned int vui_addr_pixelNW_third_range = spu_add( vui_src_ls, vui_off_pixelNW_third_range );
		vector unsigned int vui_addr_pixelNW_fourth_range = spu_add( vui_src_ls, vui_off_pixelNW_fourth_range );

		vector unsigned int vui_addr_pixelNE_first_range = spu_add( vui_src_ls, vui_off_pixelNE_first_range );
		vector unsigned int vui_addr_pixelNE_second_range = spu_add( vui_src_ls, vui_off_pixelNE_second_range );
		vector unsigned int vui_addr_pixelNE_third_range = spu_add( vui_src_ls, vui_off_pixelNE_third_range );
		vector unsigned int vui_addr_pixelNE_fourth_range = spu_add( vui_src_ls, vui_off_pixelNE_fourth_range );

		vector unsigned int vui_addr_pixelSW_first_range = spu_add( vui_src_ls, vui_off_pixelSW_first_range );
		vector unsigned int vui_addr_pixelSW_second_range = spu_add( vui_src_ls, vui_off_pixelSW_second_range );
		vector unsigned int vui_addr_pixelSW_third_range = spu_add( vui_src_ls, vui_off_pixelSW_third_range );
		vector unsigned int vui_addr_pixelSW_fourth_range = spu_add( vui_src_ls, vui_off_pixelSW_fourth_range );

		vector unsigned int vui_addr_pixelSE_first_range = spu_add( vui_src_ls, vui_off_pixelSE_first_range );
		vector unsigned int vui_addr_pixelSE_second_range = spu_add( vui_src_ls, vui_off_pixelSE_second_range );
		vector unsigned int vui_addr_pixelSE_third_range = spu_add( vui_src_ls, vui_off_pixelSE_third_range );
		vector unsigned int vui_addr_pixelSE_fourth_range = spu_add( vui_src_ls, vui_off_pixelSE_fourth_range );


		
		
		
		
		
		vector unsigned char null_vector = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		vector unsigned char vuc_pixel_NW_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_first_range, 0 )), null_vector, 3 );
		vuc_pixel_NW_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_first_range, 1 )),
				vuc_pixel_NW_first_range, 7 );
		vuc_pixel_NW_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_first_range, 2 )),
				vuc_pixel_NW_first_range, 11 );
		vuc_pixel_NW_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_first_range, 3 )),
				vuc_pixel_NW_first_range, 15 );
		
		vector unsigned char vuc_pixel_NW_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_second_range, 0 )), null_vector, 3 );
		vuc_pixel_NW_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_second_range, 1 )),
				vuc_pixel_NW_second_range, 7 );
		vuc_pixel_NW_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_second_range, 2 )),
				vuc_pixel_NW_second_range, 11 );
		vuc_pixel_NW_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_second_range, 3 )),
				vuc_pixel_NW_second_range, 15 );
		
		vector unsigned char vuc_pixel_NW_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_third_range, 0 )), null_vector, 3 );
		vuc_pixel_NW_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_third_range, 1 )),
				vuc_pixel_NW_third_range, 7 );
		vuc_pixel_NW_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_third_range, 2 )),
				vuc_pixel_NW_third_range, 11 );
		vuc_pixel_NW_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_third_range, 3 )),
				vuc_pixel_NW_third_range, 15 );
		
		vector unsigned char vuc_pixel_NW_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_fourth_range, 0 )), null_vector, 3 );
		vuc_pixel_NW_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_fourth_range, 1 )),
				vuc_pixel_NW_fourth_range, 7 );
		vuc_pixel_NW_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_fourth_range, 2 )),
				vuc_pixel_NW_fourth_range, 11 );
		vuc_pixel_NW_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNW_fourth_range, 3 )),
				vuc_pixel_NW_fourth_range, 15 );

		
		
		vector unsigned char vuc_pixel_NE_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_first_range, 0 )), null_vector, 3 );
		vuc_pixel_NE_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_first_range, 1 )),
				vuc_pixel_NE_first_range, 7 );
		vuc_pixel_NE_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_first_range, 2 )),
				vuc_pixel_NE_first_range, 11 );
		vuc_pixel_NE_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_first_range, 3 )),
				vuc_pixel_NE_first_range, 15 );
		
		vector unsigned char vuc_pixel_NE_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_second_range, 0 )), null_vector, 3 );
		vuc_pixel_NE_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_second_range, 1 )),
				vuc_pixel_NE_second_range, 7 );
		vuc_pixel_NE_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_second_range, 2 )),
				vuc_pixel_NE_second_range, 11 );
		vuc_pixel_NE_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_second_range, 3 )),
				vuc_pixel_NE_second_range, 15 );
		
		vector unsigned char vuc_pixel_NE_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_third_range, 0 )), null_vector, 3 );
		vuc_pixel_NE_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_third_range, 1 )),
				vuc_pixel_NE_third_range, 7 );
		vuc_pixel_NE_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_third_range, 2 )),
				vuc_pixel_NE_third_range, 11 );
		vuc_pixel_NE_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_third_range, 3 )),
				vuc_pixel_NE_third_range, 15 );
		
		vector unsigned char vuc_pixel_NE_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_fourth_range, 0 )), null_vector, 3 );
		vuc_pixel_NE_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_fourth_range, 1 )),
				vuc_pixel_NE_fourth_range, 7 );
		vuc_pixel_NE_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_fourth_range, 2 )),
				vuc_pixel_NE_fourth_range, 11 );
		vuc_pixel_NE_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelNE_fourth_range, 3 )),
				vuc_pixel_NE_fourth_range, 15 );

		
		
		vector unsigned char vuc_pixel_SW_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_first_range, 0 )), null_vector, 3 );
		vuc_pixel_SW_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_first_range, 1 )),
				vuc_pixel_SW_first_range, 7 );
		vuc_pixel_SW_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_first_range, 2 )),
				vuc_pixel_SW_first_range, 11 );
		vuc_pixel_SW_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_first_range, 3 )),
				vuc_pixel_SW_first_range, 15 );
		
		vector unsigned char vuc_pixel_SW_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_second_range, 0 )), null_vector, 3 );
		vuc_pixel_SW_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_second_range, 1 )),
				vuc_pixel_SW_second_range, 7 );
		vuc_pixel_SW_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_second_range, 2 )),
				vuc_pixel_SW_second_range, 11 );
		vuc_pixel_SW_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_second_range, 3 )),
				vuc_pixel_SW_second_range, 15 );
		
		vector unsigned char vuc_pixel_SW_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_third_range, 0 )), null_vector, 3 );
		vuc_pixel_SW_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_third_range, 1 )),
				vuc_pixel_SW_third_range, 7 );
		vuc_pixel_SW_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_third_range, 2 )),
				vuc_pixel_SW_third_range, 11 );
		vuc_pixel_SW_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_third_range, 3 )),
				vuc_pixel_SW_third_range, 15 );
		
		vector unsigned char vuc_pixel_SW_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_fourth_range, 0 )), null_vector, 3 );
		vuc_pixel_SW_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_fourth_range, 1 )),
				vuc_pixel_SW_fourth_range, 7 );
		vuc_pixel_SW_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_fourth_range, 2 )),
				vuc_pixel_SW_fourth_range, 11 );
		vuc_pixel_SW_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSW_fourth_range, 3 )),
				vuc_pixel_SW_fourth_range, 15 );

		
		
		vector unsigned char vuc_pixel_SE_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_first_range, 0 )), null_vector, 3 );
		vuc_pixel_SE_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_first_range, 1 )),
				vuc_pixel_SE_first_range, 7 );
		vuc_pixel_SE_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_first_range, 2 )),
				vuc_pixel_SE_first_range, 11 );
		vuc_pixel_SE_first_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_first_range, 3 )),
				vuc_pixel_SE_first_range, 15 );
		
		vector unsigned char vuc_pixel_SE_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_second_range, 0 )), null_vector, 3 );
		vuc_pixel_SE_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_second_range, 1 )),
				vuc_pixel_SE_second_range, 7 );
		vuc_pixel_SE_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_second_range, 2 )),
				vuc_pixel_SE_second_range, 11 );
		vuc_pixel_SE_second_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_second_range, 3 )),
				vuc_pixel_SE_second_range, 15 );
		
		vector unsigned char vuc_pixel_SE_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_third_range, 0 )), null_vector, 3 );
		vuc_pixel_SE_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_third_range, 1 )),
				vuc_pixel_SE_third_range, 7 );
		vuc_pixel_SE_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_third_range, 2 )),
				vuc_pixel_SE_third_range, 11 );
		vuc_pixel_SE_third_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_third_range, 3 )),
				vuc_pixel_SE_third_range, 15 );
		
		vector unsigned char vuc_pixel_SE_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_fourth_range, 0 )), null_vector, 3 );
		vuc_pixel_SE_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_fourth_range, 1 )),
				vuc_pixel_SE_fourth_range, 7 );
		vuc_pixel_SE_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_fourth_range, 2 )),
				vuc_pixel_SE_fourth_range, 11 );
		vuc_pixel_SE_fourth_range = spu_insert(
				*((unsigned char*) spu_extract( vui_addr_pixelSE_fourth_range, 3 )),
				vuc_pixel_SE_fourth_range, 15 );



		
		vector float vf_pixel_NW_first_range = spu_convtf( (vector unsigned int) vuc_pixel_NW_first_range, 0 );
		vector float vf_pixel_NW_second_range = spu_convtf( (vector unsigned int) vuc_pixel_NW_second_range, 0 );
		vector float vf_pixel_NW_third_range = spu_convtf( (vector unsigned int) vuc_pixel_NW_third_range, 0 );
		vector float vf_pixel_NW_fourth_range = spu_convtf( (vector unsigned int) vuc_pixel_NW_fourth_range, 0 );

		vector float vf_pixel_NE_first_range = spu_convtf( (vector unsigned int) vuc_pixel_NE_first_range, 0 );
		vector float vf_pixel_NE_second_range = spu_convtf( (vector unsigned int) vuc_pixel_NE_second_range, 0 );
		vector float vf_pixel_NE_third_range = spu_convtf( (vector unsigned int) vuc_pixel_NE_third_range, 0 );
		vector float vf_pixel_NE_fourth_range = spu_convtf( (vector unsigned int) vuc_pixel_NE_fourth_range, 0 );

		vector float vf_pixel_SW_first_range = spu_convtf( (vector unsigned int) vuc_pixel_SW_first_range, 0 );
		vector float vf_pixel_SW_second_range = spu_convtf( (vector unsigned int) vuc_pixel_SW_second_range, 0 );
		vector float vf_pixel_SW_third_range = spu_convtf( (vector unsigned int) vuc_pixel_SW_third_range, 0 );
		vector float vf_pixel_SW_fourth_range = spu_convtf( (vector unsigned int) vuc_pixel_SW_fourth_range, 0 );

		vector float vf_pixel_SE_first_range = spu_convtf( (vector unsigned int) vuc_pixel_SE_first_range, 0 );
		vector float vf_pixel_SE_second_range = spu_convtf( (vector unsigned int) vuc_pixel_SE_second_range, 0 );
		vector float vf_pixel_SE_third_range = spu_convtf( (vector unsigned int) vuc_pixel_SE_third_range, 0 );
		vector float vf_pixel_SE_fourth_range = spu_convtf( (vector unsigned int) vuc_pixel_SE_fourth_range, 0 );

		
		
		
		
		vector float vf_EWtop_first_range_tmp = spu_sub( vf_pixel_NE_first_range, vf_pixel_NW_first_range );
		vector float vf_EWtop_first_range = spu_madd( vf_EWweight_first_range,
								vf_EWtop_first_range_tmp,
								vf_pixel_NW_first_range );

		
		vector float vf_EWtop_second_range_tmp = spu_sub( vf_pixel_NE_second_range, vf_pixel_NW_second_range );
		vector float vf_EWtop_second_range = spu_madd( vf_EWweight_second_range,
								vf_EWtop_second_range_tmp,
								vf_pixel_NW_second_range );

		
		vector float vf_EWtop_third_range_tmp = spu_sub( vf_pixel_NE_third_range, vf_pixel_NW_third_range );
		vector float vf_EWtop_third_range = spu_madd( vf_EWweight_third_range,
								vf_EWtop_third_range_tmp,
								vf_pixel_NW_third_range );

		
		vector float vf_EWtop_fourth_range_tmp = spu_sub( vf_pixel_NE_fourth_range, vf_pixel_NW_fourth_range );
		vector float vf_EWtop_fourth_range = spu_madd( vf_EWweight_fourth_range,
								vf_EWtop_fourth_range_tmp,
								vf_pixel_NW_fourth_range );



		
		
		
		
		vector float vf_EWbottom_first_range_tmp = spu_sub( vf_pixel_SE_first_range, vf_pixel_SW_first_range );
		vector float vf_EWbottom_first_range = spu_madd( vf_EWweight_first_range,
								vf_EWbottom_first_range_tmp,
								vf_pixel_SW_first_range );

		
		vector float vf_EWbottom_second_range_tmp = spu_sub( vf_pixel_SE_second_range, vf_pixel_SW_second_range );
		vector float vf_EWbottom_second_range = spu_madd( vf_EWweight_second_range,
								vf_EWbottom_second_range_tmp,
								vf_pixel_SW_second_range );
		
		vector float vf_EWbottom_third_range_tmp = spu_sub( vf_pixel_SE_third_range, vf_pixel_SW_third_range );
		vector float vf_EWbottom_third_range = spu_madd( vf_EWweight_third_range,
								vf_EWbottom_third_range_tmp,
								vf_pixel_SW_third_range );

		
		vector float vf_EWbottom_fourth_range_tmp = spu_sub( vf_pixel_SE_fourth_range, vf_pixel_SW_fourth_range );
		vector float vf_EWbottom_fourth_range = spu_madd( vf_EWweight_fourth_range,
								vf_EWbottom_fourth_range_tmp,
								vf_pixel_SW_fourth_range );



		
		
		
		
		vector float vf_result_first_range_tmp = spu_sub( vf_EWbottom_first_range, vf_EWtop_first_range );
		vector float vf_result_first_range = spu_madd( vf_NSweight,
								vf_result_first_range_tmp,
								vf_EWtop_first_range );

		
		vector float vf_result_second_range_tmp = spu_sub( vf_EWbottom_second_range, vf_EWtop_second_range );
		vector float vf_result_second_range = spu_madd( vf_NSweight,
								vf_result_second_range_tmp,
								vf_EWtop_second_range );

		
		vector float vf_result_third_range_tmp = spu_sub( vf_EWbottom_third_range, vf_EWtop_third_range );
		vector float vf_result_third_range = spu_madd( vf_NSweight,
								vf_result_third_range_tmp,
								vf_EWtop_third_range );

		
		vector float vf_result_fourth_range_tmp = spu_sub( vf_EWbottom_fourth_range, vf_EWtop_fourth_range );
		vector float vf_result_fourth_range = spu_madd( vf_NSweight,
								vf_result_fourth_range_tmp,
								vf_EWtop_fourth_range );



		
		vector unsigned int vui_result_first_range = vfloat_to_vuint( vf_result_first_range );
		vector unsigned int vui_result_second_range = vfloat_to_vuint( vf_result_second_range );
		vector unsigned int vui_result_third_range = vfloat_to_vuint( vf_result_third_range );
		vector unsigned int vui_result_fourth_range = vfloat_to_vuint( vf_result_fourth_range );

		
		vector unsigned char vuc_mask_merge_result_first_second = { 0x03, 0x07, 0x0B, 0x0F,
							       		    0x13, 0x17, 0x1B, 0x1F,
							       		    0x00, 0x00, 0x00, 0x00,
							       		    0x00, 0x00, 0x00, 0x00 };

		vector unsigned char vuc_mask_merge_result_third_fourth = { 0x00, 0x00, 0x00, 0x00,
							       		    0x00, 0x00, 0x00, 0x00,
									    0x03, 0x07, 0x0B, 0x0F,
							       		    0x13, 0x17, 0x1B, 0x1F };

		vector unsigned char vuc_result_first_second =
						spu_shuffle( (vector unsigned char) vui_result_first_range,
								 (vector unsigned char) vui_result_second_range,
								vuc_mask_merge_result_first_second );

		vector unsigned char vuc_result_third_fourth =
						spu_shuffle( (vector unsigned char) vui_result_third_range,
								 (vector unsigned char) vui_result_fourth_range,
								vuc_mask_merge_result_third_fourth );

		
		*((vector unsigned char*)dst) = spu_or( vuc_result_first_second,
							vuc_result_third_fourth );
		dst += 16;
	}
}

