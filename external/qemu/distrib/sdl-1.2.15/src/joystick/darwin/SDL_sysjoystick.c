/*
	SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library; if not, write to the Free
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	Sam Lantinga
	slouken@libsdl.org
*/
#include "SDL_config.h"

#ifdef SDL_JOYSTICK_IOKIT

/* Written 2001 by Max Horn */

#include <unistd.h>
#include <ctype.h>
#include <sysexits.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOCFPlugIn.h>
#ifdef MACOS_10_0_4
#include <IOKit/hidsystem/IOHIDUsageTables.h>
#else
#include <Kernel/IOKit/hidsystem/IOHIDUsageTables.h>
#endif
#include <IOKit/hid/IOHIDLib.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h> 

#include "SDL_joystick.h"
#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"

struct recElement
{
	IOHIDElementCookie cookie;				
	long min;								
	long max;								
#if 0
	

	long scaledMin;							
	long scaledMax;							
	long size;								
	Boolean relative;						
	Boolean wrapping;						
	Boolean nonLinear;						
	Boolean preferredState;					
	Boolean nullState;						
#endif 

	
	long minReport;							
	long maxReport;							
	
	struct recElement * pNext;				
};
typedef struct recElement recElement;

struct joystick_hwdata
{
	IOHIDDeviceInterface ** interface;		

	char product[256];							
	long usage;								
	long usagePage;							

	long axes;								
	long buttons;							
	long hats;								
	long elements;							

	recElement* firstAxis;
	recElement* firstButton;
	recElement* firstHat;

	int removed;
	int uncentered;

	struct joystick_hwdata* pNext;			
};
typedef struct joystick_hwdata recDevice;


static recDevice *gpDeviceList = NULL;


static void HIDReportErrorNum (char * strError, long numError)
{
	SDL_SetError(strError);
}

static void HIDGetCollectionElements (CFMutableDictionaryRef deviceProperties, recDevice *pDevice);


static SInt32 HIDGetElementValue (recDevice *pDevice, recElement *pElement)
{
	IOReturn result = kIOReturnSuccess;
	IOHIDEventStruct hidEvent;
	hidEvent.value = 0;
	
	if (NULL != pDevice && NULL != pElement && NULL != pDevice->interface)
	{
		result = (*(pDevice->interface))->getElementValue(pDevice->interface, pElement->cookie, &hidEvent);
		if (kIOReturnSuccess == result)
		{
			
			if (hidEvent.value < pElement->minReport)
				pElement->minReport = hidEvent.value;
			if (hidEvent.value > pElement->maxReport)
				pElement->maxReport = hidEvent.value;
		}
	}

	
	return hidEvent.value;
}

static SInt32 HIDScaledCalibratedValue (recDevice *pDevice, recElement *pElement, long min, long max)
{
	float deviceScale = max - min;
	float readScale = pElement->maxReport - pElement->minReport;
	SInt32 value = HIDGetElementValue(pDevice, pElement);
	if (readScale == 0)
		return value; 
	else
		return ((value - pElement->minReport) * deviceScale / readScale) + min;
}


static void HIDRemovalCallback(void * target,
                               IOReturn result,
                               void * refcon,
                               void * sender)
{
	recDevice *device = (recDevice *) refcon;
	device->removed = 1;
	device->uncentered = 1;
}




static IOReturn HIDCreateOpenDeviceInterface (io_object_t hidDevice, recDevice *pDevice)
{
	IOReturn result = kIOReturnSuccess;
	HRESULT plugInResult = S_OK;
	SInt32 score = 0;
	IOCFPlugInInterface ** ppPlugInInterface = NULL;
	
	if (NULL == pDevice->interface)
	{
		result = IOCreatePlugInInterfaceForService (hidDevice, kIOHIDDeviceUserClientTypeID,
													kIOCFPlugInInterfaceID, &ppPlugInInterface, &score);
		if (kIOReturnSuccess == result)
		{
			
			plugInResult = (*ppPlugInInterface)->QueryInterface (ppPlugInInterface,
								CFUUIDGetUUIDBytes (kIOHIDDeviceInterfaceID), (void *) &(pDevice->interface));
			if (S_OK != plugInResult)
				HIDReportErrorNum ("CouldnÕt query HID class device interface from plugInInterface", plugInResult);
			(*ppPlugInInterface)->Release (ppPlugInInterface);
		}
		else
			HIDReportErrorNum ("Failed to create **plugInInterface via IOCreatePlugInInterfaceForService.", result);
	}
	if (NULL != pDevice->interface)
	{
		result = (*(pDevice->interface))->open (pDevice->interface, 0);
		if (kIOReturnSuccess != result)
			HIDReportErrorNum ("Failed to open pDevice->interface via open.", result);
		else
			(*(pDevice->interface))->setRemovalCallback (pDevice->interface, HIDRemovalCallback, pDevice, pDevice);

	}
	return result;
}


