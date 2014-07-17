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

int count_node(List L)
{
	int count = 0;
	while (L != NULL) {
		count++;
		L = L->next;
	}

	return count;
}

bool is_empty(List L)
{
	return (L->next == NULL);
}

/*清空一个链表*/
void empty_list(List L)
{
	L->next = NULL;
}
