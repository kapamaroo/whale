/*
    Defines profile/logging in Whale.
*/

#if !defined(__WhaleLog_H)
#define __WhaleLog_H
#include "whalesys.h"

/*MC
    WhaleLogEvent - id used to identify Whale or user events which timed portions (blocks of executable)
     code.

    Level: intermediate

.seealso: WhaleLogEventRegister(), WhaleLogEventBegin(), WhaleLogEventEnd(), WhaleLogStage
M*/
typedef int WhaleLogEvent;

/*MC
    WhaleLogStage - id used to identify user stages (phases, sections) of runs - for logging

    Level: intermediate

.seealso: WhaleLogStageRegister(), WhaleLogStageBegin(), WhaleLogStageEnd(), WhaleLogEvent
M*/
typedef int WhaleLogStage;

#define WHALE_EVENT  1311311
WHALE_EXTERN WhaleLogEvent WHALE_LARGEST_EVENT;

/* Global flop counter */
WHALE_EXTERN WhaleLogDouble whale_TotalFlops;
WHALE_EXTERN WhaleLogDouble whale_tmp_flops;

/* General logging of information; different from event logging */
WHALE_EXTERN WhaleErrorCode WhaleInfo_Private(const char[],void*,const char[],...);
#if defined(WHALE_USE_INFO)
#define WhaleInfo(A,S)                       WhaleInfo_Private(WHALE_FUNCTION_NAME,A,S)
#define WhaleInfo1(A,S,a1)                   WhaleInfo_Private(WHALE_FUNCTION_NAME,A,S,a1)
#define WhaleInfo2(A,S,a1,a2)                WhaleInfo_Private(WHALE_FUNCTION_NAME,A,S,a1,a2)
#define WhaleInfo3(A,S,a1,a2,a3)             WhaleInfo_Private(WHALE_FUNCTION_NAME,A,S,a1,a2,a3)
#define WhaleInfo4(A,S,a1,a2,a3,a4)          WhaleInfo_Private(WHALE_FUNCTION_NAME,A,S,a1,a2,a3,a4)
#define WhaleInfo5(A,S,a1,a2,a3,a4,a5)       WhaleInfo_Private(WHALE_FUNCTION_NAME,A,S,a1,a2,a3,a4,a5)
#define WhaleInfo6(A,S,a1,a2,a3,a4,a5,a6)    WhaleInfo_Private(WHALE_FUNCTION_NAME,A,S,a1,a2,a3,a4,a5,a6)
#define WhaleInfo7(A,S,a1,a2,a3,a4,a5,a6,a7) WhaleInfo_Private(WHALE_FUNCTION_NAME,A,S,a1,a2,a3,a4,a5,a6,a7)
#else
#define WhaleInfo(A,S)                       0
#define WhaleInfo1(A,S,a1)                   0
#define WhaleInfo2(A,S,a1,a2)                0
#define WhaleInfo3(A,S,a1,a2,a3)             0
#define WhaleInfo4(A,S,a1,a2,a3,a4)          0
#define WhaleInfo5(A,S,a1,a2,a3,a4,a5)       0
#define WhaleInfo6(A,S,a1,a2,a3,a4,a5,a6)    0
#define WhaleInfo7(A,S,a1,a2,a3,a4,a5,a6,a7) 0
#endif
WHALE_EXTERN WhaleErrorCode WhaleInfoDeactivateClass(WhaleClassId);
WHALE_EXTERN WhaleErrorCode WhaleInfoActivateClass(WhaleClassId);
WHALE_EXTERN WhaleBool WhaleLogPrintInfo;  /* if true, indicates WhaleInfo() is turned on */

/* We must make the following structures available to access the event
     activation flags in the WhaleLogEventBegin/End() macros. These are not part of the Whale public
     API and are not intended to be used by other parts of Whale or by users.

     The code that manipulates these structures is in src/sys/plog/utils.
*/
typedef struct _n_WhaleIntStack *WhaleIntStack;

