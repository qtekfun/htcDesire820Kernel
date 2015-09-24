/*
 * jdcoefct.c
 *
 * Copyright (C) 1994-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains the coefficient buffer controller for decompression.
 * This controller is the top level of the JPEG decompressor proper.
 * The coefficient buffer lies between entropy decoding and inverse-DCT steps.
 *
 * In buffered-image mode, this controller is the interface between
 * input-oriented processing and output-oriented processing.
 * Also, the input side (only) is used when reading a file for transcoding.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"

#ifndef D_PROGRESSIVE_SUPPORTED
#undef BLOCK_SMOOTHING_SUPPORTED
#endif


typedef struct {
  struct jpeg_d_coef_controller pub; 

  
  
  JDIMENSION MCU_ctr;		
  int MCU_vert_offset;		
  int MCU_rows_per_iMCU_row;	

  

  JBLOCKROW MCU_buffer[D_MAX_BLOCKS_IN_MCU];

#ifdef D_MULTISCAN_FILES_SUPPORTED
  
  jvirt_barray_ptr whole_image[MAX_COMPONENTS];
#endif

#ifdef BLOCK_SMOOTHING_SUPPORTED
  
  int * coef_bits_latch;
#define SAVED_COEFS  6		
#endif
} my_coef_controller;

typedef my_coef_controller * my_coef_ptr;

METHODDEF(int) decompress_onepass
	JPP((j_decompress_ptr cinfo, JSAMPIMAGE output_buf));
#ifdef D_MULTISCAN_FILES_SUPPORTED
METHODDEF(int) decompress_data
	JPP((j_decompress_ptr cinfo, JSAMPIMAGE output_buf));
#endif
#ifdef BLOCK_SMOOTHING_SUPPORTED
LOCAL(boolean) smoothing_ok JPP((j_decompress_ptr cinfo));
METHODDEF(int) decompress_smooth_data
	JPP((j_decompress_ptr cinfo, JSAMPIMAGE output_buf));
#endif


LOCAL(void)
start_iMCU_row (j_decompress_ptr cinfo)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;

  if (cinfo->comps_in_scan > 1) {
    coef->MCU_rows_per_iMCU_row = 1;
  } else {
    if (cinfo->input_iMCU_row < (cinfo->total_iMCU_rows-1))
      coef->MCU_rows_per_iMCU_row = cinfo->cur_comp_info[0]->v_samp_factor;
    else
      coef->MCU_rows_per_iMCU_row = cinfo->cur_comp_info[0]->last_row_height;
  }

  coef->MCU_ctr = 0;
  coef->MCU_vert_offset = 0;
}



METHODDEF(void)
start_input_pass (j_decompress_ptr cinfo)
{
  cinfo->input_iMCU_row = 0;
  start_iMCU_row(cinfo);
}



METHODDEF(void)
start_output_pass (j_decompress_ptr cinfo)
{
#ifdef BLOCK_SMOOTHING_SUPPORTED
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;

  
  if (coef->pub.coef_arrays != NULL) {
    if (cinfo->do_block_smoothing && smoothing_ok(cinfo))
      coef->pub.decompress_data = decompress_smooth_data;
    else
      coef->pub.decompress_data = decompress_data;
  }
#endif
  cinfo->output_iMCU_row = 0;
}



METHODDEF(int)
decompress_onepass (j_decompress_ptr cinfo, JSAMPIMAGE output_buf)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
  JDIMENSION MCU_col_num;	
  JDIMENSION last_MCU_col = cinfo->MCUs_per_row - 1;
  JDIMENSION last_iMCU_row = cinfo->total_iMCU_rows - 1;
  int blkn, ci, xindex, yindex, yoffset, useful_width;
  JSAMPARRAY output_ptr;
  JDIMENSION start_col, output_col;
  jpeg_component_info *compptr;
  inverse_DCT_method_ptr inverse_DCT;

#ifdef ANDROID_TILE_BASED_DECODE
  if (cinfo->tile_decode) {
    last_MCU_col =
        (cinfo->coef->MCU_column_right_boundary -
         cinfo->coef->MCU_column_left_boundary) - 1;
  }
#endif

  
  for (yoffset = coef->MCU_vert_offset; yoffset < coef->MCU_rows_per_iMCU_row;
       yoffset++) {
    for (MCU_col_num = coef->MCU_ctr; MCU_col_num <= last_MCU_col;
	 MCU_col_num++) {
      
      if (MCU_col_num < coef->pub.MCU_columns_to_skip) {
        (*cinfo->entropy->decode_mcu_discard_coef) (cinfo);
        continue;
      } else {
        jzero_far((void FAR *) coef->MCU_buffer[0],
		(size_t) (cinfo->blocks_in_MCU * SIZEOF(JBLOCK)));
        if (! (*cinfo->entropy->decode_mcu) (cinfo, coef->MCU_buffer)) {
	  
	  coef->MCU_vert_offset = yoffset;
	  coef->MCU_ctr = MCU_col_num;
	  return JPEG_SUSPENDED;
        }
      }
      blkn = 0;			
      for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
	compptr = cinfo->cur_comp_info[ci];
	
	if (! compptr->component_needed) {
	  blkn += compptr->MCU_blocks;
	  continue;
	}
	inverse_DCT = cinfo->idct->inverse_DCT[compptr->component_index];
	useful_width = (MCU_col_num < last_MCU_col) ? compptr->MCU_width
						    : compptr->last_col_width;
	output_ptr = output_buf[compptr->component_index] +
	  yoffset * compptr->DCT_scaled_size;
	start_col = MCU_col_num * compptr->MCU_sample_width;
	for (yindex = 0; yindex < compptr->MCU_height; yindex++) {
	  if (cinfo->input_iMCU_row < last_iMCU_row ||
	      yoffset+yindex < compptr->last_row_height) {
	    output_col = start_col;
	    for (xindex = 0; xindex < useful_width; xindex++) {
	      (*inverse_DCT) (cinfo, compptr,
		        (JCOEFPTR) coef->MCU_buffer[blkn+xindex],
		        output_ptr, output_col);
	      output_col += compptr->DCT_scaled_size;
	    }
	  }
	  blkn += compptr->MCU_width;
	  output_ptr += compptr->DCT_scaled_size;
	}
      }
    }
    
    coef->MCU_ctr = 0;
  }
  
  cinfo->output_iMCU_row++;
  if (++(cinfo->input_iMCU_row) < cinfo->total_iMCU_rows) {
    start_iMCU_row(cinfo);
    return JPEG_ROW_COMPLETED;
  }
  
  (*cinfo->inputctl->finish_input_pass) (cinfo);
  return JPEG_SCAN_COMPLETED;
}



METHODDEF(int)
dummy_consume_data (j_decompress_ptr cinfo)
{
  return JPEG_SUSPENDED;	
}

#ifdef D_MULTISCAN_FILES_SUPPORTED

METHODDEF(int)
consume_data (j_decompress_ptr cinfo)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
  JDIMENSION MCU_col_num;	
  int blkn, ci, xindex, yindex, yoffset;
  JDIMENSION start_col;
  JBLOCKARRAY buffer[MAX_COMPS_IN_SCAN];
  JBLOCKROW buffer_ptr;
  jpeg_component_info *compptr;

  
  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[ci];
    buffer[ci] = (*cinfo->mem->access_virt_barray)
      ((j_common_ptr) cinfo, coef->whole_image[compptr->component_index],
       cinfo->tile_decode ? 0 : cinfo->input_iMCU_row * compptr->v_samp_factor,
       (JDIMENSION) compptr->v_samp_factor, TRUE);
  }
  unsigned int MCUs_per_row = cinfo->MCUs_per_row;
#ifdef ANDROID_TILE_BASED_DECODE
  if (cinfo->tile_decode) {
    int iMCU_width_To_MCU_width;
    if (cinfo->comps_in_scan > 1) {
      
      iMCU_width_To_MCU_width = 1;
    } else {
      
      iMCU_width_To_MCU_width = cinfo->cur_comp_info[0]->h_samp_factor;
    }
    MCUs_per_row = jmin(MCUs_per_row,
        (cinfo->coef->column_right_boundary - cinfo->coef->column_left_boundary)
        * cinfo->entropy->index->MCU_sample_size * iMCU_width_To_MCU_width);
  }
#endif

  
  for (yoffset = coef->MCU_vert_offset; yoffset < coef->MCU_rows_per_iMCU_row;
       yoffset++) {
   
#ifdef ANDROID_TILE_BASED_DECODE
    if (cinfo->tile_decode) {
      huffman_scan_header scan_header =
            cinfo->entropy->index->scan[cinfo->input_scan_number];
      int col_offset = cinfo->coef->column_left_boundary;
      (*cinfo->entropy->configure_huffman_decoder) (cinfo,
              scan_header.offset[cinfo->input_iMCU_row]
              [col_offset + yoffset * scan_header.MCUs_per_row]);
    }
#endif

    
    for (MCU_col_num = coef->MCU_ctr; MCU_col_num < MCUs_per_row;
          MCU_col_num++) {
      
      blkn = 0;			
      for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
        compptr = cinfo->cur_comp_info[ci];
        start_col = MCU_col_num * compptr->MCU_width;
        for (yindex = 0; yindex < compptr->MCU_height; yindex++) {
          buffer_ptr = buffer[ci][yindex+yoffset] + start_col;
          for (xindex = 0; xindex < compptr->MCU_width; xindex++) {
            coef->MCU_buffer[blkn++] = buffer_ptr++;
#ifdef ANDROID_TILE_BASED_DECODE
            if (cinfo->tile_decode && cinfo->input_scan_number == 0) {
              
              jzero_far((void FAR *) coef->MCU_buffer[blkn-1],
                        (size_t) (SIZEOF(JBLOCK)));
            }
#endif
          }
        }
      }


      
      if (! (*cinfo->entropy->decode_mcu) (cinfo, coef->MCU_buffer)) {
        
        coef->MCU_vert_offset = yoffset;
        coef->MCU_ctr = MCU_col_num;
        return JPEG_SUSPENDED;
      }
    }
    
    coef->MCU_ctr = 0;
  }
  
  if (++(cinfo->input_iMCU_row) < cinfo->total_iMCU_rows) {
    start_iMCU_row(cinfo);
    return JPEG_ROW_COMPLETED;
  }
  
  (*cinfo->inputctl->finish_input_pass) (cinfo);
  return JPEG_SCAN_COMPLETED;
}


METHODDEF(int)
consume_data_multi_scan (j_decompress_ptr cinfo)
{
  huffman_index *index = cinfo->entropy->index;
  int i, retcode, ci;
  int mcu = cinfo->input_iMCU_row;
  jinit_phuff_decoder(cinfo);
  for (i = 0; i < index->scan_count; i++) {
    (*cinfo->inputctl->finish_input_pass) (cinfo);
    jset_input_stream_position(cinfo, index->scan[i].bitstream_offset);
    cinfo->output_iMCU_row = mcu;
    cinfo->unread_marker = 0;
    
    retcode = (*cinfo->inputctl->consume_markers) (cinfo, index, i);
    cinfo->input_iMCU_row = mcu;
    cinfo->input_scan_number = i;
    cinfo->entropy->index = index;
    
    consume_data(cinfo);
  }
  cinfo->input_iMCU_row = mcu + 1;
  cinfo->input_scan_number = 0;
  cinfo->output_scan_number = 0;
  return JPEG_ROW_COMPLETED;
}


METHODDEF(int)
consume_data_build_huffman_index_baseline (j_decompress_ptr cinfo,
        huffman_index *index, int current_scan)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
  JDIMENSION MCU_col_num;	
  int ci, xindex, yindex, yoffset;
  JDIMENSION start_col;
  JBLOCKROW buffer_ptr;

  huffman_scan_header *scan_header = index->scan + current_scan;
  scan_header->MCU_rows_per_iMCU_row = coef->MCU_rows_per_iMCU_row;

  size_t allocate_size = coef->MCU_rows_per_iMCU_row
      * jdiv_round_up(cinfo->MCUs_per_row, index->MCU_sample_size)
      * sizeof(huffman_offset_data);
  scan_header->offset[cinfo->input_iMCU_row] =
        (huffman_offset_data*)malloc(allocate_size);
  index->mem_used += allocate_size;

  huffman_offset_data *offset_data = scan_header->offset[cinfo->input_iMCU_row];

  
  for (yoffset = coef->MCU_vert_offset; yoffset < coef->MCU_rows_per_iMCU_row;
       yoffset++) {
    for (MCU_col_num = coef->MCU_ctr; MCU_col_num < cinfo->MCUs_per_row;
	 MCU_col_num++) {
      
      if (MCU_col_num % index->MCU_sample_size == 0) {
        (*cinfo->entropy->get_huffman_decoder_configuration)
                (cinfo, offset_data);
        ++offset_data;
      }

      
      if (! (*cinfo->entropy->decode_mcu_discard_coef) (cinfo)) {
        
        coef->MCU_vert_offset = yoffset;
        coef->MCU_ctr = MCU_col_num;
        return JPEG_SUSPENDED;
      }
    }
    
    coef->MCU_ctr = 0;
  }
  
  if (++(cinfo->input_iMCU_row) < cinfo->total_iMCU_rows) {
    start_iMCU_row(cinfo);
    return JPEG_ROW_COMPLETED;
  }
  
  (*cinfo->inputctl->finish_input_pass) (cinfo);
  return JPEG_SCAN_COMPLETED;
}


METHODDEF(int)
consume_data_build_huffman_index_progressive (j_decompress_ptr cinfo,
        huffman_index *index, int current_scan)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
  JDIMENSION MCU_col_num;	
  int blkn, ci, xindex, yindex, yoffset;
  JDIMENSION start_col;
  JBLOCKARRAY buffer[MAX_COMPS_IN_SCAN];
  JBLOCKROW buffer_ptr;
  jpeg_component_info *compptr;

  int factor = 4; 
  for (ci = 0; ci < cinfo->comps_in_scan; ci++)
    factor = jmin(factor, cinfo->cur_comp_info[ci]->h_samp_factor);

  int sample_size = index->MCU_sample_size * factor;
  huffman_scan_header *scan_header = index->scan + current_scan;
  scan_header->MCU_rows_per_iMCU_row = coef->MCU_rows_per_iMCU_row;
  scan_header->MCUs_per_row = jdiv_round_up(cinfo->MCUs_per_row, sample_size);
  scan_header->comps_in_scan = cinfo->comps_in_scan;

  size_t allocate_size = coef->MCU_rows_per_iMCU_row
      * scan_header->MCUs_per_row * sizeof(huffman_offset_data);
  scan_header->offset[cinfo->input_iMCU_row] =
        (huffman_offset_data*)malloc(allocate_size);
  index->mem_used += allocate_size;

  huffman_offset_data *offset_data = scan_header->offset[cinfo->input_iMCU_row];

  
  for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
    compptr = cinfo->cur_comp_info[ci];
    buffer[ci] = (*cinfo->mem->access_virt_barray)
      ((j_common_ptr) cinfo, coef->whole_image[compptr->component_index],
       0, 
       (JDIMENSION) compptr->v_samp_factor, TRUE);
  }
  
  for (yoffset = coef->MCU_vert_offset; yoffset < coef->MCU_rows_per_iMCU_row;
       yoffset++) {
    for (MCU_col_num = coef->MCU_ctr; MCU_col_num < cinfo->MCUs_per_row;
	 MCU_col_num++) {
      blkn = 0; 
      for (ci = 0; ci < cinfo->comps_in_scan; ci++) {
        compptr = cinfo->cur_comp_info[ci];
        start_col = MCU_col_num * compptr->MCU_width;
        for (yindex = 0; yindex < compptr->MCU_height; yindex++) {
          buffer_ptr = buffer[ci][yindex+yoffset] + start_col;
          for (xindex = 0; xindex < compptr->MCU_width; xindex++) {
            coef->MCU_buffer[blkn++] = buffer_ptr++;
            if (cinfo->input_scan_number == 0) {
              
              jzero_far((void FAR *) coef->MCU_buffer[blkn-1],
                        (size_t) (SIZEOF(JBLOCK)));
            }
          }
        }
      }
      
      if (MCU_col_num % sample_size == 0) {
        (*cinfo->entropy->get_huffman_decoder_configuration)
                (cinfo, offset_data);
        ++offset_data;
      }
      
      if (! (*cinfo->entropy->decode_mcu) (cinfo, coef->MCU_buffer)) {
	
	coef->MCU_vert_offset = yoffset;
	coef->MCU_ctr = MCU_col_num;
	return JPEG_SUSPENDED;
      }
    }
    
    coef->MCU_ctr = 0;
  }
  (*cinfo->entropy->get_huffman_decoder_configuration)
        (cinfo, &scan_header->prev_MCU_offset);
  
  if (++(cinfo->input_iMCU_row) < cinfo->total_iMCU_rows) {
    start_iMCU_row(cinfo);
    return JPEG_ROW_COMPLETED;
  }
  
  (*cinfo->inputctl->finish_input_pass) (cinfo);
  return JPEG_SCAN_COMPLETED;
}


METHODDEF(int)
decompress_data (j_decompress_ptr cinfo, JSAMPIMAGE output_buf)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
  JDIMENSION last_iMCU_row = cinfo->total_iMCU_rows - 1;
  JDIMENSION block_num;
  int ci, block_row, block_rows;
  JBLOCKARRAY buffer;
  JBLOCKROW buffer_ptr;
  JSAMPARRAY output_ptr;
  JDIMENSION output_col;
  jpeg_component_info *compptr;
  inverse_DCT_method_ptr inverse_DCT;

  
  while (cinfo->input_scan_number < cinfo->output_scan_number ||
	 (cinfo->input_scan_number == cinfo->output_scan_number &&
	  cinfo->input_iMCU_row <= cinfo->output_iMCU_row)) {
    if ((*cinfo->inputctl->consume_input)(cinfo) == JPEG_SUSPENDED)
      return JPEG_SUSPENDED;
  }

  
  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    
    if (! compptr->component_needed)
      continue;
    
    buffer = (*cinfo->mem->access_virt_barray)
      ((j_common_ptr) cinfo, coef->whole_image[ci],
       cinfo->tile_decode ? 0 : cinfo->output_iMCU_row * compptr->v_samp_factor,
       (JDIMENSION) compptr->v_samp_factor, FALSE);
    
    if (cinfo->output_iMCU_row < last_iMCU_row)
      block_rows = compptr->v_samp_factor;
    else {
      
      block_rows = (int) (compptr->height_in_blocks % compptr->v_samp_factor);
      if (block_rows == 0) block_rows = compptr->v_samp_factor;
    }
    inverse_DCT = cinfo->idct->inverse_DCT[ci];
    output_ptr = output_buf[ci];
    int width_in_blocks = compptr->width_in_blocks;
    int start_block = 0;
#if ANDROID_TILE_BASED_DECODE
    if (cinfo->tile_decode) {
      
      width_in_blocks = jmin(width_in_blocks,
        (cinfo->coef->MCU_column_right_boundary -
         cinfo->coef->MCU_column_left_boundary) *
         compptr->h_samp_factor);
      start_block = coef->pub.MCU_columns_to_skip *
         compptr->h_samp_factor;
   }
#endif
    
    for (block_row = 0; block_row < block_rows; block_row++) {
      buffer_ptr = buffer[block_row];
      output_col = start_block * compptr->DCT_scaled_size;
      buffer_ptr += start_block;
      for (block_num = start_block; block_num < width_in_blocks; block_num++) {
	(*inverse_DCT) (cinfo, compptr, (JCOEFPTR) buffer_ptr,
			output_ptr, output_col);
	buffer_ptr++;
	output_col += compptr->DCT_scaled_size;
      }
      output_ptr += compptr->DCT_scaled_size;
    }
  }

  if (++(cinfo->output_iMCU_row) < cinfo->total_iMCU_rows)
    return JPEG_ROW_COMPLETED;
  return JPEG_SCAN_COMPLETED;
}

#endif 


#ifdef BLOCK_SMOOTHING_SUPPORTED


#define Q01_POS  1
#define Q10_POS  8
#define Q20_POS  16
#define Q11_POS  9
#define Q02_POS  2


LOCAL(boolean)
smoothing_ok (j_decompress_ptr cinfo)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
  boolean smoothing_useful = FALSE;
  int ci, coefi;
  jpeg_component_info *compptr;
  JQUANT_TBL * qtable;
  int * coef_bits;
  int * coef_bits_latch;

  if (! cinfo->progressive_mode || cinfo->coef_bits == NULL)
    return FALSE;

  
  if (coef->coef_bits_latch == NULL)
    coef->coef_bits_latch = (int *)
      (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  cinfo->num_components *
				  (SAVED_COEFS * SIZEOF(int)));
  coef_bits_latch = coef->coef_bits_latch;

  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    
    if ((qtable = compptr->quant_table) == NULL)
      return FALSE;
    
    if (qtable->quantval[0] == 0 ||
	qtable->quantval[Q01_POS] == 0 ||
	qtable->quantval[Q10_POS] == 0 ||
	qtable->quantval[Q20_POS] == 0 ||
	qtable->quantval[Q11_POS] == 0 ||
	qtable->quantval[Q02_POS] == 0)
      return FALSE;
    
    coef_bits = cinfo->coef_bits[ci];
    if (coef_bits[0] < 0)
      return FALSE;
    
    for (coefi = 1; coefi <= 5; coefi++) {
      coef_bits_latch[coefi] = coef_bits[coefi];
      if (coef_bits[coefi] != 0)
	smoothing_useful = TRUE;
    }
    coef_bits_latch += SAVED_COEFS;
  }

  return smoothing_useful;
}



METHODDEF(int)
decompress_smooth_data (j_decompress_ptr cinfo, JSAMPIMAGE output_buf)
{
  my_coef_ptr coef = (my_coef_ptr) cinfo->coef;
  JDIMENSION last_iMCU_row = cinfo->total_iMCU_rows - 1;
  JDIMENSION block_num, last_block_column;
  int ci, block_row, block_rows, access_rows;
  JBLOCKARRAY buffer;
  JBLOCKROW buffer_ptr, prev_block_row, next_block_row;
  JSAMPARRAY output_ptr;
  JDIMENSION output_col;
  jpeg_component_info *compptr;
  inverse_DCT_method_ptr inverse_DCT;
  boolean first_row, last_row;
  JBLOCK workspace;
  int *coef_bits;
  JQUANT_TBL *quanttbl;
  INT32 Q00,Q01,Q02,Q10,Q11,Q20, num;
  int DC1,DC2,DC3,DC4,DC5,DC6,DC7,DC8,DC9;
  int Al, pred;

  
  while (cinfo->input_scan_number <= cinfo->output_scan_number &&
	 ! cinfo->inputctl->eoi_reached) {
    if (cinfo->input_scan_number == cinfo->output_scan_number) {
      JDIMENSION delta = (cinfo->Ss == 0) ? 1 : 0;
      if (cinfo->input_iMCU_row > cinfo->output_iMCU_row+delta)
	break;
    }
    if ((*cinfo->inputctl->consume_input)(cinfo) == JPEG_SUSPENDED)
      return JPEG_SUSPENDED;
  }

  
  for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
       ci++, compptr++) {
    
    if (! compptr->component_needed)
      continue;
    
    if (cinfo->output_iMCU_row < last_iMCU_row) {
      block_rows = compptr->v_samp_factor;
      access_rows = block_rows * 2; 
      last_row = FALSE;
    } else {
      
      block_rows = (int) (compptr->height_in_blocks % compptr->v_samp_factor);
      if (block_rows == 0) block_rows = compptr->v_samp_factor;
      access_rows = block_rows; 
      last_row = TRUE;
    }
    
    if (cinfo->output_iMCU_row > 0) {
      access_rows += compptr->v_samp_factor; 
      buffer = (*cinfo->mem->access_virt_barray)
	((j_common_ptr) cinfo, coef->whole_image[ci],
	 (cinfo->output_iMCU_row - 1) * compptr->v_samp_factor,
	 (JDIMENSION) access_rows, FALSE);
      buffer += compptr->v_samp_factor;	
      first_row = FALSE;
    } else {
      buffer = (*cinfo->mem->access_virt_barray)
	((j_common_ptr) cinfo, coef->whole_image[ci],
	 (JDIMENSION) 0, (JDIMENSION) access_rows, FALSE);
      first_row = TRUE;
    }
    
    coef_bits = coef->coef_bits_latch + (ci * SAVED_COEFS);
    quanttbl = compptr->quant_table;
    Q00 = quanttbl->quantval[0];
    Q01 = quanttbl->quantval[Q01_POS];
    Q10 = quanttbl->quantval[Q10_POS];
    Q20 = quanttbl->quantval[Q20_POS];
    Q11 = quanttbl->quantval[Q11_POS];
    Q02 = quanttbl->quantval[Q02_POS];
    inverse_DCT = cinfo->idct->inverse_DCT[ci];
    output_ptr = output_buf[ci];
    
    for (block_row = 0; block_row < block_rows; block_row++) {
      buffer_ptr = buffer[block_row];
      if (first_row && block_row == 0)
	prev_block_row = buffer_ptr;
      else
	prev_block_row = buffer[block_row-1];
      if (last_row && block_row == block_rows-1)
	next_block_row = buffer_ptr;
      else
	next_block_row = buffer[block_row+1];
      DC1 = DC2 = DC3 = (int) prev_block_row[0][0];
      DC4 = DC5 = DC6 = (int) buffer_ptr[0][0];
      DC7 = DC8 = DC9 = (int) next_block_row[0][0];
      output_col = 0;
      last_block_column = compptr->width_in_blocks - 1;
      for (block_num = 0; block_num <= last_block_column; block_num++) {
	
	jcopy_block_row(buffer_ptr, (JBLOCKROW) workspace, (JDIMENSION) 1);
	
	if (block_num < last_block_column) {
	  DC3 = (int) prev_block_row[1][0];
	  DC6 = (int) buffer_ptr[1][0];
	  DC9 = (int) next_block_row[1][0];
	}
	
	if ((Al=coef_bits[1]) != 0 && workspace[1] == 0) {
	  num = 36 * Q00 * (DC4 - DC6);
	  if (num >= 0) {
	    pred = (int) (((Q01<<7) + num) / (Q01<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q01<<7) - num) / (Q01<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  workspace[1] = (JCOEF) pred;
	}
	
	if ((Al=coef_bits[2]) != 0 && workspace[8] == 0) {
	  num = 36 * Q00 * (DC2 - DC8);
	  if (num >= 0) {
	    pred = (int) (((Q10<<7) + num) / (Q10<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q10<<7) - num) / (Q10<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  workspace[8] = (JCOEF) pred;
	}
	
	if ((Al=coef_bits[3]) != 0 && workspace[16] == 0) {
	  num = 9 * Q00 * (DC2 + DC8 - 2*DC5);
	  if (num >= 0) {
	    pred = (int) (((Q20<<7) + num) / (Q20<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q20<<7) - num) / (Q20<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  workspace[16] = (JCOEF) pred;
	}
	
	if ((Al=coef_bits[4]) != 0 && workspace[9] == 0) {
	  num = 5 * Q00 * (DC1 - DC3 - DC7 + DC9);
	  if (num >= 0) {
	    pred = (int) (((Q11<<7) + num) / (Q11<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q11<<7) - num) / (Q11<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  workspace[9] = (JCOEF) pred;
	}
	
	if ((Al=coef_bits[5]) != 0 && workspace[2] == 0) {
	  num = 9 * Q00 * (DC4 + DC6 - 2*DC5);
	  if (num >= 0) {
	    pred = (int) (((Q02<<7) + num) / (Q02<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	  } else {
	    pred = (int) (((Q02<<7) - num) / (Q02<<8));
	    if (Al > 0 && pred >= (1<<Al))
	      pred = (1<<Al)-1;
	    pred = -pred;
	  }
	  workspace[2] = (JCOEF) pred;
	}
	
	(*inverse_DCT) (cinfo, compptr, (JCOEFPTR) workspace,
			output_ptr, output_col);
	
	DC1 = DC2; DC2 = DC3;
	DC4 = DC5; DC5 = DC6;
	DC7 = DC8; DC8 = DC9;
	buffer_ptr++, prev_block_row++, next_block_row++;
	output_col += compptr->DCT_scaled_size;
      }
      output_ptr += compptr->DCT_scaled_size;
    }
  }

  if (++(cinfo->output_iMCU_row) < cinfo->total_iMCU_rows)
    return JPEG_ROW_COMPLETED;
  return JPEG_SCAN_COMPLETED;
}

#endif 



GLOBAL(void)
jinit_d_coef_controller (j_decompress_ptr cinfo, boolean need_full_buffer)
{
  my_coef_ptr coef;

  coef = (my_coef_ptr)
    (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				SIZEOF(my_coef_controller));
  cinfo->coef = (struct jpeg_d_coef_controller *) coef;
  coef->pub.start_input_pass = start_input_pass;
  coef->pub.start_output_pass = start_output_pass;
  coef->pub.column_left_boundary = 0;
  coef->pub.column_right_boundary = 0;
  coef->pub.MCU_columns_to_skip = 0;
#ifdef BLOCK_SMOOTHING_SUPPORTED
  coef->coef_bits_latch = NULL;
#endif

#ifdef ANDROID_TILE_BASED_DECODE
  if (cinfo->tile_decode) {
    if (cinfo->progressive_mode) {
      int ci, access_rows;
      jpeg_component_info *compptr;

      for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
	   ci++, compptr++) {
        access_rows = compptr->v_samp_factor;
        coef->whole_image[ci] = (*cinfo->mem->request_virt_barray)
	  ((j_common_ptr) cinfo, JPOOL_IMAGE, TRUE,
	   (JDIMENSION) jround_up((long) compptr->width_in_blocks,
				(long) compptr->h_samp_factor),
	   (JDIMENSION) compptr->v_samp_factor, 
	   (JDIMENSION) access_rows);
      }
      coef->pub.consume_data_build_huffman_index =
            consume_data_build_huffman_index_progressive;
      coef->pub.consume_data = consume_data_multi_scan;
      coef->pub.coef_arrays = coef->whole_image; 
      coef->pub.decompress_data = decompress_onepass;
    } else {
      
      JBLOCKROW buffer;
      int i;

      buffer = (JBLOCKROW)
      (*cinfo->mem->alloc_large) ((j_common_ptr) cinfo, JPOOL_IMAGE,
				  D_MAX_BLOCKS_IN_MCU * SIZEOF(JBLOCK));
      for (i = 0; i < D_MAX_BLOCKS_IN_MCU; i++) {
        coef->MCU_buffer[i] = buffer + i;
      }
      coef->pub.consume_data_build_huffman_index =
            consume_data_build_huffman_index_baseline;
      coef->pub.consume_data = dummy_consume_data;
      coef->pub.coef_arrays = NULL; 
      coef->pub.decompress_data = decompress_onepass;
    }
    return;
  }
#endif

  
  if (need_full_buffer) {
#ifdef D_MULTISCAN_FILES_SUPPORTED
    
    
    
    int ci, access_rows;
    jpeg_component_info *compptr;

    for (ci = 0, compptr = cinfo->comp_info; ci < cinfo->num_components;
	 ci++, compptr++) {
      access_rows = compptr->v_samp_factor;
#ifdef BLOCK_SMOOTHING_SUPPORTED
      
      if (cinfo->progressive_mode)
	access_rows *= 3;
#endif
      coef->whole_image[ci] = (*cinfo->mem->request_virt_barray)
	((j_common_ptr) cinfo, JPOOL_IMAGE, TRUE,
	 (JDIMENSION) jround_up((long) compptr->width_in_blocks,
				(long) compptr->h_samp_factor),
	 (JDIMENSION) jround_up((long) compptr->height_in_blocks,
				(long) compptr->v_samp_factor),
	 (JDIMENSION) access_rows);
    }
    coef->pub.consume_data = consume_data;
    coef->pub.decompress_data = decompress_data;
    coef->pub.coef_arrays = coef->whole_image; 
#else
    ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif
  } else {
    
    JBLOCKROW buffer;
    int i;

    buffer = (JBLOCKROW)
      (*cinfo->mem->alloc_large) ((j_common_ptr) cinfo, JPOOL_IMAGE,
		  D_MAX_BLOCKS_IN_MCU * SIZEOF(JBLOCK));
    for (i = 0; i < D_MAX_BLOCKS_IN_MCU; i++) {
      coef->MCU_buffer[i] = buffer + i;
    }
    coef->pub.consume_data = dummy_consume_data;
    coef->pub.decompress_data = decompress_onepass;
    coef->pub.coef_arrays = NULL; 
  }
}