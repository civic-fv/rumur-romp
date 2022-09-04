/**
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp-rw.hpp
 *
 * @brief The primary simulation/random walk functions of romp-rw.
 *
 * @date 2022/05/11
 * @version 0.1
 */
#include <thread>
#include <mutex>
#ifndef __romp__GENERATED_CODE
#include "c_prefix.cpp" // FOR PRE-CODEGEN LANGUAGE SUPPORT ONLY !!
#endif

// << =================================== Type Declarations ==================================== >>

namespace romp {

/**
 * @brief helper function rand_choice 
 * 
 */
template<typename T>
T rand_choice(unsigned int &seed, T min, T max) {
    //not done fully
    seed = (((seed ^ (seed >> 3)) >> 12) & 0xffff) | ((seed & 0x7fff) << 16); // modifies the seed
    int choice = seed % (max-min) + min;  // generates the random number
    return choice;
}

class RandWalker : public ::romp::IRandWalker {
private:
  static id_t next_id;
  const id_t id;
  size_t start_id;
  const unsigned int init_rand_seed;
  unsigned int rand_seed;
  State_t state;
  size_t _fuel = OPTIONS.depth;
  bool _valid = true;  // legacy 
  bool _is_error = false; // legacy
  Result::Cause status = Result::RUNNING;
  json_file_t* json;
  // tripped thing
  IModelError* tripped = nullptr;
  IModelError* tripped_inside = nullptr;
  size_t _attempt_limit = OPTIONS.attempt_limit;
  const size_t init_attempt_limit = OPTIONS.attempt_limit;
  struct History {
    const Rule* rule;
  };
  size_t history_level = 1;
  constexpr size_t history_size() const {return _ROMP_HIST_LEN;}
  History history[_ROMP_HIST_LEN];
  size_t history_start = 1;
  /**
   * @brief call if rule is applied to store what rule made the change in the
   * history circular buffer.
   * @param r reference to the rule that was applied
   */
  void add_to_history(const Rule& r) {
    history[history_level%history_size()] = History{&r};
    ++history_level;
    if (history_level >= history_size())
      ++history_start;
  }
#ifdef __ROMP__DO_MEASURE
  time_t init_time;
  time_t start_time;
  time_t active_time = 0;
  time_t total_time = 0;
#endif
  
  void init_state() noexcept {    
    const StartState& startstate = ::__caller__::STARTSTATES[start_id];
#ifdef __ROMP__DO_MEASURE
    start_time = std::time(NULL);
#endif
    try {
      startstate.initialize(state);
      if (OPTIONS.do_trace)
        *json << "{\"$type\":\"init\",\"startstate\":" << startstate << ",\"state\":" << state <<"}";
    } catch (const IModelError& me) {
       __handle_exception/*<StartState,IModelError>*/(startstate,me);
    } catch (const std::exception& ex) {
      status = Result::UNKNOWN_CAUSE;
      __handle_exception/*<StartState,std::exception>*/(startstate,ex); 
    } catch (...) {
      status = Result::UNKNOWN_CAUSE;
      tripped_inside = new ModelStartStateError(startstate);
    }
    for (const auto& prop : ::__caller__::PROPERTIES)
      try {
        if (prop.check(state)) { // if tripped
          tripped = new ModelPropertyError(prop);
          tripped_inside = new ModelStartStateError(startstate);
          break;
        }
      } catch(IModelError& me) {
        __handle_exception/*<Property,IModelError>*/(prop,me);
      } catch (std::exception& ex) {
        __handle_exception/*<Property,std::exception>*/(prop,ex);
        status = Result::UNKNOWN_CAUSE;
      } catch (...) {
        status = Result::UNKNOWN_CAUSE;
      }
    // if (json != nullptr) delete json;
#ifdef __ROMP__DO_MEASURE
    active_time += std::time(NULL) - start_time;
#endif
  }

  template<typename R, typename E>
  void __handle_exception(const R& r, const E& er) noexcept {
    if (OPTIONS.do_trace) {
       *json << "],\"error-trace\":[" << er; // << "]";
    }
    tripped_inside = r.make_error();
  }

public:
  const std::function<void()> sim1Step;
  size_t fuel() { return _fuel; }
  size_t attempt_limit() { return _attempt_limit; }
  // bool is_valid() { return _valid; }
  // bool is_error() { return _is_error; }
#ifdef __romp__ENABLE_cover_property
  // bool is_done() const { return (_is_error || (not _valid) || _fuel <= 0 || _attempt_limit <= 0 || complete_cover()); }
  bool is_done() const { return ((status != Result::RUNNING) ||  _fuel <= 0 || _attempt_limit <= 0 || complete_cover()); }
#else
  // bool is_done() const { return (_is_error || (not _valid) || _fuel <= 0 || _attempt_limit <= 0); }
  bool is_done() const { return not (_valid && _fuel > 0 && _attempt_limit > 0); }
#endif

