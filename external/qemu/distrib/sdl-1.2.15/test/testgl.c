#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "SDL.h"

#ifdef __MACOS__
#define HAVE_OPENGL
#endif

#ifdef HAVE_OPENGL

#include "SDL_opengl.h"

#define SHADED_CUBE

#define LOGO_FILE	"icon.bmp"


static SDL_bool USE_DEPRECATED_OPENGLBLIT = SDL_FALSE;

static SDL_Surface *global_image = NULL;
static GLuint global_texture = 0;
static GLuint cursor_texture = 0;


void HotKey_ToggleFullScreen(void)
{
	SDL_Surface *screen;

	screen = SDL_GetVideoSurface();
	if ( SDL_WM_ToggleFullScreen(screen) ) {
		printf("Toggled fullscreen mode - now %s\n",
		    (screen->flags&SDL_FULLSCREEN) ? "fullscreen" : "windowed");
	} else {
		printf("Unable to toggle fullscreen mode\n");
	}
}

void HotKey_ToggleGrab(void)
{
	SDL_GrabMode mode;

	printf("Ctrl-G: toggling input grab!\n");
	mode = SDL_WM_GrabInput(SDL_GRAB_QUERY);
	if ( mode == SDL_GRAB_ON ) {
		printf("Grab was on\n");
	} else {
		printf("Grab was off\n");
	}
	mode = SDL_WM_GrabInput(!mode);
	if ( mode == SDL_GRAB_ON ) {
		printf("Grab is now on\n");
	} else {
		printf("Grab is now off\n");
	}
}

void HotKey_Iconify(void)
{
	printf("Ctrl-Z: iconifying window!\n");
	SDL_WM_IconifyWindow();
}

int HandleEvent(SDL_Event *event)
{
	int done;

	done = 0;
	switch( event->type ) {
	    case SDL_ACTIVEEVENT:
		
		printf( "app %s ", event->active.gain ? "gained" : "lost" );
		if ( event->active.state & SDL_APPACTIVE ) {
			printf( "active " );
		} else if ( event->active.state & SDL_APPMOUSEFOCUS ) {
			printf( "mouse " );
		} else if ( event->active.state & SDL_APPINPUTFOCUS ) {
			printf( "input " );
		}
		printf( "focus\n" );
		break;
		

	    case SDL_KEYDOWN:
		if ( event->key.keysym.sym == SDLK_ESCAPE ) {
			done = 1;
		}
		if ( (event->key.keysym.sym == SDLK_g) &&
		     (event->key.keysym.mod & KMOD_CTRL) ) {
			HotKey_ToggleGrab();
		}
		if ( (event->key.keysym.sym == SDLK_z) &&
		     (event->key.keysym.mod & KMOD_CTRL) ) {
			HotKey_Iconify();
		}
		if ( (event->key.keysym.sym == SDLK_RETURN) &&
		     (event->key.keysym.mod & KMOD_ALT) ) {
			HotKey_ToggleFullScreen();
		}
		printf("key '%s' pressed\n", 
			SDL_GetKeyName(event->key.keysym.sym));
		break;
	    case SDL_QUIT:
		done = 1;
		break;
	}
	return(done);
}

void SDL_GL_Enter2DMode()
{
	SDL_Surface *screen = SDL_GetVideoSurface();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, screen->w, screen->h);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, (GLdouble)screen->w, (GLdouble)screen->h, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void SDL_GL_Leave2DMode()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
}

static int power_of_two(int input)
{
	int value = 1;

	while ( value < input ) {
		value <<= 1;
	}
	return value;
}

GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord)
{
	GLuint texture;
	int w, h;
	SDL_Surface *image;
	SDL_Rect area;
	Uint32 saved_flags;
	Uint8  saved_alpha;

	
	w = power_of_two(surface->w);
	h = power_of_two(surface->h);
	texcoord[0] = 0.0f;			
	texcoord[1] = 0.0f;			
	texcoord[2] = (GLfloat)surface->w / w;	
	texcoord[3] = (GLfloat)surface->h / h;	

	image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN 
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
#endif
		       );
	if ( image == NULL ) {
		return 0;
	}

	
	saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
	saved_alpha = surface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, 0, 0);
	}

	
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}

	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     w, h,
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     image->pixels);
	SDL_FreeSurface(image); 

	return texture;
}

