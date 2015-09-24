
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>     
#include <fcntl.h>     
#include <unistd.h>    
#include <sys/stat.h>  


jmp_buf env;

#if defined(VGA_s390x)

void handle_sigill(int signum)
{
   longjmp(env, 1);
}

unsigned long long stfle(void)
{

   unsigned long long ret;

   signal(SIGILL, handle_sigill);
   if (setjmp(env)) {
      
      return 0;
   } else {
      asm volatile("lghi 0, 0\n"
                   ".insn s,0xb2b00000,%0\n" 
      : "=Q" (ret)::"0", "cc");
      return ret;
   }
}



typedef struct {
   const char *cpuinfo_name;
   const char *real_name;
} model_info;

model_info models[] = {
   { "2064", "z900"   },
   { "2066", "z800"   },
   { "2084", "z990"   },
   { "2086", "z890"   },
   { "2094", "z9-ec"  },
   { "2096", "z9-bc"  },
   { "2097", "z10-ec" },
   { "2098", "z10-bc" },
   { "2817", "z196"   },
};


static model_info *locate_model(const char *name)
{
   model_info *p;

   
   for (p = models; p != models + sizeof models / sizeof models[0]; ++p) {
      if (strcmp(p->cpuinfo_name, name) == 0) return p;  
   }

   
   for (p = models; p != models + sizeof models / sizeof models[0]; ++p) {
      if (strcmp(p->real_name, name) == 0) return p;  
   }

   return NULL;
}


static model_info *get_host(void)
{
   int    n, fh;
   size_t num_bytes, file_buf_size;
   char  *p, *m, *model_name, *file_buf;
   model_info *model;

   
   
   fh = open("/proc/cpuinfo", O_RDONLY, S_IRUSR);
   if (fh < 0) return NULL;

   num_bytes = 0;
   file_buf_size = 1000;
   file_buf = malloc(file_buf_size + 1);

   while (42) {
      n = read(fh, file_buf, file_buf_size);
      if (n < 0) break;

      num_bytes += n;
      if (n < file_buf_size) break;  
   }

   if (n < 0) num_bytes = 0;   

   if (num_bytes > file_buf_size) {
      free(file_buf);
      lseek(fh, 0, SEEK_SET);
      file_buf = malloc(num_bytes + 1);
      n = read(fh, file_buf, num_bytes);
      if (n < 0) num_bytes = 0;
   }

   file_buf[num_bytes] = '\0';
   close(fh);

   
   model = models + sizeof models / sizeof models[0];
   for (p = file_buf; *p; ++p) {
      
      if (strncmp(p, "processor", sizeof "processor" - 1 ) != 0) continue;

      m = strstr(p, "machine");
      if (m == NULL) continue;

      p = m + sizeof "machine" - 1;
      while (isspace(*p) || *p == '=') {
         if (*p == '\n') goto next_line;
         ++p;
      }

      model_name = p;
      for (n = 0; n < sizeof models / sizeof models[0]; ++n) {
         model_info *mm = models + n;
         size_t len = strlen(mm->cpuinfo_name);
         if (strncmp(mm->cpuinfo_name, model_name, len) == 0 &&
             isspace(model_name[len])) {
            if (mm < model) model = mm;
            p = model_name + len;
            break;
         }
      }
      
      while (*p != '\n')
         ++p;
   next_line: ;
   }

   free(file_buf);

   if (model == models + sizeof models / sizeof models[0]) return NULL;

   return model;
}

static int go(char *feature, char *cpu)
{
   unsigned long long facilities;
   unsigned long long match;
   model_info *host, *from, *to, *p;
   char *colon;

   facilities = stfle();

   if        (strcmp(feature, "s390x-zarch") == 0 ) {
     match = (facilities & (1ULL << 62) && (facilities & (1ULL << 61)));
   } else if (strcmp(feature, "s390x-n3") == 0 ) {
     match = (facilities & (1ULL << 63));
   } else if (strcmp(feature, "s390x-stfle") == 0 ) {
     match = (facilities & (1ULL << 56));
   } else if (strcmp(feature, "s390x-ldisp") == 0 ) {
     match = (facilities & (1ULL << 45) && (facilities & (1ULL << 44)));
   } else if (strcmp(feature, "s390x-eimm") == 0 ) {
     match = (facilities & (1ULL << 42));
   } else if (strcmp(feature, "s390x-stckf") == 0 ) {
     match = (facilities & (1ULL << 38));
   } else if (strcmp(feature, "s390x-genins") == 0 ) {
     match = (facilities & (1ULL << 29));
   } else if (strcmp(feature, "s390x-exrl") == 0 ) {
     match = (facilities & (1ULL << 28));
   } else {
     return 2;          
   }

   if (match == 0) return 1;   

   
   if (cpu == NULL) return 0;

   host = get_host();
   if (host == NULL) return 2;  

   

   
   from = to = NULL;
   colon = strchr(cpu, ':');

   if (colon == NULL) {
      
      from = to = locate_model(cpu);
   } else if (colon == cpu) {
      
      from = models;
      to   = locate_model(cpu + 1);
   } else if (colon[1] == '\0') {
      
      *colon = '\0';
      from = locate_model(cpu);
      to   = models + sizeof models / sizeof models[0] - 1;
      *colon = ':';
   } else {
      
      *colon = '\0';
      from = locate_model(cpu);
      to   = locate_model(colon + 1);
      *colon = ':';
   }

   if (from == NULL || to == NULL || from > to) {
      fprintf(stderr, "invalid cpu specification '%s'\n", cpu);
      return 3;
   }

#if 0
   printf("from  %s (%s)  to  %s (%s)\n", from->cpuinfo_name, from->real_name,
          to->cpuinfo_name, to->real_name);
#endif

   
   for (p = from; p <= to; ++p) {
      if (p == host) return 0;
   }

   return 1; 
}

#else

static int go(char *feature, char *cpu)
{
   return 2;      
}

#endif


int main(int argc, char **argv)
{
   int rc;

   if (argc < 2 || argc > 3) {
      fprintf( stderr, "usage: s390x_features <feature> [<machine-model>]\n" );
      exit(3);                
   }

   rc = go(argv[1], argv[2]);

   

   return rc;
}