  RandWalker(unsigned int rand_seed_) 
    : rand_seed(rand_seed_),
      init_rand_seed(rand_seed_),
      sim1Step(((OPTIONS.do_trace) 
                  ? std::function<void()>([this](){sim1Step_trace();}) 
                  : std::function<void()>([this](){sim1Step_no_trace();}))),
      id(RandWalker::next_id++) 
  { 
    if (OPTIONS.start_id != ~0u) {
      rand_choice(rand_seed,0ul,_ROMP_STARTSTATES_LEN); // burn one rand operation for consistency
      start_id = OPTIONS.start_id;
    } else if (OPTIONS.do_even_start) {
      rand_choice(rand_seed,0ul,_ROMP_STARTSTATES_LEN); // burn one rand operation for consistency
      start_id = id % _ROMP_STARTSTATES_LEN;
    } else
      start_id = rand_choice(rand_seed,0ul,_ROMP_STARTSTATES_LEN);
    state.__rw__ = this; /* provide a semi-hidden reference to this random walker for calling the property handlers */ 
    if (OPTIONS.do_trace) {
      init_trace();
    }
    for (int i=0; i<history_size(); ++i) history[i] = History{nullptr};
#ifdef __romp__ENABLE_symmetry
    for (int i=0; i<_ROMP_RULESETS_LEN; ++i) next_rule[i] = 0;
#endif
#ifdef __romp__ENABLE_cover_property
    for (int i=0; i<_ROMP_COVER_PROP_COUNT; ++i) cover_counts[i] = 0;
#endif
#ifdef __romp__ENABLE_liveness_property
    for (int i=0; i<_ROMP_LIVENESS_PROP_COUNT; ++i) lcounts[i] = init_lcount;
#endif
  } 

  ~RandWalker() { 
    if (json != nullptr) delete json; 
    if (tripped != nullptr) delete tripped;
    if (tripped_inside != nullptr) delete tripped_inside; 
    // if (history != nullptr) delete[] history; 
  }

  inline void init() noexcept {
#ifdef __ROMP__DO_MEASURE
    init_time = std::time(NULL);
#endif
    init_state();
  }

  inline void finalize() noexcept {
#ifdef __ROMP__DO_MEASURE
    total_time = (std::time(NULL)-init_time);
#endif
    if (status == Result::UNKNOWN_CAUSE) {
    } else if (_attempt_limit <= 0) {
      status = Result::ATTEMPT_LIMIT_REACHED;
      const Rule* _last = history[(history_level-1)%history_size()].rule;
      if (_last != nullptr)
        tripped_inside = new ModelRuleError(*_last);
      else tripped_inside = nullptr;
    } else if (_fuel <= 0) {
      status = Result::MAX_DEPTH_REACHED;
#ifdef __romp__ENABLE_cover_property
    } else if (complete_cover()) { 
      status = Result::COVER_COMPLETE;
      tripped_inside = new ModelRuleError(*history[(history_level-1)%history_size()].rule);
#endif
    }
  }

  const Result* get_result() noexcept {
    if (OPTIONS.do_trace && json != nullptr) {
      trace_result_out();
      // delete json;
    }
    Result* result = new Result{
                  id,init_rand_seed,start_id,
                  status,
                  OPTIONS.depth - _fuel,
                  tripped,tripped_inside};
    tripped = nullptr;
    tripped_inside = nullptr;
    return result;
  }


private:

  /**
   * @brief to pick a rule in random for simulation step
   */
  // const RuleSet& rand_ruleset(){
  //   return ::__caller__::RULESETS[rand_choice<size_t>(rand_seed,0ul,_ROMP_RULESETS_LEN)]; 
  // }

#ifdef __romp__ENABLE_symmetry
  // keeps track of what rule to call next for our heuristic symmetry reduction
  id_t next_rule[_ROMP_RULESETS_LEN];
#endif
  /**
   * @brief to pick a rule in random for simulation step
   */
  const Rule& get_rand_rule(){
    const size_t rs_id = rand_choice<size_t>(rand_seed,0ul,_ROMP_RULESETS_LEN);
    const RuleSet& rs = ::__caller__::RULESETS[rs_id];
#ifdef __romp__ENABLE_symmetry
    id_t& r_id = next_rule[rs_id];  // this is a reference
    const Rule& r = rs.rules[r_id]; 
    if (++r_id >= rs.rules.size())
      r_id = 0;
#else
     const Rule& r = rs.rules[rand_choice<size_t>(rand_seed,0ul,rs.rules.size())];
#endif
    return r;
  }

  void sim1Step_trace() noexcept {
#ifdef __ROMP__DO_MEASURE
    start_time = std::time(NULL);
#endif
    // const RuleSet& rs = rand_ruleset();
    // const Rule& r = rand_rule(rs);
    const Rule& r = get_rand_rule();
    bool pass = false;
    try {  
      if ((pass = r.guard(state)) == true) {
        r.action(state);
        --_fuel;
        _attempt_limit = init_attempt_limit;
        add_to_history(r);
        *json << ",{\"$type\":\"rule-hit\",\"rule\":" << r << ","
                 "\"state\":" << state
              << "}";
      } else {
        *json << ",{\"$type\":\"rule-miss\",\"rule\":" << r << "}";
        --_attempt_limit;
      }      
    } catch(IModelError& me) {
      __handle_exception/*<Rule,IModelError>*/(r,me);
      pass = false;
    } catch (std::exception& ex) {
      __handle_exception/*<Rule,std::exception>*/(r,ex);
      pass = false;
      status = Result::UNKNOWN_CAUSE;
    } catch (...) {
      pass = false;
      status = Result::UNKNOWN_CAUSE;
    }
    if (pass)
      for (const auto& prop : ::__caller__::PROPERTIES)
        try {
          if (prop.check(state)) { // if tripped
            tripped = new ModelPropertyError(prop);
            tripped_inside = new ModelRuleError(r);
            *json << "],\"error-trace\":[" << *tripped; // << "]";
            break;
          }
        } catch(IModelError& me) {
          __handle_exception/*<Property,IModelError>*/(prop,me);
        } catch (std::exception& ex) {
          __handle_exception/*<Property,std::exception>*/(prop,ex);
          status = Result::UNKNOWN_CAUSE;
        } catch (...) {
          status = Result::UNKNOWN_CAUSE;
        }
#ifdef __ROMP__DO_MEASURE
    active_time += std::time(NULL)-start_time;
#endif
    if (_fuel % _ROMP_FLUSH_FREQ == 0)
      json->out.flush();
  }

