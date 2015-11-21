#ifndef __WHALE_VECTOR_H__
#define __WHALE_VECTOR_H__

#include <whale/WhaleCore.h>

/* Example implementation

   Use the internal WhaleVersion of DOLFIN, to demonstrate how to implement
   the Whale library. In a real implementation, instead of using directly this
   internal header, one may implement a Vector structure in a separate header
   and use that in to develop this library. In this case, the internal header
   must be updated to use the aforementioned Vector structure.
 */
#include <dolfin/la/WhaleVector.h>

namespace whale {

    void WhalePrintVector(const dolfin::WhaleVector&);

}

#endif
