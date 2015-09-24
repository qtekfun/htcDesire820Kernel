/*
 * YAFFS: Yet another FFS. A NAND-flash specific file system.
 *
 * Copyright (C) 2002 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

const char *yaffs_guts_c_version =
    "$Id: yaffs_guts.c,v 1.40 2006/10/13 08:52:49 charles Exp $";

#include "yportenv.h"

#include "yaffsinterface.h"
#include "yaffs_guts.h"
#include "yaffs_tagsvalidity.h"

#include "yaffs_tagscompat.h"
#ifndef CONFIG_YAFFS_OWN_SORT
#include "yaffs_qsort.h"
#endif
#include "yaffs_nand.h"

#include "yaffs_checkptrw.h"

#include "yaffs_nand.h"
#include "yaffs_packedtags2.h"


#ifdef CONFIG_YAFFS_WINCE
void yfsd_LockYAFFS(BOOL fsLockOnly);
void yfsd_UnlockYAFFS(BOOL fsLockOnly);
#endif

#define YAFFS_PASSIVE_GC_CHUNKS 2

#include "yaffs_ecc.h"


static void yaffs_RetireBlock(yaffs_Device * dev, int blockInNAND);
static void yaffs_HandleWriteChunkError(yaffs_Device * dev, int chunkInNAND, int erasedOk);
static void yaffs_HandleWriteChunkOk(yaffs_Device * dev, int chunkInNAND,
				     const __u8 * data,
				     const yaffs_ExtendedTags * tags);
static void yaffs_HandleUpdateChunk(yaffs_Device * dev, int chunkInNAND,
				    const yaffs_ExtendedTags * tags);

static int yaffs_UnlinkObject( yaffs_Object *obj);
static int yaffs_ObjectHasCachedWriteData(yaffs_Object *obj);

static void yaffs_HardlinkFixup(yaffs_Device *dev, yaffs_Object *hardList);

static int yaffs_WriteNewChunkWithTagsToNAND(yaffs_Device * dev,
					     const __u8 * buffer,
					     yaffs_ExtendedTags * tags,
					     int useReserve);
static int yaffs_PutChunkIntoFile(yaffs_Object * in, int chunkInInode,
				  int chunkInNAND, int inScan);

static yaffs_Object *yaffs_CreateNewObject(yaffs_Device * dev, int number,
					   yaffs_ObjectType type);
static void yaffs_AddObjectToDirectory(yaffs_Object * directory,
				       yaffs_Object * obj);
static int yaffs_UpdateObjectHeader(yaffs_Object * in, const YCHAR * name,
				    int force, int isShrink, int shadows);
static void yaffs_RemoveObjectFromDirectory(yaffs_Object * obj);
static int yaffs_CheckStructures(void);
static int yaffs_DeleteWorker(yaffs_Object * in, yaffs_Tnode * tn, __u32 level,
			      int chunkOffset, int *limit);
static int yaffs_DoGenericObjectDeletion(yaffs_Object * in);

static yaffs_BlockInfo *yaffs_GetBlockInfo(yaffs_Device * dev, int blockNo);

static __u8 *yaffs_GetTempBuffer(yaffs_Device * dev, int lineNo);
static void yaffs_ReleaseTempBuffer(yaffs_Device * dev, __u8 * buffer,
				    int lineNo);

static int yaffs_CheckChunkErased(struct yaffs_DeviceStruct *dev,
				  int chunkInNAND);

static int yaffs_UnlinkWorker(yaffs_Object * obj);
static void yaffs_DestroyObject(yaffs_Object * obj);

static int yaffs_TagsMatch(const yaffs_ExtendedTags * tags, int objectId,
			   int chunkInObject);

loff_t yaffs_GetFileSize(yaffs_Object * obj);

static int yaffs_AllocateChunk(yaffs_Device * dev, int useReserve, yaffs_BlockInfo **blockUsedPtr);

static void yaffs_VerifyFreeChunks(yaffs_Device * dev);

#ifdef YAFFS_PARANOID
static int yaffs_CheckFileSanity(yaffs_Object * in);
#else
#define yaffs_CheckFileSanity(in)
#endif

static void yaffs_InvalidateWholeChunkCache(yaffs_Object * in);
static void yaffs_InvalidateChunkCache(yaffs_Object * object, int chunkId);

static void yaffs_InvalidateCheckpoint(yaffs_Device *dev);




static void yaffs_AddrToChunk(yaffs_Device *dev, loff_t addr, __u32 *chunk, __u32 *offset)
{
	if(dev->chunkShift){
		
		*chunk  = (__u32)(addr >> dev->chunkShift);
		*offset = (__u32)(addr & dev->chunkMask);
	}
	else if(dev->crumbsPerChunk)
	{
		
		*offset = (__u32)(addr & dev->crumbMask);
		addr >>= dev->crumbShift;
		*chunk = ((__u32)addr)/dev->crumbsPerChunk;
		*offset += ((addr - (*chunk * dev->crumbsPerChunk)) << dev->crumbShift);
	}
	else
		YBUG();
}

 
static __u32 ShiftsGE(__u32 x)
{
	int extraBits;
	int nShifts;
	
	nShifts = extraBits = 0;
	
	while(x>1){
		if(x & 1) extraBits++;
		x>>=1;
		nShifts++;
	}

	if(extraBits) 
		nShifts++;
		
	return nShifts;
}

 
static __u32 ShiftDiv(__u32 x)
{
	int nShifts;
	
	nShifts =  0;
	
	if(!x) return 0;
	
	while( !(x&1)){
		x>>=1;
		nShifts++;
	}
		
	return nShifts;
}




static __u8 *yaffs_GetTempBuffer(yaffs_Device * dev, int lineNo)
{
	int i, j;
	for (i = 0; i < YAFFS_N_TEMP_BUFFERS; i++) {
		if (dev->tempBuffer[i].line == 0) {
			dev->tempBuffer[i].line = lineNo;
			if ((i + 1) > dev->maxTemp) {
				dev->maxTemp = i + 1;
				for (j = 0; j <= i; j++)
					dev->tempBuffer[j].maxLine =
					    dev->tempBuffer[j].line;
			}

			return dev->tempBuffer[i].buffer;
		}
	}

	T(YAFFS_TRACE_BUFFERS,
	  (TSTR("Out of temp buffers at line %d, other held by lines:"),
	   lineNo));
	for (i = 0; i < YAFFS_N_TEMP_BUFFERS; i++) {
		T(YAFFS_TRACE_BUFFERS, (TSTR(" %d "), dev->tempBuffer[i].line));
	}
	T(YAFFS_TRACE_BUFFERS, (TSTR(" " TENDSTR)));


	dev->unmanagedTempAllocations++;
	return YMALLOC(dev->nDataBytesPerChunk);

}

static void yaffs_ReleaseTempBuffer(yaffs_Device * dev, __u8 * buffer,
				    int lineNo)
{
	int i;
	for (i = 0; i < YAFFS_N_TEMP_BUFFERS; i++) {
		if (dev->tempBuffer[i].buffer == buffer) {
			dev->tempBuffer[i].line = 0;
			return;
		}
	}

	if (buffer) {
		
		T(YAFFS_TRACE_BUFFERS,
		  (TSTR("Releasing unmanaged temp buffer in line %d" TENDSTR),
		   lineNo));
		YFREE(buffer);
		dev->unmanagedTempDeallocations++;
	}

}

int yaffs_IsManagedTempBuffer(yaffs_Device * dev, const __u8 * buffer)
{
	int i;
	for (i = 0; i < YAFFS_N_TEMP_BUFFERS; i++) {
		if (dev->tempBuffer[i].buffer == buffer)
			return 1;

	}

    for (i = 0; i < dev->nShortOpCaches; i++) {
        if( dev->srCache[i].data == buffer )
            return 1;

    }

    if (buffer == dev->checkpointBuffer)
      return 1;

    T(YAFFS_TRACE_ALWAYS,
	  (TSTR("yaffs: unmaged buffer detected.\n" TENDSTR)));
    return 0;
}


static Y_INLINE __u8 *yaffs_BlockBits(yaffs_Device * dev, int blk)
{
	if (blk < dev->internalStartBlock || blk > dev->internalEndBlock) {
		T(YAFFS_TRACE_ERROR,
		  (TSTR("**>> yaffs: BlockBits block %d is not valid" TENDSTR),
		   blk));
		YBUG();
	}
	return dev->chunkBits +
	    (dev->chunkBitmapStride * (blk - dev->internalStartBlock));
}

static Y_INLINE void yaffs_ClearChunkBits(yaffs_Device * dev, int blk)
{
	__u8 *blkBits = yaffs_BlockBits(dev, blk);

	memset(blkBits, 0, dev->chunkBitmapStride);
}

static Y_INLINE void yaffs_ClearChunkBit(yaffs_Device * dev, int blk, int chunk)
{
	__u8 *blkBits = yaffs_BlockBits(dev, blk);

	blkBits[chunk / 8] &= ~(1 << (chunk & 7));
}

static Y_INLINE void yaffs_SetChunkBit(yaffs_Device * dev, int blk, int chunk)
{
	__u8 *blkBits = yaffs_BlockBits(dev, blk);

	blkBits[chunk / 8] |= (1 << (chunk & 7));
}

static Y_INLINE int yaffs_CheckChunkBit(yaffs_Device * dev, int blk, int chunk)
{
	__u8 *blkBits = yaffs_BlockBits(dev, blk);
	return (blkBits[chunk / 8] & (1 << (chunk & 7))) ? 1 : 0;
}

static Y_INLINE int yaffs_StillSomeChunkBits(yaffs_Device * dev, int blk)
{
	__u8 *blkBits = yaffs_BlockBits(dev, blk);
	int i;
	for (i = 0; i < dev->chunkBitmapStride; i++) {
		if (*blkBits)
			return 1;
		blkBits++;
	}
	return 0;
}

 
static Y_INLINE int yaffs_HashFunction(int n)
{
	n = abs(n);
	return (n % YAFFS_NOBJECT_BUCKETS);
}

 
yaffs_Object *yaffs_Root(yaffs_Device * dev)
{
	return dev->rootDir;
}

yaffs_Object *yaffs_LostNFound(yaffs_Device * dev)
{
	return dev->lostNFoundDir;
}


 
int yaffs_CheckFF(__u8 * buffer, int nBytes)
{
	
	while (nBytes--) {
		if (*buffer != 0xFF)
			return 0;
		buffer++;
	}
	return 1;
}

static int yaffs_CheckChunkErased(struct yaffs_DeviceStruct *dev,
				  int chunkInNAND)
{

	int retval = YAFFS_OK;
	__u8 *data = yaffs_GetTempBuffer(dev, __LINE__);
	yaffs_ExtendedTags tags;
	int result;

	result = yaffs_ReadChunkWithTagsFromNAND(dev, chunkInNAND, data, &tags);
	
	if(tags.eccResult > YAFFS_ECC_RESULT_NO_ERROR)
		retval = YAFFS_FAIL;
		

	if (!yaffs_CheckFF(data, dev->nDataBytesPerChunk) || tags.chunkUsed) {
		T(YAFFS_TRACE_NANDACCESS,
		  (TSTR("Chunk %d not erased" TENDSTR), chunkInNAND));
		retval = YAFFS_FAIL;
	}

	yaffs_ReleaseTempBuffer(dev, data, __LINE__);

	return retval;

}

static int yaffs_WriteNewChunkWithTagsToNAND(struct yaffs_DeviceStruct *dev,
					     const __u8 * data,
					     yaffs_ExtendedTags * tags,
					     int useReserve)
{
	int chunk;

	int writeOk = 0;
	int erasedOk = 1;
	int attempts = 0;
	yaffs_BlockInfo *bi;
	
	yaffs_InvalidateCheckpoint(dev);

	do {
		chunk = yaffs_AllocateChunk(dev, useReserve,&bi);

		if (chunk >= 0) {
			/* First check this chunk is erased, if it needs checking.
			 * The checking policy (unless forced always on) is as follows:
			 * Check the first page we try to write in a block.
			 * - If the check passes then we don't need to check any more.
			 * - If the check fails, we check again...
			 * If the block has been erased, we don't need to check.
			 *
			 * However, if the block has been prioritised for gc, then
			 * we think there might be something odd about this block
			 * and stop using it.
			 *
			 * Rationale:
			 * We should only ever see chunks that have not been erased
			 * if there was a partially written chunk due to power loss
			 * This checking policy should catch that case with very
			 * few checks and thus save a lot of checks that are most likely not
			 * needed.
			 */
			 
			 if(bi->gcPrioritise){
			 		yaffs_DeleteChunk(dev, chunk, 1, __LINE__);
			} else {
#ifdef CONFIG_YAFFS_ALWAYS_CHECK_CHUNK_ERASED

				bi->skipErasedCheck = 0;

#endif
				if(!bi->skipErasedCheck){
					erasedOk = yaffs_CheckChunkErased(dev, chunk);
					if(erasedOk && !bi->gcPrioritise)
						bi->skipErasedCheck = 1;
				}

				if (!erasedOk) {
					T(YAFFS_TRACE_ERROR,
					  (TSTR
					   ("**>> yaffs chunk %d was not erased"
					    TENDSTR), chunk));
				} else {
					writeOk =
					    yaffs_WriteChunkWithTagsToNAND(dev, chunk,
									   data, tags);
				}
			
				attempts++;

				if (writeOk) {
					yaffs_HandleWriteChunkOk(dev, chunk, data, tags);
				
				} else {
					
					yaffs_HandleWriteChunkError(dev, chunk, erasedOk);
				}
			}
		}

	} while (chunk >= 0 && !writeOk);

	if (attempts > 1) {
		T(YAFFS_TRACE_ERROR,
		  (TSTR("**>> yaffs write required %d attempts" TENDSTR),
		   attempts));
		dev->nRetriedWrites += (attempts - 1);
	}

	return chunk;
}

 
static void yaffs_RetireBlock(yaffs_Device * dev, int blockInNAND)
{

	yaffs_InvalidateCheckpoint(dev);
	
	yaffs_MarkBlockBad(dev, blockInNAND);

	yaffs_GetBlockInfo(dev, blockInNAND)->blockState =
	    YAFFS_BLOCK_STATE_DEAD;

	dev->nRetiredBlocks++;
}

 
static void yaffs_HandleWriteChunkOk(yaffs_Device * dev, int chunkInNAND,
				     const __u8 * data,
				     const yaffs_ExtendedTags * tags)
{
}

static void yaffs_HandleUpdateChunk(yaffs_Device * dev, int chunkInNAND,
				    const yaffs_ExtendedTags * tags)
{
}

void yaffs_HandleChunkError(yaffs_Device *dev, yaffs_BlockInfo *bi)
{
	if(!bi->gcPrioritise){
		bi->gcPrioritise = 1;
		dev->hasPendingPrioritisedGCs = 1;
		bi->chunkErrorStrikes ++;
		
		if(bi->chunkErrorStrikes > 3){
			bi->needsRetiring = 1; 
			T(YAFFS_TRACE_ALWAYS, (TSTR("yaffs: Block struck out" TENDSTR)));

		}
		
	}
}

static void yaffs_HandleWriteChunkError(yaffs_Device * dev, int chunkInNAND, int erasedOk)
{

	int blockInNAND = chunkInNAND / dev->nChunksPerBlock;
	yaffs_BlockInfo *bi = yaffs_GetBlockInfo(dev, blockInNAND);

	yaffs_HandleChunkError(dev,bi);
		
	
	if(erasedOk ) {
		
		bi->needsRetiring = 1;
		
	}
	
	
	yaffs_DeleteChunk(dev, chunkInNAND, 1, __LINE__);
}


 

static __u16 yaffs_CalcNameSum(const YCHAR * name)
{
	__u16 sum = 0;
	__u16 i = 1;

	YUCHAR *bname = (YUCHAR *) name;
	if (bname) {
		while ((*bname) && (i <= YAFFS_MAX_NAME_LENGTH)) {

#ifdef CONFIG_YAFFS_CASE_INSENSITIVE
			sum += yaffs_toupper(*bname) * i;
#else
			sum += (*bname) * i;
#endif
			i++;
			bname++;
		}
	}
	return sum;
}

static void yaffs_SetObjectName(yaffs_Object * obj, const YCHAR * name)
{
#ifdef CONFIG_YAFFS_SHORT_NAMES_IN_RAM
	if (name && yaffs_strlen(name) <= YAFFS_SHORT_NAME_LENGTH) {
		yaffs_strcpy(obj->shortName, name);
	} else {
		obj->shortName[0] = _Y('\0');
	}
#endif
	obj->sum = yaffs_CalcNameSum(name);
}

 

static int yaffs_CreateTnodes(yaffs_Device * dev, int nTnodes)
{
	int i;
	int tnodeSize;
	yaffs_Tnode *newTnodes;
	__u8 *mem;
	yaffs_Tnode *curr;
	yaffs_Tnode *next;
	yaffs_TnodeList *tnl;

	if (nTnodes < 1)
		return YAFFS_OK;
		
	tnodeSize = (dev->tnodeWidth * YAFFS_NTNODES_LEVEL0)/8;

	

	newTnodes = YMALLOC(nTnodes * tnodeSize);
	mem = (__u8 *)newTnodes;

	if (!newTnodes) {
		T(YAFFS_TRACE_ERROR,
		  (TSTR("yaffs: Could not allocate Tnodes" TENDSTR)));
		return YAFFS_FAIL;
	}

	
#if 0
	for (i = 0; i < nTnodes - 1; i++) {
		newTnodes[i].internal[0] = &newTnodes[i + 1];
#ifdef CONFIG_YAFFS_TNODE_LIST_DEBUG
		newTnodes[i].internal[YAFFS_NTNODES_INTERNAL] = (void *)1;
#endif
	}

	newTnodes[nTnodes - 1].internal[0] = dev->freeTnodes;
#ifdef CONFIG_YAFFS_TNODE_LIST_DEBUG
	newTnodes[nTnodes - 1].internal[YAFFS_NTNODES_INTERNAL] = (void *)1;
#endif
	dev->freeTnodes = newTnodes;
#else
	
	for(i = 0; i < nTnodes -1; i++) {
		curr = (yaffs_Tnode *) &mem[i * tnodeSize];
		next = (yaffs_Tnode *) &mem[(i+1) * tnodeSize];
		curr->internal[0] = next;
	}
	
	curr = (yaffs_Tnode *) &mem[(nTnodes - 1) * tnodeSize];
	curr->internal[0] = dev->freeTnodes;
	dev->freeTnodes = (yaffs_Tnode *)mem;

#endif


	dev->nFreeTnodes += nTnodes;
	dev->nTnodesCreated += nTnodes;

	 
	tnl = YMALLOC(sizeof(yaffs_TnodeList));
	if (!tnl) {
		T(YAFFS_TRACE_ERROR,
		  (TSTR
		   ("yaffs: Could not add tnodes to management list" TENDSTR)));

	} else {
		tnl->tnodes = newTnodes;
		tnl->next = dev->allocatedTnodeList;
		dev->allocatedTnodeList = tnl;
	}

	T(YAFFS_TRACE_ALLOCATE, (TSTR("yaffs: Tnodes added" TENDSTR)));

	return YAFFS_OK;
}


static yaffs_Tnode *yaffs_GetTnodeRaw(yaffs_Device * dev)
{
	yaffs_Tnode *tn = NULL;

	
	if (!dev->freeTnodes) {
		yaffs_CreateTnodes(dev, YAFFS_ALLOCATION_NTNODES);
	}

	if (dev->freeTnodes) {
		tn = dev->freeTnodes;
#ifdef CONFIG_YAFFS_TNODE_LIST_DEBUG
		if (tn->internal[YAFFS_NTNODES_INTERNAL] != (void *)1) {
			
			T(YAFFS_TRACE_ALWAYS,
			  (TSTR("yaffs: Tnode list bug 1" TENDSTR)));
		}
#endif
		dev->freeTnodes = dev->freeTnodes->internal[0];
		dev->nFreeTnodes--;
	}

	return tn;
}

static yaffs_Tnode *yaffs_GetTnode(yaffs_Device * dev)
{
	yaffs_Tnode *tn = yaffs_GetTnodeRaw(dev);
	
	if(tn)
		memset(tn, 0, (dev->tnodeWidth * YAFFS_NTNODES_LEVEL0)/8);

	return tn;	
}

static void yaffs_FreeTnode(yaffs_Device * dev, yaffs_Tnode * tn)
{
	if (tn) {
#ifdef CONFIG_YAFFS_TNODE_LIST_DEBUG
		if (tn->internal[YAFFS_NTNODES_INTERNAL] != 0) {
			
			T(YAFFS_TRACE_ALWAYS,
			  (TSTR("yaffs: Tnode list bug 2" TENDSTR)));
		}
		tn->internal[YAFFS_NTNODES_INTERNAL] = (void *)1;
#endif
		tn->internal[0] = dev->freeTnodes;
		dev->freeTnodes = tn;
		dev->nFreeTnodes++;
	}
}

static void yaffs_DeinitialiseTnodes(yaffs_Device * dev)
{
	
	yaffs_TnodeList *tmp;

	while (dev->allocatedTnodeList) {
		tmp = dev->allocatedTnodeList->next;

		YFREE(dev->allocatedTnodeList->tnodes);
		YFREE(dev->allocatedTnodeList);
		dev->allocatedTnodeList = tmp;

	}

	dev->freeTnodes = NULL;
	dev->nFreeTnodes = 0;
}

static void yaffs_InitialiseTnodes(yaffs_Device * dev)
{
	dev->allocatedTnodeList = NULL;
	dev->freeTnodes = NULL;
	dev->nFreeTnodes = 0;
	dev->nTnodesCreated = 0;

}


void yaffs_PutLevel0Tnode(yaffs_Device *dev, yaffs_Tnode *tn, unsigned pos, unsigned val)
{
  __u32 *map = (__u32 *)tn;
  __u32 bitInMap;
  __u32 bitInWord;
  __u32 wordInMap;
  __u32 mask;
  
  pos &= YAFFS_TNODES_LEVEL0_MASK;
  val >>= dev->chunkGroupBits;
  
  bitInMap = pos * dev->tnodeWidth;
  wordInMap = bitInMap /32;
  bitInWord = bitInMap & (32 -1);
  
  mask = dev->tnodeMask << bitInWord;
  
  map[wordInMap] &= ~mask;
  map[wordInMap] |= (mask & (val << bitInWord));
  
  if(dev->tnodeWidth > (32-bitInWord)) {
    bitInWord = (32 - bitInWord);
    wordInMap++;;
    mask = dev->tnodeMask >> ( bitInWord);
    map[wordInMap] &= ~mask;
    map[wordInMap] |= (mask & (val >> bitInWord));
  }
}

