/*
   This is the main Whale include file (for C and C++).  It is included by all
   other Whale include files, so it almost never has to be specifically included.
*/
#if !defined(__WHALESYS_H)
#define __WHALESYS_H
/* ========================================================================== */
/*
   whaleconf.h is contained in ${WHALE_ARCH}/include/whaleconf.h it is
   found automatically by the compiler due to the -I${WHALE_DIR}/${WHALE_ARCH}/include
   in the conf/variables definition of WHALE_INCLUDE. For --prefix installs the ${WHALE_ARCH}/
   does not exist and whaleconf.h is in the same directory as the other Whale include files.
*/
#include <whaleconf.h>
#include <whalefix.h>

#if defined(WHALE_DESIRE_FEATURE_TEST_MACROS)
/*
   Feature test macros must be included before headers defined by IEEE Std 1003.1-2001
   We only turn these in Whale source files that require them by setting WHALE_DESIRE_FEATURE_TEST_MACROS
*/
#if defined(WHALE__POSIX_C_SOURCE_200112L) && !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE 200112L
#endif
#if defined(WHALE__BSD_SOURCE) && !defined(_BSD_SOURCE)
#define _BSD_SOURCE
#endif
#if defined(WHALE__GNU_SOURCE) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#endif

/* ========================================================================== */
/*
   This facilitates using the C version of Whale from C++ and the C++ version from C.
*/
#if defined(__cplusplus)
#  define WHALE_FUNCTION_NAME WHALE_FUNCTION_NAME_CXX
#else
#  define WHALE_FUNCTION_NAME WHALE_FUNCTION_NAME_C
#endif

#if defined(_WIN32) && defined(WHALE_USE_SHARED_LIBRARIES) /* For Win32 shared libraries */
#  define WHALE_DLLEXPORT __declspec(dllexport)
#  define WHALE_DLLIMPORT __declspec(dllimport)
#  define WHALE_VISIBILITY_INTERNAL
#elif defined(WHALE_USE_VISIBILITY)
#  define WHALE_DLLEXPORT __attribute__((visibility ("default")))
#  define WHALE_DLLIMPORT __attribute__((visibility ("default")))
#  define WHALE_VISIBILITY_INTERNAL __attribute__((visibility ("hidden")))
#else
#  define WHALE_DLLEXPORT
#  define WHALE_DLLIMPORT
#  define WHALE_VISIBILITY_INTERNAL
#endif

#if defined(whale_EXPORTS)      /* CMake defines this when building the shared library */
#  define WHALE_VISIBILITY_PUBLIC WHALE_DLLEXPORT
#else  /* Win32 users need this to import symbols from whale.dll */
#  define WHALE_VISIBILITY_PUBLIC WHALE_DLLIMPORT
#endif

#if defined(__cplusplus)
#define WHALE_EXTERN extern "C" WHALE_VISIBILITY_PUBLIC
#define WHALE_EXTERN_TYPEDEF extern "C"
#define WHALE_INTERN extern "C" WHALE_VISIBILITY_INTERNAL
#else
#define WHALE_EXTERN extern WHALE_VISIBILITY_PUBLIC
#define WHALE_EXTERN_TYPEDEF
#define WHALE_INTERN extern WHALE_VISIBILITY_INTERNAL
#endif

#include <whaleversion.h>
#define WHALE_AUTHOR_INFO  "       The Whale Team\n    whale-maint@mcs.anl.gov\n http://www.mcs.anl.gov/whale/\n"

/* ========================================================================== */

/*
    Defines the interface to MPI allowing the use of all MPI functions.

    Whale does not use the C++ binding of MPI at ALL. The following flag
    makes sure the C++ bindings are not included. The C++ bindings REQUIRE
    putting mpi.h before ANY C++ include files, we cannot control this
    with all Whale users. Users who want to use the MPI C++ bindings can include
    mpicxx.h directly in their code
*/
#if !defined(MPICH_SKIP_MPICXX)
#  define MPICH_SKIP_MPICXX 1
#endif
#if !defined(OMPI_SKIP_MPICXX)
#  define OMPI_SKIP_MPICXX 1
#endif
#include <mpi.h>

/*
    Need to put stdio.h AFTER mpi.h for MPICH2 with C++ compiler
    see the top of mpicxx.h in the MPICH2 distribution.
*/
#include <stdio.h>

/* MSMPI on 32bit windows requires this yukky hack - that breaks MPI standard compliance */
#if !defined(MPIAPI)
#define MPIAPI
#endif

/* Support for Clang (>=3.2) matching type tag arguments with void* buffer types */
#if defined(__has_attribute)
#  if __has_attribute(argument_with_type_tag) && __has_attribute(pointer_with_type_tag) && __has_attribute(type_tag_for_datatype)
#    define WhaleAttrMPIPointerWithType(bufno,typeno) __attribute__((pointer_with_type_tag(MPI,bufno,typeno)))
#    define WhaleAttrMPITypeTag(type)                 __attribute__((type_tag_for_datatype(MPI,type)))
#    define WhaleAttrMPITypeTagLayoutCompatible(type) __attribute__((type_tag_for_datatype(MPI,type,layout_compatible)))
#  endif
#endif
#if !defined(WhaleAttrMPIPointerWithType)
#  define WhaleAttrMPIPointerWithType(bufno,typeno)
#  define WhaleAttrMPITypeTag(type)
#  define WhaleAttrMPITypeTagLayoutCompatible(type)
#endif

/*MC
    WhaleErrorCode - datatype used for return error code from all Whale functions

    Level: beginner

.seealso: CHKERRQ, SETERRQ
M*/
typedef int WhaleErrorCode;

/*MC

    WhaleClassId - A unique id used to identify each Whale class.
         (internal integer in the data structure used for error
         checking). These are all computed by an offset from the lowest
         one, WHALE_SMALLEST_CLASSID.

    Level: advanced

.seealso: WhaleClassIdRegister(), WhaleLogEventRegister(), WhaleHeaderCreate()
M*/
typedef int WhaleClassId;


/*MC
    WhaleMPIInt - datatype used to represent 'int' parameters to MPI functions.

    Level: intermediate

    Notes: usually this is the same as WhaleInt, but if Whale was built with --with-64-bit-indices but
           standard C/Fortran integers are 32 bit then this is NOT the same as WhaleInt it remains 32 bit

    WhaleMPIIntCast(a,&b) checks if the given WhaleInt a will fit in a WhaleMPIInt, if not it
      generates a WHALE_ERR_ARG_OUTOFRANGE error.

.seealso: WhaleBLASInt, WhaleInt

M*/
typedef int WhaleMPIInt;

/*MC
    WhaleEnum - datatype used to pass enum types within Whale functions.

    Level: intermediate

.seealso: WhaleOptionsGetEnum(), WhaleOptionsEnum(), WhaleBagRegisterEnum()
M*/
typedef enum { ENUM_DUMMY } WhaleEnum;
WHALE_EXTERN MPI_Datatype MPIU_ENUM WhaleAttrMPITypeTag(WhaleEnum);

/*MC
    WhaleInt - Whale type that represents integer - used primarily to
      represent size of arrays and indexing into arrays. Its size can be configured with the option
      --with-64-bit-indices - to be either 32bit or 64bit [default 32 bit ints]

   Level: intermediate

.seealso: WhaleScalar, WhaleBLASInt, WhaleMPIInt
M*/
#if (WHALE_SIZEOF_LONG_LONG == 8)
typedef long long Whale64bitInt;
#elif defined(WHALE_HAVE___INT64)
typedef __int64 Whale64bitInt;
#else
typedef unknown64bit Whale64bitInt
#endif
#if defined(WHALE_USE_64BIT_INDICES)
typedef Whale64bitInt WhaleInt;
#define MPIU_INT MPI_LONG_LONG_INT
#else
typedef int WhaleInt;
#define MPIU_INT MPI_INT
#endif


/*MC
    WhaleBLASInt - datatype used to represent 'int' parameters to BLAS/LAPACK functions.

    Level: intermediate

    Notes: usually this is the same as WhaleInt, but if Whale was built with --with-64-bit-indices but
           standard C/Fortran integers are 32 bit then this is NOT the same as WhaleInt it remains 32 bit
           (except on very rare BLAS/LAPACK implementations that support 64 bit integers see the note below).

    WhaleErrorCode WhaleBLASIntCast(a,&b) checks if the given WhaleInt a will fit in a WhaleBLASInt, if not it
      generates a WHALE_ERR_ARG_OUTOFRANGE error

    Installation Notes: The 64bit versions of MATLAB ship with BLAS and LAPACK that use 64 bit integers for sizes etc,
     if you run ./configure with the option
     --with-blas-lapack-lib=[/Applications/MATLAB_R2010b.app/bin/maci64/libmwblas.dylib,/Applications/MATLAB_R2010b.app/bin/maci64/libmwlapack.dylib]
     but you need to also use --known-64-bit-blas-indices.

        MKL also ships with 64 bit integer versions of the BLAS and LAPACK, if you select those you must also ./configure with --known-64-bit-blas-indices

     Developer Notes: Eventually ./configure should automatically determine the size of the integers used by BLAS/LAPACK.

     External packages such as hypre, ML, SuperLU etc do not provide any support for passing 64 bit integers to BLAS/LAPACK so cannot
     be used with Whale if you have set WhaleBLASInt to long int.

.seealso: WhaleMPIInt, WhaleInt

M*/
#if defined(WHALE_HAVE_64BIT_BLAS_INDICES)
typedef Whale64bitInt WhaleBLASInt;
#else
typedef int WhaleBLASInt;
#endif

/*EC

    WhalePrecision - indicates what precision the object is using. This is currently not used.

    Level: advanced

.seealso: WhaleObjectSetPrecision()
E*/
typedef enum { WHALE_PRECISION_SINGLE=4,WHALE_PRECISION_DOUBLE=8 } WhalePrecision;
WHALE_EXTERN const char *WhalePrecisions[];

/*
    For the rare cases when one needs to send a size_t object with MPI
*/
#if (WHALE_SIZEOF_SIZE_T) == (WHALE_SIZEOF_INT)
#define MPIU_SIZE_T MPI_UNSIGNED
#elif  (WHALE_SIZEOF_SIZE_T) == (WHALE_SIZEOF_LONG)
#define MPIU_SIZE_T MPI_UNSIGNED_LONG
#elif  (WHALE_SIZEOF_SIZE_T) == (WHALE_SIZEOF_LONG_LONG)
#define MPIU_SIZE_T MPI_UNSIGNED_LONG_LONG
#else
#error "Unknown size for size_t! Send us a bugreport at whale-maint@mcs.anl.gov"
#endif


/*
      You can use WHALE_STDOUT as a replacement of stdout. You can also change
    the value of WHALE_STDOUT to redirect all standard output elsewhere
*/
WHALE_EXTERN FILE* WHALE_STDOUT;

/*
      You can use WHALE_STDERR as a replacement of stderr. You can also change
    the value of WHALE_STDERR to redirect all standard error elsewhere
*/
WHALE_EXTERN FILE* WHALE_STDERR;

/*MC
    WhaleUnlikely - hints the compiler that the given condition is usually FALSE

    Synopsis:
    #include "whalesys.h"
    WhaleBool  WhaleUnlikely(WhaleBool  cond)

    Not Collective

    Input Parameters:
.   cond - condition or expression

    Note: This returns the same truth value, it is only a hint to compilers that the resulting
    branch is unlikely.

    Level: advanced

.seealso: WhaleLikely(), CHKERRQ
M*/

/*MC
    WhaleLikely - hints the compiler that the given condition is usually TRUE

    Synopsis:
    #include "whalesys.h"
    WhaleBool  WhaleUnlikely(WhaleBool  cond)

    Not Collective

    Input Parameters:
.   cond - condition or expression

    Note: This returns the same truth value, it is only a hint to compilers that the resulting
    branch is likely.

    Level: advanced

.seealso: WhaleUnlikely()
M*/
#if defined(WHALE_HAVE_BUILTIN_EXPECT)
#  define WhaleUnlikely(cond)   __builtin_expect(!!(cond),0)
#  define WhaleLikely(cond)     __builtin_expect(!!(cond),1)
#else
#  define WhaleUnlikely(cond)   (cond)
#  define WhaleLikely(cond)     (cond)
#endif

/*
    Defines some elementary mathematics functions and constants.
*/
#include <whalemath.h>

/*
    Declare extern C stuff after including external header files
*/


/*
       Basic Whale constants
*/

/*E
    WhaleBool  - Logical variable. Actually an int in C and a logical in Fortran.

   Level: beginner

   Developer Note: Why have WhaleBool , why not use bool in C? The problem is that K and R C, C99 and C++ all have different mechanisms for
      boolean values. It is not easy to have a simple macro that that will work properly in all circumstances with all three mechanisms.

E*/
typedef enum { WHALE_FALSE,WHALE_TRUE } WhaleBool;
WHALE_EXTERN const char *const WhaleBools[];
WHALE_EXTERN MPI_Datatype MPIU_BOOL WhaleAttrMPITypeTag(WhaleBool);

