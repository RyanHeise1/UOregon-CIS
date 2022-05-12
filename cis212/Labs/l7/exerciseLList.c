#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define USAGESTR "usage: %s -h | -t | -s\n"

typedef struct node {
	struct node *next;
	char *word;
} Node;

typedef struct llist {
	Node *head;
	Node *tail;
} LList;

Node *createNode(char *word){
	Node *p = (Node *)malloc(sizeof(Node));
	if (p != NULL){
		p->word = strdup(word);
		p->next = NULL;
	}
	return p;
}

LList *createLList(){
	LList *ll = (LList *)malloc(sizeof(LList));
	if (ll != NULL){
		ll->head = NULL;
		ll->tail = NULL;
	}
	return ll;
}

void freeLList(LList *ll){
	Node *p = ll->head;

	while(p!=NULL){
		Node *q = p->next;
		free(p->word);
		free(p);
		p = q;
	}
	free(ll);
}

void printLList(LList *ll){
	bool first = true;
	Node *p = ll->head;

	while(p != NULL){
		printf("%s%s", (first) ? "": " ", p->word);
		first = false;
		p = p->next;
	}
	if (!first)
		printf("\n");
	else
		printf("Empty List\n");
}

void addHead(LList *ll, char *word){
	Node *p = createNode(word);
	p->next = ll->head;
	ll->head = p;
	if(ll->tail == NULL)
		ll->tail = p;
}

void addTail(LList *ll, char *word){
	Node *p = createNode(word);
	if (ll->tail == NULL)
		ll->head = p;
	else
		ll->tail->next = p;
	ll->tail = p;
}

void addSort(LList *ll, char *word){
	Node *prv, *cur;
	Node *p = createNode(word);

	for (prv = NULL, cur = ll->head; cur != NULL; prv = cur, cur = prv->next){
		if(strcmp(word, cur->word) <= 0)
			break;
	}
	if (cur == NULL){
		if (ll->tail == NULL)
			ll->head = p;
		else 
			ll->tail->next = p;
		ll->tail = p;
	}else{
		p->next = cur;
		if(prv == NULL)
			ll->head = p;
		else
			prv->next = p;
	}
}

void procInput(LList *ll, bool ifH, bool ifT, bool ifS){
	char buf[BUFSIZ];

	while(fgets(buf, BUFSIZ, stdin) != NULL){
		char *p = strrchr(buf, '\n');
		*p = '\0';

		if(ifH)
			addHead(ll, buf);
		else if(ifT)
			addTail(ll, buf);
		else if (ifS)
			addSort(ll, buf);
	}
}

int main(int argc, char *argv[]) {
	int opt, nOptions = 0;
	bool ifH, ifT, ifS;
	LList *ll;

	ifH = ifT = ifS = false;
	opterr = 0;

	while ((opt = getopt(argc, argv, "hts")) != -1){
		switch(opt){
			case 'h': ifH = true; nOptions++; break;
			case 't': ifT = true; nOptions++; break;
			case 's': ifS = true; nOptions++; break;
			default:
				fprintf(stderr, "%s: illegal option '%c'\n", argv[0], optopt);
				fprintf(stderr, USAGESTR, argv[0]);
				return EXIT_FAILURE;
		}
	}
	if (nOptions != 1){
		fprintf(stderr, USAGESTR, argv[0]);
		return EXIT_FAILURE;
	}
	ll= createLList();
	procInput(ll, ifH, ifT, ifS);
	printLList(ll);
	freeLList(ll);

	return EXIT_SUCCESS;
}
