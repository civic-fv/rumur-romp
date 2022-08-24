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
  bool _valid = true;
  bool _is_error = false;
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
  size_t history_size = OPTIONS.history_length;
  History* history = new History[OPTIONS.history_length];
  size_t history_start = 1;
  /**
   * @brief call if rule is applied to store what rule made the change in the
   * history circular buffer.
   * @param r reference to the rule that was applied
   */
  void add_to_history(const Rule& r) {
    history[history_level%history_size] = History{&r};
    ++history_level;
    if (history_level >= history_size)
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
    start_time = time(NULL);
#endif
    try {
      startstate.initialize(state);
      if (OPTIONS.do_trace)
        *json << "{\"$type\":\"init\",\"startstate\":" << startstate << ",\"state\":" << state <<"}";
    } catch (const IModelError& me) {
       __handle_exception<StartState,IModelError>(startstate,me); return;
    } catch (const std::exception& ex) {
      __handle_exception<StartState,std::exception>(startstate,ex); return;
    } catch (...) {
      std::cerr << "unknown non std::exception was thrown while initializing a Random Walker (id:"<<id<<")!\t[dev-error]\n" << std::flush;
    }
    // if (json != nullptr) delete json;
#ifdef __ROMP__DO_MEASURE
    active_time += time(NULL) - start_time;
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
  bool is_valid() { return _valid; }
  size_t is_error() { return _is_error; }
#ifdef __romp__ENABLE_cover_property
  bool is_done() { return (not (_valid && _fuel > 0 && _attempt_limit > 0)) || complete_cover(); }
#else
  bool is_done() { return not (_valid && _fuel > 0 && _attempt_limit > 0); }
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
      json = new json_file_t(OPTIONS.trace_dir + std::to_string(init_rand_seed) + ".json");
      // sim1Step = std::function<void()>([this]() {sim1Step_trace();});
      *json << "{\"$type\":\""
#ifdef __ROMP__SIMPLE_TRACE
      "romp-simple-trace"
#else
      "romp-trace"
#endif 
      "\",\"$version\":\"0.0.1\"";
      trace_metadata_out();
      *json << ",\"trace\":[";
    } else {
      // sim1Step = std::function<void()>([this]() {sim1Step_no_trace();});
    }
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
    if (history != nullptr) delete[] history; 
  }

  inline void init() noexcept {
#ifdef __ROMP__DO_MEASURE
    init_time = time(NULL);
#endif
    init_state();
  }

  inline void finalize() noexcept {
#ifdef __ROMP__DO_MEASURE
    total_time = (time(NULL)-init_time);
#endif
  }

  Result get_result() noexcept {
    if (OPTIONS.do_trace && json != nullptr) {
      trace_result_out();
      // delete json;
    }
    Result result{id,_is_error,_valid,tripped,tripped_inside};
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
    start_time = time(NULL);
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
      __handle_exception<Rule,IModelError>(r,me); return;
    } catch (std::exception& ex) {
      __handle_exception<Rule,std::exception>(r,ex); return;
    } catch (...) {
      std::cerr << "unexpected UNKNOWN exception inside of model (rule) \t[dev-error]\n";
    }
    if (pass)
      for (const auto& prop : ::__caller__::PROPERTIES)
        try {
          if (prop.check(state)) { // if tripped
            tripped = new ModelPropertyError(prop);
            *json << "],\"error-trace\":[" << *tripped; // << "]";
            break;
          }
        } catch(IModelError& me) {
          __handle_exception<Property,IModelError>(prop,me); return;
        } catch (std::exception& ex) {
          __handle_exception<Property,std::exception>(prop,ex); return;
        } catch (...) {
          std::cerr << "unexpected UNKNOWN exception inside of model (property-rule) \t[dev-error]\n";
        }
#ifdef __ROMP__DO_MEASURE
    active_time += time(NULL)-start_time;
