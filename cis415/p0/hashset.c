#include "hashset.h"  /* the .h file does NOT reside in /usr/local/include/ADTs */
#include <stdlib.h>
/* any other includes needed by your code */
#define UNUSED __attribute__((unused))
// pg 329
#define MAX_CAPACITY 134217728L
#define DEFAULT_CAPACITY 16
#define TRIGGER 100 /* number of changes that will trigger a load check */

typedef struct node {
    struct node *next;
    void **value;
} Node;

typedef struct s_data {
    /* definitions of the data members of self */
    long (*hashFxn)(void *, long n);
    int (*cmpFxn)(void *first, void *second);
    long size;
    long capacity;
    long changes;
    double load;
    double loadFactor;
    double increment;
    Node **buckets;
    void (*freeV)(void *V);
} SData;

/*
 * Traverses the map, calling the freeValue on each entry
 * then free storage associated with the SEntry structure
 */
static void s_purge(SData *sd, void (*freeV)(void *v)) {
    long i;

    for (i = 0L; i < sd->capacity; i++){
        Node *p, *q;
        p = sd->buckets[i];
        while(p != NULL) {
            if (freeV != NULL){
                (*freeV)(p->value);
            }
            q = p->next;
            free(p);
            p = q;
        }
        sd->buckets[i] = NULL;
    }
}

static void s_destroy(const Set *s) {
    /* implement the destroy() method */
    SData *sd = (SData *)s->self;
    s_purge(sd, sd->freeV);
    free(sd->buckets);
    free(sd);
    free((void *)s);
}

static void s_clear(const Set *s) {
    /* implement the clear() method */
    SData *sd = (SData *)s->self;
    s_purge(sd, sd->freeV);
    sd->size = 0;
    sd->load = 0.0;
    sd->changes = 0;
}

static void resize(SData *sd) {
    int N;
    Node *p, *q, **array;
    long i, j;

    N = 2 * sd->capacity;
    if (N > MAX_CAPACITY)
        N = MAX_CAPACITY;
    if (N == sd->capacity)
        return;

    array = (Node **)malloc(N * sizeof(Node *));
    if (array == NULL)
        return;
    for (j = 0; j < N; j++){
        array[j] = NULL;
    }

    // Now redistribute the entries into the new set of buckets
    for (i = 0; i < sd->capacity; i++){
        for (p = sd->buckets[i]; p != NULL; p = q) {
            q = p->next;
            j = sd->hashFxn(p->value, N);
            p->next = array[j];
            array[j] = p;
        }
    }
    free(sd->buckets);
    sd->buckets = array;
    sd->capacity = N;
    sd->load /= 2.0;
    sd->changes = 0;
    sd->increment = 1.0 / (double)N;
}

static int insertEntry(SData *sd, void *member, long i){
    Node *p = (Node *)malloc(sizeof(Node));
    int result = 0;

    if (p == NULL){
        free(p);
        return 0;
    }else{
        p->value = member;
        p->next = sd->buckets[i];
        sd->buckets[i] = p;
        sd->size++;
        sd->load += sd->increment;
        sd->changes++;
        result = 1;
    }
    return result;
}

static Node *findKey(SData *sd, void *key, long *bucket){
    /* Finds the node in the set */
    long i = sd->hashFxn(key, sd->capacity);
    Node *p;

    *bucket = i;
    for (p = sd->buckets[i]; p != NULL; p = p->next){
        if (sd->cmpFxn(p->value, key) == 0) {
            break;
        }
    }
    return p;
}

static bool s_contains(const Set *s, void *member) {
    /* implement the contains() method */
    SData *sd = (SData *)s->self;
    long bucket;

    return (findKey(sd, member, &bucket) != NULL);
}

static bool s_add(const Set *s, void *member) {
    /* implement the add() method */
    bool result = false;
    long i;
    SData *sd = (SData *)s->self;
    

    if (sd->changes > TRIGGER){
        sd->changes = 0;
        if (sd->load > sd->loadFactor)
            resize(sd);
    }

    Node *p = findKey(sd, member, &i);
    if (p == NULL){ // Node was not found
        insertEntry(sd, member, i);
        result = true;
    }
    return result;
}

