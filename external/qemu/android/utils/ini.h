/* Copyright (C) 2008 The Android Open Source Project
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
#ifndef _ANDROID_UTILS_INI_H
#define _ANDROID_UTILS_INI_H

#include <stdint.h>


typedef struct IniFile   IniFile;

IniFile*  iniFile_newFromMemory( const char*  text, const char*  fileName  );

IniFile*  iniFile_newFromFile( const char*  filePath);

int       iniFile_saveToFile( IniFile*  f, const char*  filePath );

int       iniFile_saveToFileClean( IniFile*  f, const char*  filepath );

void      iniFile_free( IniFile*  f );

int       iniFile_getPairCount( IniFile*  f );

const char*  iniFile_getValue( IniFile*  f, const char*  key );

int     iniFile_getEntry(IniFile* f, int index, char** key, char** value);

char*   iniFile_getString( IniFile*  f, const char*  key, const char* defaultValue );

int     iniFile_getInteger( IniFile*  f, const char*  key, int  defaultValue );

int64_t iniFile_getInt64( IniFile*  f, const char*  key, int64_t  defaultValue );

double  iniFile_getDouble( IniFile*  f, const char*  key, double  defaultValue );

int     iniFile_getBoolean( IniFile*  f, const char*  key, const char*  defaultValue );

int64_t  iniFile_getDiskSize( IniFile*  f, const char*  key, const char*  defaultValue );

void iniFile_setValue( IniFile* f, const char* key, const char* value );
void iniFile_setInteger( IniFile* f, const char* key, int value );
void iniFile_setInt64( IniFile* f, const char* key, int64_t value );
void iniFile_setDouble( IniFile* f, const char* key, double value );
void iniFile_setBoolean( IniFile* f, const char* key, int value );
void iniFile_setDiskSize( IniFile* f, const char* key, int64_t size );


#endif 