static IOReturn HIDCloseReleaseInterface (recDevice *pDevice)
{
	IOReturn result = kIOReturnSuccess;
	
	if ((NULL != pDevice) && (NULL != pDevice->interface))
	{
		
		result = (*(pDevice->interface))->close (pDevice->interface);
		if (kIOReturnNotOpen == result)
		{
			
		}
		else if (kIOReturnSuccess != result)
			HIDReportErrorNum ("Failed to close IOHIDDeviceInterface.", result);
		
		result = (*(pDevice->interface))->Release (pDevice->interface);
		if (kIOReturnSuccess != result)
			HIDReportErrorNum ("Failed to release IOHIDDeviceInterface.", result);
		pDevice->interface = NULL;
	}	
	return result;
}


static void HIDGetElementInfo (CFTypeRef refElement, recElement *pElement)
{
	long number;
	CFTypeRef refType;

	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementCookieKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->cookie = (IOHIDElementCookie) number;
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementMinKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->minReport = pElement->min = number;
	refType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementMaxKey));
	if (refType && CFNumberGetValue (refType, kCFNumberLongType, &number))
		pElement->maxReport = pElement->max = number;
}			


static void HIDAddElement (CFTypeRef refElement, recDevice* pDevice)
{
	recElement* element = NULL;
	recElement** headElement = NULL;
	long elementType, usagePage, usage;
	CFTypeRef refElementType = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementTypeKey));
	CFTypeRef refUsagePage = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementUsagePageKey));
	CFTypeRef refUsage = CFDictionaryGetValue (refElement, CFSTR(kIOHIDElementUsageKey));


	if ((refElementType) && (CFNumberGetValue (refElementType, kCFNumberLongType, &elementType)))
	{
		
		if ((elementType == kIOHIDElementTypeInput_Misc) || (elementType == kIOHIDElementTypeInput_Button) ||
			(elementType == kIOHIDElementTypeInput_Axis))
		{
			if (refUsagePage && CFNumberGetValue (refUsagePage, kCFNumberLongType, &usagePage) &&
				refUsage && CFNumberGetValue (refUsage, kCFNumberLongType, &usage))
			{
				switch (usagePage) 
				{
					case kHIDPage_GenericDesktop:
						{
							switch (usage) 
							{
								case kHIDUsage_GD_X:
								case kHIDUsage_GD_Y:
								case kHIDUsage_GD_Z:
								case kHIDUsage_GD_Rx:
								case kHIDUsage_GD_Ry:
								case kHIDUsage_GD_Rz:
								case kHIDUsage_GD_Slider:
								case kHIDUsage_GD_Dial:
								case kHIDUsage_GD_Wheel:
									element = (recElement *) NewPtrClear (sizeof (recElement));
									if (element)
									{
										pDevice->axes++;
										headElement = &(pDevice->firstAxis);
									}
								break;
								case kHIDUsage_GD_Hatswitch:
									element = (recElement *) NewPtrClear (sizeof (recElement));
									if (element)
									{
										pDevice->hats++;
										headElement = &(pDevice->firstHat);
									}
								break;
							}							
						}
						break;
					case kHIDPage_Button:
						element = (recElement *) NewPtrClear (sizeof (recElement));
						if (element)
						{
							pDevice->buttons++;
							headElement = &(pDevice->firstButton);
						}
						break;
					default:
						break;
				}
			}
		}
		else if (kIOHIDElementTypeCollection == elementType)
			HIDGetCollectionElements ((CFMutableDictionaryRef) refElement, pDevice);
	}

	if (element && headElement) 
	{
		pDevice->elements++;
		if (NULL == *headElement)
			*headElement = element;
		else
		{
			recElement *elementPrevious, *elementCurrent;
			elementCurrent = *headElement;
			while (elementCurrent)
			{
				elementPrevious = elementCurrent;
				elementCurrent = elementPrevious->pNext;
			}
			elementPrevious->pNext = element;
		}
		element->pNext = NULL;
		HIDGetElementInfo (refElement, element);
	}
}