  void sim1Step_no_trace() noexcept {
#ifdef __ROMP__DO_MEASURE
    start_time = std::time(NULL);
#endif
    // const RuleSet& rs= rand_ruleset();
    // const Rule& r= rand_rule(rs);
    const Rule& r = get_rand_rule();
    bool pass = false;
    try {  
      if ((pass = r.guard(state)) == true) {
        r.action(state);
        --_fuel;
        _attempt_limit = init_attempt_limit;
        add_to_history(r);
      } else { --_attempt_limit; }
    } catch(IModelError& me) {
      __handle_exception/*<Rule,IModelError>*/(r,me);
      pass = false;
    } catch (std::exception& ex) {
      __handle_exception/*<Rule,std::exception>*/(r,ex);
      pass = false;
      status = Result::UNKNOWN_CAUSE;
    } catch (...) {
      pass = false;
      status = Result::UNKNOWN_CAUSE;
    }
    if (pass)
      for (const auto& prop : ::__caller__::PROPERTIES)
        try {
          if (prop.check(state)) { // if tripped
            tripped = new ModelPropertyError(prop);
            tripped_inside = new ModelRuleError(r);
            break;
          }
        } catch(IModelError& me) {
          __handle_exception/*<Property,IModelError>*/(prop,me);
        } catch (std::exception& ex) {
          __handle_exception/*<Property,std::exception>*/(prop,ex);
          status = Result::UNKNOWN_CAUSE;
        } catch (...) {
          status = Result::UNKNOWN_CAUSE;
        }
#ifdef __ROMP__DO_MEASURE
    active_time += std::time(NULL)-start_time;
#endif             
  }

  void init_trace() const {
    json = new json_file_t(OPTIONS.get_trace_file_path(id));
    *json << "{\"$type\":\"";
#ifdef __ROMP__SIMPLE_TRACE
    *json << "romp-simple-trace";
#else
    *json << "romp-trace";
#endif 
    *json << "\",\"$version\":\"" _ROMP_TRACE_JSON_VERSION "\"";
    *json << ",\"trace-id\":" << id 
          << ",\"seed\":" << init_rand_seed
          << ",\"start-id\":" << start_id
          << ",\"metadata\":" << OPTIONS.write_metadata_json(json->out)
          << ",\"trace\":[";
  }

  void trace_result_out() {
    *json << "]"; // close trace
    // if (_valid && tripped == nullptr) // if it didn't end in an error we need to: 
    if (tripped_inside == nullptr) // if it didn't end in an error we need to: 
      *json << ",\"error-trace\":[]"; // output empty error-trace
    *json << ",\"results\":{\"depth\":"<< OPTIONS.depth-_fuel <<",\"valid\":null,\"is-error\":null"
          << ",\"result\":\"" << std::to_string(status) << "\"" 
#ifdef __ROMP__DO_MEASURE
                                << ",\"active-time\":" << active_time << ",\"total-time\":" << total_time
#else
                                << ",\"active-time\":null,\"total-time\":null" 
#endif
            << ",\"property-violated\":" << tripped
            << ",\"tripped-inside\":" << tripped_inside
            << ",\"attempts-final\":" << _attempt_limit
                              << "}" // closes results object
              << "}"; // closes top level trace object
    json->out.flush();
  }

