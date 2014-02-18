 /*
    Contains all error handling interfaces for Whale.
*/
#if !defined(__WHALEERROR_H)
#define __WHALEERROR_H

/*
   Defines the directory where the compiled source is located; used
   in printing error messages. Each makefile has an entry
   LOCDIR  =  thedirectory
   and bmake/common_variables includes in CCPPFLAGS -D__SDIR__=${LOCDIR}
   which is a flag passed to the C/C++ compilers. This declaration below
   is only needed if some code is compiled without the -D__SDIR__
*/
#if !defined(__INSDIR__)
#define __INSDIR__ "unknowndirectory/"
#endif

/*
   Defines the function where the compiled source is located; used
   in printing error messages. This is defined here in case the user
   does not declare it.
*/
#if !defined(__FUNCT__)
#define __FUNCT__ "User provided function"
#endif

/*
     These are the generic error codes. These error codes are used
     many different places in the Whale source code. The string versions are
     at src/sys/error/err.c any changes here must also be made there
     These are also define in include/finclude/whaleerror.h any CHANGES here
     must be also made there.

*/
#define WHALE_ERR_MIN_VALUE        54   /* should always be one less then the smallest value */

#define WHALE_ERR_MEM              55   /* unable to allocate requested memory */
#define WHALE_ERR_SUP              56   /* no support for requested operation */
#define WHALE_ERR_SUP_SYS          57   /* no support for requested operation on this computer system */
#define WHALE_ERR_ORDER            58   /* operation done in wrong order */
#define WHALE_ERR_SIG              59   /* signal received */
#define WHALE_ERR_FP               72   /* floating point exception */
#define WHALE_ERR_COR              74   /* corrupted Whale object */
#define WHALE_ERR_LIB              76   /* error in library called by Whale */
#define WHALE_ERR_PLIB             77   /* Whale library generated inconsistent data */
#define WHALE_ERR_MEMC             78   /* memory corruption */
#define WHALE_ERR_CONV_FAILED      82   /* iterative method (KSP or SNES) failed */
#define WHALE_ERR_USER             83   /* user has not provided needed function */
#define WHALE_ERR_SYS              88   /* error in system call */
#define WHALE_ERR_POINTER          70   /* pointer does not point to valid address */

#define WHALE_ERR_ARG_SIZ          60   /* nonconforming object sizes used in operation */
#define WHALE_ERR_ARG_IDN          61   /* two arguments not allowed to be the same */
#define WHALE_ERR_ARG_WRONG        62   /* wrong argument (but object probably ok) */
#define WHALE_ERR_ARG_CORRUPT      64   /* null or corrupted Whale object as argument */
#define WHALE_ERR_ARG_OUTOFRANGE   63   /* input argument, out of range */
#define WHALE_ERR_ARG_BADPTR       68   /* invalid pointer argument */
#define WHALE_ERR_ARG_NOTSAMETYPE  69   /* two args must be same object type */
#define WHALE_ERR_ARG_NOTSAMECOMM  80   /* two args must be same communicators */
#define WHALE_ERR_ARG_WRONGSTATE   73   /* object in argument is in wrong state, e.g. unassembled mat */
#define WHALE_ERR_ARG_TYPENOTSET   89   /* the type of the object has not yet been set */
#define WHALE_ERR_ARG_INCOMP       75   /* two arguments are incompatible */
#define WHALE_ERR_ARG_NULL         85   /* argument is null that should not be */
#define WHALE_ERR_ARG_UNKNOWN_TYPE 86   /* type name doesn't match any registered type */

#define WHALE_ERR_FILE_OPEN        65   /* unable to open file */
#define WHALE_ERR_FILE_READ        66   /* unable to read from file */
#define WHALE_ERR_FILE_WRITE       67   /* unable to write to file */
#define WHALE_ERR_FILE_UNEXPECTED  79   /* unexpected data in file */

