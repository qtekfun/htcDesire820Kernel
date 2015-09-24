

#include <stdlib.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif

#include "SDL.h"
#include "SDL_endian.h"


#include <stdio.h>	

#define FBASENAME1	"sdldata1"		
#define FBASENAME2	"sdldata2"		


#ifndef NULL
#define NULL ((void *)0)
#endif 

static void cleanup( void ) {

	unlink(FBASENAME1);
	unlink(FBASENAME2); 
}

static void rwops_error_quit( unsigned line, SDL_RWops *rwops) {
	
	printf("testfile.c(%d): failed\n",line);
	if (rwops) {
		rwops->close(rwops); 
	}
	cleanup();
	exit(1); 
}

#define RWOP_ERR_QUIT(x)	rwops_error_quit( __LINE__, (x) )



int main(int argc, char *argv[])
{
	SDL_RWops *rwops = NULL;
	char test_buf[30];
	
	cleanup();

	
	rwops = SDL_RWFromFile(NULL,NULL);
	if (rwops) RWOP_ERR_QUIT(rwops);
	rwops = SDL_RWFromFile(NULL,"ab+");
	if (rwops) RWOP_ERR_QUIT(rwops);
	rwops = SDL_RWFromFile(NULL,"sldfkjsldkfj");
	if (rwops) RWOP_ERR_QUIT(rwops);
	rwops = SDL_RWFromFile("something","");
	if (rwops) RWOP_ERR_QUIT(rwops);
	rwops = SDL_RWFromFile("something",NULL);
	if (rwops) RWOP_ERR_QUIT(rwops);	
	printf("test1 OK\n");

	rwops = SDL_RWFromFile(FBASENAME2,"rb"); 
	if (rwops) RWOP_ERR_QUIT(rwops);
	rwops = SDL_RWFromFile(FBASENAME2,"rb+"); 
	if (rwops) RWOP_ERR_QUIT(rwops);
	rwops = SDL_RWFromFile(FBASENAME2,"wb");
	if (!rwops) RWOP_ERR_QUIT(rwops); 
	rwops->close(rwops); unlink(FBASENAME2);
	rwops = SDL_RWFromFile(FBASENAME2,"wb+");
	if (!rwops) RWOP_ERR_QUIT(rwops); 
	rwops->close(rwops); unlink(FBASENAME2);
	rwops = SDL_RWFromFile(FBASENAME2,"ab"); 
	if (!rwops) RWOP_ERR_QUIT(rwops); 
	rwops->close(rwops); unlink(FBASENAME2);
	rwops = SDL_RWFromFile(FBASENAME2,"ab+");
	if (!rwops) RWOP_ERR_QUIT(rwops); 
	rwops->close(rwops); unlink(FBASENAME2);
	printf("test2 OK\n");

	rwops = SDL_RWFromFile(FBASENAME1,"wb"); 	
	if (!rwops)											RWOP_ERR_QUIT(rwops);
	if (1 != rwops->write(rwops,"1234567890",10,1) )	RWOP_ERR_QUIT(rwops);		
	if (10 != rwops->write(rwops,"1234567890",1,10) )	RWOP_ERR_QUIT(rwops);		
	if (7 != rwops->write(rwops,"1234567",1,7) )		RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);					
	if (0!=rwops->read(rwops,test_buf,1,1))				RWOP_ERR_QUIT(rwops); 
	rwops->close(rwops);

	rwops = SDL_RWFromFile(FBASENAME1,"rb"); 
	if (!rwops)											RWOP_ERR_QUIT(rwops);
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);					
	if (20!=rwops->seek(rwops,-7,RW_SEEK_END))			RWOP_ERR_QUIT(rwops);					
	if (7!=rwops->read(rwops,test_buf,1,7))				RWOP_ERR_QUIT(rwops);		
	if (SDL_memcmp(test_buf,"1234567",7))				RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->read(rwops,test_buf,1,1))				RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->read(rwops,test_buf,10,100))			RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->seek(rwops,-27,RW_SEEK_CUR))			RWOP_ERR_QUIT(rwops);		
	if (2!=rwops->read(rwops,test_buf,10,3))			RWOP_ERR_QUIT(rwops);				
	if (SDL_memcmp(test_buf,"12345678901234567890",20))	RWOP_ERR_QUIT(rwops);
	if (0!=rwops->write(rwops,test_buf,1,1))			RWOP_ERR_QUIT(rwops); 
	rwops->close(rwops);

	rwops = SDL_RWFromFile(FBASENAME1,"wb+"); 		
	if (!rwops)											RWOP_ERR_QUIT(rwops);
	if (1 != rwops->write(rwops,"1234567890",10,1) )	RWOP_ERR_QUIT(rwops);		
	if (10 != rwops->write(rwops,"1234567890",1,10) )	RWOP_ERR_QUIT(rwops);		
	if (7 != rwops->write(rwops,"1234567",1,7) )		RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);					
	if (1!=rwops->read(rwops,test_buf,1,1))				RWOP_ERR_QUIT(rwops); 
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);					
	if (20!=rwops->seek(rwops,-7,RW_SEEK_END))			RWOP_ERR_QUIT(rwops);					
	if (7!=rwops->read(rwops,test_buf,1,7))				RWOP_ERR_QUIT(rwops);		
	if (SDL_memcmp(test_buf,"1234567",7))				RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->read(rwops,test_buf,1,1))				RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->read(rwops,test_buf,10,100))			RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->seek(rwops,-27,RW_SEEK_CUR))			RWOP_ERR_QUIT(rwops);		
	if (2!=rwops->read(rwops,test_buf,10,3))			RWOP_ERR_QUIT(rwops);				
	if (SDL_memcmp(test_buf,"12345678901234567890",20))	RWOP_ERR_QUIT(rwops);
	rwops->close(rwops);
	printf("test3 OK\n");

	rwops = SDL_RWFromFile(FBASENAME1,"rb+"); 		
	if (!rwops)											RWOP_ERR_QUIT(rwops);
	if (1 != rwops->write(rwops,"1234567890",10,1) )	RWOP_ERR_QUIT(rwops);		
	if (10 != rwops->write(rwops,"1234567890",1,10) )	RWOP_ERR_QUIT(rwops);		
	if (7 != rwops->write(rwops,"1234567",1,7) )		RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);					
	if (1!=rwops->read(rwops,test_buf,1,1))				RWOP_ERR_QUIT(rwops); 
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);					
	if (20!=rwops->seek(rwops,-7,RW_SEEK_END))			RWOP_ERR_QUIT(rwops);					
	if (7!=rwops->read(rwops,test_buf,1,7))				RWOP_ERR_QUIT(rwops);		
	if (SDL_memcmp(test_buf,"1234567",7))				RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->read(rwops,test_buf,1,1))				RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->read(rwops,test_buf,10,100))			RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->seek(rwops,-27,RW_SEEK_CUR))			RWOP_ERR_QUIT(rwops);		
	if (2!=rwops->read(rwops,test_buf,10,3))			RWOP_ERR_QUIT(rwops);				
	if (SDL_memcmp(test_buf,"12345678901234567890",20))	RWOP_ERR_QUIT(rwops);
	rwops->close(rwops);
	printf("test4 OK\n");

	rwops = SDL_RWFromFile(FBASENAME1,"ab+"); 		
	if (!rwops)											RWOP_ERR_QUIT(rwops);
	if (1 != rwops->write(rwops,"1234567890",10,1) )	RWOP_ERR_QUIT(rwops);		
	if (10 != rwops->write(rwops,"1234567890",1,10) )	RWOP_ERR_QUIT(rwops);		
	if (7 != rwops->write(rwops,"1234567",1,7) )		RWOP_ERR_QUIT(rwops); 	
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);					
	
	if (1!=rwops->read(rwops,test_buf,1,1))				RWOP_ERR_QUIT(rwops); 
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);					
	
	if (20+27!=rwops->seek(rwops,-7,RW_SEEK_END))		RWOP_ERR_QUIT(rwops);					
	if (7!=rwops->read(rwops,test_buf,1,7))				RWOP_ERR_QUIT(rwops);		
	if (SDL_memcmp(test_buf,"1234567",7))				RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->read(rwops,test_buf,1,1))				RWOP_ERR_QUIT(rwops);		
	if (0!=rwops->read(rwops,test_buf,10,100))			RWOP_ERR_QUIT(rwops);		
	
	if (27!=rwops->seek(rwops,-27,RW_SEEK_CUR))			RWOP_ERR_QUIT(rwops);
	
	if (0!=rwops->seek(rwops,0L,RW_SEEK_SET))			RWOP_ERR_QUIT(rwops);
	if (3!=rwops->read(rwops,test_buf,10,3))			RWOP_ERR_QUIT(rwops);				
	if (SDL_memcmp(test_buf,"123456789012345678901234567123",30))	
														RWOP_ERR_QUIT(rwops);
	rwops->close(rwops);
	printf("test5 OK\n");
	cleanup();
	return 0; 
}