/*E
    WhaleCopyMode  - Determines how an array passed to certain functions is copied or retained

   Level: beginner

$   WHALE_COPY_VALUES - the array values are copied into new space, the user is free to reuse or delete the passed in array
$   WHALE_OWN_POINTER - the array values are NOT copied, the object takes ownership of the array and will free it later, the user cannot change or
$                       delete the array. The array MUST have been obtained with WhaleMalloc(). Hence this mode cannot be used in Fortran.
$   WHALE_USE_POINTER - the array values are NOT copied, the object uses the array but does NOT take ownership of the array. The user cannot use
                        the array but the user must delete the array after the object is destroyed.

E*/
typedef enum { WHALE_COPY_VALUES, WHALE_OWN_POINTER, WHALE_USE_POINTER} WhaleCopyMode;
WHALE_EXTERN const char *const WhaleCopyModes[];

/*MC
    WHALE_FALSE - False value of WhaleBool

    Level: beginner

    Note: Zero integer

.seealso: WhaleBool , WHALE_TRUE
M*/

/*MC
    WHALE_TRUE - True value of WhaleBool

    Level: beginner

    Note: Nonzero integer

.seealso: WhaleBool , WHALE_FALSE
M*/

/*MC
    WHALE_NULL - standard way of passing in a null or array or pointer. This is deprecated in C/C++ simply use NULL

   Level: beginner

   Notes: accepted by many Whale functions to not set a parameter and instead use
          some default

          This macro does not exist in Fortran; you must use WHALE_NULL_INTEGER,
          WHALE_NULL_DOUBLE_PRECISION, WHALE_NULL_FUNCTION, WHALE_NULL_OBJECT etc

.seealso: WHALE_DECIDE, WHALE_DEFAULT, WHALE_IGNORE, WHALE_DETERMINE

M*/
#define WHALE_NULL           NULL

/*MC
    WHALE_IGNORE - same as NULL, means Whale will ignore this argument

   Level: beginner

   Note: accepted by many Whale functions to not set a parameter and instead use
          some default

   Fortran Notes: This macro does not exist in Fortran; you must use WHALE_NULL_INTEGER,
          WHALE_NULL_DOUBLE_PRECISION etc

.seealso: WHALE_DECIDE, WHALE_DEFAULT, WHALE_NULL, WHALE_DETERMINE

M*/
#define WHALE_IGNORE         NULL

/*MC
    WHALE_DECIDE - standard way of passing in integer or floating point parameter
       where you wish Whale to use the default.

   Level: beginner

.seealso: WHALE_NULL, WHALE_DEFAULT, WHALE_IGNORE, WHALE_DETERMINE

M*/
#define WHALE_DECIDE  -1

/*MC
    WHALE_DETERMINE - standard way of passing in integer or floating point parameter
       where you wish Whale to compute the required value.

   Level: beginner


   Developer Note: I would like to use const WhaleInt WHALE_DETERMINE = WHALE_DECIDE; but for
     some reason this is not allowed by the standard even though WHALE_DECIDE is a constant value.

.seealso: WHALE_DECIDE, WHALE_DEFAULT, WHALE_IGNORE, VecSetSizes()

M*/
#define WHALE_DETERMINE WHALE_DECIDE

/*MC
    WHALE_DEFAULT - standard way of passing in integer or floating point parameter
       where you wish Whale to use the default.

   Level: beginner

   Fortran Notes: You need to use WHALE_DEFAULT_INTEGER or WHALE_DEFAULT_DOUBLE_PRECISION.

.seealso: WHALE_DECIDE, WHALE_IGNORE, WHALE_DETERMINE

M*/
#define WHALE_DEFAULT  -2

/*MC
    WHALE_COMM_WORLD - the equivalent of the MPI_COMM_WORLD communicator which represents
           all the processs that Whale knows about.

   Level: beginner

   Notes: By default WHALE_COMM_WORLD and MPI_COMM_WORLD are identical unless you wish to
          run Whale on ONLY a subset of MPI_COMM_WORLD. In that case create your new (smaller)
          communicator, call it, say comm, and set WHALE_COMM_WORLD = comm BEFORE calling
          WhaleInitialize()

.seealso: WHALE_COMM_SELF

M*/
WHALE_EXTERN MPI_Comm WHALE_COMM_WORLD;

/*MC
    WHALE_COMM_SELF - This is always MPI_COMM_SELF

   Level: beginner

.seealso: WHALE_COMM_WORLD

M*/
#define WHALE_COMM_SELF MPI_COMM_SELF

WHALE_EXTERN WhaleBool WhaleBeganMPI;
WHALE_EXTERN WhaleBool WhaleInitializeCalled;
WHALE_EXTERN WhaleBool WhaleFinalizeCalled;
WHALE_EXTERN WhaleBool WhaleCUSPSynchronize;

WHALE_EXTERN WhaleErrorCode WhaleSetHelpVersionFunctions(WhaleErrorCode (*)(MPI_Comm),WhaleErrorCode (*)(MPI_Comm));
WHALE_EXTERN WhaleErrorCode WhaleCommDuplicate(MPI_Comm,MPI_Comm*,int*);
WHALE_EXTERN WhaleErrorCode WhaleCommDestroy(MPI_Comm*);

/*MC
   WhaleMalloc - Allocates memory

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleMalloc(size_t m,void **result)

   Not Collective

   Input Parameter:
.  m - number of bytes to allocate

   Output Parameter:
.  result - memory allocated

   Level: beginner

   Notes: Memory is always allocated at least double aligned

          If you request memory of zero size it will allocate no space and assign the pointer to 0; WhaleFree() will
          properly handle not freeing the null pointer.

.seealso: WhaleFree(), WhaleNew()

  Concepts: memory allocation

M*/
#define WhaleMalloc(a,b)  ((a != 0) ? (*WhaleTrMalloc)((a),__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,(void**)(b)) : (*(b) = 0,0) )

/*MC
   WhaleAddrAlign - Rounds up an address to WHALE_MEMALIGN alignment

   Synopsis:
    #include "whalesys.h"
   void *WhaleAddrAlign(void *addr)

   Not Collective

   Input Parameters:
.  addr - address to align (any pointer type)

   Level: developer

.seealso: WhaleMallocAlign()

  Concepts: memory allocation
M*/
#define WhaleAddrAlign(a) (void*)((((WHALE_UINTPTR_T)(a))+(WHALE_MEMALIGN-1)) & ~(WHALE_MEMALIGN-1))

/*MC
   WhaleMalloc2 - Allocates 2 chunks of  memory both aligned to WHALE_MEMALIGN

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleMalloc2(size_t m1,type, t1,void **r1,size_t m2,type t2,void **r2)

   Not Collective

   Input Parameter:
+  m1 - number of elements to allocate in 1st chunk  (may be zero)
.  t1 - type of first memory elements
.  m2 - number of elements to allocate in 2nd chunk  (may be zero)
-  t2 - type of second memory elements

   Output Parameter:
+  r1 - memory allocated in first chunk
-  r2 - memory allocated in second chunk

   Level: developer

.seealso: WhaleFree(), WhaleNew(), WhaleMalloc()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleMalloc2(m1,t1,r1,m2,t2,r2) (WhaleMalloc((m1)*sizeof(t1),r1) || WhaleMalloc((m2)*sizeof(t2),r2))
#else
#define WhaleMalloc2(m1,t1,r1,m2,t2,r2) ((*(r2) = 0,WhaleMalloc((m1)*sizeof(t1)+(m2)*sizeof(t2)+(WHALE_MEMALIGN-1),r1)) || (*(r2) = (t2*)WhaleAddrAlign(*(r1)+m1),0))
#endif

/*MC
   WhaleMalloc3 - Allocates 3 chunks of  memory  all aligned to WHALE_MEMALIGN

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleMalloc3(size_t m1,type, t1,void **r1,size_t m2,type t2,void **r2,size_t m3,type t3,void **r3)

   Not Collective

   Input Parameter:
+  m1 - number of elements to allocate in 1st chunk  (may be zero)
.  t1 - type of first memory elements
.  m2 - number of elements to allocate in 2nd chunk  (may be zero)
.  t2 - type of second memory elements
.  m3 - number of elements to allocate in 3rd chunk  (may be zero)
-  t3 - type of third memory elements

   Output Parameter:
+  r1 - memory allocated in first chunk
.  r2 - memory allocated in second chunk
-  r3 - memory allocated in third chunk

   Level: developer

.seealso: WhaleFree(), WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree3()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleMalloc3(m1,t1,r1,m2,t2,r2,m3,t3,r3) (WhaleMalloc((m1)*sizeof(t1),r1) || WhaleMalloc((m2)*sizeof(t2),r2) || WhaleMalloc((m3)*sizeof(t3),r3))
#else
#define WhaleMalloc3(m1,t1,r1,m2,t2,r2,m3,t3,r3) ((*(r2) = 0,*(r3) = 0,WhaleMalloc((m1)*sizeof(t1)+(m2)*sizeof(t2)+(m3)*sizeof(t3)+2*(WHALE_MEMALIGN-1),r1)) \
                                                  || (*(r2) = (t2*)WhaleAddrAlign(*(r1)+m1),*(r3) = (t3*)WhaleAddrAlign(*(r2)+m2),0))
#endif

/*MC
   WhaleMalloc4 - Allocates 4 chunks of  memory  all aligned to WHALE_MEMALIGN

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleMalloc4(size_t m1,type, t1,void **r1,size_t m2,type t2,void **r2,size_t m3,type t3,void **r3,size_t m4,type t4,void **r4)

   Not Collective

   Input Parameter:
+  m1 - number of elements to allocate in 1st chunk  (may be zero)
.  t1 - type of first memory elements
.  m2 - number of elements to allocate in 2nd chunk  (may be zero)
.  t2 - type of second memory elements
.  m3 - number of elements to allocate in 3rd chunk  (may be zero)
.  t3 - type of third memory elements
.  m4 - number of elements to allocate in 4th chunk  (may be zero)
-  t4 - type of fourth memory elements

   Output Parameter:
+  r1 - memory allocated in first chunk
.  r2 - memory allocated in second chunk
.  r3 - memory allocated in third chunk
-  r4 - memory allocated in fourth chunk

   Level: developer

.seealso: WhaleFree(), WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree3(), WhaleFree4()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleMalloc4(m1,t1,r1,m2,t2,r2,m3,t3,r3,m4,t4,r4) (WhaleMalloc((m1)*sizeof(t1),r1) || WhaleMalloc((m2)*sizeof(t2),r2) || WhaleMalloc((m3)*sizeof(t3),r3) || WhaleMalloc((m4)*sizeof(t4),r4))
#else
#define WhaleMalloc4(m1,t1,r1,m2,t2,r2,m3,t3,r3,m4,t4,r4)               \
  ((*(r2) = 0, *(r3) = 0, *(r4) = 0,WhaleMalloc((m1)*sizeof(t1)+(m2)*sizeof(t2)+(m3)*sizeof(t3)+(m4)*sizeof(t4)+3*(WHALE_MEMALIGN-1),r1)) \
   || (*(r2) = (t2*)WhaleAddrAlign(*(r1)+m1),*(r3) = (t3*)WhaleAddrAlign(*(r2)+m2),*(r4) = (t4*)WhaleAddrAlign(*(r3)+m3),0))
#endif

/*MC
   WhaleMalloc5 - Allocates 5 chunks of  memory all aligned to WHALE_MEMALIGN

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleMalloc5(size_t m1,type, t1,void **r1,size_t m2,type t2,void **r2,size_t m3,type t3,void **r3,size_t m4,type t4,void **r4,size_t m5,type t5,void **r5)

   Not Collective

   Input Parameter:
+  m1 - number of elements to allocate in 1st chunk  (may be zero)
.  t1 - type of first memory elements
.  m2 - number of elements to allocate in 2nd chunk  (may be zero)
.  t2 - type of second memory elements
.  m3 - number of elements to allocate in 3rd chunk  (may be zero)
.  t3 - type of third memory elements
.  m4 - number of elements to allocate in 4th chunk  (may be zero)
.  t4 - type of fourth memory elements
.  m5 - number of elements to allocate in 5th chunk  (may be zero)
-  t5 - type of fifth memory elements

   Output Parameter:
+  r1 - memory allocated in first chunk
.  r2 - memory allocated in second chunk
.  r3 - memory allocated in third chunk
.  r4 - memory allocated in fourth chunk
-  r5 - memory allocated in fifth chunk

   Level: developer

.seealso: WhaleFree(), WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree3(), WhaleFree4(), WhaleFree5()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleMalloc5(m1,t1,r1,m2,t2,r2,m3,t3,r3,m4,t4,r4,m5,t5,r5) (WhaleMalloc((m1)*sizeof(t1),r1) || WhaleMalloc((m2)*sizeof(t2),r2) || WhaleMalloc((m3)*sizeof(t3),r3) || WhaleMalloc((m4)*sizeof(t4),r4) || WhaleMalloc((m5)*sizeof(t5),r5))
#else
#define WhaleMalloc5(m1,t1,r1,m2,t2,r2,m3,t3,r3,m4,t4,r4,m5,t5,r5)      \
  ((*(r2) = 0, *(r3) = 0, *(r4) = 0,*(r5) = 0,WhaleMalloc((m1)*sizeof(t1)+(m2)*sizeof(t2)+(m3)*sizeof(t3)+(m4)*sizeof(t4)+(m5)*sizeof(t5)+4*(WHALE_MEMALIGN-1),r1)) \
   || (*(r2) = (t2*)WhaleAddrAlign(*(r1)+m1),*(r3) = (t3*)WhaleAddrAlign(*(r2)+m2),*(r4) = (t4*)WhaleAddrAlign(*(r3)+m3),*(r5) = (t5*)WhaleAddrAlign(*(r4)+m4),0))
#endif


/*MC
   WhaleMalloc6 - Allocates 6 chunks of  memory all aligned to WHALE_MEMALIGN

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleMalloc6(size_t m1,type, t1,void **r1,size_t m2,type t2,void **r2,size_t m3,type t3,void **r3,size_t m4,type t4,void **r4,size_t m5,type t5,void **r5,size_t m6,type t6,void **r6)

   Not Collective

   Input Parameter:
+  m1 - number of elements to allocate in 1st chunk  (may be zero)
.  t1 - type of first memory elements
.  m2 - number of elements to allocate in 2nd chunk  (may be zero)
.  t2 - type of second memory elements
.  m3 - number of elements to allocate in 3rd chunk  (may be zero)
.  t3 - type of third memory elements
.  m4 - number of elements to allocate in 4th chunk  (may be zero)
.  t4 - type of fourth memory elements
.  m5 - number of elements to allocate in 5th chunk  (may be zero)
.  t5 - type of fifth memory elements
.  m6 - number of elements to allocate in 6th chunk  (may be zero)
-  t6 - type of sixth memory elements

   Output Parameter:
+  r1 - memory allocated in first chunk
.  r2 - memory allocated in second chunk
.  r3 - memory allocated in third chunk
.  r4 - memory allocated in fourth chunk
.  r5 - memory allocated in fifth chunk
-  r6 - memory allocated in sixth chunk

   Level: developer

.seealso: WhaleFree(), WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree3(), WhaleFree4(), WhaleFree5(), WhaleFree6()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleMalloc6(m1,t1,r1,m2,t2,r2,m3,t3,r3,m4,t4,r4,m5,t5,r5,m6,t6,r6) (WhaleMalloc((m1)*sizeof(t1),r1) || WhaleMalloc((m2)*sizeof(t2),r2) || WhaleMalloc((m3)*sizeof(t3),r3) || WhaleMalloc((m4)*sizeof(t4),r4) || WhaleMalloc((m5)*sizeof(t5),r5) || WhaleMalloc((m6)*sizeof(t6),r6))
#else
#define WhaleMalloc6(m1,t1,r1,m2,t2,r2,m3,t3,r3,m4,t4,r4,m5,t5,r5,m6,t6,r6) \
  ((*(r2) = 0, *(r3) = 0, *(r4) = 0,*(r5) = 0,*(r6) = 0,WhaleMalloc((m1)*sizeof(t1)+(m2)*sizeof(t2)+(m3)*sizeof(t3)+(m4)*sizeof(t4)+(m5)*sizeof(t5)+(m6)*sizeof(t6)+5*(WHALE_MEMALIGN-1),r1)) \
   || (*(r2) = (t2*)WhaleAddrAlign(*(r1)+m1),*(r3) = (t3*)WhaleAddrAlign(*(r2)+m2),*(r4) = (t4*)WhaleAddrAlign(*(r3)+m3),*(r5) = (t5*)WhaleAddrAlign(*(r4)+m4),*(r6) = (t6*)WhaleAddrAlign(*(r5)+m5),0))
#endif

/*MC
   WhaleMalloc7 - Allocates 7 chunks of  memory all aligned to WHALE_MEMALIGN

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleMalloc7(size_t m1,type, t1,void **r1,size_t m2,type t2,void **r2,size_t m3,type t3,void **r3,size_t m4,type t4,void **r4,size_t m5,type t5,void **r5,size_t m6,type t6,void **r6,size_t m7,type t7,void **r7)

   Not Collective

   Input Parameter:
+  m1 - number of elements to allocate in 1st chunk  (may be zero)
.  t1 - type of first memory elements
.  m2 - number of elements to allocate in 2nd chunk  (may be zero)
.  t2 - type of second memory elements
.  m3 - number of elements to allocate in 3rd chunk  (may be zero)
.  t3 - type of third memory elements
.  m4 - number of elements to allocate in 4th chunk  (may be zero)
.  t4 - type of fourth memory elements
.  m5 - number of elements to allocate in 5th chunk  (may be zero)
.  t5 - type of fifth memory elements
.  m6 - number of elements to allocate in 6th chunk  (may be zero)
.  t6 - type of sixth memory elements
.  m7 - number of elements to allocate in 7th chunk  (may be zero)
-  t7 - type of sixth memory elements

   Output Parameter:
+  r1 - memory allocated in first chunk
.  r2 - memory allocated in second chunk
.  r3 - memory allocated in third chunk
.  r4 - memory allocated in fourth chunk
.  r5 - memory allocated in fifth chunk
.  r6 - memory allocated in sixth chunk
-  r7 - memory allocated in seventh chunk

   Level: developer

.seealso: WhaleFree(), WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree3(), WhaleFree4(), WhaleFree5(), WhaleFree6(), WhaleFree7()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleMalloc7(m1,t1,r1,m2,t2,r2,m3,t3,r3,m4,t4,r4,m5,t5,r5,m6,t6,r6,m7,t7,r7) (WhaleMalloc((m1)*sizeof(t1),r1) || WhaleMalloc((m2)*sizeof(t2),r2) || WhaleMalloc((m3)*sizeof(t3),r3) || WhaleMalloc((m4)*sizeof(t4),r4) || WhaleMalloc((m5)*sizeof(t5),r5) || WhaleMalloc((m6)*sizeof(t6),r6) || WhaleMalloc((m7)*sizeof(t7),r7))
#else
#define WhaleMalloc7(m1,t1,r1,m2,t2,r2,m3,t3,r3,m4,t4,r4,m5,t5,r5,m6,t6,r6,m7,t7,r7) \
  ((*(r2) = 0, *(r3) = 0, *(r4) = 0,*(r5) = 0,*(r6) = 0,*(r7) = 0,WhaleMalloc((m1)*sizeof(t1)+(m2)*sizeof(t2)+(m3)*sizeof(t3)+(m4)*sizeof(t4)+(m5)*sizeof(t5)+(m6)*sizeof(t6)+(m7)*sizeof(t7)+6*(WHALE_MEMALIGN-1),r1)) \
   || (*(r2) = (t2*)WhaleAddrAlign(*(r1)+m1),*(r3) = (t3*)WhaleAddrAlign(*(r2)+m2),*(r4) = (t4*)WhaleAddrAlign(*(r3)+m3),*(r5) = (t5*)WhaleAddrAlign(*(r4)+m4),*(r6) = (t6*)WhaleAddrAlign(*(r5)+m5),*(r7) = (t7*)WhaleAddrAlign(*(r6)+m6),0))
#endif

/*MC
   WhaleNew - Allocates memory of a particular type, zeros the memory! Aligned to WHALE_MEMALIGN

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleNew(struct type,((type *))result)

   Not Collective

   Input Parameter:
.  type - structure name of space to be allocated. Memory of size sizeof(type) is allocated

   Output Parameter:
.  result - memory allocated

   Level: beginner

.seealso: WhaleFree(), WhaleMalloc(), WhaleNewLog()

  Concepts: memory allocation

M*/
#define WhaleNew(A,b)      (WhaleMalloc(sizeof(A),(b)) || WhaleMemzero(*(b),sizeof(A)))

