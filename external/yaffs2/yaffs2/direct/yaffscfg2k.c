
#include "yaffscfg.h"
#include "yaffsfs.h"
#include "yaffs_fileem2k.h"
#include "yaffs_nandemul2k.h"

#include <errno.h>

unsigned yaffs_traceMask = YAFFS_TRACE_SCAN |  YAFFS_TRACE_GC  | YAFFS_TRACE_ERASE | YAFFS_TRACE_TRACING | YAFFS_TRACE_ALLOCATE | YAFFS_TRACE_CHECKPOINT;


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
#include "yaffs_nandemul2k.h"

static yaffs_Device ramDev;
static yaffs_Device bootDev;
static yaffs_Device flashDev;
static yaffs_Device ram2kDev;

static yaffsfs_DeviceConfiguration yaffsfs_config[] = {
#if 0
	{ "/ram", &ramDev},
	{ "/boot", &bootDev},
	{ "/flash/", &flashDev},
	{ "/ram2k", &ram2kDev},
	{(void *)0,(void *)0}
#else
	{ "/", &ramDev},
	{ "/flash/boot", &bootDev},
	{ "/flash/flash", &flashDev},
	{ "/ram2k", &ram2kDev},
	{(void *)0,(void *)0} 
#endif
};


int yaffs_StartUp(void)
{
	
	
	yaffsfs_LocalInitialisation();
	
	
	
	memset(&ramDev,0,sizeof(ramDev));
	ramDev.nDataBytesPerChunk = 512;
	ramDev.nChunksPerBlock = 32;
	ramDev.nReservedBlocks = 2; 
	ramDev.startBlock = 0; 
	ramDev.endBlock = 127; 
	
	ramDev.nShortOpCaches = 0;	
	ramDev.genericDevice = (void *) 0;	
	ramDev.writeChunkWithTagsToNAND = yramdisk_WriteChunkWithTagsToNAND;
	ramDev.readChunkWithTagsFromNAND = yramdisk_ReadChunkWithTagsFromNAND;
	ramDev.eraseBlockInNAND = yramdisk_EraseBlockInNAND;
	ramDev.initialiseNAND = yramdisk_InitialiseNAND;

	
	memset(&bootDev,0,sizeof(bootDev));
	bootDev.nDataBytesPerChunk = 512;
	bootDev.nChunksPerBlock = 32;
	bootDev.nReservedBlocks = 5;
	bootDev.startBlock = 0; 
	bootDev.endBlock = 63; 
	
	bootDev.nShortOpCaches = 10; 
	bootDev.genericDevice = (void *) 1;	
	bootDev.writeChunkWithTagsToNAND = yflash_WriteChunkWithTagsToNAND;
	bootDev.readChunkWithTagsFromNAND = yflash_ReadChunkWithTagsFromNAND;
	bootDev.eraseBlockInNAND = yflash_EraseBlockInNAND;
	bootDev.initialiseNAND = yflash_InitialiseNAND;
	bootDev.markNANDBlockBad = yflash_MarkNANDBlockBad;
	bootDev.queryNANDBlock = yflash_QueryNANDBlock;



	
	
	
	
	memset(&flashDev,0,sizeof(flashDev));

	flashDev.nDataBytesPerChunk = 2048;
	flashDev.nChunksPerBlock = 64;
	flashDev.nReservedBlocks = 5;
	flashDev.nCheckpointReservedBlocks = 5;
	
	
	flashDev.startBlock = 20; 
	
	flashDev.endBlock = yflash_GetNumberOfBlocks()-1;
	flashDev.isYaffs2 = 1;
	flashDev.wideTnodesDisabled=0;
	flashDev.nShortOpCaches = 10; 
	flashDev.genericDevice = (void *) 2;	
	flashDev.writeChunkWithTagsToNAND = yflash_WriteChunkWithTagsToNAND;
	flashDev.readChunkWithTagsFromNAND = yflash_ReadChunkWithTagsFromNAND;
	flashDev.eraseBlockInNAND = yflash_EraseBlockInNAND;
	flashDev.initialiseNAND = yflash_InitialiseNAND;
	flashDev.markNANDBlockBad = yflash_MarkNANDBlockBad;
	flashDev.queryNANDBlock = yflash_QueryNANDBlock;

	
	
	
	
	memset(&ram2kDev,0,sizeof(ram2kDev));

	ram2kDev.nDataBytesPerChunk = nandemul2k_GetBytesPerChunk();
	ram2kDev.nChunksPerBlock = nandemul2k_GetChunksPerBlock();
	ram2kDev.nReservedBlocks = 5;
	ram2kDev.startBlock = 0; 
	
	ram2kDev.endBlock = nandemul2k_GetNumberOfBlocks() - 1; 
	ram2kDev.isYaffs2 = 1;
	ram2kDev.nShortOpCaches = 10; 
	ram2kDev.genericDevice = (void *) 3;	
	ram2kDev.writeChunkWithTagsToNAND = nandemul2k_WriteChunkWithTagsToNAND;
	ram2kDev.readChunkWithTagsFromNAND = nandemul2k_ReadChunkWithTagsFromNAND;
	ram2kDev.eraseBlockInNAND = nandemul2k_EraseBlockInNAND;
	ram2kDev.initialiseNAND = nandemul2k_InitialiseNAND;
	ram2kDev.markNANDBlockBad = nandemul2k_MarkNANDBlockBad;
	ram2kDev.queryNANDBlock = nandemul2k_QueryNANDBlock;

	yaffs_initialise(yaffsfs_config);
	
	return 0;
}



void SetCheckpointReservedBlocks(int n)
{
	flashDev.nCheckpointReservedBlocks = n;
}