/*
    WhaleClassRegInfo, WhaleClassPerfInfo - Each class has two data structures associated with it. The first has
       static information about it, the second collects statistics on how many objects of the class are created,
       how much memory they use, etc.

    WhaleClassRegLog, WhaleClassPerfLog - arrays of the WhaleClassRegInfo and WhaleClassPerfInfo for all classes.
*/
typedef struct  {
  char           *name;   /* The class name */
  WhaleClassId   classid; /* The integer identifying this class */
} WhaleClassRegInfo;

typedef struct {
  WhaleClassId   id;           /* The integer identifying this class */
  int            creations;    /* The number of objects of this class created */
  int            destructions; /* The number of objects of this class destroyed */
  WhaleLogDouble mem;          /* The total memory allocated by objects of this class */
  WhaleLogDouble descMem;      /* The total memory allocated by descendents of these objects */
} WhaleClassPerfInfo;

typedef struct _n_WhaleClassRegLog *WhaleClassRegLog;
struct _n_WhaleClassRegLog {
  int               numClasses; /* The number of classes registered */
  int               maxClasses; /* The maximum number of classes */
  WhaleClassRegInfo *classInfo; /* The structure for class information (classids are monotonicly increasing) */
};

typedef struct _n_WhaleClassPerfLog *WhaleClassPerfLog;
struct _n_WhaleClassPerfLog {
  int                numClasses; /* The number of logging classes */
  int                maxClasses; /* The maximum number of classes */
  WhaleClassPerfInfo *classInfo; /* The structure for class information (classids are monotonicly increasing) */
};
/* -----------------------------------------------------------------------------------------------------*/
/*
    WhaleEventRegInfo, WhaleEventPerfInfo - Each event has two data structures associated with it. The first has
       static information about it, the second collects statistics on how many times the event is used, how
       much time it takes, etc.

    WhaleEventRegLog, WhaleEventPerfLog - an array of all WhaleEventRegInfo and WhaleEventPerfInfo for all events. There is one
      of these for each stage.

*/
typedef struct {
  char         *name;         /* The name of this event */
  WhaleClassId classid;       /* The class the event is associated with */
#if defined (WHALE_HAVE_MPE)
  int          mpe_id_begin; /* MPE IDs that define the event */
  int          mpe_id_end;
#endif
} WhaleEventRegInfo;

typedef struct {
  int            id;            /* The integer identifying this event */
  WhaleBool      active;        /* The flag to activate logging */
  WhaleBool      visible;       /* The flag to print info in summary */
  int            depth;         /* The nesting depth of the event call */
  int            count;         /* The number of times this event was executed */
  WhaleLogDouble flops, flops2,flopsTmp; /* The flops and flops^2 used in this event */
  WhaleLogDouble time, time2, timeTmp;   /* The time and time^2 taken for this event */
  WhaleLogDouble numMessages;   /* The number of messages in this event */
  WhaleLogDouble messageLength; /* The total message lengths in this event */
  WhaleLogDouble numReductions; /* The number of reductions in this event */
} WhaleEventPerfInfo;

typedef struct _n_WhaleEventRegLog *WhaleEventRegLog;
struct _n_WhaleEventRegLog {
  int               numEvents;  /* The number of registered events */
  int               maxEvents;  /* The maximum number of events */
  WhaleEventRegInfo *eventInfo; /* The registration information for each event */
};

typedef struct _n_WhaleEventPerfLog *WhaleEventPerfLog;
struct _n_WhaleEventPerfLog {
  int                numEvents;  /* The number of logging events */
  int                maxEvents;  /* The maximum number of events */
  WhaleEventPerfInfo *eventInfo; /* The performance information for each event */
};
/* ------------------------------------------------------------------------------------------------------------*/
/*
   WhaleStageInfo - Contains all the information about a particular stage.

   WhaleStageLog - An array of WhaleStageInfo for each registered stage. There is a single one of these in the code.
*/
typedef struct _WhaleStageInfo {
  char               *name;     /* The stage name */
  WhaleBool          used;      /* The stage was pushed on this processor */
  WhaleEventPerfInfo perfInfo;  /* The stage performance information */
  WhaleEventPerfLog  eventLog;  /* The event information for this stage */
  WhaleClassPerfLog  classLog;  /* The class information for this stage */
} WhaleStageInfo;

