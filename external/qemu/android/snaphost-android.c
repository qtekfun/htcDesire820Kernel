/* Copyright (C) 2012 The Android Open Source Project
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

#include "qemu-common.h"
#include "android/globals.h"
#include "android/snaphost-android.h"
#include "android/utils/debug.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(init,__VA_ARGS__)

static int
_cmp_hw_config(IniFile* current, IniFile* saved)
{
    int n, ret = 0;
    const int num_pairs = iniFile_getPairCount(current);

    
    if (num_pairs != iniFile_getPairCount(saved)) {
        D("Different numbers of entries in the HW config files. Current contans %d, while saved contains %d entries.",
          num_pairs, iniFile_getPairCount(saved));
        return -1;
    }

    for (n = 0; n < num_pairs && ret == 0; n++) {
        char* key, *value1, *value2;

        if (iniFile_getEntry(current, n, &key, &value1)) {
            D("Unable to obtain entry %d from the current HW config file", n);
            return -1;
        }

        value2 = iniFile_getString(saved, key, "");
        if (value2 == NULL) {
            D("Saved HW config file is missing entry ('%s', '%s') found in the current HW config.",
              key, value1);
            free(key);
            free(value1);
            return 1;
        }

        ret = strcmp(value1, value2);
        if (ret) {
            D("HW config value mismatch for a key '%s': current is '%s' while saved was '%s'",
              key, value1, value2);
        }

        free(value2);
        free(value1);
        free(key);
    }

    return ret ? 1 : 0;
}

static char*
_build_hwcfg_path(const char* name)
{
    const int path_len = strlen(android_hw->disk_snapStorage_path) +
                         strlen(name) + 6;
    char* bkp_path = malloc(path_len);
    if (bkp_path == NULL) {
        E("Unable to allocate %d bytes for HW config path!", path_len);
        return NULL;
    }

    snprintf(bkp_path, path_len, "%s.%s.ini",
             android_hw->disk_snapStorage_path, name);

    return bkp_path;
}

int
snaphost_match_configs(IniFile* hw_ini, const char* name)
{
    
    if (android_hw->disk_snapStorage_path == NULL ||
        *android_hw->disk_snapStorage_path == '\0') {
        return 1;
    }

    
    char* bkp_path = _build_hwcfg_path(name);
    if (bkp_path == NULL) {
        return 0;
    }

    
    IniFile* hwcfg_bkp = iniFile_newFromFile(bkp_path);

    if (hwcfg_bkp != NULL) {
        if (_cmp_hw_config(hw_ini, hwcfg_bkp)) {
            E("Unable to load VM from snapshot. The snapshot has been saved for a different hardware configuration.");
            free(bkp_path);
            return 0;
        }
        iniFile_free(hwcfg_bkp);
    } else {
        D("Missing HW config backup file '%s'", bkp_path);
    }

    free(bkp_path);

    return 1;
}

void
snaphost_save_config(const char* name)
{
    
    if (android_hw->disk_snapStorage_path == NULL ||
        *android_hw->disk_snapStorage_path == '\0') {
        return;
    }

    
    char* bkp_path = _build_hwcfg_path(name);
    if (bkp_path == NULL) {
        return;
    }

    
    IniFile* hwcfg_bkp = iniFile_newFromMemory("", bkp_path);
    if (hwcfg_bkp == NULL) {
        W("Unable to create backup HW config file '%s'. Error: %s",
          bkp_path, strerror(errno));
        return;
    }
    androidHwConfig_write(android_hw, hwcfg_bkp);

    iniFile_setValue(hwcfg_bkp, "disk.dataPartition.initPath", "");

    
    if (!iniFile_saveToFileClean(hwcfg_bkp, bkp_path)) {
        D("HW config has been backed up to '%s'", bkp_path);
    } else {
        W("Unable to save HW config file '%s'. Error: %s", bkp_path, strerror(errno));
    }
    iniFile_free(hwcfg_bkp);
    free(bkp_path);
}
