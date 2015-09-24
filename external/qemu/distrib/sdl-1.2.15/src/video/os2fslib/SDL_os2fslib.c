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

#define _ULS_CALLCONV_
#define CALLCONV _System
#include <unidef.h>                    
#include <uconv.h>                     

#include <process.h>
#include <time.h>

#include "SDL_video.h"
#include "SDL_mouse.h"
#include "../SDL_sysvideo.h"
#include "../SDL_pixels_c.h"
#include "../../events/SDL_events_c.h"

#include "SDL_os2fslib.h"

static ULONG ulFCFToUse =
        FCF_TITLEBAR |
        FCF_SYSMENU |
        FCF_MINBUTTON |
        FCF_MAXBUTTON |
        FCF_NOBYTEALIGN |
        FCF_SIZEBORDER |
        FCF_TASKLIST;

static int bMouseCaptured   = 0;
static int bMouseCapturable = 0;
static HPOINTER hptrGlobalPointer = NULL;
static HPOINTER hptrCurrentIcon = NULL;
static int iWindowSizeX = 320;
static int iWindowSizeY = 200;
static int bWindowResized = 0;

#pragma pack(1)
typedef struct BMPINFO
{
   BITMAPINFO;
   RGB  clr;
} BMPINFO, *PBMPINFO;
#pragma pack()


DECLSPEC void SDLCALL SDL_OS2FSLIB_SetFCFToUse(ULONG ulFCF)
{
  ulFCFToUse = ulFCF;
}


#define REPORT_EMPTY_ALPHA_MASK




static SDLKey HWScanKeyMap[256];
static SDL_keysym *TranslateKey(int vkey, int chcode, int scancode, SDL_keysym *keysym, int iPressed);
static int  iShiftIsPressed;

#ifdef BITBLT_IN_WINMESSAGEPROC
#define WM_UPDATERECTSREQUEST   WM_USER+50
#endif

#ifdef USE_WINLOCKWINDOWUPDATE_AROUND_BITBLTS
#define FSLIB_BITBLT(hwnd, buffer, top, left, width, height) \
    { \
      WinLockWindowUpdate(HWND_DESKTOP, HWND_DESKTOP); \
      FSLib_BitBlt(hwnd, buffer, top, left, width, height); \
      WinLockWindowUpdate(HWND_DESKTOP, NULL); \
    }
#else
#define FSLIB_BITBLT(hwnd, buffer, top, left, width, height) \
    FSLib_BitBlt(hwnd, buffer, top, left, width, height);
#endif

static BOOL SetAccessableWindowPos(HWND hwnd, HWND hwndInsertBehind,
                                   LONG x, LONG y,
                                   LONG cx, LONG cy,
                                   ULONG fl)
{
  SWP swpDesktop, swp;
  
  WinQueryWindowPos(HWND_DESKTOP, &swpDesktop);

  if ((fl & SWP_MOVE) && (fl & SWP_SIZE))
  {
    
    if (x+cx>swpDesktop.cx)
      x = swpDesktop.cx - cx;
    if (x<0)
      x = 0;
    if (y<0)
      y = 0;
    if (y+cy>swpDesktop.cy)
      y = swpDesktop.cy - cy;
    return WinSetWindowPos(hwnd, hwndInsertBehind, x, y, cx, cy, fl);
  } else
  if (fl & SWP_MOVE)
  {
    
    WinQueryWindowPos(hwnd, &swp);
    if (x+swp.cx>swpDesktop.cx)
      x = swpDesktop.cx - swp.cx;
    if (x<0)
      x = 0;
    if (y<0)
      y = 0;
    if (y+swp.cy>swpDesktop.cy)
      y = swpDesktop.cy - swp.cy;
    return WinSetWindowPos(hwnd, hwndInsertBehind, x, y, cx, cy, fl);
  } else
  if (fl & SWP_SIZE)
  {
    
    WinQueryWindowPos(hwnd, &swp);
    x = swp.x;
    y = swp.y;
    if (x+cx>swpDesktop.cx)
      x = swpDesktop.cx - cx;
    if (x<0)
      x = 0;
    if (y<0)
      y = 0;
    if (y+cy>swpDesktop.cy)
      y = swpDesktop.cy - cy;
    return WinSetWindowPos(hwnd, hwndInsertBehind, x, y, cx, cy, fl | SWP_MOVE);
  } else
  return WinSetWindowPos(hwnd, hwndInsertBehind, x, y, cx, cy, fl);
}

static UniChar NativeCharToUniChar(int chcode)
{
  UniChar ucResult = (UniChar) chcode;
  int rc;
  UconvObject ucoTemp;
  char     achFrom[2];
  char     *pchFrom;
  size_t   iFromCount;
  UniChar  aucTo[10];
  UniChar  *pucTo;
  size_t   iToCount;
  size_t   iNonIdentical;

  
  rc = UniCreateUconvObject(L"", &ucoTemp);
  if (rc!=ULS_SUCCESS)
  {
    
    return ucResult;
  }

  
  achFrom[0] = (char) chcode;
  achFrom[1] = 0;
  iFromCount = sizeof(char) * 2;
  iToCount = sizeof(UniChar) * 2;
  pucTo = &(aucTo[0]);
  pchFrom = &(achFrom[0]);

  rc = UniUconvToUcs(ucoTemp,
                     &pchFrom,
                     &iFromCount,
                     &pucTo,
                     &iToCount,
                     &iNonIdentical);

  if (rc!=ULS_SUCCESS)
  {
    
    UniFreeUconvObject(ucoTemp);
    return ucResult;
  }

  UniFreeUconvObject(ucoTemp);

#ifdef DEBUG_BUILD
  printf("%02x converted to %02x\n", (int) chcode, (int) (aucTo[0]));
#endif

  return aucTo[0];
}

static SDL_keysym *TranslateKey(int vkey, int chcode, int scancode, SDL_keysym *keysym, int iPressed)
{
  keysym->scancode = (unsigned char) scancode;
  keysym->mod = KMOD_NONE;
  keysym->unicode = 0;

  if (iPressed && SDL_TranslateUNICODE)
  {
    if (chcode)
      keysym->unicode = NativeCharToUniChar(chcode);
    else
      keysym->unicode = vkey;
  }

  keysym->sym = HWScanKeyMap[scancode];

  
  if (vkey == VK_SHIFT)
  {
    iShiftIsPressed = iPressed;
  }

  if ((iShiftIsPressed) && (SDL_TranslateUNICODE))
  {
    
    
    switch (keysym->sym)
    {
      case SDLK_BACKQUOTE:
        keysym->sym = '~';
        break;
      case SDLK_1:
        keysym->sym = SDLK_EXCLAIM;
        break;
      case SDLK_2:
        keysym->sym = SDLK_AT;
        break;
      case SDLK_3:
        keysym->sym = SDLK_HASH;
        break;
      case SDLK_4:
        keysym->sym = SDLK_DOLLAR;
        break;
      case SDLK_5:
        keysym->sym = '%';
        break;
      case SDLK_6:
        keysym->sym = SDLK_CARET;
        break;
      case SDLK_7:
        keysym->sym = SDLK_AMPERSAND;
        break;
      case SDLK_8:
        keysym->sym = SDLK_ASTERISK;
        break;
      case SDLK_9:
        keysym->sym = SDLK_LEFTPAREN;
        break;
      case SDLK_0:
        keysym->sym = SDLK_RIGHTPAREN;
        break;
      case SDLK_MINUS:
        keysym->sym = SDLK_UNDERSCORE;
        break;
      case SDLK_PLUS:
        keysym->sym = SDLK_EQUALS;
        break;

      case SDLK_LEFTBRACKET:
        keysym->sym = '{';
        break;
      case SDLK_RIGHTBRACKET:
        keysym->sym = '}';
        break;

      case SDLK_SEMICOLON:
        keysym->sym = SDLK_COLON;
        break;
      case SDLK_QUOTE:
        keysym->sym = SDLK_QUOTEDBL;
        break;
      case SDLK_BACKSLASH:
        keysym->sym = '|';
        break;

      case SDLK_COMMA:
        keysym->sym = SDLK_LESS;
        break;
      case SDLK_PERIOD:
        keysym->sym = SDLK_GREATER;
        break;
      case SDLK_SLASH:
        keysym->sym = SDLK_QUESTION;
        break;

      default:
        break;
    }
  }
  return keysym;
}

#define CONVERTMOUSEPOSITION()  \
                                                        \
                                                        \
                                                        \
        if (FSLib_QueryFSMode(hwnd))                                                                                              \
        {                                                                                                                         \
                                                    \
                                                    \
                                                    \
                                                    \
          if (ppts->x<0) ppts->x = 0;                                                                                             \
          if (ppts->y<0) ppts->y = 0;                                                                                             \
          if (ppts->x>=pVideo->hidden->SrcBufferDesc.uiXResolution) ppts->x = pVideo->hidden->SrcBufferDesc.uiXResolution-1;      \
          if (ppts->y>=pVideo->hidden->SrcBufferDesc.uiYResolution) ppts->y = pVideo->hidden->SrcBufferDesc.uiYResolution-1;      \
          pVideo->hidden->iSkipWMMOUSEMOVE++;                                    \
          ptl.x = ppts->x; ptl.y = ppts->y;                                                                                       \
          WinMapWindowPoints(pVideo->hidden->hwndClient, HWND_DESKTOP, &ptl, 1);                                                  \
          WinSetPointerPos(HWND_DESKTOP, ptl.x, ptl.y);                                                                           \
                                                                                  \
          ppts->y = pVideo->hidden->SrcBufferDesc.uiYResolution - ppts->y - 1;                                                    \
        } else                                                                                                                    \
        {                                                                                                                         \
          SWP swpClient;                                                                                                          \
                                                                                                     \
          WinQueryWindowPos(pVideo->hidden->hwndClient, &swpClient);                                                              \
                                                             \
          (ppts->x) = (ppts->x) * pVideo->hidden->SrcBufferDesc.uiXResolution / swpClient.cx;                                       \
          (ppts->y) = (ppts->y) * pVideo->hidden->SrcBufferDesc.uiYResolution / swpClient.cy;                                       \
          (ppts->y) = pVideo->hidden->SrcBufferDesc.uiYResolution - (ppts->y)  - 1;                                                 \
        }



