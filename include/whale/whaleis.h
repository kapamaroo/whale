/*
   An index set is a generalization of a subset of integers.  Index sets
   are used for defining scatters and gathers.
*/
#if !defined(__WHALEIS_H)
#define __WHALEIS_H
#include <whalesys.h>
#include <whaleviewertypes.h>
#include <whalesftypes.h>

#define IS_FILE_CLASSID 1211218
WHALE_EXTERN WhaleClassId IS_CLASSID;

WHALE_EXTERN WhaleErrorCode ISInitializePackage(void);

/*S
     IS - Abstract Whale object that allows indexing.

   Level: beginner

  Concepts: indexing, stride

.seealso:  ISCreateGeneral(), ISCreateBlock(), ISCreateStride(), ISGetIndices(), ISDestroy()
S*/
typedef struct _p_IS* IS;

/*J
    ISType - String with the name of a Whale index set type

   Level: beginner

.seealso: ISSetType(), IS, ISCreate(), ISRegister()
J*/
typedef const char* ISType;
#define ISGENERAL      "general"
#define ISSTRIDE       "stride"
#define ISBLOCK        "block"

/* Dynamic creation and loading functions */
WHALE_EXTERN WhaleFunctionList ISList;
WHALE_EXTERN WhaleBool         ISRegisterAllCalled;
WHALE_EXTERN WhaleErrorCode ISSetType(IS, ISType);
WHALE_EXTERN WhaleErrorCode ISGetType(IS, ISType *);
WHALE_EXTERN WhaleErrorCode ISRegister(const char[],WhaleErrorCode (*)(IS));
WHALE_EXTERN WhaleErrorCode ISRegisterAll(void);
WHALE_EXTERN WhaleErrorCode ISCreate(MPI_Comm,IS*);

/*
    Default index set data structures that Whale provides.
*/
WHALE_EXTERN WhaleErrorCode ISCreateGeneral(MPI_Comm,WhaleInt,const WhaleInt[],WhaleCopyMode,IS *);
WHALE_EXTERN WhaleErrorCode ISGeneralSetIndices(IS,WhaleInt,const WhaleInt[],WhaleCopyMode);
WHALE_EXTERN WhaleErrorCode ISCreateBlock(MPI_Comm,WhaleInt,WhaleInt,const WhaleInt[],WhaleCopyMode,IS *);
WHALE_EXTERN WhaleErrorCode ISBlockSetIndices(IS,WhaleInt,WhaleInt,const WhaleInt[],WhaleCopyMode);
WHALE_EXTERN WhaleErrorCode ISCreateStride(MPI_Comm,WhaleInt,WhaleInt,WhaleInt,IS *);
WHALE_EXTERN WhaleErrorCode ISStrideSetStride(IS,WhaleInt,WhaleInt,WhaleInt);

WHALE_EXTERN WhaleErrorCode ISDestroy(IS*);
WHALE_EXTERN WhaleErrorCode ISSetPermutation(IS);
WHALE_EXTERN WhaleErrorCode ISPermutation(IS,WhaleBool *);
WHALE_EXTERN WhaleErrorCode ISSetIdentity(IS);
WHALE_EXTERN WhaleErrorCode ISIdentity(IS,WhaleBool *);
WHALE_EXTERN WhaleErrorCode ISContiguousLocal(IS,WhaleInt,WhaleInt,WhaleInt*,WhaleBool*);

