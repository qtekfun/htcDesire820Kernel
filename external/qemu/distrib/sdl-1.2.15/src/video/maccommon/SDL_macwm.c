/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <Carbon/Carbon.h>
#elif TARGET_API_MAC_CARBON && (UNIVERSAL_INTERFACES_VERSION > 0x0335)
#include <Carbon.h>
#else
#include <Windows.h>
#include <Strings.h>
#endif

#if SDL_MACCLASSIC_GAMMA_SUPPORT
#include <Devices.h>
#include <Files.h>
#include <MacTypes.h>
#include <QDOffscreen.h>
#include <Quickdraw.h>
#include <Video.h>
#endif

#include "SDL_stdinc.h"
#include "SDL_macwm_c.h"

void Mac_SetCaption(_THIS, const char *title, const char *icon)
{
	
	Str255		ptitle; 
	ptitle[0] = strlen (title);
	SDL_memcpy(ptitle+1, title, ptitle[0]); 
	if (SDL_Window)
		SetWTitle(SDL_Window, ptitle); 
}

#if SDL_MACCLASSIC_GAMMA_SUPPORT
/*
 * ADC Gamma Ramp support...
 *
 * Mac Gamma Ramp code was originally from sample code provided by
 *  Apple Developer Connection, and not written specifically for SDL:
 * "Contains: Functions to enable Mac OS device gamma adjustments using 3 channel 256 element 8 bit gamma ramps
 *  Written by: Geoff Stahl (ggs)
 *  Copyright: Copyright (c) 1999 Apple Computer, Inc., All Rights Reserved
 *  Disclaimer: You may incorporate this sample code into your applications without
 *              restriction, though the sample code has been provided "AS IS" and the
 *              responsibility for its operation is 100% yours.  However, what you are
 *              not permitted to do is to redistribute the source as "DSC Sample Code"
 *              after having made changes. If you're going to re-distribute the source,
 *              we require that you make it clear in the source that the code was
 *              descended from Apple Sample Code, but that you've made changes."
 * (The sample code has been integrated into this file, and thus is modified from the original Apple sources.)
 */

typedef struct recDeviceGamma											
{
	GDHandle hGD;												
	GammaTblPtr pDeviceGamma;									
} recDeviceGamma;
typedef recDeviceGamma * precDeviceGamma;

typedef struct recSystemGamma											
{
	short numDevices;											
	precDeviceGamma * devGamma;									
} recSystemGamma;
typedef recSystemGamma * precSystemGamma;

static Ptr CopyGammaTable (GammaTblPtr pTableGammaIn)
{
	GammaTblPtr		pTableGammaOut = NULL;
	short			tableSize, dataWidth;

	if (pTableGammaIn)												
	{
		dataWidth = (pTableGammaIn->gDataWidth + 7) / 8;			
		tableSize = sizeof (GammaTbl) + pTableGammaIn->gFormulaSize +
					(pTableGammaIn->gChanCnt * pTableGammaIn->gDataCnt * dataWidth);
		pTableGammaOut = (GammaTblPtr) NewPtr (tableSize);			
		if (pTableGammaOut)											
			BlockMove( (Ptr)pTableGammaIn, (Ptr)pTableGammaOut, tableSize);	
	}
	return (Ptr)pTableGammaOut;										
}

static OSErr GetGammaTable (GDHandle hGD, GammaTblPtr * ppTableGammaOut)
{
	VDGammaRecord   DeviceGammaRec;
	CntrlParam		cParam;
	OSErr			err;
	
	cParam.ioCompletion = NULL;										
	cParam.ioNamePtr = NULL;
	cParam.ioVRefNum = 0;
	cParam.ioCRefNum = (**hGD).gdRefNum;
	cParam.csCode = cscGetGamma;									
	*(Ptr *)cParam.csParam = (Ptr) &DeviceGammaRec;					

	err = PBStatusSync( (ParmBlkPtr)&cParam );						
	
	*ppTableGammaOut = (GammaTblPtr)(DeviceGammaRec.csGTable);		
	
	return err;	
}