__u32 yaffs_GetChunkGroupBase(yaffs_Device *dev, yaffs_Tnode *tn, unsigned pos)
{
  __u32 *map = (__u32 *)tn;
  __u32 bitInMap;
  __u32 bitInWord;
  __u32 wordInMap;
  __u32 val;
  
  pos &= YAFFS_TNODES_LEVEL0_MASK;
  
  bitInMap = pos * dev->tnodeWidth;
  wordInMap = bitInMap /32;
  bitInWord = bitInMap & (32 -1);
  
  val = map[wordInMap] >> bitInWord;
  
  if(dev->tnodeWidth > (32-bitInWord)) {
    bitInWord = (32 - bitInWord);
    wordInMap++;;
    val |= (map[wordInMap] << bitInWord);
  }
  
  val &= dev->tnodeMask;
  val <<= dev->chunkGroupBits;
  
  return val;
}



static yaffs_Tnode *yaffs_FindLevel0Tnode(yaffs_Device * dev,
					  yaffs_FileStructure * fStruct,
					  __u32 chunkId)
{

	yaffs_Tnode *tn = fStruct->top;
	__u32 i;
	int requiredTallness;
	int level = fStruct->topLevel;

	
	if (level < 0 || level > YAFFS_TNODES_MAX_LEVEL) {
		return NULL;
	}

	if (chunkId > YAFFS_MAX_CHUNK_ID) {
		return NULL;
	}

	

	i = chunkId >> YAFFS_TNODES_LEVEL0_BITS;
	requiredTallness = 0;
	while (i) {
		i >>= YAFFS_TNODES_INTERNAL_BITS;
		requiredTallness++;
	}

	if (requiredTallness > fStruct->topLevel) {
		
		return NULL;
	}

	
	while (level > 0 && tn) {
		tn = tn->
		    internal[(chunkId >>
			       ( YAFFS_TNODES_LEVEL0_BITS + 
			         (level - 1) *
			         YAFFS_TNODES_INTERNAL_BITS)
			      ) &
			     YAFFS_TNODES_INTERNAL_MASK];
		level--;

	}

	return tn;
}

 
static yaffs_Tnode *yaffs_AddOrFindLevel0Tnode(yaffs_Device * dev,
					       yaffs_FileStructure * fStruct,
					       __u32 chunkId,
					       yaffs_Tnode *passedTn)
{

	int requiredTallness;
	int i;
	int l;
	yaffs_Tnode *tn;

	__u32 x;


	
	if (fStruct->topLevel < 0 || fStruct->topLevel > YAFFS_TNODES_MAX_LEVEL) {
		return NULL;
	}

	if (chunkId > YAFFS_MAX_CHUNK_ID) {
		return NULL;
	}

	

	x = chunkId >> YAFFS_TNODES_LEVEL0_BITS;
	requiredTallness = 0;
	while (x) {
		x >>= YAFFS_TNODES_INTERNAL_BITS;
		requiredTallness++;
	}


	if (requiredTallness > fStruct->topLevel) {
		
		for (i = fStruct->topLevel; i < requiredTallness; i++) {
		
			tn = yaffs_GetTnode(dev);

			if (tn) {
				tn->internal[0] = fStruct->top;
				fStruct->top = tn;
			} else {
				T(YAFFS_TRACE_ERROR,
				  (TSTR("yaffs: no more tnodes" TENDSTR)));
			}
		}

		fStruct->topLevel = requiredTallness;
	}

	

	l = fStruct->topLevel;
	tn = fStruct->top;
	
	if(l > 0) {
		while (l > 0 && tn) {
			x = (chunkId >>
			     ( YAFFS_TNODES_LEVEL0_BITS +
			      (l - 1) * YAFFS_TNODES_INTERNAL_BITS)) &
			    YAFFS_TNODES_INTERNAL_MASK;


			if((l>1) && !tn->internal[x]){
				
				tn->internal[x] = yaffs_GetTnode(dev);

			} else if(l == 1) {
				
			 	if (passedTn) {
					
					if(tn->internal[x])
						yaffs_FreeTnode(dev,tn->internal[x]);
					tn->internal[x] = passedTn;
			
				} else if(!tn->internal[x]) {
					
					tn->internal[x] = yaffs_GetTnode(dev);
				}
			}
		
			tn = tn->internal[x];
			l--;
		}
	} else {
		
		if(passedTn) {
			memcpy(tn,passedTn,(dev->tnodeWidth * YAFFS_NTNODES_LEVEL0)/8);
			yaffs_FreeTnode(dev,passedTn);
		}
	}

	return tn;
}

static int yaffs_FindChunkInGroup(yaffs_Device * dev, int theChunk,
				  yaffs_ExtendedTags * tags, int objectId,
				  int chunkInInode)
{
	int j;

	for (j = 0; theChunk && j < dev->chunkGroupSize; j++) {
		if (yaffs_CheckChunkBit
		    (dev, theChunk / dev->nChunksPerBlock,
		     theChunk % dev->nChunksPerBlock)) {
			yaffs_ReadChunkWithTagsFromNAND(dev, theChunk, NULL,
							tags);
			if (yaffs_TagsMatch(tags, objectId, chunkInInode)) {
				
				return theChunk;

			}
		}
		theChunk++;
	}
	return -1;
}



static int yaffs_DeleteWorker(yaffs_Object * in, yaffs_Tnode * tn, __u32 level,
			      int chunkOffset, int *limit)
{
	int i;
	int chunkInInode;
	int theChunk;
	yaffs_ExtendedTags tags;
	int foundChunk;
	yaffs_Device *dev = in->myDev;

	int allDone = 1;

	if (tn) {
		if (level > 0) {

			for (i = YAFFS_NTNODES_INTERNAL - 1; allDone && i >= 0;
			     i--) {
				if (tn->internal[i]) {
					if (limit && (*limit) < 0) {
						allDone = 0;
					} else {
						allDone =
						    yaffs_DeleteWorker(in,
								       tn->
								       internal
								       [i],
								       level -
								       1,
								       (chunkOffset
									<<
									YAFFS_TNODES_INTERNAL_BITS)
								       + i,
								       limit);
					}
					if (allDone) {
						yaffs_FreeTnode(dev,
								tn->
								internal[i]);
						tn->internal[i] = NULL;
					}
				}

			}
			return (allDone) ? 1 : 0;
		} else if (level == 0) {
			int hitLimit = 0;

			for (i = YAFFS_NTNODES_LEVEL0 - 1; i >= 0 && !hitLimit;
			     i--) {
			        theChunk = yaffs_GetChunkGroupBase(dev,tn,i);
				if (theChunk) {

					chunkInInode =
					    (chunkOffset <<
					     YAFFS_TNODES_LEVEL0_BITS) + i;

					foundChunk =
					    yaffs_FindChunkInGroup(dev,
								   theChunk,
								   &tags,
								   in->objectId,
								   chunkInInode);

					if (foundChunk > 0) {
						yaffs_DeleteChunk(dev,
								  foundChunk, 1,
								  __LINE__);
						in->nDataChunks--;
						if (limit) {
							*limit = *limit - 1;
							if (*limit <= 0) {
								hitLimit = 1;
							}
						}

					}

					yaffs_PutLevel0Tnode(dev,tn,i,0);
				}

			}
			return (i < 0) ? 1 : 0;

		}

	}

	return 1;

}

static void yaffs_SoftDeleteChunk(yaffs_Device * dev, int chunk)
{

	yaffs_BlockInfo *theBlock;

	T(YAFFS_TRACE_DELETION, (TSTR("soft delete chunk %d" TENDSTR), chunk));

	theBlock = yaffs_GetBlockInfo(dev, chunk / dev->nChunksPerBlock);
	if (theBlock) {
		theBlock->softDeletions++;
		dev->nFreeChunks++;
	}
}

 
static int yaffs_SoftDeleteWorker(yaffs_Object * in, yaffs_Tnode * tn,
				  __u32 level, int chunkOffset)
{
	int i;
	int theChunk;
	int allDone = 1;
	yaffs_Device *dev = in->myDev;

	if (tn) {
		if (level > 0) {

			for (i = YAFFS_NTNODES_INTERNAL - 1; allDone && i >= 0;
			     i--) {
				if (tn->internal[i]) {
					allDone =
					    yaffs_SoftDeleteWorker(in,
								   tn->
								   internal[i],
								   level - 1,
								   (chunkOffset
								    <<
								    YAFFS_TNODES_INTERNAL_BITS)
								   + i);
					if (allDone) {
						yaffs_FreeTnode(dev,
								tn->
								internal[i]);
						tn->internal[i] = NULL;
					} else {
						
					}
				}
			}
			return (allDone) ? 1 : 0;
		} else if (level == 0) {

			for (i = YAFFS_NTNODES_LEVEL0 - 1; i >= 0; i--) {
				theChunk = yaffs_GetChunkGroupBase(dev,tn,i);
				if (theChunk) {
					yaffs_SoftDeleteChunk(dev, theChunk);
					yaffs_PutLevel0Tnode(dev,tn,i,0);
				}

			}
			return 1;

		}

	}

	return 1;

}

static void yaffs_SoftDeleteFile(yaffs_Object * obj)
{
	if (obj->deleted &&
	    obj->variantType == YAFFS_OBJECT_TYPE_FILE && !obj->softDeleted) {
		if (obj->nDataChunks <= 0) {
			
			yaffs_FreeTnode(obj->myDev,
					obj->variant.fileVariant.top);
			obj->variant.fileVariant.top = NULL;
			T(YAFFS_TRACE_TRACING,
			  (TSTR("yaffs: Deleting empty file %d" TENDSTR),
			   obj->objectId));
			yaffs_DoGenericObjectDeletion(obj);
		} else {
			yaffs_SoftDeleteWorker(obj,
					       obj->variant.fileVariant.top,
					       obj->variant.fileVariant.
					       topLevel, 0);
			obj->softDeleted = 1;
		}
	}
}


static yaffs_Tnode *yaffs_PruneWorker(yaffs_Device * dev, yaffs_Tnode * tn,
				      __u32 level, int del0)
{
	int i;
	int hasData;

	if (tn) {
		hasData = 0;

		for (i = 0; i < YAFFS_NTNODES_INTERNAL; i++) {
			if (tn->internal[i] && level > 0) {
				tn->internal[i] =
				    yaffs_PruneWorker(dev, tn->internal[i],
						      level - 1,
						      (i == 0) ? del0 : 1);
			}

			if (tn->internal[i]) {
				hasData++;
			}
		}

		if (hasData == 0 && del0) {
			

			yaffs_FreeTnode(dev, tn);
			tn = NULL;
		}

	}

	return tn;

}

static int yaffs_PruneFileStructure(yaffs_Device * dev,
				    yaffs_FileStructure * fStruct)
{
	int i;
	int hasData;
	int done = 0;
	yaffs_Tnode *tn;

	if (fStruct->topLevel > 0) {
		fStruct->top =
		    yaffs_PruneWorker(dev, fStruct->top, fStruct->topLevel, 0);


		while (fStruct->topLevel && !done) {
			tn = fStruct->top;

			hasData = 0;
			for (i = 1; i < YAFFS_NTNODES_INTERNAL; i++) {
				if (tn->internal[i]) {
					hasData++;
				}
			}

			if (!hasData) {
				fStruct->top = tn->internal[0];
				fStruct->topLevel--;
				yaffs_FreeTnode(dev, tn);
			} else {
				done = 1;
			}
		}
	}

	return YAFFS_OK;
}


static int yaffs_CreateFreeObjects(yaffs_Device * dev, int nObjects)
{
	int i;
	yaffs_Object *newObjects;
	yaffs_ObjectList *list;

	if (nObjects < 1)
		return YAFFS_OK;

	
	newObjects = YMALLOC(nObjects * sizeof(yaffs_Object));

	if (!newObjects) {
		T(YAFFS_TRACE_ALLOCATE,
		  (TSTR("yaffs: Could not allocate more objects" TENDSTR)));
		return YAFFS_FAIL;
	}
	
	
	for (i = 0; i < nObjects - 1; i++) {
		newObjects[i].siblings.next =
		    (struct list_head *)(&newObjects[i + 1]);
	}

	newObjects[nObjects - 1].siblings.next = (void *)dev->freeObjects;
	dev->freeObjects = newObjects;
	dev->nFreeObjects += nObjects;
	dev->nObjectsCreated += nObjects;

	

	list = YMALLOC(sizeof(yaffs_ObjectList));
	if (!list) {
		T(YAFFS_TRACE_ALLOCATE,
		  (TSTR("Could not add objects to management list" TENDSTR)));
	} else {
		list->objects = newObjects;
		list->next = dev->allocatedObjectList;
		dev->allocatedObjectList = list;
	}

	return YAFFS_OK;
}


static yaffs_Object *yaffs_AllocateEmptyObject(yaffs_Device * dev)
{
	yaffs_Object *tn = NULL;

	
	if (!dev->freeObjects) {
		yaffs_CreateFreeObjects(dev, YAFFS_ALLOCATION_NOBJECTS);
	}

	if (dev->freeObjects) {
		tn = dev->freeObjects;
		dev->freeObjects =
		    (yaffs_Object *) (dev->freeObjects->siblings.next);
		dev->nFreeObjects--;

		

		memset(tn, 0, sizeof(yaffs_Object));
		tn->myDev = dev;
		tn->chunkId = -1;
		tn->variantType = YAFFS_OBJECT_TYPE_UNKNOWN;
		INIT_LIST_HEAD(&(tn->hardLinks));
		INIT_LIST_HEAD(&(tn->hashLink));
		INIT_LIST_HEAD(&tn->siblings);

		if (dev->lostNFoundDir) {
			yaffs_AddObjectToDirectory(dev->lostNFoundDir, tn);
		}
	}

	return tn;
}

static yaffs_Object *yaffs_CreateFakeDirectory(yaffs_Device * dev, int number,
					       __u32 mode)
{

	yaffs_Object *obj =
	    yaffs_CreateNewObject(dev, number, YAFFS_OBJECT_TYPE_DIRECTORY);
	if (obj) {
		obj->fake = 1;		
		obj->renameAllowed = 0;	
		obj->unlinkAllowed = 0;	
		obj->deleted = 0;
		obj->unlinked = 0;
		obj->yst_mode = mode;
		obj->myDev = dev;
		obj->chunkId = 0;	
	}

	return obj;

}

static void yaffs_UnhashObject(yaffs_Object * tn)
{
	int bucket;
	yaffs_Device *dev = tn->myDev;

	
	if (!list_empty(&tn->hashLink)) {
		list_del_init(&tn->hashLink);
		bucket = yaffs_HashFunction(tn->objectId);
		dev->objectBucket[bucket].count--;
	}

}

static void yaffs_FreeObject(yaffs_Object * tn)
{

	yaffs_Device *dev = tn->myDev;

#ifdef  __KERNEL__
	if (tn->myInode) {
		tn->deferedFree = 1;
		return;
	}
#endif

	yaffs_UnhashObject(tn);

	
	tn->siblings.next = (struct list_head *)(dev->freeObjects);
	dev->freeObjects = tn;
	dev->nFreeObjects++;
}

#ifdef __KERNEL__

void yaffs_HandleDeferedFree(yaffs_Object * obj)
{
	if (obj->deferedFree) {
		yaffs_FreeObject(obj);
	}
}

#endif

static void yaffs_DeinitialiseObjects(yaffs_Device * dev)
{
	

	yaffs_ObjectList *tmp;

	while (dev->allocatedObjectList) {
		tmp = dev->allocatedObjectList->next;
		YFREE(dev->allocatedObjectList->objects);
		YFREE(dev->allocatedObjectList);

		dev->allocatedObjectList = tmp;
	}

	dev->freeObjects = NULL;
	dev->nFreeObjects = 0;
}

static void yaffs_InitialiseObjects(yaffs_Device * dev)
{
	int i;

	dev->allocatedObjectList = NULL;
	dev->freeObjects = NULL;
	dev->nFreeObjects = 0;

	for (i = 0; i < YAFFS_NOBJECT_BUCKETS; i++) {
		INIT_LIST_HEAD(&dev->objectBucket[i].list);
		dev->objectBucket[i].count = 0;
	}

}

static int yaffs_FindNiceObjectBucket(yaffs_Device * dev)
{
	static int x = 0;
	int i;
	int l = 999;
	int lowest = 999999;

	

	for (i = 0; i < 10 && lowest > 0; i++) {
		x++;
		x %= YAFFS_NOBJECT_BUCKETS;
		if (dev->objectBucket[x].count < lowest) {
			lowest = dev->objectBucket[x].count;
			l = x;
		}

	}


	for (i = 0; i < 10 && lowest > 3; i++) {
		x++;
		x %= YAFFS_NOBJECT_BUCKETS;
		if (dev->objectBucket[x].count < lowest) {
			lowest = dev->objectBucket[x].count;
			l = x;
		}

	}

	return l;
}

static int yaffs_CreateNewObjectNumber(yaffs_Device * dev)
{
	int bucket = yaffs_FindNiceObjectBucket(dev);


	int found = 0;
	struct list_head *i;

	__u32 n = (__u32) bucket;

	

	while (!found) {
		found = 1;
		n += YAFFS_NOBJECT_BUCKETS;
		if (1 || dev->objectBucket[bucket].count > 0) {
			list_for_each(i, &dev->objectBucket[bucket].list) {
				
				if (i
				    && list_entry(i, yaffs_Object,
						  hashLink)->objectId == n) {
					found = 0;
				}
			}
		}
	}


	return n;
}

static void yaffs_HashObject(yaffs_Object * in)
{
	int bucket = yaffs_HashFunction(in->objectId);
	yaffs_Device *dev = in->myDev;

	list_add(&in->hashLink, &dev->objectBucket[bucket].list);
	dev->objectBucket[bucket].count++;

}

yaffs_Object *yaffs_FindObjectByNumber(yaffs_Device * dev, __u32 number)
{
	int bucket = yaffs_HashFunction(number);
	struct list_head *i;
	yaffs_Object *in;

	list_for_each(i, &dev->objectBucket[bucket].list) {
		
		if (i) {
			in = list_entry(i, yaffs_Object, hashLink);
			if (in->objectId == number) {
#ifdef __KERNEL__
				
				if (in->deferedFree)
					return NULL;
#endif

				return in;
			}
		}
	}

	return NULL;
}

yaffs_Object *yaffs_CreateNewObject(yaffs_Device * dev, int number,
				    yaffs_ObjectType type)
{

	yaffs_Object *theObject;

	if (number < 0) {
		number = yaffs_CreateNewObjectNumber(dev);
	}

	theObject = yaffs_AllocateEmptyObject(dev);

	if (theObject) {
		theObject->fake = 0;
		theObject->renameAllowed = 1;
		theObject->unlinkAllowed = 1;
		theObject->objectId = number;
		yaffs_HashObject(theObject);
		theObject->variantType = type;
#ifdef CONFIG_YAFFS_WINCE
		yfsd_WinFileTimeNow(theObject->win_atime);
		theObject->win_ctime[0] = theObject->win_mtime[0] =
		    theObject->win_atime[0];
		theObject->win_ctime[1] = theObject->win_mtime[1] =
		    theObject->win_atime[1];

#else

		theObject->yst_atime = theObject->yst_mtime =
		    theObject->yst_ctime = Y_CURRENT_TIME;
#endif
		switch (type) {
		case YAFFS_OBJECT_TYPE_FILE:
			theObject->variant.fileVariant.fileSize = 0;
			theObject->variant.fileVariant.scannedFileSize = 0;
			theObject->variant.fileVariant.shrinkSize = 0xFFFFFFFF;	
			theObject->variant.fileVariant.topLevel = 0;
			theObject->variant.fileVariant.top =
			    yaffs_GetTnode(dev);
			break;
		case YAFFS_OBJECT_TYPE_DIRECTORY:
			INIT_LIST_HEAD(&theObject->variant.directoryVariant.
				       children);
			break;
		case YAFFS_OBJECT_TYPE_SYMLINK:
		case YAFFS_OBJECT_TYPE_HARDLINK:
		case YAFFS_OBJECT_TYPE_SPECIAL:
			
			break;
		case YAFFS_OBJECT_TYPE_UNKNOWN:
			
			break;
		}
	}

	return theObject;
}

static yaffs_Object *yaffs_FindOrCreateObjectByNumber(yaffs_Device * dev,
						      int number,
						      yaffs_ObjectType type)
{
	yaffs_Object *theObject = NULL;

	if (number > 0) {
		theObject = yaffs_FindObjectByNumber(dev, number);
	}

	if (!theObject) {
		theObject = yaffs_CreateNewObject(dev, number, type);
	}

	return theObject;

}
			

static YCHAR *yaffs_CloneString(const YCHAR * str)
{
	YCHAR *newStr = NULL;

	if (str && *str) {
		newStr = YMALLOC((yaffs_strlen(str) + 1) * sizeof(YCHAR));
		yaffs_strcpy(newStr, str);
	}

	return newStr;

}

 
static yaffs_Object *yaffs_MknodObject(yaffs_ObjectType type,
				       yaffs_Object * parent,
				       const YCHAR * name,
				       __u32 mode,
				       __u32 uid,
				       __u32 gid,
				       yaffs_Object * equivalentObject,
				       const YCHAR * aliasString, __u32 rdev)
{
	yaffs_Object *in;

	yaffs_Device *dev = parent->myDev;

	
	if (yaffs_FindObjectByName(parent, name)) {
		return NULL;
	}

	in = yaffs_CreateNewObject(dev, -1, type);

	if (in) {
		in->chunkId = -1;
		in->valid = 1;
		in->variantType = type;

		in->yst_mode = mode;

#ifdef CONFIG_YAFFS_WINCE
		yfsd_WinFileTimeNow(in->win_atime);
		in->win_ctime[0] = in->win_mtime[0] = in->win_atime[0];
		in->win_ctime[1] = in->win_mtime[1] = in->win_atime[1];

#else
		in->yst_atime = in->yst_mtime = in->yst_ctime = Y_CURRENT_TIME;

		in->yst_rdev = rdev;
		in->yst_uid = uid;
		in->yst_gid = gid;
#endif
		in->nDataChunks = 0;

		yaffs_SetObjectName(in, name);
		in->dirty = 1;

		yaffs_AddObjectToDirectory(parent, in);

		in->myDev = parent->myDev;

		switch (type) {
		case YAFFS_OBJECT_TYPE_SYMLINK:
			in->variant.symLinkVariant.alias =
			    yaffs_CloneString(aliasString);
			break;
		case YAFFS_OBJECT_TYPE_HARDLINK:
			in->variant.hardLinkVariant.equivalentObject =
			    equivalentObject;
			in->variant.hardLinkVariant.equivalentObjectId =
			    equivalentObject->objectId;
			list_add(&in->hardLinks, &equivalentObject->hardLinks);
			break;
		case YAFFS_OBJECT_TYPE_FILE:	
		case YAFFS_OBJECT_TYPE_DIRECTORY:
		case YAFFS_OBJECT_TYPE_SPECIAL:
		case YAFFS_OBJECT_TYPE_UNKNOWN:
			
			break;
		}

		if (yaffs_UpdateObjectHeader(in, name, 0, 0, 0) < 0) {
			
			yaffs_DestroyObject(in);
			in = NULL;
		}

	}

	return in;
}