typedef struct _n_WhaleStageLog *WhaleStageLog;
struct _n_WhaleStageLog {
  int              numStages;   /* The number of registered stages */
  int              maxStages;   /* The maximum number of stages */
  WhaleIntStack    stack;       /* The stack for active stages */
  int              curStage;    /* The current stage (only used in macros so we don't call WhaleIntStackTop) */
  WhaleStageInfo   *stageInfo;  /* The information for each stage */
  WhaleEventRegLog eventLog;    /* The registered events */
  WhaleClassRegLog classLog;    /* The registered classes */
};

WHALE_EXTERN WhaleErrorCode WhaleLogGetStageLog(WhaleStageLog*);
WHALE_EXTERN WhaleErrorCode WhaleStageLogGetCurrent(WhaleStageLog,int*);
WHALE_EXTERN WhaleErrorCode WhaleStageLogGetEventPerfLog(WhaleStageLog,int,WhaleEventPerfLog*);


#if defined(WHALE_USE_LOG)  /* --- Logging is turned on --------------------------------*/
WHALE_EXTERN WhaleStageLog whale_stageLog;

/*
   Flop counting:  We count each arithmetic operation (e.g., addition, multiplication) separately.

   For the complex numbers version, note that
       1 complex addition = 2 flops
       1 complex multiplication = 6 flops,
   where we define 1 flop as that for a double precision scalar.  We roughly approximate
   flop counting for complex numbers by multiplying the total flops by 4; this corresponds
   to the assumption that we're counting mostly additions and multiplications -- and
   roughly the same number of each.  More accurate counting could be done by distinguishing
   among the various arithmetic operations.
 */

#if defined(WHALE_USE_COMPLEX)
#define WHALE_FLOPS_PER_OP 4.0
#else
#define WHALE_FLOPS_PER_OP 1.0
#endif

#undef __FUNCT__
#define __FUNCT__ "WhaleLogFlops"
WHALE_STATIC_INLINE WhaleErrorCode WhaleLogFlops(WhaleLogDouble n)
{
  WhaleFunctionBegin;
#if defined(WHALE_USE_DEBUG)
  if (n < 0) SETERRQ(WHALE_COMM_SELF,WHALE_ERR_ARG_OUTOFRANGE,"Cannot log negative flops");
#endif
  whale_TotalFlops += WHALE_FLOPS_PER_OP*n;
  WhaleFunctionReturn(0);
}

#if defined (WHALE_HAVE_MPE)
WHALE_EXTERN WhaleErrorCode WhaleLogMPEBegin(void);
WHALE_EXTERN WhaleErrorCode WhaleLogMPEDump(const char[]);
#endif

WHALE_EXTERN WhaleErrorCode (*WhaleLogPLB)(WhaleLogEvent,int,WhaleObject,WhaleObject,WhaleObject,WhaleObject);
WHALE_EXTERN WhaleErrorCode (*WhaleLogPLE)(WhaleLogEvent,int,WhaleObject,WhaleObject,WhaleObject,WhaleObject);
WHALE_EXTERN WhaleErrorCode (*WhaleLogPHC)(WhaleObject);
WHALE_EXTERN WhaleErrorCode (*WhaleLogPHD)(WhaleObject);

#define WhaleLogObjectParent(p,c) \
  (c && p && (((WhaleObject)(c))->parent = (WhaleObject)(p),((WhaleObject)(c))->parentid = ((WhaleObject)p)->id,0))

