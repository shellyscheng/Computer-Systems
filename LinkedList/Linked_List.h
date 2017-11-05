//Authors: Shiying Cheng, Nikihil Kovelamudi


#ifndef LINKED_LIST_H
#define LINKED_LIST_H

//decleare the node and its pointer
typedef struct ll_node ll_node, *ll_node_ptr;

//construct the node with  value and a pointer inside
struct ll_node
{
	float value;
	ll_node_ptr after;
};

//add_front, rev, free, and print functions
ll_node_ptr ll_add_front(ll_node_ptr ptr, float num);
ll_node_ptr ll_rev(ll_node_ptr ptr);
void ll_free(ll_node_ptr ptr);
void ll_print(ll_node_ptr ptr);

#endif