/*MC
   WhaleNewLog - Allocates memory of a particular type, zeros the memory! Aligned to WHALE_MEMALIGN. Associates the memory allocated
         with the given object using WhaleLogObjectMemory().

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleNewLog(WhaleObject obj,struct type,((type *))result)

   Not Collective

   Input Parameter:
+  obj - object memory is logged to
-  type - structure name of space to be allocated. Memory of size sizeof(type) is allocated

   Output Parameter:
.  result - memory allocated

   Level: developer

.seealso: WhaleFree(), WhaleMalloc(), WhaleNew(), WhaleLogObjectMemory()

  Concepts: memory allocation

M*/
#define WhaleNewLog(o,A,b) (WhaleNew(A,b) || ((o) ? WhaleLogObjectMemory(o,sizeof(A)) : 0))

/*MC
   WhaleFree - Frees memory

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleFree(void *memory)

   Not Collective

   Input Parameter:
.   memory - memory to free (the pointer is ALWAYS set to 0 upon sucess)

   Level: beginner

   Notes: Memory must have been obtained with WhaleNew() or WhaleMalloc()

.seealso: WhaleNew(), WhaleMalloc(), WhaleFreeVoid()

  Concepts: memory allocation

M*/
#define WhaleFree(a)   ((a) && ((*WhaleTrFree)((void*)(a),__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__) || ((a) = 0,0)))

/*MC
   WhaleFreeVoid - Frees memory

   Synopsis:
    #include "whalesys.h"
   void WhaleFreeVoid(void *memory)

   Not Collective

   Input Parameter:
.   memory - memory to free

   Level: beginner

   Notes: This is different from WhaleFree() in that no error code is returned

.seealso: WhaleFree(), WhaleNew(), WhaleMalloc()

  Concepts: memory allocation

M*/
#define WhaleFreeVoid(a) ((*WhaleTrFree)((a),__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__),(a) = 0)


/*MC
   WhaleFree2 - Frees 2 chunks of memory obtained with WhaleMalloc2()

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleFree2(void *memory1,void *memory2)

   Not Collective

   Input Parameter:
+   memory1 - memory to free
-   memory2 - 2nd memory to free

   Level: developer

   Notes: Memory must have been obtained with WhaleMalloc2()

.seealso: WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleFree2(m1,m2)   (WhaleFree(m2) || WhaleFree(m1))
#else
#define WhaleFree2(m1,m2)   ((m2)=0, WhaleFree(m1))
#endif

/*MC
   WhaleFree3 - Frees 3 chunks of memory obtained with WhaleMalloc3()

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleFree3(void *memory1,void *memory2,void *memory3)

   Not Collective

   Input Parameter:
+   memory1 - memory to free
.   memory2 - 2nd memory to free
-   memory3 - 3rd memory to free

   Level: developer

   Notes: Memory must have been obtained with WhaleMalloc3()

.seealso: WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree(), WhaleMalloc3()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleFree3(m1,m2,m3)   (WhaleFree(m3) || WhaleFree(m2) || WhaleFree(m1))
#else
#define WhaleFree3(m1,m2,m3)   ((m3)=0,(m2)=0,WhaleFree(m1))
#endif

/*MC
   WhaleFree4 - Frees 4 chunks of memory obtained with WhaleMalloc4()

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleFree4(void *m1,void *m2,void *m3,void *m4)

   Not Collective

   Input Parameter:
+   m1 - memory to free
.   m2 - 2nd memory to free
.   m3 - 3rd memory to free
-   m4 - 4th memory to free

   Level: developer

   Notes: Memory must have been obtained with WhaleMalloc4()

.seealso: WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree(), WhaleMalloc3(), WhaleMalloc4()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleFree4(m1,m2,m3,m4)   (WhaleFree(m4) || WhaleFree(m3) || WhaleFree(m2) || WhaleFree(m1))
#else
#define WhaleFree4(m1,m2,m3,m4)   ((m4)=0,(m3)=0,(m2)=0,WhaleFree(m1))
#endif

/*MC
   WhaleFree5 - Frees 5 chunks of memory obtained with WhaleMalloc5()

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleFree5(void *m1,void *m2,void *m3,void *m4,void *m5)

   Not Collective

   Input Parameter:
+   m1 - memory to free
.   m2 - 2nd memory to free
.   m3 - 3rd memory to free
.   m4 - 4th memory to free
-   m5 - 5th memory to free

   Level: developer

   Notes: Memory must have been obtained with WhaleMalloc5()

.seealso: WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree(), WhaleMalloc3(), WhaleMalloc4(), WhaleMalloc5()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleFree5(m1,m2,m3,m4,m5)   (WhaleFree(m5) || WhaleFree(m4) || WhaleFree(m3) || WhaleFree(m2) || WhaleFree(m1))
#else
#define WhaleFree5(m1,m2,m3,m4,m5)   ((m5)=0,(m4)=0,(m3)=0,(m2)=0,WhaleFree(m1))
#endif


/*MC
   WhaleFree6 - Frees 6 chunks of memory obtained with WhaleMalloc6()

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleFree6(void *m1,void *m2,void *m3,void *m4,void *m5,void *m6)

   Not Collective

   Input Parameter:
+   m1 - memory to free
.   m2 - 2nd memory to free
.   m3 - 3rd memory to free
.   m4 - 4th memory to free
.   m5 - 5th memory to free
-   m6 - 6th memory to free


   Level: developer

   Notes: Memory must have been obtained with WhaleMalloc6()

.seealso: WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree(), WhaleMalloc3(), WhaleMalloc4(), WhaleMalloc5(), WhaleMalloc6()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleFree6(m1,m2,m3,m4,m5,m6)   (WhaleFree(m6) || WhaleFree(m5) || WhaleFree(m4) || WhaleFree(m3) || WhaleFree(m2) || WhaleFree(m1))
#else
#define WhaleFree6(m1,m2,m3,m4,m5,m6)   ((m6)=0,(m5)=0,(m4)=0,(m3)=0,(m2)=0,WhaleFree(m1))
#endif

/*MC
   WhaleFree7 - Frees 7 chunks of memory obtained with WhaleMalloc7()

   Synopsis:
    #include "whalesys.h"
   WhaleErrorCode WhaleFree7(void *m1,void *m2,void *m3,void *m4,void *m5,void *m6,void *m7)

   Not Collective

   Input Parameter:
+   m1 - memory to free
.   m2 - 2nd memory to free
.   m3 - 3rd memory to free
.   m4 - 4th memory to free
.   m5 - 5th memory to free
.   m6 - 6th memory to free
-   m7 - 7th memory to free


   Level: developer

   Notes: Memory must have been obtained with WhaleMalloc7()

.seealso: WhaleNew(), WhaleMalloc(), WhaleMalloc2(), WhaleFree(), WhaleMalloc3(), WhaleMalloc4(), WhaleMalloc5(), WhaleMalloc6(),
          WhaleMalloc7()

  Concepts: memory allocation

M*/
#if defined(WHALE_USE_DEBUG)
#define WhaleFree7(m1,m2,m3,m4,m5,m6,m7)   (WhaleFree(m7) || WhaleFree(m6) || WhaleFree(m5) || WhaleFree(m4) || WhaleFree(m3) || WhaleFree(m2) || WhaleFree(m1))
#else
#define WhaleFree7(m1,m2,m3,m4,m5,m6,m7)   ((m7)=0,(m6)=0,(m5)=0,(m4)=0,(m3)=0,(m2)=0,WhaleFree(m1))
#endif

