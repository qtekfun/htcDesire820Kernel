/* Updated 5/03/00    - Added copyright and placed into PD. No code changes   */

/* This file is Copyright (C) Matt Slot, 1999-2012. It is hereby placed into 
   the public domain. The author makes no warranty as to fitness or stability */

#include <Gestalt.h>
#include <LowMem.h>
#include <CodeFragments.h>
#include <DriverServices.h>
#include <Timer.h>

#include "FastTimes.h"

#ifdef TARGET_CPU_PPC
#undef GENERATINGPOWERPC 
#define GENERATINGPOWERPC TARGET_CPU_PPC
#endif


#define RTCToNano(w)	((double) (w).hi * 1000000000.0 + (double) (w).lo)
#define WideTo64bit(w)	(*(UInt64 *) &(w))

#define MyLMGetTicks()	(*(volatile UInt32 *) 0x16A)

#if GENERATINGPOWERPC

static asm UnsignedWide PollRTC(void);
static asm UnsignedWide PollTBR(void);
static Ptr FindFunctionInSharedLib(StringPtr libName, StringPtr funcName);

static Boolean			gInited = false;
static Boolean			gNative = false;
static Boolean			gUseRTC = false;
static Boolean			gUseTBR = false;
static double			gScaleUSec = 1.0 / 1000.0;    
static double			gScaleMSec = 1.0 / 1000000.0; 

typedef AbsoluteTime 	(*UpTimeProcPtr)(void);
typedef Nanoseconds 	(*A2NSProcPtr)(AbsoluteTime);
static UpTimeProcPtr 	gUpTime = NULL;
static A2NSProcPtr 		gA2NS = NULL;

#endif 


void FastInitialize() {
	SInt32			result;

	if (!gInited) {

#if GENERATINGPOWERPC

		
		gNative = gUseRTC = gUseTBR = false;

		gUpTime = (UpTimeProcPtr) FindFunctionInSharedLib(
				"\pDriverServicesLib", "\pUpTime");
		if (gUpTime) gA2NS = (A2NSProcPtr) FindFunctionInSharedLib(
				"\pDriverServicesLib", "\pAbsoluteToNanoseconds");
		if (!gA2NS) gUpTime = nil; 

		if (gUpTime) {

			
			if (!Gestalt(gestaltTimeMgrVersion, &result) &&
					(result > gestaltExtendedTimeMgr)) 
				gNative = true;
			}
		  else {

			
		 	if (!Gestalt(gestaltNativeCPUtype, &result)) {
				if (result == gestaltCPU601) gUseRTC = true;
				  else if (result > gestaltCPU601) gUseTBR = true;
				}
			}

		
		if ((gUpTime && !gNative) || gUseRTC || gUseTBR) {
			UInt64			tick, usec1, usec2;
			UnsignedWide	wide;

			
			for(tick = MyLMGetTicks() + 1; tick > MyLMGetTicks(); );
			
			
			wide = (gUpTime) ? (*gA2NS)((*gUpTime)()) : 
					((gUseRTC) ? PollRTC() : PollTBR());
			usec1 = (gUseRTC) ? RTCToNano(wide) : WideTo64bit(wide);
			
			
			while(tick + 60 > MyLMGetTicks());

			
			wide = (gUpTime) ? (*gA2NS)((*gUpTime)()) : 
					((gUseRTC) ? PollRTC() : PollTBR());
			usec2 = (gUseRTC) ? RTCToNano(wide) : WideTo64bit(wide);
			
			gScaleUSec = (60.0 * 1000000.0) / ((usec2 - usec1) * 60.15);
			gScaleMSec = gScaleUSec / 1000.0;
			}

#endif 

		
		gInited = true;
		}
	}


UInt64 FastMicroseconds() {
	UnsignedWide	wide;
	UInt64			usec;
	
#if GENERATINGPOWERPC
	
	if (!gInited) FastInitialize();
	
	if (gNative) {
		
		wide = (*gA2NS)((*gUpTime)());
		usec = (double) WideTo64bit(wide) * gScaleUSec + 0.5;
		}
	  else if (gUpTime) {
		
		wide = (*gA2NS)((*gUpTime)());
		usec = (double) WideTo64bit(wide) * gScaleUSec + 0.5;
		}
	  else if (gUseTBR) {
		
		wide = PollTBR();
		usec = (double) WideTo64bit(wide) * gScaleUSec + 0.5;
		}
	  else if (gUseRTC) {
		
		wide = PollRTC();
		usec = (double) RTCToNano(wide) * gScaleUSec + 0.5;
		}
	  else 
#endif 
		{
		
		Microseconds(&wide);
		usec = WideTo64bit(wide);
		}

	return(usec);
	}


UInt64 FastMilliseconds() {
	UnsignedWide	wide;
	UInt64			msec;	
	
#if GENERATINGPOWERPC
	
	if (!gInited) FastInitialize();
	
	if (gNative) {
		
		wide = (*gA2NS)((*gUpTime)());
		msec = (double) WideTo64bit(wide) * gScaleMSec + 0.5;
		}
	  else if (gUpTime) {
		
		wide = (*gA2NS)((*gUpTime)());
		msec = (double) WideTo64bit(wide) * gScaleMSec + 0.5;
		}
	  else if (gUseTBR) {
		
		wide = PollTBR();
		msec = (double) WideTo64bit(wide) * gScaleMSec + 0.5;
		}
	  else if (gUseRTC) {
		
		wide = PollRTC();
		msec = (double) RTCToNano(wide) * gScaleMSec + 0.5;
		}
	  else 
#endif 
		{
		
		Microseconds(&wide);
		msec = ((double) WideTo64bit(wide) + 500.0) / 1000.0;
		}

	return(msec);
	}


StringPtr FastMethod() {
	StringPtr	method = "\p<Unknown>";

#if GENERATINGPOWERPC
	
	if (!gInited) FastInitialize();
	
	if (gNative) {
		
		method = "\pNative UpTime()";
		}
	  else if (gUpTime) {
		
		method = "\pUpTime()";
		}
	  else if (gUseTBR) {
		
		method = "\pPowerPC TBR";
		}
	  else if (gUseRTC) {
		
		method = "\pPowerPC RTC";
		}
	  else 
#endif 
		{
		
		method = "\pMicroseconds()";
		}

	return(method);
	}

#pragma mark -

#if GENERATINGPOWERPC
asm static UnsignedWide PollRTC_() {
entry PollRTC 
	machine 601
@AGAIN:
	mfrtcu	r4 
	mfrtcl	r5 
	mfrtcu	r6
	cmpw	r4,r6
	bne		@AGAIN
	stw		r4,0(r3)
	stw		r5,4(r3)
	blr
	}


asm static UnsignedWide PollTBR_() {
entry PollTBR 
	machine 604
@AGAIN:
	mftbu	r4 
	mftb	r5 
	mftbu	r6
	cmpw	r4,r6
	bne		@AGAIN
	stw		r4,0(r3)
	stw		r5,4(r3)
	blr
	}


static Ptr FindFunctionInSharedLib(StringPtr libName, StringPtr funcName) {
	OSErr				error = noErr;
	Str255				errorStr;
	Ptr					func = NULL;
	Ptr					entry = NULL;
	CFragSymbolClass	symClass;
	CFragConnectionID	connID;
	
	
	if ( GetSharedLibrary(libName, kCompiledCFragArch,
			kLoadCFrag, &connID, &entry, errorStr)) return(NULL);
	if ( FindSymbol(connID, funcName, &func, &symClass))
		return(NULL);
	
	return(func);
	}
#endif 
