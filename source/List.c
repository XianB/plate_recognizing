#include "include/List.h"

List create_list()
{
	List L;
	L = (struct Node *)malloc(sizeof(Node));

	if (L == NULL) {
		fprintf(stderr, "Error! out of space!\n");
	}

	L->next = NULL;

	return L;
}

void push_back(List L, CvRect i)
{
	struct Node * tmp = (struct Node *)malloc(sizeof(Node));
	if (tmp == NULL) {
		fprintf(stderr, "Error! out of space!\n");
	}


	tmp->item = i;
	L->next = tmp;
	tmp->next = NULL;
}


