#ifndef __WHALEVEC_H__
#define __WHALEVEC_H__

#include <whalesys.h>
#include <whaleis.h>
#include <whaleviewer.h>

/*
  Defines the vector component of Whale. Vectors generally represent
  degrees of freedom for finite element/finite difference functions
  on a grid. They have more mathematical structure then simple arrays.
*/

/*S
  Vec - Abstract Whale vector object

  Level: beginner

  Concepts: field variables, unknowns, arrays

  .seealso:  VecCreate(), VecType, VecSetType()
  S*/
typedef struct _whale_vector* Vec;

/*S
  VecScatter - Object used to manage communication of data
  between vectors in parallel. Manages both scatters and gathers

  Level: beginner

  Concepts: scatter

  .seealso:  VecScatterCreate(), VecScatterBegin(), VecScatterEnd()
  S*/
typedef struct _whale_scatter* VecScatter;

/*E
  ScatterMode - Determines the direction of a scatter

  Level: beginner

  .seealso: VecScatter, VecScatterBegin(), VecScatterEnd()
  E*/
typedef enum {SCATTER_FORWARD=0, SCATTER_REVERSE=1, SCATTER_FORWARD_LOCAL=2, SCATTER_REVERSE_LOCAL=3, SCATTER_LOCAL=2} ScatterMode;

/*MC
  SCATTER_FORWARD - Scatters the values as dictated by the VecScatterCreate() call

  Level: beginner

  .seealso: VecScatter, ScatterMode, VecScatterCreate(), VecScatterBegin(), VecScatterEnd(), SCATTER_REVERSE, SCATTER_FORWARD_LOCAL,
  SCATTER_REVERSE_LOCAL

  M*/

/*MC
  SCATTER_REVERSE - Moves the values in the opposite direction then the directions indicated in
  in the VecScatterCreate()

  Level: beginner

  .seealso: VecScatter, ScatterMode, VecScatterCreate(), VecScatterBegin(), VecScatterEnd(), SCATTER_FORWARD, SCATTER_FORWARD_LOCAL,
  SCATTER_REVERSE_LOCAL

  M*/

/*MC
  SCATTER_FORWARD_LOCAL - Scatters the values as dictated by the VecScatterCreate() call except NO parallel communication
  is done. Any variables that have be moved between processes are ignored

  Level: developer

  .seealso: VecScatter, ScatterMode, VecScatterCreate(), VecScatterBegin(), VecScatterEnd(), SCATTER_REVERSE, SCATTER_FORWARD,
  SCATTER_REVERSE_LOCAL

  M*/

/*MC
  SCATTER_REVERSE_LOCAL - Moves the values in the opposite direction then the directions indicated in
  in the VecScatterCreate()  except NO parallel communication
  is done. Any variables that have be moved between processes are ignored

  Level: developer

  .seealso: VecScatter, ScatterMode, VecScatterCreate(), VecScatterBegin(), VecScatterEnd(), SCATTER_FORWARD, SCATTER_FORWARD_LOCAL,
  SCATTER_REVERSE

  M*/

/*J
  VecType - String with the name of a Whale vector

  Level: beginner

  .seealso: VecSetType(), Vec, VecCreate(), VecDestroy()
  J*/
typedef const char* VecType;
#define VECSEQ         "seq"
#define VECMPI         "mpi"
#define VECSTANDARD    "standard"   /* seq on one process and mpi on several */
#define VECSHARED      "shared"
#define VECSIEVE       "sieve"
#define VECSEQCUSP     "seqcusp"
#define VECMPICUSP     "mpicusp"
#define VECCUSP        "cusp"       /* seqcusp on one process and mpicusp on several */
#define VECNEST        "nest"
#define VECSEQPTHREAD  "seqpthread"
#define VECMPIPTHREAD  "mpipthread"
#define VECPTHREAD     "pthread"    /* seqpthread on one process and mpipthread on several */


/* Logging support */
#define    VEC_FILE_CLASSID 1211214
WHALE_EXTERN WhaleClassId VEC_CLASSID;
WHALE_EXTERN WhaleClassId VEC_SCATTER_CLASSID;


WHALE_EXTERN WhaleErrorCode VecInitializePackage(void);
WHALE_EXTERN WhaleErrorCode VecFinalizePackage(void);