static void HIDGetElementsCFArrayHandler (const void * value, void * parameter)
{
	if (CFGetTypeID (value) == CFDictionaryGetTypeID ())
		HIDAddElement ((CFTypeRef) value, (recDevice *) parameter);
}


static void HIDGetElements (CFTypeRef refElementCurrent, recDevice *pDevice)
{
	CFTypeID type = CFGetTypeID (refElementCurrent);
	if (type == CFArrayGetTypeID()) 
	{
		CFRange range = {0, CFArrayGetCount (refElementCurrent)};
		
		CFArrayApplyFunction (refElementCurrent, range, HIDGetElementsCFArrayHandler, pDevice);
	}
}			


static void HIDGetCollectionElements (CFMutableDictionaryRef deviceProperties, recDevice *pDevice)
{
	CFTypeRef refElementTop = CFDictionaryGetValue (deviceProperties, CFSTR(kIOHIDElementKey));
	if (refElementTop)
		HIDGetElements (refElementTop, pDevice);
}


static void HIDTopLevelElementHandler (const void * value, void * parameter)
{
	CFTypeRef refCF = 0;
	if (CFGetTypeID (value) != CFDictionaryGetTypeID ())
		return;
	refCF = CFDictionaryGetValue (value, CFSTR(kIOHIDElementUsagePageKey));
	if (!CFNumberGetValue (refCF, kCFNumberLongType, &((recDevice *) parameter)->usagePage))
		SDL_SetError ("CFNumberGetValue error retrieving pDevice->usagePage.");
	refCF = CFDictionaryGetValue (value, CFSTR(kIOHIDElementUsageKey));
	if (!CFNumberGetValue (refCF, kCFNumberLongType, &((recDevice *) parameter)->usage))
		SDL_SetError ("CFNumberGetValue error retrieving pDevice->usage.");
}


static void HIDGetDeviceInfo (io_object_t hidDevice, CFMutableDictionaryRef hidProperties, recDevice *pDevice)
{
	CFMutableDictionaryRef usbProperties = 0;
	io_registry_entry_t parent1, parent2;
	
	if ((KERN_SUCCESS == IORegistryEntryGetParentEntry (hidDevice, kIOServicePlane, &parent1)) &&
		(KERN_SUCCESS == IORegistryEntryGetParentEntry (parent1, kIOServicePlane, &parent2)) &&
		(KERN_SUCCESS == IORegistryEntryCreateCFProperties (parent2, &usbProperties, kCFAllocatorDefault, kNilOptions)))
	{
		if (usbProperties)
		{
			CFTypeRef refCF = 0;
			
			
			
			refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDProductKey));
			if (!refCF)
				refCF = CFDictionaryGetValue (usbProperties, CFSTR("USB Product Name"));
			if (refCF)
			{
				if (!CFStringGetCString (refCF, pDevice->product, 256, CFStringGetSystemEncoding ()))
					SDL_SetError ("CFStringGetCString error retrieving pDevice->product.");
			}
			
			
			refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDPrimaryUsagePageKey));
			if (refCF)
			{
				if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->usagePage))
					SDL_SetError ("CFNumberGetValue error retrieving pDevice->usagePage.");
				refCF = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDPrimaryUsageKey));
				if (refCF)
					if (!CFNumberGetValue (refCF, kCFNumberLongType, &pDevice->usage))
						SDL_SetError ("CFNumberGetValue error retrieving pDevice->usage.");
			}

			if (NULL == refCF) 
			{
				
				CFTypeRef refCFTopElement = 0;
				refCFTopElement = CFDictionaryGetValue (hidProperties, CFSTR(kIOHIDElementKey));
				{
					
					CFRange range = {0, CFArrayGetCount (refCFTopElement)};
					CFArrayApplyFunction (refCFTopElement, range, HIDTopLevelElementHandler, pDevice);
				}
			}

			CFRelease (usbProperties);
		}
		else
			SDL_SetError ("IORegistryEntryCreateCFProperties failed to create usbProperties.");

		if (kIOReturnSuccess != IOObjectRelease (parent2))
			SDL_SetError ("IOObjectRelease error with parent2.");
		if (kIOReturnSuccess != IOObjectRelease (parent1))
			SDL_SetError ("IOObjectRelease error with parent1.");
	}
}


