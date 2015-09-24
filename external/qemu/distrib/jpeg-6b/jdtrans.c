/*
 * jdtrans.c
 *
 * Copyright (C) 1995-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains library routines for transcoding decompression,
 * that is, reading raw DCT coefficient arrays from an input JPEG file.
 * The routines in jdapimin.c will also be needed by a transcoder.
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"


LOCAL(void) transdecode_master_selection JPP((j_decompress_ptr cinfo));



GLOBAL(jvirt_barray_ptr *)
jpeg_read_coefficients (j_decompress_ptr cinfo)
{
  if (cinfo->global_state == DSTATE_READY) {
    
    transdecode_master_selection(cinfo);
    cinfo->global_state = DSTATE_RDCOEFS;
  }
  if (cinfo->global_state == DSTATE_RDCOEFS) {
    
    for (;;) {
      int retcode;
      
      if (cinfo->progress != NULL)
        (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
      
      retcode = (*cinfo->inputctl->consume_input) (cinfo);
      if (retcode == JPEG_SUSPENDED)
        return NULL;
      if (retcode == JPEG_REACHED_EOI)
        break;
      
      if (cinfo->progress != NULL &&
	  (retcode == JPEG_ROW_COMPLETED || retcode == JPEG_REACHED_SOS)) {
        if (++cinfo->progress->pass_counter >= cinfo->progress->pass_limit) {
	  
        cinfo->progress->pass_limit += (long) cinfo->total_iMCU_rows;
        }
      }
    }
    
    cinfo->global_state = DSTATE_STOPPING;
  }
  if ((cinfo->global_state == DSTATE_STOPPING ||
       cinfo->global_state == DSTATE_BUFIMAGE) && cinfo->buffered_image) {
    return cinfo->coef->coef_arrays;
  }
  
  ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  return NULL;			
}

LOCAL(boolean)
jpeg_build_huffman_index_progressive(j_decompress_ptr cinfo,
        huffman_index *index)
{
  if (cinfo->global_state == DSTATE_READY) {
    printf("Progressive Mode\n");
    
    transdecode_master_selection(cinfo);
    cinfo->global_state = DSTATE_RDCOEFS;
  }
  if (cinfo->global_state == DSTATE_RDCOEFS) {
    int mcu, i;
    cinfo->marker->get_sos_marker_position(cinfo, index);

    
    for (mcu = 0; mcu < cinfo->total_iMCU_rows; mcu++) {
      int retcode = 0;
      
      if (cinfo->progress != NULL)
        (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
      
      jinit_phuff_decoder(cinfo);
      for (i = 0; i < index->scan_count; i++) {
        (*cinfo->inputctl->finish_input_pass) (cinfo);
        jset_input_stream_position(cinfo, index->scan[i].bitstream_offset);
        cinfo->unread_marker = 0;
        retcode = (*cinfo->inputctl->consume_input_build_huffman_index)
                    (cinfo, index, i);
        if (retcode == JPEG_REACHED_EOI)
          break;
        cinfo->input_iMCU_row = mcu;
        if (mcu != 0)
          (*cinfo->entropy->configure_huffman_decoder)
                (cinfo, index->scan[i].prev_MCU_offset);
        cinfo->input_scan_number = i;
        retcode = (*cinfo->inputctl->consume_input_build_huffman_index)
                    (cinfo, index, i);
      }
      if (retcode == JPEG_SUSPENDED)
        return FALSE;
      if (retcode == JPEG_REACHED_EOI)
        break;
      
      if (cinfo->progress != NULL &&
	  (retcode == JPEG_ROW_COMPLETED || retcode == JPEG_REACHED_SOS)) {
        if (++cinfo->progress->pass_counter >= cinfo->progress->pass_limit) {
	  
          cinfo->progress->pass_limit += (long) cinfo->total_iMCU_rows;
        }
      }
    }
    cinfo->global_state = DSTATE_STOPPING;
  }
  if ((cinfo->global_state == DSTATE_STOPPING ||
       cinfo->global_state == DSTATE_BUFIMAGE) && cinfo->buffered_image) {
    return TRUE;
  }
  
  ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  return FALSE;			
}

LOCAL(boolean)
jpeg_build_huffman_index_baseline(j_decompress_ptr cinfo, huffman_index *index)
{
  if (cinfo->global_state == DSTATE_READY) {
    printf("Baseline Mode\n");
    
    transdecode_master_selection(cinfo);
    cinfo->global_state = DSTATE_RDCOEFS;
  }
  if (cinfo->global_state == DSTATE_RDCOEFS) {
    
    for (;;) {
      int retcode;
      
      if (cinfo->progress != NULL)
        (*cinfo->progress->progress_monitor) ((j_common_ptr) cinfo);
      
      retcode = (*cinfo->inputctl->consume_input_build_huffman_index)
                    (cinfo, index, 0);
      if (retcode == JPEG_SUSPENDED)
        return FALSE;
      if (retcode == JPEG_REACHED_EOI)
        break;
      if (retcode == JPEG_SCAN_COMPLETED)
        break;

      
      if (cinfo->progress != NULL &&
	  (retcode == JPEG_ROW_COMPLETED || retcode == JPEG_REACHED_SOS)) {
        if (++cinfo->progress->pass_counter >= cinfo->progress->pass_limit) {
	  
        cinfo->progress->pass_limit += (long) cinfo->total_iMCU_rows;
        }
      }
    }
    
    cinfo->global_state = DSTATE_STOPPING;
  }
  if ((cinfo->global_state == DSTATE_STOPPING ||
       cinfo->global_state == DSTATE_BUFIMAGE) && cinfo->buffered_image) {
    return TRUE;
  }
  
  ERREXIT1(cinfo, JERR_BAD_STATE, cinfo->global_state);
  return FALSE;			
}

GLOBAL(boolean)
jpeg_build_huffman_index(j_decompress_ptr cinfo, huffman_index *index)
{
    cinfo->tile_decode = TRUE;
    if (cinfo->progressive_mode)
      return jpeg_build_huffman_index_progressive(cinfo, index);
    else
      return jpeg_build_huffman_index_baseline(cinfo, index);
}


LOCAL(void)
transdecode_master_selection (j_decompress_ptr cinfo)
{
  
  cinfo->buffered_image = TRUE;

  
  if (cinfo->arith_code) {
    ERREXIT(cinfo, JERR_ARITH_NOTIMPL);
  } else {
    if (cinfo->progressive_mode) {
#ifdef D_PROGRESSIVE_SUPPORTED
      jinit_phuff_decoder(cinfo);
#else
      ERREXIT(cinfo, JERR_NOT_COMPILED);
#endif
    } else {
      jinit_huff_decoder(cinfo);
    }
  }

  
  jinit_d_coef_controller(cinfo, TRUE);

  
  (*cinfo->mem->realize_virt_arrays) ((j_common_ptr) cinfo);

  
  (*cinfo->inputctl->start_input_pass) (cinfo);

  
  if (cinfo->progress != NULL) {
    int nscans;
    
    if (cinfo->progressive_mode) {
      
      nscans = 2 + 3 * cinfo->num_components;
    } else if (cinfo->inputctl->has_multiple_scans) {
      
      nscans = cinfo->num_components;
    } else {
      nscans = 1;
    }
    cinfo->progress->pass_counter = 0L;
    cinfo->progress->pass_limit = (long) cinfo->total_iMCU_rows * nscans;
    cinfo->progress->completed_passes = 0;
    cinfo->progress->total_passes = 1;
  }
}
