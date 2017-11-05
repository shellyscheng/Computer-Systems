/* Name: Shiying Cheng
 * The equivalent c Code for Pahse 6
 * Mjority of my notes of assembly code are in comment area, not on the seperate txt file.
 */


struct node{
  int data;
  int index;
  struct node *next;
}


struct node node1  node <0x20C, 1, &node2>;
struct node node2  node <0x3AA, 2, &node3>;
struct node node3  node <0x3CF, 3, &node4>;
struct node node4  node <0x3B2, 4, &node5>;
struct node node5  node <0x2CB, 5, &node6>;
struct node node6  node <0x28C, 6, 0>;


void phase_6( char *str )
{
  int *Num; // r12
  int j; // er13
  int k; // ebx
  struct node *now_node; // rdx
  int index_i; // eax
  long i; // rsi
  int index; // ecx
  struct node *sort_node; // rbx
  struct node **Sotr_tbl; // rax
  struct node *fd_node; // rcx
  struct node *bk_node; // rdx
  int count; // ebp
  long next_node_data; // rax
  struct node *sort_tbl[6];
  int num[6]; // decleare the array of six number

  read_six_numbers(str, num); //read the input and call read_six_number functiosn
  Num = num;
  j = 0;

  while ( 1 ) 
  {
    if ( (unsigned int)(*Num - 1) > 5 ) //corrspond to line 46, if %eax is larger than 6
      explode_bomb(); //the bomb exploded

    if ( ++j == 6 ) //j++ and then compare the new value with 6; line 51-55
      break;

    k = j; //line 61

    do // six numbers within the array are from (1, 2, 3, 4, 5, 6). All of them need to be different
    {
      if ( *Num == num[k] ) //correspond to assembly line 61 - 76
        explode_bomb();
      k++; 
    }
    while ( k <= 5 ); //corresponde to line 87, every time when k is less than 5, 
    //jump back to line 31, start the loop again

    Num++; // Line 89： add  $0x4,%r12
  }            

  i = 0LL; //line 122： mov  $0x0,%esi

  do
  {
    index = num[i]; //line 127: mov  0x30(%rsp,%rsi,1),%ecx
    index_i = 1; //line 131: mov  $0x1,%eax
    now_node = &node1; //line 136: mov  $0x6033b0,%edx

    /* The purpose of this part is to change six node into the order to 
     * the same as the users' input
     */
    if ( index > 1 ) //line 141: cmp  $0x1,%ecx; jump to line 95
    {
      do
      {
        now_node = now_node->next; //line 95: mov  0x8(%rdx),%rdx; put the next node inside the array
        ++index_i; // increment the index of the linked list by 1, line 99 add $0x1,%eax
      }
      while ( index_i != index ); //line 102  cmp  %ecx,%eax
    }

    sort_tbl[i] = now_node;
    i++; //increment i by 4 character, line 
  }
  while ( i != 6 ); //jump out of the loop, line 114: int is 4 bytes

  
  //Line 152 - 160, linked list manipulation, sorting
  sort_node = sort_tbl[0];
  Sotr_tbl = sort_tbl;
  fd_node = sort_tbl[0];

  do
  {
    //linked list, move to the next node
    bk_node = Sotr_tbl[1]; //line 163 mov  0x8(%rax),%rdx 
    fd_node->next = bk_node; //line 167 mov  %rdx,0x8(%rcx)
    Sotr_tbl++; //line 171, increment to the next node
    fd_node = bk_node; //line 175, move the value to the next node
  }
  while ( Sotr_tbl != &sort_tbl[5] );  // sort nodes in input order

  bk_node->next = 0LL; //line 183
  count = 5; //line 191

  do
  {
    next_node_data = sort_node->next->data; //line 196-200

    if ( sort_node->data < (int)next_node_data ) //line 202: cmp  %eax,(%rbx)
      explode_bomb(); //line 206

    else
    {
      sort_node = sort_node->next; //line 211 mov 0x8(%rbx),%rbx
      count--; //line 215 sub $0x1,%ebp
    }
  }
  while ( count );  // check whehter the sorted nodes are in the dec-order

  return; //Therefore, the whole assembly code is just to re-order all the nodes to ensure a decreasing order
} 







