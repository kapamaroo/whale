/*
   Routines to determine options set in the options database.
*/
#if !defined(__WHALEOPTIONS_H)
#define __WHALEOPTIONS_H
#include <whalesys.h>
#include <whaleviewertypes.h>

WHALE_EXTERN WhaleErrorCode WhaleOptionsHasName(const char[],const char[],WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetInt(const char[],const char [],WhaleInt *,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetBool(const char[],const char [],WhaleBool  *,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetReal(const char[],const char[],WhaleReal *,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetScalar(const char[],const char[],WhaleScalar *,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetIntArray(const char[],const char[],WhaleInt[],WhaleInt *,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetRealArray(const char[],const char[],WhaleReal[],WhaleInt *,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetBoolArray(const char[],const char[],WhaleBool [],WhaleInt *,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetString(const char[],const char[],char[],size_t,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetStringArray(const char[],const char[],char*[],WhaleInt*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetEList(const char[],const char[],const char*const*,WhaleInt,WhaleInt*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetEnum(const char[],const char[],const char*const*,WhaleEnum*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsValidKey(const char[],WhaleBool *);

WHALE_EXTERN WhaleErrorCode WhaleOptionsSetAlias(const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleOptionsSetValue(const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleOptionsClearValue(const char[]);

WHALE_EXTERN WhaleErrorCode WhaleOptionsAllUsed(WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleOptionsUsed(const char *,WhaleBool*);
WHALE_EXTERN WhaleErrorCode WhaleOptionsLeft(void);
WHALE_EXTERN WhaleErrorCode WhaleOptionsView(WhaleViewer);

WHALE_EXTERN WhaleErrorCode WhaleOptionsCreate(void);
WHALE_EXTERN WhaleErrorCode WhaleOptionsInsert(int*,char ***,const char[]);
WHALE_EXTERN WhaleErrorCode WhaleOptionsInsertFile(MPI_Comm,const char[],WhaleBool );
#if defined(WHALE_HAVE_YAML)
WHALE_EXTERN WhaleErrorCode WhaleOptionsInsertFileYAML(MPI_Comm,const char[],WhaleBool);
#endif
WHALE_EXTERN WhaleErrorCode WhaleOptionsInsertString(const char[]);
WHALE_EXTERN WhaleErrorCode WhaleOptionsDestroy(void);
WHALE_EXTERN WhaleErrorCode WhaleOptionsClear(void);
WHALE_EXTERN WhaleErrorCode WhaleOptionsPrefixPush(const char[]);
WHALE_EXTERN WhaleErrorCode WhaleOptionsPrefixPop(void);

WHALE_EXTERN WhaleErrorCode WhaleOptionsReject(const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleOptionsGetAll(char*[]);

WHALE_EXTERN WhaleErrorCode WhaleOptionsGetenv(MPI_Comm,const char[],char[],size_t,WhaleBool  *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsStringToInt(const char[],WhaleInt*);
WHALE_EXTERN WhaleErrorCode WhaleOptionsStringToReal(const char[],WhaleReal*);
WHALE_EXTERN WhaleErrorCode WhaleOptionsStringToBool(const char[],WhaleBool*);

WHALE_EXTERN WhaleErrorCode WhaleOptionsMonitorSet(WhaleErrorCode (*)(const char[], const char[], void*), void *, WhaleErrorCode (*)(void**));
WHALE_EXTERN WhaleErrorCode WhaleOptionsMonitorCancel(void);
WHALE_EXTERN WhaleErrorCode WhaleOptionsMonitorDefault(const char[], const char[], void *);

WHALE_EXTERN WhaleBool WhaleOptionsPublish;
WHALE_EXTERN WhaleInt WhaleOptionsPublishCount;

/*MC
    WhaleOptionsBegin - Begins a set of queries on the options database that are related and should be
     displayed on the same window of a GUI that allows the user to set the options interactively.

   Synopsis:
    #include "whaleoptions.h"
    WhaleErrorCode WhaleOptionsBegin(MPI_Comm comm,const char prefix[],const char title[],const char mansec[])

    Collective on MPI_Comm

  Input Parameters:
+   comm - communicator that shares GUI
.   prefix - options prefix for all options displayed on window
.   title - short descriptive text, for example "Krylov Solver Options"
-   mansec - section of manual pages for options, for example KSP

  Level: intermediate

  Notes: Needs to be ended by a call the WhaleOptionsEnd()
         Can add subheadings with WhaleOptionsHead()

  Developer notes: WhaleOptionsPublish is set in WhaleOptionsCheckInitial_Private() with -options_gui. When WhaleOptionsPublish is set the
$             loop between WhaleOptionsBegin() and WhaleOptionsEnd() is run THREE times with WhaleOptionsPublishCount of values -1,0,1 otherwise
$             the loop is run ONCE with a WhaleOptionsPublishCount of 1.
$             = -1 : The WhaleOptionsInt() etc just call the WhaleOptionsGetInt() etc
$             = 0  : The GUI objects are created in WhaleOptionsInt() etc and displayed in WhaleOptionsEnd() and the options
$                    database updated updated with user changes; WhaleOptionsGetInt() etc are also called
$             = 1 : The WhaleOptionsInt() etc again call the WhaleOptionsGetInt() etc (possibly getting new values), in addition the help message and
$                   default values are printed if -help was given.
$           When WhaleOptionsObject.changedmethod is set this causes WhaleOptionsPublishCount to be reset to -2 (so in the next loop iteration it is -1)
$           and the whole process is repeated. This is to handle when, for example, the KSPType is changed thus changing the list of
$           options available so they need to be redisplayed so the user can change the. Chaning WhaleOptionsObjects.changedmethod is never
$           currently set.


.seealso: WhaleOptionsGetReal(), WhaleOptionsHasName(), WhaleOptionsGetString(), WhaleOptionsGetInt(),
          WhaleOptionsGetIntArray(), WhaleOptionsGetRealArray(), WhaleOptionsBool()
          WhaleOptionsInt(), WhaleOptionsString(), WhaleOptionsReal(), WhaleOptionsBool(),
          WhaleOptionsName(), WhaleOptionsBegin(), WhaleOptionsEnd(), WhaleOptionsHead(),
          WhaleOptionsStringArray(),WhaleOptionsRealArray(), WhaleOptionsScalar(),
          WhaleOptionsBoolGroupBegin(), WhaleOptionsBoolGroup(), WhaleOptionsBoolGroupEnd(),
          WhaleOptionsList(), WhaleOptionsEList(), WhaleObjectOptionsBegin()

M*/
#define    WhaleOptionsBegin(comm,prefix,mess,sec) 0; do {\
             for (WhaleOptionsPublishCount=(WhaleOptionsPublish?-1:1); WhaleOptionsPublishCount<2; WhaleOptionsPublishCount++) {\
             WhaleErrorCode _5_ierr = WhaleOptionsBegin_Private(comm,prefix,mess,sec);CHKERRQ(_5_ierr);

/*MC
    WhaleObjectOptionsBegin - Begins a set of queries on the options database that are related and should be
     displayed on the same window of a GUI that allows the user to set the options interactively.

   Synopsis:
    #include "whaleoptions.h"
    WhaleErrorCode WhaleObjectOptionsBegin(WhaleObject obj)

    Collective on WhaleObject

  Input Parameters:
.   obj - object to set options for

  Level: intermediate

  Notes: Needs to be ended by a call the WhaleOptionsEnd()
         Can add subheadings with WhaleOptionsHead()

.seealso: WhaleOptionsGetReal(), WhaleOptionsHasName(), WhaleOptionsGetString(), WhaleOptionsGetInt(),
          WhaleOptionsGetIntArray(), WhaleOptionsGetRealArray(), WhaleOptionsBool()
          WhaleOptionsInt(), WhaleOptionsString(), WhaleOptionsReal(), WhaleOptionsBool(),
          WhaleOptionsName(), WhaleOptionsBegin(), WhaleOptionsEnd(), WhaleOptionsHead(),
          WhaleOptionsStringArray(),WhaleOptionsRealArray(), WhaleOptionsScalar(),
          WhaleOptionsBoolGroupBegin(), WhaleOptionsBoolGroup(), WhaleOptionsBoolGroupEnd(),
          WhaleOptionsList(), WhaleOptionsEList()

M*/
#define WhaleObjectOptionsBegin(obj) 0; do {                            \
  for (WhaleOptionsPublishCount=(WhaleOptionsPublish?-1:1); WhaleOptionsPublishCount<2; WhaleOptionsPublishCount++) { \
  WhaleErrorCode _5_ierr = WhaleObjectOptionsBegin_Private(obj);CHKERRQ(_5_ierr);

/*MC
    WhaleOptionsEnd - Ends a set of queries on the options database that are related and should be
     displayed on the same window of a GUI that allows the user to set the options interactively.

    Collective on the MPI_Comm used in WhaleOptionsBegin()

   Synopsis:
     #include "whaleoptions.h"
     WhaleErrorCode WhaleOptionsEnd(void)

  Level: intermediate

  Notes: Needs to be preceded by a call to WhaleOptionsBegin()

.seealso: WhaleOptionsGetReal(), WhaleOptionsHasName(), WhaleOptionsGetString(), WhaleOptionsGetInt(),
          WhaleOptionsGetIntArray(), WhaleOptionsGetRealArray(), WhaleOptionsBool()
          WhaleOptionsInt(), WhaleOptionsString(), WhaleOptionsReal(), WhaleOptionsBool(),
          WhaleOptionsName(), WhaleOptionsBegin(), WhaleOptionsEnd(), WhaleOptionsHead(),
          WhaleOptionsStringArray(),WhaleOptionsRealArray(), WhaleOptionsScalar(),
          WhaleOptionsBoolGroupBegin(), WhaleOptionsBoolGroup(), WhaleOptionsBoolGroupEnd(),
          WhaleOptionsList(), WhaleOptionsEList(), WhaleObjectOptionsBegin()

M*/
#define    WhaleOptionsEnd() _5_ierr = WhaleOptionsEnd_Private();CHKERRQ(_5_ierr);}} while (0)

WHALE_EXTERN WhaleErrorCode WhaleOptionsBegin_Private(MPI_Comm,const char[],const char[],const char[]);
WHALE_EXTERN WhaleErrorCode WhaleObjectOptionsBegin_Private(WhaleObject);
WHALE_EXTERN WhaleErrorCode WhaleOptionsEnd_Private(void);
WHALE_EXTERN WhaleErrorCode WhaleOptionsHead(const char[]);

/*MC
     WhaleOptionsTail - Ends a section of options begun with WhaleOptionsHead()
            See, for example, KSPSetFromOptions_GMRES().

   Collective on the communicator passed in WhaleOptionsBegin()

   Synopsis:
     #include "whaleoptions.h"
     WhaleErrorCode WhaleOptionsTail(void)

  Level: intermediate

   Notes: Must be between a WhaleOptionsBegin() and a WhaleOptionsEnd()

          Must be preceded by a call to WhaleOptionsHead() in the same function.

          This needs to be used only if the code below WhaleOptionsTail() can be run ONLY once.
      See, for example, PCSetFromOptions_Composite(). This is a return(0) in it for early exit
      from the function.

          This is only for use with the Whale options GUI; which does not currently exist.

   Concepts: options database^subheading

.seealso: WhaleOptionsGetInt(), WhaleOptionsGetReal(),
           WhaleOptionsHasName(), WhaleOptionsGetIntArray(), WhaleOptionsGetRealArray(), WhaleOptionsBool(),
          WhaleOptionsName(), WhaleOptionsBegin(), WhaleOptionsEnd(), WhaleOptionsHead(),
          WhaleOptionsStringArray(),WhaleOptionsRealArray(), WhaleOptionsScalar(),
          WhaleOptionsBoolGroupBegin(), WhaleOptionsBoolGroup(), WhaleOptionsBoolGroupEnd(),
          WhaleOptionsList(), WhaleOptionsEList(), WhaleOptionsEnum()
M*/
#define    WhaleOptionsTail() 0; {if (WhaleOptionsPublishCount != 1) WhaleFunctionReturn(0);}

WHALE_EXTERN WhaleErrorCode WhaleOptionsEnum(const char[],const char[],const char[],const char *const*,WhaleEnum,WhaleEnum*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsInt(const char[],const char[],const char[],WhaleInt,WhaleInt*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsReal(const char[],const char[],const char[],WhaleReal,WhaleReal*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsScalar(const char[],const char[],const char[],WhaleScalar,WhaleScalar*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsName(const char[],const char[],const char[],WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsString(const char[],const char[],const char[],const char[],char*,size_t,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsBool(const char[],const char[],const char[],WhaleBool ,WhaleBool *,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsBoolGroupBegin(const char[],const char[],const char[],WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsBoolGroup(const char[],const char[],const char[],WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsBoolGroupEnd(const char[],const char[],const char[],WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsList(const char[],const char[],const char[],WhaleFunctionList,const char[],char[],size_t,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsEList(const char[],const char[],const char[],const char*const*,WhaleInt,const char[],WhaleInt*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsRealArray(const char[],const char[],const char[],WhaleReal[],WhaleInt*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsIntArray(const char[],const char[],const char[],WhaleInt[],WhaleInt*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsStringArray(const char[],const char[],const char[],char*[],WhaleInt*,WhaleBool *);
WHALE_EXTERN WhaleErrorCode WhaleOptionsBoolArray(const char[],const char[],const char[],WhaleBool [],WhaleInt*,WhaleBool *);


WHALE_EXTERN WhaleErrorCode WhaleOptionsSetFromOptions(void);
WHALE_EXTERN WhaleErrorCode WhaleOptionsAMSDestroy(void);

/*
    See manual page for WhaleOptionsBegin()
*/
typedef enum {OPTION_INT,OPTION_LOGICAL,OPTION_REAL,OPTION_LIST,OPTION_STRING,OPTION_REAL_ARRAY,OPTION_HEAD,OPTION_INT_ARRAY,OPTION_ELIST,OPTION_LOGICAL_ARRAY,OPTION_STRING_ARRAY} WhaleOptionType;
typedef struct _n_WhaleOptions* WhaleOptions;
struct _n_WhaleOptions {
  char              *option;
  char              *text;
  void              *data;         /* used to hold the default value and then any value it is changed to by GUI */
  WhaleFunctionList flist;         /* used for available values for WhaleOptionsList() */
  const char *const *list;        /* used for available values for WhaleOptionsEList() */
  char              nlist;         /* number of entries in list */
  char              *man;
  size_t            arraylength;   /* number of entries in data in the case that it is an array (of WhaleInt etc) */
  WhaleBool         set;           /* the user has changed this value in the GUI */
  WhaleOptionType   type;
  WhaleOptions      next;
  char              *pman;
  void              *edata;
};

typedef struct {
  WhaleOptions     next;
  char             *prefix,*pprefix;
  char             *title;
  MPI_Comm         comm;
  WhaleBool        printhelp,changedmethod,alreadyprinted;
  WhaleObject      object;
} WhaleOptionsObjectType;
#endif
