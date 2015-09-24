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
#include "android/utils/path.h"
#include "android/utils/misc.h"
#include "android/utils/debug.h"
#include "android/utils/system.h"
#include "android/charmap.h"
#include <stdio.h>
#include <errno.h>


#define KCM_MAX_LINE_LEN    1024

#define KCM_MAX_TOKEN_LEN   512

typedef struct AKeycodeMapEntry {
    
    const char* key_name;

    
    int         key_code;
} AKeycodeMapEntry;

typedef enum {
    
    BAD_FORMAT,

    
    SKIP_LINE,

    
    KEY_ENTRY,
} ParseStatus;

static const AKeycodeMapEntry keycode_map[] = {
    

      { "A",             kKeyCodeA },
      { "B",             kKeyCodeB },
      { "C",             kKeyCodeC },
      { "D",             kKeyCodeD },
      { "E",             kKeyCodeE },
      { "F",             kKeyCodeF },
      { "G",             kKeyCodeG },
      { "H",             kKeyCodeH },
      { "I",             kKeyCodeI },
      { "J",             kKeyCodeJ },
      { "K",             kKeyCodeK },
      { "L",             kKeyCodeL },
      { "M",             kKeyCodeM },
      { "N",             kKeyCodeN },
      { "O",             kKeyCodeO },
      { "P",             kKeyCodeP },
      { "Q",             kKeyCodeQ },
      { "R",             kKeyCodeR },
      { "S",             kKeyCodeS },
      { "T",             kKeyCodeT },
      { "U",             kKeyCodeU },
      { "V",             kKeyCodeV },
      { "W",             kKeyCodeW },
      { "X",             kKeyCodeX },
      { "Y",             kKeyCodeY },
      { "Z",             kKeyCodeZ },
      { "0",             kKeyCode0 },
      { "1",             kKeyCode1 },
      { "2",             kKeyCode2 },
      { "3",             kKeyCode3 },
      { "4",             kKeyCode4 },
      { "5",             kKeyCode5 },
      { "6",             kKeyCode6 },
      { "7",             kKeyCode7 },
      { "8",             kKeyCode8 },
      { "9",             kKeyCode9 },
      { "COMMA",         kKeyCodeComma },
      { "PERIOD",        kKeyCodePeriod },
      { "AT",            kKeyCodeAt },
      { "SLASH",         kKeyCodeSlash },
      { "SPACE",         kKeyCodeSpace },
      { "ENTER",         kKeyCodeNewline },
      { "TAB",           kKeyCodeTab },
      { "GRAVE",         kKeyCodeGrave },
      { "MINUS",         kKeyCodeMinus },
      { "EQUALS",        kKeyCodeEquals },
      { "LEFT_BRACKET",  kKeyCodeLeftBracket },
      { "RIGHT_BRACKET", kKeyCodeRightBracket },
      { "BACKSLASH",     kKeyCodeBackslash },
      { "SEMICOLON",     kKeyCodeSemicolon },
      { "APOSTROPHE",    kKeyCodeApostrophe },
      { "STAR",          kKeyCodeStar },
      { "POUND",         kKeyCodePound },
      { "PLUS",          kKeyCodePlus },
      { "DEL",           kKeyCodeDel },
};


