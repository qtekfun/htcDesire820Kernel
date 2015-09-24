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

    This file based on Apple sample code. We haven't changed the file name, 
    so if you want to see the original search for it on apple.com/developer
*/
#include "SDL_config.h"
#include "SDL_endian.h"

#include "AudioFilePlayer.h"


#if DEBUG
static void PrintStreamDesc (AudioStreamBasicDescription *inDesc)
{
    if (!inDesc) {
        printf ("Can't print a NULL desc!\n");
        return;
    }
    
    printf ("- - - - - - - - - - - - - - - - - - - -\n");
    printf ("  Sample Rate:%f\n", inDesc->mSampleRate);
    printf ("  Format ID:%s\n", (char*)&inDesc->mFormatID);
    printf ("  Format Flags:%lX\n", inDesc->mFormatFlags);
    printf ("  Bytes per Packet:%ld\n", inDesc->mBytesPerPacket);
    printf ("  Frames per Packet:%ld\n", inDesc->mFramesPerPacket);
    printf ("  Bytes per Frame:%ld\n", inDesc->mBytesPerFrame);
    printf ("  Channels per Frame:%ld\n", inDesc->mChannelsPerFrame);
    printf ("  Bits per Channel:%ld\n", inDesc->mBitsPerChannel);
    printf ("- - - - - - - - - - - - - - - - - - - -\n");
}
#endif


static int AudioFilePlayer_SetDestination (AudioFilePlayer *afp, AudioUnit  *inDestUnit)
{
     
    if (afp->mConnected)
        return 0 ;

    SDL_memcpy(&afp->mPlayUnit, inDestUnit, sizeof (afp->mPlayUnit));

    OSStatus result = noErr;
    

        
    ComponentDescription desc;
    result = GetComponentInfo ((Component)*inDestUnit, &desc, 0, 0, 0);
    if (result) return 0; 
        
    if (desc.componentType != kAudioUnitType_Output) {
        result = badComponentInstance;
        
        if (result) return 0;
    }

    
    result = AudioUnitSetProperty (*inDestUnit,
                               kAudioUnitProperty_StreamFormat,
                               kAudioUnitScope_Input,
                               0,
                               &afp->mFileDescription,
                               sizeof (afp->mFileDescription));
        
    if (result) return 0;
    return 1;
}

static void AudioFilePlayer_SetNotifier(AudioFilePlayer *afp, AudioFilePlayNotifier inNotifier, void *inRefCon)
{
    afp->mNotifier = inNotifier;
    afp->mRefCon = inRefCon;
}

static int AudioFilePlayer_IsConnected(AudioFilePlayer *afp)
{
    return afp->mConnected;
}

static AudioUnit AudioFilePlayer_GetDestUnit(AudioFilePlayer *afp)
{
   return afp->mPlayUnit;
}

static void AudioFilePlayer_Print(AudioFilePlayer *afp)
{
#if DEBUG    
    printf ("Is Connected:%s\n", (IsConnected() ? "true" : "false"));
    printf ("- - - - - - - - - - - - - - \n");
#endif
}

static void    AudioFilePlayer_SetStartFrame (AudioFilePlayer *afp, int frame)
{
    SInt64 position = frame * 2352;

    afp->mStartFrame = frame;
    afp->mAudioFileManager->SetPosition (afp->mAudioFileManager, position);
}

    
static int    AudioFilePlayer_GetCurrentFrame (AudioFilePlayer *afp)
{
    return afp->mStartFrame + (afp->mAudioFileManager->GetByteCounter(afp->mAudioFileManager) / 2352);
}
    
static void    AudioFilePlayer_SetStopFrame (AudioFilePlayer *afp, int frame)
{
    SInt64 position  = frame * 2352;
    
    afp->mAudioFileManager->SetEndOfFile (afp->mAudioFileManager, position);
}
    
