#! /usr/bin/python3 
"""*
 * @proj romp
 * @author Andrew Osterhout
 * @author Ajantha Varadharaaj
 * @org University of Utah (UofU) School of Computing (SoC)
 * @org Center for Parallel compute at Utah (CPU)
 * @org <a href="https://civic-fv.github.io">Civic-fv NSF Grant</a>
 * @org Ganesh Gopalakrishnan's Research Group
 * @file romp-rw.hpp
 *
 * @brief A utility to extract romp model-checkers (random walkers) trace files
 *         and perform some statistics on the traces.
 *
 * @date 2022/05/11
 * @version 0.1
 *"""

from concurrent.futures import process
from sys import argv
import os
from json import load as j_load, dumps as j_dump
from typing import List, Union as Un, MutableSet as Set, FrozenSet as FSet
from dataclasses import dataclass, field
from math import inf
from io import StringIO
import datetime
import traceback

DEBUG: bool = False

class StateSet(set):
    def add(self,state:dict) -> None:
        set.add(self,j_dump(state))
#? END class StateSet

@dataclass(eq=True,frozen=True,unsafe_hash=False)
class RompID:
    romp_vers: str
    model: str
    romp_init_time: int
    root_seed: str
    # seed: Un[str,int]
    def __str__(self) -> str:
        time_stamp = datetime.datetime.fromtimestamp(self.romp_init_time).strftime('%y-%m-%d %H:%M:%S')
        model_name = os.path.split(self.model)[1] # .rsplit('.',1)[0]
        return f"{time_stamp} :: {model_name} w/ seed: {self.root_seed}"
    #? END def __str__(self) -> str
#? END @dataclass RompID

@dataclass(eq=True,frozen=True,unsafe_hash=False)
class TraceID:
    trace_dir: str
    file_name: str
    seed: int
#? END @dataclass TraceID

class RompTraceParseError(Exception):
    pass
#? END class RompTraceParseError

# @dataclass(init=False,unsafe_hash=True,frozen=False,eq=False)
class StatRange:
    # min: int
    # max: int
    # total: int
    # n: int
    # __data: list = field(repr=False,hash=False)
    def __init__(self) -> None:
        self.min: int = inf
        self.max: int = -inf
        self.total: int = 0
        self.n: int = 0
        self.__data: list = list()
    def add(self,data,n:int=1) -> None:
        self.max = max(data,self.max)
        self.min = min(data,self.min)
        self.total += data * n
        self.n += n
        self.__data.append((data,n))
    @property
    def avg(self) -> float:
        if self.n == 0:
            return None
        return self.total / self.n
    def __len__(self) -> int:
        return self.n
    # def __iadd__(self,other):
    #     self.add(other)
    #     return self
    def __repr__(self) -> str:
        return f"StatRange(n={self.n},min={self.min},avg={self.avg},max={self.max})"
#? END class StatRange
    

