/*

/usr/src/ext2ed/main.c

A part of the extended file system 2 disk editor.

------------
Main program
------------

This file mostly contains:

1.	A list of global variables used through the entire program.
2.	The parser, which asks the command line from the user.
3.	The dispatcher, which analyzes the command line and calls the appropriate handler function.
4.	A command pattern matcher which is used along with the readline completion feature.
5.	A function which tells the user that an internal error has occured.

First written on: March 30 1995

Copyright (C) 1995 Gadi Oxman

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef HAVE_READLINE
#include <readline.h>
#include <history.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#else
extern int optind;
extern char *optarg;
#endif

#include "ext2ed.h"




char Ext2Descriptors [200]="ext2.descriptors";	
char AlternateDescriptors [200]="";		
char LogFile [200]="ext2ed.log";		
int LogChanges=1;				
int AllowChanges=0;				
int AllowMountedRead=0;				
int ForceExt2=0;				
int DefaultBlockSize=1024;
unsigned long DefaultTotalBlocks=2097151;
unsigned long DefaultBlocksInGroup=8192;	
int ForceDefault=0;				

char last_command_line [80];			

char device_name [80];				
FILE *device_handle=NULL;			
long device_offset;				
						

int mounted=0;					

struct struct_commands general_commands,ext2_commands;		
struct struct_descriptor *first_type,*last_type,*current_type;	
struct struct_type_data type_data;				
struct struct_file_system_info file_system_info;		
struct struct_file_info file_info,first_file_info;		
struct struct_group_info group_info;				
struct struct_super_info super_info;				
struct struct_remember_lifo remember_lifo;			
struct struct_block_bitmap_info block_bitmap_info;		
struct struct_inode_bitmap_info inode_bitmap_info;		

int redraw_request=0;						
								


int main (int argc, char **argv)
{
	int	write_priv = 0;
	int	c;
	char	*buf;

	if (!init ())
		return (1);
	while ((c = getopt (argc, argv, "w")) != EOF) {
		switch (c) {
		case 'w':
			write_priv++;
			break;
		}
	}
	if (optind < argc) {
		buf = malloc(strlen(argv[optind]) + 32);
		if (!buf) {
			fprintf(stderr, "Couldn't allocate filename buffer\n");
			exit(1);
		}
		strcpy(buf, "set_device ");
		strcat(buf, argv[optind]);
		set_device(buf);
		free(buf);
		if (write_priv) {
			wprintw (command_win,"\n");
			enable_write("enable_write");
		}
	}
	parser ();			
	prepare_to_close();		
	printf("Quitting ...\n");
	return(0);
}


int command_read_key()
{
	int	key = 0;

	while (!key) {
		if (redraw_request) {
			redraw_all();
			redraw_request=0;
		}
		key = wgetch(command_win);
		switch (key) {
		case 0x1A:
			key = 0;
			kill(getpid(), SIGTSTP);
			break;

		case KEY_NPAGE:
			pgdn("");
			refresh_command_win ();
			break;

		case KEY_PPAGE:
			pgup("");
			refresh_command_win ();
			break;
		case ERR:
			key = 0;
			break;

		case KEY_BACKSPACE:
			key = '\b';
		}
		if ((key < 32 && key != '\b' && key != '\n') ||
		    (key > 127))
			key = 0;
	}
	return key;
}

#ifdef HAVE_READLINE
int rl_getc_replacement(FILE *f)
{
	int	key = command_read_key();

	if (key == '\b') {
		if (rl_point > 0)
			wprintw(command_win, "\b \b");
	} else
		wprintw(command_win, "%c", key);
	return key;
}

void parser (void)
{
	char *ptr,command_line [80];
	int quit=0;

#if 0
	noecho();
	cbreak();
	keypad(command_win, 1);
	wtimeout(command_win, 100);

	rl_getc_function = rl_getc_replacement;
#endif

	while (!quit) {
		
		if (redraw_request) {
			redraw_all();
			redraw_request=0;
		}

		wmove (command_win,0,0);
		wclrtoeol (command_win);
		wprintw (command_win,"ext2ed > ");
		refresh_command_win ();

		mvcur (-1,-1,LINES-COMMAND_WIN_LINES,0);

		
		ptr=readline ("ext2ed > ");
		

		strcpy (command_line,ptr);
		free (ptr);

		if (*command_line != 0)
			add_history (command_line);

		
		if (*command_line==0)
			strcpy (command_line,last_command_line);

		
		mvcur (-1,-1,LINES-COMMAND_WIN_LINES,0);
		werase (command_win);
		wprintw (command_win,"ext2ed > ");
		wprintw (command_win,command_line);
		wprintw (command_win,"\n");
		refresh_command_win ();

		
		strcpy (last_command_line,command_line);

		
		quit=dispatch (command_line);
	}
}
#else
void read_line(char * foo) {
	char * chptr = foo;
	int ch;
	int done = 0;

	while (!done && (ch = command_read_key())) {
		switch (ch) {
		case '\n':
			done = 1;
			break;

		case '\b':
			if (chptr > foo) {
				wprintw(command_win, "\b \b");
				chptr--;
			}
			break;

		default:
			if (ch > 256)
				break;
			if (ch == '\n') break;
			*chptr++ = ch;
			wprintw(command_win, "%c", ch);
			break;
		}
	}
	*chptr = '\0';
}

void parser (void)
{
	char command_line [80];
	int quit=0;

	noecho();
	cbreak();
	wtimeout(command_win, 100);
	keypad(command_win, 1);

	while (!quit) {
		
		if (redraw_request) {
			redraw_all();
			redraw_request=0;
		}

		wmove (command_win,0,0);wclrtoeol (command_win);

		wmove(command_win, 0, 0);
		wprintw(command_win, "ext2ed > ");
		read_line(command_line);

		
 		if (*command_line==0)
 			strcpy (command_line,last_command_line);

		mvcur (-1,-1,LINES-COMMAND_WIN_LINES + 1,0);

 		strcpy (last_command_line,command_line);	

		
		quit=dispatch (command_line);
	}
}
#endif


int dispatch (char *command_line)
{
	int i,found=0;

	char command [80];

	parse_word (command_line,command);

	if (strcasecmp (command,"quit")==0) return (1);


	if (current_type != NULL)
		for (i=0;
		     i<=current_type->type_commands.last_command && !found;
		     i++) {
			if (strcasecmp (command,current_type->type_commands.names [i])==0) {
				(*current_type->type_commands.callback [i]) (command_line);
				found=1;
			}
		}

	

	if (!found)
		for (i=0;i<=ext2_commands.last_command && !found;i++) {
			if (strcasecmp (command,ext2_commands.names [i])==0) {
				(*ext2_commands.callback [i]) (command_line);
				found=1;
			}
		}


	

	if (!found)
		for (i=0;i<=general_commands.last_command && !found;i++) {
			if (strcasecmp (command,general_commands.names [i])==0) {
				(*general_commands.callback [i]) (command_line);
				found=1;
			}
		}

	

	if (!found) {
		wprintw (command_win,"Error: Unknown command\n");
		refresh_command_win ();
	}

	return (0);
}


char *parse_word (char *source,char *dest)
{
	char ch,*source_ptr,*target_ptr;

	if (*source==0) {
		*dest=0;
		return (source);
	};

	source_ptr=source;target_ptr=dest;
	do {
		ch=*source_ptr++;
	} while (! (ch>' ' && ch<='z') && ch!=0);

	while (ch>' ' && ch<='z') {
		*target_ptr++=ch;
		ch=*source_ptr++;
	}

	*target_ptr=0;

	source_ptr--;
	do {
		ch=*source_ptr++;
	} while (! (ch>' ' && ch<='z') && ch!=0);

	return (--source_ptr);
}

#if HAVE_READLINE
char *complete_command (char *text,int state)
{
	int state_index=-1;
	int i,len;

	len=strlen (text);

	

	if (current_type != NULL)
		for (i=0;i<=current_type->type_commands.last_command;i++) {
			if (strncmp (current_type->type_commands.names [i],text,len)==0) {
				state_index++;
				if (state==state_index) {
					return (dupstr (current_type->type_commands.names [i]));
				}
			}
		}

	

	for (i=0;i<=ext2_commands.last_command;i++) {
		if (strncmp (ext2_commands.names [i],text,len)==0) {
			state_index++;
			if (state==state_index)
			return (dupstr (ext2_commands.names [i]));
		}
	}


	

	for (i=0;i<=general_commands.last_command;i++) {
		if (strncmp (general_commands.names [i],text,len)==0) {
				state_index++;
				if (state==state_index)
					return (dupstr (general_commands.names [i]));
		}
	}

	

	if (strncmp ("quit",text,len)==0) {
		state_index++;
		if (state==state_index)
			return (dupstr ("quit"));
	}

	

	return ((char *) NULL);
}
#endif


char *dupstr (char *src)
{
	char *ptr;

	ptr=(char *) malloc (strlen (src)+1);
	strcpy (ptr,src);
	return (ptr);
}

#ifdef DEBUG
void internal_error (char *description,char *source_name,char *function_name)
{
	wprintw (command_win,"Internal error - Found by source: %s.c , function: %s\n",source_name,function_name);
	wprintw (command_win,"\t%s\n",description);
	wprintw (command_win,"Press enter to (hopefully) continue\n");
	refresh_command_win ();getch ();werase (command_win);
}

#endif