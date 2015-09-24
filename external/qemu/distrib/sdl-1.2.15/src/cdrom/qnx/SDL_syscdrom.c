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

#ifdef SDL_CDROM_QNX


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/cdrom.h>
#include <sys/dcmd_cam.h>

#include "SDL_timer.h"
#include "SDL_cdrom.h"
#include "../SDL_syscdrom.h"

#define MAX_DRIVES 16

#define QNX_CD_OPENMODE O_RDONLY | O_EXCL

static char *SDL_cdlist[MAX_DRIVES];
static dev_t SDL_cdmode[MAX_DRIVES];
static int   SDL_cdopen[MAX_DRIVES];

static const char *SDL_SYS_CDName(int drive);
static int SDL_SYS_CDOpen(int drive);
static int SDL_SYS_CDGetTOC(SDL_CD *cdrom);
static CDstatus SDL_SYS_CDStatus(SDL_CD *cdrom, int *position);
static int SDL_SYS_CDPlay(SDL_CD *cdrom, int start, int length);
static int SDL_SYS_CDPause(SDL_CD *cdrom);
static int SDL_SYS_CDResume(SDL_CD *cdrom);
static int SDL_SYS_CDStop(SDL_CD *cdrom);
static int SDL_SYS_CDEject(SDL_CD *cdrom);
static void SDL_SYS_CDClose(SDL_CD *cdrom);

static int CheckDrive(char *drive, struct stat *stbuf)
{
    int is_cd, cdfd;
    cam_devinfo_t dinfo;
    int devctlret=0;

    int atapi;
    int removable;
    int cdb10;

    
    if (stat(drive, stbuf) < 0)
    {
        return(-1);
    }

    
    is_cd = 0;

    if (S_ISCHR(stbuf->st_mode) || S_ISBLK(stbuf->st_mode))
    {
        cdfd = open(drive, QNX_CD_OPENMODE);
        if ( cdfd >= 0 )
        {
            devctlret=devctl(cdfd, DCMD_CAM_DEVINFO, &dinfo, sizeof(cam_devinfo_t), NULL);

            if (devctlret==EOK)
            {
               atapi=dinfo.flags & DEV_ATAPI;
               removable=dinfo.flags & DEV_REMOVABLE;
               cdb10=dinfo.flags & DEV_CDB_10; 

               
               if ((atapi)&&(removable))
               {
                   is_cd = 1;
               }
            }

            close(cdfd);
        }
    }
    return(is_cd);
}

static void AddDrive(char *drive, struct stat *stbuf)
{
    int i;

    if (SDL_numcds < MAX_DRIVES)
    {

        for (i=0; i<SDL_numcds; ++i)
        {
            if (stbuf->st_rdev == SDL_cdmode[i])
            {
                return;
            }
        }

        

        i = SDL_numcds;
        SDL_cdlist[i] = SDL_strdup(drive);
        if (SDL_cdlist[i] == NULL)
        {
            SDL_OutOfMemory();
            return;
        }
        SDL_cdmode[i] = stbuf->st_rdev;
        ++SDL_numcds;
    }
}

