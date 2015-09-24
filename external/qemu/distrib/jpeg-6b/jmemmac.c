/*
 * jmemmac.c
 *
 * Copyright (C) 1992-1997, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * jmemmac.c provides an Apple Macintosh implementation of the system-
 * dependent portion of the JPEG memory manager.
 *
 * If you use jmemmac.c, then you must define USE_MAC_MEMMGR in the
 * JPEG_INTERNALS part of jconfig.h.
 *
 * jmemmac.c uses the Macintosh toolbox routines NewPtr and DisposePtr
 * instead of malloc and free.  It accurately determines the amount of
 * memory available by using CompactMem.  Notice that if left to its
 * own devices, this code can chew up all available space in the
 * application's zone, with the exception of the rather small "slop"
 * factor computed in jpeg_mem_available().  The application can ensure
 * that more space is left over by reducing max_memory_to_use.
 *
 * Large images are swapped to disk using temporary files and System 7.0+'s
 * temporary folder functionality.
 *
 * Note that jmemmac.c depends on two features of MacOS that were first
 * introduced in System 7: FindFolder and the FSSpec-based calls.
 * If your application uses jmemmac.c and is run under System 6 or earlier,
 * and the jpeg library decides it needs a temporary file, it will abort,
 * printing error messages about requiring System 7.  (If no temporary files
 * are created, it will run fine.)
 *
 * If you want to use jmemmac.c in an application that might be used with
 * System 6 or earlier, then you should remove dependencies on FindFolder
 * and the FSSpec calls.  You will need to replace FindFolder with some
 * other mechanism for finding a place to put temporary files, and you
 * should replace the FSSpec calls with their HFS equivalents:
 *
 *     FSpDelete     ->  HDelete
 *     FSpGetFInfo   ->  HGetFInfo
 *     FSpCreate     ->  HCreate
 *     FSpOpenDF     ->  HOpen      *** Note: not HOpenDF ***
 *     FSMakeFSSpec  ->  (fill in spec by hand.)
 *
 * (Use HOpen instead of HOpenDF.  HOpen is just a glue-interface to PBHOpen,
 * which is on all HFS macs.  HOpenDF is a System 7 addition which avoids the
 * ages-old problem of names starting with a period.)
 *
 * Contributed by Sam Bushell (jsam@iagu.on.net) and
 * Dan Gildor (gyld@in-touch.com).
 */

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jmemsys.h"    

#ifndef USE_MAC_MEMMGR	
  You forgot to define USE_MAC_MEMMGR in jconfig.h. 
#endif

#include <Memory.h>     
#include <Files.h>      
#include <Folders.h>    
#include <Script.h>     
#include <Gestalt.h>    

#ifndef TEMP_FILE_NAME		
#define TEMP_FILE_NAME  "JPG%03d.TMP"
#endif

static int next_file_num;	



GLOBAL(void *)
jpeg_get_small (j_common_ptr cinfo, size_t sizeofobject)
{
  return (void *) NewPtr(sizeofobject);
}

GLOBAL(void)
jpeg_free_small (j_common_ptr cinfo, void * object, size_t sizeofobject)
{
  DisposePtr((Ptr) object);
}



GLOBAL(void FAR *)
jpeg_get_large (j_common_ptr cinfo, size_t sizeofobject)
{
  return (void FAR *) NewPtr(sizeofobject);
}

GLOBAL(void)
jpeg_free_large (j_common_ptr cinfo, void FAR * object, size_t sizeofobject)
{
  DisposePtr((Ptr) object);
}



GLOBAL(long)
jpeg_mem_available (j_common_ptr cinfo, long min_bytes_needed,
		    long max_bytes_needed, long already_allocated)
{
  long limit = cinfo->mem->max_memory_to_use - already_allocated;
  long slop, mem;

  
  if (max_bytes_needed > limit && limit > 0)
    max_bytes_needed = limit;
  slop = max_bytes_needed / 16 + 32768L;
  mem = CompactMem(max_bytes_needed + slop) - slop;
  if (mem < 0)
    mem = 0;			
  
  if (mem > limit && limit > 0)
    mem = limit;
  return mem;
}




