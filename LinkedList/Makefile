CFLAGS += -g

clean:
	rm *.o
	rm test

test: Linked_List.o Linked_List.c
	gcc $(CFLAGS) -o test Linked_List_test.c Linked_List.o