static const AKeyEntry  _qwerty2_keys[] =
{
   

    { kKeyCodeA             ,   'a',   'A',   'a',    'A',   'a' },
    { kKeyCodeB             ,   'b',   'B',   'b',    'B',   'b' },
    { kKeyCodeC             ,   'c',   'C', 0x00e7, 0x00E7,   'c' },
    { kKeyCodeD             ,   'd',   'D',  '\'',   '\'',  '\'' },
    { kKeyCodeE             ,   'e',   'E',   '"', 0x0301,   '"' },
    { kKeyCodeF             ,   'f',   'F',   '[',    '[',   '[' },
    { kKeyCodeG             ,   'g',   'G',   ']',    ']',   ']' },
    { kKeyCodeH             ,   'h',   'H',   '<',    '<',   '<' },
    { kKeyCodeI             ,   'i',   'I',   '-', 0x0302,   '-' },
    { kKeyCodeJ             ,   'j',   'J',   '>',    '>',   '>' },
    { kKeyCodeK             ,   'k',   'K',   ';',    '~',   ';' },
    { kKeyCodeL             ,   'l',   'L',   ':',    '`',   ':' },
    { kKeyCodeM             ,   'm',   'M',   '%',   0x00,   '%' },
    { kKeyCodeN             ,   'n',   'N',  0x00, 0x0303,   'n' },
    { kKeyCodeO             ,   'o',   'O',   '+',    '+',   '+' },
    { kKeyCodeP             ,   'p',   'P',   '=', 0x00A5,   '=' },
    { kKeyCodeQ             ,   'q',   'Q',   '|', 0x0300,   '|' },
    { kKeyCodeR             ,   'r',   'R',   '`', 0x20AC,   '`' },
    { kKeyCodeS             ,   's',   'S',  '\\', 0x00DF,  '\\' },
    { kKeyCodeT             ,   't',   'T',   '{', 0x00A3,   '}' },
    { kKeyCodeU             ,   'u',   'U',   '_', 0x0308,   '_' },
    { kKeyCodeV             ,   'v',   'V',   'v',    'V',   'v' },
    { kKeyCodeW             ,   'w',   'W',   '~',    '~',   '~' },
    { kKeyCodeX             ,   'x',   'X',   'x',    'X',   'x' },
    { kKeyCodeY             ,   'y',   'Y',   '}', 0x00A1,   '}' },
    { kKeyCodeZ             ,   'z',   'Z',   'z',    'Z',   'z' },
    { kKeyCodeComma         ,   ',',   '<',   ',',    ',',   ',' },
    { kKeyCodePeriod        ,   '.',   '>',   '.', 0x2026,   '.' },
    { kKeyCodeAt            ,   '@',   '@',   '@', 0x2022,   '@' },
    { kKeyCodeSlash         ,   '/',   '?',   '?',    '?',   '/' },
    { kKeyCodeSpace         ,  0x20,  0x20,   0x9,    0x9,  0x20 },
    { kKeyCodeNewline       ,   0xa,   0xa,   0xa,    0xa,   0xa },
    { kKeyCode0             ,   '0',   ')',   ')',    ')',   '0' },
    { kKeyCode1             ,   '1',   '!',   '!',    '!',   '1' },
    { kKeyCode2             ,   '2',   '@',   '@',    '@',   '2' },
    { kKeyCode3             ,   '3',   '#',   '#',    '#',   '3' },
    { kKeyCode4             ,   '4',   '$',   '$',    '$',   '4' },
    { kKeyCode5             ,   '5',   '%',   '%',    '%',   '5' },
    { kKeyCode6             ,   '6',   '^',   '^',    '^',   '6' },
    { kKeyCode7             ,   '7',   '&',   '&',    '&',   '7' },
    { kKeyCode8             ,   '8',   '*',   '*',    '*',   '8' },
    { kKeyCode9             ,   '9',   '(',   '(',    '(',   '9' },
    { kKeyCodeTab           ,   0x9,   0x9,   0x9,    0x9,   0x9 },
    { kKeyCodeGrave         ,   '`',   '~',   '`',    '~',   '`' },
    { kKeyCodeMinus         ,   '-',   '_',   '-',    '_',   '-' },
    { kKeyCodeEquals        ,   '=',   '+',   '=',    '+',   '=' },
    { kKeyCodeLeftBracket   ,   '[',   '{',   '[',    '{',   '[' },
    { kKeyCodeRightBracket  ,   ']',   '}',   ']',    '}',   ']' },
    { kKeyCodeBackslash     ,  '\\',   '|',  '\\',    '|',  '\\' },
    { kKeyCodeSemicolon     ,   ';',   ':',   ';',    ':',   ';' },
    { kKeyCodeApostrophe    ,  '\'',   '"',  '\'',    '"',  '\'' },
};

