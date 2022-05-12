#include "addresslist.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define UNUSED __attribute__((unused))
#define DEFAULT_CAPACITY 50L

typedef struct self {
	long capacity;
	long size;
	void **theArray;
} SlData;

/*  
* destroy the AddressList, invoking free() on each element  
*/
static void sl_destroy(const AddressList *sl) {
    SlData *sld = (SlData *)(sl->self);
    long i;
    for(i = 0L; i < sld->size; i++){
        free(sld->theArray[i]);
    }
    free(sld->theArray);
    free(sld);
    free((void *)sl);
}

/*  
* insert string (allocated from the heap) into the AddressList at the specified  
* index, moving the current values at [index, size) to [index+1, size+1);  
* grow the capacity of the list, if necessary  
* if the current size of the list is N, legal values of index are in the 
 * interval [0, N]  
 *  
 * returns true (non-zero) if the string was successfully inserted into the  
 * list, false (zero) if the insert was unsuccessful  
 */
static int sl_insert(const AddressList *sl, long index, char *addr) {
    SlData *sld = (SlData *)(sl->self);
    int status = 1;

    if (index > sld->size)
    	return 0;

    if (sld->capacity <= sld->size) { /*need to reallocate*/
    	size_t nbytes = 2 * sld->capacity * sizeof(void *);
    	void **tmp = (void **)realloc(sld->theArray, nbytes);
    	if (tmp == NULL){
    		status = 0; /* allocation failure*/
    	}
    	else {
    		sld->theArray = tmp;
    		sld->capacity *= 2;
    	}
    }
    if (status){
    	long j;
    	for (j = sld->size; j > index; j--) /*slide items up*/
    		sld->theArray[j] = sld->theArray[j-1];
    	sld->theArray[index] = addr;
    	sld->size++;
    }
    return status;
}

/*  
* retrieve string at `index’ from the AddressList  
*  
* returns true (non-zero) if `index’ was legal, false (zero) if not  
*/     
static int sl_get(const AddressList *sl, long index, char **addrptr) {
    SlData *sld = (SlData *)(sl->self);
    int status = (index >= 0L && index < sld->size);

    if (status)
    	*addrptr = sld->theArray[index];
    return status;
}

/*  
* return the number of addresses in the list, N;  
* legal indices: 0 <= index < N  
*/  
static long sl_size(const AddressList *sl) {
    SlData *sld = (SlData *)(sl->self);
    return sld->size;
}

static AddressList template = {
	NULL, sl_destroy, sl_insert, sl_get, sl_size
};

const AddressList *AddressList_create(long capacity) {
    AddressList *sl = (AddressList *)malloc(sizeof(AddressList));

    if (sl != NULL) {
    	SlData *sld = (SlData *)malloc(sizeof(SlData));
    	if (sld != NULL) {
    		long cap = (capacity <= 0) ? DEFAULT_CAPACITY : capacity;
    		void **array = (void **)malloc(cap * sizeof (void *));
    		if (array != NULL){
    			sld->capacity = cap;
    			sld ->size = 0L;
    			sld->theArray = array;
    			*sl = template;
    			sl->self = sld;
    		} else {
                free(sld);
    			free(sl);
    			sl = NULL;
    		}
    	} else {
    		free(sl);
    		sl = NULL;
    	}	
    }
    return sl;
}