yaffs_Object *yaffs_MknodFile(yaffs_Object * parent, const YCHAR * name,
			      __u32 mode, __u32 uid, __u32 gid)
{
	return yaffs_MknodObject(YAFFS_OBJECT_TYPE_FILE, parent, name, mode,
				 uid, gid, NULL, NULL, 0);
}

yaffs_Object *yaffs_MknodDirectory(yaffs_Object * parent, const YCHAR * name,
				   __u32 mode, __u32 uid, __u32 gid)
{
	return yaffs_MknodObject(YAFFS_OBJECT_TYPE_DIRECTORY, parent, name,
				 mode, uid, gid, NULL, NULL, 0);
}

yaffs_Object *yaffs_MknodSpecial(yaffs_Object * parent, const YCHAR * name,
				 __u32 mode, __u32 uid, __u32 gid, __u32 rdev)
{
	return yaffs_MknodObject(YAFFS_OBJECT_TYPE_SPECIAL, parent, name, mode,
				 uid, gid, NULL, NULL, rdev);
}

yaffs_Object *yaffs_MknodSymLink(yaffs_Object * parent, const YCHAR * name,
				 __u32 mode, __u32 uid, __u32 gid,
				 const YCHAR * alias)
{
	return yaffs_MknodObject(YAFFS_OBJECT_TYPE_SYMLINK, parent, name, mode,
				 uid, gid, NULL, alias, 0);
}

yaffs_Object *yaffs_Link(yaffs_Object * parent, const YCHAR * name,
			 yaffs_Object * equivalentObject)
{
	
	equivalentObject = yaffs_GetEquivalentObject(equivalentObject);

	if (yaffs_MknodObject
	    (YAFFS_OBJECT_TYPE_HARDLINK, parent, name, 0, 0, 0,
	     equivalentObject, NULL, 0)) {
		return equivalentObject;
	} else {
		return NULL;
	}

}

static int yaffs_ChangeObjectName(yaffs_Object * obj, yaffs_Object * newDir,
				  const YCHAR * newName, int force, int shadows)
{
	int unlinkOp;
	int deleteOp;

	yaffs_Object *existingTarget;

	if (newDir == NULL) {
		newDir = obj->parent;	
	}

	if (newDir->variantType != YAFFS_OBJECT_TYPE_DIRECTORY) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("tragendy: yaffs_ChangeObjectName: newDir is not a directory"
		    TENDSTR)));
		YBUG();
	}
	
	
	if (obj->myDev->isYaffs2) {
		unlinkOp = (newDir == obj->myDev->unlinkedDir);
	} else {
		unlinkOp = (newDir == obj->myDev->unlinkedDir
			    && obj->variantType == YAFFS_OBJECT_TYPE_FILE);
	}

	deleteOp = (newDir == obj->myDev->deletedDir);

	existingTarget = yaffs_FindObjectByName(newDir, newName);

	if ((unlinkOp ||
	     deleteOp ||
	     force ||
	     (shadows > 0) ||
	     !existingTarget) &&
	    newDir->variantType == YAFFS_OBJECT_TYPE_DIRECTORY) {
		yaffs_SetObjectName(obj, newName);
		obj->dirty = 1;

		yaffs_AddObjectToDirectory(newDir, obj);

		if (unlinkOp)
			obj->unlinked = 1;

		
		if (yaffs_UpdateObjectHeader(obj, newName, 0, deleteOp, shadows)>= 0)
			return YAFFS_OK;
	}

	return YAFFS_FAIL;
}

int yaffs_RenameObject(yaffs_Object * oldDir, const YCHAR * oldName,
		       yaffs_Object * newDir, const YCHAR * newName)
{
	yaffs_Object *obj;
	yaffs_Object *existingTarget;
	int force = 0;

#ifdef CONFIG_YAFFS_CASE_INSENSITIVE
	if (oldDir == newDir && yaffs_strcmp(oldName, newName) == 0) {
		force = 1;
	}
#endif

	obj = yaffs_FindObjectByName(oldDir, oldName);
	
	if (obj->variantType == YAFFS_OBJECT_TYPE_SYMLINK &&
	    yaffs_strlen(newName) > YAFFS_MAX_ALIAS_LENGTH)
	  
	  return YAFFS_FAIL;
	else if (obj->variantType != YAFFS_OBJECT_TYPE_SYMLINK &&
		 yaffs_strlen(newName) > YAFFS_MAX_NAME_LENGTH)
	  
	  return YAFFS_FAIL;

	if (obj && obj->renameAllowed) {

		

		existingTarget = yaffs_FindObjectByName(newDir, newName);
		if (existingTarget &&
		    existingTarget->variantType == YAFFS_OBJECT_TYPE_DIRECTORY &&
		    !list_empty(&existingTarget->variant.directoryVariant.children)) {
			
			return YAFFS_FAIL;	
		} else if (existingTarget && existingTarget != obj) {
			yaffs_ChangeObjectName(obj, newDir, newName, force,
					       existingTarget->objectId);
			yaffs_UnlinkObject(existingTarget);
		}

		return yaffs_ChangeObjectName(obj, newDir, newName, 1, 0);
	}
	return YAFFS_FAIL;
}


static int yaffs_InitialiseBlocks(yaffs_Device * dev)
{
	int nBlocks = dev->internalEndBlock - dev->internalStartBlock + 1;
	
	dev->allocationBlock = -1;	

	
	dev->blockInfo = YMALLOC(nBlocks * sizeof(yaffs_BlockInfo));
	if(!dev->blockInfo){
		dev->blockInfo = YMALLOC_ALT(nBlocks * sizeof(yaffs_BlockInfo));
		dev->blockInfoAlt = 1;
	}
	else
		dev->blockInfoAlt = 0;
	
	
	dev->chunkBitmapStride = (dev->nChunksPerBlock + 7) / 8; 
	dev->chunkBits = YMALLOC(dev->chunkBitmapStride * nBlocks);
	if(!dev->chunkBits){
		dev->chunkBits = YMALLOC_ALT(dev->chunkBitmapStride * nBlocks);
		dev->chunkBitsAlt = 1;
	}
	else
		dev->chunkBitsAlt = 0;
	
	if (dev->blockInfo && dev->chunkBits) {
		memset(dev->blockInfo, 0, nBlocks * sizeof(yaffs_BlockInfo));
		memset(dev->chunkBits, 0, dev->chunkBitmapStride * nBlocks);
		return YAFFS_OK;
	}

	return YAFFS_FAIL;

}

static void yaffs_DeinitialiseBlocks(yaffs_Device * dev)
{
	if(dev->blockInfoAlt)
		YFREE_ALT(dev->blockInfo);
	else
		YFREE(dev->blockInfo);
	dev->blockInfoAlt = 0;

	dev->blockInfo = NULL;
	
	if(dev->chunkBitsAlt)
		YFREE_ALT(dev->chunkBits);
	else
		YFREE(dev->chunkBits);
	dev->chunkBitsAlt = 0;
	dev->chunkBits = NULL;
}

static int yaffs_BlockNotDisqualifiedFromGC(yaffs_Device * dev,
					    yaffs_BlockInfo * bi)
{
	int i;
	__u32 seq;
	yaffs_BlockInfo *b;

	if (!dev->isYaffs2)
		return 1;	

	if (!bi->hasShrinkHeader)
		return 1;	

	if (!dev->oldestDirtySequence) {
		seq = dev->sequenceNumber;

		for (i = dev->internalStartBlock; i <= dev->internalEndBlock;
		     i++) {
			b = yaffs_GetBlockInfo(dev, i);
			if (b->blockState == YAFFS_BLOCK_STATE_FULL &&
			    (b->pagesInUse - b->softDeletions) <
			    dev->nChunksPerBlock && b->sequenceNumber < seq) {
				seq = b->sequenceNumber;
			}
		}
		dev->oldestDirtySequence = seq;
	}

	return (bi->sequenceNumber <= dev->oldestDirtySequence);

	return 1;

}


static int yaffs_FindBlockForGarbageCollection(yaffs_Device * dev,
					       int aggressive)
{

	int b = dev->currentDirtyChecker;

	int i;
	int iterations;
	int dirtiest = -1;
	int pagesInUse;
	int prioritised=0;
	yaffs_BlockInfo *bi;
	static int nonAggressiveSkip = 0;
	int pendingPrioritisedExist = 0;
	
	
	if(dev->hasPendingPrioritisedGCs){
		for(i = dev->internalStartBlock; i < dev->internalEndBlock && !prioritised; i++){

			bi = yaffs_GetBlockInfo(dev, i);
			if(bi->gcPrioritise)
				pendingPrioritisedExist = 1;
			if(bi->blockState == YAFFS_BLOCK_STATE_FULL &&
			   bi->gcPrioritise &&
			   yaffs_BlockNotDisqualifiedFromGC(dev, bi)){
				pagesInUse = (bi->pagesInUse - bi->softDeletions);
				dirtiest = i;
				prioritised = 1;
				aggressive = 1; 
			}
		}
		
		if(!pendingPrioritisedExist) 
			dev->hasPendingPrioritisedGCs = 0;
	}


	nonAggressiveSkip--;

	if (!aggressive && (nonAggressiveSkip > 0)) {
		return -1;
	}

	if(!prioritised)
		pagesInUse =
	    		(aggressive) ? dev->nChunksPerBlock : YAFFS_PASSIVE_GC_CHUNKS + 1;

	if (aggressive) {
		iterations =
		    dev->internalEndBlock - dev->internalStartBlock + 1;
	} else {
		iterations =
		    dev->internalEndBlock - dev->internalStartBlock + 1;
		iterations = iterations / 16;
		if (iterations > 200) {
			iterations = 200;
		}
	}

	for (i = 0; i <= iterations && pagesInUse > 0 && !prioritised; i++) {
		b++;
		if (b < dev->internalStartBlock || b > dev->internalEndBlock) {
			b = dev->internalStartBlock;
		}

		if (b < dev->internalStartBlock || b > dev->internalEndBlock) {
			T(YAFFS_TRACE_ERROR,
			  (TSTR("**>> Block %d is not valid" TENDSTR), b));
			YBUG();
		}

		bi = yaffs_GetBlockInfo(dev, b);

#if 0
		if (bi->blockState == YAFFS_BLOCK_STATE_CHECKPOINT) {
			dirtiest = b;
			pagesInUse = 0;
		}
		else 
#endif

		if (bi->blockState == YAFFS_BLOCK_STATE_FULL &&
		       (bi->pagesInUse - bi->softDeletions) < pagesInUse &&
		        yaffs_BlockNotDisqualifiedFromGC(dev, bi)) {
			dirtiest = b;
			pagesInUse = (bi->pagesInUse - bi->softDeletions);
		}
	}

	dev->currentDirtyChecker = b;

	if (dirtiest > 0) {
		T(YAFFS_TRACE_GC,
		  (TSTR("GC Selected block %d with %d free, prioritised:%d" TENDSTR), dirtiest,
		   dev->nChunksPerBlock - pagesInUse,prioritised));
	}

	dev->oldestDirtySequence = 0;

	if (dirtiest > 0) {
		nonAggressiveSkip = 4;
	}

	return dirtiest;
}

static void yaffs_BlockBecameDirty(yaffs_Device * dev, int blockNo)
{
	yaffs_BlockInfo *bi = yaffs_GetBlockInfo(dev, blockNo);

	int erasedOk = 0;

	bi->blockState = YAFFS_BLOCK_STATE_DIRTY;

	if (!bi->needsRetiring) {
		yaffs_InvalidateCheckpoint(dev);
		erasedOk = yaffs_EraseBlockInNAND(dev, blockNo);
		if (!erasedOk) {
			dev->nErasureFailures++;
			T(YAFFS_TRACE_ERROR | YAFFS_TRACE_BAD_BLOCKS,
			  (TSTR("**>> Erasure failed %d" TENDSTR), blockNo));
		}
	}

	if (erasedOk && (yaffs_traceMask & YAFFS_TRACE_ERASE)) {
		int i;
		for (i = 0; i < dev->nChunksPerBlock; i++) {
			if (!yaffs_CheckChunkErased
			    (dev, blockNo * dev->nChunksPerBlock + i)) {
				T(YAFFS_TRACE_ERROR,
				  (TSTR
				   (">>Block %d erasure supposedly OK, but chunk %d not erased"
				    TENDSTR), blockNo, i));
			}
		}
	}

	if (erasedOk) {
		
		bi->blockState = YAFFS_BLOCK_STATE_EMPTY;
		dev->nErasedBlocks++;
		bi->pagesInUse = 0;
		bi->softDeletions = 0;
		bi->hasShrinkHeader = 0;
		bi->skipErasedCheck = 1;  
		bi->gcPrioritise = 0;
		yaffs_ClearChunkBits(dev, blockNo);

		T(YAFFS_TRACE_ERASE,
		  (TSTR("Erased block %d" TENDSTR), blockNo));
	} else {
		dev->nFreeChunks -= dev->nChunksPerBlock;	

		yaffs_RetireBlock(dev, blockNo);
		T(YAFFS_TRACE_ERROR | YAFFS_TRACE_BAD_BLOCKS,
		  (TSTR("**>> Block %d retired" TENDSTR), blockNo));
	}
}

static int yaffs_FindBlockForAllocation(yaffs_Device * dev)
{
	int i;

	yaffs_BlockInfo *bi;

	if (dev->nErasedBlocks < 1) {
		T(YAFFS_TRACE_ERROR,
		  (TSTR("yaffs tragedy: no more eraased blocks" TENDSTR)));

		return -1;
	}
	
	

	for (i = dev->internalStartBlock; i <= dev->internalEndBlock; i++) {
		dev->allocationBlockFinder++;
		if (dev->allocationBlockFinder < dev->internalStartBlock
		    || dev->allocationBlockFinder > dev->internalEndBlock) {
			dev->allocationBlockFinder = dev->internalStartBlock;
		}

		bi = yaffs_GetBlockInfo(dev, dev->allocationBlockFinder);

		if (bi->blockState == YAFFS_BLOCK_STATE_EMPTY) {
			bi->blockState = YAFFS_BLOCK_STATE_ALLOCATING;
			dev->sequenceNumber++;
			bi->sequenceNumber = dev->sequenceNumber;
			dev->nErasedBlocks--;
			T(YAFFS_TRACE_ALLOCATE,
			  (TSTR("Allocated block %d, seq  %d, %d left" TENDSTR),
			   dev->allocationBlockFinder, dev->sequenceNumber,
			   dev->nErasedBlocks));
			return dev->allocationBlockFinder;
		}
	}

	T(YAFFS_TRACE_ALWAYS,
	  (TSTR
	   ("yaffs tragedy: no more eraased blocks, but there should have been %d"
	    TENDSTR), dev->nErasedBlocks));

	return -1;
}


static int yaffs_CheckSpaceForAllocation(yaffs_Device * dev)
{
	int reservedChunks;
	int reservedBlocks = dev->nReservedBlocks;
	int checkpointBlocks;
	
	checkpointBlocks =  dev->nCheckpointReservedBlocks - dev->blocksInCheckpoint;
	if(checkpointBlocks < 0)
		checkpointBlocks = 0;
	
	reservedChunks = ((reservedBlocks + checkpointBlocks) * dev->nChunksPerBlock);
	
	return (dev->nFreeChunks > reservedChunks);
}

static int yaffs_AllocateChunk(yaffs_Device * dev, int useReserve, yaffs_BlockInfo **blockUsedPtr)
{
	int retVal;
	yaffs_BlockInfo *bi;

	if (dev->allocationBlock < 0) {
		
		dev->allocationBlock = yaffs_FindBlockForAllocation(dev);
		dev->allocationPage = 0;
	}

	if (!useReserve && !yaffs_CheckSpaceForAllocation(dev)) {
		
		return -1;
	}

	if (dev->nErasedBlocks < dev->nReservedBlocks
	    && dev->allocationPage == 0) {
		T(YAFFS_TRACE_ALLOCATE, (TSTR("Allocating reserve" TENDSTR)));
	}

	
	if (dev->allocationBlock >= 0) {
		bi = yaffs_GetBlockInfo(dev, dev->allocationBlock);

		retVal = (dev->allocationBlock * dev->nChunksPerBlock) +
		    dev->allocationPage;
		bi->pagesInUse++;
		yaffs_SetChunkBit(dev, dev->allocationBlock,
				  dev->allocationPage);

		dev->allocationPage++;

		dev->nFreeChunks--;

		
		if (dev->allocationPage >= dev->nChunksPerBlock) {
			bi->blockState = YAFFS_BLOCK_STATE_FULL;
			dev->allocationBlock = -1;
		}

		if(blockUsedPtr)
			*blockUsedPtr = bi;
			
		return retVal;
	}
	
	T(YAFFS_TRACE_ERROR,
	  (TSTR("!!!!!!!!! Allocator out !!!!!!!!!!!!!!!!!" TENDSTR)));

	return -1;
}

static int yaffs_GetErasedChunks(yaffs_Device * dev)
{
	int n;

	n = dev->nErasedBlocks * dev->nChunksPerBlock;

	if (dev->allocationBlock > 0) {
		n += (dev->nChunksPerBlock - dev->allocationPage);
	}

	return n;

}

static int yaffs_GarbageCollectBlock(yaffs_Device * dev, int block)
{
	int oldChunk;
	int newChunk;
	int chunkInBlock;
	int markNAND;
	int retVal = YAFFS_OK;
	int cleanups = 0;
	int i;
	int isCheckpointBlock;

	int chunksBefore = yaffs_GetErasedChunks(dev);
	int chunksAfter;

	yaffs_ExtendedTags tags;

	yaffs_BlockInfo *bi = yaffs_GetBlockInfo(dev, block);

	yaffs_Object *object;

	isCheckpointBlock = (bi->blockState == YAFFS_BLOCK_STATE_CHECKPOINT);
	
	bi->blockState = YAFFS_BLOCK_STATE_COLLECTING;

	T(YAFFS_TRACE_TRACING,
	  (TSTR("Collecting block %d, in use %d, shrink %d, " TENDSTR), block,
	   bi->pagesInUse, bi->hasShrinkHeader));

	

	bi->hasShrinkHeader = 0;	

	dev->nFreeChunks -= bi->softDeletions;

	dev->isDoingGC = 1;

	if (isCheckpointBlock ||
	    !yaffs_StillSomeChunkBits(dev, block)) {
		T(YAFFS_TRACE_TRACING,
		  (TSTR
		   ("Collecting block %d that has no chunks in use" TENDSTR),
		   block));
		yaffs_BlockBecameDirty(dev, block);
	} else {

		__u8 *buffer = yaffs_GetTempBuffer(dev, __LINE__);

		for (chunkInBlock = 0, oldChunk = block * dev->nChunksPerBlock;
		     chunkInBlock < dev->nChunksPerBlock
		     && yaffs_StillSomeChunkBits(dev, block);
		     chunkInBlock++, oldChunk++) {
			if (yaffs_CheckChunkBit(dev, block, chunkInBlock)) {

				

				markNAND = 1;

				yaffs_InitialiseTags(&tags);

				yaffs_ReadChunkWithTagsFromNAND(dev, oldChunk,
								buffer, &tags);

				object =
				    yaffs_FindObjectByNumber(dev,
							     tags.objectId);

				T(YAFFS_TRACE_GC_DETAIL,
				  (TSTR
				   ("Collecting page %d, %d %d %d " TENDSTR),
				   chunkInBlock, tags.objectId, tags.chunkId,
				   tags.byteCount));

				if (!object) {
					T(YAFFS_TRACE_ERROR,
					  (TSTR
					   ("page %d in gc has no object "
					    TENDSTR), oldChunk));
				}

				if (object && object->deleted
				    && tags.chunkId != 0) {

					object->nDataChunks--;

					if (object->nDataChunks <= 0) {
						
						dev->gcCleanupList[cleanups] =
						    tags.objectId;
						cleanups++;
					}
					markNAND = 0;
				} else if (0
					   
					   ) {
					object->chunkId = 0;
					yaffs_FreeTnode(object->myDev,
							object->variant.
							fileVariant.top);
					object->variant.fileVariant.top = NULL;
					yaffs_DoGenericObjectDeletion(object);

				} else if (object) {
					tags.serialNumber++;

					dev->nGCCopies++;

					if (tags.chunkId == 0) {

						yaffs_ObjectHeader *oh;
						oh = (yaffs_ObjectHeader *)buffer;
						oh->isShrink = 0;
						oh->shadowsObject = -1;
						tags.extraShadows = 0;
						tags.extraIsShrinkHeader = 0;
					}

					newChunk =
					    yaffs_WriteNewChunkWithTagsToNAND(dev, buffer, &tags, 1);

					if (newChunk < 0) {
						retVal = YAFFS_FAIL;
					} else {

						

						if (tags.chunkId == 0) {
							
							object->chunkId =  newChunk;
							object->serial =   tags.serialNumber;
						} else {
							
							yaffs_PutChunkIntoFile
							    (object,
							     tags.chunkId,
							     newChunk, 0);
						}
					}
				}

				yaffs_DeleteChunk(dev, oldChunk, markNAND, __LINE__);

			}
		}

		yaffs_ReleaseTempBuffer(dev, buffer, __LINE__);


		
		for (i = 0; i < cleanups; i++) {
			
			object =
			    yaffs_FindObjectByNumber(dev,
						     dev->gcCleanupList[i]);
			if (object) {
				yaffs_FreeTnode(dev,
						object->variant.fileVariant.
						top);
				object->variant.fileVariant.top = NULL;
				T(YAFFS_TRACE_GC,
				  (TSTR
				   ("yaffs: About to finally delete object %d"
				    TENDSTR), object->objectId));
				yaffs_DoGenericObjectDeletion(object);
				object->myDev->nDeletedFiles--;
			}

		}

	}

	if (chunksBefore >= (chunksAfter = yaffs_GetErasedChunks(dev))) {
		T(YAFFS_TRACE_GC,
		  (TSTR
		   ("gc did not increase free chunks before %d after %d"
		    TENDSTR), chunksBefore, chunksAfter));
	}

	dev->isDoingGC = 0;

	return YAFFS_OK;
}

