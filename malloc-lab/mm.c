/*
 * mm - Segregated Fits, explicited linked list: My optimization
 * 
 * Name: Shying Cheng
 *
 * Sort free blocks into different lists according to its size
 * Each block has a 4-byte header and 4-byte footer
 * nth list: contains blocks (size between 2^n - 2^(n+1)-1 )
 * minima block size is 16
 * number of segregated lists is 20
 * lists are also sorted by memory addreses (ascending order)
 *
 *
 * Simple allocator based on implicit free lists,
 * first fit placement, and boundary tag coalescing.
 *
 * 
 * Each block has header and footer of the form:
 *
 *      31                     3  2  1  0
 *      -----------------------------------
 *     | s  s  s  s  ... s  s  s  0  0  a/f
 *      -----------------------------------
 *
 * where s are the meaningful size bits and a/f is set
 * iff the block is allocated. The list has the following form:
 *
 * begin                                                          end
 * heap                                                           heap
 *  -----------------------------------------------------------------
 * |  pad   | hdr(8:a) | ftr(8:a) | zero or more usr blks | hdr(8:a) |
 *  -----------------------------------------------------------------
 *          |       prologue      |                       | epilogue |
 *          |         block       |                       | block    |
 *
 * The allocated prologue and epilogue blocks are overhead that
 * eliminate edge conditions during coalescing.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "mm.h"
#include "memlib.h"

/* Team structure */
team_t team = {
    /* Team name */
    "Hello!",
    /* First member's full name */
    "Shiying",
    /* First member's email address */
    "shiyng.cheng@coloradocollege.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* Basic constants and macros */
#define WSIZE       4       /* word size (bytes) */
#define DSIZE       8       /* doubleword size (bytes) */
#define CHUNKSIZE  (1<<12)  /* initial heap size (bytes) */
#define MINSIZE     16      /* Minimum block size */

#define LISTS       20      /* Number of segragated lists */
#define BUFFER      (1<<7)  /* Buffer for  rellocation */

#define MAX(x, y) ((x) > (y)? (x) : (y)) /* Maximum of two numbers */
#define MIN(x, y) ((x) < (y)? (x) : (y)) /* Minimum of two numbers */

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val) | GET_TAG(p))
#define PUT_NOTAG(p, val) (*(unsigned int *)(p) = (val))

#define PUT_bp(p, bp) (*(unsigned int *)(p) = (unsigned int)(bp))

/* Adjust the tag for rellocation */
#define SET_TAG(p) (*(unsigned int *)(p) = GET(p) | 0x2)
#define UNSET_TAG(p) (*(unsigned int *)(p) = GET(p) & ~0x2)

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_TAG(p)   (GET(p) & 0x2)

/* Given block pointer bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block bp bp, compute address of next and previous blocks */
#define NEXT(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

#define PRED_bp(bp) ((char *)(bp))
#define SUCC_bp(bp) ((char *)(bp) + WSIZE)

/* Addresses of the previous and successor block of a free block */
#define PRED(bp) (*(char **)(bp))
#define SUCC(bp) (*(char **)(SUCC_bp(bp)))

/* rounds up to the nearest multiple */
#define ALIGN(P) (((size_t)(p) + 7) & ~0x7)


/* Global variables */
char *prologue_block;  /* pointer to first block */
void *free_lists[LISTS]; /* array of pointers to segregated free lists */


/* function prototypes for internal helper routines */
static void *extend_heap(size_t size);
static void *coalesce(void *bp);
static void place(void *bp, size_t asize);
static void insert_node(void *bp, size_t size);
static void delete_node(void *bp);


/*
 * mm_init - Initialize the memory manager
 */
