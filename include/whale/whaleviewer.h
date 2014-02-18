/*
     WhaleViewers are objects where other objects can be looked at or stored.
*/

#if !defined(__WHALEVIEWER_H)
#define __WHALEVIEWER_H

#include <whalesys.h>
#include <whaleviewertypes.h>

WHALE_EXTERN WhaleClassId WHALE_VIEWER_CLASSID;

/*J
    WhaleViewerType - String with the name of a Whale WhaleViewer

   Level: beginner

.seealso: WhaleViewerSetType(), WhaleViewer, WhaleViewerRegister(), WhaleViewerCreate()
J*/
typedef const char* WhaleViewerType;
#define WHALEVIEWERSOCKET       "socket"
#define WHALEVIEWERASCII        "ascii"
#define WHALEVIEWERBINARY       "binary"
#define WHALEVIEWERSTRING       "string"
#define WHALEVIEWERDRAW         "draw"
#define WHALEVIEWERVU           "vu"
#define WHALEVIEWERMATHEMATICA  "mathematica"
#define WHALEVIEWERNETCDF       "netcdf"
#define WHALEVIEWERHDF5         "hdf5"
#define WHALEVIEWERVTK          "vtk"
#define WHALEVIEWERMATLAB       "matlab"
#define WHALEVIEWERAMS          "ams"

WHALE_EXTERN WhaleFunctionList WhaleViewerList;
WHALE_EXTERN WhaleErrorCode WhaleViewerRegisterAll(void);
WHALE_EXTERN WhaleErrorCode WhaleViewerInitializePackage(void);

WHALE_EXTERN WhaleErrorCode WhaleViewerRegister(const char[],WhaleErrorCode (*)(WhaleViewer));