@dataclass(init=False)
class TraceData:
    id: TraceID
    romp_id: RompID
    seed: Un[str,int]
    attempts: StatRange
    depth: int
    tries: int
    is_valid: bool
    is_error: bool
    unique_states: FSet[str]
    property_violated: Un[None,str,dict]
    property_violated_inside: Un[None,str,dict]
    def __init__(self,_trace_dir:str,_file_name:str) -> None:
        self.id: TraceID = None
        self.romp_id: RompID = None
        self.seed: Un[str,int] = None
        self.attempts: StatRange = StatRange()
        self.depth: int = 0
        self.tries: int = 0
        self.is_valid: bool = None
        self.is_error: bool = None
        self.unique_states: FSet[str] = None
        self.property_violated: Un[None,str,dict] = None
        self.property_violated_inside: Un[None,str,dict] = None
        self.__process_trace_file(_trace_dir,_file_name)
    #? END def __init__()
    def __process_trace_file(self,_trace_dir:str,_file_name:str) -> None:
        with open(_trace_dir+'/'+_file_name,'r') as tf:
            json = j_load(tf)
        try :
            if json['$type'] != "romp-trace":
                raise Exception("Not a romp trace file")
            self.id = TraceID(_trace_dir,_file_name,json['metadata']['seed'])
            self.romp_id = RompID(json['$version'],
                                json['metadata']['model'],
                                int(json['metadata']['romp-id']),
                                json['metadata']['root-seed'])
            _attempts = 0
            attempts = StatRange()
            unique_states: StateSet = StateSet()
            unique_states.add(json['trace'][0]['state'])
            for elm in json['trace'][1:]:
                self.tries += 1
                if elm['$type'] != "rule-applied":
                    if DEBUG and self.attempts is None:
                        print("DEBUG: StatRange was None!!")
                    attempts.add(_attempts)
                    _attempts = 0
                else:
                    _attempts += 1
                    unique_states.add(elm['state'])
            results = json['results']
            self.depth = int(results['depth'])
            self.is_valid = results['valid']
            self.is_error = bool(results['is-error'])
            self.property_violated = results['property-violated']
            self.property_violated_inside = results['tripped-inside']
            self.unique_states = frozenset(unique_states)
            self.attempts = attempts
        except Exception as ex:
            if DEBUG:
                traceback.print_exc()
            raise RompTraceParseError(str(ex))
    #? END def __process_trace_file()
    @property
    def missed_rules(self) -> int:
        return self.tries - self.depth
    #? END @property avg_attempts(self) -> float:
    @property
    def rule_miss_rate(self) -> float:
        return self.missed_rules / self.tries
    #? END @property avg_attempts(self) -> float:
    @property
    def rule_hit_rate(self) -> float:
        return self.depth / self.tries
    #? END @property avg_attempts(self) -> float:
    @property
    def unique_state_count(self) -> float:
        return len(self.unique_states)
    #? END @property avg_attempts(self) -> float:
    @property
    def state_hit_rate(self) -> float:
        return self.unique_state_count / self.depth
    #? END @property avg_attempts(self) -> float:
    @property
    def abs_state_hit_rate(self) -> float:
        return self.unique_state_count / self.tries
    #? END @property avg_attempts(self) -> float:
    @property
    def state_miss_rate(self) -> float:
        return (self.depth - self.unique_state_count) / self.depth
    #? END @property avg_attempts(self) -> float:
    @property
    def abs_state_miss_rate(self) -> float:
        return (self.tries - self.unique_state_count) / self.tries
    #? END @property avg_attempts(self) -> float:
    def __hash__(self) -> int:
        return hash(self.id)
    #? END def __hash__(self) -> int:
#? END @dataclass TraceData