void delete_AudioFilePlayer(AudioFilePlayer *afp)
{
    if (afp != NULL)
    {
        afp->Disconnect(afp);
        
        if (afp->mAudioFileManager) {
            delete_AudioFileManager(afp->mAudioFileManager);
            afp->mAudioFileManager = 0;
        }
    
        if (afp->mForkRefNum) {
            FSCloseFork (afp->mForkRefNum);
            afp->mForkRefNum = 0;
        }
        SDL_free(afp);
    }
}

static int    AudioFilePlayer_Connect(AudioFilePlayer *afp)
{
#if DEBUG
    printf ("Connect:%x, engaged=%d\n", (int)afp->mPlayUnit, (afp->mConnected ? 1 : 0));
#endif
    if (!afp->mConnected)
    {           
        if (!afp->mAudioFileManager->DoConnect(afp->mAudioFileManager))
            return 0;

        
        afp->mInputCallback.inputProc = afp->mAudioFileManager->FileInputProc;
        afp->mInputCallback.inputProcRefCon = afp->mAudioFileManager;

        OSStatus result = AudioUnitSetProperty (afp->mPlayUnit, 
                            kAudioUnitProperty_SetRenderCallback,
                            kAudioUnitScope_Input, 
                            0,
                            &afp->mInputCallback, 
                            sizeof(afp->mInputCallback));
        if (result) return 0;  
        afp->mConnected = 1;
    }

    return 1;
}

static void    AudioFilePlayer_DoNotification (AudioFilePlayer *afp, OSStatus inStatus)
{
    if (afp->mNotifier) {
        (*afp->mNotifier) (afp->mRefCon, inStatus);
    } else {
        SDL_SetError ("Notification posted with no notifier in place");
        
        if (inStatus == kAudioFilePlay_FileIsFinished)
            afp->Disconnect(afp);
        else if (inStatus != kAudioFilePlayErr_FilePlayUnderrun)
            afp->Disconnect(afp);
    }
}

static void    AudioFilePlayer_Disconnect (AudioFilePlayer *afp)
{
#if DEBUG
    printf ("Disconnect:%x,%ld, engaged=%d\n", (int)afp->mPlayUnit, 0, (afp->mConnected ? 1 : 0));
#endif
    if (afp->mConnected)
    {
        afp->mConnected = 0;
            
        afp->mInputCallback.inputProc = 0;
        afp->mInputCallback.inputProcRefCon = 0;
        OSStatus result = AudioUnitSetProperty (afp->mPlayUnit, 
                                        kAudioUnitProperty_SetRenderCallback,
                                        kAudioUnitScope_Input, 
                                        0,
                                        &afp->mInputCallback, 
                                        sizeof(afp->mInputCallback));
        if (result) 
            SDL_SetError ("AudioUnitSetProperty:RemoveInputCallback:%ld", result);

        afp->mAudioFileManager->Disconnect(afp->mAudioFileManager);
    }
}

typedef struct {
    UInt32 offset;
    UInt32 blockSize;
} SSNDData;