static Ptr GetDeviceGamma (GDHandle hGD)
{
	GammaTblPtr		pTableGammaDevice = NULL;
	GammaTblPtr		pTableGammaReturn = NULL;	
	OSErr			err;
	
	err = GetGammaTable (hGD, &pTableGammaDevice);					
	if ((noErr == err) && pTableGammaDevice)						
		pTableGammaReturn = (GammaTblPtr) CopyGammaTable (pTableGammaDevice); 

	return (Ptr) pTableGammaReturn;
}

static void DisposeGammaTable (Ptr pGamma)
{
	if (pGamma)
		DisposePtr((Ptr) pGamma);									
}

static void DisposeSystemGammas (Ptr* ppSystemGammas)
{
	precSystemGamma pSysGammaIn;
	if (ppSystemGammas)
	{
		pSysGammaIn = (precSystemGamma) *ppSystemGammas;
		if (pSysGammaIn)
		{
			short i;
			for (i = 0; i < pSysGammaIn->numDevices; i++)		
				if (pSysGammaIn->devGamma [i])						
				{
					DisposeGammaTable ((Ptr) pSysGammaIn->devGamma [i]->pDeviceGamma); 
					DisposePtr ((Ptr) pSysGammaIn->devGamma [i]);					   
				}
			DisposePtr ((Ptr) pSysGammaIn->devGamma);				
			DisposePtr ((Ptr) pSysGammaIn);							
			*ppSystemGammas = NULL;
		}	
	}
}

static Boolean GetDeviceGammaRampGD (GDHandle hGD, Ptr pRamp)
{
	GammaTblPtr		pTableGammaTemp = NULL;
	long 			indexChan, indexEntry;
	OSErr			err;
	
	if (pRamp)															
	{
		err = GetGammaTable (hGD, &pTableGammaTemp);					
		if ((noErr == err) && pTableGammaTemp)							
		{															
			
			unsigned char * pEntry = (unsigned char *) &pTableGammaTemp->gFormulaData + pTableGammaTemp->gFormulaSize; 
			short bytesPerEntry = (pTableGammaTemp->gDataWidth + 7) / 8; 
			short shiftRightValue = pTableGammaTemp->gDataWidth - 8; 	 
			short channels = pTableGammaTemp->gChanCnt;	
			short entries = pTableGammaTemp->gDataCnt;									
			if (3 == channels)											
			{															
				for (indexChan = 0; indexChan < channels; indexChan++)
					for (indexEntry = 0; indexEntry < 256; indexEntry++)
						*((unsigned char *) pRamp + (indexChan * 256) + indexEntry) = 
						  *(pEntry + indexChan * entries * bytesPerEntry + indexEntry * entries * bytesPerEntry / 256) >> shiftRightValue;
			}
			else														
			{
				for (indexChan = 0; indexChan < 768; indexChan += 256)	
					for (indexEntry = 0; indexEntry < 256; indexEntry++) 
						*((unsigned char *) pRamp + indexChan + indexEntry) = 
						  *(pEntry + indexEntry * entries * bytesPerEntry / 256) >> shiftRightValue;
			}
			return true;
		}
	}
	return false;
}

static Ptr GetSystemGammas (void)
{
	precSystemGamma pSysGammaOut;									
	short devCount = 0;												
	Boolean fail = false;
	GDHandle hGDevice;
	
	pSysGammaOut = (precSystemGamma) NewPtr (sizeof (recSystemGamma)); 
	
	hGDevice = GetDeviceList ();							
	do																
	{
		devCount++;													
		hGDevice = GetNextDevice (hGDevice);						
	} while (hGDevice);
	
	pSysGammaOut->devGamma = (precDeviceGamma *) NewPtr (sizeof (precDeviceGamma) * devCount); 
	if (pSysGammaOut)
	{
		pSysGammaOut->numDevices = devCount;						
		
		devCount = 0;												
		hGDevice = GetDeviceList ();
		do
		{
			pSysGammaOut->devGamma [devCount] = (precDeviceGamma) NewPtr (sizeof (recDeviceGamma));	  
			if (pSysGammaOut->devGamma [devCount])					
			{
				pSysGammaOut->devGamma [devCount]->hGD = hGDevice;										  
				pSysGammaOut->devGamma [devCount]->pDeviceGamma = (GammaTblPtr)GetDeviceGamma (hGDevice); 
			}
			else													
			 fail = true;
			devCount++;												
			hGDevice = GetNextDevice (hGDevice);						
		} while (hGDevice);
	}
	if (!fail)														
		return (Ptr) pSysGammaOut;									
	else
	{
		DisposeSystemGammas ((Ptr *) &pSysGammaOut);					
		return NULL;												
	}
}

