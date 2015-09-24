
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXPATHS (256*1024)

static void die(
  const char*msg
) {
  fprintf(stderr,"%s\n",msg);
  exit(1);
}


static int find_in(
  char *tab[],
  int max,
  const char *pat
) {
  int left=0, right=max-1;
  while (left <= right) {
    int mid = ((right-left)/2)+left;
    int v   = strcmp(pat,tab[mid]);
    if (v>0) {
      left = mid + 1;
    } else if (v<0) {
      right = mid -1;
    } else {
      return mid;
    }
  }
  return -1;
}


static void swap2(
  char *a[],
  char *b[],
  int i,
  int j
) {
  if (i==j) return;
  char*v;
  v=a[i]; a[i]=a[j]; a[j]=v;
  v=b[i]; b[i]=b[j]; b[j]=v;
}


static int partition(
  char *a[],
  char *b[],
  int left,
  int right,
  int p
) {
  const char *pivotValue = a[p];
  int i;
  swap2(a,b,p,right); 
  p = left;
  for (i=left; i<right; i++) {
    if (strcmp(a[i],pivotValue)<=0) {
      swap2(a,b,p,i);
      p++;
    }
  }
  swap2(a,b,right,p); 
  return p;
}


static void qsort_arr(
  char *a[],
  char *b[],
  int left,
  int right
) {
  while (right > left) {
    int p = left + (right-left)/2; 
    p = partition(a,b, left, right, p);
    if ((p-1) - left < right - (p+1)) {
      qsort_arr(a,b, left, p-1);
      left  = p+1;
    } else {
      qsort_arr(a,b, p+1, right);
      right = p-1;
    }
  }
}


static char* compact(
  char *expr
) {
  int l=strlen(expr);
  int i,j;
  for (i=0,j=0; i<l; i++) {
    if (expr[i]!='0') expr[j++] = expr[i];
  }
  expr[j]=0;
  return expr;
}


static void expand(
  char *expr,
  const char *pat,
  int l
) {
  int  el = 0;
  char last = '.';
  int  i;
  for (i=0; i<l; i++) {
    char c = pat[i];
    if ( (last<'0' || last>'9')
      && (c   <'0' || c   >'9')
      ) {
      expr[el++] = '0';
    }
    expr[el++] = c;
    last = c;
  }
  if (last<'0' || last>'9') expr[el++] = '0';
  expr[el]=0;
}


static char *combine(
  char *expr,
  const char *subexpr
) {
  int l1 = strlen(expr);
  int l2 = strlen(subexpr);
  int off = l1-l2;
  int j;
  
  
  
  for (j=0; j<l2; j++) {
    if (subexpr[j]>expr[off+j]) {
      expr[off+j] = subexpr[j];
    }
  }
  return expr;
}


int main(int argc, const char* argv[]) {
  FILE *in, *out;
  char *pattab_key[MAXPATHS];
  char *pattab_val[MAXPATHS];
  int   patterns = 0;
  char *newpattab_key[MAXPATHS];
  char *newpattab_val[MAXPATHS];
  int   newpatterns = 0;
  char format[132]; 
  int p;
  if (argc!=3) die("Usage: <orig-file> <new-file>\n");
  if ((in = fopen(argv[1],"r"))==NULL) die("Could not read input");
  if ((out = fopen(argv[2],"w"))==NULL) die("Could not create output");
  
  while(fgets(format,132,in)) {
    int l = strlen(format);
    if (format[l-1]=='\n') { l--; format[l]=0; } 
    if (format[0]=='%' || format[0]==0) {
      
    } else {
      if (format[l-1]=='%') {
        l--;
        format[l] = 0; 
      }
      int i,j;
      char *pat = (char*) malloc(l+1);
      char *org = (char*) malloc(l*2+1);
      expand(org,format,l);
      
      for (i=0,j=0; i<l; i++) {
        
        char c = format[i];
        if (c<'0' || c>'9') pat[j++]=c;
      }
      pat[j]=0;
      p = patterns;
      pattab_key[patterns]   = pat;
      pattab_val[patterns++] = org;
      if (patterns>MAXPATHS) die("to many base patterns");
    }
  }
  fclose(in);
  
  qsort_arr(pattab_key,pattab_val,0,patterns-1);

  for (p=0; p<patterns; p++) {
    char *pat = pattab_key[p];
    int   patsize = strlen(pat);
    int   j,l;
    for (l=1; l<=patsize; l++) {
      for (j=1; j<=l; j++) {
        int i = l-j;
        int  subpat_ndx;
        char subpat[132];
        strncpy(subpat,pat+i,j); subpat[j]=0;
        if ((subpat_ndx = find_in(pattab_key,patterns,subpat))>=0) {
          int   newpat_ndx;
          char *newpat=malloc(l+1);
      
          strncpy(newpat, pat+0,l); newpat[l]=0;
          if ((newpat_ndx = find_in(newpattab_key,newpatterns,newpat))<0) {
            char *neworg = malloc(132); 
            expand(neworg,newpat,l);
            newpattab_key[newpatterns]   = newpat;
            newpattab_val[newpatterns++] = combine(neworg,pattab_val[subpat_ndx]);
            if (newpatterns>MAXPATHS) die("to many new patterns");
    
          } else {
            free(newpat);
            newpattab_val[newpat_ndx] = combine(
              newpattab_val[newpat_ndx], pattab_val[subpat_ndx] ); 
          }
        }
      }
    }
  }

  for (p=0; p<newpatterns; p++) {
    fprintf(out,"%s\n",compact(newpattab_val[p]));
    free(newpattab_key[p]);
    free(newpattab_val[p]);
  }
  fclose(out);

  for (p=0; p<patterns; p++) {
    free(pattab_key[p]);
    free(pattab_val[p]);
  }
  return 0;
}