#endif
    if (_fuel % _ROMP_FLUSH_FREQ == 0)
      json->out.flush();
  }

  void sim1Step_no_trace() noexcept {
#ifdef __ROMP__DO_MEASURE
    start_time = time(NULL);
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
      __handle_exception<Rule,IModelError>(r,me); return;
    } catch (std::exception& ex) {
      __handle_exception<Rule,std::exception>(r,ex); return;
    } catch (...) {
      std::cerr << "unexpected UNKNOWN exception inside of model (rule) \t[dev-error]\n";
    }
    if (pass)
      for (const auto& prop : ::__caller__::PROPERTIES)
        try {
          if (prop.check(state)) { // if tripped
            tripped = new ModelPropertyError(prop);
            break;
          }
        } catch(IModelError& me) {
          __handle_exception<Property,IModelError>(prop,me); return;
        } catch (std::exception& ex) {
          __handle_exception<Property,std::exception>(prop,ex); return;
        } catch (...) {
          std::cerr << "unexpected UNKNOWN exception inside of model (property-rule) \t[dev-error]\n";
        }
#ifdef __ROMP__DO_MEASURE
    active_time += time(NULL)-start_time;
#endif             
  }

  void trace_metadata_out() const {
    *json << ",\"metadata\":{"
                  "\"model\":\"" __model__filepath "\","
                  "\"romp-id\":" << ROMP_ID << ","
                  "\"trace-id\":" << id << ","
                  "\"root-seed\":\"" << OPTIONS.seed_str << "\","
                  "\"seed\":" << init_rand_seed << ","
                  "\"max-depth\":" << OPTIONS.depth << ","
                  "\"attempt-limit\":" << ((OPTIONS.attempt_limit != UINT64_MAX) 
                                            ? std::to_string(OPTIONS.attempt_limit) 
                                            : "null") << ","
#ifdef __romp__ENABLE_symmetry
                  "\"symmetry-reduction\":true,"
#else
                  "\"symmetry-reduction\":false,"
#endif
#ifdef __romp__ENABLE_assume_property
                  "\"enable-assume\":true,"
#else
                  "\"enable-assume\":false,"
#endif
#ifdef __romp__ENABLE_cover_property
                  "\"enable-cover\":" << OPTIONS.complete_on_cover << ","
                  "\"cover-count\":" << ((OPTIONS.complete_on_cover) 
                                          ? std::to_string(OPTIONS.cover_count) 
                                          : "null") << ","
#else
                  "\"enable-cover\":false,"
                  "\"cover-count\":null,"
#endif
#ifdef __romp__ENABLE_liveness_property
                  "\"enable-liveness\":" << OPTIONS.liveness << ","
                  "\"liveness-limit\":" << ((OPTIONS.liveness) 
                                            ? std::to_string(OPTIONS.lcount) 
                                            : "null") << ","
#else
                  "\"enable-liveness\":false,"
                  "\"liveness-limit\":null,"
#endif
#ifdef __ROMP__DO_MEASURE
                  "\"do-measure\":true,"
#else
                  "\"do-measure\":false,"
#endif
#ifdef __ROMP__SIMPLE_TRACE
                  "\"simple-trace\":true,"
#else
                  "\"simple-trace\":false,"
#endif
                  "\"start-id\":" << start_id << ","
                  "\"total-rule-count\":" << std::to_string(_ROMP_RULE_COUNT) << ","
                  "\"possible-state-count\":" _ROMP_STATESPACE_COUNT_str ""
                  "}";
  }

  void trace_result_out() {
    *json << "]"; // close trace
    if (_valid && tripped == nullptr) // if it didn't end in an error we need to: 
      *json << ",\"error-trace\":[]"; // output empty error-trace
    *json << ",\"results\":{\"depth\":"<< OPTIONS.depth-_fuel <<",\"valid\":" << _valid << ",\"is-error\":"<< _is_error
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

  // called when trying to print the results of the random walker when it finishes (will finish up trace file if nessasary too)
  //  the calling context should ensure that the RandWalker is not being used else where & safe output to the ostream 
  friend std::ostream& operator << (std::ostream& out, const RandWalker& rw) {
    if (not rw._is_error && not (OPTIONS.result_all)) return out; // don't output non-error state unless --report-all
#ifdef __romp__ENABLE_assume_property
    if (not rw._is_error && rw.tripped != nullptr && not OPTIONS.r_assume) return out; // don't output assumption violations unless --report-assume
#endif
// #ifdef __romp__ENABLE_liveness_property
//     if (OPTIONS.r_assume) // don't output attempt guard violations when --no-deadlock enabled
// #endif
    if ((OPTIONS.deadlock == false || OPTIONS.result_all == false) && rw._attempt_limit == 0) 
      return out; // don't output attempt guard violations when --no-deadlock enabled
    // ostream_p out(out_,0);
    out << "\n====== BEGIN :: Report of Walk #" << rw.id << " ======"
        << "\nBASIC INFO: "
        << "\n    RandSeed: " << rw.init_rand_seed
        << "\n  StartState: " << ::__caller__::STARTSTATES[rw.start_id]
        << "\n  Fuel level: " << rw._fuel
        << "\n"
           "\nTRACE LITE:"
        << "\n  NOTE - " << ((OPTIONS.do_trace) ? 
                          "see \"" + OPTIONS.trace_dir + std::to_string(rw.init_rand_seed) + ".json\" for full trace." 
                        : "use the --trace/-t option to generate a full & detailed trace." ) 
        << "\n  rules applied: " << rw.history_level-1
        << "\n  History: [\n"; // how to get for thr rule vs ruleset
    if (rw.history_start > 1)
      out << "        ... forgotten past ...\n";
    for (size_t i=rw.history_start; i<rw.history_level; ++i)
      out << "      (" << i <<") " << *(rw.history[i%rw.history_size].rule) << "\n";
    out << "    ]";
    if (OPTIONS.result_emit_state)
      out << "\n          State: " << rw.state << "\n";
    out << "\nPROPERTY/ERROR REPORT:"
        << "\n  Still a ``valid'' State?: " << (rw._valid ? "true" : "false") //    is it a valid state
        << "\n    In an ``Error State''?: " << (rw._is_error ? "true" : "false"); //    is it a valid state
    if (rw.tripped != nullptr) {
        out << "\n         Property Violated: " << *rw.tripped;
      if (rw.tripped_inside != nullptr)
        out << "\n              While Inside: " << *rw.tripped_inside;
    } else out << "\n         Property Violated: " << ((rw._fuel == 0) ? "MAX DEPTH REACHED" : 
                                             ((rw._attempt_limit == 0) ? "ATTEMPT LIMIT REACHED" 
                                                : "UNKNOWN"));
        
#ifdef __ROMP__DO_MEASURE
    out << "\nTIME REPORT:" //  states discovered (TODO)
        << "\n  Active Time: " << rw.active_time //  runtime info sub-header
        << "\n   Total Time: " << rw.total_time; //      active time
        // << "TODO" // metrics header
        // << "TODO" //      total time
#endif
    out << "\n======= END :: Report of Walk #" << rw.id << " =======\n" << std::flush;

    return out;
  }

  bool error_handler(id_t error_id) {
    tripped = new ModelMErrorError(error_id);
    _valid = false;
    _is_error = true;
    return true;
  }

  bool assertion_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    tripped = new ModelPropertyError(prop_id);
    _valid = false;
    _is_error = true;
    return true;
  }
  bool invariant_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    // no need to store exception in tripped if a property rule the catch will give us a better error dump
    // invar_handler is only called from a property rule
    _valid = false;
    _is_error = true;
    return true;
  }