// Dump of assembler code for function phase_6:
//    0x0000000000401050 <+0>:	push   %r13
//    0x0000000000401052 <+2>:	push   %r12
//    0x0000000000401054 <+4>:	push   %rbp
//    0x0000000000401055 <+5>:	push   %rbx
//    0x0000000000401056 <+6>:	sub    $0x58,%rsp
//    0x000000000040105a <+10>:	lea    0x30(%rsp),%rsi
//    0x000000000040105f <+15>:	callq  0x4014a5 <read_six_numbers> //call the functions for 
//    0x0000000000401064 <+20>:	lea    0x30(%rsp),%r12
//    0x0000000000401069 <+25>:	mov    $0x0,%r13d
//    0x000000000040106f <+31>:	mov    %r12,%rbp
//    0x0000000000401072 <+34>:	mov    (%r12),%eax
//    0x0000000000401076 <+38>:	sub    $0x1,%eax
//    0x0000000000401079 <+41>:	cmp    $0x5,%eax
//    0x000000000040107c <+44>:	jbe    0x401083 <phase_6+51>
//    0x000000000040107e <+46>:	callq  0x40146f <explode_bomb>
//    0x0000000000401083 <+51>:	add    $0x1,%r13d
//    0x0000000000401087 <+55>:	cmp    $0x6,%r13d
//    0x000000000040108b <+59>:	je     0x4010ca <phase_6+122>
//    0x000000000040108d <+61>:	mov    %r13d,%ebx
//    0x0000000000401090 <+64>:	movslq %ebx,%rax
//    0x0000000000401093 <+67>:	mov    0x30(%rsp,%rax,4),%eax
//    0x0000000000401097 <+71>:	cmp    %eax,0x0(%rbp)
//    0x000000000040109a <+74>:	jne    0x4010a1 <phase_6+81>
//    0x000000000040109c <+76>:	callq  0x40146f <explode_bomb>
//    0x00000000004010a1 <+81>:	add    $0x1,%ebx
//    0x00000000004010a4 <+84>:	cmp    $0x5,%ebx
//    0x00000000004010a7 <+87>:	jle    0x401090 <phase_6+64>
//    0x00000000004010a9 <+89>:	add    $0x4,%r12
//    0x00000000004010ad <+93>:	jmp    0x40106f <phase_6+31>
//    0x00000000004010af <+95>:	mov    0x8(%rdx),%rdx
//    0x00000000004010b3 <+99>:	add    $0x1,%eax
//    0x00000000004010b6 <+102>:	cmp    %ecx,%eax
//    0x00000000004010b8 <+104>:	jne    0x4010af <phase_6+95>
//    0x00000000004010ba <+106>:	mov    %rdx,(%rsp,%rsi,2)
//    0x00000000004010be <+110>:	add    $0x4,%rsi
//    0x00000000004010c2 <+114>:	cmp    $0x18,%rsi
//    0x00000000004010c6 <+118>:	jne    0x4010cf <phase_6+127>
//    0x00000000004010c8 <+120>:	jmp    0x4010e4 <phase_6+148>
//    0x00000000004010ca <+122>:	mov    $0x0,%esi
//    0x00000000004010cf <+127>:	mov    0x30(%rsp,%rsi,1),%ecx
//    0x00000000004010d3 <+131>:	mov    $0x1,%eax
//    0x00000000004010d8 <+136>:	mov    $0x6033b0,%edx
//    0x00000000004010dd <+141>:	cmp    $0x1,%ecx
//    0x00000000004010e0 <+144>:	jg     0x4010af <phase_6+95>
//    0x00000000004010e2 <+146>:	jmp    0x4010ba <phase_6+106>
//    0x00000000004010e4 <+148>:	mov    (%rsp),%rbx
//    0x00000000004010e8 <+152>:	mov    %rsp,%rax
//    0x00000000004010eb <+155>:	lea    0x28(%rsp),%rsi
//    0x00000000004010f0 <+160>:	mov    %rbx,%rcx
//    0x00000000004010f3 <+163>:	mov    0x8(%rax),%rdx
//    0x00000000004010f7 <+167>:	mov    %rdx,0x8(%rcx)
//    0x00000000004010fb <+171>:	add    $0x8,%rax
//    0x00000000004010ff <+175>:	mov    %rdx,%rcx
//    0x0000000000401102 <+178>:	cmp    %rsi,%rax
//    0x0000000000401105 <+181>:	jne    0x4010f3 <phase_6+163>
//    0x0000000000401107 <+183>:	movq   $0x0,0x8(%rdx)
//    0x000000000040110f <+191>:	mov    $0x5,%ebp
//    0x0000000000401114 <+196>:	mov    0x8(%rbx),%rax
//    0x0000000000401118 <+200>:	mov    (%rax),%eax
//    0x000000000040111a <+202>:	cmp    %eax,(%rbx)
//    0x000000000040111c <+204>:	jge    0x401123 <phase_6+211>
//    0x000000000040111e <+206>:	callq  0x40146f <explode_bomb>
//    0x0000000000401123 <+211>:	mov    0x8(%rbx),%rbx
//    0x0000000000401127 <+215>:	sub    $0x1,%ebp
//    0x000000000040112a <+218>:	jne    0x401114 <phase_6+196>
//    0x000000000040112c <+220>:	add    $0x58,%rsp
//    0x0000000000401130 <+224>:	pop    %rbx
//    0x0000000000401131 <+225>:	pop    %rbp
//    0x0000000000401132 <+226>:	pop    %r12
//    0x0000000000401134 <+228>:	pop    %r13
//    0x0000000000401136 <+230>:	retq   