#define WHALE_ERR_MAT_LU_ZRPVT     71   /* detected a zero pivot during LU factorization */
#define WHALE_ERR_MAT_CH_ZRPVT     81   /* detected a zero pivot during Cholesky factorization */

#define WHALE_ERR_INT_OVERFLOW     84   /* should always be one less then the smallest value */

#define WHALE_ERR_FLOP_COUNT       90
#define WHALE_ERR_NOT_CONVERGED    91  /* solver did not converge */
#define WHALE_ERR_MAX_VALUE        92  /* this is always the one more than the largest error code */

#define WhaleStringizeArg(a) #a
#define WhaleStringize(a) WhaleStringizeArg(a)
#define __SDIR__ WhaleStringize(__INSDIR__)

#if defined(WHALE_USE_ERRORCHECKING)

/*MC
   SETERRQ - Macro that is called when an error has been detected,

   Synopsis:
   #include "whalesys.h"
   WhaleErrorCode SETERRQ(MPI_Comm comm,WhaleErrorCode errorcode,char *message)

   Not Collective

   Input Parameters:
+  errorcode - nonzero error code, see the list of standard error codes in include/whaleerror.h
-  message - error message

  Level: beginner

   Notes:
    Once the error handler is called the calling function is then returned from with the given error code.

    See SETERRQ1(), SETERRQ2(), SETERRQ3() for versions that take arguments

    In Fortran MPI_Abort() is always called

    Experienced users can set the error handler with WhalePushErrorHandler().

   Concepts: error^setting condition

.seealso: WhaleTraceBackErrorHandler(), WhalePushErrorHandler(), WhaleError(), CHKERRQ(), CHKMEMQ, SETERRQ1(), SETERRQ2(), SETERRQ3()
M*/
#define SETERRQ(comm,n,s)              return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s)

/*MC
   SETERRQ1 - Macro that is called when an error has been detected,

   Synopsis:
   #include "whalesys.h"
   WhaleErrorCode SETERRQ1(MPI_Comm comm,WhaleErrorCode errorcode,char *formatmessage,arg)

   Not Collective

   Input Parameters:
+  errorcode - nonzero error code, see the list of standard error codes in include/whaleerror.h
.  message - error message in the printf format
-  arg - argument (for example an integer, string or double)

  Level: beginner

   Notes:
    Once the error handler is called the calling function is then returned from with the given error code.

   Experienced users can set the error handler with WhalePushErrorHandler().

   Concepts: error^setting condition

.seealso: WhaleTraceBackErrorHandler(), WhalePushErrorHandler(), WhaleError(), CHKERRQ(), CHKMEMQ, SETERRQ(), SETERRQ2(), SETERRQ3()
M*/
#define SETERRQ1(comm,n,s,a1)          return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s,a1)

/*MC
   SETERRQ2 - Macro that is called when an error has been detected,

   Synopsis:
   #include "whalesys.h"
   WhaleErrorCode SETERRQ2(WhaleErrorCode errorcode,char *formatmessage,arg1,arg2)

   Not Collective

   Input Parameters:
+  errorcode - nonzero error code, see the list of standard error codes in include/whaleerror.h
.  message - error message in the printf format
.  arg1 - argument (for example an integer, string or double)
-  arg2 - argument (for example an integer, string or double)

  Level: beginner

   Notes:
    Once the error handler is called the calling function is then returned from with the given error code.

   Experienced users can set the error handler with WhalePushErrorHandler().

   Concepts: error^setting condition

.seealso: WhaleTraceBackErrorHandler(), WhalePushErrorHandler(), WhaleError(), CHKERRQ(), CHKMEMQ, SETERRQ1(), SETERRQ3()
M*/
#define SETERRQ2(comm,n,s,a1,a2)       return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s,a1,a2)