static int    AudioFilePlayer_OpenFile (AudioFilePlayer *afp, const FSRef *inRef, SInt64 *outFileDataSize)
{
    ContainerChunk chunkHeader;
    ChunkHeader chunk;
    SSNDData ssndData;

    OSErr result;
    HFSUniStr255 dfName;
    ByteCount actual;
    SInt64 offset;

    
    result = FSGetDataForkName(&dfName);
       if (result) return 0; 

    result = FSOpenFork(inRef, dfName.length, dfName.unicode, fsRdPerm, &afp->mForkRefNum);
       if (result) return 0; 
 
    
    result = FSReadFork(afp->mForkRefNum, fsAtMark, 0, sizeof(chunkHeader), &chunkHeader, &actual);
       if (result) return 0; 

    if (SDL_SwapBE32(chunkHeader.ckID) != 'FORM') {
        result = -1;
        if (result) return 0; 
    }

    if (SDL_SwapBE32(chunkHeader.formType) != 'AIFC') {
        result = -1;
        if (result) return 0; 
    }

    offset = 0;
    do {
        result = FSReadFork(afp->mForkRefNum, fsFromMark, offset, sizeof(chunk), &chunk, &actual);
        if (result) return 0; 

        chunk.ckID = SDL_SwapBE32(chunk.ckID);
        chunk.ckSize = SDL_SwapBE32(chunk.ckSize);

        
        offset = chunk.ckSize;
    } while (chunk.ckID != 'SSND');

    result = FSReadFork(afp->mForkRefNum, fsAtMark, 0, sizeof(ssndData), &ssndData, &actual);
    if (result) return 0; 

    ssndData.offset = SDL_SwapBE32(ssndData.offset);

    result = FSSetForkPosition(afp->mForkRefNum, fsFromMark, ssndData.offset);
    if (result) return 0; 

    
    *outFileDataSize = chunk.ckSize - ssndData.offset - 8;

    
    afp->mFileDescription.mSampleRate = 44100;
    afp->mFileDescription.mFormatID = kAudioFormatLinearPCM;
    afp->mFileDescription.mFormatFlags = kLinearPCMFormatFlagIsPacked | kLinearPCMFormatFlagIsSignedInteger;
    afp->mFileDescription.mBytesPerPacket = 4;
    afp->mFileDescription.mFramesPerPacket = 1;
    afp->mFileDescription.mBytesPerFrame = 4;
    afp->mFileDescription.mChannelsPerFrame = 2;
    afp->mFileDescription.mBitsPerChannel = 16;

    return 1;
}

AudioFilePlayer *new_AudioFilePlayer (const FSRef *inFileRef)
{
    SInt64 fileDataSize  = 0;

    AudioFilePlayer *afp = (AudioFilePlayer *) SDL_malloc(sizeof (AudioFilePlayer));
    if (afp == NULL)
        return NULL;
    SDL_memset(afp, '\0', sizeof (*afp));

    #define SET_AUDIOFILEPLAYER_METHOD(m) afp->m = AudioFilePlayer_##m
    SET_AUDIOFILEPLAYER_METHOD(SetDestination);
    SET_AUDIOFILEPLAYER_METHOD(SetNotifier);
    SET_AUDIOFILEPLAYER_METHOD(SetStartFrame);
    SET_AUDIOFILEPLAYER_METHOD(GetCurrentFrame);
    SET_AUDIOFILEPLAYER_METHOD(SetStopFrame);
    SET_AUDIOFILEPLAYER_METHOD(Connect);
    SET_AUDIOFILEPLAYER_METHOD(Disconnect);
    SET_AUDIOFILEPLAYER_METHOD(DoNotification);
    SET_AUDIOFILEPLAYER_METHOD(IsConnected);
    SET_AUDIOFILEPLAYER_METHOD(GetDestUnit);
    SET_AUDIOFILEPLAYER_METHOD(Print);
    SET_AUDIOFILEPLAYER_METHOD(OpenFile);
    #undef SET_AUDIOFILEPLAYER_METHOD

    if (!afp->OpenFile (afp, inFileRef, &fileDataSize))
    {
        SDL_free(afp);
        return NULL;
    }
        
    
    int bytesPerSecond = (UInt32) (4 * afp->mFileDescription.mSampleRate * afp->mFileDescription.mBytesPerFrame);
    
#if DEBUG
    printf("File format:\n");
    PrintStreamDesc (&afp->mFileDescription);
#endif
    
    afp->mAudioFileManager = new_AudioFileManager(afp, afp->mForkRefNum,
                                                  fileDataSize,
                                                  bytesPerSecond);
    if (afp->mAudioFileManager == NULL)
    {
        delete_AudioFilePlayer(afp);
        return NULL;
    }

    return afp;
}