int mm_init(void)
{
  /* create the initial empty heap
   * the structure is similar to textbook P860
   */
  char *heap_start; //pointer to the beginning of heap
  int i; //list counter

  /* Initialize the pointer array for the segregated lists */
  for (i = 0; i < LISTS; i++) {
    free_lists[i] = NULL;
  }

  /* Allocate memory */
  if ((long)(heap_start = mem_sbrk(4 * WSIZE)) == -1)
    return -1;

  PUT_NOTAG(heap_start, 0);                               /* alignment padding */
  PUT_NOTAG(heap_start + (1 * WSIZE), PACK(DSIZE, 1));    /* header of the prologue block */
  PUT_NOTAG(heap_start + (2 * WSIZE), PACK(DSIZE, 1));    /* footer of the prologue block */
  PUT_NOTAG(heap_start + (3 * WSIZE), PACK(0, 1));        /* last block */
  prologue_block = heap_start + DSIZE;

  /* Extend the empty heap with a free block of CHUNKSIZE bytes */
  if( extend_heap(CHUNKSIZE) == NULL )
    return -1;

  return 0;
}


/*
 * mm_malloc - Allocate a block with at least size bytes of payload
 *             Blocks has boundary tags
 */
void *mm_malloc(size_t size)
{
  size_t asize;      /* adjusted block size */
  size_t extendsize; /* amount to extend heap if no fit */
  void *bp = NULL;   /* pointer */
  int i = 0;         /* counter */

  /* Ignore spurious requests */
  if( size == 0 )
    return NULL;

  /* Adjust block size to include overhead and alignment reqs. */
  if( size <= DSIZE )
    asize = 2 * DSIZE;
  else
    asize = DSIZE * ( ( size + (DSIZE) + ( DSIZE - 1 ) ) / DSIZE );

  /* Search the free list for a fit */
  size = asize;
  while ( i < LISTS ) {
    if ((i == LISTS - 1) || ((size <= 1) && (free_lists[i] != NULL))) {
      bp = free_lists[i];

      while ((bp != NULL) && ((asize > GET_SIZE(HDRP(bp))) || (GET_TAG(HDRP(bp)))))
        bp = PRED(bp);

      if (bp != NULL )
        break;
    }

    size >>= 1;
    i++;
  }

  /* No fit found. Get more memory and place the block */
  if (bp == NULL) {
    extendsize = MAX( asize, CHUNKSIZE );
    if( ( bp = extend_heap( extendsize ) ) == NULL )
      return NULL;
  }
  place( bp, asize ); //place the block

  return bp;
    
}

/*
 * mm_free - Free a block by adding or coalescing
 */
void mm_free(void *bp)
{
  size_t size = GET_SIZE( HDRP( bp ) ); /* Size of each block */

  /* Unset the rellocation tag for the next block */
  UNSET_TAG(HDRP(NEXT(bp)));

  /* Adjust the boundary tags */
  PUT( HDRP( bp ), PACK( size, 0 ) );
  PUT( FTRP( bp ), PACK( size, 0 ) );

  /* insert new block into the right list */
  insert_node(bp, size);

  coalesce( bp );

  return;
}

/*
 * mm_realloc - use the combination between mallloc and free
 *              new block is padded with bufer, ensuring the next reallocation is successful
 */
void *mm_realloc(void *bp, size_t size)
{
  void *newp = bp;          /* Pointer to be returned */
  size_t new_size = size;    /* Size of a new block */
  int remainder;             /* blcok size */
  int extendsize;            /* extend the heap if neccessary */
  int block_buffer;          /* block buffer */

  /* Ignore spurious requests */
  if( size == 0 )
    return NULL;

  /* Adjust block size to include overhead and alignment reqs. */
  if( new_size <= DSIZE )
      new_size = 2 * DSIZE;
  else
    new_size = DSIZE * ( ( new_size + (DSIZE) + ( DSIZE - 1 ) ) / DSIZE );

  new_size += BUFFER; //block buffer


  block_buffer = GET_SIZE(HDRP(bp)) - new_size;

  /* allocate more space if overhead is too small */
  if ( block_buffer < 0 ) {
    if (!GET_ALLOC(HDRP(NEXT(bp))) || !GET_SIZE(HDRP(NEXT(bp)))) {
      remainder = GET_SIZE(HDRP(bp)) + GET_SIZE(HDRP(NEXT(bp))) - new_size;

      if ( remainder < 0 ) {
        extendsize = MAX(-remainder, CHUNKSIZE);

        if(extend_heap(extendsize) == NULL)
          return NULL;

        remainder += extendsize;
      }

      delete_node(NEXT(bp));

      /* Not split the block */
      PUT_NOTAG(HDRP(bp), PACK(new_size + remainder, 1)); /* Block header */
      PUT_NOTAG(FTRP(bp), PACK(new_size + remainder, 1)); /* Block footer */
    }

    else {
      newp = mm_malloc(new_size - DSIZE);
      memmove(newp, bp, MIN(size, new_size)); //Counter--
      mm_free(bp); //counter--
    }

    block_buffer = GET_SIZE(HDRP(newp)) - new_size;
  }


  /* If the block's overhead is smaller than twice of the overhead */
  if ( block_buffer < 2 * BUFFER)
    SET_TAG(HDRP(NEXT(newp)));

    return newp;
}


