/* Copyright (C) 2011 The Android Open Source Project
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
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#include <fcntl.h>
#ifdef _WIN32
#include <process.h>
#endif

#include <SDL.h>
#include <SDL_syswm.h>

#include "console.h"

#include "android/avd/util.h"
#include "android/utils/debug.h"
#include "android/utils/path.h"
#include "android/utils/bufprint.h"
#include "android/utils/dirscanner.h"
#include "android/main-common.h"
#include "android/globals.h"
#include "android/resource.h"
#include "android/user-config.h"
#include "android/qemulator.h"
#include "android/display.h"
#include "android/skin/image.h"
#include "android/skin/trackball.h"
#include "android/skin/keyboard.h"
#include "android/skin/file.h"
#include "android/skin/window.h"




#define  D(...)  do {  if (VERBOSE_CHECK(init)) dprint(__VA_ARGS__); } while (0)


static AUserConfig*  userConfig;

void
user_config_init( void )
{
    userConfig = auserConfig_new( android_avdInfo );
}

void
user_config_done( void )
{
    int  win_x, win_y;

    if (!userConfig) {
        D("no user configuration?");
        return;
    }

    SDL_WM_GetPos( &win_x, &win_y );
    auserConfig_setWindowPos(userConfig, win_x, win_y);
    auserConfig_save(userConfig);
}

void
user_config_get_window_pos( int *window_x, int *window_y )
{
    *window_x = *window_y = 10;

    if (userConfig)
        auserConfig_getWindowPos(userConfig, window_x, window_y);
}

unsigned convertBytesToMB( uint64_t  size )
{
    if (size == 0)
        return 0;

    size = (size + ONE_MB-1) >> 20;
    if (size > UINT_MAX)
        size = UINT_MAX;

    return (unsigned) size;
}

uint64_t convertMBToBytes( unsigned  megaBytes )
{
    return ((uint64_t)megaBytes << 20);
}



#define  KEYSET_FILE    "default.keyset"

SkinKeyset*  android_keyset = NULL;

static int
load_keyset(const char*  path)
{
    if (path_can_read(path)) {
        AConfig*  root = aconfig_node("","");
        if (!aconfig_load_file(root, path)) {
            android_keyset = skin_keyset_new(root);
            if (android_keyset != NULL) {
                D( "keyset loaded from: %s", path);
                return 0;
            }
        }
    }
    return -1;
}

void
parse_keyset(const char*  keyset, AndroidOptions*  opts)
{
    char   kname[MAX_PATH];
    char   temp[MAX_PATH];
    char*  p;
    char*  end;

    
    if (strchr(keyset, '.') == NULL) {
        p   =  kname;
        end = p + sizeof(kname);
        p   = bufprint(p, end, "%s.keyset", keyset);
        if (p >= end) {
            derror( "keyset name too long: '%s'\n", keyset);
            exit(1);
        }
        keyset = kname;
    }

    
    p   = temp;
    end = p + sizeof(temp);
    p = bufprint_config_file(p, end, keyset);
    if (p < end && load_keyset(temp) == 0)
        return;

    p = temp;
    p = bufprint(p, end, "%s" PATH_SEP "keysets" PATH_SEP "%s", opts->sysdir, keyset);
    if (p < end && load_keyset(temp) == 0)
        return;

    p = temp;
    p = bufprint_app_dir(p, end);
    p = bufprint(p, end, PATH_SEP "keysets" PATH_SEP "%s", keyset);
    if (p < end && load_keyset(temp) == 0)
        return;

    return;
}

void
write_default_keyset( void )
{
    char   path[MAX_PATH];

    bufprint_config_file( path, path+sizeof(path), KEYSET_FILE );

    
    if ( !path_exists(path) ) {
        int          fd = open( path, O_WRONLY | O_CREAT, 0666 );
        int          ret;
        const char*  ks = skin_keyset_get_default();


        D( "writing default keyset file to %s", path );

        if (fd < 0) {
            D( "%s: could not create file: %s", __FUNCTION__, strerror(errno) );
            return;
        }
        CHECKED(ret, write(fd, ks, strlen(ks)));
        close(fd);
    }
}




void *readpng(const unsigned char*  base, size_t  size, unsigned *_width, unsigned *_height);

#ifdef CONFIG_DARWIN
#  define  ANDROID_ICON_PNG  "android_icon_256.png"
#else
#  define  ANDROID_ICON_PNG  "android_icon_16.png"
#endif

static void
sdl_set_window_icon( void )
{
    static int  window_icon_set;

    if (!window_icon_set)
    {
#ifdef _WIN32
        HANDLE         handle = GetModuleHandle( NULL );
        HICON          icon   = LoadIcon( handle, MAKEINTRESOURCE(1) );
        SDL_SysWMinfo  wminfo;

        SDL_GetWMInfo(&wminfo);

        SetClassLongPtr( wminfo.window, GCLP_HICON, (LONG)icon );
#else  
        unsigned              icon_w, icon_h;
        size_t                icon_bytes;
        const unsigned char*  icon_data;
        void*                 icon_pixels;

        window_icon_set = 1;

        icon_data = android_icon_find( ANDROID_ICON_PNG, &icon_bytes );
        if ( !icon_data )
            return;

        icon_pixels = readpng( icon_data, icon_bytes, &icon_w, &icon_h );
        if ( !icon_pixels )
            return;

        {
            unsigned*  d     = icon_pixels;
            unsigned*  d_end = d + icon_w*icon_h;

            for ( ; d < d_end; d++ ) {
                unsigned  pix = d[0];
#if HOST_WORDS_BIGENDIAN
                
                pix = ((pix >> 8) & 0xffffff) | (pix << 24);
#else
                
                pix = (pix & 0xff00ff00) | ((pix >> 16) & 0xff) | ((pix & 0xff) << 16);
#endif
                d[0] = pix;
            }
        }

        SDL_Surface* icon = sdl_surface_from_argb32( icon_pixels, icon_w, icon_h );
        if (icon != NULL) {
            SDL_WM_SetIcon(icon, NULL);
            SDL_FreeSurface(icon);
            free( icon_pixels );
        }
#endif  
    }
}


const char*  skin_network_speed = NULL;
const char*  skin_network_delay = NULL;


static void sdl_at_exit(void)
{
    user_config_done();
    qemulator_done(qemulator_get());
    SDL_Quit();
}


void sdl_display_init(DisplayState *ds, int full_screen, int  no_frame)
{
    QEmulator*    emulator = qemulator_get();
    SkinDisplay*  disp     = skin_layout_get_display(emulator->layout);
    int           width, height;
    char          buf[128];

    if (disp->rotation & 1) {
        width  = disp->rect.size.h;
        height = disp->rect.size.w;
    } else {
        width  = disp->rect.size.w;
        height = disp->rect.size.h;
    }

    snprintf(buf, sizeof buf, "width=%d,height=%d", width, height);
#if !defined(CONFIG_STANDALONE_UI) && !defined(CONFIG_STANDALONE_CORE)
    android_display_init(ds, qframebuffer_fifo_get());
#endif
}

typedef struct part_properties part_properties;
struct part_properties {
    const char*      name;
    int              width;
    int              height;
    part_properties* next;
};

part_properties*
read_all_part_properties(AConfig* parts)
{
    part_properties* head = NULL;
    part_properties* prev = NULL;

    AConfig *node = parts->first_child;
    while (node) {
        part_properties* t = calloc(1, sizeof(part_properties));
        t->name = node->name;

        AConfig* bg = aconfig_find(node, "background");
        if (bg != NULL) {
            t->width = aconfig_int(bg, "width", 0);
            t->height = aconfig_int(bg, "height", 0);
        }

        if (prev == NULL) {
            head = t;
        } else {
            prev->next = t;
        }
        prev = t;
        node = node->next;
    }

    return head;
}

void
free_all_part_properties(part_properties* head)
{
    part_properties* prev = head;
    while (head) {
        prev = head;
        head = head->next;
        free(prev);
    }
}

part_properties*
get_part_properties(part_properties* allparts, char *partname)
{
    part_properties* p;
    for (p = allparts; p != NULL; p = p->next) {
        if (!strcmp(partname, p->name))
            return p;
    }

    return NULL;
}

void
add_parts_to_layout(AConfig* layout,
                    char* parts[],
                    int n_parts,
                    part_properties *props,
                    int xoffset,
                    int x_margin,
                    int y_margin)
{
    int     i;
    int     y = 10;
    char    tmp[512];
    for (i = 0; i < n_parts; i++) {
        part_properties *p = get_part_properties(props, parts[i]);
        snprintf(tmp, sizeof tmp,
            "part%d {\n \
                name %s\n \
                x %d\n \
                y %d\n \
            }",
            i + 2,  
            p->name,
            xoffset + x_margin,
            y
            );
        y += p->height + y_margin;
        aconfig_load(layout, strdup(tmp));
    }
}

int
load_dynamic_skin(AndroidHwConfig* hwConfig,
                  char**           skinDirPath,
                  int              width,
                  int              height,
                  AConfig*         root)
{
    char      tmp[1024];
    AConfig*  node;
    int       i;
    int       max_part_width;
    char      fromEnv;
    char*     sdkRoot = path_getSdkRoot(&fromEnv);

    if (sdkRoot == NULL) {
        dwarning("Unable to locate sdk root. Will not use dynamic skin.");
        return 0;
    }

    snprintf(tmp, sizeof(tmp), "%s/tools/lib/emulator/skins/dynamic/", sdkRoot);
    free(sdkRoot);

    if (!path_exists(tmp))
        return 0;

    *skinDirPath = strdup(tmp);
    snprintf(tmp, sizeof(tmp), "%s/layout", *skinDirPath);
    D("trying to load skin file '%s'", tmp);

    if(aconfig_load_file(root, tmp) < 0) {
        dwarning("could not load skin file '%s', won't use a skin\n", tmp);
        return 0;
    }

    
    node = aconfig_find(root, "parts");
    if (node != NULL) {
        node = aconfig_find(node, "device");
        if (node != NULL) {
            node = aconfig_find(node, "display");
            if (node != NULL) {
                snprintf(tmp, sizeof tmp, "%d", width);
                aconfig_set(node, "width", strdup(tmp));
                snprintf(tmp, sizeof tmp, "%d", height);
                aconfig_set(node, "height", strdup(tmp));
            }
        }
    }


    part_properties* props = read_all_part_properties(aconfig_find(root, "parts"));

    const int N_PARTS = 4;
    char* parts[N_PARTS];
    parts[0] = "basic_controls";
    parts[1] = hwConfig->hw_mainKeys ? "hwkeys_on" : "hwkeys_off";
    parts[2] = hwConfig->hw_dPad ? "dpad_on" : "dpad_off";
    parts[3] = hwConfig->hw_keyboard ? "keyboard_on" : "keyboard_off";

    for (i = 0, max_part_width = 0; i < N_PARTS; i++) {
        part_properties *p = get_part_properties(props, parts[i]);
        if (p != NULL && p->width > max_part_width)
                max_part_width = p->width;
    }

    int x_margin = 10;
    int y_margin = 10;
    snprintf(tmp, sizeof tmp,
            "layouts {\n \
                portrait {\n \
                    width %d\n \
                    height %d\n \
                    color 0x404040\n \
                    event EV_SW:0:1\n \
                    part1 {\n name device\n x 0\n y 0\n}\n \
                }\n \
                landscape {\n \
                    width %d\n \
                    height %d\n \
                    color 0x404040\n \
                    event EV_SW:0:0\n \
                    dpad-rotation 3\n \
                    part1 {\n name device\n x 0\n y %d\n rotation 3\n }\n \
                    }\n \
                }\n \
             }\n",
            width  + max_part_width + 2 * x_margin,
            height,
            height + max_part_width + 2 * x_margin,
            width,
            width);
    aconfig_load(root, strdup(tmp));

    
    node = aconfig_find(root, "layouts");
    if (node != NULL) {
        node = aconfig_find(node, "portrait");
        if (node != NULL) {
            add_parts_to_layout(node, parts, N_PARTS, props, width, x_margin, y_margin);
        }
    }

    
    node = aconfig_find(root, "layouts");
    if (node != NULL) {
        node = aconfig_find(node, "landscape");
        if (node != NULL) {
            add_parts_to_layout(node, parts, N_PARTS, props, height, x_margin, y_margin);
        }
    }

    free_all_part_properties(props);

    return 1;
}

static const struct {
    const char*  name;
    const char*  alias;
} skin_aliases[] = {
    { "QVGA-L", "320x240" },
    { "QVGA-P", "240x320" },
    { "HVGA-L", "480x320" },
    { "HVGA-P", "320x480" },
    { "QVGA", "320x240" },
    { "HVGA", "320x480" },
    { NULL, NULL }
};

void
parse_skin_files(const char*      skinDirPath,
                 const char*      skinName,
                 AndroidOptions*  opts,
                 AndroidHwConfig* hwConfig,
                 AConfig*        *skinConfig,
                 char*           *skinPath)
{
    char      tmp[1024];
    AConfig*  root;
    const char* path = NULL;
    AConfig*  n;

    root = aconfig_node("", "");

    if (skinName == NULL)
        goto DEFAULT_SKIN;

    int  checkAlias = 1;

    if (skinDirPath != NULL) {
        bufprint(tmp, tmp+sizeof(tmp), "%s/%s", skinDirPath, skinName);
        if (path_exists(tmp)) {
            checkAlias = 0;
        } else {
            D("there is no '%s' skin in '%s'", skinName, skinDirPath);
        }
    }

    if (checkAlias) {
        int  nn;

        for (nn = 0; ; nn++ ) {
            const char*  skin_name  = skin_aliases[nn].name;
            const char*  skin_alias = skin_aliases[nn].alias;

            if (!skin_name)
                break;

            if (!strcasecmp( skin_name, skinName )) {
                D("skin name '%s' aliased to '%s'", skinName, skin_alias);
                skinName = skin_alias;
                break;
            }
        }
    }

    
    if(isdigit(skinName[0])) {
        char *x = strchr(skinName, 'x');
        if(x && isdigit(x[1])) {
            int width = atoi(skinName);
            int height = atoi(x+1);
            int bpp   = 16;
            char* y = strchr(x+1, 'x');
            if (y && isdigit(y[1])) {
                bpp = atoi(y+1);
            }

            if (opts->dynamic_skin) {
                char *dynamicSkinDirPath;
                if (load_dynamic_skin(hwConfig, &dynamicSkinDirPath, width, height, root)) {
                    path = dynamicSkinDirPath;
                    D("loaded dynamic skin width=%d height=%d bpp=%d\n", width, height, bpp);
                    goto FOUND_SKIN;
                }
            }

            snprintf(tmp, sizeof tmp,
                    "display {\n  width %d\n  height %d\n bpp %d}\n",
                    width, height,bpp);
            aconfig_load(root, strdup(tmp));
            path = ":";
            D("found magic skin width=%d height=%d bpp=%d\n", width, height, bpp);
            goto FOUND_SKIN;
        }
    }

    if (skinDirPath == NULL) {
        derror("unknown skin name '%s'", skinName);
        exit(1);
    }

    snprintf(tmp, sizeof tmp, "%s/%s/layout", skinDirPath, skinName);
    D("trying to load skin file '%s'", tmp);

    if(aconfig_load_file(root, tmp) < 0) {
        dwarning("could not load skin file '%s', using built-in one\n",
                 tmp);
        goto DEFAULT_SKIN;
    }

    snprintf(tmp, sizeof tmp, "%s/%s/", skinDirPath, skinName);
    path = tmp;
    goto FOUND_SKIN;

FOUND_SKIN:
    
    n = aconfig_find(root, "network");
    if (n != NULL) {
        skin_network_speed = aconfig_str(n, "speed", 0);
        skin_network_delay = aconfig_str(n, "delay", 0);
    }

    n = aconfig_find(root, "display");
    if (n == NULL) {
        n = aconfig_find(root, "parts");
        if (n != NULL) {
            n = aconfig_find(n, "device");
            if (n != NULL) {
                n = aconfig_find(n, "display");
            }
        }
    }

    if (n != NULL) {
        int  width  = aconfig_int(n, "width", hwConfig->hw_lcd_width);
        int  height = aconfig_int(n, "height", hwConfig->hw_lcd_height);
        int  depth  = aconfig_int(n, "bpp", hwConfig->hw_lcd_depth);

        if (width > 0 && height > 0) {
            
            if (((width|height) & 3) != 0) {
                width  = (width+3) & ~3;
                height = (height+3) & ~3;
                D("adjusting LCD dimensions to (%dx%dx)", width, height);
            }

            
            if (depth != 32 && depth != 16) {
                depth = 16;
                D("adjusting LCD bit depth to %d", depth);
            }

            hwConfig->hw_lcd_width  = width;
            hwConfig->hw_lcd_height = height;
            hwConfig->hw_lcd_depth  = depth;
        }
        else {
            D("ignoring invalid skin LCD dimensions (%dx%dx%d)",
              width, height, depth);
        }
    }

    *skinConfig = root;
    *skinPath   = strdup(path);
    return;

DEFAULT_SKIN:
    {
        const unsigned char*  layout_base;
        size_t                layout_size;
        char*                 base;

        skinName = "<builtin>";

        layout_base = android_resource_find( "layout", &layout_size );
        if (layout_base == NULL) {
            fprintf(stderr, "Couldn't load builtin skin\n");
            exit(1);
        }
        base = malloc( layout_size+1 );
        memcpy( base, layout_base, layout_size );
        base[layout_size] = 0;

        D("parsing built-in skin layout file (%d bytes)", (int)layout_size);
        aconfig_load(root, base);
        path = ":";
    }
    goto FOUND_SKIN;
}


void
init_sdl_ui(AConfig*         skinConfig,
            const char*      skinPath,
            AndroidOptions*  opts)
{
    int  win_x, win_y, flags;

    signal(SIGINT, SIG_DFL);
#ifndef _WIN32
    signal(SIGQUIT, SIG_DFL);
#endif

    
    setenv("SDL_VIDEO_ALLOW_SCREENSAVER","1",1);

    flags = SDL_INIT_NOPARACHUTE;
    if (!opts->no_window)
        flags |= SDL_INIT_VIDEO;

    if(SDL_Init(flags)){
        fprintf(stderr, "SDL init failure, reason is: %s\n", SDL_GetError() );
        exit(1);
    }

    if (!opts->no_window) {
        SDL_EnableUNICODE(!opts->raw_keys);
        SDL_EnableKeyRepeat(0,0);

        sdl_set_window_icon();
    }
    else
    {
#ifndef _WIN32
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
#endif
    }
    atexit(sdl_at_exit);

    user_config_get_window_pos(&win_x, &win_y);

    if ( qemulator_init(qemulator_get(), skinConfig, skinPath, win_x, win_y, opts) < 0 ) {
        fprintf(stderr, "### Error: could not load emulator skin from '%s'\n", skinPath);
        exit(1);
    }

    
    if (opts->onion) {
        SkinImage*  onion = skin_image_find_simple( opts->onion );
        int         alpha, rotate;

        if ( opts->onion_alpha && 1 == sscanf( opts->onion_alpha, "%d", &alpha ) ) {
            alpha = (256*alpha)/100;
        } else
            alpha = 128;

        if ( opts->onion_rotation && 1 == sscanf( opts->onion_rotation, "%d", &rotate ) ) {
            rotate &= 3;
        } else
            rotate = SKIN_ROTATION_0;

        qemulator_get()->onion          = onion;
        qemulator_get()->onion_alpha    = alpha;
        qemulator_get()->onion_rotation = rotate;
    }
}

static char*
_getSdkImagePath( const char*  fileName )
{
    char   temp[MAX_PATH];
    char*  p   = temp;
    char*  end = p + sizeof(temp);
    char*  q;
    char*  app;

    static const char* const  searchPaths[] = {
        "",                                  
        "/lib/images",                       
        "/../platforms/android-1.1/images",  
        NULL
    };

    app = bufprint_app_dir(temp, end);
    if (app >= end)
        return NULL;

    do {
        int  nn;

        
        for (nn = 0; searchPaths[nn] != NULL; nn++) {
            p = bufprint(app, end, "%s", searchPaths[nn]);
            q = bufprint(p, end, "/%s", fileName);
            if (q < end && path_exists(temp)) {
                *p = 0;
                goto FOUND_IT;
            }
        }

        p = bufprint(app, end, "/../platforms");
        if (p < end) {
            DirScanner*  scanner = dirScanner_new(temp);
            if (scanner != NULL) {
                int          found = 0;
                const char*  subdir;

                for (;;) {
                    subdir = dirScanner_next(scanner);
                    if (!subdir) break;

                    q = bufprint(p, end, "/%s/images/%s", subdir, fileName);
                    if (q >= end || !path_exists(temp))
                        continue;

                    found = 1;
                    p = bufprint(p, end, "/%s/images", subdir);
                    break;
                }
                dirScanner_free(scanner);
                if (found)
                    break;
            }
        }

        
        return NULL;

    } while (0);

FOUND_IT:
    
    return android_strdup(temp);
}

static char*
_getSdkImage( const char*  path, const char*  file )
{
    char  temp[MAX_PATH];
    char  *p = temp, *end = p + sizeof(temp);

    p = bufprint(temp, end, "%s/%s", path, file);
    if (p >= end || !path_exists(temp))
        return NULL;

    return android_strdup(temp);
}

static char*
_getSdkSystemImage( const char*  path, const char*  optionName, const char*  file )
{
    char*  image = _getSdkImage(path, file);

    if (image == NULL) {
        derror("Your system directory is missing the '%s' image file.\n"
               "Please specify one with the '%s <filepath>' option",
               file, optionName);
        exit(2);
    }
    return image;
}

void sanitizeOptions( AndroidOptions* opts )
{
    if (opts->image != NULL) {
        if (opts->system != NULL) {
            if (opts->sysdir != NULL) {
                derror( "You can't use -sysdir, -system and -image at the same time.\n"
                        "You should probably use '-sysdir <path> -system <file>'.\n" );
                exit(2);
            }
        }
        dwarning( "Please note that -image is obsolete and that -system is now used to point\n"
                  "to the system image. Next time, try using '-sysdir <path> -system <file>' instead.\n" );
        opts->sysdir = opts->system;
        opts->system = opts->image;
        opts->image  = NULL;
    }
    else if (opts->system != NULL && path_is_dir(opts->system)) {
        if (opts->sysdir != NULL) {
            derror( "Option -system should now be followed by a file path, not a directory one.\n"
                    "Please use '-sysdir <path>' to point to the system directory.\n" );
            exit(1);
        }
        dwarning( "Please note that the -system option should now be used to point to the initial\n"
                  "system image (like the obsolete -image option). To point to the system directory\n"
                  "please now use '-sysdir <path>' instead.\n" );

        opts->sysdir = opts->system;
        opts->system = NULL;
    }

    if (opts->nojni) {
        opts->no_jni = opts->nojni;
        opts->nojni  = 0;
    }

    if (opts->nocache) {
        opts->no_cache = opts->nocache;
        opts->nocache  = 0;
    }

    if (opts->noaudio) {
        opts->no_audio = opts->noaudio;
        opts->noaudio  = 0;
    }

    if (opts->noskin) {
        opts->no_skin = opts->noskin;
        opts->noskin  = 0;
    }

    
    if (opts->no_cache) {
        opts->cache = 0;
    }

    if (opts->no_audio)
        opts->audio = "none";

    if (opts->no_skin) {
        opts->skin    = "320x480";
        opts->skindir = NULL;
    }

    if (opts->skindir) {
        if (!opts->skin) {
            derror( "the -skindir <path> option requires a -skin <name> option");
            exit(1);
        }
    }

    if (opts->bootchart) {
        char*  end;
        int    timeout = strtol(opts->bootchart, &end, 10);
        if (timeout == 0)
            opts->bootchart = NULL;
        else if (timeout < 0 || timeout > 15*60) {
            derror( "timeout specified for -bootchart option is invalid.\n"
                    "please use integers between 1 and 900\n");
            exit(1);
        }
    }
}

AvdInfo* createAVD(AndroidOptions* opts, int* inAndroidBuild)
{
    AvdInfo* ret = NULL;
    char   tmp[MAX_PATH];
    char*  tmpend = tmp + sizeof(tmp);
    char*  android_build_root = NULL;
    char*  android_build_out  = NULL;

    if (opts->avd == NULL) {
        do {
            char*  out = getenv("ANDROID_PRODUCT_OUT");

            if (out == NULL || out[0] == 0)
                break;

            if (!path_exists(out)) {
                derror("Can't access ANDROID_PRODUCT_OUT as '%s'\n"
                    "You need to build the Android system before launching the emulator",
                    out);
                exit(2);
            }

            android_build_root = getenv("ANDROID_BUILD_TOP");
            if (android_build_root == NULL || android_build_root[0] == 0)
                break;

            if (!path_exists(android_build_root)) {
                derror("Can't find the Android build root '%s'\n"
                    "Please check the definition of the ANDROID_BUILD_TOP variable.\n"
                    "It should point to the root of your source tree.\n",
                    android_build_root );
                exit(2);
            }
            android_build_out = out;
            D( "found Android build root: %s", android_build_root );
            D( "found Android build out:  %s", android_build_out );
        } while (0);
    }
    if (opts->avd == NULL && !android_build_out)
    {
        char   dataDirIsSystem = 0;

        if (!opts->sysdir) {
            opts->sysdir = _getSdkImagePath("system.img");
            if (!opts->sysdir) {
                derror(
                "You did not specify a virtual device name, and the system\n"
                "directory could not be found.\n\n"
                "If you are an Android SDK user, please use '@<name>' or '-avd <name>'\n"
                "to start a given virtual device (see -help-avd for details).\n\n"

                "Otherwise, follow the instructions in -help-disk-images to start the emulator\n"
                );
                exit(2);
            }
            D("autoconfig: -sysdir %s", opts->sysdir);
        }

        if (!opts->system) {
            opts->system = _getSdkSystemImage(opts->sysdir, "-image", "system.img");
            D("autoconfig: -system %s", opts->system);
        }

        if (!opts->kernel) {
            opts->kernel = _getSdkSystemImage(opts->sysdir, "-kernel", "kernel-qemu");
            D("autoconfig: -kernel %s", opts->kernel);
        }

        if (!opts->ramdisk) {
            opts->ramdisk = _getSdkSystemImage(opts->sysdir, "-ramdisk", "ramdisk.img");
            D("autoconfig: -ramdisk %s", opts->ramdisk);
        }

        
        if (!opts->datadir) {
            opts->datadir   = android_strdup(opts->sysdir);
            dataDirIsSystem = 1;
            D("autoconfig: -datadir %s", opts->sysdir);
        }

        if (!opts->data) {
            
            bufprint(tmp, tmpend, "%s/userdata-qemu.img", opts->datadir);
            if (!path_exists(tmp)) {
                derror(
                "You did not provide the name of an Android Virtual Device\n"
                "with the '-avd <name>' option. Read -help-avd for more information.\n\n"

                "If you *really* want to *NOT* run an AVD, consider using '-data <file>'\n"
                "to specify a data partition image file (I hope you know what you're doing).\n"
                );
                exit(2);
            }

            opts->data = android_strdup(tmp);
            D("autoconfig: -data %s", opts->data);
        }

        if (!opts->snapstorage && opts->datadir) {
            bufprint(tmp, tmpend, "%s/snapshots.img", opts->datadir);
            if (path_exists(tmp)) {
                opts->snapstorage = android_strdup(tmp);
                D("autoconfig: -snapstorage %s", opts->snapstorage);
            }
        }
    }

    if (opts->avd != NULL)
    {
        ret = avdInfo_new( opts->avd, android_avdParams );
        if (ret == NULL) {
            
            dprint("could not find virtual device named '%s'", opts->avd);
            exit(1);
        }
    }
    else
    {
        if (!android_build_out) {
            android_build_out = android_build_root = opts->sysdir;
        }
        ret = avdInfo_newForAndroidBuild(
                            android_build_root,
                            android_build_out,
                            android_avdParams );

        if(ret == NULL) {
            D("could not start virtual device\n");
            exit(1);
        }
    }

    if (android_build_out) {
        *inAndroidBuild = 1;
    } else {
        *inAndroidBuild = 0;
    }

    return ret;
}




#ifdef CONFIG_STANDALONE_UI

#include "android/protocol/core-connection.h"
#include "android/protocol/fb-updates-impl.h"
#include "android/protocol/user-events-proxy.h"
#include "android/protocol/core-commands-proxy.h"
#include "android/protocol/ui-commands-impl.h"
#include "android/protocol/attach-ui-impl.h"

int android_base_port = 0;

#define CORE_BASE_PORT          5554

#define MAX_CORE_PROCS          16

#define CORE_PORT_TIMEOUT_MS    5000

#include "android/async-console.h"

typedef struct {
    LoopIo                 io[1];
    int                    port;
    int                    ok;
    AsyncConsoleConnector  connector[1];
} CoreConsole;

static void
coreconsole_io_func(void* opaque, int fd, unsigned events)
{
    CoreConsole* cc = opaque;
    AsyncStatus  status;
    status = asyncConsoleConnector_run(cc->connector);
    if (status == ASYNC_COMPLETE) {
        cc->ok = 1;
    }
}

static void
coreconsole_init(CoreConsole* cc, const SockAddress* address, Looper* looper)
{
    int fd = socket_create_inet(SOCKET_STREAM);
    AsyncStatus status;
    cc->port = sock_address_get_port(address);
    cc->ok   = 0;
    loopIo_init(cc->io, looper, fd, coreconsole_io_func, cc);
    if (fd >= 0) {
        status = asyncConsoleConnector_connect(cc->connector, address, cc->io);
        if (status == ASYNC_ERROR) {
            cc->ok = 0;
        }
    }
}

static void
coreconsole_done(CoreConsole* cc)
{
    socket_close(cc->io->fd);
    loopIo_done(cc->io);
}

static void
list_running_cores(const char* host)
{
    Looper*         looper;
    CoreConsole     cores[MAX_CORE_PROCS];
    SockAddress     address;
    int             nn, found;

    if (sock_address_init_resolve(&address, host, CORE_BASE_PORT, 0) < 0) {
        derror("Unable to resolve hostname %s: %s", host, errno_str);
        return;
    }

    looper = looper_newGeneric();

    for (nn = 0; nn < MAX_CORE_PROCS; nn++) {
        int port = CORE_BASE_PORT + nn*2;
        sock_address_set_port(&address, port);
        coreconsole_init(&cores[nn], &address, looper);
    }

    looper_runWithTimeout(looper, CORE_PORT_TIMEOUT_MS*2);

    found = 0;
    for (nn = 0; nn < MAX_CORE_PROCS; nn++) {
        int port = CORE_BASE_PORT + nn*2;
        if (cores[nn].ok) {
            if (found == 0) {
                fprintf(stdout, "Running emulator core processes:\n");
            }
            fprintf(stdout, "Emulator console port %d\n", port);
            found++;
        }
        coreconsole_done(&cores[nn]);
    }
    looper_free(looper);

    if (found == 0) {
       fprintf(stdout, "There were no running emulator core processes found on %s.\n",
               host);
    }
}

static int
attach_to_core(AndroidOptions* opts) {
    int iter;
    SockAddress console_socket;
    SockAddress** sockaddr_list;
    QEmulator* emulator;

    
    char* console_address = strdup(opts->attach_core);
    char* host_name = console_address;
    char* port_num = strchr(console_address, ':');
    if (port_num == NULL) {
        
        host_name = "localhost";
        port_num = console_address;
    } else if (port_num == console_address) {
        
        derror("Invalid value %s for -attach-core parameter\n",
               opts->attach_core);
        return -1;
    } else {
        *port_num = '\0';
        port_num++;
        if (*port_num == '\0') {
            
            derror("Invalid value %s for -attach-core parameter\n",
                   opts->attach_core);
            return -1;
        }
    }

    sockaddr_list =
        sock_address_list_create(host_name, port_num, SOCKET_LIST_FORCE_INET);
    free(console_address);
    if (sockaddr_list == NULL) {
        derror("Unable to resolve address %s: %s\n",
               opts->attach_core, errno_str);
        return -1;
    }
    for (iter = 0; sockaddr_list[iter] != NULL; iter++) {
        if (sock_address_get_family(sockaddr_list[iter]) == SOCKET_INET ||
            sock_address_get_family(sockaddr_list[iter]) == SOCKET_IN6) {
            memcpy(&console_socket, sockaddr_list[iter], sizeof(SockAddress));
            break;
        }
    }
    if (sockaddr_list[iter] == NULL) {
        derror("Unable to resolve address %s. Note that 'port' parameter passed to -attach-core\n"
               "must be resolvable into an IP address.\n", opts->attach_core);
        sock_address_list_free(sockaddr_list);
        return -1;
    }
    sock_address_list_free(sockaddr_list);

    if (attachUiImpl_create(&console_socket)) {
        return -1;
    }

    
    android_base_port = sock_address_get_port(&console_socket);
    emulator = qemulator_get();
    qemulator_set_title(emulator);

    return 0;
}


void handle_ui_options( AndroidOptions* opts )
{
    
    if (opts->list_cores) {
        fprintf(stdout, "Enumerating running core processes.\n");
        list_running_cores(opts->list_cores);
        exit(0);
    }
}

int attach_ui_to_core( AndroidOptions* opts )
{
    
    if (opts->attach_core) {
        if (attach_to_core(opts)) {
            return -1;
        }
        
        if (coreCmdProxy_create(attachUiImpl_get_console_socket())) {
            return -1;
        }
        
        if (userEventsProxy_create(attachUiImpl_get_console_socket())) {
            return -1;
        }
    }
    return 0;
}

#else  

void handle_ui_options( AndroidOptions* opts )
{
    return;
}

int attach_ui_to_core( AndroidOptions* opts )
{
    return 0;
}

#endif 