  // called when trying to print the results of the random walker when it finishes (will finish up trace file if necessary too)
  //  the calling context should ensure that the RandWalker is not being used else where & safe output to the ostream 
  friend ostream_p& operator << (ostream_p& out, const RandWalker& rw) {
    std::string res_color = get_color(rw.status);
    if ((rw.status == Result::UNKNOWN_CAUSE // don't output non-error state unless --report-all
          || rw.status == Result::PROPERTY_VIOLATED
          || rw.status == Result::MERROR_REACHED)
        && not (OPTIONS.result_all)) return out;
#ifdef __romp__ENABLE_assume_property
    if (rw.status == Result::ASSUMPTION_VIOLATED && not OPTIONS.r_assume) return out; // don't output assumption violations unless --report-assume
#endif
// #ifdef __romp__ENABLE_liveness_property
//     if (OPTIONS.r_assume) // don't output attempt guard violations when --no-deadlock enabled
// #endif
    if ((OPTIONS.deadlock == false || OPTIONS.result_all == false) 
        && rw.status == Result::ATTEMPT_LIMIT_REACHED) 
      return out; // don't output attempt guard violations when --no-deadlock enabled
    // ostream_p out(out_,0);
    out << out.nl()
        << "======== BEGIN :: Report of Walk #" << rw.id << " ========"             << out.nl()
        << "BASIC INFO: "                                           << out.indent() << out.nl()
        << "         w#: " << rw.id                                                 << out.nl()
        << "       Seed: " << rw.init_rand_seed                                     << out.nl()
        << "      Depth: " << OPTIONS.depth - rw._fuel                              << out.nl()
        << "   Start ID: " << rw.start_id                                           << out.nl()
        << "             {" << ::__caller__::STARTSTATES[rw.start_id] << '}'        << out.nl()
        << "     Result: " << res_color << std::to_string(rw.status) << "\033[0m"   << out.nl()
        << out.dedent()                                                             << out.nl()
        << "TRACE LITE:"                                            << out.indent() << out.nl()
        << "NOTE - " << ((OPTIONS.do_trace) 
                          ? "see \"" + OPTIONS.trace_dir + std::to_string(rw.init_rand_seed) + ".json\" for full trace." 
                          : "use the --trace/-t option to generate a full & detailed trace." ) << out.nl()
        << "History: [\n" << out.indent();
    if (rw.history_start > 1)
      out << out.indentation() << "-(..) ... forgotten past ...\n";
    for (size_t i=rw.history_start; i<rw.history_level; ++i)
      out << out.indentation() << "-(" << i <<") " << *(rw.history[i%rw.history_size()].rule) << "\n";
    out << out.dedent() << "]"                                                      << out.nl();
    if (OPTIONS.result_emit_state)
      out << "  State: " <<  out.indent() << rw.state << out.dedent()               ;// << out.nl();
    if (rw.tripped != nullptr || rw.tripped_inside != nullptr) {
      out << out.dedent()                                                           << out.nl()
          << "ISSUE REPORT:"                                        << out.indent() << out.nl();
      if (rw.tripped != nullptr)
        out << "Tripped: \033[31m" << *rw.tripped << "\033[0m"                      << out.nl();
      if (rw.tripped_inside != nullptr)
        out << "  Trace: "                                          << out.indent() << out.nl()
            << *rw.tripped_inside                                   << out.dedent() << out.nl();
            // << '}'                                                                  << out.nl(); 
    }
        
#ifdef __ROMP__DO_MEASURE
    out << out.dedent()                                                             << out.nl()
        << "TIME REPORT:"                                           << out.indent() << out.nl()
        << "Active Time: " << rw.active_time                                        << out.nl()           
        << " Total Time: " << rw.total_time                                         << out.nl();
#endif
    out << out.dedent()                                                             << out.nl()
        << "======== END :: Report of Walk #" << rw.id << " ========"               << out.nl();
    out.out.flush();

    return out;
  }
  // called when trying to print the results of the random walker when it finishes (will finish up trace file if necessary too)
  //  the calling context should ensure that the RandWalker is not being used else where & safe output to the ostream 
  friend std::ostream& operator << (std::ostream& out, const RandWalker& rw) 
  { ostream_p _out(out,0); _out << rw; return out; }

  bool error_handler(id_t error_id) {
    tripped = new ModelMErrorError(error_id);
    _valid = true; // legacy
    _is_error = true; // legacy
    status = Result::MERROR_REACHED;
    return true;
  }

  bool assertion_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    tripped = new ModelPropertyError(prop_id);
    _valid = false;  // legacy
    _is_error = true;  // legacy
    status = Result::MERROR_REACHED;
    return true;
  }
  bool invariant_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    // no need to store exception in tripped if a property rule the catch will give us a better error dump
    // invar_handler is only called from a property rule
    _valid = false;  // legacy
    _is_error = true; // legacy
    status = Result::PROPERTY_VIOLATED;
    return true;
  }
#ifdef __romp__ENABLE_assume_property
  bool assumption_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    tripped = new ModelPropertyError(prop_id);
    _valid = false;  // legacy
    // this is what makes an assumption different from an assertion
    _is_error = false; // legacy
    status = Result::ASSUMPTION_VIOLATED;
    return true;
  }
#else
  bool assumption_handler(bool expr, id_t prop_id) {
    return false;  // don't do anything if the assume property is nto enabled
  }
#endif
#ifdef __romp__ENABLE_cover_property
  const bool enable_cover = OPTIONS.complete_on_cover;
  const id_t goal_cover_count = OPTIONS.cover_count;
  size_t cover_counts[_ROMP_COVER_PROP_COUNT];
  bool cover_handler(bool expr, id_t cover_id, id_t prop_id) {
    if (expr) cover_counts[cover_id]++;
    return false; // cover never throws an error
  }
#ifdef __GNUC__
  __attribute__((optimize("unroll-loops")))
#endif
  bool complete_cover() const {
    if (not enable_cover) return false;
    bool res = true;
    for (int i=0; i<_ROMP_COVER_PROP_COUNT; ++i) 
      res &= (cover_counts[i] >= goal_cover_count);
    return res;
  }
#else
  bool cover_handler(bool expr, id_t cover_id, id_t prop_id) {
    return false;  // never throw anything if cover is not enabled by romp generator
  }
#endif
#ifdef __romp__ENABLE_liveness_property
private:
  const bool enable_liveness = OPTIONS.liveness;
  const size_t init_lcount = OPTIONS.lcount;
  size_t lcounts[_ROMP_LIVENESS_PROP_COUNT];