int SDL_SYS_CDInit(void)
{
    
    static char *checklist[]={"cdrom", "?0 cd?", "?1 cd?", "?0 scd?", NULL};

    char *SDLcdrom;
    int i, j, exists;
    char drive[32];
    struct stat stbuf;

    
    SDL_CDcaps.Name = SDL_SYS_CDName;
    SDL_CDcaps.Open = SDL_SYS_CDOpen;
    SDL_CDcaps.GetTOC = SDL_SYS_CDGetTOC;
    SDL_CDcaps.Status = SDL_SYS_CDStatus;
    SDL_CDcaps.Play = SDL_SYS_CDPlay;
    SDL_CDcaps.Pause = SDL_SYS_CDPause;
    SDL_CDcaps.Resume = SDL_SYS_CDResume;
    SDL_CDcaps.Stop = SDL_SYS_CDStop;
    SDL_CDcaps.Eject = SDL_SYS_CDEject;
    SDL_CDcaps.Close = SDL_SYS_CDClose;

    
    for (i=0; i<MAX_DRIVES; i++)
    {
       SDL_cdopen[i]=0;
    }

    
    SDLcdrom = SDL_getenv("SDL_CDROM");	
    if ( SDLcdrom != NULL )
    {
        char *cdpath, *delim;
	size_t len = SDL_strlen(SDLcdrom)+1;
        cdpath = SDL_stack_alloc(char, len);
        if (cdpath != NULL)
        {
            SDL_strlcpy(cdpath, SDLcdrom, len);
            SDLcdrom = cdpath;
            do {
                delim = SDL_strchr(SDLcdrom, ':');
                if (delim)
                {
                    *delim++ = '\0';
                }
                if (CheckDrive(SDLcdrom, &stbuf) > 0)
                {
                    AddDrive(SDLcdrom, &stbuf);
                }
                if (delim)
                {
                    SDLcdrom = delim;
                }
                else
                {
                    SDLcdrom = NULL;
                }
            } while (SDLcdrom);
            SDL_stack_free(cdpath);
        }

        
        if (SDL_numcds > 0)
        {
            return(0);
        }
    }

    
    for ( i=0; checklist[i]; ++i )
    {
        if (checklist[i][0] == '?')
        {
            char* insert;
            exists = 1;

            for ( j=checklist[i][1]; exists; ++j )
            {
                SDL_snprintf(drive, SDL_arraysize(drive), "/dev/%s", &checklist[i][3]);
                insert = SDL_strchr(drive, '?');
                if (insert != NULL)
                {
                    *insert = j;
                }
                switch (CheckDrive(drive, &stbuf))
                {
                    
                    case 1:
                             AddDrive(drive, &stbuf);
                             break;
                    
                    case 0:
                             break;
                    
                    case -1:
                             exists = 0;
                             break;
                }
            }
        }
        else
        {
            SDL_snprintf(drive, SDL_arraysize(drive), "/dev/%s", checklist[i]);
            if (CheckDrive(drive, &stbuf) > 0)
            {
                AddDrive(drive, &stbuf);
            }
        }
    }
    return(0);
}

static const char *SDL_SYS_CDName(int drive)
{
    return(SDL_cdlist[drive]);
}

static int SDL_SYS_CDOpen(int drive)
{
    int handle;

    handle=open(SDL_cdlist[drive], QNX_CD_OPENMODE);

    if (handle>0)
    {
        SDL_cdopen[drive]=handle;
    }

    return (handle);
}

static int SDL_SYS_CDGetTOC(SDL_CD *cdrom)
{
    cdrom_read_toc_t toc;
    int i, okay;

    okay = 0;
    if (devctl(cdrom->id, DCMD_CAM_CDROMREADTOC, &toc, sizeof(toc), NULL) == 0)
    {
        cdrom->numtracks = toc.last_track - toc.first_track + 1;
        if (cdrom->numtracks > SDL_MAX_TRACKS)
        {
            cdrom->numtracks = SDL_MAX_TRACKS;
        }
        
        for (i=0; i<=cdrom->numtracks; ++i)
        {
            if (i == cdrom->numtracks)
            {
                cdrom->track[i].id = CDROM_LEADOUT;
            }
            else
            {
                cdrom->track[i].id = toc.first_track+i;
            }

            cdrom->track[i].type = toc.toc_entry[i].control_adr & 0x0F;
            cdrom->track[i].offset = toc.toc_entry[i].addr.lba;
            cdrom->track[i].length = 0;

            if (i > 0)
            {
                 cdrom->track[i-1].length = cdrom->track[i].offset-cdrom->track[i-1].offset;
            }
        }
        if (i == (cdrom->numtracks+1))
        {
            okay = 1;
        }
    }
    return (okay ? 0 : -1);
}