static MRESULT EXPENTRY WndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  HPS ps;
  RECTL rcl;
  SDL_VideoDevice *pVideo = NULL;

  switch (msg)
  {
    case WM_CHAR:  
#ifdef DEBUG_BUILD
#endif
      pVideo = WinQueryWindowPtr(hwnd, 0);
      if (pVideo)
      {

        
        if (SHORT1FROMMP(mp1) & KC_KEYUP)
        {
          
          SDL_keysym keysym;

#ifdef DEBUG_BUILD
#endif

          
          
          
          
          
          

          SDL_PrivateKeyboard(SDL_RELEASED, TranslateKey(SHORT2FROMMP(mp2), 
                                                         SHORT1FROMMP(mp2), 
                                                         CHAR4FROMMP(mp1),  
                                                         &keysym,0));
          
        } else
        {
          
          SDL_keysym keysym;

#ifdef DEBUG_BUILD
#endif
          
          
          if ((SHORT1FROMMP(mp1) & KC_ALT) &&
              (SHORT2FROMMP(mp2) == VK_HOME))
          {
#ifdef DEBUG_BUILD
            printf(" Pressed ALT+HOME!\n"); fflush(stdout);
#endif
            
            
            if (
                (!pVideo->hidden->pSDLSurface) ||
                ((pVideo->hidden->pSDLSurface)
                 && ((pVideo->hidden->pSDLSurface->flags & SDL_RESIZABLE)==0)
                )
               )
              FSLib_ToggleFSMode(hwnd, !FSLib_QueryFSMode(hwnd));
#ifdef DEBUG_BUILD
            else
              printf(" Resizable mode, so discarding ALT+HOME!\n"); fflush(stdout);
#endif
          } else
          if ((SHORT1FROMMP(mp1) & KC_ALT) &&
              (SHORT2FROMMP(mp2) == VK_END))
          {
#ifdef DEBUG_BUILD
            printf(" Pressed ALT+END!\n"); fflush(stdout);
#endif
            
            
            
            SDL_PrivateQuit();
            WinPostMsg(hwnd, WM_QUIT, 0, 0);
          } else
          {
            
            SDL_PrivateKeyboard(SDL_PRESSED, TranslateKey(SHORT2FROMMP(mp2), 
                                                          SHORT1FROMMP(mp2), 
                                                          CHAR4FROMMP(mp1),  
                                                          &keysym,1));
            
          }
        }
      }
      return (MRESULT) TRUE;

    case WM_TRANSLATEACCEL:
      {
        PQMSG pqmsg;
        pqmsg = (PQMSG) mp1;
        if (mp1)
        {
          if (pqmsg->msg == WM_CHAR)
          {
            
            
            return (MRESULT) FALSE;
          }
        }
        break; 
      }

    case WM_PAINT:  
#ifdef DEBUG_BUILD
      printf("WM_PAINT (0x%x)\n", hwnd); fflush(stdout);
#endif
      ps = WinBeginPaint(hwnd,0,&rcl);
      pVideo = FSLib_GetUserParm(hwnd);
      if (pVideo)
      {
        if (!pVideo->hidden->pSDLSurface)
        {
          RECTL rclRect;
          
#ifdef DEBUG_BUILD
          printf("WM_PAINT : Skipping blit while resizing (Pre!)!\n"); fflush(stdout);
#endif
          WinQueryWindowRect(hwnd, &rclRect);
          
          WinFillRect(ps, &rclRect, CLR_BLACK);
        } else
        {
          if (DosRequestMutexSem(pVideo->hidden->hmtxUseSrcBuffer, 1000)==NO_ERROR)
          {
            int iTop, iLeft, iWidth, iHeight;
            int iXScaleError, iYScaleError;
            int iXScaleError2, iYScaleError2;
            SWP swp;
            
            
            
            WinQueryWindowPos(hwnd, &swp);
#ifdef DEBUG_BUILD
            printf("WM_PAINT : WinSize: %d %d, BufSize: %d %d\n",
                   swp.cx,
                   swp.cy,
                   pVideo->hidden->SrcBufferDesc.uiXResolution,
                   pVideo->hidden->SrcBufferDesc.uiYResolution
                  );
            fflush(stdout);
#endif

#ifndef RESIZE_EVEN_IF_RESIZABLE
            
            
            
            if ((!pVideo->hidden->pSDLSurface) ||
                ((pVideo->hidden->pSDLSurface) &&
                 (pVideo->hidden->pSDLSurface->flags & SDL_RESIZABLE) &&
                 ((swp.cx != pVideo->hidden->SrcBufferDesc.uiXResolution) ||
                  (swp.cy != pVideo->hidden->SrcBufferDesc.uiYResolution)
                 ) &&
                 (!FSLib_QueryFSMode(hwnd))
                )
               )
            {
              RECTL rclRect;
              
              
#ifdef DEBUG_BUILD
              printf("WM_PAINT : Skipping blit while resizing!\n"); fflush(stdout);
#endif
              WinQueryWindowRect(hwnd, &rclRect);
              
              WinFillRect(ps, &rclRect, CLR_BLACK);
            } else
#endif
            {
  
              iXScaleError = (pVideo->hidden->SrcBufferDesc.uiXResolution-1) / swp.cx;
              iYScaleError = (pVideo->hidden->SrcBufferDesc.uiYResolution-1) / swp.cy;
              if (iXScaleError<0) iXScaleError = 0;
              if (iYScaleError<0) iYScaleError = 0;
              iXScaleError2 = (swp.cx-1)/(pVideo->hidden->SrcBufferDesc.uiXResolution);
              iYScaleError2 = (swp.cy-1)/(pVideo->hidden->SrcBufferDesc.uiYResolution);
              if (iXScaleError2<0) iXScaleError2 = 0;
              if (iYScaleError2<0) iYScaleError2 = 0;
      
              iTop = (swp.cy - rcl.yTop) * pVideo->hidden->SrcBufferDesc.uiYResolution / swp.cy - iYScaleError;
              iLeft = rcl.xLeft * pVideo->hidden->SrcBufferDesc.uiXResolution / swp.cx - iXScaleError;
              iWidth = ((rcl.xRight-rcl.xLeft) * pVideo->hidden->SrcBufferDesc.uiXResolution + swp.cx-1)
                / swp.cx + 2*iXScaleError;
              iHeight = ((rcl.yTop-rcl.yBottom) * pVideo->hidden->SrcBufferDesc.uiYResolution + swp.cy-1)
                / swp.cy + 2*iYScaleError;
      
              iWidth+=iXScaleError2;
              iHeight+=iYScaleError2;
      
              if (iTop<0) iTop = 0;
              if (iLeft<0) iLeft = 0;
              if (iTop+iHeight>pVideo->hidden->SrcBufferDesc.uiYResolution) iHeight = pVideo->hidden->SrcBufferDesc.uiYResolution-iTop;
              if (iLeft+iWidth>pVideo->hidden->SrcBufferDesc.uiXResolution) iWidth = pVideo->hidden->SrcBufferDesc.uiXResolution-iLeft;
    
#ifdef DEBUG_BUILD
              printf("WM_PAINT : BitBlt: %d %d -> %d %d (Buf %d x %d)\n",
                     iTop, iLeft, iWidth, iHeight,
                     pVideo->hidden->SrcBufferDesc.uiXResolution,
                     pVideo->hidden->SrcBufferDesc.uiYResolution
                    );
              fflush(stdout);
#endif
                    
              FSLIB_BITBLT(hwnd, pVideo->hidden->pchSrcBuffer, iTop, iLeft, iWidth, iHeight);
            }
  
            DosReleaseMutexSem(pVideo->hidden->hmtxUseSrcBuffer);
          }
        }
      }
#ifdef DEBUG_BUILD
      else
      {
        printf("WM_PAINT : No pVideo!\n"); fflush(stdout);
      }
#endif
      WinEndPaint(ps);
#ifdef DEBUG_BUILD
      printf("WM_PAINT : Done.\n");
      fflush(stdout);
#endif
      return 0;

    case WM_SIZE:
      {
#ifdef DEBUG_BUILD
        printf("WM_SIZE : (%d %d)\n",
               SHORT1FROMMP(mp2), SHORT2FROMMP(mp2)); fflush(stdout);
#endif
        iWindowSizeX = SHORT1FROMMP(mp2);
        iWindowSizeY = SHORT2FROMMP(mp2);
        bWindowResized = 1;

        
        WinInvalidateRegion(hwnd, NULL, TRUE);
      }
      break;

    case WM_FSLIBNOTIFICATION:
#ifdef DEBUG_BUILD
        printf("WM_FSLIBNOTIFICATION\n"); fflush(stdout);
#endif
      if ((int)mp1 == FSLN_TOGGLEFSMODE)
      {
        
        pVideo = FSLib_GetUserParm(hwnd);
        if (pVideo)
        {
          if (!pVideo->hidden->pSDLSurface)
          {
            
            
#ifdef DEBUG_BUILD
            printf("WM_FSLIBNOTIFICATION : Can not blit if there is no surface, doing nothing.\n"); fflush(stdout);
#endif
          } else
          {
            if (DosRequestMutexSem(pVideo->hidden->hmtxUseSrcBuffer, 1000)==NO_ERROR)
            {
              if (pVideo->hidden->pSDLSurface)
              {
#ifndef RESIZE_EVEN_IF_RESIZABLE
                SWP swp;

                
                
                
                WinQueryWindowPos(hwnd, &swp);
                if ((!pVideo->hidden->pSDLSurface) ||
                    (
                     (pVideo->hidden->pSDLSurface) &&
                     (pVideo->hidden->pSDLSurface->flags & SDL_RESIZABLE) &&
                     ((swp.cx != pVideo->hidden->SrcBufferDesc.uiXResolution) ||
                      (swp.cy != pVideo->hidden->SrcBufferDesc.uiYResolution)
                     ) &&
                     (!FSLib_QueryFSMode(hwnd))
                    )
                   )
                {
                  
                  
#ifdef DEBUG_BUILD
                  printf("WM_FSLIBNOTIFICATION : Cannot blit while resizing, doing nothing.\n"); fflush(stdout);
#endif
                } else
#endif
                {
#ifdef DEBUG_BUILD
                  printf("WM_FSLIBNOTIFICATION : Blitting!\n"); fflush(stdout);
#endif
                  FSLIB_BITBLT(hwnd, pVideo->hidden->pchSrcBuffer,
                               0, 0,
                               pVideo->hidden->SrcBufferDesc.uiXResolution,
                               pVideo->hidden->SrcBufferDesc.uiYResolution);
                }
              }
#ifdef DEBUG_BUILD
              else
                printf("WM_FSLIBNOTIFICATION : No public surface!\n"); fflush(stdout);
#endif
  
              DosReleaseMutexSem(pVideo->hidden->hmtxUseSrcBuffer);
            }
          }
        }
      }
      return (MPARAM) 1;

    case WM_ACTIVATE:
#ifdef DEBUG_BUILD
      printf("WM_ACTIVATE\n"); fflush(stdout);
#endif

      pVideo = FSLib_GetUserParm(hwnd);
      if (pVideo)
      {
        pVideo->hidden->fInFocus = (int) mp1;
        if (pVideo->hidden->fInFocus)
        {
          
          if ((pVideo->hidden->iMouseVisible) && (!bMouseCaptured))
            WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));
          else
            WinSetPointer(HWND_DESKTOP, NULL);

          if (bMouseCapturable)
          {
            
            WinSetCapture(HWND_DESKTOP, hwnd);
            bMouseCaptured = 1;
            {
              SWP swpClient;
              POINTL ptl;
              
              WinQueryWindowPos(pVideo->hidden->hwndClient, &swpClient);
              ptl.x = 0; ptl.y = 0;
              WinMapWindowPoints(pVideo->hidden->hwndClient, HWND_DESKTOP, &ptl, 1);
              pVideo->hidden->iSkipWMMOUSEMOVE++; 
              WinSetPointerPos(HWND_DESKTOP,
                               ptl.x + swpClient.cx/2,
                               ptl.y + swpClient.cy/2);
            }
          }
        } else
        {
          
          WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));

          if (bMouseCaptured)
          {
            
            WinSetCapture(HWND_DESKTOP, hwnd);
            bMouseCaptured = 0;
          }
        }
      }
#ifdef DEBUG_BUILD
      printf("WM_ACTIVATE done\n"); fflush(stdout);
#endif

      break;

    case WM_BUTTON1DOWN:
#ifdef DEBUG_BUILD
      printf("WM_BUTTON1DOWN\n"); fflush(stdout);
#endif

      pVideo = FSLib_GetUserParm(hwnd);
      if (pVideo)
      {
        SDL_PrivateMouseButton(SDL_PRESSED,
                               SDL_BUTTON_LEFT,
                               0, 0); 

        if (bMouseCapturable)
        {
          
          if (!bMouseCaptured)
          {
            WinSetCapture(HWND_DESKTOP, hwnd);
            WinSetPointer(HWND_DESKTOP, NULL);
            bMouseCaptured = 1;
            {
              SWP swpClient;
              POINTL ptl;
              
              WinQueryWindowPos(pVideo->hidden->hwndClient, &swpClient);
              ptl.x = 0; ptl.y = 0;
              WinMapWindowPoints(pVideo->hidden->hwndClient, HWND_DESKTOP, &ptl, 1);
              pVideo->hidden->iSkipWMMOUSEMOVE++; 
              WinSetPointerPos(HWND_DESKTOP,
                               ptl.x + swpClient.cx/2,
                               ptl.y + swpClient.cy/2);
            }
          }
        }
      }
      break;
    case WM_BUTTON1UP:
#ifdef DEBUG_BUILD
      printf("WM_BUTTON1UP\n"); fflush(stdout);
#endif
      SDL_PrivateMouseButton(SDL_RELEASED,
                             SDL_BUTTON_LEFT,
                             0, 0); 
      break;
    case WM_BUTTON2DOWN:
#ifdef DEBUG_BUILD
      printf("WM_BUTTON2DOWN\n"); fflush(stdout);
#endif

      pVideo = FSLib_GetUserParm(hwnd);
      if (pVideo)
      {
        SDL_PrivateMouseButton(SDL_PRESSED,
                               SDL_BUTTON_RIGHT,
                               0, 0); 

        if (bMouseCapturable)
        {
          
          if (!bMouseCaptured)
          {
            WinSetCapture(HWND_DESKTOP, hwnd);
            WinSetPointer(HWND_DESKTOP, NULL);
            bMouseCaptured = 1;
            {
              SWP swpClient;
              POINTL ptl;
              
              WinQueryWindowPos(pVideo->hidden->hwndClient, &swpClient);
              ptl.x = 0; ptl.y = 0;
              WinMapWindowPoints(pVideo->hidden->hwndClient, HWND_DESKTOP, &ptl, 1);
              pVideo->hidden->iSkipWMMOUSEMOVE++; 
              WinSetPointerPos(HWND_DESKTOP,
                               ptl.x + swpClient.cx/2,
                               ptl.y + swpClient.cy/2);
            }
          }
        }

      }
      break;
    case WM_BUTTON2UP:
#ifdef DEBUG_BUILD
      printf("WM_BUTTON2UP\n"); fflush(stdout);
#endif
      SDL_PrivateMouseButton(SDL_RELEASED,
                             SDL_BUTTON_RIGHT,
                             0, 0); 
      break;
    case WM_BUTTON3DOWN:
#ifdef DEBUG_BUILD
      printf("WM_BUTTON3DOWN\n"); fflush(stdout);
#endif

      pVideo = FSLib_GetUserParm(hwnd);
      if (pVideo)
      {
        SDL_PrivateMouseButton(SDL_PRESSED,
                               SDL_BUTTON_MIDDLE,
                               0, 0); 
        
        if (bMouseCapturable)
        {
          
          if (!bMouseCaptured)
          {
            WinSetCapture(HWND_DESKTOP, hwnd);
            WinSetPointer(HWND_DESKTOP, NULL);
            bMouseCaptured = 1;
            {
              SWP swpClient;
              POINTL ptl;
              
              WinQueryWindowPos(pVideo->hidden->hwndClient, &swpClient);
              ptl.x = 0; ptl.y = 0;
              WinMapWindowPoints(pVideo->hidden->hwndClient, HWND_DESKTOP, &ptl, 1);
              pVideo->hidden->iSkipWMMOUSEMOVE++; 
              WinSetPointerPos(HWND_DESKTOP,
                               ptl.x + swpClient.cx/2,
                               ptl.y + swpClient.cy/2);
            }
          }
        }
      }
      break;
    case WM_BUTTON3UP:
#ifdef DEBUG_BUILD
      printf("WM_BUTTON3UP\n"); fflush(stdout);
#endif
      SDL_PrivateMouseButton(SDL_RELEASED,
                             SDL_BUTTON_MIDDLE,
                             0, 0); 
      break;
    case WM_MOUSEMOVE:
#ifdef DEBUG_BUILD
#endif

      pVideo = FSLib_GetUserParm(hwnd);
      if (pVideo)
      {
        if (pVideo->hidden->iSkipWMMOUSEMOVE)
        {
          pVideo->hidden->iSkipWMMOUSEMOVE--;
        } else
        {
          POINTS *ppts = (POINTS *) (&mp1);
          POINTL ptl;

          if (bMouseCaptured)
          {
            SWP swpClient;

            WinQueryWindowPos(pVideo->hidden->hwndClient, &swpClient);

            
            
            SDL_PrivateMouseMotion(0, 
                                   1, 
                                   ppts->x - (swpClient.cx/2),
                                   (swpClient.cy/2) - ppts->y);

            ptl.x = 0; ptl.y = 0;
            WinMapWindowPoints(pVideo->hidden->hwndClient, HWND_DESKTOP, &ptl, 1);
            pVideo->hidden->iSkipWMMOUSEMOVE++; 
            
            WinSetPointerPos(HWND_DESKTOP,
                             ptl.x + swpClient.cx/2,
                             ptl.y + swpClient.cy/2);
          } else
          {
            CONVERTMOUSEPOSITION();

            
            SDL_PrivateMouseMotion(0, 
                                   0, 
                                   ppts->x,
                                   ppts->y);
          }
        }
        if ((pVideo->hidden->iMouseVisible) && (!bMouseCaptured))
        {
#ifdef DEBUG_BUILD
#endif

          if (hptrGlobalPointer)
            WinSetPointer(HWND_DESKTOP, hptrGlobalPointer);
          else
            WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));
        }
        else
        {
          WinSetPointer(HWND_DESKTOP, NULL);
        }
      }
#ifdef DEBUG_BUILD
#endif

      return (MRESULT) FALSE;
    case WM_CLOSE: 
#ifdef DEBUG_BUILD
      printf("WM_CLOSE\n"); fflush(stdout);
#endif

      pVideo = FSLib_GetUserParm(hwnd);
      if (pVideo)
      {
        
        SDL_PrivateQuit();
        return 0;
      }
      break;

#ifdef BITBLT_IN_WINMESSAGEPROC
    case WM_UPDATERECTSREQUEST:
      pVideo = FSLib_GetUserParm(hwnd);
      if ((pVideo) && (pVideo->hidden->pSDLSurface))
      {
        if (DosRequestMutexSem(pVideo->hidden->hmtxUseSrcBuffer, SEM_INDEFINITE_WAIT)==NO_ERROR)
        {
          int numrects;
          SDL_Rect *rects;
          int i;
          SWP swp;

          numrects = (int) mp1;
          rects = (SDL_Rect *) mp2;

          WinQueryWindowPos(hwnd, &swp);
#ifndef RESIZE_EVEN_IF_RESIZABLE
          if ((!pVideo->hidden->pSDLSurface) ||
              (
               (pVideo->hidden->pSDLSurface) &&
               (pVideo->hidden->pSDLSurface->flags & SDL_RESIZABLE) &&
               ((swp.cx != pVideo->hidden->SrcBufferDesc.uiXResolution) ||
                (swp.cy != pVideo->hidden->SrcBufferDesc.uiYResolution)
               ) &&
               (!FSLib_QueryFSMode(hwnd))
              )
             )
          {
            
            
#ifdef DEBUG_BUILD
            printf("[WM_UPDATERECTSREQUEST] : Skipping blit while resizing!\n"); fflush(stdout);
#endif
          } else
#endif
          {
#ifdef DEBUG_BUILD
            printf("[WM_UPDATERECTSREQUEST] : Blitting!\n"); fflush(stdout);
#endif
          
            
            for (i=0; i<numrects; i++)
              FSLIB_BITBLT(hwnd, pVideo->hidden->pchSrcBuffer,
                           rects[i].y, rects[i].x, rects[i].w, rects[i].h);
          }
          DosReleaseMutexSem(pVideo->hidden->hmtxUseSrcBuffer);
        }
      }
      return 0;
#endif

    default:
#ifdef DEBUG_BUILD
      printf("Unhandled: %x\n", msg); fflush(stdout);
#endif

      break;
  }
  
  return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

static MRESULT EXPENTRY FrameWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PFNWP pOldFrameProc;
  MRESULT result;
  PTRACKINFO ti;
  int cx, cy, ncx, ncy;
  RECTL rclTemp;
  PSWP pswpTemp;

  SDL_VideoDevice *pVideo = NULL;

  pVideo = (SDL_VideoDevice *) WinQueryWindowULong(hwnd, QWL_USER);

  pOldFrameProc = pVideo->hidden->pfnOldFrameProc;

  if ((pVideo->hidden->bProportionalResize) &&
      (msg==WM_ADJUSTWINDOWPOS) &&
      (!FSLib_QueryFSMode(pVideo->hidden->hwndClient))
     )
  {
    pswpTemp = (PSWP) mp1;

    
    if (pswpTemp->fl & SWP_SIZE)
    {
      
      rclTemp.xLeft = pswpTemp->x;
      rclTemp.xRight = pswpTemp->x + pswpTemp->cx;
      rclTemp.yBottom = pswpTemp->y;
      rclTemp.yTop = pswpTemp->y + pswpTemp->cy;
      WinCalcFrameRect(hwnd, &rclTemp, TRUE);

      ncx = cx = rclTemp.xRight - rclTemp.xLeft;
      ncy = cy = rclTemp.yTop - rclTemp.yBottom;

      

      if ((pVideo->hidden->ulResizingFlag & TF_LEFT) || (pVideo->hidden->ulResizingFlag & TF_RIGHT))
      {
        
        ncy = pVideo->hidden->SrcBufferDesc.uiYResolution * cx / pVideo->hidden->SrcBufferDesc.uiXResolution;
      } else
      if ((pVideo->hidden->ulResizingFlag & TF_TOP) || (pVideo->hidden->ulResizingFlag & TF_BOTTOM))
      {
        
        ncx = pVideo->hidden->SrcBufferDesc.uiXResolution * cy / pVideo->hidden->SrcBufferDesc.uiYResolution;
      }

      
      rclTemp.xLeft = pswpTemp->x;
      rclTemp.xRight = pswpTemp->x + ncx;
      rclTemp.yBottom = pswpTemp->y;
      rclTemp.yTop = pswpTemp->y + ncy;
      WinCalcFrameRect(hwnd, &rclTemp, FALSE);

      
      pswpTemp->cx = rclTemp.xRight - rclTemp.xLeft;

      if (!(pVideo->hidden->ulResizingFlag & TF_TOP))
      {
        pswpTemp->y = pswpTemp->y + pswpTemp->cy - (rclTemp.yTop - rclTemp.yBottom);
        pswpTemp->cy = rclTemp.yTop - rclTemp.yBottom;
      } else
      {
        pswpTemp->cy = rclTemp.yTop - rclTemp.yBottom;
      }
    }
  }

  result = (*pOldFrameProc)(hwnd, msg, mp1, mp2);

  if ((pVideo->hidden->bProportionalResize) && (msg==WM_QUERYTRACKINFO))
  {
    ti = (PTRACKINFO) mp2;

    
    if ((ti->fs & TF_LEFT) || (ti->fs & TF_RIGHT) ||
        (ti->fs & TF_TOP) || (ti->fs & TF_BOTTOM))
      pVideo->hidden->ulResizingFlag = ti->fs;
  }

  return result;
}

