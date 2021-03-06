/*
 * jconfig.doc
 *
 * Copyright (C) 1991-1994, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file documents the configuration options that are required to
 * customize the JPEG software for a particular system.
 *
 * The actual configuration options for a particular installation are stored
 * in jconfig.h.  On many machines, jconfig.h can be generated automatically
 * or copied from one of the "canned" jconfig files that we supply.  But if
 * you need to generate a jconfig.h file by hand, this file tells you how.
 *
 * DO NOT EDIT THIS FILE --- IT WON'T ACCOMPLISH ANYTHING.
 * EDIT A COPY NAMED JCONFIG.H.
 */



#define HAVE_PROTOTYPES

#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT



#undef CHAR_IS_UNSIGNED

#define HAVE_STDDEF_H

#define HAVE_STDLIB_H

#undef NEED_BSD_STRINGS

#undef NEED_SYS_TYPES_H

#undef NEED_FAR_POINTERS

#undef NEED_SHORT_EXTERNAL_NAMES

#undef INCOMPLETE_TYPES_BROKEN



#ifdef JPEG_INTERNALS

#undef RIGHT_SHIFT_IS_UNSIGNED


#endif 



#ifdef JPEG_CJPEG_DJPEG


#define BMP_SUPPORTED		
#define GIF_SUPPORTED		
#define PPM_SUPPORTED		
#undef RLE_SUPPORTED		
#define TARGA_SUPPORTED		

#undef TWO_FILE_COMMANDLINE

#undef NEED_SIGNAL_CATCHER

#undef DONT_USE_B_MODE

#undef PROGRESS_REPORT


#endif 