@dataclass(init=False,eq=False,unsafe_hash=True,frozen=False)
class ModelResult:
    id: RompID
    traces: Set[TraceID]
    # attempts: StatRange
    avg_attempts: StatRange
    depth: StatRange
    missed_rules: StatRange
    tries: StatRange
    rule_hit_rate: StatRange
    rule_miss_rate: StatRange
    state_hit_rate: StatRange
    state_miss_rate: StatRange
    abs_state_hit_rate: StatRange
    abs_state_miss_rate: StatRange
    unique_state_count: StatRange
    unique_states: StateSet
    properties_violated: Set[Un[None,str]]
    errors_found: int
    def __init__(self,_romp_id:RompID) -> None:
        self.id: RompID = _romp_id
        self.traces: Set[TraceID] = set()
        # self.attempts: StatRange = StatRange()
        self.avg_attempts: StatRange = StatRange()
        self.depth: StatRange = StatRange()
        self.missed_rules: StatRange = StatRange()
        self.tries: StatRange = StatRange()
        self.rule_hit_rate: StatRange = StatRange()
        self.rule_miss_rate: StatRange = StatRange()
        self.state_hit_rate: StatRange = StatRange()
        self.state_miss_rate: StatRange = StatRange()
        self.abs_state_hit_rate: StatRange = StatRange()
        self.abs_state_miss_rate: StatRange = StatRange()
        self.unique_state_count: StatRange = StatRange()
        self.unique_states: StateSet = StateSet()
        self.properties_violated: Set[Un[None,str]] = set()
        self.errors_found: int = 0
    #? END __init__()
    def add(self,trace:TraceData) -> None:
        if trace is None: return None
        if not isinstance(trace, TraceData):
            raise Exception("ModelResult.add() : can only accept TraceData objects !!")
        if trace in self.traces:
            return None
        self.traces.add(trace)
        # self.attempts.add(trace.attempts.avg,trace.attempts.total)
        self.avg_attempts.add(trace.attempts.avg,trace.attempts.n)
        self.depth.add(trace.depth)
        self.missed_rules.add(trace.missed_rules)
        self.rule_hit_rate.add(trace.rule_hit_rate)
        self.rule_miss_rate.add(trace.rule_miss_rate)
        self.state_hit_rate.add(trace.state_hit_rate) 
        self.state_miss_rate.add(trace.state_miss_rate) 
        self.abs_state_hit_rate.add(trace.abs_state_hit_rate) 
        self.abs_state_miss_rate.add(trace.abs_state_miss_rate) 
        self.tries.add(trace.tries)
        self.unique_state_count.add(trace.unique_state_count)
        self.unique_states |= trace.unique_states
        if trace.property_violated is not None:
            self.properties_violated.add(trace.property_violated)
        if trace.is_error: 
            self.errors_found += 1
    #? END def add() -> None:
    @property
    def trace_count(self) -> int:
        return len(self.traces)
    #? END @property trace_count() -> int:
    @property
    def abs_unique_state_count(self) -> int:
        return len(self.unique_states)
    #? END @property unique_state_count() -> int:
    @property
    def error_detection_rate(self) -> float:
        return self.errors_found / len(self.traces)
    #? END @property unique_state_count() -> int:
    # def __hash__(self) -> int:
    #     return hash(self.id)
    # #? END def __hash__(self) -> int:
    # def __eq__(self,other) -> int:
    #     return isinstance(other,ModelResult) and hash(self) == hash(other)
    # #? END def __eq__(self) -> int:
    # def __ne__(self,other) -> int:
    #     return not self == other
    # #? END def __ne__(self) -> int:
    def __str__(self) -> str:
        return (f"{'='*80}\n"
                f"  {self.id!s:^76s}  \n"
                f"  {'-'*76}  \n"
                f"      walks: {len(self.traces)!s:<67s}\n"
                f"  ERRORS:\n"
                f"     #-disc: {self.errors_found:<67g}\n"
                f"       rate: {self.error_detection_rate:<67g}\n"
                f"  RULES:\n"
                f"             {'min:':>16s} {'avg:':>16s} {'max:':>16s} {'total:':>16s}\n"
                f"       hits: {self.depth.min:16g} {self.depth.avg:16.4g} {self.depth.max:16g} {self.depth.total:16g}\n"
                f"     misses: {self.missed_rules.min:16g} {self.missed_rules.avg:16.4g} {self.missed_rules.max:16g} {self.missed_rules.total:16g}\n"
                f"      total: {self.tries.min:16g} {self.tries.avg:16.2g} {self.tries.max:16g} {self.tries.total:16g}\n"
                f"             {'min:':>16s} {'avg:':>16s} {'max:':>16s}\n"
                f"   hit-rate: {self.rule_hit_rate.min:16g} {self.rule_hit_rate.avg:16.4g} {self.rule_hit_rate.max:16g}\n"
                f"  miss-rate: {self.rule_miss_rate.min:16g} {self.rule_miss_rate.avg:16.4g} {self.rule_miss_rate.max:16g}\n"
                f"             {'avg-min:':>16s} {'avg:':>16s} {'avg-max:':>16s}\n"
                f"   mStreaks: {self.avg_attempts.min:16g} {self.avg_attempts.avg:16.4g} {self.avg_attempts.max:16g}\n"
                f"  STATES:\n"
                f"   #-unique: {self.abs_unique_state_count:<67d}\n"
                f"             {'min:':>16s} {'avg:':>16s} {'max:':>16s}\n"
                f"   hit-rate: {self.state_hit_rate.min:16.2g} {self.state_hit_rate.avg:16.4g} {self.state_hit_rate.max:16.2g}\n"
                f"  miss-rate: {self.state_miss_rate.min:16.2g} {self.state_miss_rate.avg:16.4g} {self.state_miss_rate.max:16.2g}\n"
                f"  abs-hit-r: {self.abs_state_hit_rate.min:16.2g} {self.abs_state_hit_rate.avg:16.4g} {self.abs_state_hit_rate.max:16.2g}\n"
                f" abs-miss-r: {self.abs_state_miss_rate.min:16.2g} {self.abs_state_miss_rate.avg:16.4g} {self.abs_state_miss_rate.max:16.2g}\n"
                f"      total: {self.unique_state_count.min:16.2g} {self.unique_state_count.avg:16.4g} {self.unique_state_count.max:16.2g}\n"
                f"  PROPS:\n"
                f"   violated: {len(self.properties_violated):<67d}\n"
                f"{'='*80}\n")
    #? END def __str__(self) -> str:
#? END @dataclass ModelResult

class ModelResults(dict):
    def add(self,trace:Un[TraceData,None]) -> None:
        if trace is None: return None
        if not isinstance(trace, TraceData):
            raise Exception("ModelResults.add() : can only accept TraceData objects !!")
        if trace.romp_id not in self:
            self.update({trace.romp_id: ModelResult(trace.romp_id)})
        self[trace.romp_id].add(trace)
    #? END def add() -> None
#? END class ModelResults


def fs_DFS(path, dirCallback=None, fileCallback=None) -> List[str]:
    if not os.path.exists(path):
        raise Exception("Path does not exist!!")
    stack = []
    ret = []
    stack.append(path);
    while len(stack) > 0:
        tmp = stack.pop(len(stack) - 1)
        if(os.path.isdir(tmp)):
            ret.append(tmp)
            for item in os.listdir(tmp):
                stack.append(os.path.join(tmp, item))
            if dirCallback:
                dirCallback(tmp)
        elif(os.path.isfile(tmp)):
            ret.append(tmp)
            if fileCallback:
                fileCallback(tmp)
    return ret
#? END def fs_DFS() -> List[str]

def genTraceData(path:str) -> Un[TraceData,None]:
    dir_name, file_name = os.path.split(path)
    if not file_name.endswith('.json'): return None
    try:
        return TraceData(dir_name,file_name)
    except RompTraceParseError as te:
        print(f"{path:<16s} :parse-error: {te!s}")
    except Exception as ex:
        print(f"{path:<16s} :exception: {ex!s}")
    except:
        print(f"{path:<16s} :???-error: ???")
    return None # error occurred return none
#? END def genTraceData() -> Un[TraceData,None]

def process_data(trace_dir:str) -> ModelResults:
    results = ModelResults()
    file_paths = fs_DFS(trace_dir,fileCallback=lambda p: results.add(genTraceData(p)))
    return results
#? END def process_data() -> ModelResults:
    

def print_results(results:ModelResults) -> None:
    print()
    for key in results:
        print(results[key])
#?END def print_results() -> None:



def main() -> None:
    '''perform the functionality of the code'''
    trace_dir = "./traces"
    if (len(argv) >= 2):
        trace_dir = argv[1].strip()
        while trace_dir[-1] in '/\\':
            trace_dir = trace_dir[:len(trace_dir)-1]
    results = process_data(trace_dir)
    print_results(results)
#? END def main() -> None

if __name__ == "__main__":
    if DEBUG:
        _t = StatRange()
        if _t is None:
            print("StatRange() returns None")
        print(repr(StatRange()))
    main()