static void RestoreDeviceGamma (GDHandle hGD, Ptr pGammaTable)
{
	VDSetEntryRecord setEntriesRec;
	VDGammaRecord	gameRecRestore;
	CTabHandle      hCTabDeviceColors;
	Ptr				csPtr;
	OSErr			err = noErr;
	
	if (pGammaTable)												
	{
		gameRecRestore.csGTable = pGammaTable;						
		csPtr = (Ptr) &gameRecRestore;
		err = Control((**hGD).gdRefNum, cscSetGamma, (Ptr) &csPtr);	

		if ((noErr == err) && (8 == (**(**hGD).gdPMap).pixelSize))	
		{
			hCTabDeviceColors = (**(**hGD).gdPMap).pmTable;			
			setEntriesRec.csTable = (ColorSpec *) &(**hCTabDeviceColors).ctTable;
			setEntriesRec.csStart = 0;
			setEntriesRec.csCount = (**hCTabDeviceColors).ctSize;
			csPtr = (Ptr) &setEntriesRec;
			
			err = Control((**hGD).gdRefNum, cscSetEntries, (Ptr) &csPtr); 
		}
	}
}

static void RestoreSystemGammas (Ptr pSystemGammas)
{
	short i;
	precSystemGamma pSysGammaIn = (precSystemGamma) pSystemGammas;
	if (pSysGammaIn)
		for (i = 0; i < pSysGammaIn->numDevices; i++)			
			RestoreDeviceGamma (pSysGammaIn->devGamma [i]->hGD, (Ptr) pSysGammaIn->devGamma [i]->pDeviceGamma);	
}

static Ptr CreateEmptyGammaTable (short channels, short entries, short bits)
{
	GammaTblPtr		pTableGammaOut = NULL;
	short			tableSize, dataWidth;

	dataWidth = (bits + 7) / 8;										
	tableSize = sizeof (GammaTbl) + (channels * entries * dataWidth);
	pTableGammaOut = (GammaTblPtr) NewPtrClear (tableSize);			

	if (pTableGammaOut)												
	{
		pTableGammaOut->gVersion = 0;								
		pTableGammaOut->gType = 0;
		pTableGammaOut->gFormulaSize = 0;
		pTableGammaOut->gChanCnt = channels;
		pTableGammaOut->gDataCnt = entries;
		pTableGammaOut->gDataWidth = bits;
	}
	return (Ptr)pTableGammaOut;										
}