void DrawLogoCursor(void)
{
	static GLfloat texMinX, texMinY;
	static GLfloat texMaxX, texMaxY;
	static int w, h;
	int x, y;

	if ( ! cursor_texture ) {
		SDL_Surface *image;
		GLfloat texcoord[4];

		
		image = SDL_LoadBMP(LOGO_FILE);
		if ( image == NULL ) {
			return;
		}
		w = image->w;
		h = image->h;

		
		cursor_texture = SDL_GL_LoadTexture(image, texcoord);

		
		texMinX = texcoord[0];
		texMinY = texcoord[1];
		texMaxX = texcoord[2];
		texMaxY = texcoord[3];

		
		SDL_FreeSurface(image);

		
		if ( ! cursor_texture ) {
			return;
		}
	}

	
	SDL_GetMouseState(&x, &y);
	x -= w/2;
	y -= h/2;

	
	SDL_GL_Enter2DMode();
	glBindTexture(GL_TEXTURE_2D, cursor_texture);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texMinX, texMinY); glVertex2i(x,   y  );
	glTexCoord2f(texMaxX, texMinY); glVertex2i(x+w, y  );
	glTexCoord2f(texMinX, texMaxY); glVertex2i(x,   y+h);
	glTexCoord2f(texMaxX, texMaxY); glVertex2i(x+w, y+h);
	glEnd();
	SDL_GL_Leave2DMode();
}

void DrawLogoTexture(void)
{
	static GLfloat texMinX, texMinY;
	static GLfloat texMaxX, texMaxY;
	static int x = 0;
	static int y = 0;
	static int w, h;
	static int delta_x = 1;
	static int delta_y = 1;

	SDL_Surface *screen = SDL_GetVideoSurface();

	if ( ! global_texture ) {
		SDL_Surface *image;
		GLfloat texcoord[4];

		
		image = SDL_LoadBMP(LOGO_FILE);
		if ( image == NULL ) {
			return;
		}
		w = image->w;
		h = image->h;

		
		global_texture = SDL_GL_LoadTexture(image, texcoord);

		
		texMinX = texcoord[0];
		texMinY = texcoord[1];
		texMaxX = texcoord[2];
		texMaxY = texcoord[3];

		
		SDL_FreeSurface(image);

		
		if ( ! global_texture ) {
			return;
		}
	}

	
	x += delta_x;
	if ( x < 0 ) {
		x = 0;
		delta_x = -delta_x;
	} else
	if ( (x+w) > screen->w ) {
		x = screen->w-w;
		delta_x = -delta_x;
	}
	y += delta_y;
	if ( y < 0 ) {
		y = 0;
		delta_y = -delta_y;
	} else
	if ( (y+h) > screen->h ) {
		y = screen->h-h;
		delta_y = -delta_y;
	}

	
	SDL_GL_Enter2DMode();
	glBindTexture(GL_TEXTURE_2D, global_texture);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(texMinX, texMinY); glVertex2i(x,   y  );
	glTexCoord2f(texMaxX, texMinY); glVertex2i(x+w, y  );
	glTexCoord2f(texMinX, texMaxY); glVertex2i(x,   y+h);
	glTexCoord2f(texMaxX, texMaxY); glVertex2i(x+w, y+h);
	glEnd();
	SDL_GL_Leave2DMode();
}

void DrawLogoBlit(void)
{
	static int x = 0;
	static int y = 0;
	static int w, h;
	static int delta_x = 1;
	static int delta_y = 1;

	SDL_Rect dst;
	SDL_Surface *screen = SDL_GetVideoSurface();

	if ( global_image == NULL ) {
		SDL_Surface *temp;

		
		temp = SDL_LoadBMP(LOGO_FILE);
		if ( temp == NULL ) {
			return;
		}
		w = temp->w;
		h = temp->h;

		
		global_image = SDL_CreateRGBSurface(
				SDL_SWSURFACE,
				w, h,
				screen->format->BitsPerPixel,
				screen->format->Rmask,
				screen->format->Gmask,
				screen->format->Bmask,
				screen->format->Amask);
		if ( global_image ) {
			SDL_BlitSurface(temp, NULL, global_image, NULL);
		}
		SDL_FreeSurface(temp);

		
		if ( ! global_image ) {
			return;
		}
	}

	x += delta_x;
	if ( x < 0 ) {
		x = 0;
		delta_x = -delta_x;
	} else
	if ( (x+w) > screen->w ) {
		x = screen->w-w;
		delta_x = -delta_x;
	}
	y += delta_y;
	if ( y < 0 ) {
		y = 0;
		delta_y = -delta_y;
	} else
	if ( (y+h) > screen->h ) {
		y = screen->h-h;
		delta_y = -delta_y;
	}
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_BlitSurface(global_image, NULL, screen, &dst);

	
	SDL_UpdateRects(screen, 1, &dst);
}

