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

#include <nds.h>

#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_joystick.h"
#include "../SDL_sysjoystick.h"
#include "../SDL_joystick_c.h"

#include "../../video/nds/SDL_ndsevents_c.h"

int SDL_SYS_JoystickInit(void)
{
	SDL_numjoysticks = 1;
    

	return(1);
}

const char *SDL_SYS_JoystickName(int index)
{
	if(!index)
		return "NDS builtin joypad";
	SDL_SetError("No joystick available with that index");
	return (NULL);
}

int SDL_SYS_JoystickOpen(SDL_Joystick *joystick)
{
	joystick->nbuttons=8;
	joystick->nhats=0;
	joystick->nballs=0;
	joystick->naxes=2;
	return 0;
}



int prevbutton=0;
int prevkey=0;

int dc=0;int ldc=0;
u32 keysd,keysu=0;
void SDL_SYS_JoystickUpdate(SDL_Joystick *joystick)
{
    
	
	
		
		
		
		
		
		
		 
	
	
	
		keysd = keysDown(); 
		keysu = keysUp();
		
	
	short ax=0,v=0,h=0;
	if((keysd&KEY_UP)) {ax=1;v=-10;SDL_PrivateJoystickAxis(joystick,ax,v);prevkey=KEY_UP;}
	if((keysd&KEY_DOWN)) {ax=1;v=10;SDL_PrivateJoystickAxis(joystick,ax,v);prevkey=KEY_DOWN;}
	if((keysd&KEY_LEFT)) {ax=0;h=-10;SDL_PrivateJoystickAxis(joystick,ax,h);prevkey=KEY_LEFT;}
	if((keysd&KEY_RIGHT)) {ax=0;h=10;SDL_PrivateJoystickAxis(joystick,ax,h);prevkey=KEY_RIGHT;}

	if((keysu&KEY_UP)) {ax=1;v=0;SDL_PrivateJoystickAxis(joystick,ax,v);prevkey=0;}
	if((keysu&KEY_DOWN)) {ax=1;v=0;SDL_PrivateJoystickAxis(joystick,ax,v);prevkey=0;}
	if((keysu&KEY_LEFT)) {ax=0;h=0;SDL_PrivateJoystickAxis(joystick,ax,h);prevkey=0;}
	if((keysu&KEY_RIGHT)) {ax=0;h=0;SDL_PrivateJoystickAxis(joystick,ax,h);prevkey=0;}

	if((keysd&KEY_A))		{SDL_PrivateJoystickButton(joystick,0,SDL_PRESSED);prevbutton=KEY_A;}
	if((keysd&KEY_B))		{SDL_PrivateJoystickButton(joystick,1,SDL_PRESSED);prevbutton=KEY_B;}
	if((keysd&KEY_X))		{SDL_PrivateJoystickButton(joystick,2,SDL_PRESSED);prevbutton=KEY_X;}
	if((keysd&KEY_Y))		{SDL_PrivateJoystickButton(joystick,3,SDL_PRESSED);prevbutton=KEY_Y;}
	if((keysd&KEY_SELECT))	{SDL_PrivateJoystickButton(joystick,6,SDL_PRESSED);prevbutton=KEY_SELECT;}
	if((keysd&KEY_START))	{SDL_PrivateJoystickButton(joystick,7,SDL_PRESSED);prevbutton=KEY_START;}
	if((keysd&KEY_L))		{SDL_PrivateJoystickButton(joystick,4,SDL_PRESSED);prevbutton=KEY_L;}
	if((keysd&KEY_R))		{SDL_PrivateJoystickButton(joystick,5,SDL_PRESSED);prevbutton=KEY_R;}

	if((keysu&KEY_A))		{SDL_PrivateJoystickButton(joystick,0,SDL_RELEASED);prevbutton=0;}
	if((keysu&KEY_B))		{SDL_PrivateJoystickButton(joystick,1,SDL_RELEASED);prevbutton=0;}
	if((keysu&KEY_X))		{SDL_PrivateJoystickButton(joystick,2,SDL_RELEASED);prevbutton=0;}
	if((keysu&KEY_Y))		{SDL_PrivateJoystickButton(joystick,3,SDL_RELEASED);prevbutton=0;}
	if((keysu&KEY_SELECT))  {SDL_PrivateJoystickButton(joystick,6,SDL_RELEASED);prevbutton=0;}
	if((keysu&KEY_START))	{SDL_PrivateJoystickButton(joystick,7,SDL_RELEASED);prevbutton=0;}
	if((keysu&KEY_L))		{SDL_PrivateJoystickButton(joystick,4,SDL_RELEASED);prevbutton=0;}
	if((keysu&KEY_R))		{SDL_PrivateJoystickButton(joystick,5,SDL_RELEASED);prevbutton=0;}



}

void SDL_SYS_JoystickClose(SDL_Joystick *joystick)
{
}

void SDL_SYS_JoystickQuit(void)
{
}