public:
  bool liveness_handler(bool expr, id_t liveness_id, id_t prop_id) {
    if (not enable_liveness) return false;
    if (expr) {
      lcounts[liveness_id] = init_lcount;
      return false;
    }
    if (--lcounts[liveness_id] > 0) return false; 
    _valid = false;
    _is_error = true;
    tripped = new ModelPropertyError(prop_id);
    return true;  // TODO actually handle this as described in the help page
  }
#else
  bool liveness_handler(bool expr, id_t liveness_id, id_t prop_id) {
    return false;  // never throw anything if cover is not enabled by romp generator
  }
#endif
}; //? END class RandomWalker

id_t RandWalker::next_id = 0u;


/**
 * @brief A class that indexes and groups the various results a RandWalker can produce,
 *        and provides a helpful operator for writing a nice summary of the results 
 *        to a \c std::ostream as well.
 */
class ResultTree {
  time_t start_time = std::time(NULL);
  time_t end_time = 0;
  size_t rules_fired = 0;
  size_t size = 0;
  std::vector<const Result*> unknown_causes;
  std::vector<const Result*> attempt_limits_reached;
  std::vector<const Result*> max_depths_reached;
  std::unordered_map<ModelPropertyError,std::vector<const Result*>> properties_violated;
  size_t n_properties_violated = 0;
#ifdef __romp__ENABLE_cover_property
  std::vector<const Result*> completed_covers;
#endif
#ifdef __romp__ENABLE_assume_property
  std::unordered_map<ModelPropertyError,std::vector<const Result*>> assumptions_violated;
  size_t n_assumptions_violated = 0;
#endif
  std::unordered_map<ModelMErrorError,std::vector<const Result*>> merrors_reached;
  size_t n_merrors_reached = 0;
public:
  ~ResultTree() {
    for (auto p : unknown_causes) if (p!=nullptr) delete p;
    for (auto p : attempt_limits_reached) if (p!=nullptr) delete p;
    for (auto p : max_depths_reached) if (p!=nullptr) delete p;
#ifdef __romp__ENABLE_cover_property  
    for (auto p : completed_covers) if (p!=nullptr) delete p;
#endif
    for (auto _p : properties_violated)
      for (auto p : _p.second) if (p!=nullptr) delete p;
    for (auto _p : merrors_reached)
      for (auto p : _p.second) if (p!=nullptr) delete p;
#ifdef __romp__ENABLE_assume_property  
    for (auto _p : assumptions_violated)
      for (auto p : _p.second) if (p!=nullptr) delete p;
#endif
  }
  ResultTree() {}
  void insert(const Result* res) {
    switch (res->cause) {
      case Result::ATTEMPT_LIMIT_REACHED:
        attempt_limits_reached.push_back(res);
        break;
      case Result::MAX_DEPTH_REACHED:
        max_depths_reached.push_back(res);
        break;
      case Result::PROPERTY_VIOLATED: {
        const auto _me_p = dynamic_cast<const ModelPropertyError*>(res->tripped);
        if (properties_violated.find(*_me_p) == properties_violated.end())
          properties_violated[*_me_p] = std::vector<const Result*>{res};
        else
          properties_violated[*_me_p].push_back(res);
        ++n_properties_violated;
        break;
      }
      case Result::MERROR_REACHED: {
        const auto _me_e = dynamic_cast<const ModelMErrorError*>(res->tripped);
        if (merrors_reached.find(*_me_e) == merrors_reached.end())
          merrors_reached[*_me_e] = std::vector<const Result*>{res};
        else
          merrors_reached[*_me_e].push_back(res);
        ++n_merrors_reached;
        break;
      }
#ifdef __romp__ENABLE_assume_property
      case Result::ASSUMPTION_VIOLATED: {
        const auto _me_a = dynamic_cast<const ModelPropertyError*>(res->tripped);
        if (assumptions_violated.find(*_me_a) == assumptions_violated.end())
          assumptions_violated[*_me_a] = std::vector<const Result*>{res};
        else
          assumptions_violated[*_me_a].push_back(res);
        ++n_assumptions_violated;
        break;
      }
#endif
#ifdef __romp__ENABLE_cover_property
      case Result::COVER_COMPLETE:
        completed_covers.push_back(res);
        break;
#endif
      case Result::UNKNOWN_CAUSE:
      default:
        unknown_causes.push_back(res);
        break;
    }
    ++size;
    rules_fired += res->depth;
  }
  void start_timer() { start_time = std::time(NULL); }
  void reset_timer() { start_time = std::time(NULL); }
  void stop_timer() { end_time = std::time(NULL); }
  time_t get_time() { return ((end_time>0) ? end_time : std::time(NULL)) - start_time; }
  friend std::ostream& operator << (std::ostream& out, const ResultTree& results) {
    ostream_p _out(out);
    _out << results;
    return out;
  }
  friend ostream_p& operator << (ostream_p& out, const ResultTree& r) {
    int i = 1; // int j = 1;
    size_t issues = 0; size_t abs_issues = 0;
#ifdef __romp__ENABLE_assume_property
    if (OPTIONS.r_assume && r.n_assumptions_violated > 0) {
      issues += r.assumptions_violated.size();
      abs_issues += r.n_assumptions_violated;
      out << out.indentation() << "\033[1;4mASSUMPTION(S) VIOLATED (n="<< r.assumptions_violated.size() 
          << " |n|=" << r.n_assumptions_violated << "):\033[0m\n";
      out.indent(); i = 1;
      for (const auto& _a : r.assumptions_violated) {  
        out << out.indentation()
            << "\033[1m-(" << (i++) << ") assume \"" << _a.first.label() << "\":\033[0m\n";
        out.indent();
        for (const auto& a : _a.second) {
          out << out.indentation()
              << "-[w#" << a->id << "] seed=" << a->root_seed << " start-id=" << a->start_id <<" depth=" << a->depth << "\n";
          if (not a->tripped->is_flat())
            out << out.indentation()
                << "        quantifiers(" << a->tripped->quants() << ")\n";
          out << out.indentation() 
              << ((a->tripped->is_generic())
                  ? "        inside"
                  : "        last-rule")
              << "={"
              << a->inside->get_type()
              <<" \"" << a->inside->label() << "\"";
          if (not a->inside->is_flat())
              out << " quantifiers(" << a->inside->quants() << ")";
          out << "}\n";
        }
        out.dedent();
        ++i;
      }
      out.out << out._dedent() << std::endl;
    }
#endif
    if ((OPTIONS.attempt_limit != _ROMP_ATTEMPT_LIMIT_DEFAULT && OPTIONS.deadlock) 
          && r.attempt_limits_reached.size() > 0) {
        out << out.indentation() << "\033[1;4mATTEMPT LIMIT(S) REACHED (n=" 
                << r.attempt_limits_reached.size() << "):\033[0m\n";
        out.indent();
        i = 1;
        for (const auto& _al : r.attempt_limits_reached) {
          out << out.indentation()
              << "-(" << i << ") [w#" << _al->id << "] seed=" << _al->root_seed << " start-id=" << _al->start_id <<" depth=" << _al->depth << "\n"
              << out.indentation()
              << "        last-rule=";
          if (_al->inside != nullptr) {
            out << '{' << _al->inside->get_type() << " \"" << _al->inside->label() << "\"";
            if (not _al->inside->is_flat())
              out << " quantifiers(" << _al->inside->quants() << ")";
            out << "}\n";
          } else {
            out << "none\n";
          }
          ++i;
        }
      out.out << out._dedent() << std::endl;
     }
#ifdef __romp__ENABLE_cover_property
     if (OPTIONS.result_all && r.completed_covers.size() > 0) {
      out << out.indentation() << "\033[1;4mCOVER(S) COMPLETED (n=" 
                << r.completed_covers.size() << "):\033[0m\n";
        out.indent();
        i = 1;
        for (const auto& _c : r.completed_covers) {
          out << out.indentation()
              << "-(" << i << ") [w#" << _c->id << "] seed=" << _c->root_seed << " start-id=" << _c->start_id <<" depth=" << _c->depth << "\n"
              << out.indentation()
              << "        last-rule={" << _c->inside->get_type() << " \"" << _c->inside->label() << "\"";
              if (not _c->inside->is_flat())
                out << " quantifiers(" << _c->inside->quants() << ")";
          out << "}\n";
          ++i; 
        }
      out.out << out._dedent() << std::endl;
     }
#endif
    if (OPTIONS.result_all && r.max_depths_reached.size() > 0) {
      out << out.indentation() << "\033[1;4mMAX DEPTH(S) REACHED (n=" 
                << r.max_depths_reached.size() << "):\033[0m\n";
        out.indent();
        i = 1;
        for (const auto& _r : r.max_depths_reached)
          out << out.indentation()
              << "-(" << i++ << ") [w#" << _r->id << "] seed=" << _r->root_seed << " start-id=" << _r->start_id << "\n";
      out.out << out._dedent() << std::endl;
    }
    if (r.n_merrors_reached > 0) {
      issues += r.merrors_reached.size();
      abs_issues += r.n_merrors_reached;
      out << out.indentation() << "\033[1;4mERROR STATEMENT(S) REACHED (n="<< r.merrors_reached.size() 
          << " |n|=" << r.n_merrors_reached << "):\033[0m\n";
      out.indent(); i = 1;
      for (const auto& _a : r.merrors_reached) {
        out << out.indentation()
            << "\033[1m-(" << (i++) << ") error \"" << _a.first.label() << "\":\033[0m\n";
        out.indent();
        for (const auto& a : _a.second) {
          out << out.indentation()
              << "-[w#" << a->id << "] seed=" << a->root_seed << " start-id=" << a->start_id <<" depth=" << a->depth << "\n"
              << out.indentation()
              << "        IN-RULE"
              << "={" << a->inside->get_type() << " \"" << a->inside->label() << "\"";
          if (not a->inside->is_flat())
              out << " quantifiers(" << a->inside->quants() << ")";
          out << "}\n";
        }
        out.dedent();
        ++i;
      }
      out.out << out._dedent() << std::endl;
    }
    if (r.n_properties_violated > 0) {
      issues += r.properties_violated.size();
      abs_issues += r.n_properties_violated;
      out << out.indentation() << "\033[1;4mPROPERTY(S) VIOLATED (n="<< r.properties_violated.size() 
          << " |n|=" << r.n_properties_violated << "):\033[0m\n";
      out.indent(); i = 1;
      for (const auto& _a : r.properties_violated) {
        const PropertyInfo& _info =_a.first.info();
        out << out.indentation()
            << "\033[1m-(" << (i++) << ") " << _info.type << " \"" << _info.label << "\":\033[0m "
                "(x"<< _a.second.size() <<")\n";
        out.indent();
        int j = 0;
        for (const auto& a : _a.second) {
          if (j > 2 && not (OPTIONS.result_all || OPTIONS.result)) { 
            out << out.indentation() << "-[..] ... " << _a.second.size()-j << " more ...\n"; 
            break;
          }
          out << out.indentation()
              << "-[w#" << a->id << "] seed=" << a->root_seed << " start-id=" << a->start_id <<" depth=" << a->depth << "\n";
          if (not a->tripped->is_flat())
            out << out.indentation()
                << "        quantifiers(" << a->tripped->quants() << ")\n";
          out << out.indentation()
              << ((a->tripped->is_generic())
                  ? "        inside"
                  : "        last-rule")
              << "={"
              << a->inside->get_type()
              <<" \"" << a->inside->label() << "\"";
          if (not a->inside->is_flat())
              out << " quantifiers(" << a->inside->quants() << ")";
          out << "}\n";
          ++j;
        }
        out.dedent();
        ++i;
      }
      out.out << out._dedent() << std::endl;
    }
    if (OPTIONS.result_all && r.unknown_causes.size() > 0) {
      issues += r.unknown_causes.size();
      abs_issues += r.unknown_causes.size();
      out << out.indentation() << "\033[41;37;1;4mUNKNOWN ERROR(S) (n=" 
                << r.unknown_causes.size() << "):\033[21;24m\n";
        out.indent();
        i = 1;
        for (const auto& _r : r.unknown_causes) {
          out << out.indentation()
              << "-(" << i++ << ") ``" << ({_r->inside->write_root_excpt_what(out.out); "''\n";})
              << out._indent()
              << "-[w#" << _r->id << "] seed=" << _r->root_seed << " start-id=" << _r->start_id << " depth=" << _r->depth << "\n"
              << "        inside={"
              << _r->inside->get_type()
              << " \"" << _r->inside->label() << '"';
          if (not _r->inside->is_flat())
            out << " quantifiers(" << _r->inside->quants() << ")";
          out << "}\n";
          out.dedent();
        }
      out.out << out._dedent() << "\033[0m" << std::endl;
    }
    std::string color = ((issues>0) ? "\033[30;1;4m" : "\033[32;1m");
    out << out.indentation() << '\n'
        << out.indentation() << "\033[1;4mSUMMARY:\033[0m\n" << out.indent()
        << out.indentation() << "       issues found: n=" << color << issues << "\033[0m"
                                  << "  (|n|=" << color << abs_issues << "\033[0m)\n"
        << out.indentation() << "total rules applied: " << r.rules_fired << '\n'
        << out.indentation() << "  avg rules applied: " << std::setprecision(1) << r.rules_fired/r.size << '\n'
        << out.indentation() << "         time taken: " << (((r.end_time > 0) ? r.end_time : std::time(NULL)) - r.start_time) << "s\n";
    out.out << std::endl;
    return out;
  }
};