static CDstatus SDL_SYS_CDStatus(SDL_CD *cdrom, int *position)
{
    CDstatus status;

    cdrom_read_toc_t toc;
    cdrom_subch_data_t info;
    cam_devinfo_t dinfo;

    int devctlret=0;
    int drive=-1;
    int i;
    int eagaincnt=0;

    
    
    devctlret=devctl(cdrom->id, DCMD_CAM_DEVINFO, &dinfo, sizeof(cam_devinfo_t), NULL);

    if (devctlret==EOK)
    {
        if ((dinfo.flags & DEV_NO_MEDIA)!=0)
        {
            status = CD_TRAYEMPTY;
            if (position)
            {
                *position = 0;
            }
            return (status);
        }
    }

    

    SDL_memset(&info, 0x00, sizeof(info));
    info.subch_command.data_format = CDROM_SUBCH_CURRENT_POSITION;

    do {
        devctlret=devctl(cdrom->id, DCMD_CAM_CDROMSUBCHNL, &info, sizeof(info), NULL);
        if (devctlret==EIO)
        {

            for (i=0; i<MAX_DRIVES; i++)
            {
                if (SDL_cdopen[i]==cdrom->id)
                {
                    drive=i;
                    break;
                }
            }
            if (drive==-1)
            {
               
               break;
            }
            close(cdrom->id);
            cdrom->id=open(SDL_cdlist[drive], QNX_CD_OPENMODE);
            devctlret=EAGAIN;
        }
        if (devctlret==EAGAIN)
        {
            eagaincnt++;
        }
        if (eagaincnt==2)
        {
            
            
            devctlret=ENXIO;
            break;
        }
    } while ((devctlret==EAGAIN)||(devctlret==ESTALE));

    if (devctlret != 0)
    {
        if (devctlret==ENXIO)
        {
            status = CD_TRAYEMPTY;
        }
        else
        {
            status = CD_ERROR;
        }
    }
    else
    {
        switch (info.current_position.header.audio_status)
        {
            case CDROM_AUDIO_INVALID:
            case CDROM_AUDIO_NO_STATUS:
                 
                 if (devctl(cdrom->id, DCMD_CAM_CDROMREADTOC, &toc, sizeof(toc), NULL)==0)
                     status = CD_STOPPED;
                 else
                     status = CD_TRAYEMPTY;
                 break;
            case CDROM_AUDIO_COMPLETED:
                 status = CD_STOPPED;
                 break;
            case CDROM_AUDIO_PLAY:
                 status = CD_PLAYING;
                 break;
            case CDROM_AUDIO_PAUSED:
                 
                 if (info.current_position.data_format == CDROM_LEADOUT)
                 {
                     status = CD_STOPPED;
                 }
                 else
                 {
                     status = CD_PAUSED;
                 }
                 break;
            default:
                 status = CD_ERROR;
                 break;
        }
    }

    if (position)
    {
       if (status==CD_PLAYING || (status==CD_PAUSED))
       {
           *position = MSF_TO_FRAMES(info.current_position.addr.msf.minute,
                                     info.current_position.addr.msf.second,
                                     info.current_position.addr.msf.frame);
       }
       else
       {
           *position = 0;
       }
    }

    return (status);
}

static int SDL_SYS_CDPlay(SDL_CD *cdrom, int start, int length)
{
    cdrom_playmsf_t playtime;

    FRAMES_TO_MSF(start, &playtime.start_minute, &playtime.start_second, &playtime.start_frame);
    FRAMES_TO_MSF(start+length, &playtime.end_minute, &playtime.end_second, &playtime.end_frame);

    if (devctl(cdrom->id, DCMD_CAM_CDROMPLAYMSF, &playtime, sizeof(playtime), NULL) != 0)
    {
       return -1;
    }
    else
    {
       return 0;
    }
}

static int SDL_SYS_CDPause(SDL_CD *cdrom)
{
    if (devctl(cdrom->id, DCMD_CAM_CDROMPAUSE, NULL, 0, NULL)!=0)
    {
       return -1;
    }
    else
    {
       return 0;
    }
}

static int SDL_SYS_CDResume(SDL_CD *cdrom)
{
    if (devctl(cdrom->id, DCMD_CAM_CDROMRESUME, NULL, 0, NULL)!=0)
    {
       return -1;
    }
    else
    {
       return 0;
    }
}

static int SDL_SYS_CDStop(SDL_CD *cdrom)
{
    if (devctl(cdrom->id, DCMD_CAM_CDROMSTOP, NULL, 0, NULL)!=0)
    {
       return -1;
    }
    else
    {
       return 0;
    }
}

static int SDL_SYS_CDEject(SDL_CD *cdrom)
{
    if (devctl(cdrom->id, DCMD_CAM_EJECT_MEDIA, NULL, 0, NULL)!=0)
    {
       return -1;
    }
    else
    {
       return 0;
    }
}

static void SDL_SYS_CDClose(SDL_CD *cdrom)
{
    int i;

    for (i=0; i<MAX_DRIVES; i++)
    {
       if (SDL_cdopen[i]==cdrom->id)
       {
           SDL_cdopen[i]=0;
           break;
       }
    }

    close(cdrom->id);
}

void SDL_SYS_CDQuit(void)
{
    int i;

    if (SDL_numcds > 0)
    {
        for (i=0; i<SDL_numcds; ++i)
        {
            SDL_free(SDL_cdlist[i]);
        }
        SDL_numcds = 0;
    }
}

#endif 