/*MC
   SETERRQ3 - Macro that is called when an error has been detected,

   Synopsis:
   #include "whalesys.h"
   WhaleErrorCode SETERRQ3(WhaleErrorCode errorcode,char *formatmessage,arg1,arg2,arg3)

   Not Collective

   Input Parameters:
+  errorcode - nonzero error code, see the list of standard error codes in include/whaleerror.h
.  message - error message in the printf format
.  arg1 - argument (for example an integer, string or double)
.  arg2 - argument (for example an integer, string or double)
-  arg3 - argument (for example an integer, string or double)

  Level: beginner

   Notes:
    Once the error handler is called the calling function is then returned from with the given error code.

    There are also versions for 4, 5, 6 and 7 arguments.

   Experienced users can set the error handler with WhalePushErrorHandler().

   Concepts: error^setting condition

.seealso: WhaleTraceBackErrorHandler(), WhalePushErrorHandler(), WhaleError(), CHKERRQ(), CHKMEMQ, SETERRQ1(), SETERRQ2()
M*/
#define SETERRQ3(comm,n,s,a1,a2,a3)    return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s,a1,a2,a3)

#define SETERRQ4(comm,n,s,a1,a2,a3,a4) return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s,a1,a2,a3,a4)
#define SETERRQ5(comm,n,s,a1,a2,a3,a4,a5)       return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s,a1,a2,a3,a4,a5)
#define SETERRQ6(comm,n,s,a1,a2,a3,a4,a5,a6)    return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s,a1,a2,a3,a4,a5,a6)
#define SETERRQ7(comm,n,s,a1,a2,a3,a4,a5,a6,a7) return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s,a1,a2,a3,a4,a5,a6,a7)
#define SETERRQ8(comm,n,s,a1,a2,a3,a4,a5,a6,a7,a8) return WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s,a1,a2,a3,a4,a5,a6,a7,a8)
#define SETERRABORT(comm,n,s)     do {WhaleError(comm,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_INITIAL,s);MPI_Abort(comm,n);} while (0)

/*MC
   CHKERRQ - Checks error code, if non-zero it calls the error handler and then returns

   Synopsis:
   #include "whalesys.h"
   WhaleErrorCode CHKERRQ(WhaleErrorCode errorcode)

   Not Collective

   Input Parameters:
.  errorcode - nonzero error code, see the list of standard error codes in include/whaleerror.h

  Level: beginner

   Notes:
    Once the error handler is called the calling function is then returned from with the given error code.

    Experienced users can set the error handler with WhalePushErrorHandler().

    CHKERRQ(n) is fundamentally a macro replacement for
         if (n) return(WhaleError(...,n,...));

    Although typical usage resembles "void CHKERRQ(WhaleErrorCode)" as described above, for certain uses it is
    highly inappropriate to use it in this manner as it invokes return(WhaleErrorCode). In particular,
    it cannot be used in functions which return(void) or any other datatype.  In these types of functions,
    you can use CHKERRV() which returns without an error code (bad idea since the error is ignored or
         if (n) {WhaleError(....); return(YourReturnType);}
    where you may pass back a NULL to indicate an error. You can also call CHKERRABORT(comm,n) to have
    MPI_Abort() returned immediately.

    In Fortran MPI_Abort() is always called

   Concepts: error^setting condition

.seealso: WhaleTraceBackErrorHandler(), WhalePushErrorHandler(), WhaleError(), SETERRQ(), CHKMEMQ, SETERRQ1(), SETERRQ2(), SETERRQ2()
M*/
#define CHKERRQ(n)             do {if (WhaleUnlikely(n)) return WhaleError(WHALE_COMM_SELF,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_REPEAT," ");} while (0)

#define CHKERRV(n)             do {if (WhaleUnlikely(n)) {n = WhaleError(WHALE_COMM_SELF,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_REPEAT," ");return;}} while(0)
#define CHKERRABORT(comm,n)    do {if (WhaleUnlikely(n)) {WhaleError(WHALE_COMM_SELF,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_REPEAT," ");MPI_Abort(comm,n);}} while (0)
#define CHKERRCONTINUE(n)      do {if (WhaleUnlikely(n)) {WhaleError(WHALE_COMM_SELF,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_REPEAT," ");}} while (0)