static int yaffs_CheckGarbageCollection(yaffs_Device * dev)
{
	int block;
	int aggressive;
	int gcOk = YAFFS_OK;
	int maxTries = 0;
	
	int checkpointBlockAdjust;

	if (dev->isDoingGC) {
		
		return YAFFS_OK;
	}
	

	do {
		maxTries++;
		
		checkpointBlockAdjust = (dev->nCheckpointReservedBlocks - dev->blocksInCheckpoint);
		if(checkpointBlockAdjust < 0)
			checkpointBlockAdjust = 0;

		if (dev->nErasedBlocks < (dev->nReservedBlocks + checkpointBlockAdjust)) {
			
			aggressive = 1;
		} else {
			
			aggressive = 0;
		}

		block = yaffs_FindBlockForGarbageCollection(dev, aggressive);

		if (block > 0) {
			dev->garbageCollections++;
			if (!aggressive) {
				dev->passiveGarbageCollections++;
			}

			T(YAFFS_TRACE_GC,
			  (TSTR
			   ("yaffs: GC erasedBlocks %d aggressive %d" TENDSTR),
			   dev->nErasedBlocks, aggressive));

			gcOk = yaffs_GarbageCollectBlock(dev, block);
		}

		if (dev->nErasedBlocks < (dev->nReservedBlocks) && block > 0) {
			T(YAFFS_TRACE_GC,
			  (TSTR
			   ("yaffs: GC !!!no reclaim!!! erasedBlocks %d after try %d block %d"
			    TENDSTR), dev->nErasedBlocks, maxTries, block));
		}
	} while ((dev->nErasedBlocks < dev->nReservedBlocks) && (block > 0)
		 && (maxTries < 2));

	return aggressive ? gcOk : YAFFS_OK;
}


static int yaffs_TagsMatch(const yaffs_ExtendedTags * tags, int objectId,
			   int chunkInObject)
{
	return (tags->chunkId == chunkInObject &&
		tags->objectId == objectId && !tags->chunkDeleted) ? 1 : 0;

}



static int yaffs_FindChunkInFile(yaffs_Object * in, int chunkInInode,
				 yaffs_ExtendedTags * tags)
{
	
	yaffs_Tnode *tn;
	int theChunk = -1;
	yaffs_ExtendedTags localTags;
	int retVal = -1;

	yaffs_Device *dev = in->myDev;

	if (!tags) {
		
		tags = &localTags;
	}

	tn = yaffs_FindLevel0Tnode(dev, &in->variant.fileVariant, chunkInInode);

	if (tn) {
		theChunk = yaffs_GetChunkGroupBase(dev,tn,chunkInInode);

		retVal =
		    yaffs_FindChunkInGroup(dev, theChunk, tags, in->objectId,
					   chunkInInode);
	}
	return retVal;
}

static int yaffs_FindAndDeleteChunkInFile(yaffs_Object * in, int chunkInInode,
					  yaffs_ExtendedTags * tags)
{
	
	yaffs_Tnode *tn;
	int theChunk = -1;
	yaffs_ExtendedTags localTags;

	yaffs_Device *dev = in->myDev;
	int retVal = -1;

	if (!tags) {
		
		tags = &localTags;
	}

	tn = yaffs_FindLevel0Tnode(dev, &in->variant.fileVariant, chunkInInode);

	if (tn) {

		theChunk = yaffs_GetChunkGroupBase(dev,tn,chunkInInode);

		retVal =
		    yaffs_FindChunkInGroup(dev, theChunk, tags, in->objectId,
					   chunkInInode);

		
		if (retVal != -1) {
			yaffs_PutLevel0Tnode(dev,tn,chunkInInode,0);
		}
	} else {
		
	}

	if (retVal == -1) {
		
	}
	return retVal;
}

#ifdef YAFFS_PARANOID

static int yaffs_CheckFileSanity(yaffs_Object * in)
{
	int chunk;
	int nChunks;
	int fSize;
	int failed = 0;
	int objId;
	yaffs_Tnode *tn;
	yaffs_Tags localTags;
	yaffs_Tags *tags = &localTags;
	int theChunk;
	int chunkDeleted;

	if (in->variantType != YAFFS_OBJECT_TYPE_FILE) {
		
		return YAFFS_FAIL;
	}

	objId = in->objectId;
	fSize = in->variant.fileVariant.fileSize;
	nChunks =
	    (fSize + in->myDev->nDataBytesPerChunk - 1) / in->myDev->nDataBytesPerChunk;

	for (chunk = 1; chunk <= nChunks; chunk++) {
		tn = yaffs_FindLevel0Tnode(in->myDev, &in->variant.fileVariant,
					   chunk);

		if (tn) {

			theChunk = yaffs_GetChunkGroupBase(dev,tn,chunk);

			if (yaffs_CheckChunkBits
			    (dev, theChunk / dev->nChunksPerBlock,
			     theChunk % dev->nChunksPerBlock)) {

				yaffs_ReadChunkTagsFromNAND(in->myDev, theChunk,
							    tags,
							    &chunkDeleted);
				if (yaffs_TagsMatch
				    (tags, in->objectId, chunk, chunkDeleted)) {
					

				}
			} else {

				failed = 1;
			}

		} else {
			
		}
	}

	return failed ? YAFFS_FAIL : YAFFS_OK;
}

#endif

static int yaffs_PutChunkIntoFile(yaffs_Object * in, int chunkInInode,
				  int chunkInNAND, int inScan)
{
	 
	yaffs_Tnode *tn;
	yaffs_Device *dev = in->myDev;
	int existingChunk;
	yaffs_ExtendedTags existingTags;
	yaffs_ExtendedTags newTags;
	unsigned existingSerial, newSerial;

	if (in->variantType != YAFFS_OBJECT_TYPE_FILE) {
		if (!inScan) {
			T(YAFFS_TRACE_ERROR,
			  (TSTR
			   ("yaffs tragedy:attempt to put data chunk into a non-file"
			    TENDSTR)));
			YBUG();
		}

		yaffs_DeleteChunk(dev, chunkInNAND, 1, __LINE__);
		return YAFFS_OK;
	}

	tn = yaffs_AddOrFindLevel0Tnode(dev, 
					&in->variant.fileVariant,
					chunkInInode,
					NULL);
	if (!tn) {
		return YAFFS_FAIL;
	}

	existingChunk = yaffs_GetChunkGroupBase(dev,tn,chunkInInode);

	if (inScan != 0) {

		if (existingChunk != 0) {

			if (inScan > 0) {
				
				yaffs_ReadChunkWithTagsFromNAND(dev,
								chunkInNAND,
								NULL, &newTags);

				
				existingChunk =
				    yaffs_FindChunkInFile(in, chunkInInode,
							  &existingTags);
			}

			if (existingChunk <= 0) {
				

				T(YAFFS_TRACE_ERROR,
				  (TSTR
				   ("yaffs tragedy: existing chunk < 0 in scan"
				    TENDSTR)));

			}


			newSerial = newTags.serialNumber;
			existingSerial = existingTags.serialNumber;

			if ((inScan > 0) &&
			    (in->myDev->isYaffs2 ||
			     existingChunk <= 0 ||
			     ((existingSerial + 1) & 3) == newSerial)) {
				yaffs_DeleteChunk(dev, existingChunk, 1,
						  __LINE__);
			} else {
				yaffs_DeleteChunk(dev, chunkInNAND, 1,
						  __LINE__);
				return YAFFS_OK;
			}
		}

	}

	if (existingChunk == 0) {
		in->nDataChunks++;
	}

	yaffs_PutLevel0Tnode(dev,tn,chunkInInode,chunkInNAND);

	return YAFFS_OK;
}

static int yaffs_ReadChunkDataFromObject(yaffs_Object * in, int chunkInInode,
					 __u8 * buffer)
{
	int chunkInNAND = yaffs_FindChunkInFile(in, chunkInInode, NULL);

	if (chunkInNAND >= 0) {
		return yaffs_ReadChunkWithTagsFromNAND(in->myDev, chunkInNAND,
						       buffer,NULL);
	} else {
		T(YAFFS_TRACE_NANDACCESS,
		  (TSTR("Chunk %d not found zero instead" TENDSTR),
		   chunkInNAND));
		
		memset(buffer, 0, in->myDev->nDataBytesPerChunk);	
		return 0;
	}

}

void yaffs_DeleteChunk(yaffs_Device * dev, int chunkId, int markNAND, int lyn)
{
	int block;
	int page;
	yaffs_ExtendedTags tags;
	yaffs_BlockInfo *bi;

	if (chunkId <= 0)
		return;

	dev->nDeletions++;
	block = chunkId / dev->nChunksPerBlock;
	page = chunkId % dev->nChunksPerBlock;

	bi = yaffs_GetBlockInfo(dev, block);

	T(YAFFS_TRACE_DELETION,
	  (TSTR("line %d delete of chunk %d" TENDSTR), lyn, chunkId));

	if (markNAND &&
	    bi->blockState != YAFFS_BLOCK_STATE_COLLECTING && !dev->isYaffs2) {

		yaffs_InitialiseTags(&tags);

		tags.chunkDeleted = 1;

		yaffs_WriteChunkWithTagsToNAND(dev, chunkId, NULL, &tags);
		yaffs_HandleUpdateChunk(dev, chunkId, &tags);
	} else {
		dev->nUnmarkedDeletions++;
	}

	if (bi->blockState == YAFFS_BLOCK_STATE_ALLOCATING ||
	    bi->blockState == YAFFS_BLOCK_STATE_FULL ||
	    bi->blockState == YAFFS_BLOCK_STATE_NEEDS_SCANNING ||
	    bi->blockState == YAFFS_BLOCK_STATE_COLLECTING) {
		dev->nFreeChunks++;

		yaffs_ClearChunkBit(dev, block, page);

		bi->pagesInUse--;

		if (bi->pagesInUse == 0 &&
		    !bi->hasShrinkHeader &&
		    bi->blockState != YAFFS_BLOCK_STATE_ALLOCATING &&
		    bi->blockState != YAFFS_BLOCK_STATE_NEEDS_SCANNING) {
			yaffs_BlockBecameDirty(dev, block);
		}

	} else {
		
	}

}

static int yaffs_WriteChunkDataToObject(yaffs_Object * in, int chunkInInode,
					const __u8 * buffer, int nBytes,
					int useReserve)
{

	int prevChunkId;
	yaffs_ExtendedTags prevTags;

	int newChunkId;
	yaffs_ExtendedTags newTags;

	yaffs_Device *dev = in->myDev;

	yaffs_CheckGarbageCollection(dev);

	
	prevChunkId = yaffs_FindChunkInFile(in, chunkInInode, &prevTags);

	
	yaffs_InitialiseTags(&newTags);

	newTags.chunkId = chunkInInode;
	newTags.objectId = in->objectId;
	newTags.serialNumber =
	    (prevChunkId >= 0) ? prevTags.serialNumber + 1 : 1;
	newTags.byteCount = nBytes;

	newChunkId =
	    yaffs_WriteNewChunkWithTagsToNAND(dev, buffer, &newTags,
					      useReserve);

	if (newChunkId >= 0) {
		yaffs_PutChunkIntoFile(in, chunkInInode, newChunkId, 0);

		if (prevChunkId >= 0) {
			yaffs_DeleteChunk(dev, prevChunkId, 1, __LINE__);

		}

		yaffs_CheckFileSanity(in);
	}
	return newChunkId;

}

int yaffs_UpdateObjectHeader(yaffs_Object * in, const YCHAR * name, int force,
			     int isShrink, int shadows)
{

	yaffs_BlockInfo *bi;

	yaffs_Device *dev = in->myDev;

	int prevChunkId;
	int retVal = 0;
	int result = 0;

	int newChunkId;
	yaffs_ExtendedTags newTags;

	__u8 *buffer = NULL;
	YCHAR oldName[YAFFS_MAX_NAME_LENGTH + 1];

	yaffs_ObjectHeader *oh = NULL;

	if (!in->fake || force) {

		yaffs_CheckGarbageCollection(dev);

		buffer = yaffs_GetTempBuffer(in->myDev, __LINE__);
		oh = (yaffs_ObjectHeader *) buffer;

		prevChunkId = in->chunkId;

		if (prevChunkId >= 0) {
			result = yaffs_ReadChunkWithTagsFromNAND(dev, prevChunkId,
							buffer, NULL);
			memcpy(oldName, oh->name, sizeof(oh->name));
		}

		memset(buffer, 0xFF, dev->nDataBytesPerChunk);

		oh->type = in->variantType;
		oh->yst_mode = in->yst_mode;
		oh->shadowsObject = shadows;

#ifdef CONFIG_YAFFS_WINCE
		oh->win_atime[0] = in->win_atime[0];
		oh->win_ctime[0] = in->win_ctime[0];
		oh->win_mtime[0] = in->win_mtime[0];
		oh->win_atime[1] = in->win_atime[1];
		oh->win_ctime[1] = in->win_ctime[1];
		oh->win_mtime[1] = in->win_mtime[1];
#else
		oh->yst_uid = in->yst_uid;
		oh->yst_gid = in->yst_gid;
		oh->yst_atime = in->yst_atime;
		oh->yst_mtime = in->yst_mtime;
		oh->yst_ctime = in->yst_ctime;
		oh->yst_rdev = in->yst_rdev;
#endif
		if (in->parent) {
			oh->parentObjectId = in->parent->objectId;
		} else {
			oh->parentObjectId = 0;
		}

		if (name && *name) {
			memset(oh->name, 0, sizeof(oh->name));
			yaffs_strncpy(oh->name, name, YAFFS_MAX_NAME_LENGTH);
		} else if (prevChunkId) {
			memcpy(oh->name, oldName, sizeof(oh->name));
		} else {
			memset(oh->name, 0, sizeof(oh->name));
		}

		oh->isShrink = isShrink;

		switch (in->variantType) {
		case YAFFS_OBJECT_TYPE_UNKNOWN:
			
			break;
		case YAFFS_OBJECT_TYPE_FILE:
			oh->fileSize =
			    (oh->parentObjectId == YAFFS_OBJECTID_DELETED
			     || oh->parentObjectId ==
			     YAFFS_OBJECTID_UNLINKED) ? 0 : in->variant.
			    fileVariant.fileSize;
			break;
		case YAFFS_OBJECT_TYPE_HARDLINK:
			oh->equivalentObjectId =
			    in->variant.hardLinkVariant.equivalentObjectId;
			break;
		case YAFFS_OBJECT_TYPE_SPECIAL:
			
			break;
		case YAFFS_OBJECT_TYPE_DIRECTORY:
			
			break;
		case YAFFS_OBJECT_TYPE_SYMLINK:
			yaffs_strncpy(oh->alias,
				      in->variant.symLinkVariant.alias,
				      YAFFS_MAX_ALIAS_LENGTH);
			oh->alias[YAFFS_MAX_ALIAS_LENGTH] = 0;
			break;
		}

		
		yaffs_InitialiseTags(&newTags);
		in->serial++;
		newTags.chunkId = 0;
		newTags.objectId = in->objectId;
		newTags.serialNumber = in->serial;

		

		newTags.extraHeaderInfoAvailable = 1;
		newTags.extraParentObjectId = oh->parentObjectId;
		newTags.extraFileLength = oh->fileSize;
		newTags.extraIsShrinkHeader = oh->isShrink;
		newTags.extraEquivalentObjectId = oh->equivalentObjectId;
		newTags.extraShadows = (oh->shadowsObject > 0) ? 1 : 0;
		newTags.extraObjectType = in->variantType;

		
		newChunkId =
		    yaffs_WriteNewChunkWithTagsToNAND(dev, buffer, &newTags,
						      (prevChunkId >= 0) ? 1 : 0);

		if (newChunkId >= 0) {

			in->chunkId = newChunkId;

			if (prevChunkId >= 0) {
				yaffs_DeleteChunk(dev, prevChunkId, 1,
						  __LINE__);
			}

			if(!yaffs_ObjectHasCachedWriteData(in))
				in->dirty = 0;

			
			if (isShrink) {
				bi = yaffs_GetBlockInfo(in->myDev,
							newChunkId /in->myDev->	nChunksPerBlock);
				bi->hasShrinkHeader = 1;
			}

		}

		retVal = newChunkId;

	}

	if (buffer)
		yaffs_ReleaseTempBuffer(dev, buffer, __LINE__);

	return retVal;
}


static int yaffs_ObjectHasCachedWriteData(yaffs_Object *obj)
{
	yaffs_Device *dev = obj->myDev;
	int i;
	yaffs_ChunkCache *cache;
	int nCaches = obj->myDev->nShortOpCaches;
	
	for(i = 0; i < nCaches; i++){
		cache = &dev->srCache[i];
		if (cache->object == obj &&
		    cache->dirty)
			return 1;
	}
	
	return 0;
}


static void yaffs_FlushFilesChunkCache(yaffs_Object * obj)
{
	yaffs_Device *dev = obj->myDev;
	int lowest = -99;	
	int i;
	yaffs_ChunkCache *cache;
	int chunkWritten = 0;
	int nCaches = obj->myDev->nShortOpCaches;

	if (nCaches > 0) {
		do {
			cache = NULL;

			
			for (i = 0; i < nCaches; i++) {
				if (dev->srCache[i].object == obj &&
				    dev->srCache[i].dirty) {
					if (!cache
					    || dev->srCache[i].chunkId <
					    lowest) {
						cache = &dev->srCache[i];
						lowest = cache->chunkId;
					}
				}
			}

			if (cache && !cache->locked) {
				

				chunkWritten =
				    yaffs_WriteChunkDataToObject(cache->object,
								 cache->chunkId,
								 cache->data,
								 cache->nBytes,
								 1);
				cache->dirty = 0;
				cache->object = NULL;
			}

		} while (cache && chunkWritten > 0);

		if (cache) {
			
			T(YAFFS_TRACE_ERROR,
			  (TSTR("yaffs tragedy: no space during cache write" TENDSTR)));

		}
	}

}


void yaffs_FlushEntireDeviceCache(yaffs_Device *dev)
{
	yaffs_Object *obj;
	int nCaches = dev->nShortOpCaches;
	int i;
	
	do {
		obj = NULL;
		for( i = 0; i < nCaches && !obj; i++) {
			if (dev->srCache[i].object &&
			    dev->srCache[i].dirty)
				obj = dev->srCache[i].object;
			    
		}
		if(obj)
			yaffs_FlushFilesChunkCache(obj);
			
	} while(obj);
	
}


static yaffs_ChunkCache *yaffs_GrabChunkCacheWorker(yaffs_Device * dev)
{
	int i;
	int usage;
	int theOne;

	if (dev->nShortOpCaches > 0) {
		for (i = 0; i < dev->nShortOpCaches; i++) {
			if (!dev->srCache[i].object) 
				return &dev->srCache[i];
		}

		return NULL;

		theOne = -1;
		usage = 0;	

		for (i = 0; i < dev->nShortOpCaches; i++) {
			if (!dev->srCache[i].dirty &&
			    ((dev->srCache[i].lastUse < usage && theOne >= 0) ||
			     theOne < 0)) {
				usage = dev->srCache[i].lastUse;
				theOne = i;
			}
		}


		return theOne >= 0 ? &dev->srCache[theOne] : NULL;
	} else {
		return NULL;
	}

}

static yaffs_ChunkCache *yaffs_GrabChunkCache(yaffs_Device * dev)
{
	yaffs_ChunkCache *cache;
	yaffs_Object *theObj;
	int usage;
	int i;
	int pushout;

	if (dev->nShortOpCaches > 0) {
		

		cache = yaffs_GrabChunkCacheWorker(dev);

		if (!cache) {

			

			theObj = NULL;
			usage = -1;
			cache = NULL;
			pushout = -1;

			for (i = 0; i < dev->nShortOpCaches; i++) {
				if (dev->srCache[i].object &&
				    !dev->srCache[i].locked &&
				    (dev->srCache[i].lastUse < usage || !cache))
				{
					usage = dev->srCache[i].lastUse;
					theObj = dev->srCache[i].object;
					cache = &dev->srCache[i];
					pushout = i;
				}
			}

			if (!cache || cache->dirty) {
				
				yaffs_FlushFilesChunkCache(theObj);
				cache = yaffs_GrabChunkCacheWorker(dev);
			}

		}
		return cache;
	} else
		return NULL;

}

static yaffs_ChunkCache *yaffs_FindChunkCache(const yaffs_Object * obj,
					      int chunkId)
{
	yaffs_Device *dev = obj->myDev;
	int i;
	if (dev->nShortOpCaches > 0) {
		for (i = 0; i < dev->nShortOpCaches; i++) {
			if (dev->srCache[i].object == obj &&
			    dev->srCache[i].chunkId == chunkId) {
				dev->cacheHits++;

				return &dev->srCache[i];
			}
		}
	}
	return NULL;
}

static void yaffs_UseChunkCache(yaffs_Device * dev, yaffs_ChunkCache * cache,
				int isAWrite)
{

	if (dev->nShortOpCaches > 0) {
		if (dev->srLastUse < 0 || dev->srLastUse > 100000000) {
			
			int i;
			for (i = 1; i < dev->nShortOpCaches; i++) {
				dev->srCache[i].lastUse = 0;
			}
			dev->srLastUse = 0;
		}

		dev->srLastUse++;

		cache->lastUse = dev->srLastUse;

		if (isAWrite) {
			cache->dirty = 1;
		}
	}
}

/* Invalidate a single cache page.
 * Do this when a whole page gets written,
 * ie the short cache for this page is no longer valid.
 */
static void yaffs_InvalidateChunkCache(yaffs_Object * object, int chunkId)
{
	if (object->myDev->nShortOpCaches > 0) {
		yaffs_ChunkCache *cache = yaffs_FindChunkCache(object, chunkId);

		if (cache) {
			cache->object = NULL;
		}
	}
}

static void yaffs_InvalidateWholeChunkCache(yaffs_Object * in)
{
	int i;
	yaffs_Device *dev = in->myDev;

	if (dev->nShortOpCaches > 0) {
		
		for (i = 0; i < dev->nShortOpCaches; i++) {
			if (dev->srCache[i].object == in) {
				dev->srCache[i].object = NULL;
			}
		}
	}
}



static int yaffs_WriteCheckpointValidityMarker(yaffs_Device *dev,int head)
{
	yaffs_CheckpointValidity cp;
	cp.structType = sizeof(cp);
	cp.magic = YAFFS_MAGIC;
	cp.version = YAFFS_CHECKPOINT_VERSION;
	cp.head = (head) ? 1 : 0;
	
	return (yaffs_CheckpointWrite(dev,&cp,sizeof(cp)) == sizeof(cp))?
		1 : 0;
}

