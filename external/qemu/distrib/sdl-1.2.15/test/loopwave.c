

#include "SDL_config.h"

#include <stdio.h>
#include <stdlib.h>

#if HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "SDL.h"
#include "SDL_audio.h"

struct {
	SDL_AudioSpec spec;
	Uint8   *sound;			
	Uint32   soundlen;		
	int      soundpos;		
} wave;


static void quit(int rc)
{
	SDL_Quit();
	exit(rc);
}


void SDLCALL fillerup(void *unused, Uint8 *stream, int len)
{
	Uint8 *waveptr;
	int    waveleft;

	
	waveptr = wave.sound + wave.soundpos;
	waveleft = wave.soundlen - wave.soundpos;

	
	while ( waveleft <= len ) {
		SDL_memcpy(stream, waveptr, waveleft);
		stream += waveleft;
		len -= waveleft;
		waveptr = wave.sound;
		waveleft = wave.soundlen;
		wave.soundpos = 0;
	}
	SDL_memcpy(stream, waveptr, len);
	wave.soundpos += len;
}

static int done = 0;
void poked(int sig)
{
	done = 1;
}

int main(int argc, char *argv[])
{
	char name[32];

	
	if ( SDL_Init(SDL_INIT_AUDIO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return(1);
	}
	if ( argv[1] == NULL ) {
		argv[1] = "sample.wav";
	}
	
	if ( SDL_LoadWAV(argv[1],
			&wave.spec, &wave.sound, &wave.soundlen) == NULL ) {
		fprintf(stderr, "Couldn't load %s: %s\n",
						argv[1], SDL_GetError());
		quit(1);
	}

	wave.spec.callback = fillerup;
#if HAVE_SIGNAL_H
	
#ifdef SIGHUP
	signal(SIGHUP, poked);
#endif
	signal(SIGINT, poked);
#ifdef SIGQUIT
	signal(SIGQUIT, poked);
#endif
	signal(SIGTERM, poked);
#endif 

	
	if ( SDL_OpenAudio(&wave.spec, NULL) < 0 ) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		SDL_FreeWAV(wave.sound);
		quit(2);
	}
	SDL_PauseAudio(0);

	
	printf("Using audio driver: %s\n", SDL_AudioDriverName(name, 32));
	while ( ! done && (SDL_GetAudioStatus() == SDL_AUDIO_PLAYING) )
		SDL_Delay(1000);

	
	SDL_CloseAudio();
	SDL_FreeWAV(wave.sound);
	SDL_Quit();
	return(0);
}