#ifdef WHALE_CLANGUAGE_CXX

/*MC
   CHKERRXX - Checks error code, if non-zero it calls the C++ error handler which throws an exception

   Synopsis:
   #include "whalesys.h"
   void CHKERRXX(WhaleErrorCode errorcode)

   Not Collective

   Input Parameters:
.  errorcode - nonzero error code, see the list of standard error codes in include/whaleerror.h

  Level: beginner

   Notes:
    Once the error handler throws a ??? exception.

    You can use CHKERRV() which returns without an error code (bad idea since the error is ignored)
    or CHKERRABORT(comm,n) to have MPI_Abort() returned immediately.

   Concepts: error^setting condition

.seealso: WhaleTraceBackErrorHandler(), WhalePushErrorHandler(), WhaleError(), SETERRQ(), CHKERRQ(), CHKMEMQ
M*/
#define CHKERRXX(n)            do {if (WhaleUnlikely(n)) {WhaleError(WHALE_COMM_SELF,__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__,n,WHALE_ERROR_IN_CXX,0);}} while(0)

#endif

/*MC
   CHKMEMQ - Checks the memory for corruption, calls error handler if any is detected

   Synopsis:
   #include "whalesys.h"
   CHKMEMQ;

   Not Collective

  Level: beginner

   Notes:
    Must run with the option -malloc_debug to enable this option

    Once the error handler is called the calling function is then returned from with the given error code.

    By defaults prints location where memory that is corrupted was allocated.

    Use CHKMEMA for functions that return void

   Concepts: memory corruption

.seealso: WhaleTraceBackErrorHandler(), WhalePushErrorHandler(), WhaleError(), SETERRQ(), CHKMEMQ, SETERRQ1(), SETERRQ2(), SETERRQ3(),
          WhaleMallocValidate()
M*/
#define CHKMEMQ do {WhaleErrorCode _7_ierr = WhaleMallocValidate(__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__);CHKERRQ(_7_ierr);} while(0)

#define CHKMEMA WhaleMallocValidate(__LINE__,WHALE_FUNCTION_NAME,__FILE__,__SDIR__)

#else /* WHALE_USE_ERRORCHECKING */

/*
    These are defined to be empty for when error checking is turned off, with ./configure --with-errorchecking=0
*/

#define SETERRQ(c,n,s)
#define SETERRQ1(c,n,s,a1)
#define SETERRQ2(c,n,s,a1,a2)
#define SETERRQ3(c,n,s,a1,a2,a3)
#define SETERRQ4(c,n,s,a1,a2,a3,a4)
#define SETERRQ5(c,n,s,a1,a2,a3,a4,a5)
#define SETERRQ6(c,n,s,a1,a2,a3,a4,a5,a6)
#define SETERRQ7(c,n,s,a1,a2,a3,a4,a5,a6,a7)
#define SETERRQ8(c,n,s,a1,a2,a3,a4,a5,a6,a7,a8)
#define SETERRABORT(comm,n,s)

#define CHKERRQ(n)     ;
#define CHKERRABORT(comm,n) ;
#define CHKERRCONTINUE(n) ;
#define CHKMEMQ        ;

#ifdef WHALE_CLANGUAGE_CXX
#define CHKERRXX(n) ;
#endif

#endif /* WHALE_USE_ERRORCHECKING */

/*E
  WhaleErrorType - passed to the Whale error handling routines indicating if this is the first or a later call to the error handlers

  Level: advanced

  WHALE_ERROR_IN_CXX indicates the error was detected in C++ and an exception should be generated

  Developer Notes: This is currently used to decide when to print the detailed information about the run in WhaleTraceBackErrorHandling()

.seealso: WhaleError(), SETERRXX()
E*/
typedef enum {WHALE_ERROR_INITIAL=0,WHALE_ERROR_REPEAT=1,WHALE_ERROR_IN_CXX = 2} WhaleErrorType;