static int yaffs_ReadCheckpointValidityMarker(yaffs_Device *dev, int head)
{
	yaffs_CheckpointValidity cp;
	int ok;
	
	ok = (yaffs_CheckpointRead(dev,&cp,sizeof(cp)) == sizeof(cp));
	
	if(ok)
		ok = (cp.structType == sizeof(cp)) &&
		     (cp.magic == YAFFS_MAGIC) &&
		     (cp.version == YAFFS_CHECKPOINT_VERSION) &&
		     (cp.head == ((head) ? 1 : 0));
	return ok ? 1 : 0;
}

static void yaffs_DeviceToCheckpointDevice(yaffs_CheckpointDevice *cp, 
					   yaffs_Device *dev)
{
	cp->nErasedBlocks = dev->nErasedBlocks;
	cp->allocationBlock = dev->allocationBlock;
	cp->allocationPage = dev->allocationPage;
	cp->nFreeChunks = dev->nFreeChunks;
	
	cp->nDeletedFiles = dev->nDeletedFiles;
	cp->nUnlinkedFiles = dev->nUnlinkedFiles;
	cp->nBackgroundDeletions = dev->nBackgroundDeletions;
	cp->sequenceNumber = dev->sequenceNumber;
	cp->oldestDirtySequence = dev->oldestDirtySequence;
	
}

static void yaffs_CheckpointDeviceToDevice(yaffs_Device *dev,
					   yaffs_CheckpointDevice *cp)
{
	dev->nErasedBlocks = cp->nErasedBlocks;
	dev->allocationBlock = cp->allocationBlock;
	dev->allocationPage = cp->allocationPage;
	dev->nFreeChunks = cp->nFreeChunks;
	
	dev->nDeletedFiles = cp->nDeletedFiles;
	dev->nUnlinkedFiles = cp->nUnlinkedFiles;
	dev->nBackgroundDeletions = cp->nBackgroundDeletions;
	dev->sequenceNumber = cp->sequenceNumber;
	dev->oldestDirtySequence = cp->oldestDirtySequence;
}


static int yaffs_WriteCheckpointDevice(yaffs_Device *dev)
{
	yaffs_CheckpointDevice cp;
	__u32 nBytes;
	__u32 nBlocks = (dev->internalEndBlock - dev->internalStartBlock + 1);

	int ok;
		
	
	yaffs_DeviceToCheckpointDevice(&cp,dev);
	cp.structType = sizeof(cp);
	
	ok = (yaffs_CheckpointWrite(dev,&cp,sizeof(cp)) == sizeof(cp));
	
	
	if(ok) {
		nBytes = nBlocks * sizeof(yaffs_BlockInfo);
		ok = (yaffs_CheckpointWrite(dev,dev->blockInfo,nBytes) == nBytes);
	}
		
			
	if(ok) {
		nBytes = nBlocks * dev->chunkBitmapStride;
		ok = (yaffs_CheckpointWrite(dev,dev->chunkBits,nBytes) == nBytes);
	}
	return	 ok ? 1 : 0;

}

static int yaffs_ReadCheckpointDevice(yaffs_Device *dev)
{
	yaffs_CheckpointDevice cp;
	__u32 nBytes;
	__u32 nBlocks = (dev->internalEndBlock - dev->internalStartBlock + 1);

	int ok;	
	
	ok = (yaffs_CheckpointRead(dev,&cp,sizeof(cp)) == sizeof(cp));
	if(!ok)
		return 0;
		
	if(cp.structType != sizeof(cp))
		return 0;
		
	
	yaffs_CheckpointDeviceToDevice(dev,&cp);
	
	nBytes = nBlocks * sizeof(yaffs_BlockInfo);
	
	ok = (yaffs_CheckpointRead(dev,dev->blockInfo,nBytes) == nBytes);
	
	if(!ok)
		return 0;
	nBytes = nBlocks * dev->chunkBitmapStride;
	
	ok = (yaffs_CheckpointRead(dev,dev->chunkBits,nBytes) == nBytes);
	
	return ok ? 1 : 0;
}

static void yaffs_ObjectToCheckpointObject(yaffs_CheckpointObject *cp,
					   yaffs_Object *obj)
{

	cp->objectId = obj->objectId;
	cp->parentId = (obj->parent) ? obj->parent->objectId : 0;
	cp->chunkId = obj->chunkId;
	cp->variantType = obj->variantType;			
	cp->deleted = obj->deleted;
	cp->softDeleted = obj->softDeleted;
	cp->unlinked = obj->unlinked;
	cp->fake = obj->fake;
	cp->renameAllowed = obj->renameAllowed;
	cp->unlinkAllowed = obj->unlinkAllowed;
	cp->serial = obj->serial;
	cp->nDataChunks = obj->nDataChunks;
	
	if(obj->variantType == YAFFS_OBJECT_TYPE_FILE)
		cp->fileSizeOrEquivalentObjectId = obj->variant.fileVariant.fileSize;
	else if(obj->variantType == YAFFS_OBJECT_TYPE_HARDLINK)
		cp->fileSizeOrEquivalentObjectId = obj->variant.hardLinkVariant.equivalentObjectId;
}

static void yaffs_CheckpointObjectToObject( yaffs_Object *obj,yaffs_CheckpointObject *cp)
{

	yaffs_Object *parent;
	
	obj->objectId = cp->objectId;
	
	if(cp->parentId)
		parent = yaffs_FindOrCreateObjectByNumber(
					obj->myDev,
					cp->parentId,
					YAFFS_OBJECT_TYPE_DIRECTORY);
	else
		parent = NULL;
		
	if(parent)
		yaffs_AddObjectToDirectory(parent, obj);
		
	obj->chunkId = cp->chunkId;
	obj->variantType = cp->variantType;			
	obj->deleted = cp->deleted;
	obj->softDeleted = cp->softDeleted;
	obj->unlinked = cp->unlinked;
	obj->fake = cp->fake;
	obj->renameAllowed = cp->renameAllowed;
	obj->unlinkAllowed = cp->unlinkAllowed;
	obj->serial = cp->serial;
	obj->nDataChunks = cp->nDataChunks;
	
	if(obj->variantType == YAFFS_OBJECT_TYPE_FILE)
		obj->variant.fileVariant.fileSize = cp->fileSizeOrEquivalentObjectId;
	else if(obj->variantType == YAFFS_OBJECT_TYPE_HARDLINK)
		obj->variant.hardLinkVariant.equivalentObjectId = cp->fileSizeOrEquivalentObjectId;
		
	if(obj->objectId >= YAFFS_NOBJECT_BUCKETS)
		obj->lazyLoaded = 1;
}



static int yaffs_CheckpointTnodeWorker(yaffs_Object * in, yaffs_Tnode * tn,
				  	__u32 level, int chunkOffset)
{
	int i;
	yaffs_Device *dev = in->myDev;
	int ok = 1;
	int nTnodeBytes = (dev->tnodeWidth * YAFFS_NTNODES_LEVEL0)/8;

	if (tn) {
		if (level > 0) {

			for (i = 0; i < YAFFS_NTNODES_INTERNAL && ok; i++){
				if (tn->internal[i]) {
					ok = yaffs_CheckpointTnodeWorker(in,
							tn->internal[i],
							level - 1,
							(chunkOffset<<YAFFS_TNODES_INTERNAL_BITS) + i);
				}
			}
		} else if (level == 0) {
			__u32 baseOffset = chunkOffset <<  YAFFS_TNODES_LEVEL0_BITS;
			
			ok = (yaffs_CheckpointWrite(dev,&baseOffset,sizeof(baseOffset)) == sizeof(baseOffset));
			if(ok)
				ok = (yaffs_CheckpointWrite(dev,tn,nTnodeBytes) == nTnodeBytes);
		}
	}

	return ok;

}

static int yaffs_WriteCheckpointTnodes(yaffs_Object *obj)
{
	__u32 endMarker = ~0;
	int ok = 1;
	
	if(obj->variantType == YAFFS_OBJECT_TYPE_FILE){
		ok = yaffs_CheckpointTnodeWorker(obj,
					    obj->variant.fileVariant.top,
					    obj->variant.fileVariant.topLevel,
					    0);
		if(ok)
			ok = (yaffs_CheckpointWrite(obj->myDev,&endMarker,sizeof(endMarker)) == 
				sizeof(endMarker));
	}
	
	return ok ? 1 : 0;
}

static int yaffs_ReadCheckpointTnodes(yaffs_Object *obj)
{
	__u32 baseChunk;
	int ok = 1;
	yaffs_Device *dev = obj->myDev;
	yaffs_FileStructure *fileStructPtr = &obj->variant.fileVariant;
	yaffs_Tnode *tn;
	
	ok = (yaffs_CheckpointRead(dev,&baseChunk,sizeof(baseChunk)) == sizeof(baseChunk));
	
	while(ok && (~baseChunk)){
		
		
		
		tn = yaffs_GetTnodeRaw(dev);
		if(tn)
			ok = (yaffs_CheckpointRead(dev,tn,(dev->tnodeWidth * YAFFS_NTNODES_LEVEL0)/8) ==
			      (dev->tnodeWidth * YAFFS_NTNODES_LEVEL0)/8);
		else
			ok = 0;
			
		if(tn && ok){
			ok = yaffs_AddOrFindLevel0Tnode(dev,
					       		fileStructPtr,
					       		baseChunk,
					       		tn) ? 1 : 0;
		}
			
		if(ok)
			ok = (yaffs_CheckpointRead(dev,&baseChunk,sizeof(baseChunk)) == sizeof(baseChunk));
		
	}

	return ok ? 1 : 0;	
}
 

static int yaffs_WriteCheckpointObjects(yaffs_Device *dev)
{
	yaffs_Object *obj;
	yaffs_CheckpointObject cp;
	int i;
	int ok = 1;
	struct list_head *lh;

	
	 
	 for(i = 0; ok &&  i <  YAFFS_NOBJECT_BUCKETS; i++){
	 	list_for_each(lh, &dev->objectBucket[i].list) {
			if (lh) {
				obj = list_entry(lh, yaffs_Object, hashLink);
				if (!obj->deferedFree) {
					yaffs_ObjectToCheckpointObject(&cp,obj);
					cp.structType = sizeof(cp);
					
					ok = (yaffs_CheckpointWrite(dev,&cp,sizeof(cp)) == sizeof(cp));
					
					if(ok && obj->variantType == YAFFS_OBJECT_TYPE_FILE){
						ok = yaffs_WriteCheckpointTnodes(obj);
					}
				}
			}
		}
	 }
	 
	 
	memset(&cp,0xFF,sizeof(yaffs_CheckpointObject));
	cp.structType = sizeof(cp);
	
	if(ok)
		ok = (yaffs_CheckpointWrite(dev,&cp,sizeof(cp)) == sizeof(cp));
		
	return ok ? 1 : 0;
}

static int yaffs_ReadCheckpointObjects(yaffs_Device *dev)
{
	yaffs_Object *obj;
	yaffs_CheckpointObject cp;
	int ok = 1;
	int done = 0;
	yaffs_Object *hardList = NULL;
	
	while(ok && !done) {
		ok = (yaffs_CheckpointRead(dev,&cp,sizeof(cp)) == sizeof(cp));
		if(cp.structType != sizeof(cp)) {
			
			ok = 0;
		}
			
		if(ok && cp.objectId == ~0)
			done = 1;
		else if(ok){
			T(YAFFS_TRACE_CHECKPOINT,(TSTR("Read object %d parent %d type %d" TENDSTR),
				cp.objectId,cp.parentId,cp.variantType));
			obj = yaffs_FindOrCreateObjectByNumber(dev,cp.objectId, cp.variantType);
			if(obj) {
				yaffs_CheckpointObjectToObject(obj,&cp);
				if(obj->variantType == YAFFS_OBJECT_TYPE_FILE) {
					ok = yaffs_ReadCheckpointTnodes(obj);
				} else if(obj->variantType == YAFFS_OBJECT_TYPE_HARDLINK) {
					obj->hardLinks.next =
						    (struct list_head *)
						    hardList;
					hardList = obj;
				}
			   
			}
		}
	}
	
	if(ok)
		yaffs_HardlinkFixup(dev,hardList);
	
	return ok ? 1 : 0;
}

static int yaffs_WriteCheckpointData(yaffs_Device *dev)
{

	int ok;
	
	ok = yaffs_CheckpointOpen(dev,1);
	
	if(ok)
		ok = yaffs_WriteCheckpointValidityMarker(dev,1);
	if(ok)
		ok = yaffs_WriteCheckpointDevice(dev);
	if(ok)
		ok = yaffs_WriteCheckpointObjects(dev);
	if(ok)
		ok = yaffs_WriteCheckpointValidityMarker(dev,0);
		
	if(!yaffs_CheckpointClose(dev))
		 ok = 0;
		 
	if(ok)
	    	dev->isCheckpointed = 1;
	 else 
	 	dev->isCheckpointed = 0;

	return dev->isCheckpointed;
}

static int yaffs_ReadCheckpointData(yaffs_Device *dev)
{
	int ok;
	
	ok = yaffs_CheckpointOpen(dev,0); 
	
	if(ok)
		ok = yaffs_ReadCheckpointValidityMarker(dev,1);
	if(ok)
		ok = yaffs_ReadCheckpointDevice(dev);
	if(ok)
		ok = yaffs_ReadCheckpointObjects(dev);
	if(ok)
		ok = yaffs_ReadCheckpointValidityMarker(dev,0);
		


	if(!yaffs_CheckpointClose(dev))
		ok = 0;

	if(ok)
	    	dev->isCheckpointed = 1;
	 else 
	 	dev->isCheckpointed = 0;

	return ok ? 1 : 0;

}

static void yaffs_InvalidateCheckpoint(yaffs_Device *dev)
{
	if(dev->isCheckpointed || 
	   dev->blocksInCheckpoint > 0){
		dev->isCheckpointed = 0;
		yaffs_CheckpointInvalidateStream(dev);
		if(dev->superBlock && dev->markSuperBlockDirty)
			dev->markSuperBlockDirty(dev->superBlock);
	}
}


int yaffs_CheckpointSave(yaffs_Device *dev)
{
	T(YAFFS_TRACE_CHECKPOINT,(TSTR("save entry: isCheckpointed %d"TENDSTR),dev->isCheckpointed));

	if(!dev->isCheckpointed)
		yaffs_WriteCheckpointData(dev);
	
	T(YAFFS_TRACE_CHECKPOINT,(TSTR("save exit: isCheckpointed %d"TENDSTR),dev->isCheckpointed));

	return dev->isCheckpointed;
}

int yaffs_CheckpointRestore(yaffs_Device *dev)
{
	int retval;
	T(YAFFS_TRACE_CHECKPOINT,(TSTR("restore entry: isCheckpointed %d"TENDSTR),dev->isCheckpointed));
	
	retval = yaffs_ReadCheckpointData(dev);

	T(YAFFS_TRACE_CHECKPOINT,(TSTR("restore exit: isCheckpointed %d"TENDSTR),dev->isCheckpointed));
	
	return retval;
}


int yaffs_ReadDataFromFile(yaffs_Object * in, __u8 * buffer, loff_t offset,
			   int nBytes)
{

	int chunk;
	int start;
	int nToCopy;
	int n = nBytes;
	int nDone = 0;
	yaffs_ChunkCache *cache;

	yaffs_Device *dev;

	dev = in->myDev;

	while (n > 0) {
		
		
		yaffs_AddrToChunk(dev,offset,&chunk,&start);
		chunk++;

		if ((start + n) < dev->nDataBytesPerChunk) {
			nToCopy = n;
		} else {
			nToCopy = dev->nDataBytesPerChunk - start;
		}

		cache = yaffs_FindChunkCache(in, chunk);

		if (cache || nToCopy != dev->nDataBytesPerChunk) {
			if (dev->nShortOpCaches > 0) {

				

				if (!cache) {
					cache = yaffs_GrabChunkCache(in->myDev);
					cache->object = in;
					cache->chunkId = chunk;
					cache->dirty = 0;
					cache->locked = 0;
					yaffs_ReadChunkDataFromObject(in, chunk,
								      cache->
								      data);
					cache->nBytes = 0;
				}

				yaffs_UseChunkCache(dev, cache, 0);

				cache->locked = 1;

#ifdef CONFIG_YAFFS_WINCE
				yfsd_UnlockYAFFS(TRUE);
#endif
				memcpy(buffer, &cache->data[start], nToCopy);

#ifdef CONFIG_YAFFS_WINCE
				yfsd_LockYAFFS(TRUE);
#endif
				cache->locked = 0;
			} else {
				

				__u8 *localBuffer =
				    yaffs_GetTempBuffer(dev, __LINE__);
				yaffs_ReadChunkDataFromObject(in, chunk,
							      localBuffer);
#ifdef CONFIG_YAFFS_WINCE
				yfsd_UnlockYAFFS(TRUE);
#endif
				memcpy(buffer, &localBuffer[start], nToCopy);

#ifdef CONFIG_YAFFS_WINCE
				yfsd_LockYAFFS(TRUE);
#endif
				yaffs_ReleaseTempBuffer(dev, localBuffer,
							__LINE__);
			}

		} else {
#ifdef CONFIG_YAFFS_WINCE
			__u8 *localBuffer = yaffs_GetTempBuffer(dev, __LINE__);

			yaffs_ReadChunkDataFromObject(in, chunk, localBuffer);

#ifdef CONFIG_YAFFS_WINCE
			yfsd_UnlockYAFFS(TRUE);
#endif
			memcpy(buffer, localBuffer, dev->nDataBytesPerChunk);

#ifdef CONFIG_YAFFS_WINCE
			yfsd_LockYAFFS(TRUE);
			yaffs_ReleaseTempBuffer(dev, localBuffer, __LINE__);
#endif

#else
			
			yaffs_ReadChunkDataFromObject(in, chunk, buffer);
#endif
		}

		n -= nToCopy;
		offset += nToCopy;
		buffer += nToCopy;
		nDone += nToCopy;

	}

	return nDone;
}

int yaffs_WriteDataToFile(yaffs_Object * in, const __u8 * buffer, loff_t offset,
			  int nBytes, int writeThrough)
{

	int chunk;
	int start;
	int nToCopy;
	int n = nBytes;
	int nDone = 0;
	int nToWriteBack;
	int startOfWrite = offset;
	int chunkWritten = 0;
	int nBytesRead;

	yaffs_Device *dev;

	dev = in->myDev;

	while (n > 0 && chunkWritten >= 0) {
		
		
		yaffs_AddrToChunk(dev,offset,&chunk,&start);
		chunk++;


		if ((start + n) < dev->nDataBytesPerChunk) {
			nToCopy = n;


			nBytesRead =
			    in->variant.fileVariant.fileSize -
			    ((chunk - 1) * dev->nDataBytesPerChunk);

			if (nBytesRead > dev->nDataBytesPerChunk) {
				nBytesRead = dev->nDataBytesPerChunk;
			}

			nToWriteBack =
			    (nBytesRead >
			     (start + n)) ? nBytesRead : (start + n);

		} else {
			nToCopy = dev->nDataBytesPerChunk - start;
			nToWriteBack = dev->nDataBytesPerChunk;
		}

		if (nToCopy != dev->nDataBytesPerChunk) {
			
			if (dev->nShortOpCaches > 0) {
				yaffs_ChunkCache *cache;
				
				cache = yaffs_FindChunkCache(in, chunk);
				
				if (!cache
				    && yaffs_CheckSpaceForAllocation(in->
								     myDev)) {
					cache = yaffs_GrabChunkCache(in->myDev);
					cache->object = in;
					cache->chunkId = chunk;
					cache->dirty = 0;
					cache->locked = 0;
					yaffs_ReadChunkDataFromObject(in, chunk,
								      cache->
								      data);
				}
				else if(cache && 
				        !cache->dirty &&
					!yaffs_CheckSpaceForAllocation(in->myDev)){
 
					 cache = NULL;
				}

				if (cache) {
					yaffs_UseChunkCache(dev, cache, 1);
					cache->locked = 1;
#ifdef CONFIG_YAFFS_WINCE
					yfsd_UnlockYAFFS(TRUE);
#endif

					memcpy(&cache->data[start], buffer,
					       nToCopy);

#ifdef CONFIG_YAFFS_WINCE
					yfsd_LockYAFFS(TRUE);
#endif
					cache->locked = 0;
					cache->nBytes = nToWriteBack;

					if (writeThrough) {
						chunkWritten =
						    yaffs_WriteChunkDataToObject
						    (cache->object,
						     cache->chunkId,
						     cache->data, cache->nBytes,
						     1);
						cache->dirty = 0;
					}

				} else {
					chunkWritten = -1;	
				}
			} else {

				__u8 *localBuffer =
				    yaffs_GetTempBuffer(dev, __LINE__);

				yaffs_ReadChunkDataFromObject(in, chunk,
							      localBuffer);

#ifdef CONFIG_YAFFS_WINCE
				yfsd_UnlockYAFFS(TRUE);
#endif

				memcpy(&localBuffer[start], buffer, nToCopy);

#ifdef CONFIG_YAFFS_WINCE
				yfsd_LockYAFFS(TRUE);
#endif
				chunkWritten =
				    yaffs_WriteChunkDataToObject(in, chunk,
								 localBuffer,
								 nToWriteBack,
								 0);

				yaffs_ReleaseTempBuffer(dev, localBuffer,
							__LINE__);

			}

		} else {

#ifdef CONFIG_YAFFS_WINCE
			__u8 *localBuffer = yaffs_GetTempBuffer(dev, __LINE__);
#ifdef CONFIG_YAFFS_WINCE
			yfsd_UnlockYAFFS(TRUE);
#endif
			memcpy(localBuffer, buffer, dev->nDataBytesPerChunk);
#ifdef CONFIG_YAFFS_WINCE
			yfsd_LockYAFFS(TRUE);
#endif
			chunkWritten =
			    yaffs_WriteChunkDataToObject(in, chunk, localBuffer,
							 dev->nDataBytesPerChunk,
							 0);
			yaffs_ReleaseTempBuffer(dev, localBuffer, __LINE__);
#else
			
			chunkWritten =
			    yaffs_WriteChunkDataToObject(in, chunk, buffer,
							 dev->nDataBytesPerChunk,
							 0);
#endif
			/* Since we've overwritten the cached data, we better invalidate it. */
			yaffs_InvalidateChunkCache(in, chunk);
		}

		if (chunkWritten >= 0) {
			n -= nToCopy;
			offset += nToCopy;
			buffer += nToCopy;
			nDone += nToCopy;
		}

	}

	

	if ((startOfWrite + nDone) > in->variant.fileVariant.fileSize) {
		in->variant.fileVariant.fileSize = (startOfWrite + nDone);
	}

	in->dirty = 1;

	return nDone;
}



