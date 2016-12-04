//Shiying Cheng, Nikihil Kovelamudi

#include <stdlib.h>
#include "Linked_List.h"

ll_node_ptr ll_add_front(ll_node_ptr ptr, float num) //adds a new node and returns the new pointer to the front of the linked list
{
	ll_node_ptr new_ptr= (ll_node_ptr)malloc(sizeof(ll_node)); //allocates memory for the node pointer
	new_ptr->value = num;
	new_ptr->after = ptr;
	return new_ptr;
}

ll_node_ptr ll_rev(ll_node_ptr ptr)	//reverses the linked list
{
	ll_node_ptr A = ptr;
	ll_node_ptr B = A->after;
	ll_node_ptr C = B->after;
	A->after = NULL;
	while (C)
	{
		B->after = A;
		if (!C->after)
		{
			C->after = B;
			break;  //breaks if C is at the last node
		}		
		A = B;
		B = C;
		C = B->after;	
	}
	/*A->after = NULL;  //testing code
	B-> after = A;
	C->after = B;*/
	return C;
}

void ll_free (ll_node_ptr ptr) //frees the memory slots used
{
	if(ptr->after)
		{ll_free(ptr->after);}
	free(ptr); //base case of recursion
}

void ll_print(ll_node_ptr ptr)
{	
	printf("%16.2f",ptr->value);
	if (!ptr->after)
		{return;} //ends the print if there are no nodes left
	ll_print(ptr->after); //recursive printing 
}