static Boolean SetDeviceGammaRampGD (GDHandle hGD, Ptr pRamp)
{
	VDSetEntryRecord setEntriesRec;
	VDGammaRecord	gameRecRestore;
	GammaTblPtr		pTableGammaNew;
	GammaTblPtr		pTableGammaCurrent = NULL;
	CTabHandle      hCTabDeviceColors;
	Ptr				csPtr;
	OSErr			err;
	short 			dataBits, entries, channels = 3;						
	
	if (pRamp)																
	{
		err= GetGammaTable (hGD, &pTableGammaCurrent);						
		if ((noErr == err) && pTableGammaCurrent)
		{
			dataBits = pTableGammaCurrent->gDataWidth;						
			entries = pTableGammaCurrent->gDataCnt;							
			pTableGammaNew = (GammaTblPtr) CreateEmptyGammaTable (channels, entries, dataBits); 
			if (pTableGammaNew)												
			{	
				unsigned char * pGammaBase = (unsigned char *) &pTableGammaNew->gFormulaData + pTableGammaNew->gFormulaSize; 
				if ((256 == entries) && (8 == dataBits)) 						
					BlockMove ((Ptr)pRamp, (Ptr)pGammaBase, channels * entries); 
				else														
				{
					short indexChan, indexEntry;
					short bytesPerEntry = (dataBits + 7) / 8; 				
					short shiftRightValue = 8 - dataBits;					
					shiftRightValue += ((bytesPerEntry - 1) * 8);  			
					for (indexChan = 0; indexChan < channels; indexChan++) 
						for (indexEntry = 0; indexEntry < entries; indexEntry++) 
						{
							short currentShift = shiftRightValue;			
							long temp = *((unsigned char *)pRamp + (indexChan << 8) + (indexEntry << 8) / entries); 
							short indexByte;
							for (indexByte = 0; indexByte < bytesPerEntry; indexByte++) 
							{
								if (currentShift < 0)						
									*(pGammaBase++) = temp << -currentShift;
								else
									*(pGammaBase++) = temp >> currentShift;
								currentShift -= 8;							
							}
						}
				}
				
				
				gameRecRestore.csGTable = (Ptr) pTableGammaNew;				
				csPtr = (Ptr) &gameRecRestore;
				err = Control((**hGD).gdRefNum, cscSetGamma, (Ptr) &csPtr);	
				
				if ((8 == (**(**hGD).gdPMap).pixelSize) && (noErr == err))	
				{
					hCTabDeviceColors = (**(**hGD).gdPMap).pmTable;			
					setEntriesRec.csTable = (ColorSpec *) &(**hCTabDeviceColors).ctTable;
					setEntriesRec.csStart = 0;
					setEntriesRec.csCount = (**hCTabDeviceColors).ctSize;
					csPtr = (Ptr) &setEntriesRec;
					err = Control((**hGD).gdRefNum, cscSetEntries, (Ptr) &csPtr);	
				}
				DisposeGammaTable ((Ptr) pTableGammaNew);					
				if (noErr == err)
					return true;
			}
		}
	}
	else																	
	{
		gameRecRestore.csGTable = (Ptr) NULL;								
		csPtr = (Ptr) &gameRecRestore;
		err = Control((**hGD).gdRefNum, cscSetGamma, (Ptr) &csPtr);			
		
		if ((8 == (**(**hGD).gdPMap).pixelSize) && (noErr == err))			
		{
			hCTabDeviceColors = (**(**hGD).gdPMap).pmTable;					
			setEntriesRec.csTable = (ColorSpec *) &(**hCTabDeviceColors).ctTable;
			setEntriesRec.csStart = 0;
			setEntriesRec.csCount = (**hCTabDeviceColors).ctSize;
			csPtr = (Ptr) &setEntriesRec;
			err = Control((**hGD).gdRefNum, cscSetEntries, (Ptr) &csPtr);	
		}
		if (noErr == err)
			return true;
	}
	return false;															
}


static Ptr systemGammaPtr;

void Mac_QuitGamma(_THIS)
{
	if (systemGammaPtr)
	{
		RestoreSystemGammas(systemGammaPtr);
		DisposeSystemGammas(&systemGammaPtr);
	}
}

static unsigned char shiftedRamp[3 * 256];

int Mac_SetGammaRamp(_THIS, Uint16 *ramp)
{
	int i;
	if (!systemGammaPtr)
		systemGammaPtr = GetSystemGammas();
	for (i = 0; i < 3 * 256; i++)
	{
		shiftedRamp[i] = ramp[i] >> 8;
	}

	if (SetDeviceGammaRampGD(GetMainDevice(), (Ptr) shiftedRamp))
		return 0;
	else
		return -1;
}

int Mac_GetGammaRamp(_THIS, Uint16 *ramp)
{
	if (GetDeviceGammaRampGD(GetMainDevice(), (Ptr) shiftedRamp))
	{
		int i;
		for (i = 0; i < 3 * 256; i++)
		{
			ramp[i] = shiftedRamp[i] << 8;
		}
		return 0;
	}
	else
		return -1;
}

#endif  