int RunGLTest( int argc, char* argv[],
               int logo, int logocursor, int slowly, int bpp, float gamma, int noframe, int fsaa, int sync, int accel )
{
	int i;
	int rgb_size[3];
	int w = 640;
	int h = 480;
	int done = 0;
	int frames;
	Uint32 start_time, this_time;
        float color[8][3]= {{ 1.0,  1.0,  0.0}, 
			    { 1.0,  0.0,  0.0},
			    { 0.0,  0.0,  0.0},
			    { 0.0,  1.0,  0.0},
			    { 0.0,  1.0,  1.0},
			    { 1.0,  1.0,  1.0},
			    { 1.0,  0.0,  1.0},
			    { 0.0,  0.0,  1.0}};
	float cube[8][3]= {{ 0.5,  0.5, -0.5}, 
			   { 0.5, -0.5, -0.5},
			   {-0.5, -0.5, -0.5},
			   {-0.5,  0.5, -0.5},
			   {-0.5,  0.5,  0.5},
			   { 0.5,  0.5,  0.5},
			   { 0.5, -0.5,  0.5},
			   {-0.5, -0.5,  0.5}};
	Uint32 video_flags;
	int value;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf(stderr,"Couldn't initialize SDL: %s\n",SDL_GetError());
		exit( 1 );
	}

	
	if ( bpp == 0 ) {
		if ( SDL_GetVideoInfo()->vfmt->BitsPerPixel <= 8 ) {
			bpp = 8;
		} else {
			bpp = 16;  
		}
	}

	
	if ( logo && USE_DEPRECATED_OPENGLBLIT ) {
		video_flags = SDL_OPENGLBLIT;
	} else {
		video_flags = SDL_OPENGL;
	}
	for ( i=1; argv[i]; ++i ) {
		if ( strcmp(argv[i], "-fullscreen") == 0 ) {
			video_flags |= SDL_FULLSCREEN;
		}
	}

        if (noframe) {
           video_flags |= SDL_NOFRAME;
        }

	
	switch (bpp) {
	    case 8:
		rgb_size[0] = 3;
		rgb_size[1] = 3;
		rgb_size[2] = 2;
		break;
	    case 15:
	    case 16:
		rgb_size[0] = 5;
		rgb_size[1] = 5;
		rgb_size[2] = 5;
		break;
            default:
		rgb_size[0] = 8;
		rgb_size[1] = 8;
		rgb_size[2] = 8;
		break;
	}
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, rgb_size[0] );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, rgb_size[1] );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, rgb_size[2] );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	if ( fsaa ) {
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, fsaa );
	}
	if ( accel ) {
		SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	}
	if ( sync ) {
		SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
	} else {
		SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 0 );
	}
	if ( SDL_SetVideoMode( w, h, bpp, video_flags ) == NULL ) {
		fprintf(stderr, "Couldn't set GL mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	printf("Screen BPP: %d\n", SDL_GetVideoSurface()->format->BitsPerPixel);
	printf("\n");
	printf( "Vendor     : %s\n", glGetString( GL_VENDOR ) );
	printf( "Renderer   : %s\n", glGetString( GL_RENDERER ) );
	printf( "Version    : %s\n", glGetString( GL_VERSION ) );
	printf( "Extensions : %s\n", glGetString( GL_EXTENSIONS ) );
	printf("\n");

	SDL_GL_GetAttribute( SDL_GL_RED_SIZE, &value );
	printf( "SDL_GL_RED_SIZE: requested %d, got %d\n", rgb_size[0],value);
	SDL_GL_GetAttribute( SDL_GL_GREEN_SIZE, &value );
	printf( "SDL_GL_GREEN_SIZE: requested %d, got %d\n", rgb_size[1],value);
	SDL_GL_GetAttribute( SDL_GL_BLUE_SIZE, &value );
	printf( "SDL_GL_BLUE_SIZE: requested %d, got %d\n", rgb_size[2],value);
	SDL_GL_GetAttribute( SDL_GL_DEPTH_SIZE, &value );
	printf( "SDL_GL_DEPTH_SIZE: requested %d, got %d\n", bpp, value );
	SDL_GL_GetAttribute( SDL_GL_DOUBLEBUFFER, &value );
	printf( "SDL_GL_DOUBLEBUFFER: requested 1, got %d\n", value );
	if ( fsaa ) {
		SDL_GL_GetAttribute( SDL_GL_MULTISAMPLEBUFFERS, &value );
		printf("SDL_GL_MULTISAMPLEBUFFERS: requested 1, got %d\n", value );
		SDL_GL_GetAttribute( SDL_GL_MULTISAMPLESAMPLES, &value );
		printf("SDL_GL_MULTISAMPLESAMPLES: requested %d, got %d\n", fsaa, value );
	}
	if ( accel ) {
		SDL_GL_GetAttribute( SDL_GL_ACCELERATED_VISUAL, &value );
		printf( "SDL_GL_ACCELERATED_VISUAL: requested 1, got %d\n", value );
	}
	if ( sync ) {
		SDL_GL_GetAttribute( SDL_GL_SWAP_CONTROL, &value );
		printf( "SDL_GL_SWAP_CONTROL: requested 1, got %d\n", value );
	}

	
	SDL_WM_SetCaption( "SDL GL test", "testgl" );

	
	if ( gamma != 0.0 ) {
		SDL_SetGamma(gamma, gamma, gamma);
	}

	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );

	glOrtho( -2.0, 2.0, -2.0, 2.0, -20.0, 20.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LESS);

	glShadeModel(GL_SMOOTH);

	
	start_time = SDL_GetTicks();
	frames = 0;
	while( !done ) {
		GLenum gl_error;
		char* sdl_error;
		SDL_Event event;

		
		glClearColor( 0.0, 0.0, 0.0, 1.0 );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin( GL_QUADS );

#ifdef SHADED_CUBE
			glColor3fv(color[0]);
			glVertex3fv(cube[0]);
			glColor3fv(color[1]);
			glVertex3fv(cube[1]);
			glColor3fv(color[2]);
			glVertex3fv(cube[2]);
			glColor3fv(color[3]);
			glVertex3fv(cube[3]);
			
			glColor3fv(color[3]);
			glVertex3fv(cube[3]);
			glColor3fv(color[4]);
			glVertex3fv(cube[4]);
			glColor3fv(color[7]);
			glVertex3fv(cube[7]);
			glColor3fv(color[2]);
			glVertex3fv(cube[2]);
			
			glColor3fv(color[0]);
			glVertex3fv(cube[0]);
			glColor3fv(color[5]);
			glVertex3fv(cube[5]);
			glColor3fv(color[6]);
			glVertex3fv(cube[6]);
			glColor3fv(color[1]);
			glVertex3fv(cube[1]);
			
			glColor3fv(color[5]);
			glVertex3fv(cube[5]);
			glColor3fv(color[4]);
			glVertex3fv(cube[4]);
			glColor3fv(color[7]);
			glVertex3fv(cube[7]);
			glColor3fv(color[6]);
			glVertex3fv(cube[6]);

			glColor3fv(color[5]);
			glVertex3fv(cube[5]);
			glColor3fv(color[0]);
			glVertex3fv(cube[0]);
			glColor3fv(color[3]);
			glVertex3fv(cube[3]);
			glColor3fv(color[4]);
			glVertex3fv(cube[4]);

			glColor3fv(color[6]);
			glVertex3fv(cube[6]);
			glColor3fv(color[1]);
			glVertex3fv(cube[1]);
			glColor3fv(color[2]);
			glVertex3fv(cube[2]);
			glColor3fv(color[7]);
			glVertex3fv(cube[7]);
#else 
			glColor3f(1.0, 0.0, 0.0);
			glVertex3fv(cube[0]);
			glVertex3fv(cube[1]);
			glVertex3fv(cube[2]);
			glVertex3fv(cube[3]);
			
			glColor3f(0.0, 1.0, 0.0);
			glVertex3fv(cube[3]);
			glVertex3fv(cube[4]);
			glVertex3fv(cube[7]);
			glVertex3fv(cube[2]);
			
			glColor3f(0.0, 0.0, 1.0);
			glVertex3fv(cube[0]);
			glVertex3fv(cube[5]);
			glVertex3fv(cube[6]);
			glVertex3fv(cube[1]);
			
			glColor3f(0.0, 1.0, 1.0);
			glVertex3fv(cube[5]);
			glVertex3fv(cube[4]);
			glVertex3fv(cube[7]);
			glVertex3fv(cube[6]);

			glColor3f(1.0, 1.0, 0.0);
			glVertex3fv(cube[5]);
			glVertex3fv(cube[0]);
			glVertex3fv(cube[3]);
			glVertex3fv(cube[4]);

			glColor3f(1.0, 0.0, 1.0);
			glVertex3fv(cube[6]);
			glVertex3fv(cube[1]);
			glVertex3fv(cube[2]);
			glVertex3fv(cube[7]);
#endif 

		glEnd( );
		
		glMatrixMode(GL_MODELVIEW);
		glRotatef(5.0, 1.0, 1.0, 1.0);

		
		if ( logo ) {
			if ( USE_DEPRECATED_OPENGLBLIT ) {
				DrawLogoBlit();
			} else {
				DrawLogoTexture();
			}
		}
		if ( logocursor ) {
			DrawLogoCursor();
		}

		SDL_GL_SwapBuffers( );

		
		gl_error = glGetError( );

		if( gl_error != GL_NO_ERROR ) {
			fprintf( stderr, "testgl: OpenGL error: %d\n", gl_error );
		}

		sdl_error = SDL_GetError( );

		if( sdl_error[0] != '\0' ) {
			fprintf(stderr, "testgl: SDL error '%s'\n", sdl_error);
			SDL_ClearError();
		}

		
		if ( slowly ) {
			SDL_Delay( 20 );
		}

		
		while( SDL_PollEvent( &event ) ) {
			done = HandleEvent(&event);
		}
		++frames;
	}

	
	this_time = SDL_GetTicks();
	if ( this_time != start_time ) {
		printf("%2.2f FPS\n",
			((float)frames/(this_time-start_time))*1000.0);
	}

	if ( global_image ) {
	   	SDL_FreeSurface(global_image);
		global_image = NULL;
	}
	if ( global_texture ) {
		glDeleteTextures( 1, &global_texture );
		global_texture = 0;
	}
	if ( cursor_texture ) {
		glDeleteTextures( 1, &cursor_texture );
		cursor_texture = 0;
	}

	
	SDL_Quit( );
	return(0);
}