/* The remaining routines are internal helper routines */

/*
 * extend_heap - Extend heap with free block and return its block pointer
 */
static void *extend_heap( size_t size )
{
  void *bp;
  size_t words = size / WSIZE; //size of extension in words */
  size_t asize;

  /* Allocate an even number of words to maintain alignment */
  asize = ( words % 2 ) ? ( words + 1 ) * WSIZE : words * WSIZE;

  /* Extend the heap */
  if( (long)( bp = mem_sbrk( asize ) ) == -1 )
    return NULL;

  /* Initialize free block header/footer and the epilogue header */
  PUT_NOTAG( HDRP( bp ), PACK( asize, 0 ) );         /* free block header */
  PUT_NOTAG( FTRP( bp ), PACK( asize, 0 ) );         /* free block footer */
  PUT_NOTAG( HDRP( NEXT( bp ) ), PACK( 0, 1 ) ); /* new epilogue header */


  /* Insert new block to the list */
  insert_node(bp, asize);

  /* Coalesce if the previous block was free */
  return coalesce( bp );
}

/*
 * place - Place block of asize bytes at start of free block bp
 *         and split if remainder would be at least minimum block size
 */
static void place(void *bp, size_t asize)
{
  size_t bp_size = GET_SIZE(HDRP(bp));
  size_t remainder = bp_size - asize;
  
  /* Remove block from list */
  delete_node(bp);
  
  if ( remainder >= MINSIZE ) {
    /* Split block */
    PUT( HDRP( bp ), PACK( asize, 1 ) ); /* Block HDRPer */
    PUT( FTRP( bp ), PACK( asize, 1 ) ); /* Block FTRPer */
    PUT_NOTAG( HDRP( NEXT( bp ) ), PACK( remainder, 0 ) ); /* Next HDRPer */
    PUT_NOTAG( FTRP( NEXT( bp ) ), PACK( remainder, 0 ) ); /* Next FTRPer */  
    insert_node( NEXT( bp ), remainder );
  } 
  else {
    /* Do not split block */
    PUT( HDRP( bp ), PACK( bp_size, 1 ) ); /* Block HDRPer */
    PUT( FTRP( bp ), PACK( bp_size, 1 ) ); /* Block FTRPer */
  }
  return;
}

/*
 * coalesce - boundary tag coalescing. Return bp to coalesced block
 */
static void *coalesce(void *bp)
{
  size_t prev_alloc = GET_ALLOC( HDRP( PREV( bp ) ) );
  size_t next_alloc = GET_ALLOC( HDRP( NEXT( bp ) ) );
  size_t size = GET_SIZE( HDRP( bp ) );

  /* Case 1: when previous and next blocks are both allocated*/
  if( prev_alloc && next_alloc ) {            
    return bp;
  }

  /* not coalesce with the previous block when it is tagged */
  if (GET_TAG( HDRP(PREV(bp))))
    prev_alloc = 1;

  delete_node(bp); //remove the old block

  /* Case 2: if the front block is allocated, but not the next one */
  if( prev_alloc && !next_alloc ) {      
    delete_node(NEXT( bp ));
    size += GET_SIZE( HDRP( NEXT( bp ) ) );
    PUT( HDRP( bp ), PACK( size, 0 ) );
    PUT( FTRP( bp ), PACK( size, 0 ) );
  }

  /* Case 3: if the previous block is not allocated, but the next is */
  else if( !prev_alloc && next_alloc ) {
    delete_node(PREV( bp ));      
    size += GET_SIZE( HDRP( PREV( bp ) ) );
    PUT( FTRP( bp ), PACK( size, 0 ) );
    PUT( HDRP( PREV( bp ) ), PACK( size, 0 ) );
    bp = PREV( bp );
  }

  /* Case 4: both the previous and the next blocks are not allocated */   
  else {                                   
    delete_node(NEXT(bp));
    delete_node(PREV(bp));
    size += GET_SIZE( HDRP( PREV( bp ) ) ) + GET_SIZE( FTRP( NEXT( bp ) ) );
    PUT( HDRP( PREV( bp ) ), PACK( size, 0 ) );
    PUT( FTRP( NEXT( bp ) ), PACK( size, 0 ) );
    bp = PREV( bp );
  }

  insert_node( bp, size );
  return bp;
}

