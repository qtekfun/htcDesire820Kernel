
#define HASH_MAP_H <hash_map>

#if _MSC_VER < 1310 || _MSC_VER >= 1600
#define HASH_NAMESPACE std
#else
#define HASH_NAMESPACE stdext
#endif

#define HASH_SET_H <hash_set>

#define HAVE_HASH_MAP 1

#define HAVE_HASH_SET 1