int iNumOfPMThreadInstances = 0; 
static void PMThreadFunc(void *pParm)
{
  SDL_VideoDevice *pVideo = pParm;
  HAB hab;
  HMQ hmq;
  QMSG msg;
  ULONG fcf;

#ifdef DEBUG_BUILD
  printf("[PMThreadFunc] : Starting\n"); fflush(stdout);
#endif

  iNumOfPMThreadInstances++;

  

  hab=WinInitialize(0);
  hmq=WinCreateMsgQueue(hab,0);
  if (hmq==0)
  {
#ifdef DEBUG_BUILD
    printf("[PMThreadFunc] : Could not create message queue!\n");
    printf("                 It might be that the application using SDL is not a PM app!\n");
    fflush(stdout);
#endif
    pVideo->hidden->iPMThreadStatus = 2;
  } else
  {
    int rc;
    RECTL rectl;

    fcf = ulFCFToUse; 

#ifdef DEBUG_BUILD
    printf("[PMThreadFunc] : FSLib_CreateWindow()!\n");
    fflush(stdout);
#endif

    rc = FSLib_CreateWindow(HWND_DESKTOP, 0, &fcf,
                            "SDL Application",
                            NULLHANDLE, 0,
                            &(pVideo->hidden->SrcBufferDesc),
                            WndProc,
                            &(pVideo->hidden->hwndClient),
                            &(pVideo->hidden->hwndFrame));

#ifdef DEBUG_BUILD
    printf("[PMThreadFunc] : FSLib_CreateWindow() rc = %d\n", rc);
    fflush(stdout);
#endif

    if (!rc)
    {
#ifdef DEBUG_BUILD
      printf("[PMThreadFunc] : Could not create FSLib window!\n");
      fflush(stdout);
#endif
      pVideo->hidden->iPMThreadStatus = 3;
    } else
    {
#ifdef DEBUG_BUILD
      printf("[PMThreadFunc] : FSLib_AddUserParm()!\n");
      fflush(stdout);
#endif

      
      
      FSLib_AddUserParm(pVideo->hidden->hwndClient, pVideo);

      
#ifdef DEBUG_BUILD
      printf("[PMThreadFunc] : SetWindowPos()!\n");
      fflush(stdout);
#endif

      
      
      
      rectl.xLeft = 0;
      rectl.yBottom = 0;
      rectl.xRight = pVideo->hidden->SrcBufferDesc.uiXResolution; 
      rectl.yTop = pVideo->hidden->SrcBufferDesc.uiYResolution; 
      WinCalcFrameRect(pVideo->hidden->hwndFrame, &rectl, FALSE);

      SetAccessableWindowPos(pVideo->hidden->hwndFrame,
                             HWND_TOP,
                             (WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN) - (rectl.xRight-rectl.xLeft)) / 2,
                             (WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN) - (rectl.yTop-rectl.yBottom)) / 2,
                             (rectl.xRight-rectl.xLeft),
                             (rectl.yTop-rectl.yBottom),
                             SWP_SIZE | SWP_ACTIVATE | SWP_SHOW | SWP_MOVE);

      
      pVideo->hidden->pfnOldFrameProc =
        WinSubclassWindow(pVideo->hidden->hwndFrame, FrameWndProc);
      WinSetWindowULong(pVideo->hidden->hwndFrame, QWL_USER, (ULONG) pVideo);

#ifdef DEBUG_BUILD
      printf("[PMThreadFunc] : Entering message loop\n"); fflush(stdout);
#endif
      pVideo->hidden->iPMThreadStatus = 1;
  
      while (WinGetMsg(hab, (PQMSG)&msg, 0, 0, 0))
        WinDispatchMsg(hab, (PQMSG) &msg);

#ifdef DEBUG_BUILD
      printf("[PMThreadFunc] : Leaving message loop\n"); fflush(stdout);
#endif
      
      if (bMouseCaptured)
      {
        WinSetCapture(HWND_DESKTOP, NULLHANDLE);
        bMouseCaptured = 0;
      }
      
      WinDestroyWindow(pVideo->hidden->hwndFrame); pVideo->hidden->hwndFrame=NULL;
      
      WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE));
      WinShowPointer(HWND_DESKTOP, TRUE);
    }
    
    WinDestroyMsgQueue(hmq);
    
    pVideo->hidden->iPMThreadStatus = 0;
  }
  WinTerminate(hab);
#ifdef DEBUG_BUILD
  printf("[PMThreadFunc] : End, status is %d!\n", pVideo->hidden->iPMThreadStatus); fflush(stdout);
#endif

  iNumOfPMThreadInstances--;

  
  
  
  
  
  
  
  
  if (pVideo->hidden->iPMThreadStatus==0)
  {
    DosSleep(5000); 
    
    
    if (iNumOfPMThreadInstances==0)
    {
#ifdef DEBUG_BUILD
      printf("[PMThreadFunc] : It seems that the application haven't terminated itself\n"); fflush(stdout);
      printf("[PMThreadFunc] : in the last 5 seconds, so we go berserk.\n"); fflush(stdout);
      printf("[PMThreadFunc] : Brute force mode. :) Killing process! Dieeeee...\n"); fflush(stdout);
#endif
      DosExit(EXIT_PROCESS, -1);
    }
  }
  _endthread();
}

struct WMcursor
{
  HBITMAP hbm;
  HPOINTER hptr;
  char *pchData;
};

void os2fslib_FreeWMCursor(_THIS, WMcursor *cursor)
{
  if (cursor)
  {
    GpiDeleteBitmap(cursor->hbm);
    WinDestroyPointer(cursor->hptr);
    SDL_free(cursor->pchData);
    SDL_free(cursor);
  }
}

static void memnot(Uint8 *dst, Uint8 *src, int len)
{
  while ( len-- > 0 )
    *dst++ = ~*src++;
}
static void memxor(Uint8 *dst, Uint8 *src1, Uint8 *src2, int len)
{
  while ( len-- > 0 )
    *dst++ = (*src1++)^(*src2++);
}

WMcursor *os2fslib_CreateWMCursor_Win(_THIS, Uint8 *data, Uint8 *mask,
                                      int w, int h, int hot_x, int hot_y)
{
  HPOINTER hptr;
  HBITMAP hbm;
  BITMAPINFOHEADER bmih;
  BMPINFO          bmi;
  HPS              hps;
  char *pchTemp;
  char *xptr, *aptr;
  int maxx, maxy;
  int i, run, pad;
  WMcursor *pResult;

  maxx = WinQuerySysValue(HWND_DESKTOP, SV_CXPOINTER);
  maxy = WinQuerySysValue(HWND_DESKTOP, SV_CYPOINTER);

  
  if ((w>maxx) || (h>maxy))
    return (WMcursor *) NULL;

  pResult = (WMcursor *) SDL_malloc(sizeof(WMcursor));
  if (!pResult) return (WMcursor *) NULL;

  pchTemp = (char *) SDL_malloc((maxx + 7)/8 * maxy*2);
  if (!pchTemp)
  {
    SDL_free(pResult);
    return (WMcursor *) NULL;
  }

  SDL_memset(pchTemp, 0, (maxx + 7)/8 * maxy*2);

  hps = WinGetPS(_this->hidden->hwndClient);

  bmi.cbFix = sizeof(BITMAPINFOHEADER);
  bmi.cx = maxx;
  bmi.cy = 2*maxy;
  bmi.cPlanes = 1;
  bmi.cBitCount = 1;
  bmi.argbColor[0].bBlue = 0x00;
  bmi.argbColor[0].bGreen = 0x00;
  bmi.argbColor[0].bRed = 0x00;
  bmi.argbColor[1].bBlue = 0x00;
  bmi.argbColor[1].bGreen = 0x00;
  bmi.argbColor[1].bRed = 0xff;

  SDL_memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
  bmih.cbFix = sizeof(BITMAPINFOHEADER);
  bmih.cx = maxx;
  bmih.cy = 2*maxy;
  bmih.cPlanes = 1;
  bmih.cBitCount = 1;

  run = (w+7)/8;
  pad = (maxx+7)/8 - run;

  for (i=0; i<h; i++)
  {
    xptr = pchTemp + (maxx+7)/8 * (maxy-1-i);
    aptr = pchTemp + (maxx+7)/8 * (maxy+maxy-1-i);
    memxor(xptr, data, mask, run);
    xptr += run;
    data += run;
    memnot(aptr, mask, run);
    mask += run;
    aptr += run;
    SDL_memset(xptr,  0, pad);
    xptr += pad;
    SDL_memset(aptr, ~0, pad);
    aptr += pad;
  }
  pad += run;
  for (i=h ; i<maxy; i++ )
  {
    xptr = pchTemp + (maxx+7)/8 * (maxy-1-i);
    aptr = pchTemp + (maxx+7)/8 * (maxy+maxy-1-i);

    SDL_memset(xptr,  0, (maxx+7)/8);
    xptr += (maxx+7)/8;
    SDL_memset(aptr, ~0, (maxx+7)/8);
    aptr += (maxx+7)/8;
  }

  hbm = GpiCreateBitmap(hps, (PBITMAPINFOHEADER2)&bmih, CBM_INIT, (PBYTE) pchTemp, (PBITMAPINFO2)&bmi);
  hptr = WinCreatePointer(HWND_DESKTOP, hbm, TRUE, hot_x, maxy - hot_y - 1);

#ifdef DEBUG_BUILD
  printf("HotSpot          : %d ; %d\n", hot_x, hot_y);
  printf("HPS returned     : %x\n", (ULONG)hps);
  printf("HBITMAP returned : %x\n", (ULONG)hbm);
  printf("HPOINTER returned: %x\n", (ULONG)hptr);
#endif

  WinReleasePS(hps);

#ifdef DEBUG_BUILD
  printf("[CreateWMCursor] : ptr = %p\n", hptr); fflush(stdout);
#endif

  pResult->hptr = hptr;
  pResult->hbm = hbm;
  pResult->pchData = pchTemp;

#ifdef DEBUG_BUILD
  printf("[CreateWMCursor] : ptr = %p return.\n", hptr); fflush(stdout);
#endif

  return (WMcursor *) pResult;
}

WMcursor *os2fslib_CreateWMCursor_FS(_THIS, Uint8 *data, Uint8 *mask,
                                     int w, int h, int hot_x, int hot_y)
{
#ifdef DEBUG_BUILD
  printf("[CreateWMCursor_FS] : returning pointer NULL\n"); fflush(stdout);
#endif

  
  return (WMcursor *) NULL;
}

int os2fslib_ShowWMCursor(_THIS, WMcursor *cursor)
{
#ifdef DEBUG_BUILD
  printf("[ShowWMCursor] : ptr = %p\n", cursor); fflush(stdout);
#endif

  if (cursor)
  {
    WinSetPointer(HWND_DESKTOP, cursor->hptr);
    hptrGlobalPointer = cursor->hptr;
    _this->hidden->iMouseVisible = 1;
  }
  else
  {
    WinSetPointer(HWND_DESKTOP, FALSE);
    hptrGlobalPointer = NULL;
    _this->hidden->iMouseVisible = 0;
  }

#ifdef DEBUG_BUILD
  printf("[ShowWMCursor] : ptr = %p, DONE\n", cursor); fflush(stdout);
#endif

  return 1;
}

void os2fslib_WarpWMCursor(_THIS, Uint16 x, Uint16 y)
{
  LONG lx, ly;
  SWP swpClient;
  POINTL ptlPoints;
  WinQueryWindowPos(_this->hidden->hwndClient, &swpClient);
  ptlPoints.x = swpClient.x;
  ptlPoints.y = swpClient.y;
  WinMapWindowPoints(_this->hidden->hwndFrame, HWND_DESKTOP, &ptlPoints, 1);
  lx = ptlPoints.x + (x*swpClient.cx) / _this->hidden->SrcBufferDesc.uiXResolution;
  ly = ptlPoints.y + swpClient.cy - ((y*swpClient.cy) / _this->hidden->SrcBufferDesc.uiYResolution) - 1;

  SDL_PrivateMouseMotion(0, 
                         0, 
                         x,
                         y);

  WinSetPointerPos(HWND_DESKTOP, lx, ly);

}

void os2fslib_MoveWMCursor(_THIS, int x, int y)
{
}

void os2fslib_CheckMouseMode(_THIS)
{
}