/**
 * @brief to generate random seeds for the no of random-walkers
 * rand is generated using UNIX timestamp 
 * @param root_seed the parent seed for generating the random seeds.
 */
unsigned int gen_random_seed(unsigned int &root_seed) {
  return rand_choice(root_seed, 1u, UINT32_MAX);
}

/**
 * @brief generate all startstates of the model. 
 * To do - how to get the size of startstate
 * 
 */
std::vector<RandWalker*> gen_random_walkers(unsigned int root_seed)   {
  std::vector<RandWalker*> rws;
  for(int i=0; i<OPTIONS.random_walkers; i++) {
    rws.push_back(new RandWalker(gen_random_seed(root_seed)));
  }
  return rws;
}

// example of how to get a copy of an object in C++:
//   use the copy constructor (always has the signature)
//    ClassName(const ClassName &obj)
//  this means your's should look like:
//    auto copied_state = State_t(startstates[i%startstates.size()]);

void print_romp_results_summary(const ResultTree& summary) {
  std::cout << "\n\033[34;1m"
         "================================\n"
         "          ROMP RESULTS:         \n"
         "================================\n"
         "\033[0m\n\n"
         << summary;
}


/**
 * @brief implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param rw_count the number of \c RandWalker 's to use.
 * @param rand_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 * @param thread_count the max number of threads to use to accomplish all said random walks.
 */