static recDevice *HIDBuildDevice (io_object_t hidDevice)
{
	recDevice *pDevice = (recDevice *) NewPtrClear (sizeof (recDevice));
	if (pDevice)
	{
		
		CFMutableDictionaryRef hidProperties = 0;
		kern_return_t result = IORegistryEntryCreateCFProperties (hidDevice, &hidProperties, kCFAllocatorDefault, kNilOptions);
		if ((result == KERN_SUCCESS) && hidProperties)
		{
			
			result = HIDCreateOpenDeviceInterface (hidDevice, pDevice);
			if (kIOReturnSuccess == result)
			{
				HIDGetDeviceInfo (hidDevice, hidProperties, pDevice); 
				HIDGetCollectionElements (hidProperties, pDevice);
			}
			else
			{
				DisposePtr((Ptr)pDevice);
				pDevice = NULL;
			}
			CFRelease (hidProperties);
		}
		else
		{
			DisposePtr((Ptr)pDevice);
			pDevice = NULL;
		}
	}
	return pDevice;
}


static void HIDDisposeElementList (recElement **elementList)
{
	recElement *pElement = *elementList;
	while (pElement)
	{
		recElement *pElementNext = pElement->pNext;
		DisposePtr ((Ptr) pElement);
		pElement = pElementNext;
	}
	*elementList = NULL;
}


static recDevice *HIDDisposeDevice (recDevice **ppDevice)
{
	kern_return_t result = KERN_SUCCESS;
	recDevice *pDeviceNext = NULL;
	if (*ppDevice)
	{
		
		pDeviceNext = (*ppDevice)->pNext;
		
		
		HIDDisposeElementList (&(*ppDevice)->firstAxis);
		HIDDisposeElementList (&(*ppDevice)->firstButton);
		HIDDisposeElementList (&(*ppDevice)->firstHat);
		
		result = HIDCloseReleaseInterface (*ppDevice); 
		if (kIOReturnSuccess != result)
			HIDReportErrorNum ("HIDCloseReleaseInterface failed when trying to dipose device.", result);
		DisposePtr ((Ptr)*ppDevice);
		*ppDevice = NULL;
	}
	return pDeviceNext;
}


int SDL_SYS_JoystickInit(void)
{
	IOReturn result = kIOReturnSuccess;
	mach_port_t masterPort = 0;
	io_iterator_t hidObjectIterator = 0;
	CFMutableDictionaryRef hidMatchDictionary = NULL;
	recDevice *device, *lastDevice;
	io_object_t ioHIDDeviceObject = 0;
	
	SDL_numjoysticks = 0;
	
	if (gpDeviceList)
	{
		SDL_SetError("Joystick: Device list already inited.");
		return -1;
	}
	
	result = IOMasterPort (bootstrap_port, &masterPort);
	if (kIOReturnSuccess != result)
	{
		SDL_SetError("Joystick: IOMasterPort error with bootstrap_port.");
		return -1;
	}

	
	hidMatchDictionary = IOServiceMatching (kIOHIDDeviceKey);
	if (hidMatchDictionary)
	{
		
		
	}
	else
	{
		SDL_SetError("Joystick: Failed to get HID CFMutableDictionaryRef via IOServiceMatching.");
		return -1;
	}
	
	
	result = IOServiceGetMatchingServices (masterPort, hidMatchDictionary, &hidObjectIterator);
	
	if (kIOReturnSuccess != result)
	{
		SDL_SetError("Joystick: Couldn't create a HID object iterator.");
		return -1;
	}
	if (!hidObjectIterator) 
	{
		gpDeviceList = NULL;
		SDL_numjoysticks = 0;
		return 0;
	}
	

	

	gpDeviceList = lastDevice = NULL;
	
	while ((ioHIDDeviceObject = IOIteratorNext (hidObjectIterator)))
	{
		
		device = HIDBuildDevice (ioHIDDeviceObject);
		if (!device)
			continue;

		
		result = IOObjectRelease (ioHIDDeviceObject);

		 
		if ( (device->usagePage != kHIDPage_GenericDesktop) ||
		     ((device->usage != kHIDUsage_GD_Joystick &&
		      device->usage != kHIDUsage_GD_GamePad &&
		      device->usage != kHIDUsage_GD_MultiAxisController)) ) {

			
			HIDDisposeDevice (&device);
			DisposePtr((Ptr)device);
			continue;
		}
		
		
		if (lastDevice)
			lastDevice->pNext = device;
		else
			gpDeviceList = device;
		lastDevice = device;
	}
	result = IOObjectRelease (hidObjectIterator); 

	
	device = gpDeviceList;
	while (device)
	{
		SDL_numjoysticks++;
		device = device->pNext;
	}
	
	return SDL_numjoysticks;
}