WHALE_EXTERN WhaleErrorCode (*WhaleTrMalloc)(size_t,int,const char[],const char[],const char[],void**);
WHALE_EXTERN WhaleErrorCode (*WhaleTrFree)(void*,int,const char[],const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleMallocSet(WhaleErrorCode (*)(size_t,int,const char[],const char[],const char[],void**),WhaleErrorCode (*)(void*,int,const char[],const char[],const char[]));
WHALE_EXTERN WhaleErrorCode WhaleMallocClear(void);

/*
    WhaleLogDouble variables are used to contain double precision numbers
  that are not used in the numerical computations, but rather in logging,
  timing etc.
*/
typedef double WhaleLogDouble;
#define MPIU_WHALELOGDOUBLE MPI_DOUBLE

/*
   Routines for tracing memory corruption/bleeding with default Whale  memory allocation
*/
WHALE_EXTERN WhaleErrorCode WhaleMallocDump(FILE *);
WHALE_EXTERN WhaleErrorCode WhaleMallocDumpLog(FILE *);
WHALE_EXTERN WhaleErrorCode WhaleMallocGetCurrentUsage(WhaleLogDouble *);
WHALE_EXTERN WhaleErrorCode WhaleMallocGetMaximumUsage(WhaleLogDouble *);
WHALE_EXTERN WhaleErrorCode WhaleMallocDebug(WhaleBool);
WHALE_EXTERN WhaleErrorCode WhaleMallocGetDebug(WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleMallocValidate(int,const char[],const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleMallocSetDumpLog(void);
WHALE_EXTERN WhaleErrorCode WhaleMallocSetDumpLogThreshold(WhaleLogDouble);
WHALE_EXTERN WhaleErrorCode WhaleMallocGetDumpLog(WhaleBool*);

/*E
    WhaleDataType - Used for handling different basic data types.

   Level: beginner

   Developer comment: It would be nice if we could always just use MPI Datatypes, why can we not?

.seealso: WhaleBinaryRead(), WhaleBinaryWrite(), WhaleDataTypeToMPIDataType(),
          WhaleDataTypeGetSize()

E*/
typedef enum {WHALE_INT = 0,WHALE_DOUBLE = 1,WHALE_COMPLEX = 2, WHALE_LONG = 3 ,WHALE_SHORT = 4,WHALE_FLOAT = 5,
              WHALE_CHAR = 6,WHALE_BIT_LOGICAL = 7,WHALE_ENUM = 8,WHALE_BOOL=9, WHALE___FLOAT128 = 10,WHALE_OBJECT = 11, WHALE_FUNCTION = 12} WhaleDataType;
WHALE_EXTERN const char *const WhaleDataTypes[];

#if defined(WHALE_USE_COMPLEX)
#define  WHALE_SCALAR  WHALE_COMPLEX
#else
#if defined(WHALE_USE_REAL_SINGLE)
#define  WHALE_SCALAR  WHALE_FLOAT
#elif defined(WHALE_USE_REAL___FLOAT128)
#define  WHALE_SCALAR  WHALE___FLOAT128
#else
#define  WHALE_SCALAR  WHALE_DOUBLE
#endif
#endif
#if defined(WHALE_USE_REAL_SINGLE)
#define  WHALE_REAL  WHALE_FLOAT
#elif defined(WHALE_USE_REAL___FLOAT128)
#define  WHALE_REAL  WHALE___FLOAT128
#else
#define  WHALE_REAL  WHALE_DOUBLE
#endif
#define  WHALE_FORTRANADDR  WHALE_LONG

WHALE_EXTERN WhaleErrorCode WhaleDataTypeToMPIDataType(WhaleDataType,MPI_Datatype*);
WHALE_EXTERN WhaleErrorCode WhaleMPIDataTypeToWhaleDataType(MPI_Datatype,WhaleDataType*);
WHALE_EXTERN WhaleErrorCode WhaleDataTypeGetSize(WhaleDataType,size_t*);

/*
    Basic memory and string operations. These are usually simple wrappers
   around the basic Unix system calls, but a few of them have additional
   functionality and/or error checking.
*/
WHALE_EXTERN WhaleErrorCode WhaleBitMemcpy(void*,WhaleInt,const void*,WhaleInt,WhaleInt,WhaleDataType);
WHALE_EXTERN WhaleErrorCode WhaleMemmove(void*,void *,size_t);
WHALE_EXTERN WhaleErrorCode WhaleMemcmp(const void*,const void*,size_t,WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleStrlen(const char[],size_t*);
WHALE_EXTERN WhaleErrorCode WhaleStrToArray(const char[],char,int*,char ***);
WHALE_EXTERN WhaleErrorCode WhaleStrToArrayDestroy(int,char **);
WHALE_EXTERN WhaleErrorCode WhaleStrcmp(const char[],const char[],WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleStrgrt(const char[],const char[],WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleStrcasecmp(const char[],const char[],WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleStrncmp(const char[],const char[],size_t,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleStrcpy(char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleStrcat(char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleStrncat(char[],const char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleStrncpy(char[],const char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleStrchr(const char[],char,char *[]);
WHALE_EXTERN WhaleErrorCode WhaleStrtolower(char[]);
WHALE_EXTERN WhaleErrorCode WhaleStrtoupper(char[]);
WHALE_EXTERN WhaleErrorCode WhaleStrrchr(const char[],char,char *[]);
WHALE_EXTERN WhaleErrorCode WhaleStrstr(const char[],const char[],char *[]);
WHALE_EXTERN WhaleErrorCode WhaleStrrstr(const char[],const char[],char *[]);
WHALE_EXTERN WhaleErrorCode WhaleStrendswith(const char[],const char[],WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleStrbeginswith(const char[],const char[],WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleStrendswithwhich(const char[],const char *const*,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleStrallocpy(const char[],char *[]);
WHALE_EXTERN WhaleErrorCode WhaleStrArrayallocpy(const char *const*,char***);
WHALE_EXTERN WhaleErrorCode WhaleStrArrayDestroy(char***);
WHALE_EXTERN WhaleErrorCode WhaleStrreplace(MPI_Comm,const char[],char[],size_t);

WHALE_EXTERN void WhaleStrcmpNoError(const char[],const char[],WhaleBool  *);

/*S
    WhaleToken - 'Token' used for managing tokenizing strings

  Level: intermediate

.seealso: WhaleTokenCreate(), WhaleTokenFind(), WhaleTokenDestroy()
S*/
typedef struct _p_WhaleToken* WhaleToken;

WHALE_EXTERN WhaleErrorCode WhaleTokenCreate(const char[],const char,WhaleToken*);
WHALE_EXTERN WhaleErrorCode WhaleTokenFind(WhaleToken,char *[]);
WHALE_EXTERN WhaleErrorCode WhaleTokenDestroy(WhaleToken*);

WHALE_EXTERN WhaleErrorCode WhaleEListFind(WhaleInt,const char *const*,const char*,WhaleInt*,WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleEnumFind(const char *const*,const char*,WhaleEnum*,WhaleBool*);

/*
   These are  MPI operations for MPI_Allreduce() etc
*/
WHALE_EXTERN MPI_Op WhaleMaxSum_Op;
#if (defined(WHALE_HAVE_COMPLEX) && !defined(WHALE_HAVE_MPI_C_DOUBLE_COMPLEX)) || defined(WHALE_USE_REAL___FLOAT128)
WHALE_EXTERN MPI_Op MPIU_SUM;
#else
#define MPIU_SUM MPI_SUM
#endif
#if defined(WHALE_USE_REAL___FLOAT128)
WHALE_EXTERN MPI_Op MPIU_MAX;
WHALE_EXTERN MPI_Op MPIU_MIN;
#else
#define MPIU_MAX MPI_MAX
#define MPIU_MIN MPI_MIN
#endif
WHALE_EXTERN WhaleErrorCode WhaleMaxSum(MPI_Comm,const WhaleInt[],WhaleInt*,WhaleInt*);

WHALE_EXTERN WhaleErrorCode MPIULong_Send(void*,WhaleInt,MPI_Datatype,WhaleMPIInt,WhaleMPIInt,MPI_Comm);
WHALE_EXTERN WhaleErrorCode MPIULong_Recv(void*,WhaleInt,MPI_Datatype,WhaleMPIInt,WhaleMPIInt,MPI_Comm);

/*S
     WhaleObject - any Whale object, WhaleViewer, Mat, Vec, KSP etc

   Level: beginner

   Note: This is the base class from which all Whale objects are derived from.

.seealso:  WhaleObjectDestroy(), WhaleObjectView(), WhaleObjectGetName(), WhaleObjectSetName(), WhaleObjectReference(), WhaleObjectDereferenc()
S*/
typedef struct _p_WhaleObject* WhaleObject;

/*S
     WhaleFunctionList - Linked list of functions, possibly stored in dynamic libraries, accessed
      by string name

   Level: advanced

.seealso:  WhaleFunctionListAdd(), WhaleFunctionListDestroy(), WhaleOpFlist
S*/
typedef struct _n_WhaleFunctionList *WhaleFunctionList;

/*E
  WhaleFileMode - Access mode for a file.

  Level: beginner

  FILE_MODE_READ - open a file at its beginning for reading

  FILE_MODE_WRITE - open a file at its beginning for writing (will create if the file does not exist)

  FILE_MODE_APPEND - open a file at end for writing

  FILE_MODE_UPDATE - open a file for updating, meaning for reading and writing

  FILE_MODE_APPEND_UPDATE - open a file for updating, meaning for reading and writing, at the end

.seealso: WhaleViewerFileSetMode()
E*/
typedef enum {FILE_MODE_READ, FILE_MODE_WRITE, FILE_MODE_APPEND, FILE_MODE_UPDATE, FILE_MODE_APPEND_UPDATE} WhaleFileMode;
extern const char *const WhaleFileModes[];

/*
    Defines Whale error handling.
*/
#include <whaleerror.h>

#define WHALE_SMALLEST_CLASSID  1211211
WHALE_EXTERN WhaleClassId WHALE_LARGEST_CLASSID;
WHALE_EXTERN WhaleClassId WHALE_OBJECT_CLASSID;
WHALE_EXTERN WhaleErrorCode WhaleClassIdRegister(const char[],WhaleClassId *);

/*
   Routines that get memory usage information from the OS
*/
WHALE_EXTERN WhaleErrorCode WhaleMemoryGetCurrentUsage(WhaleLogDouble *);
WHALE_EXTERN WhaleErrorCode WhaleMemoryGetMaximumUsage(WhaleLogDouble *);
WHALE_EXTERN WhaleErrorCode WhaleMemorySetGetMaximumUsage(void);

WHALE_EXTERN WhaleErrorCode WhaleInfoAllow(WhaleBool ,const char []);
WHALE_EXTERN WhaleErrorCode WhaleSleep(WhaleReal);

/*
   Initialization of Whale
*/
WHALE_EXTERN WhaleErrorCode WhaleInitialize(int*,char***,const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleInitializeNoPointers(int,char**,const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleInitializeNoArguments(void);
WHALE_EXTERN WhaleErrorCode WhaleInitialized(WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleFinalized(WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleFinalize(void);
WHALE_EXTERN WhaleErrorCode WhaleInitializeFortran(void);
WHALE_EXTERN WhaleErrorCode WhaleGetArgs(int*,char ***);
WHALE_EXTERN WhaleErrorCode WhaleGetArguments(char ***);
WHALE_EXTERN WhaleErrorCode WhaleFreeArguments(char **);

WHALE_EXTERN WhaleErrorCode WhaleEnd(void);
WHALE_EXTERN WhaleErrorCode WhaleSysInitializePackage(void);

WHALE_EXTERN MPI_Comm WHALE_COMM_LOCAL_WORLD;
WHALE_EXTERN WhaleErrorCode WhaleHMPIMerge(WhaleMPIInt,WhaleErrorCode (*)(void*),void*);
WHALE_EXTERN WhaleErrorCode WhaleHMPISpawn(WhaleMPIInt);
WHALE_EXTERN WhaleErrorCode WhaleHMPIFinalize(void);
WHALE_EXTERN WhaleErrorCode WhaleHMPIRun(MPI_Comm,WhaleErrorCode (*)(MPI_Comm,void *),void*);
WHALE_EXTERN WhaleErrorCode WhaleHMPIRunCtx(MPI_Comm,WhaleErrorCode (*)(MPI_Comm,void*,void *),void*);
WHALE_EXTERN WhaleErrorCode WhaleHMPIFree(MPI_Comm,void*);
WHALE_EXTERN WhaleErrorCode WhaleHMPIMalloc(MPI_Comm,size_t,void**);

WHALE_EXTERN WhaleErrorCode WhalePythonInitialize(const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhalePythonFinalize(void);
WHALE_EXTERN WhaleErrorCode WhalePythonPrintError(void);
WHALE_EXTERN WhaleErrorCode WhalePythonMonitorSet(WhaleObject,const char[]);

/*
     These are so that in extern C code we can caste function pointers to non-extern C
   function pointers. Since the regular C++ code expects its function pointers to be C++
*/
WHALE_EXTERN_TYPEDEF typedef void (**WhaleVoidStarFunction)(void);
WHALE_EXTERN_TYPEDEF typedef void (*WhaleVoidFunction)(void);
WHALE_EXTERN_TYPEDEF typedef WhaleErrorCode (*WhaleErrorCodeFunction)(void);

/*
    Functions that can act on any Whale object.
*/
WHALE_EXTERN WhaleErrorCode WhaleObjectDestroy(WhaleObject*);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetComm(WhaleObject,MPI_Comm *);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetClassId(WhaleObject,WhaleClassId *);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetClassName(WhaleObject,const char *[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectSetType(WhaleObject,const char []);
WHALE_EXTERN WhaleErrorCode WhaleObjectSetPrecision(WhaleObject,WhalePrecision);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetType(WhaleObject,const char *[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectSetName(WhaleObject,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetName(WhaleObject,const char*[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectSetTabLevel(WhaleObject,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetTabLevel(WhaleObject,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleObjectIncrementTabLevel(WhaleObject,WhaleObject,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleObjectReference(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetReference(WhaleObject,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleObjectDereference(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetNewTag(WhaleObject,WhaleMPIInt *);
WHALE_EXTERN WhaleErrorCode WhaleObjectCompose(WhaleObject,const char[],WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectRemoveReference(WhaleObject,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectQuery(WhaleObject,const char[],WhaleObject *);
WHALE_EXTERN WhaleErrorCode WhaleObjectComposeFunction_Private(WhaleObject,const char[],void (*)(void));
#define WhaleObjectComposeFunction(a,b,d) WhaleObjectComposeFunction_Private(a,b,(WhaleVoidFunction)(d))
WHALE_EXTERN WhaleErrorCode WhaleObjectSetFromOptions(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectSetUp(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleCommGetNewTag(MPI_Comm,WhaleMPIInt *);
WHALE_EXTERN WhaleErrorCode WhaleObjectAddOptionsHandler(WhaleObject,WhaleErrorCode (*)(WhaleObject,void*),WhaleErrorCode (*)(WhaleObject,void*),void*);
WHALE_EXTERN WhaleErrorCode WhaleObjectProcessOptionsHandlers(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectDestroyOptionsHandlers(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectsGetGlobalNumbering(MPI_Comm,WhaleInt,WhaleObject*,WhaleInt*,WhaleInt*);

#include <whaleviewertypes.h>
#include <whaleoptions.h>

WHALE_EXTERN WhaleErrorCode WhaleMemoryShowUsage(WhaleViewer,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectPrintClassNamePrefixType(WhaleObject,WhaleViewer,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectView(WhaleObject,WhaleViewer);
#define WhaleObjectQueryFunction(obj,name,fptr) WhaleObjectQueryFunction_Private((obj),(name),(WhaleVoidFunction*)(fptr))
WHALE_EXTERN WhaleErrorCode WhaleObjectQueryFunction_Private(WhaleObject,const char[],void (**)(void));
WHALE_EXTERN WhaleErrorCode WhaleObjectSetOptionsPrefix(WhaleObject,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectAppendOptionsPrefix(WhaleObject,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectPrependOptionsPrefix(WhaleObject,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectGetOptionsPrefix(WhaleObject,const char*[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectChangeTypeName(WhaleObject,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectRegisterDestroy(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectRegisterDestroyAll(void);
WHALE_EXTERN WhaleErrorCode WhaleObjectName(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectTypeCompare(WhaleObject,const char[],WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleObjectTypeCompareAny(WhaleObject,WhaleBool*,const char[],...);
WHALE_EXTERN WhaleErrorCode WhaleRegisterFinalize(WhaleErrorCode (*)(void));
WHALE_EXTERN WhaleErrorCode WhaleRegisterFinalizeAll(void);

#if defined(WHALE_HAVE_AMS)
WHALE_EXTERN WhaleErrorCode WhaleObjectAMSViewOff(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectAMSSetBlock(WhaleObject,WhaleBool);
WHALE_EXTERN WhaleErrorCode WhaleObjectAMSBlock(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectAMSGrantAccess(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectAMSTakeAccess(WhaleObject);
WHALE_EXTERN void           WhaleStackAMSGrantAccess(void);
WHALE_EXTERN void           WhaleStackAMSTakeAccess(void);
WHALE_EXTERN WhaleErrorCode WhaleStackViewAMS(void);
WHALE_EXTERN WhaleErrorCode WhaleStackAMSViewOff(void);

#else
#define WhaleObjectAMSViewOff(obj)             0
#define WhaleObjectAMSSetBlock(obj,flg)        0
#define WhaleObjectAMSBlock(obj)               0
#define WhaleObjectAMSGrantAccess(obj)         0
#define WhaleObjectAMSTakeAccess(obj)          0
#define WhaleStackViewAMS()                    0
#define WhaleStackAMSViewOff()                 0
#define WhaleStackAMSTakeAccess()
#define WhaleStackAMSGrantAccess()

#endif

typedef void* WhaleDLHandle;
typedef enum {WHALE_DL_DECIDE=0,WHALE_DL_NOW=1,WHALE_DL_LOCAL=2} WhaleDLMode;
WHALE_EXTERN WhaleErrorCode WhaleDLOpen(const char[],WhaleDLMode,WhaleDLHandle *);
WHALE_EXTERN WhaleErrorCode WhaleDLClose(WhaleDLHandle *);
WHALE_EXTERN WhaleErrorCode WhaleDLSym(WhaleDLHandle,const char[],void **);


#if defined(WHALE_USE_DEBUG)
WHALE_EXTERN WhaleErrorCode WhaleMallocGetStack(void*,WhaleStack**);
#endif
WHALE_EXTERN WhaleErrorCode WhaleObjectsDump(FILE*,WhaleBool);

/*S
     WhaleObjectList - Linked list of Whale objects, each accessable by string name

   Level: developer

   Notes: Used by WhaleObjectCompose() and WhaleObjectQuery()

.seealso:  WhaleObjectListAdd(), WhaleObjectListDestroy(), WhaleObjectListFind(), WhaleObjectCompose(), WhaleObjectQuery(), WhaleFunctionList
S*/
typedef struct _n_WhaleObjectList *WhaleObjectList;

WHALE_EXTERN WhaleErrorCode WhaleObjectListDestroy(WhaleObjectList*);
WHALE_EXTERN WhaleErrorCode WhaleObjectListFind(WhaleObjectList,const char[],WhaleObject*);
WHALE_EXTERN WhaleErrorCode WhaleObjectListReverseFind(WhaleObjectList,WhaleObject,char**,WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleObjectListAdd(WhaleObjectList *,const char[],WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleObjectListRemoveReference(WhaleObjectList *,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectListDuplicate(WhaleObjectList,WhaleObjectList *);

/*
    Dynamic library lists. Lists of names of routines in objects or in dynamic
  link libraries that will be loaded as needed.
*/

#define WhaleFunctionListAdd(list,name,fptr) WhaleFunctionListAdd_Private((list),(name),(WhaleVoidFunction)(fptr))
WHALE_EXTERN WhaleErrorCode WhaleFunctionListAdd_Private(WhaleFunctionList*,const char[],void (*)(void));
WHALE_EXTERN WhaleErrorCode WhaleFunctionListDestroy(WhaleFunctionList*);
#define WhaleFunctionListFind(list,name,fptr) WhaleFunctionListFind_Private((list),(name),(WhaleVoidFunction*)(fptr))
WHALE_EXTERN WhaleErrorCode WhaleFunctionListFind_Private(WhaleFunctionList,const char[],void (**)(void));
WHALE_EXTERN WhaleErrorCode WhaleFunctionListPrintTypes(MPI_Comm,FILE*,const char[],const char[],const char[],const char[],WhaleFunctionList,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleFunctionListDuplicate(WhaleFunctionList,WhaleFunctionList *);
WHALE_EXTERN WhaleErrorCode WhaleFunctionListView(WhaleFunctionList,WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleFunctionListGet(WhaleFunctionList,const char ***,int*);

/*S
     WhaleDLLibrary - Linked list of dynamics libraries to search for functions

   Level: advanced

   --with-shared-libraries --with-dynamic-loading must be used with ./configure to use dynamic libraries

.seealso:  WhaleDLLibraryOpen()
S*/
typedef struct _n_WhaleDLLibrary *WhaleDLLibrary;
WHALE_EXTERN WhaleDLLibrary  WhaleDLLibrariesLoaded;
WHALE_EXTERN WhaleErrorCode WhaleDLLibraryAppend(MPI_Comm,WhaleDLLibrary *,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleDLLibraryPrepend(MPI_Comm,WhaleDLLibrary *,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleDLLibrarySym(MPI_Comm,WhaleDLLibrary *,const char[],const char[],void **);
WHALE_EXTERN WhaleErrorCode WhaleDLLibraryPrintPath(WhaleDLLibrary);
WHALE_EXTERN WhaleErrorCode WhaleDLLibraryRetrieve(MPI_Comm,const char[],char *,size_t,WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleDLLibraryOpen(MPI_Comm,const char[],WhaleDLLibrary *);
WHALE_EXTERN WhaleErrorCode WhaleDLLibraryClose(WhaleDLLibrary);

/*
     Useful utility routines
*/
WHALE_EXTERN WhaleErrorCode WhaleSplitOwnership(MPI_Comm,WhaleInt*,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSplitOwnershipBlock(MPI_Comm,WhaleInt,WhaleInt*,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSequentialPhaseBegin(MPI_Comm,WhaleMPIInt);
WHALE_EXTERN WhaleErrorCode WhaleSequentialPhaseEnd(MPI_Comm,WhaleMPIInt);
WHALE_EXTERN WhaleErrorCode WhaleBarrier(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleMPIDump(FILE*);

/*
    WhaleNot - negates a logical type value and returns result as a WhaleBool

    Notes: This is useful in cases like
$     int        *a;
$     WhaleBool  flag = WhaleNot(a)
     where !a does not return a WhaleBool  because we cannot provide a cast from int to WhaleBool  in C.
*/
#define WhaleNot(a) ((a) ? WHALE_FALSE : WHALE_TRUE)

#if defined(WHALE_HAVE_VALGRIND)
#  include <valgrind/valgrind.h>
#  define WHALE_RUNNING_ON_VALGRIND RUNNING_ON_VALGRIND
#else
#  define WHALE_RUNNING_ON_VALGRIND WHALE_FALSE
#endif


/*MC
    WhaleHelpPrintf - Prints help messages.

   Synopsis:
    #include "whalesys.h"
     WhaleErrorCode (*WhaleHelpPrintf)(const char format[],...);

    Not Collective

    Input Parameters:
.   format - the usual printf() format string

   Level: developer

    Fortran Note:
    This routine is not supported in Fortran.

    Concepts: help messages^printing
    Concepts: printing^help messages

.seealso: WhaleFPrintf(), WhaleSynchronizedPrintf(), WhaleErrorPrintf()
M*/
WHALE_EXTERN WhaleErrorCode (*WhaleHelpPrintf)(MPI_Comm,const char[],...);

/*
     Defines Whale profiling.
*/
#include <whalelog.h>



/*
      Simple Whale parallel IO for ASCII printing
*/
WHALE_EXTERN WhaleErrorCode WhaleFixFilename(const char[],char[]);
WHALE_EXTERN WhaleErrorCode WhaleFOpen(MPI_Comm,const char[],const char[],FILE**);
WHALE_EXTERN WhaleErrorCode WhaleFClose(MPI_Comm,FILE*);
WHALE_EXTERN WhaleErrorCode WhaleFPrintf(MPI_Comm,FILE*,const char[],...);
WHALE_EXTERN WhaleErrorCode WhalePrintf(MPI_Comm,const char[],...);
WHALE_EXTERN WhaleErrorCode WhaleSNPrintf(char*,size_t,const char [],...);
WHALE_EXTERN WhaleErrorCode WhaleSNPrintfCount(char*,size_t,const char [],size_t*,...);

/* These are used internally by Whale ASCII IO routines*/
#include <stdarg.h>
WHALE_EXTERN WhaleErrorCode WhaleVSNPrintf(char*,size_t,const char[],size_t*,va_list);
WHALE_EXTERN WhaleErrorCode (*WhaleVFPrintf)(FILE*,const char[],va_list);
WHALE_EXTERN WhaleErrorCode WhaleVFPrintfDefault(FILE*,const char[],va_list);

#if defined(WHALE_HAVE_MATLAB_ENGINE)
WHALE_EXTERN WhaleErrorCode WhaleVFPrintf_Matlab(FILE*,const char[],va_list);
#endif

WHALE_EXTERN WhaleErrorCode WhaleErrorPrintfDefault(const char [],...);
WHALE_EXTERN WhaleErrorCode WhaleErrorPrintfNone(const char [],...);
WHALE_EXTERN WhaleErrorCode WhaleHelpPrintfDefault(MPI_Comm,const char [],...);

#if defined(WHALE_HAVE_POPEN)
WHALE_EXTERN WhaleErrorCode WhalePOpen(MPI_Comm,const char[],const char[],const char[],FILE **);
WHALE_EXTERN WhaleErrorCode WhalePClose(MPI_Comm,FILE*,WhaleInt*);
#endif

WHALE_EXTERN WhaleErrorCode WhaleSynchronizedPrintf(MPI_Comm,const char[],...);
WHALE_EXTERN WhaleErrorCode WhaleSynchronizedFPrintf(MPI_Comm,FILE*,const char[],...);
WHALE_EXTERN WhaleErrorCode WhaleSynchronizedFlush(MPI_Comm);
WHALE_EXTERN WhaleErrorCode WhaleSynchronizedFGets(MPI_Comm,FILE*,size_t,char[]);
WHALE_EXTERN WhaleErrorCode WhaleStartMatlab(MPI_Comm,const char[],const char[],FILE**);
WHALE_EXTERN WhaleErrorCode WhaleStartJava(MPI_Comm,const char[],const char[],FILE**);
WHALE_EXTERN WhaleErrorCode WhaleGetWhaleDir(const char*[]);

WHALE_EXTERN WhaleErrorCode WhalePopUpSelect(MPI_Comm,const char*,const char*,int,const char**,int*);

/*S
     WhaleContainer - Simple Whale object that contains a pointer to any required data

   Level: advanced

.seealso:  WhaleObject, WhaleContainerCreate()
S*/
WHALE_EXTERN WhaleClassId WHALE_CONTAINER_CLASSID;
typedef struct _p_WhaleContainer*  WhaleContainer;
WHALE_EXTERN WhaleErrorCode WhaleContainerGetPointer(WhaleContainer,void **);
WHALE_EXTERN WhaleErrorCode WhaleContainerSetPointer(WhaleContainer,void *);
WHALE_EXTERN WhaleErrorCode WhaleContainerDestroy(WhaleContainer*);
WHALE_EXTERN WhaleErrorCode WhaleContainerCreate(MPI_Comm,WhaleContainer *);
WHALE_EXTERN WhaleErrorCode WhaleContainerSetUserDestroy(WhaleContainer, WhaleErrorCode (*)(void*));

/*
   For use in debuggers
*/
WHALE_EXTERN WhaleMPIInt WhaleGlobalRank;
WHALE_EXTERN WhaleMPIInt WhaleGlobalSize;
WHALE_EXTERN WhaleErrorCode WhaleIntView(WhaleInt,const WhaleInt[],WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleRealView(WhaleInt,const WhaleReal[],WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleScalarView(WhaleInt,const WhaleScalar[],WhaleViewer);

#include <stddef.h>
#if defined(WHALE_HAVE_MEMORY_H)
#include <memory.h>
#endif
#if defined(WHALE_HAVE_STDLIB_H)
#include <stdlib.h>
#endif

#if defined(WHALE_HAVE_XMMINTRIN_H) && !defined(__CUDACC__)
#include <xmmintrin.h>
#endif
#if defined(WHALE_HAVE_STDINT_H)
#include <stdint.h>
#endif

#undef __FUNCT__
#define __FUNCT__ "WhaleMemcpy"
/*@C
   WhaleMemcpy - Copies n bytes, beginning at location b, to the space
   beginning at location a. The two memory regions CANNOT overlap, use
   WhaleMemmove() in that case.

   Not Collective

   Input Parameters:
+  b - pointer to initial memory space
-  n - length (in bytes) of space to copy

   Output Parameter:
.  a - pointer to copy space

   Level: intermediate

   Compile Option:
    WHALE_PREFER_DCOPY_FOR_MEMCPY will cause the BLAS dcopy() routine to be used
                                  for memory copies on double precision values.
    WHALE_PREFER_COPY_FOR_MEMCPY will cause C code to be used
                                  for memory copies on double precision values.
    WHALE_PREFER_FORTRAN_FORMEMCPY will cause Fortran code to be used
                                  for memory copies on double precision values.

   Note:
   This routine is analogous to memcpy().

   Developer Note: this is inlined for fastest performance

  Concepts: memory^copying
  Concepts: copying^memory

.seealso: WhaleMemmove()

@*/
WHALE_STATIC_INLINE WhaleErrorCode WhaleMemcpy(void *a,const void *b,size_t n)
{
#if defined(WHALE_USE_DEBUG)
  unsigned long al = (unsigned long) a,bl = (unsigned long) b;
  unsigned long nl = (unsigned long) n;
  WhaleFunctionBegin;
  if (n > 0 && !b) SETERRQ(WHALE_COMM_SELF,WHALE_ERR_ARG_NULL,"Trying to copy from a null pointer");
  if (n > 0 && !a) SETERRQ(WHALE_COMM_SELF,WHALE_ERR_ARG_NULL,"Trying to copy to a null pointer");
#else
  WhaleFunctionBegin;
#endif
  if (a != b) {
#if defined(WHALE_USE_DEBUG)
    if ((al > bl && (al - bl) < nl) || (bl - al) < nl)  SETERRQ3(WHALE_COMM_SELF,WHALE_ERR_ARG_INCOMP,"Memory regions overlap: either use WhaleMemmov()\n\
              or make sure your copy regions and lengths are correct. \n\
              Length (bytes) %ld first address %ld second address %ld",nl,al,bl);
#endif
#if (defined(WHALE_PREFER_DCOPY_FOR_MEMCPY) || defined(WHALE_PREFER_COPY_FOR_MEMCPY) || defined(WHALE_PREFER_FORTRAN_FORMEMCPY))
   if (!(((long) a) % sizeof(WhaleScalar)) && !(n % sizeof(WhaleScalar))) {
      size_t len = n/sizeof(WhaleScalar);
#if defined(WHALE_PREFER_DCOPY_FOR_MEMCPY)
      WhaleBLASInt   one = 1,blen;
      WhaleErrorCode ierr;
      ierr = WhaleBLASIntCast(len,&blen);CHKERRQ(ierr);
      WhaleStackCallBLAS("BLAScopy",BLAScopy_(&blen,(WhaleScalar *)b,&one,(WhaleScalar *)a,&one));
#elif defined(WHALE_PREFER_FORTRAN_FORMEMCPY)
      fortrancopy_(&len,(WhaleScalar*)b,(WhaleScalar*)a);
#else
      size_t      i;
      WhaleScalar *x = (WhaleScalar*)b, *y = (WhaleScalar*)a;
      for (i=0; i<len; i++) y[i] = x[i];
#endif
    } else {
      memcpy((char*)(a),(char*)(b),n);
    }
#else
    memcpy((char*)(a),(char*)(b),n);
#endif
  }
  WhaleFunctionReturn(0);
}

/*@C
   WhaleMemzero - Zeros the specified memory.

   Not Collective

   Input Parameters:
+  a - pointer to beginning memory location
-  n - length (in bytes) of memory to initialize

   Level: intermediate

   Compile Option:
   WHALE_PREFER_BZERO - on certain machines (the IBM RS6000) the bzero() routine happens
  to be faster than the memset() routine. This flag causes the bzero() routine to be used.

   Developer Note: this is inlined for fastest performance

   Concepts: memory^zeroing
   Concepts: zeroing^memory

.seealso: WhaleMemcpy()
@*/
WHALE_STATIC_INLINE WhaleErrorCode  WhaleMemzero(void *a,size_t n)
{
  if (n > 0) {
#if defined(WHALE_USE_DEBUG)
    if (!a) SETERRQ(WHALE_COMM_SELF,WHALE_ERR_ARG_NULL,"Trying to zero at a null pointer");
#endif
#if defined(WHALE_PREFER_ZERO_FOR_MEMZERO)
    if (!(((long) a) % sizeof(WhaleScalar)) && !(n % sizeof(WhaleScalar))) {
      size_t      i,len = n/sizeof(WhaleScalar);
      WhaleScalar *x = (WhaleScalar*)a;
      for (i=0; i<len; i++) x[i] = 0.0;
    } else {
#elif defined(WHALE_PREFER_FORTRAN_FOR_MEMZERO)
    if (!(((long) a) % sizeof(WhaleScalar)) && !(n % sizeof(WhaleScalar))) {
      WhaleInt len = n/sizeof(WhaleScalar);
      fortranzero_(&len,(WhaleScalar*)a);
    } else {
#endif
#if defined(WHALE_PREFER_BZERO)
      bzero((char *)a,n);
#else
      memset((char*)a,0,n);
#endif
#if defined(WHALE_PREFER_ZERO_FOR_MEMZERO) || defined(WHALE_PREFER_FORTRAN_FOR_MEMZERO)
    }
#endif
  }
  return 0;
}

/*MC
   WhalePrefetchBlock - Prefetches a block of memory

   Synopsis:
    #include "whalesys.h"
    void WhalePrefetchBlock(const anytype *a,size_t n,int rw,int t)

   Not Collective

   Input Parameters:
+  a - pointer to first element to fetch (any type but usually WhaleInt or WhaleScalar)
.  n - number of elements to fetch
.  rw - 1 if the memory will be written to, otherwise 0 (ignored by many processors)
-  t - temporal locality (WHALE_PREFETCH_HINT_{NTA,T0,T1,T2}), see note

   Level: developer

   Notes:
   The last two arguments (rw and t) must be compile-time constants.

   Adopting Intel's x86/x86-64 conventions, there are four levels of temporal locality.  Not all architectures offer
   equivalent locality hints, but the following macros are always defined to their closest analogue.
+  WHALE_PREFETCH_HINT_NTA - Non-temporal.  Prefetches directly to L1, evicts to memory (skips higher level cache unless it was already there when prefetched).
.  WHALE_PREFETCH_HINT_T0 - Fetch to all levels of cache and evict to the closest level.  Use this when the memory will be reused regularly despite necessary eviction from L1.
.  WHALE_PREFETCH_HINT_T1 - Fetch to level 2 and higher (not L1).
-  WHALE_PREFETCH_HINT_T2 - Fetch to high-level cache only.  (On many systems, T0 and T1 are equivalent.)

   This function does nothing on architectures that do not support prefetch and never errors (even if passed an invalid
   address).

   Concepts: memory
M*/
#define WhalePrefetchBlock(a,n,rw,t) do {                               \
    const char *_p = (const char*)(a),*_end = (const char*)((a)+(n));   \
    for ( ; _p < _end; _p += WHALE_LEVEL1_DCACHE_LINESIZE) WHALE_Prefetch(_p,(rw),(t)); \
  } while (0)

/*
      Determine if some of the kernel computation routines use
   Fortran (rather than C) for the numerical calculations. On some machines
   and compilers (like complex numbers) the Fortran version of the routines
   is faster than the C/C++ versions. The flag --with-fortran-kernels
   should be used with ./configure to turn these on.
*/
#if defined(WHALE_USE_FORTRAN_KERNELS)

#if !defined(WHALE_USE_FORTRAN_KERNEL_MULTCRL)
#define WHALE_USE_FORTRAN_KERNEL_MULTCRL
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_MULTAIJPERM)
#define WHALE_USE_FORTRAN_KERNEL_MULTAIJPERM
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_MULTAIJ)
#define WHALE_USE_FORTRAN_KERNEL_MULTAIJ
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_MULTTRANSPOSEAIJ)
#define WHALE_USE_FORTRAN_KERNEL_MULTTRANSPOSEAIJ
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_NORM)
#define WHALE_USE_FORTRAN_KERNEL_NORM
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_MAXPY)
#define WHALE_USE_FORTRAN_KERNEL_MAXPY
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_SOLVEAIJ)
#define WHALE_USE_FORTRAN_KERNEL_SOLVEAIJ
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_RELAXAIJ)
#define WHALE_USE_FORTRAN_KERNEL_RELAXAIJ
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_SOLVEBAIJ)
#define WHALE_USE_FORTRAN_KERNEL_SOLVEBAIJ
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_MULTADDAIJ)
#define WHALE_USE_FORTRAN_KERNEL_MULTADDAIJ
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_MDOT)
#define WHALE_USE_FORTRAN_KERNEL_MDOT
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_XTIMESY)
#define WHALE_USE_FORTRAN_KERNEL_XTIMESY
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_AYPX)
#define WHALE_USE_FORTRAN_KERNEL_AYPX
#endif

#if !defined(WHALE_USE_FORTRAN_KERNEL_WAXPY)
#define WHALE_USE_FORTRAN_KERNEL_WAXPY
#endif

#endif

/*
    Macros for indicating code that should be compiled with a C interface,
   rather than a C++ interface. Any routines that are dynamically loaded
   (such as the PCCreate_XXX() routines) must be wrapped so that the name
   mangler does not change the functions symbol name. This just hides the
   ugly extern "C" {} wrappers.
*/
#if defined(__cplusplus)
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

/* --------------------------------------------------------------------*/

/*MC
    MPI_Comm - the basic object used by MPI to determine which processes are involved in a
        communication

   Level: beginner

   Note: This manual page is a place-holder because MPICH does not have a manual page for MPI_Comm

.seealso: WHALE_COMM_WORLD, WHALE_COMM_SELF
M*/

/*MC
    WhaleScalar - Whale type that represents either a double precision real number, a double precision
       complex number, a single precision real number, a long double or an int - if the code is configured
       with --with-scalar-type=real,complex --with-precision=single,double,longdouble,int,matsingle


   Level: beginner

.seealso: WhaleReal, PassiveReal, PassiveScalar, MPIU_SCALAR, WhaleInt
M*/

/*MC
    WhaleComplex - Whale type that represents a complex number with precision matching that of WhaleReal.

   Synopsis:
   #define WHALE_DESIRE_COMPLEX
   #include <whalesys.h>
   WhaleComplex number = 1. + 2.*WHALE_i;

   Level: beginner

   Note:
   Complex numbers are automatically available if Whale was configured --with-scalar-type=complex (in which case
   WhaleComplex will match WhaleScalar), otherwise the macro WHALE_DESIRE_COMPLEX must be defined before including any
   Whale headers. If the compiler supports complex numbers, WhaleComplex and associated variables and functions will be
   defined and WHALE_HAVE_COMPLEX will be set.

.seealso: WhaleReal, WhaleComplex, MPIU_COMPLEX, WhaleInt, WHALE_i
M*/

/*MC
    WhaleReal - Whale type that represents a real number version of WhaleScalar

   Level: beginner

.seealso: WhaleScalar, PassiveReal, PassiveScalar
M*/

/*MC
    PassiveScalar - Whale type that represents a WhaleScalar
   Level: beginner

    This is the same as a WhaleScalar except in code that is automatically differentiated it is
   treated as a constant (not an indendent or dependent variable)

.seealso: WhaleReal, PassiveReal, WhaleScalar
M*/

/*MC
    PassiveReal - Whale type that represents a WhaleReal

   Level: beginner

    This is the same as a WhaleReal except in code that is automatically differentiated it is
   treated as a constant (not an indendent or dependent variable)

.seealso: WhaleScalar, WhaleReal, PassiveScalar
M*/

/*MC
    MPIU_SCALAR - MPI datatype corresponding to WhaleScalar

   Level: beginner

    Note: In MPI calls that require an MPI datatype that matches a WhaleScalar or array of WhaleScalars
          pass this value

.seealso: WhaleReal, PassiveReal, PassiveScalar, WhaleScalar, MPIU_INT
M*/

#if defined(WHALE_HAVE_MPIIO)
#if !defined(WHALE_WORDS_BIGENDIAN)
WHALE_EXTERN WhaleErrorCode MPIU_File_write_all(MPI_File,void*,WhaleMPIInt,MPI_Datatype,MPI_Status*);
WHALE_EXTERN WhaleErrorCode MPIU_File_read_all(MPI_File,void*,WhaleMPIInt,MPI_Datatype,MPI_Status*);
#else
#define MPIU_File_write_all(a,b,c,d,e) MPI_File_write_all(a,b,c,d,e)
#define MPIU_File_read_all(a,b,c,d,e) MPI_File_read_all(a,b,c,d,e)
#endif
#endif

/* the following whale_static_inline require whaleerror.h */

/* Limit MPI to 32-bits */
#define WHALE_MPI_INT_MAX  2147483647
#define WHALE_MPI_INT_MIN -2147483647
/* Limit BLAS to 32-bits */
#define WHALE_BLAS_INT_MAX  2147483647
#define WHALE_BLAS_INT_MIN -2147483647

#undef __FUNCT__
#define __FUNCT__ "WhaleBLASIntCast"
WHALE_STATIC_INLINE WhaleErrorCode WhaleBLASIntCast(WhaleInt a,WhaleBLASInt *b)
{
  WhaleFunctionBegin;
#if defined(WHALE_USE_64BIT_INDICES) && !defined(WHALE_HAVE_64BIT_BLAS_INDICES)
  if ((a) > WHALE_BLAS_INT_MAX) SETERRQ(WHALE_COMM_SELF,WHALE_ERR_ARG_OUTOFRANGE,"Array too long for BLAS/LAPACK");
#endif
  *b =  (WhaleBLASInt)(a);
  WhaleFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "WhaleMPIIntCast"
WHALE_STATIC_INLINE WhaleErrorCode WhaleMPIIntCast(WhaleInt a,WhaleMPIInt *b)
{
  WhaleFunctionBegin;
#if defined(WHALE_USE_64BIT_INDICES)
  if ((a) > WHALE_MPI_INT_MAX) SETERRQ(WHALE_COMM_SELF,WHALE_ERR_ARG_OUTOFRANGE,"Array too long for MPI");
#endif
  *b =  (WhaleMPIInt)(a);
  WhaleFunctionReturn(0);
}


/*
     The IBM include files define hz, here we hide it so that it may be used as a regular user variable.
*/
#if defined(hz)
#undef hz
#endif

/*  For arrays that contain filenames or paths */


#if defined(WHALE_HAVE_LIMITS_H)
#include <limits.h>
#endif
#if defined(WHALE_HAVE_SYS_PARAM_H)
#include <sys/param.h>
#endif
#if defined(WHALE_HAVE_SYS_TYPES_H)
#include <sys/types.h>
#endif
#if defined(MAXPATHLEN)
#  define WHALE_MAX_PATH_LEN     MAXPATHLEN
#elif defined(MAX_PATH)
#  define WHALE_MAX_PATH_LEN     MAX_PATH
#elif defined(_MAX_PATH)
#  define WHALE_MAX_PATH_LEN     _MAX_PATH
#else
#  define WHALE_MAX_PATH_LEN     4096
#endif

/* Special support for C++ */
#if defined(WHALE_CLANGUAGE_CXX) && defined(__cplusplus)
#include <whalesys.hh>
#endif

/*MC

    UsingFortran - Fortran can be used with Whale in four distinct approaches

$    1) classic Fortran 77 style
$#include "finclude/whaleXXX.h" to work with material from the XXX component of Whale
$       XXX variablename
$      You cannot use this approach if you wish to use the Fortran 90 specific Whale routines
$      which end in F90; such as VecGetArrayF90()
$
$    2) classic Fortran 90 style
$#include "finclude/whaleXXX.h"
$#include "finclude/whaleXXX.h90" to work with material from the XXX component of Whale
$       XXX variablename
$
$    3) Using Fortran modules
$#include "finclude/whaleXXXdef.h"
$         use whaleXXXX
$       XXX variablename
$
$    4) Use Fortran modules and Fortran data types for Whale types
$#include "finclude/whaleXXXdef.h"
$         use whaleXXXX
$       type(XXX) variablename
$      To use this approach you must ./configure Whale with the additional
$      option --with-fortran-datatypes You cannot use the type(XXX) declaration approach without using Fortran modules

    Finally if you absolutely do not want to use any #include you can use either

$    3a) skip the #include BUT you cannot use any Whale data type names like Vec, Mat, WhaleInt, WhaleErrorCode etc
$        and you must declare the variables as integer, for example
$        integer variablename
$
$    4a) skip the #include, you use the object types like type(Vec) type(Mat) but cannot use the data type
$        names like WhaleErrorCode, WhaleInt etc. again for those you must use integer

   We recommend either 2 or 3. Approaches 2 and 3 provide type checking for most Whale function calls; 4 has type checking
for only a few Whale functions.

   Fortran type checking with interfaces is strick, this means you cannot pass a scalar value when an array value
is expected (even though it is legal Fortran). For example when setting a single value in a matrix with MatSetValues()
you cannot have something like
$      WhaleInt row,col
$      WhaleScalar val
$        ...
$      call MatSetValues(mat,1,row,1,col,val,INSERT_VALUES,ierr)
You must instead have
$      WhaleInt row(1),col(1)
$      WhaleScalar val(1)
$        ...
$      call MatSetValues(mat,1,row,1,col,val,INSERT_VALUES,ierr)


    See the example src/vec/vec/examples/tutorials/ex20f90.F90 for an example that can use all four approaches

    Developer Notes: The finclude/whaleXXXdef.h contain all the #defines (would be typedefs in C code) these
     automatically include their predecessors; for example finclude/whalevecdef.h includes finclude/whaleisdef.h

     The finclude/whaleXXXX.h contain all the parameter statements for that package. These automatically include
     their finclude/whaleXXXdef.h file but DO NOT automatically include their predecessors;  for example
     finclude/whalevec.h does NOT automatically include finclude/whaleis.h

     The finclude/ftn-custom/whaleXXXdef.h90 are not intended to be used directly in code, they define the
     Fortran data type type(XXX) (for example type(Vec)) when Whale is ./configure with the --with-fortran-datatypes option.

     The finclude/ftn-custom/whaleXXX.h90 (not included directly by code) contain interface definitions for
     the Whale Fortran stubs that have different bindings then their C version (for example VecGetArrayF90).

     The finclude/ftn-auto/whaleXXX.h90 (not included directly by code) contain interface definitions generated
     automatically by "make allfortranstubs".

     The finclude/whaleXXX.h90 includes the custom finclude/ftn-custom/whaleXXX.h90 and if ./configure
     was run with --with-fortran-interfaces it also includes the finclude/ftn-auto/whaleXXX.h90 These DO NOT automatically
     include their predecessors

    Level: beginner

M*/

WHALE_EXTERN WhaleErrorCode WhaleGetArchType(char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleGetHostName(char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleGetUserName(char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleGetProgramName(char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleSetProgramName(const char[]);
WHALE_EXTERN WhaleErrorCode WhaleGetDate(char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleGetVersion(char[], size_t);

WHALE_EXTERN WhaleErrorCode WhaleSortInt(WhaleInt,WhaleInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortRemoveDupsInt(WhaleInt*,WhaleInt[]);
WHALE_EXTERN WhaleErrorCode WhaleFindInt(WhaleInt, WhaleInt, const WhaleInt[], WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSortIntWithPermutation(WhaleInt,const WhaleInt[],WhaleInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortStrWithPermutation(WhaleInt,const char*[],WhaleInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortIntWithArray(WhaleInt,WhaleInt[],WhaleInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortIntWithArrayPair(WhaleInt,WhaleInt*,WhaleInt*,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSortMPIInt(WhaleInt,WhaleMPIInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortRemoveDupsMPIInt(WhaleInt*,WhaleMPIInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortMPIIntWithArray(WhaleMPIInt,WhaleMPIInt[],WhaleMPIInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortIntWithScalarArray(WhaleInt,WhaleInt[],WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode WhaleSortReal(WhaleInt,WhaleReal[]);
WHALE_EXTERN WhaleErrorCode WhaleSortRealWithPermutation(WhaleInt,const WhaleReal[],WhaleInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortSplit(WhaleInt,WhaleInt,WhaleScalar[],WhaleInt[]);
WHALE_EXTERN WhaleErrorCode WhaleSortSplitReal(WhaleInt,WhaleInt,WhaleReal[],WhaleInt[]);
WHALE_EXTERN WhaleErrorCode WhaleProcessTree(WhaleInt,const WhaleBool [],const WhaleInt[],WhaleInt*,WhaleInt**,WhaleInt**,WhaleInt**,WhaleInt**);
WHALE_EXTERN WhaleErrorCode WhaleMergeIntArrayPair(WhaleInt,const WhaleInt*,const WhaleInt*,WhaleInt,const WhaleInt*,const WhaleInt*,WhaleInt*,WhaleInt**,WhaleInt**);

WHALE_EXTERN WhaleErrorCode WhaleSetDisplay(void);
WHALE_EXTERN WhaleErrorCode WhaleGetDisplay(char[],size_t);

/*J
    WhaleRandomType - String with the name of a Whale randomizer

   Level: beginner

   Notes: to use the SPRNG you must have ./configure Whale
   with the option --download-sprng

.seealso: WhaleRandomSetType(), WhaleRandom, WhaleRandomCreate()
J*/
typedef const char* WhaleRandomType;
#define WHALERAND       "rand"
#define WHALERAND48     "rand48"
#define WHALESPRNG      "sprng"

/* Logging support */
WHALE_EXTERN WhaleClassId WHALE_RANDOM_CLASSID;

WHALE_EXTERN WhaleErrorCode WhaleRandomInitializePackage(void);

/*S
     WhaleRandom - Abstract Whale object that manages generating random numbers

   Level: intermediate

  Concepts: random numbers

.seealso:  WhaleRandomCreate(), WhaleRandomGetValue(), WhaleRandomType
S*/
typedef struct _p_WhaleRandom*   WhaleRandom;

/* Dynamic creation and loading functions */
WHALE_EXTERN WhaleFunctionList WhaleRandomList;
WHALE_EXTERN WhaleBool         WhaleRandomRegisterAllCalled;

WHALE_EXTERN WhaleErrorCode WhaleRandomRegisterAll(void);
WHALE_EXTERN WhaleErrorCode WhaleRandomRegister(const char[],WhaleErrorCode (*)(WhaleRandom));
WHALE_EXTERN WhaleErrorCode WhaleRandomSetType(WhaleRandom, WhaleRandomType);
WHALE_EXTERN WhaleErrorCode WhaleRandomSetFromOptions(WhaleRandom);
WHALE_EXTERN WhaleErrorCode WhaleRandomGetType(WhaleRandom, WhaleRandomType*);
 WHALE_EXTERN WhaleErrorCode WhaleRandomViewFromOptions(WhaleRandom,const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleRandomView(WhaleRandom,WhaleViewer);

WHALE_EXTERN WhaleErrorCode WhaleRandomCreate(MPI_Comm,WhaleRandom*);
WHALE_EXTERN WhaleErrorCode WhaleRandomGetValue(WhaleRandom,WhaleScalar*);
WHALE_EXTERN WhaleErrorCode WhaleRandomGetValueReal(WhaleRandom,WhaleReal*);
WHALE_EXTERN WhaleErrorCode WhaleRandomGetInterval(WhaleRandom,WhaleScalar*,WhaleScalar*);
WHALE_EXTERN WhaleErrorCode WhaleRandomSetInterval(WhaleRandom,WhaleScalar,WhaleScalar);
WHALE_EXTERN WhaleErrorCode WhaleRandomSetSeed(WhaleRandom,unsigned long);
WHALE_EXTERN WhaleErrorCode WhaleRandomGetSeed(WhaleRandom,unsigned long *);
WHALE_EXTERN WhaleErrorCode WhaleRandomSeed(WhaleRandom);
WHALE_EXTERN WhaleErrorCode WhaleRandomDestroy(WhaleRandom*);

WHALE_EXTERN WhaleErrorCode WhaleGetFullPath(const char[],char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleGetRelativePath(const char[],char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleGetWorkingDirectory(char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleGetRealPath(const char[],char[]);
WHALE_EXTERN WhaleErrorCode WhaleGetHomeDirectory(char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleTestFile(const char[],char,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleTestDirectory(const char[],char,WhaleBool *);

WHALE_EXTERN WhaleErrorCode WhaleBinaryRead(int,void*,WhaleInt,WhaleDataType);
WHALE_EXTERN WhaleErrorCode WhaleBinarySynchronizedRead(MPI_Comm,int,void*,WhaleInt,WhaleDataType);
WHALE_EXTERN WhaleErrorCode WhaleBinarySynchronizedWrite(MPI_Comm,int,void*,WhaleInt,WhaleDataType,WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleBinaryWrite(int,void*,WhaleInt,WhaleDataType,WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleBinaryOpen(const char[],WhaleFileMode,int *);
WHALE_EXTERN WhaleErrorCode WhaleBinaryClose(int);
WHALE_EXTERN WhaleErrorCode WhaleSharedTmp(MPI_Comm,WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleSharedWorkingDirectory(MPI_Comm,WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleGetTmp(MPI_Comm,char[],size_t);
WHALE_EXTERN WhaleErrorCode WhaleFileRetrieve(MPI_Comm,const char[],char[],size_t,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleLs(MPI_Comm,const char[],char[],size_t,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOpenSocket(char*,int,int*);
WHALE_EXTERN WhaleErrorCode WhaleWebServe(MPI_Comm,int);

/*
   In binary files variables are stored using the following lengths,
  regardless of how they are stored in memory on any one particular
  machine. Use these rather then sizeof() in computing sizes for
  WhaleBinarySeek().
*/
#define WHALE_BINARY_INT_SIZE   (32/8)
#define WHALE_BINARY_FLOAT_SIZE  (32/8)
#define WHALE_BINARY_CHAR_SIZE  (8/8)
#define WHALE_BINARY_SHORT_SIZE  (16/8)
#define WHALE_BINARY_DOUBLE_SIZE  (64/8)
#define WHALE_BINARY_SCALAR_SIZE  sizeof(WhaleScalar)

/*E
  WhaleBinarySeekType - argument to WhaleBinarySeek()

  Level: advanced

.seealso: WhaleBinarySeek(), WhaleBinarySynchronizedSeek()
E*/
typedef enum {WHALE_BINARY_SEEK_SET = 0,WHALE_BINARY_SEEK_CUR = 1,WHALE_BINARY_SEEK_END = 2} WhaleBinarySeekType;
WHALE_EXTERN WhaleErrorCode WhaleBinarySeek(int,off_t,WhaleBinarySeekType,off_t*);
WHALE_EXTERN WhaleErrorCode WhaleBinarySynchronizedSeek(MPI_Comm,int,off_t,WhaleBinarySeekType,off_t*);
WHALE_EXTERN WhaleErrorCode WhaleByteSwap(void *,WhaleDataType,WhaleInt);

WHALE_EXTERN WhaleErrorCode WhaleSetDebugTerminal(const char[]);
WHALE_EXTERN WhaleErrorCode WhaleSetDebugger(const char[],WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleSetDefaultDebugger(void);
WHALE_EXTERN WhaleErrorCode WhaleSetDebuggerFromString(const char*);
WHALE_EXTERN WhaleErrorCode WhaleAttachDebugger(void);
WHALE_EXTERN WhaleErrorCode WhaleStopForDebugger(void);

WHALE_EXTERN WhaleErrorCode WhaleGatherNumberOfMessages(MPI_Comm,const WhaleMPIInt[],const WhaleMPIInt[],WhaleMPIInt*);
WHALE_EXTERN WhaleErrorCode WhaleGatherMessageLengths(MPI_Comm,WhaleMPIInt,WhaleMPIInt,const WhaleMPIInt[],WhaleMPIInt**,WhaleMPIInt**);
WHALE_EXTERN WhaleErrorCode WhaleGatherMessageLengths2(MPI_Comm,WhaleMPIInt,WhaleMPIInt,const WhaleMPIInt[],const WhaleMPIInt[],WhaleMPIInt**,WhaleMPIInt**,WhaleMPIInt**);
WHALE_EXTERN WhaleErrorCode WhalePostIrecvInt(MPI_Comm,WhaleMPIInt,WhaleMPIInt,const WhaleMPIInt[],const WhaleMPIInt[],WhaleInt***,MPI_Request**);
WHALE_EXTERN WhaleErrorCode WhalePostIrecvScalar(MPI_Comm,WhaleMPIInt,WhaleMPIInt,const WhaleMPIInt[],const WhaleMPIInt[],WhaleScalar***,MPI_Request**);
WHALE_EXTERN WhaleErrorCode WhaleCommBuildTwoSided(MPI_Comm,WhaleMPIInt,MPI_Datatype,WhaleInt,const WhaleMPIInt*,const void*,WhaleInt*,WhaleMPIInt**,void*) WhaleAttrMPIPointerWithType(6,3);

/*E
    WhaleBuildTwoSidedType - algorithm for setting up two-sided communication

$  WHALE_BUILDTWOSIDED_ALLREDUCE - classical algorithm using an MPI_Allreduce with
$      a buffer of length equal to the communicator size. Not memory-scalable due to
$      the large reduction size. Requires only MPI-1.
$  WHALE_BUILDTWOSIDED_IBARRIER - nonblocking algorithm based on MPI_Issend and MPI_Ibarrier.
$      Proved communication-optimal in Hoefler, Siebert, and Lumsdaine (2010). Requires MPI-3.

   Level: developer

.seealso: WhaleCommBuildTwoSided(), WhaleCommBuildTwoSidedSetType(), WhaleCommBuildTwoSidedGetType()
E*/
typedef enum {
  WHALE_BUILDTWOSIDED_NOTSET = -1,
  WHALE_BUILDTWOSIDED_ALLREDUCE = 0,
  WHALE_BUILDTWOSIDED_IBARRIER = 1
  /* Updates here must be accompanied by updates in finclude/whalesys.h and the string array in mpits.c */
} WhaleBuildTwoSidedType;
WHALE_EXTERN const char *const WhaleBuildTwoSidedTypes[];
WHALE_EXTERN WhaleErrorCode WhaleCommBuildTwoSidedSetType(MPI_Comm,WhaleBuildTwoSidedType);
WHALE_EXTERN WhaleErrorCode WhaleCommBuildTwoSidedGetType(MPI_Comm,WhaleBuildTwoSidedType*);

WHALE_EXTERN WhaleErrorCode WhaleSSEIsEnabled(MPI_Comm,WhaleBool  *,WhaleBool  *);

/*E
  InsertMode - Whether entries are inserted or added into vectors or matrices

  Level: beginner

.seealso: VecSetValues(), MatSetValues(), VecSetValue(), VecSetValuesBlocked(),
          VecSetValuesLocal(), VecSetValuesBlockedLocal(), MatSetValuesBlocked(),
          MatSetValuesBlockedLocal(), MatSetValuesLocal(), VecScatterBegin(), VecScatterEnd()
E*/
 typedef enum {NOT_SET_VALUES, INSERT_VALUES, ADD_VALUES, MAX_VALUES, INSERT_ALL_VALUES, ADD_ALL_VALUES, INSERT_BC_VALUES, ADD_BC_VALUES} InsertMode;

/*MC
    INSERT_VALUES - Put a value into a vector or matrix, overwrites any previous value

    Level: beginner

.seealso: InsertMode, VecSetValues(), MatSetValues(), VecSetValue(), VecSetValuesBlocked(),
          VecSetValuesLocal(), VecSetValuesBlockedLocal(), MatSetValuesBlocked(), ADD_VALUES,
          MatSetValuesBlockedLocal(), MatSetValuesLocal(), VecScatterBegin(), VecScatterEnd(), MAX_VALUES

M*/

/*MC
    ADD_VALUES - Adds a value into a vector or matrix, if there previously was no value, just puts the
                value into that location

    Level: beginner

.seealso: InsertMode, VecSetValues(), MatSetValues(), VecSetValue(), VecSetValuesBlocked(),
          VecSetValuesLocal(), VecSetValuesBlockedLocal(), MatSetValuesBlocked(), INSERT_VALUES,
          MatSetValuesBlockedLocal(), MatSetValuesLocal(), VecScatterBegin(), VecScatterEnd(), MAX_VALUES

M*/

/*MC
    MAX_VALUES - Puts the maximum of the scattered/gathered value and the current value into each location

    Level: beginner

.seealso: InsertMode, VecScatterBegin(), VecScatterEnd(), ADD_VALUES, INSERT_VALUES

M*/

WHALE_EXTERN MPI_Comm WhaleObjectComm(WhaleObject);

/*S
   WhaleSubcomm - Context of MPI subcommunicators, used by PCREDUNDANT

   Level: advanced

   Concepts: communicator, create
S*/
typedef struct _n_WhaleSubcomm* WhaleSubcomm;

struct _n_WhaleSubcomm {
  MPI_Comm   parent;      /* parent communicator */
  MPI_Comm   dupparent;   /* duplicate parent communicator, under which the processors of this subcomm have contiguous rank */
  MPI_Comm   comm;        /* this communicator */
  WhaleInt   n;           /* num of subcommunicators under the parent communicator */
  WhaleInt   color;       /* color of processors belong to this communicator */
};

typedef enum {WHALE_SUBCOMM_GENERAL=0,WHALE_SUBCOMM_CONTIGUOUS=1,WHALE_SUBCOMM_INTERLACED=2} WhaleSubcommType;
WHALE_EXTERN const char *const WhaleSubcommTypes[];

WHALE_EXTERN WhaleErrorCode WhaleSubcommCreate(MPI_Comm,WhaleSubcomm*);
WHALE_EXTERN WhaleErrorCode WhaleSubcommDestroy(WhaleSubcomm*);
WHALE_EXTERN WhaleErrorCode WhaleSubcommSetNumber(WhaleSubcomm,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSubcommSetType(WhaleSubcomm,WhaleSubcommType);
WHALE_EXTERN WhaleErrorCode WhaleSubcommSetTypeGeneral(WhaleSubcomm,WhaleMPIInt,WhaleMPIInt,WhaleMPIInt);

/*S
   WhaleSegBuffer - a segmented extendable buffer

   Level: developer

.seealso: WhaleSegBufferCreate(), WhaleSegBufferGet(), WhaleSegBufferExtract(), WhaleSegBufferDestroy()
S*/
typedef struct _n_WhaleSegBuffer *WhaleSegBuffer;
WHALE_EXTERN WhaleErrorCode WhaleSegBufferCreate(size_t,size_t,WhaleSegBuffer*);
WHALE_EXTERN WhaleErrorCode WhaleSegBufferDestroy(WhaleSegBuffer*);
WHALE_EXTERN WhaleErrorCode WhaleSegBufferGet(WhaleSegBuffer,size_t,void*);
WHALE_EXTERN WhaleErrorCode WhaleSegBufferExtractAlloc(WhaleSegBuffer,void*);
WHALE_EXTERN WhaleErrorCode WhaleSegBufferExtractTo(WhaleSegBuffer,void*);
WHALE_EXTERN WhaleErrorCode WhaleSegBufferExtractInPlace(WhaleSegBuffer,void*);
WHALE_EXTERN WhaleErrorCode WhaleSegBufferGetSize(WhaleSegBuffer,size_t*);
WHALE_EXTERN WhaleErrorCode WhaleSegBufferUnuse(WhaleSegBuffer,size_t);

/* Type-safe wrapper to encourage use of WHALE_RESTRICT. Does not use WhaleFunctionBegin because the error handling
 * prevents the compiler from completely erasing the stub. This is called in inner loops so it has to be as fast as
 * possible. */
WHALE_STATIC_INLINE WhaleErrorCode WhaleSegBufferGetInts(WhaleSegBuffer seg,WhaleInt count,WhaleInt *WHALE_RESTRICT *slot) {return WhaleSegBufferGet(seg,count,(void**)slot);}

/* Reset __FUNCT__ in case the user does not define it themselves */
#undef __FUNCT__
#define __FUNCT__ "User provided function"

#endif