void launch_OpenMP(unsigned int root_seed) {
  std::cout << "\n\t!! NOT YET IMPLEMENTED !!\n" << std::endl; return; //!! temp, remove when finished !! 
  
  // std::vector<RandWalker> rws;
  // try {
  //   rws = gen_random_walkers(rw_count, root_seed, fuel);
  // } catch (const IModelError& ex) {
  //   std::cerr << "\nModel raised an error when initializing our start state(s)!! (message below)\n"
  //             << ex << std::endl;
  // }
  //TODO: launch the random walkers !!
}

/**
 * @brief implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param rw_count the number of \c RandWalker 's to use.
 * @param rand_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 * @param thread_count the max number of threads to use to accomplish all said random walks.
 */
void launch_threads(unsigned int rand_seed) {
  auto rws = gen_random_walkers(rand_seed);
  std::queue<RandWalker*> in_rws(std::deque<RandWalker*>(rws.begin(),rws.end()));
  // std::queue<RandWalkers*> parallel_rws; // probs threads
  std::queue<RandWalker*> out_rws;
  size_t walks_done = 0;
  std::mutex in_queue;
  std::mutex out_queue;
  // std::mutex _in_queue_mutex;
  // std::mutex _out_queue_mutex;
  
  auto lambda = [&]() { // code the threads run
    // std::lock_guard<std::mutex> in_queue(_in_queue_mutex);
    // std::lock_guard<std::mutex> out_queue(_out_queue_mutex);
    in_queue.lock();
    do {
      RandWalker *rw = in_rws.front();
      in_rws.pop(); 
      in_queue.unlock();

      if (rw == nullptr) {
        break;  //DEBUG catch bad data in queue
        // in_queue.lock();  
        // continue;
      };  // just in case might not need (if so remove)

      rw->init();
      while (not rw->is_done())
        rw->sim1Step();
      rw->finalize();

      out_queue.lock();
      out_rws.push(rw);
      ++walks_done;
      out_queue.unlock();

      in_queue.lock();
    } while (in_rws.size() > 0);
    in_queue.unlock();
  };

  ResultTree summary;
  ostream_p out(std::cout,0);
  std::vector<std::thread> threads;
  // std::vector<std::thread> threads(OPTIONS.threads);
  for (size_t i=0; i<OPTIONS.threads; ++i) {
    threads.push_back(std::thread(lambda));
  }
  // std::lock_guard<std::mutex> in_queue(_in_queue_mutex);
  // std::lock_guard<std::mutex> out_queue(_out_queue_mutex);

  sleep(1u);
  if (OPTIONS.result) {
    out << "\n\033[34;1m"
           "===================================\n"
           "  WALKS OF INTEREST ROMP RESULTS:  \n"
           "===================================\n"
           "\033[0m\n\n";
    out.out.flush();
  }
  while (true) {
    for (int _; _<560; ++_) { _++; }  //DEBUG SLOW ME DOWN
    while (true) {  // handel outputs
      out_queue.lock();
      if (not (out_rws.size() > 0)) {
        out_queue.unlock();
        break;
      }
      RandWalker* rw = out_rws.front();
      out_rws.pop();
      out_queue.unlock();
      if (rw != nullptr) {
        if (OPTIONS.result)
          std::cout << *rw << std::endl;
        // todo get the results
        summary.insert(rw->get_result());
        delete rw;
      }
    }
    for (int _; _<560; ++_) { _++; }  //DEBUG SLOW ME DOWN
    out_queue.lock();
    if (not (walks_done < OPTIONS.random_walkers)) {
      out_queue.unlock();
      break;
    }
    out_queue.unlock();
  }
  // we might need to do this one more time for the last one

  summary.stop_timer();

  for (size_t i=0; i<OPTIONS.threads; ++i) // join threads
     threads[i].join();

  print_romp_results_summary(summary);
}
  