static bool s_isEmpty(const Set *s) {
    /* implement the isEmpty() method */
    SData *sd = (SData *)s->self;
    return ((sd->size == 0L) ? true : false);
}

static bool s_remove(const Set *s, void *member) {
    /* implement the remove() method */
    SData *sd = (SData *)s->self;
    long i;
    Node *entry;
    int ans = 0;

    entry = findKey(sd, member, &i);
    if (entry != NULL){
        Node *p, *c;
        /*determine where the entry lives in the set*/
        for (p = NULL, c = sd->buckets[i]; c != entry; p = c, c = c->next){
            ;
        }
        if (p == NULL)
            sd->buckets[i] = entry->next;
        else
            p->next = entry->next;
        sd->size--;
        sd->load -= sd->increment;
        sd->changes++;
        sd->freeV(entry->value);
        free(entry);
        ans = 1;
    }
    return ans;
}

static long s_size(const Set *s) {
    /* implement the size() method */
    SData *sd = (SData *)s->self;
    return sd->size;
}

static void **keys(SData *sd){
    void **tmp  = NULL;
    if (sd->size > 0L){
        size_t nbytes = sd->size * sizeof(void *);
        tmp = (void *)malloc(nbytes);
        if (tmp != NULL){
            long i, n = 0L;
            for (i = 0L; i < sd->capacity; i++){
                Node *p = sd->buckets[i];
                while(p != NULL){
                    tmp[n++] = p->value;
                    p = p->next;
                }
            }
        }
    }
    return tmp;
}

static void **s_toArray(const Set *s, long *len) {
    /* implement the toArray() method */
    SData *sd = (SData *)s->self;
    void **tmp = keys(sd);

    if (tmp != NULL)
        *len = sd->size;
    return tmp;
}

static const Iterator *s_itCreate(const Set *s) {
    /* implement the itCreate() method */
    SData *sd = (SData *)s->self;
    const Iterator *it = NULL;
    void **tmp = (void **)keys(sd);
    if (tmp != NULL){
        it = Iterator_create(sd->size,tmp);
        if (it == NULL)
            free(tmp);
    }
    return it;
}

static Set template = {
    NULL, s_destroy, s_clear, s_add, s_contains, s_isEmpty, s_remove,
    s_size, s_toArray, s_itCreate
};

static const Set *newSet(void (*freeValue)(void*), int (*cmpFxn)(void*, void*),
                        long capacity, double loadFactor,
                        long (*hashFxn)(void *m, long N)){
    Set *s = (Set *)malloc(sizeof(SData));
    long N;
    double lf;
    Node **array;
    long i;

    if (s != NULL){
        SData *sd = (SData *)malloc(sizeof(SData));

        if (sd != NULL){
            N = ((capacity > 0) ? capacity : DEFAULT_CAPACITY);
            if ( N > MAX_CAPACITY)
                N = MAX_CAPACITY;
            lf = ((loadFactor > 0.000001) ? loadFactor : DEFAULT_LOAD_FACTOR);
            array = (Node **)malloc(N * sizeof(Node *));
            if (array != NULL){
                sd->capacity = N;
                sd->loadFactor = lf;
                sd->size = 0L;
                sd->load = 0.0;
                sd->changes = 0L;
                sd->increment = 1.0 / (double)N;
                sd->hashFxn = hashFxn;
                sd->cmpFxn = cmpFxn;
                sd->freeV = freeValue;
                sd->buckets = array;
                for (i = 0; i < N; i++)
                    array[i] = NULL;
                *s = template;
                s->self = sd;
            }
            else {
                free(sd);
                free(s);
                s = NULL;
            }
        }
        else{
            free(s);
            s = NULL;
        }
    }
    return s;
}

const Set *HashSet(void (*freeValue)(void*), int (*cmpFxn)(void*, void*),
                   long capacity, double loadFactor,
                   long (*hashFxn)(void *m, long N)
                  ) {
    /* construct a Set instance and return to the caller */
    return newSet(freeValue, cmpFxn, capacity, loadFactor, hashFxn);
}
