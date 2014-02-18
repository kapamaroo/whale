#if !defined(_WHALESFTYPES_H)
#define _WHALESFTYPES_H

/*S
   WhaleSF - Whale object for setting up and managing the communication of certain entries of arrays and Vecs between MPI processes.

   Level: intermediate

  Concepts: star forest

       WhaleSF uses the concept of star forests to indicate and determine the communication patterns concisely and efficiently.
  A star  http://en.wikipedia.org/wiki/Star_(graph_theory) forest is simply a collection of trees of height 1. The leave nodes represent
  "ghost locations" for the root nodes.

.seealso: WhaleSFCreate(), VecScatter, VecScatterCreate()
S*/
typedef struct _p_WhaleSF* WhaleSF;

#endif