#define WhaleLogObjectParents(p,n,d)  0;{int _i; for (_i=0; _i<n; _i++) {ierr = WhaleLogObjectParent(p,(d)[_i]);CHKERRQ(ierr);}}
#define WhaleLogObjectCreate(h)      ((WhaleLogPHC) ? (*WhaleLogPHC)((WhaleObject)h) : 0)
#define WhaleLogObjectDestroy(h)     ((WhaleLogPHD) ? (*WhaleLogPHD)((WhaleObject)h) : 0)
#define WhaleLogObjectMemory(p,m)    (((WhaleObject)(p))->mem += (m),0)
/* Initialization functions */
WHALE_EXTERN WhaleErrorCode WhaleLogBegin(void);
WHALE_EXTERN WhaleErrorCode WhaleLogAllBegin(void);
WHALE_EXTERN WhaleErrorCode WhaleLogTraceBegin(FILE *);
WHALE_EXTERN WhaleErrorCode WhaleLogActions(WhaleBool);
WHALE_EXTERN WhaleErrorCode WhaleLogObjects(WhaleBool);
/* General functions */
WHALE_EXTERN WhaleErrorCode WhaleLogDestroy(void);
WHALE_EXTERN WhaleErrorCode WhaleLogSet(WhaleErrorCode (*)(int, int, WhaleObject, WhaleObject, WhaleObject, WhaleObject),
                                   WhaleErrorCode (*)(int, int, WhaleObject, WhaleObject, WhaleObject, WhaleObject));