WHALE_EXTERN WhaleErrorCode WhaleErrorPrintfInitialize(void);
WHALE_EXTERN WhaleErrorCode WhaleErrorMessage(int,const char*[],char **);
WHALE_EXTERN WhaleErrorCode WhaleTraceBackErrorHandler(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,void*);
WHALE_EXTERN WhaleErrorCode WhaleIgnoreErrorHandler(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,void*);
WHALE_EXTERN WhaleErrorCode WhaleEmacsClientErrorHandler(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,void*);
WHALE_EXTERN WhaleErrorCode WhaleMPIAbortErrorHandler(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,void*);
WHALE_EXTERN WhaleErrorCode WhaleAbortErrorHandler(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,void*);
WHALE_EXTERN WhaleErrorCode WhaleAttachDebuggerErrorHandler(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,void*);
WHALE_EXTERN WhaleErrorCode WhaleReturnErrorHandler(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,void*);
WHALE_EXTERN WhaleErrorCode WhaleError(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,...);
WHALE_EXTERN WhaleErrorCode WhalePushErrorHandler(WhaleErrorCode (*handler)(MPI_Comm,int,const char*,const char*,const char*,WhaleErrorCode,WhaleErrorType,const char*,void*),void*);
WHALE_EXTERN WhaleErrorCode WhalePopErrorHandler(void);
WHALE_EXTERN WhaleErrorCode WhaleSignalHandlerDefault(int,void*);
WHALE_EXTERN WhaleErrorCode WhalePushSignalHandler(WhaleErrorCode (*)(int,void *),void*);
WHALE_EXTERN WhaleErrorCode WhalePopSignalHandler(void);

/*MC
    WhaleErrorPrintf - Prints error messages.

   Synopsis:
    #include "whalesys.h"
     WhaleErrorCode (*WhaleErrorPrintf)(const char format[],...);

    Not Collective

    Input Parameters:
.   format - the usual printf() format string

   Options Database Keys:
+    -error_output_stdout - cause error messages to be printed to stdout instead of the
         (default) stderr
-    -error_output_none to turn off all printing of error messages (does not change the way the
          error is handled.)

   Notes: Use
$     WhaleErrorPrintf = WhaleErrorPrintfNone; to turn off all printing of error messages (does not change the way the
$                        error is handled.) and
$     WhaleErrorPrintf = WhaleErrorPrintfDefault; to turn it back on
$        of you can use your own function

          Use
     WHALE_STDERR = FILE* obtained from a file open etc. to have stderr printed to the file.
     WHALE_STDOUT = FILE* obtained from a file open etc. to have stdout printed to the file.

          Use
      WhalePushErrorHandler() to provide your own error handler that determines what kind of messages to print

   Level: developer

    Fortran Note:
    This routine is not supported in Fortran.

    Concepts: error messages^printing
    Concepts: printing^error messages

.seealso: WhaleFPrintf(), WhaleSynchronizedPrintf(), WhaleHelpPrintf(), WhalePrintf(), WhaleErrorHandlerPush(), WhaleVFPrintf(), WhaleHelpPrintf()
M*/
WHALE_EXTERN WhaleErrorCode (*WhaleErrorPrintf)(const char[],...);

typedef enum {WHALE_FP_TRAP_OFF=0,WHALE_FP_TRAP_ON=1} WhaleFPTrap;
WHALE_EXTERN WhaleErrorCode WhaleSetFPTrap(WhaleFPTrap);
WHALE_EXTERN WhaleErrorCode WhaleFPTrapPush(WhaleFPTrap);
WHALE_EXTERN WhaleErrorCode WhaleFPTrapPop(void);