/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param root_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_CUDA(unsigned int root_seed);


/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param root_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_SYCL(unsigned int root_seed);


/**
 * @brief (NOT YET IMPLEMENTED) \n
 *        Implementing \c rw_count parallel \c RandWalker "simulations" which has the threads 
 *        and no of random-walkers specified by the user options .
 * @param root_seed the starting random seed that will generate all other random seeds
 * @param fuel the max number of rules any \c RandWalker will try to apply.
 */
void launch_OpenMPI(unsigned int root_seed);


/**
 * @brief Launches a single RandWalker (basically jst a simulation).
 * @param rand_seed the random seed
 * @param fuel the max number of rules to try to apply
 */
void launch_single(unsigned int rand_seed) {
  RandWalker* rw = new RandWalker(rand_seed);
  rw->init();
  ResultTree summary;
  while( not rw->is_done() )
    rw->sim1Step();
  rw->finalize();
  std::cout << "SINGLE ROMP RESULT:\n\t" 
            << *rw << '\n' << std::endl;
  summary.insert(rw->get_result());
  delete rw;
  print_romp_results_summary(summary);
}


// void Sim1Step(RandWalker::State state, Rule rule, size_t state_count) {
//   for (int s = 0; s < state_count; s++)
//     if (rule.Guard(states[s])) {
//       rule.Run(states[s]);
//       states[i].rule_applied(
//           rule.id); // this keeps track of history and other overhead
//       // These could possibly be parallelized even better for GPU using
//       // shared memory. (not implemented here, could introduce data races)
//       for (int i = 0; i < _ROMP_INVARIANTS_LEN; i++)
//         states[s].valid |= State::INVARIANTS[i](
//             states[i]); // this will need to change after we figure shit out
//       for (int a = 0; a < _ROMP_ASSERTIONS_LEN; a++)
//         states[s].valid |= State::ASSERTIONS[a](
//             states[i]); // this will need to change after we figure shit out
//     }
// }


// << ========================================================================================== >> 
// <<                                         ROMP CODE                                          >> 
// << ========================================================================================== >> 
}
// template<class S, class R>