static void yaffs_PruneResizedChunks(yaffs_Object * in, int newSize)
{

	yaffs_Device *dev = in->myDev;
	int oldFileSize = in->variant.fileVariant.fileSize;

	int lastDel = 1 + (oldFileSize - 1) / dev->nDataBytesPerChunk;

	int startDel = 1 + (newSize + dev->nDataBytesPerChunk - 1) /
	    dev->nDataBytesPerChunk;
	int i;
	int chunkId;

	for (i = lastDel; i >= startDel; i--) {

		chunkId = yaffs_FindAndDeleteChunkInFile(in, i, NULL);
		if (chunkId > 0) {
			if (chunkId <
			    (dev->internalStartBlock * dev->nChunksPerBlock)
			    || chunkId >=
			    ((dev->internalEndBlock +
			      1) * dev->nChunksPerBlock)) {
				T(YAFFS_TRACE_ALWAYS,
				  (TSTR("Found daft chunkId %d for %d" TENDSTR),
				   chunkId, i));
			} else {
				in->nDataChunks--;
				yaffs_DeleteChunk(dev, chunkId, 1, __LINE__);
			}
		}
	}

}

int yaffs_ResizeFile(yaffs_Object * in, loff_t newSize)
{

	int oldFileSize = in->variant.fileVariant.fileSize;
	int newSizeOfPartialChunk;
	int newFullChunks;
	
	yaffs_Device *dev = in->myDev;
	
	yaffs_AddrToChunk(dev, newSize, &newFullChunks, &newSizeOfPartialChunk);

	yaffs_FlushFilesChunkCache(in);
	yaffs_InvalidateWholeChunkCache(in);

	yaffs_CheckGarbageCollection(dev);

	if (in->variantType != YAFFS_OBJECT_TYPE_FILE) {
		return yaffs_GetFileSize(in);
	}

	if (newSize == oldFileSize) {
		return oldFileSize;
	}

	if (newSize < oldFileSize) {

		yaffs_PruneResizedChunks(in, newSize);

		if (newSizeOfPartialChunk != 0) {
			int lastChunk = 1 + newFullChunks;
			
			__u8 *localBuffer = yaffs_GetTempBuffer(dev, __LINE__);

			
			yaffs_ReadChunkDataFromObject(in, lastChunk,
						      localBuffer);

			memset(localBuffer + newSizeOfPartialChunk, 0,
			       dev->nDataBytesPerChunk - newSizeOfPartialChunk);

			yaffs_WriteChunkDataToObject(in, lastChunk, localBuffer,
						     newSizeOfPartialChunk, 1);

			yaffs_ReleaseTempBuffer(dev, localBuffer, __LINE__);
		}

		in->variant.fileVariant.fileSize = newSize;

		yaffs_PruneFileStructure(dev, &in->variant.fileVariant);
	}
	if (in->parent->objectId != YAFFS_OBJECTID_UNLINKED &&
	    in->parent->objectId != YAFFS_OBJECTID_DELETED) {
		yaffs_UpdateObjectHeader(in, NULL, 0,
					 (newSize < oldFileSize) ? 1 : 0, 0);
	}

	return newSize;
}

loff_t yaffs_GetFileSize(yaffs_Object * obj)
{
	obj = yaffs_GetEquivalentObject(obj);

	switch (obj->variantType) {
	case YAFFS_OBJECT_TYPE_FILE:
		return obj->variant.fileVariant.fileSize;
	case YAFFS_OBJECT_TYPE_SYMLINK:
		return yaffs_strlen(obj->variant.symLinkVariant.alias);
	default:
		return 0;
	}
}



int yaffs_FlushFile(yaffs_Object * in, int updateTime)
{
	int retVal;
	if (in->dirty) {
		yaffs_FlushFilesChunkCache(in);
		if (updateTime) {
#ifdef CONFIG_YAFFS_WINCE
			yfsd_WinFileTimeNow(in->win_mtime);
#else

			in->yst_mtime = Y_CURRENT_TIME;

#endif
		}

		retVal =
		    (yaffs_UpdateObjectHeader(in, NULL, 0, 0, 0) >=
		     0) ? YAFFS_OK : YAFFS_FAIL;
	} else {
		retVal = YAFFS_OK;
	}

	return retVal;

}

static int yaffs_DoGenericObjectDeletion(yaffs_Object * in)
{

	
	yaffs_InvalidateWholeChunkCache(in);

	if (in->myDev->isYaffs2 && (in->parent != in->myDev->deletedDir)) {
		
		yaffs_ChangeObjectName(in, in->myDev->deletedDir, NULL, 0, 0);

	}

	yaffs_RemoveObjectFromDirectory(in);
	yaffs_DeleteChunk(in->myDev, in->chunkId, 1, __LINE__);
	in->chunkId = -1;

	yaffs_FreeObject(in);
	return YAFFS_OK;

}

static int yaffs_UnlinkFile(yaffs_Object * in)
{

	int retVal;
	int immediateDeletion = 0;

	if (1) {
#ifdef __KERNEL__
		if (!in->myInode) {
			immediateDeletion = 1;

		}
#else
		if (in->inUse <= 0) {
			immediateDeletion = 1;

		}
#endif
		if (immediateDeletion) {
			retVal =
			    yaffs_ChangeObjectName(in, in->myDev->deletedDir,
						   NULL, 0, 0);
			T(YAFFS_TRACE_TRACING,
			  (TSTR("yaffs: immediate deletion of file %d" TENDSTR),
			   in->objectId));
			in->deleted = 1;
			in->myDev->nDeletedFiles++;
			if (0 && in->myDev->isYaffs2) {
				yaffs_ResizeFile(in, 0);
			}
			yaffs_SoftDeleteFile(in);
		} else {
			retVal =
			    yaffs_ChangeObjectName(in, in->myDev->unlinkedDir,
						   NULL, 0, 0);
		}

	}
	return retVal;
}

int yaffs_DeleteFile(yaffs_Object * in)
{
	int retVal = YAFFS_OK;

	if (in->nDataChunks > 0) {
		
		if (!in->unlinked) {
			retVal = yaffs_UnlinkFile(in);
		}
		if (retVal == YAFFS_OK && in->unlinked && !in->deleted) {
			in->deleted = 1;
			in->myDev->nDeletedFiles++;
			yaffs_SoftDeleteFile(in);
		}
		return in->deleted ? YAFFS_OK : YAFFS_FAIL;
	} else {
		
		yaffs_FreeTnode(in->myDev, in->variant.fileVariant.top);
		in->variant.fileVariant.top = NULL;
		yaffs_DoGenericObjectDeletion(in);

		return YAFFS_OK;
	}
}

static int yaffs_DeleteDirectory(yaffs_Object * in)
{
	
	if (list_empty(&in->variant.directoryVariant.children)) {
		return yaffs_DoGenericObjectDeletion(in);
	}

	return YAFFS_FAIL;

}

static int yaffs_DeleteSymLink(yaffs_Object * in)
{
	YFREE(in->variant.symLinkVariant.alias);

	return yaffs_DoGenericObjectDeletion(in);
}

static int yaffs_DeleteHardLink(yaffs_Object * in)
{
	list_del(&in->hardLinks);
	return yaffs_DoGenericObjectDeletion(in);
}

static void yaffs_DestroyObject(yaffs_Object * obj)
{
	switch (obj->variantType) {
	case YAFFS_OBJECT_TYPE_FILE:
		yaffs_DeleteFile(obj);
		break;
	case YAFFS_OBJECT_TYPE_DIRECTORY:
		yaffs_DeleteDirectory(obj);
		break;
	case YAFFS_OBJECT_TYPE_SYMLINK:
		yaffs_DeleteSymLink(obj);
		break;
	case YAFFS_OBJECT_TYPE_HARDLINK:
		yaffs_DeleteHardLink(obj);
		break;
	case YAFFS_OBJECT_TYPE_SPECIAL:
		yaffs_DoGenericObjectDeletion(obj);
		break;
	case YAFFS_OBJECT_TYPE_UNKNOWN:
		break;		
	}
}

static int yaffs_UnlinkWorker(yaffs_Object * obj)
{

	if (obj->variantType == YAFFS_OBJECT_TYPE_HARDLINK) {
		return yaffs_DeleteHardLink(obj);
	} else if (!list_empty(&obj->hardLinks)) {

		yaffs_Object *hl;
		int retVal;
		YCHAR name[YAFFS_MAX_NAME_LENGTH + 1];

		hl = list_entry(obj->hardLinks.next, yaffs_Object, hardLinks);

		list_del_init(&hl->hardLinks);
		list_del_init(&hl->siblings);

		yaffs_GetObjectName(hl, name, YAFFS_MAX_NAME_LENGTH + 1);

		retVal = yaffs_ChangeObjectName(obj, hl->parent, name, 0, 0);

		if (retVal == YAFFS_OK) {
			retVal = yaffs_DoGenericObjectDeletion(hl);
		}
		return retVal;

	} else {
		switch (obj->variantType) {
		case YAFFS_OBJECT_TYPE_FILE:
			return yaffs_UnlinkFile(obj);
			break;
		case YAFFS_OBJECT_TYPE_DIRECTORY:
			return yaffs_DeleteDirectory(obj);
			break;
		case YAFFS_OBJECT_TYPE_SYMLINK:
			return yaffs_DeleteSymLink(obj);
			break;
		case YAFFS_OBJECT_TYPE_SPECIAL:
			return yaffs_DoGenericObjectDeletion(obj);
			break;
		case YAFFS_OBJECT_TYPE_HARDLINK:
		case YAFFS_OBJECT_TYPE_UNKNOWN:
		default:
			return YAFFS_FAIL;
		}
	}
}


static int yaffs_UnlinkObject( yaffs_Object *obj)
{

	if (obj && obj->unlinkAllowed) {
		return yaffs_UnlinkWorker(obj);
	}

	return YAFFS_FAIL;

}
int yaffs_Unlink(yaffs_Object * dir, const YCHAR * name)
{
	yaffs_Object *obj;

	obj = yaffs_FindObjectByName(dir, name);
	return yaffs_UnlinkObject(obj);
}


static void yaffs_HandleShadowedObject(yaffs_Device * dev, int objId,
				       int backwardScanning)
{
	yaffs_Object *obj;

	if (!backwardScanning) {

	} else {
		if (yaffs_FindObjectByNumber(dev, objId)) {
			return;
		}
	}

	obj =
	    yaffs_FindOrCreateObjectByNumber(dev, objId,
					     YAFFS_OBJECT_TYPE_FILE);
	yaffs_AddObjectToDirectory(dev->unlinkedDir, obj);
	obj->variant.fileVariant.shrinkSize = 0;
	obj->valid = 1;		

}

typedef struct {
	int seq;
	int block;
} yaffs_BlockIndex;


static void yaffs_HardlinkFixup(yaffs_Device *dev, yaffs_Object *hardList)
{
	yaffs_Object *hl;
	yaffs_Object *in;
	
	while (hardList) {
		hl = hardList;
		hardList = (yaffs_Object *) (hardList->hardLinks.next);

		in = yaffs_FindObjectByNumber(dev,
					      hl->variant.hardLinkVariant.
					      equivalentObjectId);

		if (in) {
			
			hl->variant.hardLinkVariant.equivalentObject = in;
			list_add(&hl->hardLinks, &in->hardLinks);
		} else {
			hl->variant.hardLinkVariant.equivalentObject = NULL;
			INIT_LIST_HEAD(&hl->hardLinks);

		}

	}

}





static int ybicmp(const void *a, const void *b){
    register int aseq = ((yaffs_BlockIndex *)a)->seq;
    register int bseq = ((yaffs_BlockIndex *)b)->seq;
    register int ablock = ((yaffs_BlockIndex *)a)->block;
    register int bblock = ((yaffs_BlockIndex *)b)->block;
    if( aseq == bseq )
        return ablock - bblock;
    else
        return aseq - bseq;

}

static int yaffs_Scan(yaffs_Device * dev)
{
	yaffs_ExtendedTags tags;
	int blk;
	int blockIterator;
	int startIterator;
	int endIterator;
	int nBlocksToScan = 0;
	int result;

	int chunk;
	int c;
	int deleted;
	yaffs_BlockState state;
	yaffs_Object *hardList = NULL;
	yaffs_Object *hl;
	yaffs_BlockInfo *bi;
	int sequenceNumber;
	yaffs_ObjectHeader *oh;
	yaffs_Object *in;
	yaffs_Object *parent;
	int nBlocks = dev->internalEndBlock - dev->internalStartBlock + 1;

	__u8 *chunkData;

	yaffs_BlockIndex *blockIndex = NULL;

	if (dev->isYaffs2) {
		T(YAFFS_TRACE_SCAN,
		  (TSTR("yaffs_Scan is not for YAFFS2!" TENDSTR)));
		return YAFFS_FAIL;
	}
	
	
	
	T(YAFFS_TRACE_SCAN,
	  (TSTR("yaffs_Scan starts  intstartblk %d intendblk %d..." TENDSTR),
	   dev->internalStartBlock, dev->internalEndBlock));

	chunkData = yaffs_GetTempBuffer(dev, __LINE__);

	dev->sequenceNumber = YAFFS_LOWEST_SEQUENCE_NUMBER;

	if (dev->isYaffs2) {
		blockIndex = YMALLOC(nBlocks * sizeof(yaffs_BlockIndex));
	}

	
	for (blk = dev->internalStartBlock; blk <= dev->internalEndBlock; blk++) {
		bi = yaffs_GetBlockInfo(dev, blk);
		yaffs_ClearChunkBits(dev, blk);
		bi->pagesInUse = 0;
		bi->softDeletions = 0;

		yaffs_QueryInitialBlockState(dev, blk, &state, &sequenceNumber);

		bi->blockState = state;
		bi->sequenceNumber = sequenceNumber;

		T(YAFFS_TRACE_SCAN_DEBUG,
		  (TSTR("Block scanning block %d state %d seq %d" TENDSTR), blk,
		   state, sequenceNumber));

		if (state == YAFFS_BLOCK_STATE_DEAD) {
			T(YAFFS_TRACE_BAD_BLOCKS,
			  (TSTR("block %d is bad" TENDSTR), blk));
		} else if (state == YAFFS_BLOCK_STATE_EMPTY) {
			T(YAFFS_TRACE_SCAN_DEBUG,
			  (TSTR("Block empty " TENDSTR)));
			dev->nErasedBlocks++;
			dev->nFreeChunks += dev->nChunksPerBlock;
		} else if (state == YAFFS_BLOCK_STATE_NEEDS_SCANNING) {

			
			if (dev->isYaffs2 &&
			    sequenceNumber >= YAFFS_LOWEST_SEQUENCE_NUMBER &&
			    sequenceNumber < YAFFS_HIGHEST_SEQUENCE_NUMBER) {

				blockIndex[nBlocksToScan].seq = sequenceNumber;
				blockIndex[nBlocksToScan].block = blk;

				nBlocksToScan++;

				if (sequenceNumber >= dev->sequenceNumber) {
					dev->sequenceNumber = sequenceNumber;
				}
			} else if (dev->isYaffs2) {
				
				T(YAFFS_TRACE_SCAN,
				  (TSTR
				   ("Block scanning block %d has bad sequence number %d"
				    TENDSTR), blk, sequenceNumber));

			}
		}
	}

	if (dev->isYaffs2) {
		yaffs_BlockIndex temp;
		int i;
		int j;

		for (i = 0; i < nBlocksToScan; i++)
			for (j = i + 1; j < nBlocksToScan; j++)
				if (blockIndex[i].seq > blockIndex[j].seq) {
					temp = blockIndex[j];
					blockIndex[j] = blockIndex[i];
					blockIndex[i] = temp;
				}
	}

	
	if (dev->isYaffs2) {
		startIterator = 0;
		endIterator = nBlocksToScan - 1;
		T(YAFFS_TRACE_SCAN_DEBUG,
		  (TSTR("%d blocks to be scanned" TENDSTR), nBlocksToScan));
	} else {
		startIterator = dev->internalStartBlock;
		endIterator = dev->internalEndBlock;
	}

	
	for (blockIterator = startIterator; blockIterator <= endIterator;
	     blockIterator++) {

		if (dev->isYaffs2) {
			
			blk = blockIndex[blockIterator].block;
		} else {
			blk = blockIterator;
		}

		bi = yaffs_GetBlockInfo(dev, blk);
		state = bi->blockState;

		deleted = 0;

		
		for (c = 0; c < dev->nChunksPerBlock &&
		     state == YAFFS_BLOCK_STATE_NEEDS_SCANNING; c++) {
			
			chunk = blk * dev->nChunksPerBlock + c;

			result = yaffs_ReadChunkWithTagsFromNAND(dev, chunk, NULL,
							&tags);

			

			if (!dev->isYaffs2 && tags.chunkDeleted) {
				deleted++;
				dev->nFreeChunks++;
				
			} else if (!tags.chunkUsed) {

				if (c == 0) {
					
					state = YAFFS_BLOCK_STATE_EMPTY;
					dev->nErasedBlocks++;
				} else {
					
					T(YAFFS_TRACE_SCAN,
					  (TSTR
					   (" Allocating from %d %d" TENDSTR),
					   blk, c));
					state = YAFFS_BLOCK_STATE_ALLOCATING;
					dev->allocationBlock = blk;
					dev->allocationPage = c;
					dev->allocationBlockFinder = blk;	
					
					
					if (dev->isYaffs2
					    && (dev->sequenceNumber !=
						bi->sequenceNumber)) {
						T(YAFFS_TRACE_ALWAYS,
						  (TSTR
						   ("yaffs: Allocation block %d was not highest sequence id:"
						    " block seq = %d, dev seq = %d"
						    TENDSTR), blk,bi->sequenceNumber,dev->sequenceNumber));
					}
				}

				dev->nFreeChunks += (dev->nChunksPerBlock - c);
			} else if (tags.chunkId > 0) {
				
				unsigned int endpos;

				yaffs_SetChunkBit(dev, blk, c);
				bi->pagesInUse++;

				in = yaffs_FindOrCreateObjectByNumber(dev,
								      tags.
								      objectId,
								      YAFFS_OBJECT_TYPE_FILE);
				yaffs_PutChunkIntoFile(in, tags.chunkId, chunk,
						       1);
				endpos =
				    (tags.chunkId - 1) * dev->nDataBytesPerChunk +
				    tags.byteCount;
				if (in->variantType == YAFFS_OBJECT_TYPE_FILE
				    && in->variant.fileVariant.scannedFileSize <
				    endpos) {
					in->variant.fileVariant.
					    scannedFileSize = endpos;
					if (!dev->useHeaderFileSize) {
						in->variant.fileVariant.
						    fileSize =
						    in->variant.fileVariant.
						    scannedFileSize;
					}

				}
				
			} else {
				yaffs_SetChunkBit(dev, blk, c);
				bi->pagesInUse++;

				result = yaffs_ReadChunkWithTagsFromNAND(dev, chunk,
								chunkData,
								NULL);

				oh = (yaffs_ObjectHeader *) chunkData;

				in = yaffs_FindObjectByNumber(dev,
							      tags.objectId);
				if (in && in->variantType != oh->type) {

					yaffs_DestroyObject(in);

					in = 0;
				}

				in = yaffs_FindOrCreateObjectByNumber(dev,
								      tags.
								      objectId,
								      oh->type);

				if (oh->shadowsObject > 0) {
					yaffs_HandleShadowedObject(dev,
								   oh->
								   shadowsObject,
								   0);
				}

				if (in->valid) {
					

					unsigned existingSerial = in->serial;
					unsigned newSerial = tags.serialNumber;

					if (dev->isYaffs2 ||
					    ((existingSerial + 1) & 3) ==
					    newSerial) {
						
						yaffs_DeleteChunk(dev,
								  in->chunkId,
								  1, __LINE__);
						in->valid = 0;
					} else {
						
						yaffs_DeleteChunk(dev, chunk, 1,
								  __LINE__);
					}
				}

				if (!in->valid &&
				    (tags.objectId == YAFFS_OBJECTID_ROOT ||
				     tags.objectId == YAFFS_OBJECTID_LOSTNFOUND)) {
					
					in->valid = 1;
					in->variantType = oh->type;

					in->yst_mode = oh->yst_mode;
#ifdef CONFIG_YAFFS_WINCE
					in->win_atime[0] = oh->win_atime[0];
					in->win_ctime[0] = oh->win_ctime[0];
					in->win_mtime[0] = oh->win_mtime[0];
					in->win_atime[1] = oh->win_atime[1];
					in->win_ctime[1] = oh->win_ctime[1];
					in->win_mtime[1] = oh->win_mtime[1];
#else
					in->yst_uid = oh->yst_uid;
					in->yst_gid = oh->yst_gid;
					in->yst_atime = oh->yst_atime;
					in->yst_mtime = oh->yst_mtime;
					in->yst_ctime = oh->yst_ctime;
					in->yst_rdev = oh->yst_rdev;
#endif
					in->chunkId = chunk;

				} else if (!in->valid) {
					

					in->valid = 1;
					in->variantType = oh->type;

					in->yst_mode = oh->yst_mode;
#ifdef CONFIG_YAFFS_WINCE
					in->win_atime[0] = oh->win_atime[0];
					in->win_ctime[0] = oh->win_ctime[0];
					in->win_mtime[0] = oh->win_mtime[0];
					in->win_atime[1] = oh->win_atime[1];
					in->win_ctime[1] = oh->win_ctime[1];
					in->win_mtime[1] = oh->win_mtime[1];
#else
					in->yst_uid = oh->yst_uid;
					in->yst_gid = oh->yst_gid;
					in->yst_atime = oh->yst_atime;
					in->yst_mtime = oh->yst_mtime;
					in->yst_ctime = oh->yst_ctime;
					in->yst_rdev = oh->yst_rdev;
#endif
					in->chunkId = chunk;

					yaffs_SetObjectName(in, oh->name);
					in->dirty = 0;


					parent =
					    yaffs_FindOrCreateObjectByNumber
					    (dev, oh->parentObjectId,
					     YAFFS_OBJECT_TYPE_DIRECTORY);
					if (parent->variantType ==
					    YAFFS_OBJECT_TYPE_UNKNOWN) {
						
						parent->variantType =
						    YAFFS_OBJECT_TYPE_DIRECTORY;
						INIT_LIST_HEAD(&parent->variant.
							       directoryVariant.
							       children);
					} else if (parent->variantType !=
						   YAFFS_OBJECT_TYPE_DIRECTORY)
					{

						T(YAFFS_TRACE_ERROR,
						  (TSTR
						   ("yaffs tragedy: attempting to use non-directory as"
						    " a directory in scan. Put in lost+found."
						    TENDSTR)));
						parent = dev->lostNFoundDir;
					}

					yaffs_AddObjectToDirectory(parent, in);

					if (0 && (parent == dev->deletedDir ||
						  parent == dev->unlinkedDir)) {
						in->deleted = 1;	
						dev->nDeletedFiles++;
					}

					switch (in->variantType) {
					case YAFFS_OBJECT_TYPE_UNKNOWN:	
						
						break;
					case YAFFS_OBJECT_TYPE_FILE:
						if (dev->isYaffs2
						    && oh->isShrink) {
							
							yaffs_PruneResizedChunks
							    (in, oh->fileSize);
							
							bi->hasShrinkHeader = 1;
						}

						if (dev->useHeaderFileSize)

							in->variant.fileVariant.
							    fileSize =
							    oh->fileSize;

						break;
					case YAFFS_OBJECT_TYPE_HARDLINK:
						in->variant.hardLinkVariant.
						    equivalentObjectId =
						    oh->equivalentObjectId;
						in->hardLinks.next =
						    (struct list_head *)
						    hardList;
						hardList = in;
						break;
					case YAFFS_OBJECT_TYPE_DIRECTORY:
						
						break;
					case YAFFS_OBJECT_TYPE_SPECIAL:
						
						break;
					case YAFFS_OBJECT_TYPE_SYMLINK:	
						in->variant.symLinkVariant.
						    alias =
						    yaffs_CloneString(oh->alias);
						break;
					}

					if (parent == dev->deletedDir) {
						yaffs_DestroyObject(in);
						bi->hasShrinkHeader = 1;
					}
				}
			}
		}

		if (state == YAFFS_BLOCK_STATE_NEEDS_SCANNING) {
			
			state = YAFFS_BLOCK_STATE_FULL;
		}

		bi->blockState = state;

		
		if (bi->pagesInUse == 0 &&
		    !bi->hasShrinkHeader &&
		    bi->blockState == YAFFS_BLOCK_STATE_FULL) {
			yaffs_BlockBecameDirty(dev, blk);
		}

	}

	if (blockIndex) {
		YFREE(blockIndex);
	}
	
	

	yaffs_HardlinkFixup(dev,hardList);

	{
		struct list_head *i;
		struct list_head *n;

		yaffs_Object *l;
		
		list_for_each_safe(i, n,
				   &dev->unlinkedDir->variant.directoryVariant.
				   children) {
			if (i) {
				l = list_entry(i, yaffs_Object, siblings);
				yaffs_DestroyObject(l);
			}
		}
	}

	yaffs_ReleaseTempBuffer(dev, chunkData, __LINE__);

	T(YAFFS_TRACE_SCAN, (TSTR("yaffs_Scan ends" TENDSTR)));

	return YAFFS_OK;
}