WHALE_EXTERN WhaleErrorCode ISGetIndices(IS,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode ISRestoreIndices(IS,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode ISGetTotalIndices(IS,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode ISRestoreTotalIndices(IS,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode ISGetNonlocalIndices(IS,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode ISRestoreNonlocalIndices(IS,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode ISGetNonlocalIS(IS, IS *is);
WHALE_EXTERN WhaleErrorCode ISRestoreNonlocalIS(IS, IS *is);
WHALE_EXTERN WhaleErrorCode ISGetSize(IS,WhaleInt *);
WHALE_EXTERN WhaleErrorCode ISGetLocalSize(IS,WhaleInt *);
WHALE_EXTERN WhaleErrorCode ISInvertPermutation(IS,WhaleInt,IS*);
WHALE_EXTERN WhaleErrorCode ISView(IS,WhaleViewer);
WHALE_EXTERN WhaleErrorCode ISEqual(IS,IS,WhaleBool  *);
WHALE_EXTERN WhaleErrorCode ISSort(IS);
WHALE_EXTERN WhaleErrorCode ISSorted(IS,WhaleBool  *);
WHALE_EXTERN WhaleErrorCode ISDifference(IS,IS,IS*);
WHALE_EXTERN WhaleErrorCode ISSum(IS,IS,IS*);
WHALE_EXTERN WhaleErrorCode ISExpand(IS,IS,IS*);
WHALE_EXTERN WhaleErrorCode ISGetMinMax(IS,WhaleInt*,WhaleInt*);

WHALE_EXTERN WhaleErrorCode ISBlockGetIndices(IS,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode ISBlockRestoreIndices(IS,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode ISBlockGetLocalSize(IS,WhaleInt *);
WHALE_EXTERN WhaleErrorCode ISBlockGetSize(IS,WhaleInt *);
WHALE_EXTERN WhaleErrorCode ISGetBlockSize(IS,WhaleInt*);
WHALE_EXTERN WhaleErrorCode ISSetBlockSize(IS,WhaleInt);

WHALE_EXTERN WhaleErrorCode ISStrideGetInfo(IS,WhaleInt *,WhaleInt*);

WHALE_EXTERN WhaleErrorCode ISToGeneral(IS);

WHALE_EXTERN WhaleErrorCode ISDuplicate(IS,IS*);
WHALE_EXTERN WhaleErrorCode ISCopy(IS,IS);
WHALE_EXTERN WhaleErrorCode ISAllGather(IS,IS*);
WHALE_EXTERN WhaleErrorCode ISComplement(IS,WhaleInt,WhaleInt,IS*);
WHALE_EXTERN WhaleErrorCode ISConcatenate(MPI_Comm,WhaleInt,const IS[],IS*);
WHALE_EXTERN WhaleErrorCode ISListToPair(MPI_Comm,WhaleInt, IS[],IS*,IS*);
WHALE_EXTERN WhaleErrorCode ISPairToList(IS,IS,WhaleInt*, IS *[]);
WHALE_EXTERN WhaleErrorCode ISEmbed(IS,IS,WhaleBool,IS*);
WHALE_EXTERN WhaleErrorCode ISOnComm(IS,MPI_Comm,WhaleCopyMode,IS*);

/* --------------------------------------------------------------------------*/
WHALE_EXTERN WhaleClassId IS_LTOGM_CLASSID;

/*S
   ISLocalToGlobalMapping - mappings from an arbitrary
      local ordering from 0 to n-1 to a global Whale ordering
      used by a vector or matrix.

   Level: intermediate

   Note: mapping from Local to Global is scalable; but Global
  to Local may not be if the range of global values represented locally
  is very large.

   Note: the ISLocalToGlobalMapping is actually a private object; it is included
  here for the inline function ISLocalToGlobalMappingApply() to allow it to be inlined since
  it is used so often.

.seealso:  ISLocalToGlobalMappingCreate()
S*/
typedef struct _p_ISLocalToGlobalMapping* ISLocalToGlobalMapping;

/*E
    ISGlobalToLocalMappingType - Indicates if missing global indices are

   IS_GTOLM_MASK - missing global indices are replaced with -1
   IS_GTOLM_DROP - missing global indices are dropped

   Level: beginner

.seealso: ISGlobalToLocalMappingApply()

E*/
typedef enum {IS_GTOLM_MASK,IS_GTOLM_DROP} ISGlobalToLocalMappingType;

WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingCreate(MPI_Comm,WhaleInt,const WhaleInt[],WhaleCopyMode,ISLocalToGlobalMapping*);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingCreateIS(IS,ISLocalToGlobalMapping *);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingCreateSF(WhaleSF,WhaleInt,ISLocalToGlobalMapping*);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingView(ISLocalToGlobalMapping,WhaleViewer);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingDestroy(ISLocalToGlobalMapping*);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingApplyIS(ISLocalToGlobalMapping,IS,IS*);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingApply(ISLocalToGlobalMapping,WhaleInt,const WhaleInt[],WhaleInt[]);
WHALE_EXTERN WhaleErrorCode ISGlobalToLocalMappingApply(ISLocalToGlobalMapping,ISGlobalToLocalMappingType,WhaleInt,const WhaleInt[],WhaleInt*,WhaleInt[]);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingGetSize(ISLocalToGlobalMapping,WhaleInt*);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingGetInfo(ISLocalToGlobalMapping,WhaleInt*,WhaleInt*[],WhaleInt*[],WhaleInt**[]);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingRestoreInfo(ISLocalToGlobalMapping,WhaleInt*,WhaleInt*[],WhaleInt*[],WhaleInt**[]);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingGetIndices(ISLocalToGlobalMapping,const WhaleInt**);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingRestoreIndices(ISLocalToGlobalMapping,const WhaleInt**);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingBlock(ISLocalToGlobalMapping,WhaleInt,ISLocalToGlobalMapping*);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingUnBlock(ISLocalToGlobalMapping,WhaleInt,ISLocalToGlobalMapping*);
WHALE_EXTERN WhaleErrorCode ISLocalToGlobalMappingConcatenate(MPI_Comm,WhaleInt,const ISLocalToGlobalMapping[],ISLocalToGlobalMapping*);
WHALE_EXTERN WhaleErrorCode ISG2LMapApply(ISLocalToGlobalMapping,WhaleInt,const WhaleInt[],WhaleInt[]);

/* --------------------------------------------------------------------------*/
/*E
    ISColoringType - determines if the coloring is for the entire parallel grid/graph/matrix
                     or for just the local ghosted portion

    Level: beginner

$   IS_COLORING_GLOBAL - does not include the colors for ghost points, this is used when the function
$                        is called synchronously in parallel. This requires generating a "parallel coloring".
$   IS_COLORING_GHOSTED - includes colors for ghost points, this is used when the function can be called
$                         seperately on individual processes with the ghost points already filled in. Does not
$                         require a "parallel coloring", rather each process colors its local + ghost part.
$                         Using this can result in much less parallel communication. In the paradigm of
$                         DMGetLocalVector() and DMGetGlobalVector() this could be called IS_COLORING_LOCAL

.seealso: DMCreateColoring()
E*/
typedef enum {IS_COLORING_GLOBAL,IS_COLORING_GHOSTED} ISColoringType;
WHALE_EXTERN const char *const ISColoringTypes[];
typedef unsigned WHALE_IS_COLOR_VALUE_TYPE ISColoringValue;
WHALE_EXTERN WhaleErrorCode ISAllGatherColors(MPI_Comm,WhaleInt,ISColoringValue*,WhaleInt*,ISColoringValue*[]);

/*S
     ISColoring - sets of IS's that define a coloring
              of the underlying indices

   Level: intermediate

    Notes:
        One should not access the *is records below directly because they may not yet
    have been created. One should use ISColoringGetIS() to make sure they are
    created when needed.

    Developer Note: this is not a WhaleObject

.seealso:  ISColoringCreate(), ISColoringGetIS(), ISColoringView(), ISColoringGetIS()
S*/
struct _n_ISColoring {
  WhaleInt        refct;
  WhaleInt        n;                /* number of colors */
  IS              *is;              /* for each color indicates columns */
  MPI_Comm        comm;
  ISColoringValue *colors;          /* for each column indicates color */
  WhaleInt        N;                /* number of columns */
  ISColoringType  ctype;
};
typedef struct _n_ISColoring* ISColoring;

WHALE_EXTERN WhaleErrorCode ISColoringCreate(MPI_Comm,WhaleInt,WhaleInt,const ISColoringValue[],ISColoring*);
WHALE_EXTERN WhaleErrorCode ISColoringDestroy(ISColoring*);
WHALE_EXTERN WhaleErrorCode ISColoringView(ISColoring,WhaleViewer);
WHALE_EXTERN WhaleErrorCode ISColoringGetIS(ISColoring,WhaleInt*,IS*[]);
WHALE_EXTERN WhaleErrorCode ISColoringRestoreIS(ISColoring,IS*[]);
WHALE_EXTERN WhaleErrorCode ISColoringReference(ISColoring);
WHALE_EXTERN WhaleErrorCode ISColoringSetType(ISColoring,ISColoringType);


/* --------------------------------------------------------------------------*/

WHALE_EXTERN WhaleErrorCode ISPartitioningToNumbering(IS,IS*);
WHALE_EXTERN WhaleErrorCode ISPartitioningCount(IS,WhaleInt,WhaleInt[]);

WHALE_EXTERN WhaleErrorCode ISCompressIndicesGeneral(WhaleInt,WhaleInt,WhaleInt,WhaleInt,const IS[],IS[]);
WHALE_EXTERN WhaleErrorCode ISCompressIndicesSorted(WhaleInt,WhaleInt,WhaleInt,const IS[],IS[]);
WHALE_EXTERN WhaleErrorCode ISExpandIndicesGeneral(WhaleInt,WhaleInt,WhaleInt,WhaleInt,const IS[],IS[]);

/*S
     WhaleLayout - defines layout of vectors and matrices across processes (which rows are owned by which processes)

   Level: developer


.seealso:  WhaleLayoutCreate(), WhaleLayoutDestroy()
S*/
typedef struct _n_WhaleLayout* WhaleLayout;
struct _n_WhaleLayout{
  MPI_Comm               comm;
  WhaleInt               n,N;         /* local, global vector size */
  WhaleInt               rstart,rend; /* local start, local end + 1 */
  WhaleInt               *range;      /* the offset of each processor */
  WhaleInt               bs;          /* number of elements in each block (generally for multi-component problems) Do NOT multiply above numbers by bs */
  WhaleInt               refcnt;      /* MPI Vecs obtained with VecDuplicate() and from MatGetVecs() reuse map of input object */
  ISLocalToGlobalMapping mapping;     /* mapping used in Vec/MatSetValuesLocal() */
  ISLocalToGlobalMapping bmapping;    /* mapping used in Vec/MatSetValuesBlockedLocal() */
  WhaleInt               *trstarts;   /* local start for each thread */
};

WHALE_EXTERN WhaleErrorCode WhaleLayoutCreate(MPI_Comm,WhaleLayout*);
WHALE_EXTERN WhaleErrorCode WhaleLayoutSetUp(WhaleLayout);
WHALE_EXTERN WhaleErrorCode WhaleLayoutDestroy(WhaleLayout*);
WHALE_EXTERN WhaleErrorCode WhaleLayoutDuplicate(WhaleLayout,WhaleLayout*);
WHALE_EXTERN WhaleErrorCode WhaleLayoutReference(WhaleLayout,WhaleLayout*);
WHALE_EXTERN WhaleErrorCode WhaleLayoutSetLocalSize(WhaleLayout,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleLayoutGetLocalSize(WhaleLayout,WhaleInt *);
WHALE_EXTERN WhaleErrorCode WhaleLayoutSetSize(WhaleLayout,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleLayoutGetSize(WhaleLayout,WhaleInt *);
WHALE_EXTERN WhaleErrorCode WhaleLayoutSetBlockSize(WhaleLayout,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleLayoutGetBlockSize(WhaleLayout,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleLayoutGetRange(WhaleLayout,WhaleInt *,WhaleInt *);
WHALE_EXTERN WhaleErrorCode WhaleLayoutGetRanges(WhaleLayout,const WhaleInt *[]);
WHALE_EXTERN WhaleErrorCode WhaleLayoutSetISLocalToGlobalMapping(WhaleLayout,ISLocalToGlobalMapping);
WHALE_EXTERN WhaleErrorCode WhaleLayoutSetISLocalToGlobalMappingBlock(WhaleLayout,ISLocalToGlobalMapping);
WHALE_EXTERN WhaleErrorCode WhaleSFSetGraphLayout(WhaleSF,WhaleLayout,WhaleInt,const WhaleInt*,WhaleCopyMode,const WhaleInt*);

#undef __FUNCT__
#define __FUNCT__ "WhaleLayoutFindOwner"
/*@C
     WhaleLayoutFindOwner - Find the owning rank for a global index

    Not Collective

   Input Parameters:
+    map - the layout
-    idx - global index to find the owner of

   Output Parameter:
.    owner - the owning rank

   Level: developer

    Fortran Notes:
      Not available from Fortran

@*/
WHALE_STATIC_INLINE WhaleErrorCode WhaleLayoutFindOwner(WhaleLayout map,WhaleInt idx,WhaleInt *owner)
{
  WhaleErrorCode ierr;
  WhaleMPIInt    lo = 0,hi,t;

  WhaleFunctionBegin;
  *owner = -1;                  /* GCC erroneously issues warning about possibly uninitialized use when error condition */
  if (!((map->n >= 0) && (map->N >= 0) && (map->range))) SETERRQ(WHALE_COMM_SELF,WHALE_ERR_ARG_WRONGSTATE,"WhaleLayoutSetUp() must be called first");
  if (idx < 0 || idx > map->N) SETERRQ1(WHALE_COMM_SELF,WHALE_ERR_ARG_OUTOFRANGE,"Index %D is out of range",idx);
  ierr = MPI_Comm_size(map->comm,&hi);CHKERRQ(ierr);
  while (hi - lo > 1) {
    t = lo + (hi - lo) / 2;
    if (idx < map->range[t]) hi = t;
    else                     lo = t;
  }
  *owner = lo;
  WhaleFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "WhaleLayoutFindOwnerIndex"
/*@C
     WhaleLayoutFindOwnerIndex - Find the owning rank and the local index for a global index

    Not Collective

   Input Parameters:
+    map   - the layout
-    idx   - global index to find the owner of

   Output Parameter:
+    owner - the owning rank
-    lidx  - local index used by the owner for idx

   Level: developer

    Fortran Notes:
      Not available from Fortran

@*/
WHALE_STATIC_INLINE WhaleErrorCode WhaleLayoutFindOwnerIndex(WhaleLayout map,WhaleInt idx,WhaleInt *owner, WhaleInt *lidx)
{
  WhaleErrorCode ierr;
  WhaleMPIInt    lo = 0,hi,t;

  WhaleFunctionBegin;
  if (!((map->n >= 0) && (map->N >= 0) && (map->range))) SETERRQ(WHALE_COMM_SELF,WHALE_ERR_ARG_WRONGSTATE,"WhaleLayoutSetUp() must be called first");
  if (idx < 0 || idx > map->N) SETERRQ1(WHALE_COMM_SELF,WHALE_ERR_ARG_OUTOFRANGE,"Index %D is out of range",idx);
  ierr = MPI_Comm_size(map->comm,&hi);CHKERRQ(ierr);
  while (hi - lo > 1) {
    t = lo + (hi - lo) / 2;
    if (idx < map->range[t]) hi = t;
    else                     lo = t;
  }
  *owner = lo;
  *lidx  = idx-map->range[lo];
  WhaleFunctionReturn(0);
}

WHALE_EXTERN WhaleClassId WHALE_SECTION_CLASSID;

/*S
  WhaleSection - Mapping from integers in a designated range to contiguous sets of integers.

  In contrast to IS, which maps from integers to single integers, the range of a WhaleSection is in the space of
  contiguous sets of integers. These ranges are frequently interpreted as domains of other array-like objects,
  especially other WhaleSections, Vecs, and ISs. The domain is set with WhaleSectionSetChart() and does not need to
  start at 0. For each point in the domain of a WhaleSection, the output set is represented through an offset and a
  count, which are set using WhaleSectionSetOffset() and WhaleSectionSetDof() respectively. Lookup is typically using
  accessors or routines like VecGetValuesSection().

  Level: developer

.seealso:  WhaleSectionCreate(), WhaleSectionDestroy()
S*/
typedef struct _p_WhaleSection *WhaleSection;
WHALE_EXTERN WhaleErrorCode WhaleSectionCreate(MPI_Comm,WhaleSection*);
WHALE_EXTERN WhaleErrorCode WhaleSectionClone(WhaleSection, WhaleSection*);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetNumFields(WhaleSection, WhaleInt *);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetNumFields(WhaleSection, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetFieldName(WhaleSection, WhaleInt, const char *[]);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetFieldName(WhaleSection, WhaleInt, const char []);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetFieldComponents(WhaleSection, WhaleInt, WhaleInt *);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetFieldComponents(WhaleSection, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetChart(WhaleSection, WhaleInt *, WhaleInt *);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetChart(WhaleSection, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetDof(WhaleSection, WhaleInt, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetDof(WhaleSection, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionAddDof(WhaleSection, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetFieldDof(WhaleSection, WhaleInt, WhaleInt, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetFieldDof(WhaleSection, WhaleInt, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionAddFieldDof(WhaleSection, WhaleInt, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetConstraintDof(WhaleSection, WhaleInt, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetConstraintDof(WhaleSection, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionAddConstraintDof(WhaleSection, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetFieldConstraintDof(WhaleSection, WhaleInt, WhaleInt, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetFieldConstraintDof(WhaleSection, WhaleInt, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionAddFieldConstraintDof(WhaleSection, WhaleInt, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetConstraintIndices(WhaleSection, WhaleInt, const WhaleInt**);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetConstraintIndices(WhaleSection, WhaleInt, const WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetFieldConstraintIndices(WhaleSection, WhaleInt, WhaleInt, const WhaleInt**);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetFieldConstraintIndices(WhaleSection, WhaleInt, WhaleInt, const WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetUpBC(WhaleSection);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetUp(WhaleSection);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetMaxDof(WhaleSection, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetStorageSize(WhaleSection, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetConstrainedStorageSize(WhaleSection, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetOffset(WhaleSection, WhaleInt, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetOffset(WhaleSection, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetFieldOffset(WhaleSection, WhaleInt, WhaleInt, WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleSectionSetFieldOffset(WhaleSection, WhaleInt, WhaleInt, WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetOffsetRange(WhaleSection, WhaleInt *, WhaleInt *);
WHALE_EXTERN WhaleErrorCode WhaleSectionView(WhaleSection, WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleSectionReset(WhaleSection);
WHALE_EXTERN WhaleErrorCode WhaleSectionDestroy(WhaleSection*);
WHALE_EXTERN WhaleErrorCode WhaleSectionCreateGlobalSection(WhaleSection, WhaleSF, WhaleBool, WhaleSection *);
WHALE_EXTERN WhaleErrorCode WhaleSectionCreateGlobalSectionCensored(WhaleSection, WhaleSF, WhaleBool, WhaleInt, const WhaleInt [], WhaleSection *);
WHALE_EXTERN WhaleErrorCode WhaleSectionCreateSubsection(WhaleSection, WhaleInt, WhaleInt [], WhaleSection *);
WHALE_EXTERN WhaleErrorCode WhaleSectionCreateSubmeshSection(WhaleSection, IS, WhaleSection *);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetPointLayout(MPI_Comm, WhaleSection, WhaleLayout *);
WHALE_EXTERN WhaleErrorCode WhaleSectionGetValueLayout(MPI_Comm, WhaleSection, WhaleLayout *);

/* WhaleSF support */
WHALE_EXTERN WhaleErrorCode WhaleSFConvertPartition(WhaleSF, WhaleSection, IS, ISLocalToGlobalMapping *, WhaleSF *);
WHALE_EXTERN WhaleErrorCode WhaleSFCreateRemoteOffsets(WhaleSF, WhaleSection, WhaleSection, WhaleInt **);
WHALE_EXTERN WhaleErrorCode WhaleSFDistributeSection(WhaleSF, WhaleSection, WhaleInt **, WhaleSection);
WHALE_EXTERN WhaleErrorCode WhaleSFCreateSectionSF(WhaleSF, WhaleSection, WhaleInt [], WhaleSection, WhaleSF *);

/* Reset __FUNCT__ in case the user does not define it themselves */
#undef __FUNCT__
#define __FUNCT__ "User provided function"

#endif