METHODDEF(void)
read_backing_store (j_common_ptr cinfo, backing_store_ptr info,
		    void FAR * buffer_address,
		    long file_offset, long byte_count)
{
  long bytes = byte_count;
  long retVal;

  if ( SetFPos ( info->temp_file, fsFromStart, file_offset ) != noErr )
    ERREXIT(cinfo, JERR_TFILE_SEEK);

  retVal = FSRead ( info->temp_file, &bytes,
		    (unsigned char *) buffer_address );
  if ( retVal != noErr || bytes != byte_count )
    ERREXIT(cinfo, JERR_TFILE_READ);
}


METHODDEF(void)
write_backing_store (j_common_ptr cinfo, backing_store_ptr info,
		     void FAR * buffer_address,
		     long file_offset, long byte_count)
{
  long bytes = byte_count;
  long retVal;

  if ( SetFPos ( info->temp_file, fsFromStart, file_offset ) != noErr )
    ERREXIT(cinfo, JERR_TFILE_SEEK);

  retVal = FSWrite ( info->temp_file, &bytes,
		     (unsigned char *) buffer_address );
  if ( retVal != noErr || bytes != byte_count )
    ERREXIT(cinfo, JERR_TFILE_WRITE);
}


METHODDEF(void)
close_backing_store (j_common_ptr cinfo, backing_store_ptr info)
{
  FSClose ( info->temp_file );
  FSpDelete ( &(info->tempSpec) );
}



GLOBAL(void)
jpeg_open_backing_store (j_common_ptr cinfo, backing_store_ptr info,
			 long total_bytes_needed)
{
  short         tmpRef, vRefNum;
  long          dirID;
  FInfo         finderInfo;
  FSSpec        theSpec;
  Str255        fName;
  OSErr         osErr;
  long          gestaltResponse = 0;

  
  osErr = Gestalt( gestaltFSAttr, &gestaltResponse );
  if ( ( osErr != noErr )
       || !( gestaltResponse & (1<<gestaltHasFSSpecCalls) ) )
    ERREXITS(cinfo, JERR_TFILE_CREATE, "- System 7.0 or later required");
  

  
  osErr = Gestalt( gestaltFindFolderAttr, &gestaltResponse );
  if ( ( osErr != noErr )
       || !( gestaltResponse & (1<<gestaltFindFolderPresent) ) )
    ERREXITS(cinfo, JERR_TFILE_CREATE, "- System 7.0 or later required.");
  

  osErr = FindFolder ( kOnSystemDisk, kTemporaryFolderType, kCreateFolder,
                       &vRefNum, &dirID );
  if ( osErr != noErr )
    ERREXITS(cinfo, JERR_TFILE_CREATE, "- temporary items folder unavailable");
  

  
  for (;;) {
    next_file_num++;		

    sprintf(info->temp_name, TEMP_FILE_NAME, next_file_num);
    strcpy ( (Ptr)fName+1, info->temp_name );
    *fName = strlen (info->temp_name);
    osErr = FSMakeFSSpec ( vRefNum, dirID, fName, &theSpec );

    if ( (osErr = FSpGetFInfo ( &theSpec, &finderInfo ) ) != noErr )
      break;
  }

  osErr = FSpCreate ( &theSpec, '????', '????', smSystemScript );
  if ( osErr != noErr )
    ERREXITS(cinfo, JERR_TFILE_CREATE, info->temp_name);

  osErr = FSpOpenDF ( &theSpec, fsRdWrPerm, &(info->temp_file) );
  if ( osErr != noErr )
    ERREXITS(cinfo, JERR_TFILE_CREATE, info->temp_name);

  info->tempSpec = theSpec;

  info->read_backing_store = read_backing_store;
  info->write_backing_store = write_backing_store;
  info->close_backing_store = close_backing_store;
  TRACEMSS(cinfo, 1, JTRC_TFILE_OPEN, info->temp_name);
}



GLOBAL(long)
jpeg_mem_init (j_common_ptr cinfo)
{
  next_file_num = 0;

  return FreeMem();
}

GLOBAL(void)
jpeg_mem_term (j_common_ptr cinfo)
{
  
}
