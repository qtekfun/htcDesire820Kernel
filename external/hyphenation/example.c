#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "hyphen.h"
#include "csutil.h"

#define BUFSIZE 1000

void help() {
    fprintf(stderr,"correct syntax is:\n"); 
    fprintf(stderr,"example [-d | -dd] hyphen_dictionary_file file_of_words_to_check\n");
    fprintf(stderr,"-o = use old algorithm (without non-standard hyphenation)\n");
    fprintf(stderr,"-d = hyphenation with listing of the possible hyphenations\n");
}

char * hindex(char * word, int n, int utf8) {
    int j = 0;
    while (j < n) {
        j++;
        word++;
        while (utf8 && ((((unsigned char) *word) >> 6) == 2)) word++;
    }
    return word;
}

void single_hyphenations(char * word, char * hyphen, char ** rep, int * pos, int * cut, int utf8) {
    int i, k, j = 0;
    char r;
    for (i = 0; (i + 1) < strlen(word); i++) {
        if (utf8 && ((((unsigned char) word[i]) >> 6) == 2)) continue;
        if ((hyphen[j] & 1)) {
            if (rep && rep[j]) {
              k = hindex(word, j - pos[j] + 1, utf8) - word;
              r = word[k];
              word[k] = 0;
              printf(" - %s%s", word, rep[j]);
              word[k] = r;
              printf("%s\n", hindex(word + k, cut[j], utf8));
            } else {
              k = hindex(word, j + 1, utf8) - word;
              r = word[k];
              word[k] = 0;
              printf(" - %s=", word);
              word[k] = r;
              printf("%s\n", word + k);
            }
        }
        j++;
    }
}

int 
main(int argc, char** argv)
{

    HyphenDict *dict;
    int df;
    int wtc;
    FILE* wtclst;
    int k, n, i, j, c;
    char buf[BUFSIZE + 1];
    int  nHyphCount;
    char *hyphens;
    char *lcword;
    char *hyphword;
    char hword[BUFSIZE * 2];
    int arg = 1;
    int optd = 1;
    int optdd = 0;
    char ** rep;
    int * pos;
    int * cut;

  
  

  if (argv[arg]) {
       if (strcmp(argv[arg], "-o") == 0) {
            optd = 0;
            arg++;
       }
       if (argv[arg] && strcmp(argv[arg], "-d") == 0) {
            optd = 1;
            optdd = 1;
            arg++;
       }
  }

  if (argv[arg]) {
       df = arg++;
  } else {
    help();
    exit(1);
  }

  if (argv[arg]) {
       wtc = arg++;
  } else {
    help();
    exit(1);
  }

    
  if ((dict = hnj_hyphen_load(argv[df])) == NULL) {
       fprintf(stderr, "Couldn't find file %s\n", argv[df]);
       fflush(stderr);
       exit(1);
  }

  
  wtclst = fopen(argv[wtc],"r");
  if (!wtclst) {
    fprintf(stderr,"Error - could not open file of words to check\n");
    exit(1);
  }

    
  
    while(fgets(buf,BUFSIZE,wtclst)) {
       k = strlen(buf);
       if (buf[k - 1] == '\n') buf[k - 1] = '\0';
       if (*buf && buf[k - 2] == '\r') buf[k-- - 2] = '\0';

       
       
       lcword = (char *) malloc(k+1);
       hyphens = (char *)malloc(k+5);
       if (dict->utf8) {
         strcpy(lcword, buf);
       } else {
         enmkallsmall(lcword,buf,dict->cset);
       }

       
       n = k-1;
       while((n >=0) && (lcword[n] == '.')) n--;
       n++;

       
       
       rep = NULL;
       pos = NULL;
       cut = NULL;
       hword[0] = '\0';

       if ((!optd && hnj_hyphen_hyphenate(dict, lcword, n-1, hyphens)) ||
	    (optd && hnj_hyphen_hyphenate2(dict, lcword, n-1, hyphens, hword, &rep, &pos, &cut))) {
             free(hyphens);
             free(lcword);
             fprintf(stderr, "hyphenation error\n");
             exit(1);
       }

       if (!optd) {
         
         for (c = n; c < k; c++) hyphens[c] = '0';
         hyphens[k] = '\0';

         
         
         nHyphCount = 0;
         for (i = 0; i < n; i++)
           if (hyphens[i]&1)
             nHyphCount++;
         hyphword = (char *) malloc(k+1+nHyphCount);
         j = 0;
         for (i = 0; i < n; i++) {
	   hyphword[j++] = buf[i];
           if (hyphens[i]&1) {
	      hyphword[j++] = '-';
	   }
         }
         hyphword[j] = '\0';
         fprintf(stdout,"%s\n",hyphword);
         fflush(stdout);
         free(hyphword);
      } else {
         fprintf(stdout,"%s\n", hword);
         if (optdd) single_hyphenations(lcword, hyphens, rep, pos, cut, dict->utf8);
         if (rep) {
            for (i = 0; i < n - 1; i++) {
                if (rep[i]) free(rep[i]);
            }
            free(rep);
            free(pos);
            free(cut);
         }
      }
      free(hyphens);
      free(lcword);
    }

    fclose(wtclst);
    hnj_hyphen_free(dict);
    return 0;
}
