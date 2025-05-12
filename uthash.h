
/* uthash.h from: https://troydhanson.github.io/uthash/ */
#include <stddef.h>
#define uthash_malloc(sz) malloc(sz)
#define uthash_free(ptr,sz) free(ptr)
#define uthash_noexpand_fyi(tbl)
#define uthash_expand_fyi(tbl)
#define uthash_fatal(msg) exit(-1)

typedef struct UT_hash_handle {
    struct UT_hash_table *tbl;
    void *prev;
    void *next;
    unsigned hh_prev;
    unsigned hh_next;
} UT_hash_handle;

#define HASH_ADD_STR(head,strfield,add) HASH_ADD(hh,head,strfield,strlen(add->strfield),add)
#define HASH_FIND_STR(head,findstr,out) HASH_FIND(hh,head,findstr,strlen(findstr),out)
#define HASH_DEL(head,delptr) uthash_free((delptr),sizeof(*(delptr)))
#define HASH_ITER(hh,head,el,tmp) for((el)=(head); (el) != NULL; (el) = (tmp))