/*  Linux functions CPU_SET and others don't work if sched.h is not included before
    including pthread.h. Also, these functions are active only if either _GNU_SOURCE
    or __USE_GNU is not set (see /usr/include/sched.h and /usr/include/features.h), hence
    set these first.
*/
#if defined(WHALE_HAVE_PTHREADCLASSES)
#if defined(WHALE_HAVE_SCHED_H)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <sched.h>
#endif
#include <pthread.h>
#endif

/*
     This code is for managing thread local global variables. Each of Linux, Microsoft WINDOWS, OpenMP, and Apple OS X have
   different ways to indicate this. On OS X each thread local global is accessed by using a pthread_key_t for that variable.
   Thus we have functions for creating destroying and using the keys. Except for OS X these access functions merely directly
   acess the thread local variable.
*/

#if defined(WHALE_HAVE_PTHREADCLASSES) && !defined(WHALE_PTHREAD_LOCAL)
typedef pthread_key_t WhaleThreadKey;
/* Get the value associated with key */
WHALE_STATIC_INLINE void* WhaleThreadLocalGetValue(WhaleThreadKey key)
{
  return pthread_getspecific(key);
}

/* Set the value for key */
WHALE_STATIC_INLINE void WhaleThreadLocalSetValue(WhaleThreadKey *key,void* value)
{
  pthread_setspecific(*key,(void*)value);
}

/* Create pthread thread local key */
WHALE_STATIC_INLINE void WhaleThreadLocalRegister(WhaleThreadKey *key)
{
  pthread_key_create(key,NULL);
}

/* Delete pthread thread local key */
WHALE_STATIC_INLINE void WhaleThreadLocalDestroy(WhaleThreadKey key)
{
  pthread_key_delete(key);
}
#else
typedef void* WhaleThreadKey;
WHALE_STATIC_INLINE void* WhaleThreadLocalGetValue(WhaleThreadKey key)
{
  return key;
}

WHALE_STATIC_INLINE void WhaleThreadLocalSetValue(WhaleThreadKey *key,void* value)
{
  *key = value;
}

WHALE_STATIC_INLINE void WhaleThreadLocalRegister(WHALE_UNUSED WhaleThreadKey *key)
{
}

WHALE_STATIC_INLINE void WhaleThreadLocalDestroy(WHALE_UNUSED WhaleThreadKey key)
{
}
#endif

/*
      Allows the code to build a stack frame as it runs
*/
#if defined(WHALE_USE_DEBUG)

#define WHALESTACKSIZE 64

typedef struct  {
  const char      *function[WHALESTACKSIZE];
  const char      *file[WHALESTACKSIZE];
  const char      *directory[WHALESTACKSIZE];
        int       line[WHALESTACKSIZE];
        WhaleBool whaleroutine[WHALESTACKSIZE];
        int       currentsize;
} WhaleStack;

#if defined(WHALE_HAVE_PTHREADCLASSES)
#if defined(WHALE_PTHREAD_LOCAL)
WHALE_EXTERN WHALE_PTHREAD_LOCAL WhaleStack *whalestack;
#else
WHALE_EXTERN WhaleThreadKey whalestack;
#endif
#elif defined(WHALE_HAVE_OPENMP)
WHALE_EXTERN WhaleStack *whalestack;
#pragma omp threadprivate(whalestack)
#else
WHALE_EXTERN WhaleStack *whalestack;
#endif

WHALE_EXTERN WhaleErrorCode WhaleStackCopy(WhaleStack*,WhaleStack*);
WHALE_EXTERN WhaleErrorCode WhaleStackPrint(WhaleStack*,FILE* fp);

WHALE_STATIC_INLINE WhaleBool WhaleStackActive(void)
{
  return(WhaleThreadLocalGetValue(whalestack) ? WHALE_TRUE : WHALE_FALSE);
}

/* Stack handling is based on the following two "NoCheck" macros.  These should only be called directly by other error
 * handling macros.  We record the line of the call, which may or may not be the location of the definition.  But is at
 * least more useful than "unknown" because it can distinguish multiple calls from the same function.
 */