static const AKeyCharmap  _default_charmap =
{
    _qwerty2_keys,
    51,
    "qwerty2"
};

static AKeyCharmap android_custom_charmap = { 0 };

static const AKeyCharmap* android_charmap = &_default_charmap;

static int
kcm_is_eol(char ch) {
    
    return ('\0' == ch) || ('\n' == ch) || ('\r' == ch);
}

static int
kcm_is_token_separator(char ch) {
    
    
    return (' ' == ch) || ('\t' == ch);
}

static int
kcm_is_path_separator(char ch) {
#ifdef _WIN32
    return '/' == ch || '\\' == ch;
#else
    return '/' == ch;
#endif  
}

static const char*
kcm_skip_spaces(const char* str) {
    while (!kcm_is_eol(*str) && kcm_is_token_separator(*str)) {
        str++;
    }
    return str;
}

static const char*
kcm_skip_non_spaces(const char* str) {
    while (!kcm_is_eol(*str) && !kcm_is_token_separator(*str)) {
        str++;
    }
    return str;
}

static const char*
kcm_get_token(const char* line, char* token, size_t max_token_len) {
    
    const char* token_starts = kcm_skip_spaces(line);
    
    const char* token_ends = kcm_skip_non_spaces(token_starts);
    
    size_t token_len = token_ends - token_starts;
    if ((0 == token_len) || (token_len >= max_token_len)) {
      return NULL;
    }
    memcpy(token, token_starts, token_len);
    token[token_len] = '\0';
    return token_ends;
}

static int
kcm_is_token_comment(const char* token) {
    return '#' == *token;
}

static int
kcm_get_key_code(const char* key_name, unsigned short* key_code) {
    int n;
    
    for (n = 0; n < sizeof(keycode_map) / sizeof(keycode_map[0]); n++) {
        if (0 == strcmp(key_name, keycode_map[n].key_name)) {
            *key_code = keycode_map[n].key_code;
            return 0;
        }
    }
    return -1;
}

static int
kcm_get_ushort_hex_val(const char* token, unsigned short* val) {
    int hex_val = hex2int((const uint8_t*)token, strlen(token));
    
    if (-1 == hex_val || 0 != (hex_val & ~0xFFFF)) {
      return -1;
    }

    *val = (unsigned short)hex_val;

    return 0;
}

static int
kcm_get_char_or_hex_val(const char* token, unsigned short* val) {
    
    if ('\'' == *token) {
        if ('\0' == token[1] || '\'' != token[2] || '\0' != token[3]) {
            return 0;
        }
        *val = token[1];
        return 0;
    } else {
        
        if (('0' != *token) || ('x' != token[1])) {
            return -1;
        }
        
        return kcm_get_ushort_hex_val(token + 2, val);
    }
}

static const char*
kcm_get_char_or_hex_token_value(const char* line, unsigned short* val) {
    char token[KCM_MAX_TOKEN_LEN];
    line = kcm_get_token(line, token, KCM_MAX_TOKEN_LEN);
    if (NULL != line) {
        
        if (kcm_get_char_or_hex_val(token, val)) {
            return NULL;
        }
    }

    return line;
}