static void os2fslib_PumpEvents(_THIS)
{
  
  if (
      (_this->hidden->pSDLSurface) &&
      (_this->hidden->pSDLSurface->flags & SDL_RESIZABLE) &&
      (
       (_this->hidden->SrcBufferDesc.uiXResolution!=iWindowSizeX) ||
       (_this->hidden->SrcBufferDesc.uiYResolution!=iWindowSizeY)
      ) &&
      (iWindowSizeX>0) &&
      (iWindowSizeY>0)
     )
  {
    static time_t prev_time;
    time_t curr_time;

    curr_time = time(NULL);
    if ((difftime(curr_time, prev_time)>=0.25) ||
        (bWindowResized))
    {
      
      
      
#ifdef DEBUG_BUILD
      printf("[os2fslib_PumpEvents] : Calling PrivateResize (%d %d).\n",
             iWindowSizeX, iWindowSizeY);
      fflush(stdout);
#endif
      
      SDL_PrivateResize(iWindowSizeX, iWindowSizeY);
      prev_time = curr_time;
      bWindowResized = 0;
    }
  }
}

static int os2fslib_AllocHWSurface(_THIS, SDL_Surface *surface)
{
  return(-1);
}
static void os2fslib_FreeHWSurface(_THIS, SDL_Surface *surface)
{
  return;
}

static int os2fslib_LockHWSurface(_THIS, SDL_Surface *surface)
{
  return(0);
}

static void os2fslib_UnlockHWSurface(_THIS, SDL_Surface *surface)
{
  return;
}

static int os2fslib_SetColors(_THIS, int firstcolor, int ncolors, SDL_Color *colors)
{
  printf("[os2fslib_SetColors] : TODO!\n"); fflush(stdout);
  
  return(1);
}

static void os2fslib_DestroyIcon(HWND hwndFrame)
{
  if (hptrCurrentIcon)
  {
    WinDestroyPointer(hptrCurrentIcon);
    hptrCurrentIcon = NULL;

    WinSendMsg(hwndFrame,
               WM_SETICON,
               NULL,
               NULL);
  }

}

void os2fslib_SetIcon(_THIS, SDL_Surface *icon, Uint8 *mask)
{
  HWND hwndFrame;
  SDL_Surface *icon_rgb;
  HPOINTER hptrIcon;
  HBITMAP hbm;
  BITMAPINFOHEADER bmih;
  BMPINFO          bmi;
  HPS              hps;
  char *pchTemp;
  char *pptr, *mptr, *dptr, *dmptr;
  int maxx, maxy, w, h, x, y;
  SDL_Rect bounds;

#ifdef DEBUG_BUILD
  printf("[os2fslib_SetIcon] : Creating and setting new icon\n"); fflush(stdout);
#endif

  hwndFrame = WinQueryWindow(_this->hidden->hwndClient, QW_PARENT);

  
  os2fslib_DestroyIcon(hwndFrame);

  if ((!icon) || (!mask))
    return;

  w = icon->w;
  h = icon->h;

  maxx = WinQuerySysValue(HWND_DESKTOP, SV_CXICON);
  maxy = WinQuerySysValue(HWND_DESKTOP, SV_CYICON);

  
  if ((w>maxx) || (h>maxy))
    return;

  pchTemp = (char *) SDL_malloc(w * h*2 * 4);
  if (!pchTemp)
    return;

  SDL_memset(pchTemp, 0, w * h*2 * 4);

  
  icon_rgb = SDL_CreateRGBSurface(SDL_SWSURFACE, icon->w, icon->h,
                                  32, 0, 0, 0, 0);
  if ( icon_rgb == NULL )
  {
    SDL_free(pchTemp);
    return;
  }
  bounds.x = 0;
  bounds.y = 0;
  bounds.w = icon->w;
  bounds.h = icon->h;
  if ( SDL_LowerBlit(icon, &bounds, icon_rgb, &bounds) < 0 )
  {
    SDL_FreeSurface(icon_rgb);
    SDL_free(pchTemp);
    return;
  }

  

  
  pptr = (char *) (icon_rgb->pixels);
  
  mptr = mask;

  for (y=0; y<h; y++)
  {
    unsigned char uchMaskByte;

    
    dptr = pchTemp + w*4 * (h-y-1);
    
    dmptr = pchTemp + w*h*4 + w*4 * (h-y-1);

    for (x=0; x<w; x++)
    {
      if (x%8==0)
      {
        uchMaskByte = (unsigned char) (*mptr);
        mptr++;
      } else
        uchMaskByte <<= 1;

      if (uchMaskByte & 0x80)
      {
        
        *dptr++ = *pptr++;
        *dptr++ = *pptr++;
        *dptr++ = *pptr++;
        *dptr++ = *pptr++;

        *dmptr++ = 0;
        *dmptr++ = 0;
        *dmptr++ = 0;
        *dmptr++ = 0;
      } else
      {
        
        *dptr++ = 0; pptr++;
        *dptr++ = 0; pptr++;
        *dptr++ = 0; pptr++;
        *dptr++ = 0; pptr++;

        *dmptr++ = 255;
        *dmptr++ = 255;
        *dmptr++ = 255;
        *dmptr++ = 255;
      }
    }
  }

  
  SDL_FreeSurface(icon_rgb);

  hps = WinGetPS(_this->hidden->hwndClient);

  bmi.cbFix = sizeof(BITMAPINFOHEADER);
  bmi.cx = w;
  bmi.cy = 2*h;
  bmi.cPlanes = 1;
  bmi.cBitCount = 32;

  SDL_memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
  bmih.cbFix = sizeof(BITMAPINFOHEADER);
  bmih.cx = w;
  bmih.cy = 2*h;
  bmih.cPlanes = 1;
  bmih.cBitCount = 32;

  hbm = GpiCreateBitmap(hps, (PBITMAPINFOHEADER2)&bmih, CBM_INIT, (PBYTE) pchTemp, (PBITMAPINFO2)&bmi);
  hptrIcon = WinCreatePointer(HWND_DESKTOP, hbm, FALSE, 0, 0);

  WinReleasePS(hps);

  
  SDL_free(pchTemp);

  
  WinSendMsg(hwndFrame,
             WM_SETICON,
             (MPARAM) hptrIcon,
             NULL);


  
  hptrCurrentIcon = hptrIcon;
}



static void os2fslib_SetCursorManagementFunctions(_THIS, int iForWindowedMode)
{
  if (iForWindowedMode)
  {
    _this->FreeWMCursor = os2fslib_FreeWMCursor;
    _this->CreateWMCursor = os2fslib_CreateWMCursor_Win;
    _this->ShowWMCursor = os2fslib_ShowWMCursor;
    _this->WarpWMCursor = os2fslib_WarpWMCursor;
    _this->MoveWMCursor = os2fslib_MoveWMCursor;
    _this->CheckMouseMode = NULL;
  } else
  {
    
    _this->FreeWMCursor = os2fslib_FreeWMCursor;
    _this->CreateWMCursor = os2fslib_CreateWMCursor_FS;
    _this->ShowWMCursor = os2fslib_ShowWMCursor;
    _this->WarpWMCursor = os2fslib_WarpWMCursor;
    _this->MoveWMCursor = os2fslib_MoveWMCursor;
    _this->CheckMouseMode = NULL;
  }
}

static void os2fslib_InitOSKeymap(_THIS)
{
  int i;

  iShiftIsPressed = 0;

  
  for ( i=0; i<=255; ++i )
    HWScanKeyMap[i] = SDLK_UNKNOWN;

  
  HWScanKeyMap[0x1] = SDLK_ESCAPE;
  HWScanKeyMap[0x3b] = SDLK_F1;
  HWScanKeyMap[0x3c] = SDLK_F2;
  HWScanKeyMap[0x3d] = SDLK_F3;
  HWScanKeyMap[0x3e] = SDLK_F4;
  HWScanKeyMap[0x3f] = SDLK_F5;
  HWScanKeyMap[0x40] = SDLK_F6;
  HWScanKeyMap[0x41] = SDLK_F7;
  HWScanKeyMap[0x42] = SDLK_F8;
  HWScanKeyMap[0x43] = SDLK_F9;
  HWScanKeyMap[0x44] = SDLK_F10;
  HWScanKeyMap[0x57] = SDLK_F11;
  HWScanKeyMap[0x58] = SDLK_F12;
  HWScanKeyMap[0x5d] = SDLK_PRINT;
  HWScanKeyMap[0x46] = SDLK_SCROLLOCK;
  HWScanKeyMap[0x5f] = SDLK_PAUSE;

  
  HWScanKeyMap[0x29] = SDLK_BACKQUOTE;
  HWScanKeyMap[0x2] = SDLK_1;
  HWScanKeyMap[0x3] = SDLK_2;
  HWScanKeyMap[0x4] = SDLK_3;
  HWScanKeyMap[0x5] = SDLK_4;
  HWScanKeyMap[0x6] = SDLK_5;
  HWScanKeyMap[0x7] = SDLK_6;
  HWScanKeyMap[0x8] = SDLK_7;
  HWScanKeyMap[0x9] = SDLK_8;
  HWScanKeyMap[0xa] = SDLK_9;
  HWScanKeyMap[0xb] = SDLK_0;
  HWScanKeyMap[0xc] = SDLK_MINUS;
  HWScanKeyMap[0xd] = SDLK_EQUALS;
  HWScanKeyMap[0xe] = SDLK_BACKSPACE;
  HWScanKeyMap[0x68] = SDLK_INSERT;
  HWScanKeyMap[0x60] = SDLK_HOME;
  HWScanKeyMap[0x62] = SDLK_PAGEUP;
  HWScanKeyMap[0x45] = SDLK_NUMLOCK;
  HWScanKeyMap[0x5c] = SDLK_KP_DIVIDE;
  HWScanKeyMap[0x37] = SDLK_KP_MULTIPLY;
  HWScanKeyMap[0x4a] = SDLK_KP_MINUS;

  
  HWScanKeyMap[0xf] = SDLK_TAB;
  HWScanKeyMap[0x10] = SDLK_q;
  HWScanKeyMap[0x11] = SDLK_w;
  HWScanKeyMap[0x12] = SDLK_e;
  HWScanKeyMap[0x13] = SDLK_r;
  HWScanKeyMap[0x14] = SDLK_t;
  HWScanKeyMap[0x15] = SDLK_y;
  HWScanKeyMap[0x16] = SDLK_u;
  HWScanKeyMap[0x17] = SDLK_i;
  HWScanKeyMap[0x18] = SDLK_o;
  HWScanKeyMap[0x19] = SDLK_p;
  HWScanKeyMap[0x1a] = SDLK_LEFTBRACKET;
  HWScanKeyMap[0x1b] = SDLK_RIGHTBRACKET;
  HWScanKeyMap[0x1c] = SDLK_RETURN;
  HWScanKeyMap[0x69] = SDLK_DELETE;
  HWScanKeyMap[0x65] = SDLK_END;
  HWScanKeyMap[0x67] = SDLK_PAGEDOWN;
  HWScanKeyMap[0x47] = SDLK_KP7;
  HWScanKeyMap[0x48] = SDLK_KP8;
  HWScanKeyMap[0x49] = SDLK_KP9;
  HWScanKeyMap[0x4e] = SDLK_KP_PLUS;

  
  HWScanKeyMap[0x3a] = SDLK_CAPSLOCK;
  HWScanKeyMap[0x1e] = SDLK_a;
  HWScanKeyMap[0x1f] = SDLK_s;
  HWScanKeyMap[0x20] = SDLK_d;
  HWScanKeyMap[0x21] = SDLK_f;
  HWScanKeyMap[0x22] = SDLK_g;
  HWScanKeyMap[0x23] = SDLK_h;
  HWScanKeyMap[0x24] = SDLK_j;
  HWScanKeyMap[0x25] = SDLK_k;
  HWScanKeyMap[0x26] = SDLK_l;
  HWScanKeyMap[0x27] = SDLK_SEMICOLON;
  HWScanKeyMap[0x28] = SDLK_QUOTE;
  HWScanKeyMap[0x2b] = SDLK_BACKSLASH;
  HWScanKeyMap[0x4b] = SDLK_KP4;
  HWScanKeyMap[0x4c] = SDLK_KP5;
  HWScanKeyMap[0x4d] = SDLK_KP6;

  
  HWScanKeyMap[0x2a] = SDLK_LSHIFT;
  HWScanKeyMap[0x56] = SDLK_WORLD_1; 
  HWScanKeyMap[0x2c] = SDLK_z;
  HWScanKeyMap[0x2d] = SDLK_x;
  HWScanKeyMap[0x2e] = SDLK_c;
  HWScanKeyMap[0x2f] = SDLK_v;
  HWScanKeyMap[0x30] = SDLK_b;
  HWScanKeyMap[0x31] = SDLK_n;
  HWScanKeyMap[0x32] = SDLK_m;
  HWScanKeyMap[0x33] = SDLK_COMMA;
  HWScanKeyMap[0x34] = SDLK_PERIOD;
  HWScanKeyMap[0x35] = SDLK_SLASH;
  HWScanKeyMap[0x36] = SDLK_RSHIFT;
  HWScanKeyMap[0x61] = SDLK_UP;
  HWScanKeyMap[0x4f] = SDLK_KP1;
  HWScanKeyMap[0x50] = SDLK_KP2;
  HWScanKeyMap[0x51] = SDLK_KP3;
  HWScanKeyMap[0x5a] = SDLK_KP_ENTER;

  
  HWScanKeyMap[0x1d] = SDLK_LCTRL;
  HWScanKeyMap[0x7e] = SDLK_LSUPER; 
  HWScanKeyMap[0x38] = SDLK_LALT;
  HWScanKeyMap[0x39] = SDLK_SPACE;
  HWScanKeyMap[0x5e] = SDLK_RALT;
  HWScanKeyMap[0x7f] = SDLK_RSUPER;
  HWScanKeyMap[0x7c] = SDLK_MENU;
  HWScanKeyMap[0x5b] = SDLK_RCTRL;
  HWScanKeyMap[0x63] = SDLK_LEFT;
  HWScanKeyMap[0x66] = SDLK_DOWN;
  HWScanKeyMap[0x64] = SDLK_RIGHT;
  HWScanKeyMap[0x52] = SDLK_KP0;
  HWScanKeyMap[0x53] = SDLK_KP_PERIOD;
}