WHALE_EXTERN WhaleErrorCode WhaleViewerCreate(MPI_Comm,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerSetFromOptions(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIOpenWithFILE(MPI_Comm,FILE*,WhaleViewer*);

WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIOpen(MPI_Comm,const char[],WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIISetFILE(WhaleViewer,FILE*);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryOpen(MPI_Comm,const char[],WhaleFileMode,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryGetFlowControl(WhaleViewer,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinarySetFlowControl(WhaleViewer,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinarySetMPIIO(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryGetMPIIO(WhaleViewer,WhaleBool *);
#if defined(WHALE_HAVE_MPIIO)
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryGetMPIIODescriptor(WhaleViewer,MPI_File*);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryGetMPIIOOffset(WhaleViewer,MPI_Offset*);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryAddMPIIOOffset(WhaleViewer,MPI_Offset);
#endif

WHALE_EXTERN WhaleErrorCode WhaleViewerSocketOpen(MPI_Comm,const char[],int,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerStringOpen(MPI_Comm,char[],WhaleInt,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawOpen(MPI_Comm,const char[],const char[],int,int,int,int,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaOpen(MPI_Comm, int, const char[], const char[], WhaleViewer *);
WHALE_EXTERN WhaleErrorCode WhaleViewerSiloOpen(MPI_Comm, const char[], WhaleViewer *);
WHALE_EXTERN WhaleErrorCode WhaleViewerMatlabOpen(MPI_Comm,const char[],WhaleFileMode,WhaleViewer*);

WHALE_EXTERN WhaleErrorCode WhaleViewerGetType(WhaleViewer,WhaleViewerType*);
WHALE_EXTERN WhaleErrorCode WhaleViewerSetType(WhaleViewer,WhaleViewerType);
WHALE_EXTERN WhaleErrorCode WhaleViewerDestroy(WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerGetSingleton(WhaleViewer,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerRestoreSingleton(WhaleViewer,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerGetSubcomm(WhaleViewer,MPI_Comm,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerRestoreSubcomm(WhaleViewer,MPI_Comm,WhaleViewer*);

WHALE_EXTERN WhaleErrorCode WhaleViewerSetUp(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerView(WhaleViewer,WhaleViewer);

WHALE_EXTERN WhaleErrorCode WhaleViewerSetOptionsPrefix(WhaleViewer,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleViewerAppendOptionsPrefix(WhaleViewer,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleViewerGetOptionsPrefix(WhaleViewer,const char*[]);

/*E
    WhaleViewerFormat - Way a viewer presents the object

   Level: beginner

   The values below are also listed in finclude/whaleviewer.h. If another values is added below it
   must also be added there.

.seealso: WhaleViewerSetFormat(), WhaleViewer, WhaleViewerType, WhaleViewerPushFormat(), WhaleViewerPopFormat()
E*/
typedef enum {
  WHALE_VIEWER_DEFAULT,
  WHALE_VIEWER_ASCII_MATLAB,
  WHALE_VIEWER_ASCII_MATHEMATICA,
  WHALE_VIEWER_ASCII_IMPL,
  WHALE_VIEWER_ASCII_INFO,
  WHALE_VIEWER_ASCII_INFO_DETAIL,
  WHALE_VIEWER_ASCII_COMMON,
  WHALE_VIEWER_ASCII_SYMMODU,
  WHALE_VIEWER_ASCII_INDEX,
  WHALE_VIEWER_ASCII_DENSE,
  WHALE_VIEWER_ASCII_MATRIXMARKET,
  WHALE_VIEWER_ASCII_VTK,
  WHALE_VIEWER_ASCII_VTK_CELL,
  WHALE_VIEWER_ASCII_VTK_COORDS,
  WHALE_VIEWER_ASCII_PCICE,
  WHALE_VIEWER_ASCII_PYTHON,
  WHALE_VIEWER_ASCII_FACTOR_INFO,
  WHALE_VIEWER_ASCII_LATEX,
  WHALE_VIEWER_DRAW_BASIC,
  WHALE_VIEWER_DRAW_LG,
  WHALE_VIEWER_DRAW_CONTOUR,
  WHALE_VIEWER_DRAW_PORTS,
  WHALE_VIEWER_VTK_VTS,
  WHALE_VIEWER_VTK_VTU,
  WHALE_VIEWER_BINARY_MATLAB,
  WHALE_VIEWER_NATIVE,
  WHALE_VIEWER_NOFORMAT
  } WhaleViewerFormat;
WHALE_EXTERN const char *const WhaleViewerFormats[];

WHALE_EXTERN WhaleErrorCode WhaleViewerSetFormat(WhaleViewer,WhaleViewerFormat);
WHALE_EXTERN WhaleErrorCode WhaleViewerPushFormat(WhaleViewer,WhaleViewerFormat);
WHALE_EXTERN WhaleErrorCode WhaleViewerPopFormat(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerGetFormat(WhaleViewer,WhaleViewerFormat*);
WHALE_EXTERN WhaleErrorCode WhaleViewerFlush(WhaleViewer);

WHALE_EXTERN WhaleErrorCode WhaleOptionsGetViewer(MPI_Comm,const char[],const char[],WhaleViewer*,WhaleViewerFormat*,WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleOptionsViewer(const char[],const char[],const char[],WhaleViewer*,WhaleViewerFormat *,WhaleBool *);

/*
   Operations explicit to a particular class of viewers
*/

WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIGetPointer(WhaleViewer,FILE**);
WHALE_EXTERN WhaleErrorCode WhaleViewerFileGetMode(WhaleViewer,WhaleFileMode*);
WHALE_EXTERN WhaleErrorCode WhaleViewerFileSetMode(WhaleViewer,WhaleFileMode);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIPrintf(WhaleViewer,const char[],...);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIISynchronizedPrintf(WhaleViewer,const char[],...);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIISynchronizedAllow(WhaleViewer,WhaleBool);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIPushTab(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIPopTab(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIUseTabs(WhaleViewer,WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIISetTab(WhaleViewer,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIGetTab(WhaleViewer,WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIAddTab(WhaleViewer,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIISubtractTab(WhaleViewer,WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryGetDescriptor(WhaleViewer,int*);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryGetInfoPointer(WhaleViewer,FILE **);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryRead(WhaleViewer,void*,WhaleInt,WhaleDataType);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryWrite(WhaleViewer,void*,WhaleInt,WhaleDataType,WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleViewerStringSPrintf(WhaleViewer,const char[],...);
WHALE_EXTERN WhaleErrorCode WhaleViewerStringSetString(WhaleViewer,char[],WhaleInt);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawClear(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawSetHold(WhaleViewer,WhaleBool);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawGetHold(WhaleViewer,WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawSetPause(WhaleViewer,WhaleReal);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawGetPause(WhaleViewer,WhaleReal*);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawSetInfo(WhaleViewer,const char[],const char[],int,int,int,int);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawResize(WhaleViewer,int,int);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawSetBounds(WhaleViewer,WhaleInt,const WhaleReal*);
WHALE_EXTERN WhaleErrorCode WhaleViewerDrawGetBounds(WhaleViewer,WhaleInt*,const WhaleReal**);
WHALE_EXTERN WhaleErrorCode WhaleViewerSocketSetConnection(WhaleViewer,const char[],int);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinarySkipInfo(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinarySetSkipOptions(WhaleViewer,WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryGetSkipOptions(WhaleViewer,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinarySetSkipHeader(WhaleViewer,WhaleBool);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryGetSkipHeader(WhaleViewer,WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryReadStringArray(WhaleViewer,char***);
WHALE_EXTERN WhaleErrorCode WhaleViewerBinaryWriteStringArray(WhaleViewer,char**);

WHALE_EXTERN WhaleErrorCode WhaleViewerFileSetName(WhaleViewer,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleViewerFileGetName(WhaleViewer,const char**);

WHALE_EXTERN WhaleErrorCode WhaleViewerVUGetPointer(WhaleViewer, FILE**);
WHALE_EXTERN WhaleErrorCode WhaleViewerVUSetVecSeen(WhaleViewer, WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleViewerVUGetVecSeen(WhaleViewer, WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleViewerVUPrintDeferred(WhaleViewer, const char [], ...);
WHALE_EXTERN WhaleErrorCode WhaleViewerVUFlushDeferred(WhaleViewer);

WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaInitializePackage(void);
WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaFinalizePackage(void);
WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaGetName(WhaleViewer, const char **);
WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaSetName(WhaleViewer, const char []);
WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaClearName(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerMathematicaSkipPackets(WhaleViewer, int);

WHALE_EXTERN WhaleErrorCode WhaleViewerSiloGetName(WhaleViewer, char **);
WHALE_EXTERN WhaleErrorCode WhaleViewerSiloSetName(WhaleViewer, const char []);
WHALE_EXTERN WhaleErrorCode WhaleViewerSiloClearName(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleViewerSiloGetMeshName(WhaleViewer, char **);
WHALE_EXTERN WhaleErrorCode WhaleViewerSiloSetMeshName(WhaleViewer, const char []);
WHALE_EXTERN WhaleErrorCode WhaleViewerSiloClearMeshName(WhaleViewer);

WHALE_EXTERN WhaleErrorCode WhaleViewerNetcdfOpen(MPI_Comm,const char[],WhaleFileMode,WhaleViewer*);
WHALE_EXTERN WhaleErrorCode WhaleViewerNetcdfGetID(WhaleViewer, int *);

typedef enum {WHALE_VTK_POINT_FIELD, WHALE_VTK_POINT_VECTOR_FIELD, WHALE_VTK_CELL_FIELD, WHALE_VTK_CELL_VECTOR_FIELD} WhaleViewerVTKFieldType;
WHALE_EXTERN WhaleErrorCode WhaleViewerVTKAddField(WhaleViewer,WhaleObject,WhaleErrorCode (*WhaleViewerVTKWriteFunction)(WhaleObject,WhaleViewer),WhaleViewerVTKFieldType,WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleViewerVTKOpen(MPI_Comm,const char[],WhaleFileMode,WhaleViewer*);

/*
     These are all the default viewers that do not have to be explicitly opened
*/
WHALE_EXTERN WhaleViewer    WHALE_VIEWER_STDOUT_(MPI_Comm);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIGetStdout(MPI_Comm,WhaleViewer*);
WHALE_EXTERN WhaleViewer    WHALE_VIEWER_STDERR_(MPI_Comm);
WHALE_EXTERN WhaleErrorCode WhaleViewerASCIIGetStderr(MPI_Comm,WhaleViewer*);
WHALE_EXTERN WhaleViewer    WHALE_VIEWER_DRAW_(MPI_Comm);
WHALE_EXTERN WhaleViewer    WHALE_VIEWER_SOCKET_(MPI_Comm);
WHALE_EXTERN WhaleViewer    WHALE_VIEWER_BINARY_(MPI_Comm);
WHALE_EXTERN WhaleViewer    WHALE_VIEWER_MATLAB_(MPI_Comm);
WHALE_EXTERN WhaleViewer   WHALE_VIEWER_MATHEMATICA_WORLD_PRIVATE;

#define WHALE_VIEWER_STDERR_SELF  WHALE_VIEWER_STDERR_(WHALE_COMM_SELF)
#define WHALE_VIEWER_STDERR_WORLD WHALE_VIEWER_STDERR_(WHALE_COMM_WORLD)

/*MC
  WHALE_VIEWER_STDOUT_WORLD  - same as WHALE_VIEWER_STDOUT_(WHALE_COMM_WORLD)

  Level: beginner
M*/
#define WHALE_VIEWER_STDOUT_WORLD WHALE_VIEWER_STDOUT_(WHALE_COMM_WORLD)

/*MC
  WHALE_VIEWER_STDOUT_SELF  - same as WHALE_VIEWER_STDOUT_(WHALE_COMM_SELF)

  Level: beginner
M*/
#define WHALE_VIEWER_STDOUT_SELF  WHALE_VIEWER_STDOUT_(WHALE_COMM_SELF)

/*MC
  WHALE_VIEWER_DRAW_WORLD  - same as WHALE_VIEWER_DRAW_(WHALE_COMM_WORLD)

  Level: intermediate
M*/
#define WHALE_VIEWER_DRAW_WORLD   WHALE_VIEWER_DRAW_(WHALE_COMM_WORLD)

/*MC
  WHALE_VIEWER_DRAW_SELF  - same as WHALE_VIEWER_DRAW_(WHALE_COMM_SELF)

  Level: intermediate
M*/
#define WHALE_VIEWER_DRAW_SELF    WHALE_VIEWER_DRAW_(WHALE_COMM_SELF)

/*MC
  WHALE_VIEWER_SOCKET_WORLD  - same as WHALE_VIEWER_SOCKET_(WHALE_COMM_WORLD)

  Level: intermediate
M*/
#define WHALE_VIEWER_SOCKET_WORLD WHALE_VIEWER_SOCKET_(WHALE_COMM_WORLD)

/*MC
  WHALE_VIEWER_SOCKET_SELF  - same as WHALE_VIEWER_SOCKET_(WHALE_COMM_SELF)

  Level: intermediate
M*/
#define WHALE_VIEWER_SOCKET_SELF  WHALE_VIEWER_SOCKET_(WHALE_COMM_SELF)

/*MC
  WHALE_VIEWER_BINARY_WORLD  - same as WHALE_VIEWER_BINARY_(WHALE_COMM_WORLD)

  Level: intermediate
M*/
#define WHALE_VIEWER_BINARY_WORLD WHALE_VIEWER_BINARY_(WHALE_COMM_WORLD)

/*MC
  WHALE_VIEWER_BINARY_SELF  - same as WHALE_VIEWER_BINARY_(WHALE_COMM_SELF)

  Level: intermediate
M*/
#define WHALE_VIEWER_BINARY_SELF  WHALE_VIEWER_BINARY_(WHALE_COMM_SELF)

/*MC
  WHALE_VIEWER_MATLAB_WORLD  - same as WHALE_VIEWER_MATLAB_(WHALE_COMM_WORLD)

  Level: intermediate
M*/
#define WHALE_VIEWER_MATLAB_WORLD WHALE_VIEWER_MATLAB_(WHALE_COMM_WORLD)

/*MC
  WHALE_VIEWER_MATLAB_SELF  - same as WHALE_VIEWER_MATLAB_(WHALE_COMM_SELF)

  Level: intermediate
M*/
#define WHALE_VIEWER_MATLAB_SELF  WHALE_VIEWER_MATLAB_(WHALE_COMM_SELF)

#define WHALE_VIEWER_MATHEMATICA_WORLD (WhaleViewerInitializeMathematicaWorld_Private(),WHALE_VIEWER_MATHEMATICA_WORLD_PRIVATE)

#undef __FUNCT__
#define __FUNCT__ "WhaleViewerFlowControlStart"
WHALE_STATIC_INLINE WhaleErrorCode WhaleViewerFlowControlStart(WhaleViewer viewer,WhaleInt *mcnt,WhaleInt *cnt)
{
  WhaleErrorCode ierr;
  WhaleFunctionBegin;
  ierr = WhaleViewerBinaryGetFlowControl(viewer,mcnt);CHKERRQ(ierr);
  ierr = WhaleViewerBinaryGetFlowControl(viewer,cnt);CHKERRQ(ierr);
  WhaleFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "WhaleViewerFlowControlStepMaster"
WHALE_STATIC_INLINE WhaleErrorCode WhaleViewerFlowControlStepMaster(WhaleViewer viewer,WhaleInt i,WhaleInt *mcnt,WhaleInt cnt)
{
  WhaleErrorCode ierr;
  MPI_Comm       comm;

  WhaleFunctionBegin;
  ierr = WhaleObjectGetComm((WhaleObject)viewer,&comm);CHKERRQ(ierr);
  if (i >= *mcnt) {
    *mcnt += cnt;
    ierr = MPI_Bcast(mcnt,1,MPIU_INT,0,comm);CHKERRQ(ierr);
  }
  WhaleFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "WhaleViewerFlowControlEndMaster"
WHALE_STATIC_INLINE WhaleErrorCode WhaleViewerFlowControlEndMaster(WhaleViewer viewer,WhaleInt *mcnt)
{
  WhaleErrorCode ierr;
  MPI_Comm       comm;
  WhaleFunctionBegin;
  ierr = WhaleObjectGetComm((WhaleObject)viewer,&comm);CHKERRQ(ierr);
  *mcnt = 0;
  ierr = MPI_Bcast(mcnt,1,MPIU_INT,0,comm);CHKERRQ(ierr);
  WhaleFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "WhaleViewerFlowControlStepWorker"
WHALE_STATIC_INLINE WhaleErrorCode WhaleViewerFlowControlStepWorker(WhaleViewer viewer,WhaleMPIInt rank,WhaleInt *mcnt)
{
  WhaleErrorCode ierr;
  MPI_Comm       comm;
  WhaleFunctionBegin;
  ierr = WhaleObjectGetComm((WhaleObject)viewer,&comm);CHKERRQ(ierr);
  while (WHALE_TRUE) {
    if (rank < *mcnt) break;
    ierr = MPI_Bcast(mcnt,1,MPIU_INT,0,comm);CHKERRQ(ierr);
  }
  WhaleFunctionReturn(0);
}

#undef __FUNCT__
#define __FUNCT__ "WhaleViewerFlowControlEndWorker"
WHALE_STATIC_INLINE WhaleErrorCode WhaleViewerFlowControlEndWorker(WhaleViewer viewer,WhaleInt *mcnt)
{
  WhaleErrorCode ierr;
  MPI_Comm       comm;
  WhaleFunctionBegin;
  ierr = WhaleObjectGetComm((WhaleObject)viewer,&comm);CHKERRQ(ierr);
  while (WHALE_TRUE) {
    ierr = MPI_Bcast(mcnt,1,MPIU_INT,0,comm);CHKERRQ(ierr);
    if (!*mcnt) break;
  }
  WhaleFunctionReturn(0);
}

/*
   WhaleViewer writes to MATLAB .mat file
*/
WHALE_EXTERN WhaleErrorCode WhaleViewerMatlabPutArray(WhaleViewer,int,int,const WhaleScalar*,const char*);
WHALE_EXTERN WhaleErrorCode WhaleViewerMatlabGetArray(WhaleViewer,int,int,WhaleScalar*,const char*);
WHALE_EXTERN WhaleErrorCode WhaleViewerMatlabPutVariable(WhaleViewer,const char*,void*);

#if defined(WHALE_HAVE_AMS)
WHALE_EXTERN WhaleErrorCode WhaleObjectViewAMS(WhaleObject,WhaleViewer);
#endif

/*S
     WhaleViewers - Abstract collection of WhaleViewers. It is just an expandable array of viewers.

   Level: intermediate

  Concepts: viewing

.seealso:  WhaleViewerCreate(), WhaleViewerSetType(), WhaleViewerType, WhaleViewer, WhaleViewersCreate(),
           WhaleViewersGetViewer()
S*/
typedef struct _n_WhaleViewers* WhaleViewers;
WHALE_EXTERN WhaleErrorCode WhaleViewersCreate(MPI_Comm,WhaleViewers*);
WHALE_EXTERN WhaleErrorCode WhaleViewersDestroy(WhaleViewers*);
WHALE_EXTERN WhaleErrorCode WhaleViewersGetViewer(WhaleViewers,WhaleInt,WhaleViewer*);

/* Reset __FUNCT__ in case the user does not define it themselves */
#undef __FUNCT__
#define __FUNCT__ "User provided function"

#endif
