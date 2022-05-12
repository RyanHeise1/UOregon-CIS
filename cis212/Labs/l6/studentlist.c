#include "studentlist.h"
#include <stdlib.h>
#include <string.h>

typedef struct self{
	long capacity;
	long size;
	Student **array;
	void (*freeV)(void *e);
}Self;

static void lst_destroy(const StudentList *lst){
	Self *self = (Self *)lst->self;
	long i;
	for (i = 0L; i < self->size; i++){
		self->freeV(self->array[i]);
	}
	free(self->array);
	free(self);
	free((void*) lst);
}

static bool lst_append(const StudentList *lst, Student *st){
	Self *self = (Self*)lst->self;
	bool ifSpace = (self->size < self->capacity);

	if (!ifSpace){
		size_t nbytes = 2*self->capacity * sizeof(Student *);
		Student **temp = (Student **)realloc(self->array, nbytes);
		if (temp != NULL){
			self->array = temp;
			self->capacity *= 2;
			ifSpace = true;
		}
	}
	if (ifSpace){self->array[self->size++] = st;}
	return ifSpace;
}

static Student **copyArray(Self * self){
	Student **temp = NULL;
	if(self->size > 0L){
		size_t nbytes = self->size *sizeof(Student *);
		temp = (Student **)malloc(nbytes);
		if(temp != NULL){
			long i;
			for (i=0; i < self->size; i++){
				temp[i] = self->array[i];
			}
		}
	}
	return temp;
}

static const Iterator *lst_itCreate(const StudentList *lst){
	Self *self = (Self *)lst->self;
	const Iterator *it = NULL;
	Student ** temp = copyArray(self);

	if(temp != NULL){
		it = Iterator_create(self->size, (void **)temp);
		if (it == NULL){
			free(temp);
		}
	}
	return it;
}

#define UNUSED __attribute__((unused))
static void doNothing(UNUSED void *x){

}

const StudentList *StudentList_create(long capacity, void(*freeV)(void*)){
	StudentList *lst = (StudentList *)malloc(sizeof(StudentList));
	if(lst != NULL){
		Self *self = (Self*)malloc(sizeof(self));
		if(self != NULL){
			long cap = (capacity <= 0L) ? DEFAULT_CAPACITY : capacity;
			Student **array = (Student **)malloc(cap * sizeof(Student*));
			if (array != NULL){
				self->capacity = cap;
				self->size = 0L;
				self->array = array;
				self->freeV = (freeV == NULL) ? doNothing : freeV;

				lst->self = self;
				lst->destroy = lst_destroy;
				lst->append = lst_append;
				lst->itCreate = lst_itCreate;
			}else{
				free(self); free(lst); lst = NULL;
			}
		}else{
			free(lst); lst = NULL;
		}
	}
	return lst;
}