// Dump of assembler code for function read_six_numbers:
//    0x00000000004014a5 <+0>:	sub    $0x8,%rsp
//    0x00000000004014a9 <+4>:	mov    %rsi,%rdx
//    0x00000000004014ac <+7>:	lea    0x4(%rsi),%rcx
//    0x00000000004014b0 <+11>:	lea    0x14(%rsi),%rax
//    0x00000000004014b4 <+15>:	push   %rax
//    0x00000000004014b5 <+16>:	lea    0x10(%rsi),%rax
//    0x00000000004014b9 <+20>:	push   %rax
//    0x00000000004014ba <+21>:	lea    0xc(%rsi),%r9
//    0x00000000004014be <+25>:	lea    0x8(%rsi),%r8
//    0x00000000004014c2 <+29>:	mov    $0x4026c9,%esi
//    0x00000000004014c7 <+34>:	mov    $0x0,%eax
//    0x00000000004014cc <+39>:	callq  0x400bb0 <__isoc99_sscanf@plt>
//    0x00000000004014d1 <+44>:	add    $0x10,%rsp
//    0x00000000004014d5 <+48>:	cmp    $0x5,%eax
//    0x00000000004014d8 <+51>:	jg     0x4014df <read_six_numbers+58>
//    0x00000000004014da <+53>:	callq  0x40146f <explode_bomb>
//    0x00000000004014df <+58>:	add    $0x8,%rsp
//    0x00000000004014e3 <+62>:	retq   



// (gdb) x/30xd 0x6033b0
// 0x6033b0 <node1>:	524	1	6304704	0
// 0x6033c0 <node2>:	938	2	6304720	0
// 0x6033d0 <node3>:	975	3	6304736	0
// 0x6033e0 <node4>:	946	4	6304752	0
// 0x6033f0 <node5>:	715	5	6304768	0
// 0x603400 <node6>:	652	6	0	0


// decreasing order:

// 975>946>938>715>652>524
// 3 4 2 5 6 1


// Therefore, the input strain is 3 4 2 5 6 1