int main(int argc, char *argv[])
{
	int i, logo, logocursor = 0;
	int numtests;
	int bpp = 0;
	int slowly;
	float gamma = 0.0;
	int noframe = 0;
	int fsaa = 0;
	int accel = 0;
	int sync = 0;

	logo = 0;
	slowly = 0;
	numtests = 1;
	for ( i=1; argv[i]; ++i ) {
		if ( strcmp(argv[i], "-twice") == 0 ) {
			++numtests;
		}
		if ( strcmp(argv[i], "-logo") == 0 ) {
			logo = 1;
			USE_DEPRECATED_OPENGLBLIT = SDL_FALSE;
		}
		if ( strcmp(argv[i], "-logoblit") == 0 ) {
			logo = 1;
			USE_DEPRECATED_OPENGLBLIT = SDL_TRUE;
		}
		if ( strcmp(argv[i], "-logocursor") == 0 ) {
			logocursor = 1;
		}
		if ( strcmp(argv[i], "-slow") == 0 ) {
			slowly = 1;
		}
		if ( strcmp(argv[i], "-bpp") == 0 ) {
 		       bpp = atoi(argv[++i]);
		}
		if ( strcmp(argv[i], "-gamma") == 0 ) {
 		       gamma = (float)atof(argv[++i]);
		}
		if ( strcmp(argv[i], "-noframe") == 0 ) {
 		       noframe = 1;
		}
		if ( strcmp(argv[i], "-fsaa") == 0 ) {
 		       ++fsaa;
		}
		if ( strcmp(argv[i], "-accel") == 0 ) {
 		       ++accel;
		}
		if ( strcmp(argv[i], "-sync") == 0 ) {
 		       ++sync;
		}
		if ( strncmp(argv[i], "-h", 2) == 0 ) {
 		       printf(
"Usage: %s [-twice] [-logo] [-logocursor] [-slow] [-bpp n] [-gamma n] [-noframe] [-fsaa] [-accel] [-sync] [-fullscreen]\n",
 			      argv[0]);
			exit(0);
		}
	}
	for ( i=0; i<numtests; ++i ) {
 		RunGLTest(argc, argv, logo, logocursor, slowly, bpp, gamma, noframe, fsaa, sync, accel);
	}
	return 0;
}

#else 

int main(int argc, char *argv[])
{
	printf("No OpenGL support on this system\n");
	return 1;
}

#endif 