WHALE_EXTERN WhaleErrorCode WhaleLogObjectState(WhaleObject, const char[], ...);
/* Output functions */
WHALE_EXTERN WhaleErrorCode WhaleLogView(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleLogViewPython(WhaleViewer);
WHALE_EXTERN WhaleErrorCode WhaleLogPrintDetailed(MPI_Comm, const char[]);
WHALE_EXTERN WhaleErrorCode WhaleLogDump(const char[]);

WHALE_EXTERN WhaleErrorCode WhaleGetFlops(WhaleLogDouble *);

WHALE_EXTERN WhaleErrorCode WhaleLogStageRegister(const char[],WhaleLogStage*);
WHALE_EXTERN WhaleErrorCode WhaleLogStagePush(WhaleLogStage);
WHALE_EXTERN WhaleErrorCode WhaleLogStagePop(void);
WHALE_EXTERN WhaleErrorCode WhaleLogStageSetActive(WhaleLogStage, WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleLogStageGetActive(WhaleLogStage, WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleLogStageSetVisible(WhaleLogStage, WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleLogStageGetVisible(WhaleLogStage, WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleLogStageGetId(const char [], WhaleLogStage *);
/* Event functions */
WHALE_EXTERN WhaleErrorCode WhaleLogEventRegister(const char[], WhaleClassId,WhaleLogEvent*);
WHALE_EXTERN WhaleErrorCode WhaleLogEventActivate(WhaleLogEvent);
WHALE_EXTERN WhaleErrorCode WhaleLogEventDeactivate(WhaleLogEvent);
WHALE_EXTERN WhaleErrorCode WhaleLogEventSetActiveAll(WhaleLogEvent, WhaleBool );
WHALE_EXTERN WhaleErrorCode WhaleLogEventActivateClass(WhaleClassId);
WHALE_EXTERN WhaleErrorCode WhaleLogEventDeactivateClass(WhaleClassId);


/* Global counters */
WHALE_EXTERN WhaleLogDouble whale_irecv_ct;
WHALE_EXTERN WhaleLogDouble whale_isend_ct;
WHALE_EXTERN WhaleLogDouble whale_recv_ct;
WHALE_EXTERN WhaleLogDouble whale_send_ct;
WHALE_EXTERN WhaleLogDouble whale_irecv_len;
WHALE_EXTERN WhaleLogDouble whale_isend_len;
WHALE_EXTERN WhaleLogDouble whale_recv_len;
WHALE_EXTERN WhaleLogDouble whale_send_len;
WHALE_EXTERN WhaleLogDouble whale_allreduce_ct;
WHALE_EXTERN WhaleLogDouble whale_gather_ct;
WHALE_EXTERN WhaleLogDouble whale_scatter_ct;
WHALE_EXTERN WhaleLogDouble whale_wait_ct;
WHALE_EXTERN WhaleLogDouble whale_wait_any_ct;
WHALE_EXTERN WhaleLogDouble whale_wait_all_ct;
WHALE_EXTERN WhaleLogDouble whale_sum_of_waits_ct;

#define WhaleLogEventBarrierBegin(e,o1,o2,o3,o4,cm) \
  (((WhaleLogPLB && whale_stageLog->stageInfo[whale_stageLog->curStage].perfInfo.active &&  whale_stageLog->stageInfo[whale_stageLog->curStage].eventLog->eventInfo[e].active) ? \
    (WhaleLogEventBegin((e),o1,o2,o3,o4) || MPI_Barrier(cm) || WhaleLogEventEnd((e),o1,o2,o3,o4)) : 0 ) || \
   WhaleLogEventBegin((e)+1,o1,o2,o3,o4))

#define WhaleLogEventBegin(e,o1,o2,o3,o4) \
  (((WhaleLogPLB && whale_stageLog->stageInfo[whale_stageLog->curStage].perfInfo.active && whale_stageLog->stageInfo[whale_stageLog->curStage].eventLog->eventInfo[e].active) ? \
    (*WhaleLogPLB)((e),0,(WhaleObject)(o1),(WhaleObject)(o2),(WhaleObject)(o3),(WhaleObject)(o4)) : 0 ))

#define WhaleLogEventBarrierEnd(e,o1,o2,o3,o4,cm) WhaleLogEventEnd(e+1,o1,o2,o3,o4)

#define WhaleLogEventEnd(e,o1,o2,o3,o4) \
  (((WhaleLogPLE && whale_stageLog->stageInfo[whale_stageLog->curStage].perfInfo.active && whale_stageLog->stageInfo[whale_stageLog->curStage].eventLog->eventInfo[e].active) ? \
    (*WhaleLogPLE)((e),0,(WhaleObject)(o1),(WhaleObject)(o2),(WhaleObject)(o3),(WhaleObject)(o4)) : 0 ))

WHALE_EXTERN WhaleErrorCode WhaleLogEventGetFlops(WhaleLogEvent, WhaleLogDouble*);
WHALE_EXTERN WhaleErrorCode WhaleLogEventZeroFlops(WhaleLogEvent);

/*
     These are used internally in the Whale routines to keep a count of MPI messages and
   their sizes.

     This does not work for MPI-Uni because our include/mpiuni/mpi.h file
   uses macros to defined the MPI operations.

     It does not work correctly from HP-UX because it processes the
   macros in a way that sometimes it double counts, hence
   WHALE_HAVE_BROKEN_RECURSIVE_MACRO

     It does not work with Windows because winmpich lacks MPI_Type_size()
*/
#if !defined(__MPIUNI_H) && !defined(WHALE_HAVE_BROKEN_RECURSIVE_MACRO) && !defined (WHALE_HAVE_MPI_MISSING_TYPESIZE)
/*
   Logging of MPI activities
*/
WHALE_STATIC_INLINE WhaleErrorCode WhaleMPITypeSize(WhaleLogDouble *buff,WhaleMPIInt count,MPI_Datatype type)
{
  WhaleMPIInt mysize; return  (MPI_Type_size(type,&mysize) || ((*buff += (WhaleLogDouble) (count*mysize)),0));
}

WHALE_STATIC_INLINE WhaleErrorCode WhaleMPITypeSizeComm(MPI_Comm comm, WhaleLogDouble *buff,WhaleMPIInt *counts,MPI_Datatype type)
{
  WhaleMPIInt mysize, commsize, p;
  WhaleErrorCode _myierr;

  _myierr = MPI_Comm_size(comm,&commsize);CHKERRQ(_myierr);
  _myierr = MPI_Type_size(type,&mysize);CHKERRQ(_myierr);
  for (p = 0; p < commsize; ++p) {
    *buff += (WhaleLogDouble) (counts[p]*mysize);
  }
  return 0;
}

#define MPI_Irecv(buf,count,datatype,source,tag,comm,request) \
 ((whale_irecv_ct++,0) || WhaleMPITypeSize(&whale_irecv_len,count,datatype) || MPI_Irecv(buf,count,datatype,source,tag,comm,request))

#define MPI_Isend(buf,count,datatype,dest,tag,comm,request) \
 ((whale_isend_ct++,0) || WhaleMPITypeSize(&whale_isend_len,count,datatype) || MPI_Isend(buf,count,datatype,dest,tag,comm,request))

#define MPI_Startall_irecv(count,number,requests) \
 ((whale_irecv_ct += (WhaleLogDouble)(number),0) || WhaleMPITypeSize(&whale_irecv_len,count,MPIU_SCALAR) || MPI_Startall(number,requests))

#define MPI_Startall_isend(count,number,requests) \
 ((whale_isend_ct += (WhaleLogDouble)(number),0) || WhaleMPITypeSize(&whale_isend_len,count,MPIU_SCALAR) || MPI_Startall(number,requests))

#define MPI_Start_isend(count,requests) \
 ((whale_isend_ct++,0) || WhaleMPITypeSize(&whale_isend_len,count,MPIU_SCALAR) || MPI_Start(requests))

#define MPI_Recv(buf,count,datatype,source,tag,comm,status) \
 ((whale_recv_ct++,0) || WhaleMPITypeSize(&whale_recv_len,count,datatype) || MPI_Recv(buf,count,datatype,source,tag,comm,status))

#define MPI_Send(buf,count,datatype,dest,tag,comm) \
 ((whale_send_ct++,0) || WhaleMPITypeSize(&whale_send_len,count,datatype) || MPI_Send(buf,count,datatype,dest,tag,comm))

#define MPI_Wait(request,status) \
 ((whale_wait_ct++,whale_sum_of_waits_ct++,0) || MPI_Wait(request,status))

#define MPI_Waitany(a,b,c,d) \
 ((whale_wait_any_ct++,whale_sum_of_waits_ct++,0) || MPI_Waitany(a,b,c,d))

#define MPI_Waitall(count,array_of_requests,array_of_statuses) \
 ((whale_wait_all_ct++,whale_sum_of_waits_ct += (WhaleLogDouble) (count),0) || MPI_Waitall(count,array_of_requests,array_of_statuses))

#define MPI_Allreduce(sendbuf,recvbuf,count,datatype,op,comm) \
 ((whale_allreduce_ct++,0) || MPI_Allreduce(sendbuf,recvbuf,count,datatype,op,comm))

#define MPI_Alltoall(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,comm) \
 ((whale_allreduce_ct++,0) || WhaleMPITypeSize(&whale_send_len,sendcount,sendtype) || MPI_Alltoall(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,comm))

#define MPI_Alltoallv(sendbuf,sendcnts,sdispls,sendtype,recvbuf,recvcnts,rdispls,recvtype,comm) \
 ((whale_allreduce_ct++,0) || WhaleMPITypeSizeComm(comm,&whale_send_len,sendcnts,sendtype) || MPI_Alltoallv(sendbuf,sendcnts,sdispls,sendtype,recvbuf,recvcnts,rdispls,recvtype,comm))

#define MPI_Allgather(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,comm) \
 ((whale_gather_ct++,0) || MPI_Allgather(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,comm))

#define MPI_Allgatherv(sendbuf,sendcount,sendtype,recvbuf,recvcount,displs,recvtype,comm) \
 ((whale_gather_ct++,0) || MPI_Allgatherv(sendbuf,sendcount,sendtype,recvbuf,recvcount,displs,recvtype,comm))

#define MPI_Gather(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,root,comm) \
 ((whale_gather_ct++,0) || WhaleMPITypeSize(&whale_send_len,sendcount,sendtype) || MPI_Gather(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,root,comm))

#define MPI_Gatherv(sendbuf,sendcount,sendtype,recvbuf,recvcount,displs,recvtype,root,comm) \
 ((whale_gather_ct++,0) || WhaleMPITypeSize(&whale_send_len,sendcount,sendtype) || MPI_Gatherv(sendbuf,sendcount,sendtype,recvbuf,recvcount,displs,recvtype,root,comm))

#define MPI_Scatter(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,root,comm) \
 ((whale_scatter_ct++,0) || WhaleMPITypeSize(&whale_recv_len,recvcount,recvtype) || MPI_Scatter(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,root,comm))

#define MPI_Scatterv(sendbuf,sendcount,displs,sendtype,recvbuf,recvcount,recvtype,root,comm) \
  ((whale_scatter_ct++,0) || WhaleMPITypeSize(&whale_recv_len,recvcount,recvtype) || MPI_Scatterv(sendbuf,sendcount,displs,sendtype,recvbuf,recvcount,recvtype,root,comm))

#else

#define MPI_Startall_irecv(count,number,requests) \
 (MPI_Startall(number,requests))

#define MPI_Startall_isend(count,number,requests) \
 (MPI_Startall(number,requests))

#define MPI_Start_isend(count,requests) \
 (MPI_Start(requests))

#endif /* !__MPIUNI_H && ! WHALE_HAVE_BROKEN_RECURSIVE_MACRO */

#else  /* ---Logging is turned off --------------------------------------------*/

#define WhaleLogFlops(n) 0

#define WhaleLogEventActivate(a)   0
#define WhaleLogEventDeactivate(a) 0

#define WhaleLogEventActivateClass(a)   0
#define WhaleLogEventDeactivateClass(a) 0
#define WhaleLogEventSetActiveAll(a,b)  0

#define WhaleLogPLB                        0
#define WhaleLogPLE                        0
#define WhaleLogPHC                        0
#define WhaleLogPHD                        0
#define WhaleGetFlops(a)                (*(a) = 0.0,0)
#define WhaleLogEventBegin(e,o1,o2,o3,o4)   0
#define WhaleLogEventEnd(e,o1,o2,o3,o4)     0
#define WhaleLogEventBarrierBegin(e,o1,o2,o3,o4,cm) 0
#define WhaleLogEventBarrierEnd(e,o1,o2,o3,o4,cm)   0
#define WhaleLogObjectParent(p,c)           0
#define WhaleLogObjectParents(p,n,c)        0
#define WhaleLogObjectCreate(h)             0
#define WhaleLogObjectDestroy(h)            0
#define WhaleLogObjectMemory(p,m)           0
#define WhaleLogDestroy()                   0
#define WhaleLogStagePush(a)                0
#define WhaleLogStagePop()                  0
#define WhaleLogStageRegister(a,b)          0
#define WhaleLogStagePrint(a,flg)           0
#define WhaleLogView(viewer)                0
#define WhaleLogViewPython(viewer)          0
#define WhaleLogPrintDetailed(comm,file)    0
#define WhaleLogBegin()                     0
#define WhaleLogTraceBegin(file)            0
#define WhaleLogSet(lb,le)                  0
#define WhaleLogAllBegin()                  0
#define WhaleLogDump(c)                     0
#define WhaleLogEventRegister(a,b,c)        0
#define WhaleLogObjects(a)                  0
#define WhaleLogActions(a)                  0
WHALE_EXTERN WhaleErrorCode WhaleLogObjectState(WhaleObject,const char[],...);

/* If WHALE_USE_LOG is NOT defined, these still need to be! */
#define MPI_Startall_irecv(count,number,requests) MPI_Startall(number,requests)
#define MPI_Startall_isend(count,number,requests) MPI_Startall(number,requests)
#define MPI_Start_isend(count,requests) MPI_Start(requests)
#define WhaleLogStageGetId(a,b)                      (*(b)=0,0)
#define WhaleLogStageSetActive(a,b)                  0
#define WhaleLogStageGetActive(a,b)                  0
#define WhaleLogStageGetVisible(a,b)                 0
#define WhaleLogStageSetVisible(a,b)                 0

#endif   /* WHALE_USE_LOG */

WHALE_EXTERN WhaleErrorCode WhaleIntStackCreate(WhaleIntStack *);
WHALE_EXTERN WhaleErrorCode WhaleIntStackDestroy(WhaleIntStack);
WHALE_EXTERN WhaleErrorCode WhaleIntStackPush(WhaleIntStack, int);
WHALE_EXTERN WhaleErrorCode WhaleIntStackPop(WhaleIntStack, int *);
WHALE_EXTERN WhaleErrorCode WhaleIntStackTop(WhaleIntStack, int *);
WHALE_EXTERN WhaleErrorCode WhaleIntStackEmpty(WhaleIntStack, WhaleBool  *);

/* Special support for C++ */
#if defined(WHALE_CLANGUAGE_CXX) && defined(__cplusplus)
#include <whalelog.hh>
#endif

#define WhalePreLoadBegin(flag,name) \
do {\
  WhaleBool      WhalePreLoading = flag;\
  int            WhalePreLoadMax,WhalePreLoadIt;\
  WhaleLogStage  _stageNum;\
  WhaleErrorCode _3_ierr; \
  _3_ierr = WhaleOptionsGetBool(NULL,"-preload",&WhalePreLoading,NULL);CHKERRQ(_3_ierr);\
  WhalePreLoadMax = (int)(WhalePreLoading);\
  WhalePreLoadingUsed = WhalePreLoading ? WHALE_TRUE : WhalePreLoadingUsed;\
  for (WhalePreLoadIt=0; WhalePreLoadIt<=WhalePreLoadMax; WhalePreLoadIt++) {\
    WhalePreLoadingOn = WhalePreLoading;\
    _3_ierr = WhaleBarrier(NULL);CHKERRQ(_3_ierr);\
    if (WhalePreLoadIt>0) {\
      _3_ierr = WhaleLogStageGetId(name,&_stageNum);CHKERRQ(_3_ierr);\
    } else {\
      _3_ierr = WhaleLogStageRegister(name,&_stageNum);CHKERRQ(_3_ierr); \
    }\
    _3_ierr = WhaleLogStageSetActive(_stageNum,(WhaleBool)(!WhalePreLoadMax || WhalePreLoadIt));\
    _3_ierr = WhaleLogStagePush(_stageNum);CHKERRQ(_3_ierr);

#define WhalePreLoadEnd() \
    _3_ierr = WhaleLogStagePop();CHKERRQ(_3_ierr);\
    WhalePreLoading = WHALE_FALSE;\
  }\
} while (0)

#define WhalePreLoadStage(name) do {                                         \
    _3_ierr = WhaleLogStagePop();CHKERRQ(_3_ierr);                      \
    if (WhalePreLoadIt>0) {                                                  \
      _3_ierr = WhaleLogStageGetId(name,&_stageNum);CHKERRQ(_3_ierr);   \
    } else {                                                            \
      _3_ierr = WhaleLogStageRegister(name,&_stageNum);CHKERRQ(_3_ierr); \
    }                                                                   \
    _3_ierr = WhaleLogStageSetActive(_stageNum,(WhaleBool)(!WhalePreLoadMax || WhalePreLoadIt)); \
    _3_ierr = WhaleLogStagePush(_stageNum);CHKERRQ(_3_ierr);            \
  } while (0)

/* some vars for logging */
WHALE_EXTERN WhaleBool WhalePreLoadingUsed;       /* true if we are or have done preloading */
WHALE_EXTERN WhaleBool WhalePreLoadingOn;         /* true if we are currently in a preloading calculation */

/* Reset __FUNCT__ in case the user does not define it themselves */
#undef __FUNCT__
#define __FUNCT__ "User provided function"

#endif
