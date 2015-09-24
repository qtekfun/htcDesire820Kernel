#ifndef __CSUTILHXX__
#define __CSUTILHXX__



void   mychomp(char * s);

char * mystrdup(const char * s);

char * mystrsep(char ** sptr, const char delim);



struct cs_info {
  unsigned char ccase;
  unsigned char clower;
  unsigned char cupper;
};


struct enc_entry {
  const char * enc_name;
  struct cs_info * cs_table;
};


struct lang_map {
  const char * lang;
  const char * def_enc;
};

struct cs_info * get_current_cs(const char * es);

const char * get_default_enc(const char * lang);

void enmkallcap(char * d, const char * p, const char * encoding);

void enmkallsmall(char * d, const char * p, const char * encoding);

void enmkinitcap(char * d, const char * p, const char * encoding);

void mkallcap(char * p, const struct cs_info * csconv);

void mkallsmall(char * p, const struct cs_info * csconv);

void mkinitcap(char * p, const struct cs_info * csconv);


#endif
