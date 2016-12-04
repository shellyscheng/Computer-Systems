//Shiying Cheng, Nikihil Kovelamudi

#include "Linked_List.h"
#include <stdio.h>
int main(int argc, char **argv)
{
	//create 1st list of length 8 with reverse function
	ll_node_ptr t1 = NULL;
	t1 = ll_add_front(t1, 4.3);
	t1 = ll_add_front(t1, 1.0);
	t1 = ll_add_front(t1, 10.9);
	t1 = ll_add_front(t1, 1059.3);
	t1 = ll_add_front(t1, 9000.1);
	t1 = ll_add_front(t1, 1337);
	t1 = ll_add_front(t1, 57.4);
	t1 = ll_add_front(t1, 420.2);
	t1 = ll_rev(t1);
	ll_print(t1);
	ll_free(t1);

    //create 2nd list of length 5 with reverse fucnction
	ll_node_ptr t2 = NULL;
	t2 = ll_add_front(t2, 4.3);
	t2 = ll_add_front(t2, 1.0);
	t2 = ll_add_front(t2, 10.9);
	t2 = ll_add_front(t2, 1059.3);
	t2 = ll_add_front(t2, 9000.1);
	t2 = ll_rev(t2);
	printf("\n");
	ll_print(t2);
	ll_free(t2);
	
	//create 3rd list of length 5 without reverse functon
	ll_node_ptr t3 = NULL;
	t3 = ll_add_front(t3, 4.3);
	t3 = ll_add_front(t3, 1.0);
	t3 = ll_add_front(t3, 10.9);
	t3 = ll_add_front(t3, 1059.3);
	t3 = ll_add_front(t3, 9000.1);
	printf("\n");
	ll_print(t3);
	ll_free(t3);
	return 0;
}

