
typedef struct _SYNCH_HANDLE_STRUCTURE {
   HANDLE hEvent;
   HANDLE hMutex;
   HANDLE hSemph;
   LONG MaxCount;
   volatile LONG CurCount;
   LPCTSTR lpName;
} SYNCH_HANDLE_STRUCTURE, *SYNCHHANDLE;

#define SYNCH_HANDLE_SIZE sizeof (SYNCH_HANDLE_STRUCTURE)

        
#define SYNCH_ERROR 0X20000000   

extern SYNCHHANDLE CreateSemaphoreCE (LPSECURITY_ATTRIBUTES, LONG, LONG, LPCTSTR);

extern BOOL ReleaseSemaphoreCE (SYNCHHANDLE, LONG, LPLONG);
extern DWORD WaitForSemaphoreCE (SYNCHHANDLE, DWORD);

extern BOOL CloseSynchHandle (SYNCHHANDLE);