static ParseStatus
kcm_parse_line(const char* line,
               int line_index,
               AKeyEntry* key_entry,
               const char* kcm_file_path) {
      char token[KCM_MAX_TOKEN_LEN];
      unsigned short disp;

      
      line = kcm_get_token(line, token, KCM_MAX_TOKEN_LEN);
      if ((NULL == line) || kcm_is_token_comment(token)) {
          
          return SKIP_LINE;
      }

      
      if ('[' == token[0]) {
          return SKIP_LINE;
      }

      
      
      if (kcm_get_key_code(token, &key_entry->code)) {
          derror("Invalid format of charmap file %s. Unknown key %s in line %d",
                 kcm_file_path, token, line_index);
          return BAD_FORMAT;
      }

      
      line = kcm_get_char_or_hex_token_value(line, &disp);
      if (NULL == line) {
          derror("Invalid format of charmap file %s. Invalid display value in line %d",
                 kcm_file_path, line_index);
          return BAD_FORMAT;
      }

      
      line = kcm_get_char_or_hex_token_value(line, &key_entry->number);
      if (NULL == line) {
          derror("Invalid format of charmap file %s. Invalid number value in line %d",
                 kcm_file_path, line_index);
          return BAD_FORMAT;
      }

      
      line = kcm_get_char_or_hex_token_value(line, &key_entry->base);
      if (NULL == line) {
          derror("Invalid format of charmap file %s. Invalid base value in line %d",
                 kcm_file_path, line_index);
          return BAD_FORMAT;
      }

      
      line = kcm_get_char_or_hex_token_value(line, &key_entry->caps);
      if (NULL == line) {
          derror("Invalid format of charmap file %s. Invalid caps value in line %d",
                 kcm_file_path, line_index);
          return BAD_FORMAT;
      }

      
      line = kcm_get_char_or_hex_token_value(line, &key_entry->fn);
      if (NULL == line) {
          derror("Invalid format of charmap file %s. Invalid fn value in line %d",
                 kcm_file_path, line_index);
          return BAD_FORMAT;
      }

      
      line = kcm_get_char_or_hex_token_value(line, &key_entry->caps_fn);
      if (NULL == line) {
          derror("Invalid format of charmap file %s. Invalid caps_fn value in line %d",
                 kcm_file_path, line_index);
          return BAD_FORMAT;
      }

      
      
      line = kcm_get_token(line, token, KCM_MAX_TOKEN_LEN);
      if ((NULL == line) || kcm_is_token_comment(token)) {
          return KEY_ENTRY;
      } else {
          derror("Invalid format of charmap file %s in line %d",
                 kcm_file_path, line_index);
          return BAD_FORMAT;
      }
}

void
kcm_extract_charmap_name(const char* kcm_file_path,
                         char* charmap_name,
                         int max_len) {
    const char* ext_separator;
    size_t to_copy;

    
    
    const char* file_name = kcm_file_path + strlen(kcm_file_path);
    while (!kcm_is_path_separator(*file_name) &&
           (file_name != kcm_file_path)) {
        file_name--;
    }
    if (kcm_is_path_separator(*file_name)) {
        file_name++;
    }

    
    ext_separator = strrchr(file_name, '.');
    if (NULL == ext_separator) {
      
      ext_separator = file_name + strlen(file_name);
    } else if (ext_separator == file_name) {
      
      
      file_name++;
      ext_separator = file_name + strlen(file_name);
    }

    
    to_copy = ext_separator - file_name;
    if (to_copy > (max_len - 1)) {
        to_copy = max_len - 1;
    }
    memcpy(charmap_name, file_name, to_copy);
    charmap_name[to_copy] = '\0';
}

static void
kcm_get_charmap_name(const char* kcm_file_path, AKeyCharmap* char_map) {
    kcm_extract_charmap_name(kcm_file_path, char_map->name,
                             sizeof(char_map->name));
}

static int
parse_kcm_file(const char* kcm_file_path, AKeyCharmap* char_map) {
    
    char line[KCM_MAX_LINE_LEN];
    
    int err = 0;
    
    int cur_line = 1;
    
    int map_size = 52;
    FILE* kcm_file;

    char_map->num_entries = 0;
    char_map->entries = 0;

    kcm_file = fopen(kcm_file_path, "r");
    if (NULL == kcm_file) {
        derror("Unable to open charmap file %s : %s",
               kcm_file_path, strerror(errno));
        return -1;
    }

    
    kcm_get_charmap_name(kcm_file_path, char_map);

    
    char_map->num_entries = 0;
    AARRAY_NEW0(char_map->entries, map_size);

    
    for (; 0 != fgets(line, sizeof(line), kcm_file); cur_line++) {
        AKeyEntry key_entry;
        ParseStatus parse_res =
            kcm_parse_line(line, cur_line, &key_entry, kcm_file_path);
        if (BAD_FORMAT == parse_res) {
            err = -1;
            break;
        } else if (KEY_ENTRY == parse_res) {
            AKeyEntry* entries;
            
            
            if (map_size == char_map->num_entries) {
                AKeyEntry* entries = (AKeyEntry*)char_map->entries;
                map_size += 10;
                AARRAY_RENEW(entries, map_size);
                char_map->entries = (const AKeyEntry*)entries;
            }
            entries = (AKeyEntry*)char_map->entries;
            entries[char_map->num_entries] = key_entry;
            char_map->num_entries++;
        }
    }

    if (!err) {
        
        
        if (0 == feof(kcm_file)) {
            err = -1;
        }
        if (err) {
          derror("Error reading charmap file %s : %s",
                  kcm_file_path, strerror(errno));
        }
    }

    fclose(kcm_file);

    if (err) {
        
        if (0 != char_map->entries) {
            AFREE((void*)char_map->entries);
            char_map->entries = 0;
        }
        char_map->num_entries = 0;
    }

    return err;
}

