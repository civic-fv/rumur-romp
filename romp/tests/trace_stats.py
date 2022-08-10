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

from sys import argv
from json import load as j_load, dump as j_dump
from typing import List, Typed, Union as Un, MutableSet as Set, Tuple as Tp
from dataclasses import dataclass
from math import inf


class StateSet(set):
    def add(self,state:dict) -> None:
        set.add(self,j_dump(state))
#? END class StateSet

@dataclass(eq=True,frozen=True,unsafe_hash=False)
class RompID:
    romp_vers: str
    model: str
    romp_id: int
    root_seed: str
    # seed: Un[str,int]
#? END @dataclass RompID

@dataclass(eq=True,frozen=True,unsafe_hash=False)
class TraceID:
    trace_dir: str
    file_name: str
    seed: int
#? END @dataclass TraceID

# @dataclass(init=False)
class TraceData:
    # trace_file: str
    # model_id: str
    # ID: RompID
    # seed: Un[str,int]
    # max_attempts: int
    # min_attempts: int
    # depth: int
    # unique_states: StateSet
    # properties_violated: Un[None,str]
    def __init__(self,_trace_dir:str,_file_name:str) -> None:
        self.ID: TraceID = None
        self.romp_ID: RompID = None
        self.seed: Un[str,int] = None
        self.max_attempts: int = -inf
        self.min_attempts: int = inf
        self.__avg_attempts_sum: int = 0
        self.depth: int = 0
        self.is_valid: bool = None
        self.is_error: bool = None
        self.unique_states: StateSet = StateSet()
        self.property_violated: Un[None,str,dict] = None
        self.property_violated_inside: Un[None,str,dict] = None
        self.__process_trace_file(_trace_dir,_file_name)
    #? END def __init__()
    def __process_trace_file(self,_trace_dir:str,_file_name:str) -> None:
        with open(_trace_dir+'/'+_file_name,'r') as tf:
            json = j_load(tf)
        self.ID = TraceID(_trace_dir,_file_name,json['metadata']['seed'])
        self.romp_ID = RompID(json['$version'],
                              json['metadata']['model'],
                              int(json['metadata']['romp-id']),
                              json['metadata']['root-seed'])
        attempts = 0
        self.unique_states.add(json['trace'][0]['state'])
        for elm in json['trace'][1:]:
            if elm['$type'] != "rule-applied":
                self.min_attempts = min(attempts,self.min_attempts)
                self.max_attempts = max(attempts,self.max_attempts)
                self.__avg_attempts_sum += attempts
                attempts = 0
            else:
                attempts += 1
                self.unique_states.add(elm['state'])
        results = json['results']
        self.depth = int(results['depth'])
        self.is_valid = results['valid']
        self.is_error = results['is-error']
        self.property_violated = results['property-violated']
        self.property_violated_inside = results['violated-inside']
    #? END def __process_trace_file()
    @property
    def avg_attempts(self) -> float:
        return self.__avg_attempts_sum / self.depth
    #? END @property avg_attempts(self) -> float:
#? END @dataclass TraceData

class ModelResult:
    def __init__(self,_romp_id:RompID) -> None:
        self.ID: RompID = _romp_id
        self.romp_ids: Set[TraceID] = set()
        self.traces: List[TraceData]
        self.max_attempts: int = -inf
        self.min_attempts: int = inf
        self.max_avg_attempts: int = -inf
        self.min_avg_attempts: int = inf
        self.__avg_attempts_sum: int = 0
        self.max_depth: int = -inf
        self.min_depth: int = inf
        self.total_rules_applied: int = 0
        self.__avg_depth_sum: int = 0
        self.unique_states: StateSet = StateSet()
        self.properties_violated: Set[Un[None,str]] = set()
    #? END __init__()
    def add(self,trace:TraceData) -> None:
        if type(trace) is not TraceData:
            raise Exception("ModelResult.add() : can only accept TraceData objects !!")
        if trace.ID in self.romp_ids:
            return
        self.romp_ids.add(trace.ID)
        self.traces.append(trace)
        self.max_attempts = max(trace.max_attempts, self.max_attempts)
        self.min_attempts = min(trace.min_attempts, self.min_attempts)
        self.max_avg_attempts = max(trace.max_attempts, self.max_avg_attempts)
        self.min_avg_attempts = min(trace.min_attempts, self.min_avg_attempts)
        self.max_depth = max(trace.depth, self.max_depth)
        self.min_depth = min(trace.depth, self.min_depth)
        self.__avg_depth_sum += trace.depth
        self.__avg_attempts_sum += trace.avg_attempts * trace.depth
        self.total_rules_applied += trace.depth
        self.unique_states |= trace.unique_states
        self.properties_violated.add(trace.property_violated)
    #? END def add() -> None:
    @property
    def avg_depth(self) -> float:
        return self.__avg_attempts_sum / self.total_rules_applied
    #? END @property avg_depth() -> float:
    @property
    def avg_attempts(self) -> float:
        return self.__avg_depth_sum / len(self.traces)
    #? END @property avg_depth() -> float:
    @property
    def trace_count(self) -> int:
        return len(self.traces)
    #? END @property trace_count() -> int:
    @property
    def unique_state_count(self) -> int:
        return len(self.unique_states)
    #? END @property unique_state_count() -> int:
#? END @dataclass ModelResult

class ModelResults(dict):
    def add(self,trace:TraceData) -> None:
        if type(trace) is not TraceData:
            raise Exception("ModelResults.add() : can only accept TraceData objects !!")
        if trace.ID not in self:
            self.update({trace.romp_ID: ModelResult(trace.romp_ID)})
        self[trace.model_id].add(trace)
    #? END def add() -> None
#? END class ModelResults


def process_data(trace_dir:str) -> ModelResults:
    pass



def main() -> None:
    '''perform the functionality of the code'''
    trace_dir = "./traces"
    if (len(argv) >= 2):
        trace_dir = argv[1].strip()
        while trace_dir[-1] in '/\\':
            trace_dir = trace_dir[:len(trace_dir)-1]
    results = 
#? END def main() -> None

if __name__ == "__main__":
    main()

