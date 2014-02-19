#if !defined(__WHALELOG_HH)
#define __WHALELOG_HH

#if defined(WHALE_CLANGUAGE_CXX) && defined(__cplusplus)
#include <map>
#include <string>

namespace Whale {
  class LogStage {
  protected:
    std::string   name;
    WhaleLogEvent id;
  public:
    LogStage() : name(), id() {};
    LogStage(const std::string& name, WhaleLogStage id) : name(name), id(id) {};
    LogStage(const LogStage& stage) : name(stage.name), id(stage.id) {};
    void push() {
      WhaleErrorCode ierr = WhaleLogStagePush(this->id);CHKERRXX(ierr);
    };
    void pop() {
      WhaleErrorCode ierr = WhaleLogStagePop();CHKERRXX(ierr);
    };
    WhaleLogEvent getId() {return this->id;};
  };

  class LogEvent {
  protected:
    std::string   name;
    WhaleLogEvent id;
  public:
    LogEvent() : name(), id() {};
    LogEvent(const std::string& name, WhaleLogEvent id) : name(name), id(id) {};
    LogEvent(const LogEvent& event) : name(event.name), id(event.id) {};
    void begin(WhaleObject o1 = NULL, WhaleObject o2 = NULL, WhaleObject o3 = NULL, WhaleObject o4 = NULL) {
      WhaleErrorCode ierr = WhaleLogEventBegin(this->id, o1, o2, o3, o4);CHKERRXX(ierr);
    };
    void end(WhaleObject o1 = NULL, WhaleObject o2 = NULL, WhaleObject o3 = NULL, WhaleObject o4 = NULL) {
      WhaleErrorCode ierr = WhaleLogEventEnd(this->id, o1, o2, o3, o4);CHKERRXX(ierr);
    };
    void barrierBegin(WhaleObject o1 = NULL, WhaleObject o2 = NULL, WhaleObject o3 = NULL, WhaleObject o4 = NULL, MPI_Comm comm = MPI_COMM_NULL) {
      if (comm == MPI_COMM_NULL) comm = WHALE_COMM_WORLD;
      WhaleErrorCode ierr = WhaleLogEventBarrierBegin(this->id, o1, o2, o3, o4, comm);CHKERRXX(ierr);
    };
    void barrierEnd(WhaleObject o1 = NULL, WhaleObject o2 = NULL, WhaleObject o3 = NULL, WhaleObject o4 = NULL, MPI_Comm comm = MPI_COMM_NULL) {
      if (comm == MPI_COMM_NULL) comm = WHALE_COMM_WORLD;
      WhaleErrorCode ierr = WhaleLogEventBarrierEnd(this->id, o1, o2, o3, o4, comm);CHKERRXX(ierr);
    };
    WhaleLogEvent getId() {return this->id;};
    WhaleLogDouble time() {
      WhaleInt       stage = 0;
      WhaleStageLog       stageLog;
      WhaleEventPerfLog   eventLog;
      WhaleErrorCode ierr;

      ierr = WhaleLogGetStageLog(&stageLog);CHKERRXX(ierr);
      ierr = WhaleStageLogGetEventPerfLog(stageLog, stage, &eventLog);CHKERRXX(ierr);
      WhaleEventPerfInfo eventInfo = eventLog->eventInfo[this->id];

      return eventInfo.time;
    };
  };

  class Log {
  public:
    static std::map<std::string,LogEvent> event_registry;
    static std::map<std::string,LogStage> stage_registry;

    static LogEvent& Event(const std::string& name, WhaleClassId classid = WHALE_OBJECT_CLASSID) {
      if (event_registry.find(name) == event_registry.end()) {
        WhaleLogEvent  id;
        WhaleErrorCode ierr;

        /* Should check for already registered events */
        ierr = WhaleLogEventRegister(name.c_str(), classid, &id);CHKERRXX(ierr);
        event_registry[name] = LogEvent(name, id);
      }
      return event_registry[name];
    };

    static LogStage& Stage(const std::string& name) {
      if (stage_registry.find(name) == stage_registry.end()) {
        WhaleLogStage  id;
        WhaleErrorCode ierr;

        /* Should check for already registered stages */
        ierr = WhaleLogStageRegister(name.c_str(), &id);CHKERRXX(ierr);
        stage_registry[name] = LogStage(name, id);
      }
      return stage_registry[name];
    };
  };
}
#endif /* WHALE_CLANGUAGE_CXX */

/* Reset __FUNCT__ in case the user does not define it themselves */
#undef __FUNCT__
#define __FUNCT__ "User provided function"

#endif /* __WHALELOG_HH */