int os2fslib_IconifyWindow(_THIS)
{
  HAB hab;
  HMQ hmq;
  ERRORID hmqerror;

  
  if (_this->hidden->iPMThreadStatus!=1) return 0;

  
  if (FSLib_QueryFSMode(_this->hidden->hwndClient))
    return 0;

  
  hab = WinInitialize(0);
  hmq = WinCreateMsgQueue(hab,0);
  
  
  hmqerror = WinGetLastError(hab);

  WinSetWindowPos(_this->hidden->hwndFrame, HWND_TOP,
                 0, 0, 0, 0, SWP_MINIMIZE);

  
  if (ERRORIDERROR(hmqerror)==0)
    WinDestroyMsgQueue(hmq);

  return 1;
}

static SDL_GrabMode os2fslib_GrabInput(_THIS, SDL_GrabMode mode)
{
  HAB hab;
  HMQ hmq;
  ERRORID hmqerror;


  
  if (_this->hidden->iPMThreadStatus!=1)
    return SDL_GRAB_OFF;

  
  hab = WinInitialize(0);
  hmq = WinCreateMsgQueue(hab,0);
  
  
  hmqerror = WinGetLastError(hab);


  if (mode == SDL_GRAB_OFF)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_GrabInput] : Releasing mouse\n"); fflush(stdout);
#endif

    
    bMouseCapturable = 0;
    if (bMouseCaptured)
    {
      WinSetCapture(HWND_DESKTOP, NULLHANDLE);
      bMouseCaptured = 0;
    }
  } else
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_GrabInput] : Capturing mouse\n"); fflush(stdout);
#endif

    
    bMouseCapturable = 1;
    if (WinQueryFocus(HWND_DESKTOP) == _this->hidden->hwndClient)
    {
      WinSetCapture(HWND_DESKTOP, _this->hidden->hwndClient);
      bMouseCaptured = 1;
      {
        SWP swpClient;
        POINTL ptl;
        
        WinQueryWindowPos(_this->hidden->hwndClient, &swpClient);
        ptl.x = 0; ptl.y = 0;
        WinMapWindowPoints(_this->hidden->hwndClient, HWND_DESKTOP, &ptl, 1);
        _this->hidden->iSkipWMMOUSEMOVE++; 
        WinSetPointerPos(HWND_DESKTOP,
                         ptl.x + swpClient.cx/2,
                         ptl.y + swpClient.cy/2);
      }
    }
  }

  
  if (ERRORIDERROR(hmqerror)==0)
    WinDestroyMsgQueue(hmq);

  return mode;
}

static void os2fslib_SetCaption(_THIS, const char *title, const char *icon)
{
  HAB hab;
  HMQ hmq;
  ERRORID hmqerror;

  
  if (_this->hidden->iPMThreadStatus!=1) return;

  
  hab = WinInitialize(0);
  hmq = WinCreateMsgQueue(hab,0);
  
  
  hmqerror = WinGetLastError(hab);

  WinSetWindowText(_this->hidden->hwndFrame, (char *) title);

  
  if (ERRORIDERROR(hmqerror)==0)
    WinDestroyMsgQueue(hmq);
}

static int os2fslib_ToggleFullScreen(_THIS, int on)
{
#ifdef DEBUG_BUILD
  printf("[os2fslib_ToggleFullScreen] : %d\n", on); fflush(stdout);
#endif
  
  if (_this->hidden->iPMThreadStatus!=1) return 0;

  FSLib_ToggleFSMode(_this->hidden->hwndClient, on);
  
  os2fslib_SetCursorManagementFunctions(_this, !on);
  return 1;
}

static void os2fslib_UpdateMouse(_THIS)
{
  POINTL ptl;
  HAB hab;
  HMQ hmq;
  ERRORID hmqerror;
  SWP swpClient;

  
  if (_this->hidden->iPMThreadStatus!=1) return;


  
  hab = WinInitialize(0);
  hmq = WinCreateMsgQueue(hab,0);
  
  
  hmqerror = WinGetLastError(hab);

  

  if (_this->hidden->fInFocus)
  {
    
    SDL_PrivateAppActive(1, SDL_APPMOUSEFOCUS);
    SDL_PrivateAppActive(1, SDL_APPINPUTFOCUS);
    SDL_PrivateAppActive(1, SDL_APPACTIVE);
    WinQueryPointerPos(HWND_DESKTOP, &ptl);
    WinMapWindowPoints(HWND_DESKTOP, _this->hidden->hwndClient, &ptl, 1);
    WinQueryWindowPos(_this->hidden->hwndClient, &swpClient);
    
    ptl.x = ptl.x * _this->hidden->SrcBufferDesc.uiXResolution / swpClient.cx;
    ptl.y = ptl.y * _this->hidden->SrcBufferDesc.uiYResolution / swpClient.cy;
    ptl.y = _this->hidden->SrcBufferDesc.uiYResolution - ptl.y - 1;
    SDL_PrivateMouseMotion(0, 0, (Sint16) (ptl.x), (Sint16) (ptl.y));
  } else
  {
    
    SDL_PrivateAppActive(0, SDL_APPMOUSEFOCUS);
    SDL_PrivateAppActive(0, SDL_APPINPUTFOCUS);
    SDL_PrivateAppActive(0, SDL_APPACTIVE);
    SDL_PrivateMouseMotion(0, 0, (Sint16) -1, (Sint16) -1);
  }

  
  if (ERRORIDERROR(hmqerror)==0)
    WinDestroyMsgQueue(hmq);

}

static void os2fslib_UpdateRects(_THIS, int numrects, SDL_Rect *rects)
{
  
  if (_this->hidden->iPMThreadStatus!=1) return;

#ifdef BITBLT_IN_WINMESSAGEPROC
  WinSendMsg(_this->hidden->hwndClient,
                 WM_UPDATERECTSREQUEST,
                 (MPARAM) numrects,
                 (MPARAM) rects);
#else
  if (DosRequestMutexSem(_this->hidden->hmtxUseSrcBuffer, SEM_INDEFINITE_WAIT)==NO_ERROR)
  {
    int i;

    if (_this->hidden->pSDLSurface)
    {
#ifndef RESIZE_EVEN_IF_RESIZABLE
      SWP swp;
      
      
      
      WinQueryWindowPos(_this->hidden->hwndClient, &swp);
      if ((_this->hidden->pSDLSurface) &&
          (_this->hidden->pSDLSurface->flags & SDL_RESIZABLE) &&
          ((swp.cx != _this->hidden->SrcBufferDesc.uiXResolution) ||
           (swp.cy != _this->hidden->SrcBufferDesc.uiYResolution)
          ) &&
          (!FSLib_QueryFSMode(_this->hidden->hwndClient))
         )
      {
        
        
#ifdef DEBUG_BUILD
        printf("[UpdateRects] : Skipping blit while resizing!\n"); fflush(stdout);
#endif
      } else
#endif
      {
#ifdef DEBUG_BUILD
          printf("[os2fslib_UpdateRects] : Blitting!\n"); fflush(stdout);
#endif
  
        
        for (i=0; i<numrects; i++)
          FSLIB_BITBLT(_this->hidden->hwndClient, _this->hidden->pchSrcBuffer,
                       rects[i].y, rects[i].x, rects[i].w, rects[i].h);
      }
    }
#ifdef DEBUG_BUILD
     else
       printf("[os2fslib_UpdateRects] : No public surface!\n"); fflush(stdout);
#endif
    DosReleaseMutexSem(_this->hidden->hmtxUseSrcBuffer);
  }
#ifdef DEBUG_BUILD
  else
    printf("[os2fslib_UpdateRects] : Error in mutex!\n"); fflush(stdout);
#endif
#endif
}


static void os2fslib_VideoQuit(_THIS)
{
#ifdef DEBUG_BUILD
  printf("[os2fslib_VideoQuit]\n"); fflush(stdout);
#endif
  
  if (_this->hidden->iPMThreadStatus == 1)
  {
    int iTimeout;
    WinPostMsg(_this->hidden->hwndFrame, WM_QUIT, (MPARAM) 0, (MPARAM) 0);
    
    
    
    
    
#ifdef DEBUG_BUILD
    printf("[os2fslib_VideoQuit] : Waiting for PM thread to die\n"); fflush(stdout);
#endif

    iTimeout=0;
    while ((_this->hidden->iPMThreadStatus == 1) && (iTimeout<100))
    {
      iTimeout++;
      DosSleep(64);
    }

#ifdef DEBUG_BUILD
    printf("[os2fslib_VideoQuit] : End of wait.\n"); fflush(stdout);
#endif

    if (_this->hidden->iPMThreadStatus == 1)
    {
#ifdef DEBUG_BUILD
      printf("[os2fslib_VideoQuit] : Killing PM thread!\n"); fflush(stdout);
#endif
      
      _this->hidden->iPMThreadStatus = 0;
      DosKillThread(_this->hidden->tidPMThread);

      if (_this->hidden->hwndFrame)
      {
#ifdef DEBUG_BUILD
        printf("[os2fslib_VideoQuit] : Destroying PM window!\n"); fflush(stdout);
#endif

        WinDestroyWindow(_this->hidden->hwndFrame); _this->hidden->hwndFrame=NULL;
      }
    }

  }

  
  
  if (_this->hidden->pListModesResult)
  {
    SDL_free(_this->hidden->pListModesResult); _this->hidden->pListModesResult = NULL;
  }

  
  if (_this->hidden->pAvailableFSLibVideoModes)
  {
    FSLib_FreeVideoModeList(_this->hidden->pAvailableFSLibVideoModes);
    _this->hidden->pAvailableFSLibVideoModes = NULL;
  }

  
  if (hptrCurrentIcon)
  {
    WinDestroyPointer(hptrCurrentIcon);
    hptrCurrentIcon = NULL;
  }
}