#define WhaleStackPushNoCheck(funct,whale_routine)                            \
  do {                                                                        \
    WhaleStack* whalestackp;                                                  \
    WhaleStackAMSTakeAccess();                                                \
    whalestackp = (WhaleStack*)WhaleThreadLocalGetValue(whalestack);          \
    if (whalestackp && (whalestackp->currentsize < WHALESTACKSIZE)) {         \
      whalestackp->function[whalestackp->currentsize]  = funct;               \
      whalestackp->file[whalestackp->currentsize]      = __FILE__;            \
      whalestackp->directory[whalestackp->currentsize] = __SDIR__;            \
      whalestackp->line[whalestackp->currentsize]      = __LINE__;            \
      whalestackp->whaleroutine[whalestackp->currentsize] = whale_routine;    \
      whalestackp->currentsize++;                                             \
    }                                                                         \
    WhaleStackAMSGrantAccess();                                               \
  } while (0)

#define WhaleStackPopNoCheck                                            \
  do {WhaleStack* whalestackp;                                          \
    WhaleStackAMSTakeAccess();                                          \
    whalestackp = (WhaleStack*)WhaleThreadLocalGetValue(whalestack);    \
    if (whalestackp && whalestackp->currentsize > 0) {                  \
      whalestackp->currentsize--;                                       \
      whalestackp->function[whalestackp->currentsize]  = 0;             \
      whalestackp->file[whalestackp->currentsize]      = 0;             \
      whalestackp->directory[whalestackp->currentsize] = 0;             \
      whalestackp->line[whalestackp->currentsize]      = 0;             \
      whalestackp->whaleroutine[whalestackp->currentsize] = WHALE_FALSE;\
    }                                                                   \
    WhaleStackAMSGrantAccess();                                         \
  } while (0)

/*MC
   WhaleFunctionBegin - First executable line of each Whale function
        used for error handling.

   Synopsis:
   #include "whalesys.h"
   void WhaleFunctionBegin;

   Not Collective

   Usage:
.vb
     int something;

     WhaleFunctionBegin;
.ve

   Notes:
     Not available in Fortran

   Level: developer

.seealso: WhaleFunctionReturn()

.keywords: traceback, error handling
M*/
#define WhaleFunctionBegin do {                                   \
    WhaleStackPushNoCheck(WHALE_FUNCTION_NAME,WHALE_TRUE);        \
    WhaleCheck__FUNCT__();                                        \
    WhaleRegister__FUNCT__();                                     \
  } while (0)

/*MC
   WhaleFunctionBeginUser - First executable line of user provided Whale routine

   Synopsis:
   #include "whalesys.h"
   void WhaleFunctionBeginUser;

   Not Collective

   Usage:
.vb
     int something;

     WhaleFunctionBegin;
.ve

   Notes:
     Not available in Fortran

   Level: developer

.seealso: WhaleFunctionReturn()

.keywords: traceback, error handling
M*/
#define WhaleFunctionBeginUser                                          \
  do {                                                                  \
    WhaleStackPushNoCheck(WHALE_FUNCTION_NAME,WHALE_FALSE);             \
    WhaleCheck__FUNCT__();                                              \
    WhaleRegister__FUNCT__();                                           \
  } while (0)


#if defined(WHALE_SERIALIZE_FUNCTIONS)
#include <whale-private/whalefptimpl.h>
/*
   Registers the current function into the global function pointer to function name table

   Have to fix this to handle errors but cannot return error since used in WHALE_VIEWER_DRAW_() etc
*/
#define WhaleRegister__FUNCT__() do { \
  static WhaleBool __chked = WHALE_FALSE; \
  if (!__chked) {\
  void *ptr; WhaleDLSym(NULL,__FUNCT__,&ptr);\
  __chked = WHALE_TRUE;\
  }} while (0)