#ifdef __romp__ENABLE_assume_property
  bool assumption_handler(bool expr, id_t prop_id) {
    if (expr) return false;
    tripped = new ModelPropertyError(prop_id);
    _valid = false;
    // // this is what makes an assumption different from an assertion
    // _is_error = false; // no need to actually set this value
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
  bool complete_cover() {
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

  //TODO create & launch threads
  std::vector<std::thread> threads;
  // std::vector<std::thread> threads(OPTIONS.threads);
  for (size_t i=0; i<OPTIONS.threads; ++i) {
    threads.push_back(std::thread(lambda));
  }
  // std::lock_guard<std::mutex> in_queue(_in_queue_mutex);
  // std::lock_guard<std::mutex> out_queue(_out_queue_mutex);

  sleep(1u);
  std::cout << "\nParallel Thread ROMP RESULTs:\n" << std::flush;
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
        rw->get_result();
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

  for (size_t i=0; i<OPTIONS.threads; ++i) // join threads
     threads[i].join();
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
  while( not rw->is_done() )
    rw->sim1Step();
  rw->finalize();
  std::cout << *rw << std::endl;  // example of writing one RW's results to cout
  std::cout << "Single ROMP RESULT:\n\t" 
            << ((rw->is_error()) 
                ? "A property was violated (see report above for details)"
                : "-- no errors found --")
            << std::endl;
  rw->get_result();
  delete rw;
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