static SDL_Surface *os2fslib_SetVideoMode(_THIS, SDL_Surface *current,
                                          int width, int height, int bpp, Uint32 flags)
{
  static int bFirstCall = 1;
  FSLib_VideoMode_p pModeInfo, pModeInfoFound;
  FSLib_VideoMode TempModeInfo;
  HAB hab;
  HMQ hmq;
  ERRORID hmqerror;
  RECTL rectl;
  SDL_Surface *pResult;

  
  if (_this->hidden->iPMThreadStatus!=1) return NULL;

#ifdef DEBUG_BUILD
  printf("[os2fslib_SetVideoMode] : Request for %dx%d @ %dBPP, flags=0x%x\n", width, height, bpp, flags); fflush(stdout);
#endif

  
  if (bpp==8) bpp=32;

  
  if (flags & SDL_RESIZABLE)
    flags &= ~SDL_FULLSCREEN;

  
  if (flags & SDL_DOUBLEBUF)
    flags &= ~SDL_DOUBLEBUF;

  
  if (flags & SDL_HWSURFACE)
  {
    flags &= ~SDL_HWSURFACE;
    flags |= SDL_SWSURFACE;
  }

#ifdef DEBUG_BUILD
  printf("[os2fslib_SetVideoMode] : Changed request to %dx%d @ %dBPP, flags=0x%x\n", width, height, bpp, flags); fflush(stdout);
#endif

  
  pModeInfoFound = NULL;

  
  
  pModeInfoFound = NULL;
  pModeInfo = _this->hidden->pAvailableFSLibVideoModes;

  while (pModeInfo)
  {
    
    if ((pModeInfo->uiXResolution == width) &&
        (pModeInfo->uiYResolution == height) &&
        (pModeInfo->uiBPP!=8)) 
    {
      
      
      if (!pModeInfoFound)
        pModeInfoFound = pModeInfo;
      else
      if ((pModeInfoFound->uiBPP!=bpp) &&
          (pModeInfoFound->uiBPP<pModeInfo->uiBPP))
        pModeInfoFound = pModeInfo;
    }
    pModeInfo = pModeInfo->pNext;
  }

  
  if (!pModeInfoFound)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Requested video mode not found, looking for a similar one!\n"); fflush(stdout);
#endif
    
    pModeInfo = _this->hidden->pAvailableFSLibVideoModes;
    while (pModeInfo)
    {
      
      if ((pModeInfo->uiXResolution >= width) &&
          (pModeInfo->uiYResolution >= height) &&
          (pModeInfo->uiBPP == bpp))
      {
        if (!pModeInfoFound)
          pModeInfoFound = pModeInfo;
        else
        if (((pModeInfoFound->uiXResolution-width)*(pModeInfoFound->uiYResolution-height))>
            ((pModeInfo->uiXResolution-width)*(pModeInfo->uiYResolution-height)))
        {
          
          pModeInfoFound = pModeInfo;
        }
      }
      pModeInfo = pModeInfo->pNext;
    }
  }

  
  if (!pModeInfoFound)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Requested video mode not found!\n"); fflush(stdout);
#endif
    return NULL;
  }

#ifdef DEBUG_BUILD
  printf("[os2fslib_SetVideoMode] : Found mode!\n"); fflush(stdout);
#endif

  
  
  SDL_memcpy(&TempModeInfo, pModeInfoFound, sizeof(TempModeInfo));
  pModeInfoFound = &TempModeInfo;

  if (flags & SDL_RESIZABLE)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Requested mode is resizable, changing width/height\n"); fflush(stdout);
#endif
    
    TempModeInfo.uiXResolution = width;
    TempModeInfo.uiYResolution = height;
    TempModeInfo.uiScanLineSize = width * ((TempModeInfo.uiBPP+7)/8);
  }

  

  
  hab = WinInitialize(0);
  hmq = WinCreateMsgQueue(hab,0);
  
  
  hmqerror = WinGetLastError(hab);

  

  if (DosRequestMutexSem(_this->hidden->hmtxUseSrcBuffer, SEM_INDEFINITE_WAIT)==NO_ERROR)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Creating new SW surface\n"); fflush(stdout);
#endif

    
    pResult = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   pModeInfoFound->uiXResolution,
                                   pModeInfoFound->uiYResolution,
                                   pModeInfoFound->uiBPP,
                                   ((unsigned int) pModeInfoFound->PixelFormat.ucRedMask) << pModeInfoFound->PixelFormat.ucRedPosition,
                                   ((unsigned int) pModeInfoFound->PixelFormat.ucGreenMask) << pModeInfoFound->PixelFormat.ucGreenPosition,
                                   ((unsigned int) pModeInfoFound->PixelFormat.ucBlueMask) << pModeInfoFound->PixelFormat.ucBluePosition,
                                   ((unsigned int) pModeInfoFound->PixelFormat.ucAlphaMask) << pModeInfoFound->PixelFormat.ucAlphaPosition);

    if (pResult == NULL)
    {
      DosReleaseMutexSem(_this->hidden->hmtxUseSrcBuffer);
      SDL_OutOfMemory();
      return NULL;
    }

#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Adjusting pixel format\n"); fflush(stdout);
#endif

    
    pResult->format->Rmask = ((unsigned int) pModeInfoFound->PixelFormat.ucRedMask) << pModeInfoFound->PixelFormat.ucRedPosition;
    pResult->format->Rshift = pModeInfoFound->PixelFormat.ucRedPosition;
    pResult->format->Rloss = pModeInfoFound->PixelFormat.ucRedAdjust;
    pResult->format->Gmask = ((unsigned int) pModeInfoFound->PixelFormat.ucGreenMask) << pModeInfoFound->PixelFormat.ucGreenPosition;
    pResult->format->Gshift = pModeInfoFound->PixelFormat.ucGreenPosition;
    pResult->format->Gloss = pModeInfoFound->PixelFormat.ucGreenAdjust;
    pResult->format->Bmask = ((unsigned int) pModeInfoFound->PixelFormat.ucBlueMask) << pModeInfoFound->PixelFormat.ucBluePosition;
    pResult->format->Bshift = pModeInfoFound->PixelFormat.ucBluePosition;
    pResult->format->Bloss = pModeInfoFound->PixelFormat.ucBlueAdjust;
    pResult->format->Amask = ((unsigned int) pModeInfoFound->PixelFormat.ucAlphaMask) << pModeInfoFound->PixelFormat.ucAlphaPosition;
    pResult->format->Ashift = pModeInfoFound->PixelFormat.ucAlphaPosition;
    pResult->format->Aloss = pModeInfoFound->PixelFormat.ucAlphaAdjust;

#ifdef REPORT_EMPTY_ALPHA_MASK
    pResult->format->Amask =
        pResult->format->Ashift =
        pResult->format->Aloss = 0;
#endif

    
    pResult->flags |= (flags & SDL_FULLSCREEN);
    pResult->flags |= (flags & SDL_RESIZABLE);

    
    
    pModeInfoFound->uiScanLineSize = pResult->pitch;

    
    SDL_memcpy(&(_this->hidden->SrcBufferDesc), pModeInfoFound, sizeof(*pModeInfoFound));
    _this->hidden->pchSrcBuffer = pResult->pixels;

#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Telling FSLib the stuffs\n"); fflush(stdout);
#endif

    
    FSLib_SetSrcBufferDesc(_this->hidden->hwndClient, &(_this->hidden->SrcBufferDesc));

    if (
        ((flags & SDL_RESIZABLE)==0) ||
        (bFirstCall)
       )
    {
      bFirstCall = 0;
#ifdef DEBUG_BUILD
      printf("[os2fslib_SetVideoMode] : Modifying window size\n"); fflush(stdout);
#endif

      
      rectl.xLeft = 0;
      rectl.yBottom = 0;
      rectl.xRight = pModeInfoFound->uiXResolution; 
      rectl.yTop = pModeInfoFound->uiYResolution; 
      WinCalcFrameRect(_this->hidden->hwndFrame, &rectl, FALSE);

      
      SetAccessableWindowPos(_this->hidden->hwndFrame,
                             HWND_TOP,
                             0, 0,
                             (rectl.xRight-rectl.xLeft),
                             (rectl.yTop-rectl.yBottom),
                             SWP_SIZE | SWP_ACTIVATE | SWP_SHOW);
    }

    
    if (flags & SDL_FULLSCREEN)
    {
#ifdef DEBUG_BUILD
      printf("[os2fslib_SetVideoMode] : Also trying to switch to fullscreen\n");
      fflush(stdout);
#endif
      FSLib_ToggleFSMode(_this->hidden->hwndClient, 1);
      
      os2fslib_SetCursorManagementFunctions(_this, 0);
    } else
    {
#ifdef DEBUG_BUILD
      printf("[os2fslib_SetVideoMode] : Also trying to switch to desktop mode\n");
      fflush(stdout);
#endif
      FSLib_ToggleFSMode(_this->hidden->hwndClient, 0);
      
      os2fslib_SetCursorManagementFunctions(_this, 1);
    }

    _this->hidden->pSDLSurface = pResult;

    DosReleaseMutexSem(_this->hidden->hmtxUseSrcBuffer);
  } else
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Could not get hmtxUseSrcBuffer!\n"); fflush(stdout);
#endif
    
    pResult = NULL;
  }

  
  if ((pResult) && (current))
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Freeing old surface\n"); fflush(stdout);
#endif
    SDL_FreeSurface(current);
  }

  
  WinInvalidateRegion(_this->hidden->hwndClient, NULL, TRUE);

  
  if (ERRORIDERROR(hmqerror)==0)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_SetVideoMode] : Destroying message queue\n"); fflush(stdout);
#endif
    WinDestroyMsgQueue(hmq);
  }

#ifdef DEBUG_BUILD
  printf("[os2fslib_SetVideoMode] : Done\n"); fflush(stdout);
#endif

  

  
  return pResult;
}

static SDL_Rect **os2fslib_ListModes(_THIS, SDL_PixelFormat *format, Uint32 flags)
{
#ifdef DEBUG_BUILD
  printf("[os2fslib_ListModes] : ListModes of %d Bpp\n", format->BitsPerPixel);
#endif
  
  if (_this->hidden->pListModesResult)
  {
    SDL_free(_this->hidden->pListModesResult); _this->hidden->pListModesResult = NULL;
  }

  
  if ((flags & SDL_RESIZABLE) && ((flags & SDL_FULLSCREEN) == 0))
    return (SDL_Rect **)-1;

  
  if ((flags & SDL_FULLSCREEN) == 0)

  {
    
    return (SDL_Rect **)-1;
  } else
  {
    FSLib_VideoMode_p pFSMode;
    
    
    pFSMode = _this->hidden->pAvailableFSLibVideoModes;
    while (pFSMode)
    {
      if (pFSMode->uiBPP == format->BitsPerPixel)
      {
        SDL_Rect *pRect = (SDL_Rect *) SDL_malloc(sizeof(SDL_Rect));
        if (pRect)
        {
          
          pRect->x = 0;
          pRect->y = 0;
          pRect->w = pFSMode->uiXResolution;
          pRect->h = pFSMode->uiYResolution;
#ifdef DEBUG_BUILD
#endif
          
          if (!(_this->hidden->pListModesResult))
          {
#ifdef DEBUG_BUILD
#endif

            
            _this->hidden->pListModesResult = (SDL_Rect**) SDL_malloc(2*sizeof(SDL_Rect*));
            if (_this->hidden->pListModesResult)
            {
              _this->hidden->pListModesResult[0] = pRect;
              _this->hidden->pListModesResult[1] = NULL;
            } else
            {
              SDL_free(pRect);
            }
          } else
          {
            
            
            SDL_Rect **pNewList;
            int iPlace, iNumOfSlots, i;

#ifdef DEBUG_BUILD
#endif

            iPlace = -1; iNumOfSlots = 1; 
            for (i=0; _this->hidden->pListModesResult[i]; i++)
            {
              iNumOfSlots++;
              if (iPlace==-1)
              {
                if ((_this->hidden->pListModesResult[i]->w*_this->hidden->pListModesResult[i]->h)<
                    (pRect->w*pRect->h))
                {
                  iPlace = i;
                }
              }
            }
            if (iPlace==-1) iPlace = iNumOfSlots-1;

#ifdef DEBUG_BUILD
#endif

            pNewList = (SDL_Rect**) SDL_realloc(_this->hidden->pListModesResult, (iNumOfSlots+1)*sizeof(SDL_Rect*));
            if (pNewList)
            {
              for (i=iNumOfSlots;i>iPlace;i--)
                pNewList[i] = pNewList[i-1];
              pNewList[iPlace] = pRect;
              _this->hidden->pListModesResult = pNewList;
            } else
            {
              SDL_free(pRect);
            }
          }
        }
      }
      pFSMode = pFSMode->pNext;
    }
  }
#ifdef DEBUG_BUILD
#endif
  return _this->hidden->pListModesResult;
}