#else
#define WhaleRegister__FUNCT__()
#endif

#define WhaleCheck__FUNCT__() do { WhaleBool _sc1,_sc2;                  \
    WhaleStrcmpNoError(WHALE_FUNCTION_NAME,__FUNCT__,&_sc1);\
    WhaleStrcmpNoError(__FUNCT__,"User provided function",&_sc2);\
    if (!_sc1 && !_sc2) { \
      printf("%s%s:%d: __FUNCT__=\"%s\" does not agree with %s=\"%s\"\n",__SDIR__,__FILE__,__LINE__,__FUNCT__,WhaleStringize(WHALE_FUNCTION_NAME),WHALE_FUNCTION_NAME); \
    }                                                                   \
  } while (0)

#define WhaleStackPush(n) \
  do {                                                                  \
    WhaleStackPushNoCheck(n,WHALE_FALSE);                               \
    CHKMEMQ;                                                            \
  } while (0)

#define WhaleStackPop                           \
    do {                                        \
      CHKMEMQ;                                  \
      WhaleStackPopNoCheck;                     \
    } while (0)

/*MC
   WhaleFunctionReturn - Last executable line of each Whale function
        used for error handling. Replaces return()

   Synopsis:
   #include "whalesys.h"
   void WhaleFunctionReturn(0);

   Not Collective

   Usage:
.vb
    ....
     WhaleFunctionReturn(0);
   }
.ve

   Notes:
     Not available in Fortran

   Level: developer

.seealso: WhaleFunctionBegin()

.keywords: traceback, error handling
M*/
#define WhaleFunctionReturn(a) \
  do {                                                                \
    WhaleStackPopNoCheck;                                             \
    return(a);} while (0)

#define WhaleFunctionReturnVoid() \
  do {                                                                \
    WhaleStackPopNoCheck;                                             \
    return;} while (0)
#else

#define WhaleStackPushNoCheck(funct,whale_routine) do {} while (0)
#define WhaleStackPopNoCheck                       do {} while (0)
#define WhaleFunctionBegin
#define WhaleFunctionBeginUser
#define WhaleFunctionReturn(a)    return(a)
#define WhaleFunctionReturnVoid() return
#define WhaleStackPop             CHKMEMQ
#define WhaleStackPush(f)         CHKMEMQ
#define WhaleStackActive          WHALE_FALSE

#endif

/*
    WhaleStackCall - Calls an external library routine or user function after pushing the name of the routine on the stack.

   Input Parameters:
+   name - string that gives the name of the function being called
-   routine - actual call to the routine, including ierr = and CHKERRQ(ierr);

   Note: Often one should use WhaleStackCallStandard() instead. This routine is intended for external library routines that DO NOT return error codes

   Developer Note: this is so that when a user or external library routine results in a crash or corrupts memory, they get blamed instead of Whale.



*/
#define WhaleStackCall(name,routine) do { WhaleStackPush(name);routine;WhaleStackPop; } while(0)

/*
    WhaleStackCallStandard - Calls an external library routine after pushing the name of the routine on the stack.

   Input Parameters:
+   func-  name of the routine
-   args - arguments to the routine surrounded by ()

   Notes: This is intended for external package routines that return error codes. Use WhaleStackCall() for those that do not.

   Developer Note: this is so that when an external packge routine results in a crash or corrupts memory, they get blamed instead of Whale.

*/
#define WhaleStackCallStandard(func,args) do {                        \
    WhaleStackPush(#func);ierr = func args;WhaleStackPop; if (ierr) SETERRQ1(WHALE_COMM_SELF,WHALE_ERR_LIB,"Error in %s()",#func); \
  } while (0)

WHALE_EXTERN WhaleErrorCode WhaleStackCreate(void);
WHALE_EXTERN WhaleErrorCode WhaleStackView(FILE*);
WHALE_EXTERN WhaleErrorCode WhaleStackDestroy(void);

#endif