WHALE_EXTERN WhaleErrorCode VecCreate(MPI_Comm,Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateSeq(MPI_Comm,WhaleInt,Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateMPI(MPI_Comm,WhaleInt,WhaleInt,Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateSeqWithArray(MPI_Comm,WhaleInt,WhaleInt,const WhaleScalar[],Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateMPIWithArray(MPI_Comm,WhaleInt,WhaleInt,WhaleInt,const WhaleScalar[],Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateShared(MPI_Comm,WhaleInt,WhaleInt,Vec*);
WHALE_EXTERN WhaleErrorCode VecSetFromOptions(Vec);
WHALE_EXTERN WhaleErrorCode VecViewFromOptions(Vec,const char[],const char[]);

WHALE_EXTERN WhaleErrorCode VecSetUp(Vec);
WHALE_EXTERN WhaleErrorCode VecDestroy(Vec*);
WHALE_EXTERN WhaleErrorCode VecZeroEntries(Vec);
WHALE_EXTERN WhaleErrorCode VecSetOptionsPrefix(Vec,const char[]);
WHALE_EXTERN WhaleErrorCode VecAppendOptionsPrefix(Vec,const char[]);
WHALE_EXTERN WhaleErrorCode VecGetOptionsPrefix(Vec,const char*[]);

WHALE_EXTERN WhaleErrorCode VecSetSizes(Vec,WhaleInt,WhaleInt);

WHALE_EXTERN WhaleErrorCode VecDotNorm2(Vec,Vec,WhaleScalar*,WhaleReal*);
WHALE_EXTERN WhaleErrorCode VecDot(Vec,Vec,WhaleScalar*);
WHALE_EXTERN WhaleErrorCode VecDotRealPart(Vec,Vec,WhaleReal*);
WHALE_EXTERN WhaleErrorCode VecTDot(Vec,Vec,WhaleScalar*);
WHALE_EXTERN WhaleErrorCode VecMDot(Vec,WhaleInt,const Vec[],WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode VecMTDot(Vec,WhaleInt,const Vec[],WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode VecGetSubVector(Vec,IS,Vec*);
WHALE_EXTERN WhaleErrorCode VecRestoreSubVector(Vec,IS,Vec*);

/*E
  NormType - determines what type of norm to compute

  Level: beginner

  .seealso: VecNorm(), VecNormBegin(), VecNormEnd(), MatNorm()
  E*/
typedef enum {NORM_1=0,NORM_2=1,NORM_FROBENIUS=2,NORM_INFINITY=3,NORM_1_AND_2=4} NormType;
WHALE_EXTERN const char *const NormTypes[];
#define NORM_MAX NORM_INFINITY

/*MC
  NORM_1 - the one norm, ||v|| = sum_i | v_i |. ||A|| = max_j || v_*j ||, maximum column sum

  Level: beginner

  .seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_2, NORM_FROBENIUS,
  NORM_INFINITY, NORM_1_AND_2

  M*/

/*MC
  NORM_2 - the two norm, ||v|| = sqrt(sum_i (v_i)^2) (vectors only)

  Level: beginner

  .seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_1, NORM_FROBENIUS,
  NORM_INFINITY, NORM_1_AND_2

  M*/

/*MC
  NORM_FROBENIUS - ||A|| = sqrt(sum_ij (A_ij)^2), same as NORM_2 for vectors

  Level: beginner

  .seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_1, NORM_2,
  NORM_INFINITY, NORM_1_AND_2

  M*/

/*MC
  NORM_INFINITY - ||v|| = max_i |v_i|. ||A|| = max_i || v_i* ||, maximum row sum

  Level: beginner

  .seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_1, NORM_2,
  NORM_FROBINIUS, NORM_1_AND_2

  M*/

/*MC
  NORM_1_AND_2 - computes both the 1 and 2 norm of a vector

  Level: beginner

  .seealso:  NormType, MatNorm(), VecNorm(), VecNormBegin(), VecNormEnd(), NORM_1, NORM_2,
  NORM_FROBINIUS, NORM_INFINITY

  M*/

/*MC
  NORM_MAX - see NORM_INFINITY

  Level: beginner

  M*/

WHALE_EXTERN WhaleErrorCode VecNorm(Vec,NormType,WhaleReal *);
WHALE_EXTERN WhaleErrorCode VecNormAvailable(Vec,NormType,WhaleBool *,WhaleReal *);
WHALE_EXTERN WhaleErrorCode VecNormalize(Vec,WhaleReal *);
WHALE_EXTERN WhaleErrorCode VecSum(Vec,WhaleScalar*);
WHALE_EXTERN WhaleErrorCode VecMax(Vec,WhaleInt*,WhaleReal *);
WHALE_EXTERN WhaleErrorCode VecMin(Vec,WhaleInt*,WhaleReal *);
WHALE_EXTERN WhaleErrorCode VecScale(Vec,WhaleScalar);
WHALE_EXTERN WhaleErrorCode VecCopy(Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecSetRandom(Vec,WhaleRandom);
WHALE_EXTERN WhaleErrorCode VecSet(Vec,WhaleScalar);
WHALE_EXTERN WhaleErrorCode VecSwap(Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecAXPY(Vec,WhaleScalar,Vec);
WHALE_EXTERN WhaleErrorCode VecAXPBY(Vec,WhaleScalar,WhaleScalar,Vec);
WHALE_EXTERN WhaleErrorCode VecMAXPY(Vec,WhaleInt,const WhaleScalar[],Vec[]);
WHALE_EXTERN WhaleErrorCode VecAYPX(Vec,WhaleScalar,Vec);
WHALE_EXTERN WhaleErrorCode VecWAXPY(Vec,WhaleScalar,Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecAXPBYPCZ(Vec,WhaleScalar,WhaleScalar,WhaleScalar,Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecPointwiseMax(Vec,Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecPointwiseMaxAbs(Vec,Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecPointwiseMin(Vec,Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecPointwiseMult(Vec,Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecPointwiseDivide(Vec,Vec,Vec);
WHALE_EXTERN WhaleErrorCode VecMaxPointwiseDivide(Vec,Vec,WhaleReal*);
WHALE_EXTERN WhaleErrorCode VecShift(Vec,WhaleScalar);
WHALE_EXTERN WhaleErrorCode VecReciprocal(Vec);
WHALE_EXTERN WhaleErrorCode VecPermute(Vec, IS, WhaleBool );
WHALE_EXTERN WhaleErrorCode VecSqrtAbs(Vec);
WHALE_EXTERN WhaleErrorCode VecLog(Vec);
WHALE_EXTERN WhaleErrorCode VecExp(Vec);
WHALE_EXTERN WhaleErrorCode VecAbs(Vec);
WHALE_EXTERN WhaleErrorCode VecDuplicate(Vec,Vec*);
WHALE_EXTERN WhaleErrorCode VecDuplicateVecs(Vec,WhaleInt,Vec*[]);
WHALE_EXTERN WhaleErrorCode VecDestroyVecs(WhaleInt, Vec*[]);
WHALE_EXTERN WhaleErrorCode VecStrideNormAll(Vec,NormType,WhaleReal[]);
WHALE_EXTERN WhaleErrorCode VecStrideMaxAll(Vec,WhaleInt [],WhaleReal []);
WHALE_EXTERN WhaleErrorCode VecStrideMinAll(Vec,WhaleInt [],WhaleReal []);
WHALE_EXTERN WhaleErrorCode VecStrideScaleAll(Vec,const WhaleScalar[]);

WHALE_EXTERN WhaleErrorCode VecStrideNorm(Vec,WhaleInt,NormType,WhaleReal*);
WHALE_EXTERN WhaleErrorCode VecStrideMax(Vec,WhaleInt,WhaleInt *,WhaleReal *);
WHALE_EXTERN WhaleErrorCode VecStrideMin(Vec,WhaleInt,WhaleInt *,WhaleReal *);
WHALE_EXTERN WhaleErrorCode VecStrideScale(Vec,WhaleInt,WhaleScalar);
WHALE_EXTERN WhaleErrorCode VecStrideSet(Vec,WhaleInt,WhaleScalar);


WHALE_EXTERN WhaleErrorCode VecStrideGather(Vec,WhaleInt,Vec,InsertMode);
WHALE_EXTERN WhaleErrorCode VecStrideScatter(Vec,WhaleInt,Vec,InsertMode);
WHALE_EXTERN WhaleErrorCode VecStrideGatherAll(Vec,Vec[],InsertMode);
WHALE_EXTERN WhaleErrorCode VecStrideScatterAll(Vec[],Vec,InsertMode);

WHALE_EXTERN WhaleErrorCode VecSetValues(Vec,WhaleInt,const WhaleInt[],const WhaleScalar[],InsertMode);
WHALE_EXTERN WhaleErrorCode VecGetValues(Vec,WhaleInt,const WhaleInt[],WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode VecAssemblyBegin(Vec);
WHALE_EXTERN WhaleErrorCode VecAssemblyEnd(Vec);
WHALE_EXTERN WhaleErrorCode VecStashSetInitialSize(Vec,WhaleInt,WhaleInt);
WHALE_EXTERN WhaleErrorCode VecStashView(Vec,WhaleViewer);
WHALE_EXTERN WhaleErrorCode VecStashGetInfo(Vec,WhaleInt*,WhaleInt*,WhaleInt*,WhaleInt*);

/*MC
  VecSetValue - Set a single entry into a vector.

  Synopsis:
  #include "whalevec.h"
  WhaleErrorCode VecSetValue(Vec v,WhaleInt row,WhaleScalar value, InsertMode mode);

  Not Collective

  Input Parameters:
  +  v - the vector
  .  row - the row location of the entry
  .  value - the value to insert
  -  mode - either INSERT_VALUES or ADD_VALUES

  Notes:
  For efficiency one should use VecSetValues() and set several or
  many values simultaneously if possible.

  These values may be cached, so VecAssemblyBegin() and VecAssemblyEnd()
  MUST be called after all calls to VecSetValues() have been completed.

  VecSetValues() uses 0-based indices in Fortran as well as in C.

  Level: beginner

  .seealso: VecSetValues(), VecAssemblyBegin(), VecAssemblyEnd(), VecSetValuesBlockedLocal(), VecSetValueLocal()
  M*/
WHALE_STATIC_INLINE WhaleErrorCode VecSetValue(Vec v,WhaleInt i,WhaleScalar va,InsertMode mode) {return VecSetValues(v,1,&i,&va,mode);}


WHALE_EXTERN WhaleErrorCode VecSetBlockSize(Vec,WhaleInt);
WHALE_EXTERN WhaleErrorCode VecGetBlockSize(Vec,WhaleInt*);
WHALE_EXTERN WhaleErrorCode VecSetValuesBlocked(Vec,WhaleInt,const WhaleInt[],const WhaleScalar[],InsertMode);

/* Dynamic creation and loading functions */
WHALE_EXTERN WhaleFunctionList VecList;
WHALE_EXTERN WhaleBool         VecRegisterAllCalled;
WHALE_EXTERN WhaleErrorCode VecSetType(Vec, VecType);
WHALE_EXTERN WhaleErrorCode VecGetType(Vec, VecType *);
WHALE_EXTERN WhaleErrorCode VecRegister(const char[],WhaleErrorCode (*)(Vec));
WHALE_EXTERN WhaleErrorCode VecRegisterAll(void);

WHALE_EXTERN WhaleErrorCode VecScatterCreate(Vec,IS,Vec,IS,VecScatter *);
WHALE_EXTERN WhaleErrorCode VecScatterCreateEmpty(MPI_Comm,VecScatter *);
WHALE_EXTERN WhaleErrorCode VecScatterCreateLocal(VecScatter,WhaleInt,const WhaleInt[],const WhaleInt[],const WhaleInt[],WhaleInt,const WhaleInt[],const WhaleInt[],const WhaleInt[],WhaleInt);
WHALE_EXTERN WhaleErrorCode VecScatterBegin(VecScatter,Vec,Vec,InsertMode,ScatterMode);
WHALE_EXTERN WhaleErrorCode VecScatterEnd(VecScatter,Vec,Vec,InsertMode,ScatterMode);
WHALE_EXTERN WhaleErrorCode VecScatterDestroy(VecScatter*);
WHALE_EXTERN WhaleErrorCode VecScatterCopy(VecScatter,VecScatter *);
WHALE_EXTERN WhaleErrorCode VecScatterView(VecScatter,WhaleViewer);
WHALE_EXTERN WhaleErrorCode VecScatterRemap(VecScatter,WhaleInt *,WhaleInt*);
WHALE_EXTERN WhaleErrorCode VecScatterGetMerged(VecScatter,WhaleBool *);

WHALE_EXTERN WhaleErrorCode VecGetArray4d(Vec,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleScalar****[]);
WHALE_EXTERN WhaleErrorCode VecRestoreArray4d(Vec,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleScalar****[]);
WHALE_EXTERN WhaleErrorCode VecGetArray3d(Vec,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleScalar***[]);
WHALE_EXTERN WhaleErrorCode VecRestoreArray3d(Vec,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleScalar***[]);
WHALE_EXTERN WhaleErrorCode VecGetArray2d(Vec,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleScalar**[]);
WHALE_EXTERN WhaleErrorCode VecRestoreArray2d(Vec,WhaleInt,WhaleInt,WhaleInt,WhaleInt,WhaleScalar**[]);
WHALE_EXTERN WhaleErrorCode VecGetArray1d(Vec,WhaleInt,WhaleInt,WhaleScalar *[]);
WHALE_EXTERN WhaleErrorCode VecRestoreArray1d(Vec,WhaleInt,WhaleInt,WhaleScalar *[]);

WHALE_EXTERN WhaleErrorCode VecPlaceArray(Vec,const WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode VecResetArray(Vec);
WHALE_EXTERN WhaleErrorCode VecReplaceArray(Vec,const WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode VecGetArrays(const Vec[],WhaleInt,WhaleScalar**[]);
WHALE_EXTERN WhaleErrorCode VecRestoreArrays(const Vec[],WhaleInt,WhaleScalar**[]);

WHALE_EXTERN WhaleErrorCode VecView(Vec,WhaleViewer);
WHALE_EXTERN WhaleErrorCode VecEqual(Vec,Vec,WhaleBool *);
WHALE_EXTERN WhaleErrorCode VecLoad(Vec, WhaleViewer);

WHALE_EXTERN WhaleErrorCode VecGetSize(Vec,WhaleInt*);
WHALE_EXTERN WhaleErrorCode VecGetLocalSize(Vec,WhaleInt*);
WHALE_EXTERN WhaleErrorCode VecGetOwnershipRange(Vec,WhaleInt*,WhaleInt*);
WHALE_EXTERN WhaleErrorCode VecGetOwnershipRanges(Vec,const WhaleInt *[]);

WHALE_EXTERN WhaleErrorCode VecSetLocalToGlobalMapping(Vec,ISLocalToGlobalMapping);
WHALE_EXTERN WhaleErrorCode VecSetValuesLocal(Vec,WhaleInt,const WhaleInt[],const WhaleScalar[],InsertMode);

/*MC
  VecSetValueLocal - Set a single entry into a vector using the local numbering

  Synopsis:
  #include "whalevec.h"
  WhaleErrorCode VecSetValueLocal(Vec v,WhaleInt row,WhaleScalar value, InsertMode mode);

  Not Collective

  Input Parameters:
  +  v - the vector
  .  row - the row location of the entry
  .  value - the value to insert
  -  mode - either INSERT_VALUES or ADD_VALUES

  Notes:
  For efficiency one should use VecSetValues() and set several or
  many values simultaneously if possible.

  These values may be cached, so VecAssemblyBegin() and VecAssemblyEnd()
  MUST be called after all calls to VecSetValues() have been completed.

  VecSetValues() uses 0-based indices in Fortran as well as in C.

  Level: beginner

  .seealso: VecSetValues(), VecAssemblyBegin(), VecAssemblyEnd(), VecSetValuesBlockedLocal(), VecSetValue()
  M*/
WHALE_STATIC_INLINE WhaleErrorCode VecSetValueLocal(Vec v,WhaleInt i,WhaleScalar va,InsertMode mode) {return VecSetValuesLocal(v,1,&i,&va,mode);}

WHALE_EXTERN WhaleErrorCode VecSetLocalToGlobalMappingBlock(Vec,ISLocalToGlobalMapping);
WHALE_EXTERN WhaleErrorCode VecSetValuesBlockedLocal(Vec,WhaleInt,const WhaleInt[],const WhaleScalar[],InsertMode);
WHALE_EXTERN WhaleErrorCode VecGetLocalToGlobalMappingBlock(Vec,ISLocalToGlobalMapping*);
WHALE_EXTERN WhaleErrorCode VecGetLocalToGlobalMapping(Vec,ISLocalToGlobalMapping*);

WHALE_EXTERN WhaleErrorCode VecDotBegin(Vec,Vec,WhaleScalar *);
WHALE_EXTERN WhaleErrorCode VecDotEnd(Vec,Vec,WhaleScalar *);
WHALE_EXTERN WhaleErrorCode VecTDotBegin(Vec,Vec,WhaleScalar *);
WHALE_EXTERN WhaleErrorCode VecTDotEnd(Vec,Vec,WhaleScalar *);
WHALE_EXTERN WhaleErrorCode VecNormBegin(Vec,NormType,WhaleReal *);
WHALE_EXTERN WhaleErrorCode VecNormEnd(Vec,NormType,WhaleReal *);

WHALE_EXTERN WhaleErrorCode VecMDotBegin(Vec,WhaleInt,const Vec[],WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode VecMDotEnd(Vec,WhaleInt,const Vec[],WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode VecMTDotBegin(Vec,WhaleInt,const Vec[],WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode VecMTDotEnd(Vec,WhaleInt,const Vec[],WhaleScalar[]);
WHALE_EXTERN WhaleErrorCode WhaleCommSplitReductionBegin(MPI_Comm);


typedef enum {VEC_IGNORE_OFF_PROC_ENTRIES,VEC_IGNORE_NEGATIVE_INDICES} VecOption;
WHALE_EXTERN WhaleErrorCode VecSetOption(Vec,VecOption,WhaleBool );

WHALE_EXTERN WhaleErrorCode VecGetArray(Vec,WhaleScalar**);
WHALE_EXTERN WhaleErrorCode VecGetArrayRead(Vec,const WhaleScalar**);
WHALE_EXTERN WhaleErrorCode VecRestoreArray(Vec,WhaleScalar**);
WHALE_EXTERN WhaleErrorCode VecRestoreArrayRead(Vec,const WhaleScalar**);

WHALE_EXTERN WhaleErrorCode VecValidValues(Vec,WhaleInt,WhaleBool);

WHALE_EXTERN WhaleErrorCode VecContourScale(Vec,WhaleReal,WhaleReal);

/*
  These numbers need to match the entries in
  the function table in vecimpl.h
*/
typedef enum { VECOP_VIEW = 33, VECOP_LOAD = 41, VECOP_DUPLICATE = 0} VecOperation;
WHALE_EXTERN WhaleErrorCode VecSetOperation(Vec,VecOperation,void(*)(void));

/*
  Routines for dealing with ghosted vectors:
  vectors with ghost elements at the end of the array.
*/
WHALE_EXTERN WhaleErrorCode VecMPISetGhost(Vec,WhaleInt,const WhaleInt[]);
WHALE_EXTERN WhaleErrorCode VecCreateGhost(MPI_Comm,WhaleInt,WhaleInt,WhaleInt,const WhaleInt[],Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateGhostWithArray(MPI_Comm,WhaleInt,WhaleInt,WhaleInt,const WhaleInt[],const WhaleScalar[],Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateGhostBlock(MPI_Comm,WhaleInt,WhaleInt,WhaleInt,WhaleInt,const WhaleInt[],Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateGhostBlockWithArray(MPI_Comm,WhaleInt,WhaleInt,WhaleInt,WhaleInt,const WhaleInt[],const WhaleScalar[],Vec*);
WHALE_EXTERN WhaleErrorCode VecGhostGetLocalForm(Vec,Vec*);
WHALE_EXTERN WhaleErrorCode VecGhostRestoreLocalForm(Vec,Vec*);
WHALE_EXTERN WhaleErrorCode VecGhostIsLocalForm(Vec,Vec,WhaleBool*);
WHALE_EXTERN WhaleErrorCode VecGhostUpdateBegin(Vec,InsertMode,ScatterMode);
WHALE_EXTERN WhaleErrorCode VecGhostUpdateEnd(Vec,InsertMode,ScatterMode);

WHALE_EXTERN WhaleErrorCode VecConjugate(Vec);

WHALE_EXTERN WhaleErrorCode VecScatterCreateToAll(Vec,VecScatter*,Vec*);
WHALE_EXTERN WhaleErrorCode VecScatterCreateToZero(Vec,VecScatter*,Vec*);

WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaGetVector(WhaleViewer, Vec);
WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaPutVector(WhaleViewer, Vec);

/*S
  Vecs - Collection of vectors where the data for the vectors is stored in
  one contiguous memory

  Level: advanced

  Notes:
  Temporary construct for handling multiply right hand side solves

  This is faked by storing a single vector that has enough array space for
  n vectors

  Concepts: parallel decomposition

  S*/
struct _n_Vecs  {WhaleInt n; Vec v;};
typedef struct _n_Vecs* Vecs;
WHALE_EXTERN WhaleErrorCode VecsDestroy(Vecs);
WHALE_EXTERN WhaleErrorCode VecsCreateSeq(MPI_Comm,WhaleInt,WhaleInt,Vecs*);
WHALE_EXTERN WhaleErrorCode VecsCreateSeqWithArray(MPI_Comm,WhaleInt,WhaleInt,WhaleScalar*,Vecs*);
WHALE_EXTERN WhaleErrorCode VecsDuplicate(Vecs,Vecs*);

#if defined(WHALE_HAVE_CUSP)
typedef struct _p_whaleCUSPIndices* WhaleCUSPIndices;
WHALE_EXTERN WhaleErrorCode WhaleCUSPIndicesCreate(WhaleInt, WhaleInt*,WhaleInt, WhaleInt*,WhaleCUSPIndices*);
WHALE_EXTERN WhaleErrorCode WhaleCUSPIndicesDestroy(WhaleCUSPIndices*);
WHALE_EXTERN WhaleErrorCode VecCUSPCopyToGPUSome_Public(Vec,WhaleCUSPIndices);
WHALE_EXTERN WhaleErrorCode VecCUSPCopyFromGPUSome_Public(Vec,WhaleCUSPIndices);

#if defined(WHALE_HAVE_TXWHALEGPU)
WHALE_EXTERN WhaleErrorCode VecCUSPResetIndexBuffersFlagsGPU_Public(WhaleCUSPIndices);
WHALE_EXTERN WhaleErrorCode VecCUSPCopySomeToContiguousBufferGPU_Public(Vec,WhaleCUSPIndices);
WHALE_EXTERN WhaleErrorCode VecCUSPCopySomeFromContiguousBufferGPU_Public(Vec,WhaleCUSPIndices);
WHALE_EXTERN WhaleErrorCode VecScatterInitializeForGPU(VecScatter,Vec,ScatterMode);
WHALE_EXTERN WhaleErrorCode VecScatterFinalizeForGPU(VecScatter);
#endif

WHALE_EXTERN WhaleErrorCode VecCreateSeqCUSP(MPI_Comm,WhaleInt,Vec*);
WHALE_EXTERN WhaleErrorCode VecCreateMPICUSP(MPI_Comm,WhaleInt,WhaleInt,Vec*);
#endif

WHALE_EXTERN WhaleErrorCode VecNestGetSubVecs(Vec,WhaleInt*,Vec**);
WHALE_EXTERN WhaleErrorCode VecNestGetSubVec(Vec,WhaleInt,Vec*);
WHALE_EXTERN WhaleErrorCode VecNestSetSubVecs(Vec,WhaleInt,WhaleInt*,Vec*);
WHALE_EXTERN WhaleErrorCode VecNestSetSubVec(Vec,WhaleInt,Vec);
WHALE_EXTERN WhaleErrorCode VecCreateNest(MPI_Comm,WhaleInt,IS*,Vec*,Vec*);
WHALE_EXTERN WhaleErrorCode VecNestGetSize(Vec,WhaleInt*);

WHALE_EXTERN WhaleErrorCode WhaleOptionsVec(const char[],const char[],const char[],Vec,WhaleBool*);
WHALE_EXTERN WhaleErrorCode VecChop(Vec,WhaleReal);

WHALE_EXTERN WhaleErrorCode VecGetLayout(Vec,WhaleLayout*);
WHALE_EXTERN WhaleErrorCode VecSetLayout(Vec,WhaleLayout);

WHALE_EXTERN WhaleErrorCode WhaleSectionVecView(WhaleSection, Vec, WhaleViewer);
WHALE_EXTERN WhaleErrorCode VecGetValuesSection(Vec, WhaleSection, WhaleInt, WhaleScalar **);
WHALE_EXTERN WhaleErrorCode VecSetValuesSection(Vec, WhaleSection, WhaleInt, WhaleScalar [], InsertMode);

#endif