static int os2fslib_VideoInit(_THIS, SDL_PixelFormat *vformat)
{
  FSLib_VideoMode_p pDesktopMode;

#ifdef DEBUG_BUILD
  printf("[os2fslib_VideoInit] : Enter\n"); fflush(stdout);
#endif

  
  
  pDesktopMode = FSLib_GetDesktopVideoMode();
  if (!pDesktopMode)
  {
    SDL_SetError("Could not query desktop video mode!");
#ifdef DEBUG_BUILD
    printf("[os2fslib_VideoInit] : Could not query desktop video mode!\n");
#endif
    return -1;
  }

  
  _this->info.current_w = pDesktopMode->uiXResolution;
  _this->info.current_h = pDesktopMode->uiYResolution;

  
  vformat->BitsPerPixel = pDesktopMode->uiBPP;
  vformat->BytesPerPixel = (vformat->BitsPerPixel+7)/8;

  vformat->Rmask = ((unsigned int) pDesktopMode->PixelFormat.ucRedMask) << pDesktopMode->PixelFormat.ucRedPosition;
  vformat->Rshift = pDesktopMode->PixelFormat.ucRedPosition;
  vformat->Rloss = pDesktopMode->PixelFormat.ucRedAdjust;
  vformat->Gmask = ((unsigned int) pDesktopMode->PixelFormat.ucGreenMask) << pDesktopMode->PixelFormat.ucGreenPosition;
  vformat->Gshift = pDesktopMode->PixelFormat.ucGreenPosition;
  vformat->Gloss = pDesktopMode->PixelFormat.ucGreenAdjust;
  vformat->Bmask = ((unsigned int) pDesktopMode->PixelFormat.ucBlueMask) << pDesktopMode->PixelFormat.ucBluePosition;
  vformat->Bshift = pDesktopMode->PixelFormat.ucBluePosition;
  vformat->Bloss = pDesktopMode->PixelFormat.ucBlueAdjust;
  vformat->Amask = ((unsigned int) pDesktopMode->PixelFormat.ucAlphaMask) << pDesktopMode->PixelFormat.ucAlphaPosition;
  vformat->Ashift = pDesktopMode->PixelFormat.ucAlphaPosition;
  vformat->Aloss = pDesktopMode->PixelFormat.ucAlphaAdjust;

#ifdef REPORT_EMPTY_ALPHA_MASK
  vformat->Amask =
      vformat->Ashift =
      vformat->Aloss = 0;
#endif

  
  _this->info.wm_available = 1;

  
  _this->hidden->pListModesResult = NULL;
  _this->hidden->fInFocus = 0;
  _this->hidden->iSkipWMMOUSEMOVE = 0;
  _this->hidden->iMouseVisible = 1;

  if (getenv("SDL_USE_PROPORTIONAL_WINDOW"))
    _this->hidden->bProportionalResize = 1;
  else
  {
    PPIB pib;
    PTIB tib;
    char *pchFileName, *pchTemp;
    char achConfigFile[CCHMAXPATH];
    FILE *hFile;

    _this->hidden->bProportionalResize = 0;

    DosGetInfoBlocks(&tib, &pib);
    pchTemp = pchFileName = pib->pib_pchcmd;
    while (*pchTemp)
    {
      if (*pchTemp=='\\')
        pchFileName = pchTemp+1;
      pchTemp++;
    }
    if (getenv("HOME"))
    {
      sprintf(achConfigFile, "%s\\.sdl.proportionals", getenv("HOME"));
      hFile = fopen(achConfigFile, "rt");
      if (!hFile)
      {
        hFile = fopen(achConfigFile, "wt");
        if (hFile)
        {
          fprintf(hFile, "; This file is a config file of SDL/2, containing\n");
          fprintf(hFile, "; the list of executables that must have proportional\n");
          fprintf(hFile, "; windows.\n");
          fprintf(hFile, ";\n");
          fprintf(hFile, "; You can add executable filenames into this file,\n");
          fprintf(hFile, "; one under the other. If SDL finds that a given\n");
          fprintf(hFile, "; program is in this list, then that application\n");
          fprintf(hFile, "; will have proportional windows, just like if\n");
          fprintf(hFile, "; the SET SDL_USE_PROPORTIONAL_WINDOW env. variable\n");
          fprintf(hFile, "; would have been set for that process.\n");
          fprintf(hFile, ";\n");
          fprintf(hFile, "\n");
          fprintf(hFile, "dosbox.exe\n");
          fclose(hFile);
        }

        hFile = fopen(achConfigFile, "rt");
      }

      if (hFile)
      {
        while (fgets(achConfigFile, sizeof(achConfigFile), hFile))
        {
          

          while (achConfigFile[strlen(achConfigFile)-1] == '\n')
            achConfigFile[strlen(achConfigFile)-1] = 0;

          
          if (stricmp(achConfigFile, pchFileName)==0)
          {
            
            _this->hidden->bProportionalResize = 1;
            break;
          }
        }
        fclose(hFile);
      }
    }
  }

  DosCreateMutexSem(NULL, &(_this->hidden->hmtxUseSrcBuffer), 0, FALSE);

  

  
  
  SDL_memcpy(&(_this->hidden->SrcBufferDesc), _this->hidden->pAvailableFSLibVideoModes, sizeof(_this->hidden->SrcBufferDesc));
  
  _this->hidden->pchSrcBuffer = (char *) SDL_malloc(_this->hidden->pAvailableFSLibVideoModes->uiScanLineSize * _this->hidden->pAvailableFSLibVideoModes->uiYResolution);
  if (!_this->hidden->pchSrcBuffer)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_VideoInit] : Yikes, not enough memory for new video buffer!\n"); fflush(stdout);
#endif
    SDL_SetError("Not enough memory for new video buffer!\n");
    return -1;
  }

  
  
  
  _this->hidden->iPMThreadStatus = 0;
  _this->hidden->tidPMThread = _beginthread(PMThreadFunc, NULL, 65536, (void *) _this);
  if (_this->hidden->tidPMThread <= 0)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_VideoInit] : Could not create PM thread!\n");
#endif
    SDL_SetError("Could not create PM thread");
    return -1;
  }
#ifdef USE_DOSSETPRIORITY
  
  DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, _this->hidden->tidPMThread);
#endif
  
  while (_this->hidden->iPMThreadStatus==0)
    DosSleep(32);
  
  
  if (_this->hidden->iPMThreadStatus!=1)
  {
#ifdef DEBUG_BUILD
    printf("[os2fslib_VideoInit] : PMThread reported an error : %d\n", _this->hidden->iPMThreadStatus);
#endif
    SDL_SetError("Error initializing PM thread");
    return -1;
  }

  return 0;
}


static void os2fslib_DeleteDevice(_THIS)
{
#ifdef DEBUG_BUILD
  printf("[os2fslib_DeleteDevice]\n"); fflush(stdout);
#endif
  
  FSLib_FreeVideoModeList(_this->hidden->pAvailableFSLibVideoModes);
  if (_this->hidden->pListModesResult)
    SDL_free(_this->hidden->pListModesResult);
  if (_this->hidden->pchSrcBuffer)
    SDL_free(_this->hidden->pchSrcBuffer);
  DosCloseMutexSem(_this->hidden->hmtxUseSrcBuffer);
  SDL_free(_this->hidden);
  SDL_free(_this);
  FSLib_Uninitialize();
}

static int os2fslib_Available(void)
{

  
  
  return 1;
}

static void os2fslib_MorphToPM()
{
  PPIB pib;
  PTIB tib;

  DosGetInfoBlocks(&tib, &pib);

  
  if (pib->pib_ultype==2) pib->pib_ultype = 3;
}

static SDL_VideoDevice *os2fslib_CreateDevice(int devindex)
{
  SDL_VideoDevice *device;

#ifdef DEBUG_BUILD
  printf("[os2fslib_CreateDevice] : Enter\n"); fflush(stdout);
#endif

  
  device = (SDL_VideoDevice *)SDL_malloc(sizeof(SDL_VideoDevice));
  if ( device )
  {
    SDL_memset(device, 0, (sizeof *device));
    
    device->hidden = (struct SDL_PrivateVideoData *) SDL_malloc((sizeof(struct SDL_PrivateVideoData)));
  }
  if ( (device == NULL) || (device->hidden == NULL) )
  {
    SDL_OutOfMemory();
    if ( device )
      SDL_free(device);
    return NULL;
  }
  SDL_memset(device->hidden, 0, (sizeof *device->hidden));

  
#ifdef DEBUG_BUILD
  printf("[os2fslib_CreateDevice] : VideoInit is %p\n", os2fslib_VideoInit); fflush(stdout);
#endif

  
  device->VideoInit = os2fslib_VideoInit;
  device->ListModes = os2fslib_ListModes;
  device->SetVideoMode = os2fslib_SetVideoMode;
  device->ToggleFullScreen = os2fslib_ToggleFullScreen;
  device->UpdateMouse = os2fslib_UpdateMouse;
  device->CreateYUVOverlay = NULL;
  device->SetColors = os2fslib_SetColors;
  device->UpdateRects = os2fslib_UpdateRects;
  device->VideoQuit = os2fslib_VideoQuit;
  
  device->AllocHWSurface = os2fslib_AllocHWSurface;
  device->CheckHWBlit = NULL;
  device->FillHWRect = NULL;
  device->SetHWColorKey = NULL;
  device->SetHWAlpha = NULL;
  device->LockHWSurface = os2fslib_LockHWSurface;
  device->UnlockHWSurface = os2fslib_UnlockHWSurface;
  device->FlipHWSurface = NULL;
  device->FreeHWSurface = os2fslib_FreeHWSurface;
  
  device->SetCaption = os2fslib_SetCaption;
  device->SetIcon = os2fslib_SetIcon;
  device->IconifyWindow = os2fslib_IconifyWindow;
  device->GrabInput = os2fslib_GrabInput;
  device->GetWMInfo = NULL;
  
  os2fslib_SetCursorManagementFunctions(device, 1);
  
  device->InitOSKeymap = os2fslib_InitOSKeymap;
  device->PumpEvents = os2fslib_PumpEvents;
  
  device->free = os2fslib_DeleteDevice;

  
  
  os2fslib_MorphToPM();

  
  if (!FSLib_Initialize())
  {
    
#ifdef DEBUG_BUILD
    printf("[os2fslib_CreateDevice] : Could not initialize FSLib!\n");
#endif
    SDL_SetError("Could not initialize FSLib!");
    SDL_free(device->hidden);
    SDL_free(device);
    return NULL;
  }
  device->hidden->pAvailableFSLibVideoModes =
    FSLib_GetVideoModeList();

  return device;
}

VideoBootStrap OS2FSLib_bootstrap = {
        "os2fslib", "OS/2 Video Output using FSLib",
        os2fslib_Available, os2fslib_CreateDevice
};