static void yaffs_CheckObjectDetailsLoaded(yaffs_Object *in)
{
	__u8 *chunkData;
	yaffs_ObjectHeader *oh;
	yaffs_Device *dev = in->myDev;
	yaffs_ExtendedTags tags;
	int result;
	
	if(in->lazyLoaded){
		in->lazyLoaded = 0;
		chunkData = yaffs_GetTempBuffer(dev, __LINE__);

		result = yaffs_ReadChunkWithTagsFromNAND(dev,in->chunkId,chunkData,&tags);
		oh = (yaffs_ObjectHeader *) chunkData;		

		in->yst_mode = oh->yst_mode;
#ifdef CONFIG_YAFFS_WINCE
		in->win_atime[0] = oh->win_atime[0];
		in->win_ctime[0] = oh->win_ctime[0];
		in->win_mtime[0] = oh->win_mtime[0];
		in->win_atime[1] = oh->win_atime[1];
		in->win_ctime[1] = oh->win_ctime[1];
		in->win_mtime[1] = oh->win_mtime[1];
#else
		in->yst_uid = oh->yst_uid;
		in->yst_gid = oh->yst_gid;
		in->yst_atime = oh->yst_atime;
		in->yst_mtime = oh->yst_mtime;
		in->yst_ctime = oh->yst_ctime;
		in->yst_rdev = oh->yst_rdev;
		
#endif
		yaffs_SetObjectName(in, oh->name);
		
		if(in->variantType == YAFFS_OBJECT_TYPE_SYMLINK)
			 in->variant.symLinkVariant.alias =
						    yaffs_CloneString(oh->alias);
						    
		yaffs_ReleaseTempBuffer(dev,chunkData, __LINE__);
	}
}

static int yaffs_ScanBackwards(yaffs_Device * dev)
{
	yaffs_ExtendedTags tags;
	int blk;
	int blockIterator;
	int startIterator;
	int endIterator;
	int nBlocksToScan = 0;

	int chunk;
	int result;
	int c;
	int deleted;
	yaffs_BlockState state;
	yaffs_Object *hardList = NULL;
	yaffs_BlockInfo *bi;
	int sequenceNumber;
	yaffs_ObjectHeader *oh;
	yaffs_Object *in;
	yaffs_Object *parent;
	int nBlocks = dev->internalEndBlock - dev->internalStartBlock + 1;
	int itsUnlinked;
	__u8 *chunkData;
	
	int fileSize;
	int isShrink;
	int foundChunksInBlock;
	int equivalentObjectId;
	

	yaffs_BlockIndex *blockIndex = NULL;
	int altBlockIndex = 0;

	if (!dev->isYaffs2) {
		T(YAFFS_TRACE_SCAN,
		  (TSTR("yaffs_ScanBackwards is only for YAFFS2!" TENDSTR)));
		return YAFFS_FAIL;
	}

	T(YAFFS_TRACE_SCAN,
	  (TSTR
	   ("yaffs_ScanBackwards starts  intstartblk %d intendblk %d..."
	    TENDSTR), dev->internalStartBlock, dev->internalEndBlock));


	dev->sequenceNumber = YAFFS_LOWEST_SEQUENCE_NUMBER;

	blockIndex = YMALLOC(nBlocks * sizeof(yaffs_BlockIndex));
	
	if(!blockIndex) {
		blockIndex = YMALLOC_ALT(nBlocks * sizeof(yaffs_BlockIndex));
		altBlockIndex = 1;
	}
	
	if(!blockIndex) {
		T(YAFFS_TRACE_SCAN,
		  (TSTR("yaffs_Scan() could not allocate block index!" TENDSTR)));
		return YAFFS_FAIL;
	}
	
	chunkData = yaffs_GetTempBuffer(dev, __LINE__);

	
	for (blk = dev->internalStartBlock; blk <= dev->internalEndBlock; blk++) {
		bi = yaffs_GetBlockInfo(dev, blk);
		yaffs_ClearChunkBits(dev, blk);
		bi->pagesInUse = 0;
		bi->softDeletions = 0;

		yaffs_QueryInitialBlockState(dev, blk, &state, &sequenceNumber);

		bi->blockState = state;
		bi->sequenceNumber = sequenceNumber;

		if(bi->sequenceNumber == YAFFS_SEQUENCE_CHECKPOINT_DATA)
			bi->blockState = state = YAFFS_BLOCK_STATE_CHECKPOINT;
			
		T(YAFFS_TRACE_SCAN_DEBUG,
		  (TSTR("Block scanning block %d state %d seq %d" TENDSTR), blk,
		   state, sequenceNumber));

		
		if(state == YAFFS_BLOCK_STATE_CHECKPOINT){
			
			
		} else if (state == YAFFS_BLOCK_STATE_DEAD) {
			T(YAFFS_TRACE_BAD_BLOCKS,
			  (TSTR("block %d is bad" TENDSTR), blk));
		} else if (state == YAFFS_BLOCK_STATE_EMPTY) {
			T(YAFFS_TRACE_SCAN_DEBUG,
			  (TSTR("Block empty " TENDSTR)));
			dev->nErasedBlocks++;
			dev->nFreeChunks += dev->nChunksPerBlock;
		} else if (state == YAFFS_BLOCK_STATE_NEEDS_SCANNING) {

			
			if (dev->isYaffs2 &&
			    sequenceNumber >= YAFFS_LOWEST_SEQUENCE_NUMBER &&
			    sequenceNumber < YAFFS_HIGHEST_SEQUENCE_NUMBER) {

				blockIndex[nBlocksToScan].seq = sequenceNumber;
				blockIndex[nBlocksToScan].block = blk;

				nBlocksToScan++;

				if (sequenceNumber >= dev->sequenceNumber) {
					dev->sequenceNumber = sequenceNumber;
				}
			} else if (dev->isYaffs2) {
				
				T(YAFFS_TRACE_SCAN,
				  (TSTR
				   ("Block scanning block %d has bad sequence number %d"
				    TENDSTR), blk, sequenceNumber));

			}
		}
	}

	T(YAFFS_TRACE_SCAN,
	(TSTR("%d blocks to be sorted..." TENDSTR), nBlocksToScan));



	YYIELD();

	
#ifndef CONFIG_YAFFS_USE_OWN_SORT
	{
		
		qsort(blockIndex, nBlocksToScan, sizeof(yaffs_BlockIndex), ybicmp);
	}
#else
	{
	 	
	 	
		yaffs_BlockIndex temp;
		int i;
		int j;

		for (i = 0; i < nBlocksToScan; i++)
			for (j = i + 1; j < nBlocksToScan; j++)
				if (blockIndex[i].seq > blockIndex[j].seq) {
					temp = blockIndex[j];
					blockIndex[j] = blockIndex[i];
					blockIndex[i] = temp;
				}
	}
#endif

	YYIELD();

    	T(YAFFS_TRACE_SCAN, (TSTR("...done" TENDSTR)));

	
	startIterator = 0;
	endIterator = nBlocksToScan - 1;
	T(YAFFS_TRACE_SCAN_DEBUG,
	  (TSTR("%d blocks to be scanned" TENDSTR), nBlocksToScan));

	
	for (blockIterator = endIterator; blockIterator >= startIterator;
	     blockIterator--) {
	        YYIELD();

		
		blk = blockIndex[blockIterator].block;

		bi = yaffs_GetBlockInfo(dev, blk);
		state = bi->blockState;

		deleted = 0;

		
		foundChunksInBlock = 0;
		for (c = dev->nChunksPerBlock - 1; c >= 0 &&
		     (state == YAFFS_BLOCK_STATE_NEEDS_SCANNING ||
		      state == YAFFS_BLOCK_STATE_ALLOCATING); c--) {
			chunk = blk * dev->nChunksPerBlock + c;

			result = yaffs_ReadChunkWithTagsFromNAND(dev, chunk, NULL,
							&tags);

			

			if (!tags.chunkUsed) {

				if(foundChunksInBlock)
				{
					
					
				} else if (c == 0) {
					
					state = YAFFS_BLOCK_STATE_EMPTY;
					dev->nErasedBlocks++;
				} else {
					
					if (state ==
					    YAFFS_BLOCK_STATE_NEEDS_SCANNING) {
						T(YAFFS_TRACE_SCAN,
						  (TSTR
						   (" Allocating from %d %d"
						    TENDSTR), blk, c));
					}
					state = YAFFS_BLOCK_STATE_ALLOCATING;
					dev->allocationBlock = blk;
					dev->allocationPage = c;
					dev->allocationBlockFinder = blk;	
					 
					if (dev->isYaffs2
					    && (dev->sequenceNumber !=
						bi->sequenceNumber)) {
						T(YAFFS_TRACE_ALWAYS,
						  (TSTR
						   ("yaffs: Allocation block %d was not highest sequence "
						    "id: block seq = %d, dev seq = %d"
						    TENDSTR), blk,
						   bi->sequenceNumber,
						   dev->sequenceNumber));
					}
				}

				dev->nFreeChunks++;
			} else if (tags.chunkId > 0) {
				
				unsigned int endpos;
				__u32 chunkBase =
				    (tags.chunkId - 1) * dev->nDataBytesPerChunk;
								
				foundChunksInBlock = 1;


				yaffs_SetChunkBit(dev, blk, c);
				bi->pagesInUse++;

				in = yaffs_FindOrCreateObjectByNumber(dev,
								      tags.
								      objectId,
								      YAFFS_OBJECT_TYPE_FILE);
				if (in->variantType == YAFFS_OBJECT_TYPE_FILE
				    && chunkBase <
				    in->variant.fileVariant.shrinkSize) {
					
					yaffs_PutChunkIntoFile(in, tags.chunkId,
							       chunk, -1);

					endpos =
					    (tags.chunkId -
					     1) * dev->nDataBytesPerChunk +
					    tags.byteCount;
					    
					if (!in->valid &&	
					    in->variant.fileVariant.
					    scannedFileSize < endpos) {
						in->variant.fileVariant.
						    scannedFileSize = endpos;
						in->variant.fileVariant.
						    fileSize =
						    in->variant.fileVariant.
						    scannedFileSize;
					}

				} else {
					
					yaffs_DeleteChunk(dev, chunk, 1, __LINE__);

				}
			} else {
				foundChunksInBlock = 1;

				yaffs_SetChunkBit(dev, blk, c);
				bi->pagesInUse++;

				oh = NULL;
				in = NULL;

				if (tags.extraHeaderInfoAvailable) {
					in = yaffs_FindOrCreateObjectByNumber
					    (dev, tags.objectId,
					     tags.extraObjectType);
				}

				if (!in ||
#ifdef CONFIG_YAFFS_DISABLE_LAZY_LOAD
				    !in->valid ||
#endif
				    tags.extraShadows ||
				    (!in->valid &&
				    (tags.objectId == YAFFS_OBJECTID_ROOT ||
				     tags.objectId == YAFFS_OBJECTID_LOSTNFOUND))
				    ) {


					result = yaffs_ReadChunkWithTagsFromNAND(dev,
									chunk,
									chunkData,
									NULL);

					oh = (yaffs_ObjectHeader *) chunkData;

					if (!in)
						in = yaffs_FindOrCreateObjectByNumber(dev, tags.objectId, oh->type);

				}

				if (!in) {
					
					T(YAFFS_TRACE_ERROR,
					  (TSTR
					   ("yaffs tragedy: Could not make object for object  %d  "
					    "at chunk %d during scan"
					    TENDSTR), tags.objectId, chunk));

				}

				if (in->valid) {

					if ((in->variantType == YAFFS_OBJECT_TYPE_FILE) && 
					     ((oh && 
					       oh-> type == YAFFS_OBJECT_TYPE_FILE)||
					      (tags.extraHeaderInfoAvailable  &&
					       tags.extraObjectType == YAFFS_OBJECT_TYPE_FILE))
					    ) {
						__u32 thisSize =
						    (oh) ? oh->fileSize : tags.
						    extraFileLength;
						__u32 parentObjectId =
						    (oh) ? oh->
						    parentObjectId : tags.
						    extraParentObjectId;
						unsigned isShrink =
						    (oh) ? oh->isShrink : tags.
						    extraIsShrinkHeader;

						if (parentObjectId ==
						    YAFFS_OBJECTID_DELETED
						    || parentObjectId ==
						    YAFFS_OBJECTID_UNLINKED) {
							thisSize = 0;
							isShrink = 1;
						}

						if (isShrink &&
						    in->variant.fileVariant.
						    shrinkSize > thisSize) {
							in->variant.fileVariant.
							    shrinkSize =
							    thisSize;
						}

						if (isShrink) {
							bi->hasShrinkHeader = 1;
						}

					}
					
					yaffs_DeleteChunk(dev, chunk, 1, __LINE__);

				}

				if (!in->valid &&
				    (tags.objectId == YAFFS_OBJECTID_ROOT ||
				     tags.objectId ==
				     YAFFS_OBJECTID_LOSTNFOUND)) {
					
					in->valid = 1;
					
					if(oh) {
						in->variantType = oh->type;

						in->yst_mode = oh->yst_mode;
#ifdef CONFIG_YAFFS_WINCE
						in->win_atime[0] = oh->win_atime[0];
						in->win_ctime[0] = oh->win_ctime[0];
						in->win_mtime[0] = oh->win_mtime[0];
						in->win_atime[1] = oh->win_atime[1];
						in->win_ctime[1] = oh->win_ctime[1];
						in->win_mtime[1] = oh->win_mtime[1];
#else
						in->yst_uid = oh->yst_uid;
						in->yst_gid = oh->yst_gid;
						in->yst_atime = oh->yst_atime;
						in->yst_mtime = oh->yst_mtime;
						in->yst_ctime = oh->yst_ctime;
						in->yst_rdev = oh->yst_rdev;
		
#endif
					} else {
						in->variantType = tags.extraObjectType;
						in->lazyLoaded = 1;
					}
						
					in->chunkId = chunk;

				} else if (!in->valid) {
					

					in->valid = 1;
					in->chunkId = chunk;
					
					if(oh) {
						in->variantType = oh->type;

						in->yst_mode = oh->yst_mode;
#ifdef CONFIG_YAFFS_WINCE
						in->win_atime[0] = oh->win_atime[0];
						in->win_ctime[0] = oh->win_ctime[0];
						in->win_mtime[0] = oh->win_mtime[0];
						in->win_atime[1] = oh->win_atime[1];
						in->win_ctime[1] = oh->win_ctime[1];
						in->win_mtime[1] = oh->win_mtime[1];
#else
						in->yst_uid = oh->yst_uid;
						in->yst_gid = oh->yst_gid;
						in->yst_atime = oh->yst_atime;
						in->yst_mtime = oh->yst_mtime;
						in->yst_ctime = oh->yst_ctime;
						in->yst_rdev = oh->yst_rdev;
#endif

						if (oh->shadowsObject > 0) 
							yaffs_HandleShadowedObject(dev,
									   oh->
									   shadowsObject,
									   1);
					

						yaffs_SetObjectName(in, oh->name);
						parent =
						    yaffs_FindOrCreateObjectByNumber
					    		(dev, oh->parentObjectId,
					     		 YAFFS_OBJECT_TYPE_DIRECTORY);

						 fileSize = oh->fileSize;
 						 isShrink = oh->isShrink;
						 equivalentObjectId = oh->equivalentObjectId;

					}
					else {
						in->variantType = tags.extraObjectType;
						parent =
						    yaffs_FindOrCreateObjectByNumber
					    		(dev, tags.extraParentObjectId,
					     		 YAFFS_OBJECT_TYPE_DIRECTORY);
						 fileSize = tags.extraFileLength;
						 isShrink = tags.extraIsShrinkHeader;
						 equivalentObjectId = tags.extraEquivalentObjectId;
						in->lazyLoaded = 1;

					}
					in->dirty = 0;


					if (parent->variantType ==
					    YAFFS_OBJECT_TYPE_UNKNOWN) {
						
						parent->variantType =
						    YAFFS_OBJECT_TYPE_DIRECTORY;
						INIT_LIST_HEAD(&parent->variant.
							       directoryVariant.
							       children);
					} else if (parent->variantType !=
						   YAFFS_OBJECT_TYPE_DIRECTORY)
					{

						T(YAFFS_TRACE_ERROR,
						  (TSTR
						   ("yaffs tragedy: attempting to use non-directory as"
						    " a directory in scan. Put in lost+found."
						    TENDSTR)));
						parent = dev->lostNFoundDir;
					}

					yaffs_AddObjectToDirectory(parent, in);

					itsUnlinked = (parent == dev->deletedDir) ||
						      (parent == dev->unlinkedDir);

					if (isShrink) {
						
						bi->hasShrinkHeader = 1;
					}


					switch (in->variantType) {
					case YAFFS_OBJECT_TYPE_UNKNOWN:	
						
						break;
					case YAFFS_OBJECT_TYPE_FILE:

						if (in->variant.fileVariant.
						    scannedFileSize < fileSize) {
							in->variant.fileVariant.fileSize = fileSize;
							in->variant.fileVariant.scannedFileSize =
							    in->variant.fileVariant.fileSize;
						}

						if (isShrink &&
						    in->variant.fileVariant.shrinkSize > fileSize) {
							in->variant.fileVariant.shrinkSize = fileSize;
						}

						break;
					case YAFFS_OBJECT_TYPE_HARDLINK:
						if(!itsUnlinked) {
						  in->variant.hardLinkVariant.equivalentObjectId =
						    equivalentObjectId;
						  in->hardLinks.next =
						    (struct list_head *) hardList;
						  hardList = in;
						}
						break;
					case YAFFS_OBJECT_TYPE_DIRECTORY:
						
						break;
					case YAFFS_OBJECT_TYPE_SPECIAL:
						
						break;
					case YAFFS_OBJECT_TYPE_SYMLINK:
						if(oh)
						   in->variant.symLinkVariant.alias =
						    yaffs_CloneString(oh->
								      alias);
						break;
					}

				}
			}
		}

		if (state == YAFFS_BLOCK_STATE_NEEDS_SCANNING) {
			
			state = YAFFS_BLOCK_STATE_FULL;
		}

		bi->blockState = state;

		
		if (bi->pagesInUse == 0 &&
		    !bi->hasShrinkHeader &&
		    bi->blockState == YAFFS_BLOCK_STATE_FULL) {
			yaffs_BlockBecameDirty(dev, blk);
		}

	}

	if (altBlockIndex) 
		YFREE_ALT(blockIndex);
	else
		YFREE(blockIndex);
	
	yaffs_HardlinkFixup(dev,hardList);
	
	
	{
		struct list_head *i;
		struct list_head *n;

		yaffs_Object *l;

		
		list_for_each_safe(i, n,
				   &dev->unlinkedDir->variant.directoryVariant.
				   children) {
			if (i) {
				l = list_entry(i, yaffs_Object, siblings);
				yaffs_DestroyObject(l);
			}
		}

		
		list_for_each_safe(i, n,
				   &dev->deletedDir->variant.directoryVariant.
				   children) {
			if (i) {
				l = list_entry(i, yaffs_Object, siblings);
				yaffs_DestroyObject(l);

			}
		}
	}

	yaffs_ReleaseTempBuffer(dev, chunkData, __LINE__);

	T(YAFFS_TRACE_SCAN, (TSTR("yaffs_ScanBackwards ends" TENDSTR)));

	return YAFFS_OK;
}


static void yaffs_RemoveObjectFromDirectory(yaffs_Object * obj)
{
	yaffs_Device *dev = obj->myDev;
	
	if(dev && dev->removeObjectCallback)
		dev->removeObjectCallback(obj);
	   
	list_del_init(&obj->siblings);
	obj->parent = NULL;
}