/*
 * insert_node - insert a block pinter to a segregate lists
 */

static void insert_node(void *bp, size_t size)
{
  int i = 0;
  void *search_bp = bp;
  void *insert_bp = NULL;

  while ((i < LISTS -1) && (size > 1)) {
    size >>= 1;
    i++;
  }

  /* keep the list in order */
  search_bp = free_lists[i];

  while ((search_bp != NULL) && (size > GET_SIZE(HDRP(search_bp)))) {
    insert_bp = search_bp;
    search_bp = PRED(search_bp);
  }


  /* set the predecessor & successor of the block */
  if (search_bp != NULL) {
    if (insert_bp != NULL) {
      PUT_bp(PRED_bp(bp), search_bp);
      PUT_bp(SUCC_bp(search_bp), bp);
      PUT_bp(SUCC_bp(bp), insert_bp);
      PUT_bp(PRED_bp(insert_bp), bp);
    } 
    else {
      PUT_bp(PRED_bp(bp), search_bp); 
      PUT_bp(SUCC_bp(search_bp), bp);
      PUT_bp(SUCC_bp(bp), NULL);

      /* Add block to the list if needed */
      free_lists[i] = bp;
    }
  }
  else {
    if (insert_bp != NULL) {
      PUT_bp(PRED_bp(bp), NULL);
      PUT_bp(SUCC_bp(bp), insert_bp);
      PUT_bp(PRED_bp(insert_bp), bp);
    } 
    else {
      PUT_bp(PRED_bp(bp), NULL);
      PUT_bp(SUCC_bp(bp), NULL);
      
      /* Add block to the list if needed */
      free_lists[i] = bp;
    }
  }

  return;
  }

/* 
if (GET_bp(heap_start + WSIZE * index) == NULL) {
        PUT_bp(heap_start + WSIZE * index, bp);
        PUT_bp(bp, NULL);
        PUT_bp((unsigned int *)bp + 1, NULL);
    } 
    else {
    PUT_bp(bp, GET_bp(heap_start + WSIZE * index));
    PUT_bp(GET_bp(heap_start + WSIZE * index) + 1, bp); 
    PUT_bp((unsigned int *)bp + 1, NULL);
    PUT_bp(heap_start + WSIZE * index, bp);
*/

/* 
 * delete_node - delete the node inside the linkedlist
 */
static void delete_node(void *bp)
{
  int i = 0;
  size_t size = GET_SIZE(HDRP(bp));

  /* Segregated list selection */
  while (( i < LISTS - 1) && (size > 1)) {
    size >>= 1;
    i++;
  }

  if (PRED(bp) != NULL) {
    if (SUCC(bp) != NULL) {
      PUT_bp(SUCC_bp(PRED(bp)), SUCC(bp));
      PUT_bp(PRED_bp(SUCC(bp)), PRED(bp));
    }
    else {
      PUT_bp(SUCC_bp(PRED(bp)), NULL);
      free_lists[i] = PRED(bp);
    }
  }
  else {
    if (SUCC(bp) != NULL )
      PUT_bp(PRED_bp(SUCC(bp)), NULL);
    else {
      free_lists[i] = NULL;
    }
  }

  return;

}
