#ifndef _STUDENTLIST_H_
#define _STUDENTLIST_H_

#include "student.h"
#include <stdbool.h>
#include "ADTs/iterator.h"
#define DEFAULT_CAPACITY 2L

typedef struct studentlist StudentList;

const StudentList *StudentList_create(long capacity, void(*freeV)(void*));

struct studentlist{
	void *self;
	void (*destroy)(const StudentList *lst);
	bool (*append)(const StudentList *lst, Student *st);
	const Iterator *(*itCreate)(const StudentList *lst);
};
#endif /* STUDENTLIST_H_ */