int
android_charmap_setup(const char* kcm_file_path) {

    
    if (android_charmap != &_default_charmap || kcm_file_path == NULL)
        return 0;

    if (!parse_kcm_file(kcm_file_path, &android_custom_charmap)) {
        
        android_charmap = &android_custom_charmap;
    } else {
        derror("Unable to parse kcm file.");
        return -1;
    }

    return 0;
}

void
android_charmap_done(void) {
    if (android_charmap != &_default_charmap)
        AFREE((void*)android_charmap->entries);
}

const AKeyCharmap*
android_get_charmap_by_name(const char* name) {
    if (name != NULL) {
        if (!strcmp(android_charmap->name, name))
            return android_charmap;
        if (!strcmp(_default_charmap.name, name))
            return &_default_charmap;
    }
    return NULL;
}

int
android_charmap_reverse_map_unicode(const AKeyCharmap* cmap,
                                    unsigned int unicode,
                                    int  down,
                                    AKeycodeBuffer* keycodes)
{
    int                 n;

    if (unicode == 0)
        return 0;

    
    for (n = 0; n < cmap->num_entries; n++) {
        if (cmap->entries[n].base == unicode) {
            android_keycodes_add_key_event(keycodes, cmap->entries[n].code, down);
            return 1;
        }
    }

    
    for (n = 0; n < cmap->num_entries; n++) {
        if (cmap->entries[n].caps == unicode) {
            if (down) {
                android_keycodes_add_key_event(keycodes, kKeyCodeCapLeft, down);
            }
            android_keycodes_add_key_event(keycodes, cmap->entries[n].code, down);
            if (!down) {
                android_keycodes_add_key_event(keycodes, kKeyCodeCapLeft, down);
            }
            return 2;
        }
    }

    
    for (n = 0; n < cmap->num_entries; n++) {
        if (cmap->entries[n].fn == unicode) {
            if (down) {
                android_keycodes_add_key_event(keycodes, kKeyCodeAltLeft, down);
            }
            android_keycodes_add_key_event(keycodes, cmap->entries[n].code, down);
            if (!down) {
                android_keycodes_add_key_event(keycodes, kKeyCodeAltLeft, down);
            }
            return 2;
        }
    }

    
    for (n = 0; n < cmap->num_entries; n++) {
        if (cmap->entries[n].caps_fn == unicode) {
            if (down) {
                android_keycodes_add_key_event(keycodes, kKeyCodeAltLeft, down);
                android_keycodes_add_key_event(keycodes, kKeyCodeCapLeft, down);
            }
            android_keycodes_add_key_event(keycodes, cmap->entries[n].code, down);
            if (!down) {
                android_keycodes_add_key_event(keycodes, kKeyCodeCapLeft, down);
                android_keycodes_add_key_event(keycodes, kKeyCodeAltLeft, down);
            }
            return 3;
        }
    }

    
    return 0;
}

const AKeyCharmap* android_get_default_charmap(void)
{
    return &_default_charmap;
}

const AKeyCharmap* android_get_charmap(void)
{
    return android_charmap;
}

const char* android_get_charmap_name(void)
{
    return android_get_charmap()->name;
}
