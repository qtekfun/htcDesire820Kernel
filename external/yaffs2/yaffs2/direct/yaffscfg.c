
#include "yaffscfg.h"
#include "yaffsfs.h"
#include <errno.h>

unsigned yaffs_traceMask = 0xFFFFFFFF;


void yaffsfs_SetError(int err)
{
	
	errno = err;
}

void yaffsfs_Lock(void)
{
}

void yaffsfs_Unlock(void)
{
}

__u32 yaffsfs_CurrentTime(void)
{
	return 0;
}

void yaffsfs_LocalInitialisation(void)
{
	
}


#include "yaffs_ramdisk.h"
#include "yaffs_flashif.h"

static yaffs_Device ramDev;
static yaffs_Device bootDev;
static yaffs_Device flashDev;

static yaffsfs_DeviceConfiguration yaffsfs_config[] = {

	{ "/ram", &ramDev},
	{ "/boot", &bootDev},
	{ "/flash", &flashDev},
	{(void *)0,(void *)0}
};


int yaffs_StartUp(void)
{
	
	
	yaffsfs_LocalInitialisation();
	
	

	
	ramDev.nBytesPerChunk = 512;
	ramDev.nChunksPerBlock = 32;
	ramDev.nReservedBlocks = 2; 
	ramDev.startBlock = 1; 
	ramDev.endBlock = 127; 
	ramDev.useNANDECC = 1;
	ramDev.nShortOpCaches = 0;	
	ramDev.genericDevice = (void *) 0;	
	ramDev.writeChunkWithTagsToNAND = yramdisk_WriteChunkWithTagsToNAND;
	ramDev.readChunkWithTagsFromNAND = yramdisk_ReadChunkWithTagsFromNAND;
	ramDev.eraseBlockInNAND = yramdisk_EraseBlockInNAND;
	ramDev.initialiseNAND = yramdisk_InitialiseNAND;

	
	bootDev.nBytesPerChunk = 612;
	bootDev.nChunksPerBlock = 32;
	bootDev.nReservedBlocks = 5;
	bootDev.startBlock = 1; 
	bootDev.endBlock = 127; 
	bootDev.useNANDECC = 0; 
	bootDev.nShortOpCaches = 10; 
	bootDev.genericDevice = (void *) 1;	
	bootDev.writeChunkToNAND = yflash_WriteChunkToNAND;
	bootDev.readChunkFromNAND = yflash_ReadChunkFromNAND;
	bootDev.eraseBlockInNAND = yflash_EraseBlockInNAND;
	bootDev.initialiseNAND = yflash_InitialiseNAND;

		
	flashDev.nBytesPerChunk =  512;
	flashDev.nChunksPerBlock = 32;
	flashDev.nReservedBlocks = 5;
	flashDev.startBlock = 128; 
	flashDev.endBlock = 1023; 
	flashDev.useNANDECC = 0; 
	flashDev.nShortOpCaches = 10; 
	flashDev.genericDevice = (void *) 2;	
	flashDev.writeChunkToNAND = yflash_WriteChunkToNAND;
	flashDev.readChunkFromNAND = yflash_ReadChunkFromNAND;
	flashDev.eraseBlockInNAND = yflash_EraseBlockInNAND;
	flashDev.initialiseNAND = yflash_InitialiseNAND;

	yaffs_initialise(yaffsfs_config);
	
	return 0;
}




