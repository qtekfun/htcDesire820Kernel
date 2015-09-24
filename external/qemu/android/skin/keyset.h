/* Copyright (C) 2007-2008 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#ifndef _ANDROID_SKIN_KEYSET_H_
#define _ANDROID_SKIN_KEYSET_H_

#include "android/config.h"

typedef struct SkinKeyset  SkinKeyset;

#define  SKIN_KEY_COMMAND_LIST                  \
    _SKIN_KEY_COMMAND(NONE,"no key")            \
    _SKIN_KEY_COMMAND(BUTTON_HOME,"Home button")              \
    _SKIN_KEY_COMMAND(BUTTON_MENU,"Menu (Soft-Left) button")   \
    _SKIN_KEY_COMMAND(BUTTON_STAR,"Star (Soft-Right) button")  \
    _SKIN_KEY_COMMAND(BUTTON_BACK,"Back button")              \
    _SKIN_KEY_COMMAND(BUTTON_CALL,"Call/Dial button")              \
    _SKIN_KEY_COMMAND(BUTTON_HANGUP,"Hangup/EndCall button")            \
    _SKIN_KEY_COMMAND(BUTTON_POWER,"Power button")             \
    _SKIN_KEY_COMMAND(BUTTON_SEARCH,"Search button")            \
    _SKIN_KEY_COMMAND(BUTTON_VOLUME_UP,"Volume up button")         \
    _SKIN_KEY_COMMAND(BUTTON_VOLUME_DOWN,"Volume down button")       \
    _SKIN_KEY_COMMAND(BUTTON_CAMERA,"Camera button")            \
    _SKIN_KEY_COMMAND(CHANGE_LAYOUT_PREV,"Switch to previous layout")       \
    _SKIN_KEY_COMMAND(CHANGE_LAYOUT_NEXT,"Switch to next layout")       \
    _SKIN_KEY_COMMAND(TOGGLE_NETWORK,"Toggle cell network on/off")           \
    _SKIN_KEY_COMMAND(TOGGLE_TRACING,"Toggle code profiling")           \
    _SKIN_KEY_COMMAND(TOGGLE_FULLSCREEN,"Toggle fullscreen mode")        \
    _SKIN_KEY_COMMAND(TOGGLE_TRACKBALL,"Toggle trackball mode")         \
    _SKIN_KEY_COMMAND(SHOW_TRACKBALL,"Show trackball") \
    _SKIN_KEY_COMMAND(BUTTON_DPAD_CENTER,"DPad center")       \
    _SKIN_KEY_COMMAND(BUTTON_DPAD_LEFT,"DPad left") \
    _SKIN_KEY_COMMAND(BUTTON_DPAD_RIGHT,"DPad right")        \
    _SKIN_KEY_COMMAND(BUTTON_DPAD_UP,"DPad up")           \
    _SKIN_KEY_COMMAND(BUTTON_DPAD_DOWN,"DPad down")         \
    _SKIN_KEY_COMMAND(ONION_ALPHA_UP,"Increase onion alpha")           \
    _SKIN_KEY_COMMAND(ONION_ALPHA_DOWN,"Decrease onion alpha")         \
    _SKIN_KEY_COMMAND(BUTTON_TV,"TV button")         \
    _SKIN_KEY_COMMAND(BUTTON_EPG,"EPG button")         \
    _SKIN_KEY_COMMAND(BUTTON_DVR,"DVR button")         \
    _SKIN_KEY_COMMAND(BUTTON_PREV,"Previous button")         \
    _SKIN_KEY_COMMAND(BUTTON_NEXT,"Next button")         \
    _SKIN_KEY_COMMAND(BUTTON_PLAY,"Play button")         \
    _SKIN_KEY_COMMAND(BUTTON_PAUSE,"Pause button")         \
    _SKIN_KEY_COMMAND(BUTTON_STOP,"Stop button")         \
    _SKIN_KEY_COMMAND(BUTTON_REWIND,"Rewind button")         \
    _SKIN_KEY_COMMAND(BUTTON_FFWD,"Fast forward button")         \
    _SKIN_KEY_COMMAND(BUTTON_BOOKMARKS,"Bookmarks button")         \
    _SKIN_KEY_COMMAND(BUTTON_WINDOW,"Window button")         \
    _SKIN_KEY_COMMAND(BUTTON_CHANNELUP,"Channel up button")         \
    _SKIN_KEY_COMMAND(BUTTON_CHANNELDOWN,"Channel down button")         \


#define _SKIN_KEY_COMMAND(x,y)  SKIN_KEY_COMMAND_##x,
typedef enum {
    SKIN_KEY_COMMAND_LIST
    SKIN_KEY_COMMAND_MAX  
} SkinKeyCommand;
#undef _SKIN_KEY_COMMAND

extern const char*      skin_key_command_to_str  ( SkinKeyCommand  command );

extern SkinKeyCommand   skin_key_command_from_str( const char*  str, int  len );

extern const char*      skin_key_command_description( SkinKeyCommand  cmd );

extern int              skin_keysym_str_count( void );

extern const char*      skin_keysym_str( int  index );

/* convert a (sym,mod) pair into a descriptive string. e.g. "Ctrl-K" or "Alt-A", etc..
 * result is a static string that is overwritten on each call
 */
extern const char*      skin_key_symmod_to_str   ( int  sym, int  mod );

extern int              skin_key_symmod_from_str ( const char*  str, int  *psym, int  *pmod );

extern SkinKeyset*      skin_keyset_new ( AConfig*  root );
extern SkinKeyset*      skin_keyset_new_from_text( const char*  text );

extern void             skin_keyset_free( SkinKeyset*  kset );

#define  SKIN_KEY_COMMAND_MAX_BINDINGS  3

typedef struct {
    int  sym;   
    int  mod;   
} SkinKeyBinding;

extern int              skin_keyset_get_bindings( SkinKeyset*      kset,
                                                  SkinKeyCommand   command,
                                                  SkinKeyBinding*  bindings );

extern SkinKeyCommand   skin_keyset_get_command( SkinKeyset*  kset, int  sym, int  mod );

extern const char*      skin_keyset_get_default( void );

extern SkinKeyset*      android_keyset;

#endif 