const char *SDL_SYS_JoystickName(int index)
{
	recDevice *device = gpDeviceList;
	
	for (; index > 0; index--)
		device = device->pNext;

	return device->product;
}

int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	recDevice *device = gpDeviceList;
	int index;
	
	for (index = joystick->index; index > 0; index--)
		device = device->pNext;

	joystick->hwdata = device;
	joystick->name = device->product;

	joystick->naxes = device->axes;
	joystick->nhats = device->hats;
	joystick->nballs = 0;
	joystick->nbuttons = device->buttons;

	return 0;
}

void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
	recDevice *device = joystick->hwdata;
	recElement *element;
	SInt32 value, range;
	int i;

	if (device->removed)  
	{
		if (device->uncentered)
		{
			device->uncentered = 0;

			
			for (i = 0; i < device->axes; i++)
				SDL_PrivateJoystickAxis(joystick, i, 0);

			for (i = 0; i < device->buttons; i++)
				SDL_PrivateJoystickButton(joystick, i, 0);

			for (i = 0; i < device->hats; i++)
				SDL_PrivateJoystickHat(joystick, i, SDL_HAT_CENTERED);
		}

		return;
	}

	element = device->firstAxis;
	i = 0;
	while (element)
	{
		value = HIDScaledCalibratedValue(device, element, -32768, 32767);
		if ( value != joystick->axes[i] )
			SDL_PrivateJoystickAxis(joystick, i, value);
		element = element->pNext;
		++i;
	}
	
	element = device->firstButton;
	i = 0;
	while (element)
	{
		value = HIDGetElementValue(device, element);
        if (value > 1)  
            value = 1;
		if ( value != joystick->buttons[i] )
			SDL_PrivateJoystickButton(joystick, i, value);
		element = element->pNext;
		++i;
	}
	    
	element = device->firstHat;
	i = 0;
	while (element)
	{
		Uint8 pos = 0;

		range = (element->max - element->min + 1);
		value = HIDGetElementValue(device, element) - element->min;
		if (range == 4) 
			value *= 2;
		else if (range != 8) 
			value = -1;
		switch(value)
		{
			case 0:
				pos = SDL_HAT_UP;
				break;
			case 1:
				pos = SDL_HAT_RIGHTUP;
				break;
			case 2:
				pos = SDL_HAT_RIGHT;
				break;
			case 3:
				pos = SDL_HAT_RIGHTDOWN;
				break;
			case 4:
				pos = SDL_HAT_DOWN;
				break;
			case 5:
				pos = SDL_HAT_LEFTDOWN;
				break;
			case 6:
				pos = SDL_HAT_LEFT;
				break;
			case 7:
				pos = SDL_HAT_LEFTUP;
				break;
			default:
				pos = SDL_HAT_CENTERED;
				break;
		}
		if ( pos != joystick->hats[i] )
			SDL_PrivateJoystickHat(joystick, i, pos);
		element = element->pNext;
		++i;
	}
	
	return;
}

void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
	
	return;
}

void SDL_SYS_JoystickQuit(void)
{
	while (NULL != gpDeviceList)
		gpDeviceList = HIDDisposeDevice (&gpDeviceList);
}

#endif 
