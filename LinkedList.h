#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATATYPE char
#define FSPEC "%s"

int length(char*);

typedef struct node node;

struct node {
	DATATYPE *data;
	node *next;
};

typedef struct List {
	node *head;
} List;

List *createList(void){
	List *list = (List *) malloc(sizeof(List));
	list->head = NULL;
	return list;
}

node *makeNode(DATATYPE *data){
	node *n = malloc(sizeof(node));
	n->data = data;
	n->next = NULL;
	return n;
}

char *copy(char *a){
	char *c = malloc(sizeof(a) * length(a));
	for(int i = 0; i < length(a); i++){
		c[i] = a[i];
	}
	return a;
}

List *add(List *list, DATATYPE *data){
	node *curr = list->head;
	if(curr == NULL){
		list->head = makeNode(copy(data));
		return list;
	}
	while(curr->next != NULL)
		curr = curr->next;
	curr->next = makeNode(copy(data));
	return list;
}

void printList(List *l){
	node *c = l->head;
	if(c==NULL) return;
	while(c->next != NULL){
		printf(FSPEC, c->data);
		printf(" -> "); 
		c = c->next;
	}
	printf(FSPEC, c->data);
	printf("\n");
}

int length(char *str){
	int l = 0;
	while(str[l++] != '\0'){
	}
	return l - 1;
}

/*
int strcomp(char *str1, char *str2){
	int len1 = length(str1);
	int len2 = length(str2);
	if(len1 != len2)
		return 0;
}
*/

int inList(List *l, DATATYPE *d){
	node *c = l->head;
	while(c != NULL){
		if(strcmp(c->data, d) == 0)
			return 1;
		c = c->next;
	}
	return 0;
}

void import(List *l, char *filename){
	FILE *f;
        char *name = malloc(sizeof(char));
        name[0] = '\0';
        f = fopen(filename, "r");
        if (f != NULL) {
                char c;
                while((c = fgetc(f))!=EOF){
                        if(c==',' || c == '\n'){
                                add(l, name);
                                name = malloc(sizeof(char));
                                name[0] = '\0';
                        }
                        else if(c!='"'){
                                name = append(name, c);
                        }
                }
                add(l, name);              //add the last name not marked by a comma.
                fclose(f);
        }
}
