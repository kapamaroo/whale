#ifndef __WHALEVEC_H__
#define __WHALEVEC_H__

#include <whalesys.h>

struct _whale_vector {
    void *data;
    size_t size;
};

typedef struct _whale_vector whale_vector;

WHALE_EXTERN whale_error whale_vector_destroy(whale_vector *);

#endif
