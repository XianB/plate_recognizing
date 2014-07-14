
#include "plate.h"
#ifndef _LIST_H_
#define _LIST_H_
struct Node {
	CvRect item;
	Node * next;
};

typedef struct Node * List;

List create_list();
void push_back(List L, CvRect i);
/*有头结点的链表*/

#endif