static void yaffs_AddObjectToDirectory(yaffs_Object * directory,
				       yaffs_Object * obj)
{

	if (!directory) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("tragedy: Trying to add an object to a null pointer directory"
		    TENDSTR)));
		YBUG();
	}
	if (directory->variantType != YAFFS_OBJECT_TYPE_DIRECTORY) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("tragedy: Trying to add an object to a non-directory"
		    TENDSTR)));
		YBUG();
	}

	if (obj->siblings.prev == NULL) {
		
		INIT_LIST_HEAD(&obj->siblings);

	} else if (!list_empty(&obj->siblings)) {
		
		yaffs_RemoveObjectFromDirectory(obj);
	}
	
	list_add(&obj->siblings, &directory->variant.directoryVariant.children);
	obj->parent = directory;

	if (directory == obj->myDev->unlinkedDir
	    || directory == obj->myDev->deletedDir) {
		obj->unlinked = 1;
		obj->myDev->nUnlinkedFiles++;
		obj->renameAllowed = 0;
	}
}

yaffs_Object *yaffs_FindObjectByName(yaffs_Object * directory,
				     const YCHAR * name)
{
	int sum;

	struct list_head *i;
	YCHAR buffer[YAFFS_MAX_NAME_LENGTH + 1];

	yaffs_Object *l;

	if (!name) {
		return NULL;
	}

	if (!directory) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("tragedy: yaffs_FindObjectByName: null pointer directory"
		    TENDSTR)));
		YBUG();
	}
	if (directory->variantType != YAFFS_OBJECT_TYPE_DIRECTORY) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("tragedy: yaffs_FindObjectByName: non-directory" TENDSTR)));
		YBUG();
	}

	sum = yaffs_CalcNameSum(name);

	list_for_each(i, &directory->variant.directoryVariant.children) {
		if (i) {
			l = list_entry(i, yaffs_Object, siblings);
			
			yaffs_CheckObjectDetailsLoaded(l);

			
			if (l->objectId == YAFFS_OBJECTID_LOSTNFOUND) {
				if (yaffs_strcmp(name, YAFFS_LOSTNFOUND_NAME) == 0) {
					return l;
				}
			} else if (yaffs_SumCompare(l->sum, sum) || l->chunkId <= 0)	
			{
				yaffs_GetObjectName(l, buffer,
						    YAFFS_MAX_NAME_LENGTH);
				if (yaffs_strcmp(name, buffer) == 0) {
					return l;
				}

			}
		}
	}

	return NULL;
}


#if 0
int yaffs_ApplyToDirectoryChildren(yaffs_Object * theDir,
				   int (*fn) (yaffs_Object *))
{
	struct list_head *i;
	yaffs_Object *l;

	if (!theDir) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("tragedy: yaffs_FindObjectByName: null pointer directory"
		    TENDSTR)));
		YBUG();
	}
	if (theDir->variantType != YAFFS_OBJECT_TYPE_DIRECTORY) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("tragedy: yaffs_FindObjectByName: non-directory" TENDSTR)));
		YBUG();
	}

	list_for_each(i, &theDir->variant.directoryVariant.children) {
		if (i) {
			l = list_entry(i, yaffs_Object, siblings);
			if (l && !fn(l)) {
				return YAFFS_FAIL;
			}
		}
	}

	return YAFFS_OK;

}
#endif


yaffs_Object *yaffs_GetEquivalentObject(yaffs_Object * obj)
{
	if (obj && obj->variantType == YAFFS_OBJECT_TYPE_HARDLINK) {
		
		obj = obj->variant.hardLinkVariant.equivalentObject;
	}
	return obj;

}

int yaffs_GetObjectName(yaffs_Object * obj, YCHAR * name, int buffSize)
{
	memset(name, 0, buffSize * sizeof(YCHAR));
	
	yaffs_CheckObjectDetailsLoaded(obj);

	if (obj->objectId == YAFFS_OBJECTID_LOSTNFOUND) {
		yaffs_strncpy(name, YAFFS_LOSTNFOUND_NAME, buffSize - 1);
	} else if (obj->chunkId <= 0) {
		YCHAR locName[20];
		
		yaffs_sprintf(locName, _Y("%s%d"), YAFFS_LOSTNFOUND_PREFIX,
			      obj->objectId);
		yaffs_strncpy(name, locName, buffSize - 1);

	}
#ifdef CONFIG_YAFFS_SHORT_NAMES_IN_RAM
	else if (obj->shortName[0]) {
		yaffs_strcpy(name, obj->shortName);
	}
#endif
	else {
		int result;
		__u8 *buffer = yaffs_GetTempBuffer(obj->myDev, __LINE__);

		yaffs_ObjectHeader *oh = (yaffs_ObjectHeader *) buffer;

		memset(buffer, 0, obj->myDev->nDataBytesPerChunk);

		if (obj->chunkId >= 0) {
			result = yaffs_ReadChunkWithTagsFromNAND(obj->myDev,
							obj->chunkId, buffer,
							NULL);
		}
		yaffs_strncpy(name, oh->name, buffSize - 1);

		yaffs_ReleaseTempBuffer(obj->myDev, buffer, __LINE__);
	}

	return yaffs_strlen(name);
}

int yaffs_GetObjectFileLength(yaffs_Object * obj)
{

	
	obj = yaffs_GetEquivalentObject(obj);

	if (obj->variantType == YAFFS_OBJECT_TYPE_FILE) {
		return obj->variant.fileVariant.fileSize;
	}
	if (obj->variantType == YAFFS_OBJECT_TYPE_SYMLINK) {
		return yaffs_strlen(obj->variant.symLinkVariant.alias);
	} else {
		
		return obj->myDev->nDataBytesPerChunk;
	}
}

int yaffs_GetObjectLinkCount(yaffs_Object * obj)
{
	int count = 0;
	struct list_head *i;

	if (!obj->unlinked) {
		count++;	
	}
	list_for_each(i, &obj->hardLinks) {
		count++;	
	}
	return count;

}

int yaffs_GetObjectInode(yaffs_Object * obj)
{
	obj = yaffs_GetEquivalentObject(obj);

	return obj->objectId;
}

unsigned yaffs_GetObjectType(yaffs_Object * obj)
{
	obj = yaffs_GetEquivalentObject(obj);

	switch (obj->variantType) {
	case YAFFS_OBJECT_TYPE_FILE:
		return DT_REG;
		break;
	case YAFFS_OBJECT_TYPE_DIRECTORY:
		return DT_DIR;
		break;
	case YAFFS_OBJECT_TYPE_SYMLINK:
		return DT_LNK;
		break;
	case YAFFS_OBJECT_TYPE_HARDLINK:
		return DT_REG;
		break;
	case YAFFS_OBJECT_TYPE_SPECIAL:
		if (S_ISFIFO(obj->yst_mode))
			return DT_FIFO;
		if (S_ISCHR(obj->yst_mode))
			return DT_CHR;
		if (S_ISBLK(obj->yst_mode))
			return DT_BLK;
		if (S_ISSOCK(obj->yst_mode))
			return DT_SOCK;
	default:
		return DT_REG;
		break;
	}
}

YCHAR *yaffs_GetSymlinkAlias(yaffs_Object * obj)
{
	obj = yaffs_GetEquivalentObject(obj);
	if (obj->variantType == YAFFS_OBJECT_TYPE_SYMLINK) {
		return yaffs_CloneString(obj->variant.symLinkVariant.alias);
	} else {
		return yaffs_CloneString(_Y(""));
	}
}

#ifndef CONFIG_YAFFS_WINCE

int yaffs_SetAttributes(yaffs_Object * obj, struct iattr *attr)
{
	unsigned int valid = attr->ia_valid;

	if (valid & ATTR_MODE)
		obj->yst_mode = attr->ia_mode;
	if (valid & ATTR_UID)
		obj->yst_uid = attr->ia_uid;
	if (valid & ATTR_GID)
		obj->yst_gid = attr->ia_gid;

	if (valid & ATTR_ATIME)
		obj->yst_atime = Y_TIME_CONVERT(attr->ia_atime);
	if (valid & ATTR_CTIME)
		obj->yst_ctime = Y_TIME_CONVERT(attr->ia_ctime);
	if (valid & ATTR_MTIME)
		obj->yst_mtime = Y_TIME_CONVERT(attr->ia_mtime);

	if (valid & ATTR_SIZE)
		yaffs_ResizeFile(obj, attr->ia_size);

	yaffs_UpdateObjectHeader(obj, NULL, 1, 0, 0);

	return YAFFS_OK;

}
int yaffs_GetAttributes(yaffs_Object * obj, struct iattr *attr)
{
	unsigned int valid = 0;

	attr->ia_mode = obj->yst_mode;
	valid |= ATTR_MODE;
	attr->ia_uid = obj->yst_uid;
	valid |= ATTR_UID;
	attr->ia_gid = obj->yst_gid;
	valid |= ATTR_GID;

	Y_TIME_CONVERT(attr->ia_atime) = obj->yst_atime;
	valid |= ATTR_ATIME;
	Y_TIME_CONVERT(attr->ia_ctime) = obj->yst_ctime;
	valid |= ATTR_CTIME;
	Y_TIME_CONVERT(attr->ia_mtime) = obj->yst_mtime;
	valid |= ATTR_MTIME;

	attr->ia_size = yaffs_GetFileSize(obj);
	valid |= ATTR_SIZE;

	attr->ia_valid = valid;

	return YAFFS_OK;

}

#endif

#if 0
int yaffs_DumpObject(yaffs_Object * obj)
{
	YCHAR name[257];

	yaffs_GetObjectName(obj, name, 256);

	T(YAFFS_TRACE_ALWAYS,
	  (TSTR
	   ("Object %d, inode %d \"%s\"\n dirty %d valid %d serial %d sum %d"
	    " chunk %d type %d size %d\n"
	    TENDSTR), obj->objectId, yaffs_GetObjectInode(obj), name,
	   obj->dirty, obj->valid, obj->serial, obj->sum, obj->chunkId,
	   yaffs_GetObjectType(obj), yaffs_GetObjectFileLength(obj)));

	return YAFFS_OK;
}
#endif


static int yaffs_CheckDevFunctions(const yaffs_Device * dev)
{

	
	if (!dev->eraseBlockInNAND || !dev->initialiseNAND)
		return 0;

#ifdef CONFIG_YAFFS_YAFFS2

	
	if (dev->writeChunkWithTagsToNAND &&
	    dev->readChunkWithTagsFromNAND &&
	    !dev->writeChunkToNAND &&
	    !dev->readChunkFromNAND &&
	    dev->markNANDBlockBad && dev->queryNANDBlock)
		return 1;
#endif

	
	if (!dev->isYaffs2 &&
	    !dev->writeChunkWithTagsToNAND &&
	    !dev->readChunkWithTagsFromNAND &&
	    dev->writeChunkToNAND &&
	    dev->readChunkFromNAND &&
	    !dev->markNANDBlockBad && !dev->queryNANDBlock)
		return 1;

	return 0;		
}


static void yaffs_CreateInitialDirectories(yaffs_Device *dev)
{
	
	
	dev->lostNFoundDir = dev->rootDir =  NULL;
	dev->unlinkedDir = dev->deletedDir = NULL;

	dev->unlinkedDir =
	    yaffs_CreateFakeDirectory(dev, YAFFS_OBJECTID_UNLINKED, S_IFDIR);
	dev->deletedDir =
	    yaffs_CreateFakeDirectory(dev, YAFFS_OBJECTID_DELETED, S_IFDIR);

	dev->rootDir =
	    yaffs_CreateFakeDirectory(dev, YAFFS_OBJECTID_ROOT,
				      YAFFS_ROOT_MODE | S_IFDIR);
	dev->lostNFoundDir =
	    yaffs_CreateFakeDirectory(dev, YAFFS_OBJECTID_LOSTNFOUND,
				      YAFFS_LOSTNFOUND_MODE | S_IFDIR);
	yaffs_AddObjectToDirectory(dev->rootDir, dev->lostNFoundDir);
}

int yaffs_GutsInitialise(yaffs_Device * dev)
{
	unsigned x;
	int bits;

	T(YAFFS_TRACE_TRACING, (TSTR("yaffs: yaffs_GutsInitialise()" TENDSTR)));

	

	if (!dev) {
		T(YAFFS_TRACE_ALWAYS, (TSTR("yaffs: Need a device" TENDSTR)));
		return YAFFS_FAIL;
	}

	dev->internalStartBlock = dev->startBlock;
	dev->internalEndBlock = dev->endBlock;
	dev->blockOffset = 0;
	dev->chunkOffset = 0;
	dev->nFreeChunks = 0;

	if (dev->startBlock == 0) {
		dev->internalStartBlock = dev->startBlock + 1;
		dev->internalEndBlock = dev->endBlock + 1;
		dev->blockOffset = 1;
		dev->chunkOffset = dev->nChunksPerBlock;
	}

	

	if ((dev->isYaffs2 && dev->nDataBytesPerChunk < 1024) || 
	    (!dev->isYaffs2 && dev->nDataBytesPerChunk != 512) || 
	     dev->nChunksPerBlock < 2 || 
	     dev->nReservedBlocks < 2 || 
	     dev->internalStartBlock <= 0 || 
	     dev->internalEndBlock <= 0 || 
	     dev->internalEndBlock <= (dev->internalStartBlock + dev->nReservedBlocks + 2)	
	    ) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("yaffs: NAND geometry problems: chunk size %d, type is yaffs%s "
		    TENDSTR), dev->nDataBytesPerChunk, dev->isYaffs2 ? "2" : ""));
		return YAFFS_FAIL;
	}

	if (yaffs_InitialiseNAND(dev) != YAFFS_OK) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR("yaffs: InitialiseNAND failed" TENDSTR)));
		return YAFFS_FAIL;
	}

	
	if (!yaffs_CheckDevFunctions(dev)) {
		
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR
		   ("yaffs: device function(s) missing or wrong\n" TENDSTR)));

		return YAFFS_FAIL;
	}

	
	if (!yaffs_CheckStructures()) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR("yaffs_CheckStructures failed\n" TENDSTR)));
		return YAFFS_FAIL;
	}

	if (dev->isMounted) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR("yaffs: device already mounted\n" TENDSTR)));
		return YAFFS_FAIL;
	}

	

	dev->isMounted = 1;



	
	
	 
	 dev->chunkShift = ShiftDiv(dev->nDataBytesPerChunk);
	 dev->chunkMask = (1<<dev->chunkShift) - 1;

	 if(dev->nDataBytesPerChunk == (dev->chunkMask + 1)){
	 	
		dev->crumbMask = 0;
		dev->crumbShift = 0;
		dev->crumbsPerChunk = 0;
	 } else {
	 	
		dev->crumbShift = ShiftDiv(sizeof(yaffs_PackedTags2TagsPart));
		dev->crumbMask = (1<<dev->crumbShift)-1;
		dev->crumbsPerChunk = dev->nDataBytesPerChunk/(1 << dev->crumbShift);
		dev->chunkShift = 0;
		dev->chunkMask = 0;
	}
	 	


	x = dev->nChunksPerBlock * (dev->internalEndBlock + 1);
	
	bits = ShiftsGE(x);
	
	
	if(!dev->wideTnodesDisabled){
		
		if(bits & 1)
			bits++;
		if(bits < 16)
			dev->tnodeWidth = 16;
		else
			dev->tnodeWidth = bits;
	}
	else
		dev->tnodeWidth = 16;
 
	dev->tnodeMask = (1<<dev->tnodeWidth)-1;
		
		 
	if (bits <= dev->tnodeWidth)
		dev->chunkGroupBits = 0;
	else
		dev->chunkGroupBits = bits - dev->tnodeWidth;
		

	dev->chunkGroupSize = 1 << dev->chunkGroupBits;

	if (dev->nChunksPerBlock < dev->chunkGroupSize) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR("yaffs: chunk group too large\n" TENDSTR)));

		return YAFFS_FAIL;
	}

	

	
	dev->garbageCollections = 0;
	dev->passiveGarbageCollections = 0;
	dev->currentDirtyChecker = 0;
	dev->bufferedBlock = -1;
	dev->doingBufferedBlockRewrite = 0;
	dev->nDeletedFiles = 0;
	dev->nBackgroundDeletions = 0;
	dev->nUnlinkedFiles = 0;
	dev->eccFixed = 0;
	dev->eccUnfixed = 0;
	dev->tagsEccFixed = 0;
	dev->tagsEccUnfixed = 0;
	dev->nErasureFailures = 0;
	dev->nErasedBlocks = 0;
	dev->isDoingGC = 0;
	dev->hasPendingPrioritisedGCs = 1; 

	
	{
		int i;
		for (i = 0; i < YAFFS_N_TEMP_BUFFERS; i++) {
			dev->tempBuffer[i].line = 0;	
			dev->tempBuffer[i].buffer =
			    YMALLOC_DMA(dev->nDataBytesPerChunk);
		}
	}
	
	if (dev->nShortOpCaches > 0) {
		int i;

		if (dev->nShortOpCaches > YAFFS_MAX_SHORT_OP_CACHES) {
			dev->nShortOpCaches = YAFFS_MAX_SHORT_OP_CACHES;
		}

		dev->srCache =
		    YMALLOC(dev->nShortOpCaches * sizeof(yaffs_ChunkCache));

		for (i = 0; i < dev->nShortOpCaches; i++) {
			dev->srCache[i].object = NULL;
			dev->srCache[i].lastUse = 0;
			dev->srCache[i].dirty = 0;
			dev->srCache[i].data = YMALLOC_DMA(dev->nDataBytesPerChunk);
		}
		dev->srLastUse = 0;
	}

	dev->cacheHits = 0;
	
	dev->gcCleanupList = YMALLOC(dev->nChunksPerBlock * sizeof(__u32));

	if (dev->isYaffs2) {
		dev->useHeaderFileSize = 1;
	}

	yaffs_InitialiseBlocks(dev);
	yaffs_InitialiseTnodes(dev);
	yaffs_InitialiseObjects(dev);

	yaffs_CreateInitialDirectories(dev);


	
	if (dev->isYaffs2) {
		if(yaffs_CheckpointRestore(dev)) {
			T(YAFFS_TRACE_ALWAYS,
			  (TSTR("yaffs: restored from checkpoint" TENDSTR)));
		} else {

			yaffs_DeinitialiseBlocks(dev);
			yaffs_DeinitialiseTnodes(dev);
			yaffs_DeinitialiseObjects(dev);
			yaffs_InitialiseBlocks(dev);
			yaffs_InitialiseTnodes(dev);
			yaffs_InitialiseObjects(dev);
			yaffs_CreateInitialDirectories(dev);

			yaffs_ScanBackwards(dev);
		}
	}else
		yaffs_Scan(dev);

	
	dev->nPageReads = 0;
	dev->nPageWrites = 0;
	dev->nBlockErasures = 0;
	dev->nGCCopies = 0;
	dev->nRetriedWrites = 0;

	dev->nRetiredBlocks = 0;

	yaffs_VerifyFreeChunks(dev);

	T(YAFFS_TRACE_TRACING,
	  (TSTR("yaffs: yaffs_GutsInitialise() done.\n" TENDSTR)));
	return YAFFS_OK;

}

void yaffs_Deinitialise(yaffs_Device * dev)
{
	if (dev->isMounted) {
		int i;

		yaffs_DeinitialiseBlocks(dev);
		yaffs_DeinitialiseTnodes(dev);
		yaffs_DeinitialiseObjects(dev);
		if (dev->nShortOpCaches > 0) {

			for (i = 0; i < dev->nShortOpCaches; i++) {
				YFREE(dev->srCache[i].data);
			}

			YFREE(dev->srCache);
		}

		YFREE(dev->gcCleanupList);

		for (i = 0; i < YAFFS_N_TEMP_BUFFERS; i++) {
			YFREE(dev->tempBuffer[i].buffer);
		}

		dev->isMounted = 0;
	}

}

static int yaffs_CountFreeChunks(yaffs_Device * dev)
{
	int nFree;
	int b;

	yaffs_BlockInfo *blk;

	for (nFree = 0, b = dev->internalStartBlock; b <= dev->internalEndBlock;
	     b++) {
		blk = yaffs_GetBlockInfo(dev, b);

		switch (blk->blockState) {
		case YAFFS_BLOCK_STATE_EMPTY:
		case YAFFS_BLOCK_STATE_ALLOCATING:
		case YAFFS_BLOCK_STATE_COLLECTING:
		case YAFFS_BLOCK_STATE_FULL:
			nFree +=
			    (dev->nChunksPerBlock - blk->pagesInUse +
			     blk->softDeletions);
			break;
		default:
			break;
		}

	}

	return nFree;
}

int yaffs_GetNumberOfFreeChunks(yaffs_Device * dev)
{
	

	int nFree;
	int nDirtyCacheChunks;
	int blocksForCheckpoint;

#if 1
	nFree = dev->nFreeChunks;
#else
	nFree = yaffs_CountFreeChunks(dev);
#endif

	nFree += dev->nDeletedFiles;
	
	

	{
		int i;
		for (nDirtyCacheChunks = 0, i = 0; i < dev->nShortOpCaches; i++) {
			if (dev->srCache[i].dirty)
				nDirtyCacheChunks++;
		}
	}

	nFree -= nDirtyCacheChunks;

	nFree -= ((dev->nReservedBlocks + 1) * dev->nChunksPerBlock);
	
	
	blocksForCheckpoint = dev->nCheckpointReservedBlocks - dev->blocksInCheckpoint;
	if(blocksForCheckpoint < 0)
		blocksForCheckpoint = 0;
		
	nFree -= (blocksForCheckpoint * dev->nChunksPerBlock);

	if (nFree < 0)
		nFree = 0;

	return nFree;

}

static int yaffs_freeVerificationFailures;

static void yaffs_VerifyFreeChunks(yaffs_Device * dev)
{
	int counted = yaffs_CountFreeChunks(dev);

	int difference = dev->nFreeChunks - counted;

	if (difference) {
		T(YAFFS_TRACE_ALWAYS,
		  (TSTR("Freechunks verification failure %d %d %d" TENDSTR),
		   dev->nFreeChunks, counted, difference));
		yaffs_freeVerificationFailures++;
	}
}


#define yaffs_CheckStruct(structure,syze, name) \
           if(sizeof(structure) != syze) \
	       { \
	         T(YAFFS_TRACE_ALWAYS,(TSTR("%s should be %d but is %d\n" TENDSTR),\
		 name,syze,sizeof(structure))); \
	         return YAFFS_FAIL; \
		}

static int yaffs_CheckStructures(void)
{
#ifndef CONFIG_YAFFS_TNODE_LIST_DEBUG
	yaffs_CheckStruct(yaffs_Tnode, 2 * YAFFS_NTNODES_LEVEL0, "yaffs_Tnode")
#endif
	    yaffs_CheckStruct(yaffs_ObjectHeader, 512, "yaffs_ObjectHeader")

	    return YAFFS_OK;
}
