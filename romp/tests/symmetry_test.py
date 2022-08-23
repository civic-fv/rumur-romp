#!/usr/bin/python3

from os import system as run_cmd, path
from sys import argv
from trace_stats import process_data
from time import time


DEBUG: bool = False


def print_results(results) -> None:
    if len(results) != 2 :
        print(f"Error :: found unexpected trace data in trace dir !! (n={len(results)})")
        if not DEBUG: return
    sym = None
    no_sym = None
    for key in results:
        res = results[key]
        if res.do_symmetry:
            if DEBUG and sym is not None: 
                print("ERROR :: more than one sym trace !!")
            sym = res
        else:
            if DEBUG and no_sym is not None: 
                print("ERROR :: more than one no sym trace !!")
            no_sym = res
        # print(res)
        
    if DEBUG:
        if sym is None :
            print("ERROR :: missing sim trace data !!")
        if no_sym is None: 
            print("ERROR :: missing no sim trace data !!")
            
    for l_s,l_n in zip(str(sym).split('\n'),str(no_sym).split('\n')):
        print(f"{l_s:<80s} || {l_n:<80s}")
        
    # TODO: compare and give summery
    pass
#? END def print_results() -> None:
   
   
   
def run_tests(model_path:str,
              test_args:str=None,
              trace_dir:str="./traces",
              romp_path:str="build/romp/romp",
              cc:str="g++") -> None:
    root_seed = f"{int(time()):x}"
    if test_args is None:
        test_args = "-s '{}' -d 2000 -w 64 -ll"
    test_args = test_args.format(root_seed)
    trace_dir = path.join(trace_dir,root_seed)
    # generate model checker
    ret = run_cmd(f"{romp_path} --simple-trace --do-measure "
                  f"-s -o{model_path}.sym.test.cpp {model_path}")
    if ret != 0: return
    # build sym model
    ret = run_cmd(f"{cc} -g -O2 -pthread -D__romp__ENABLE_symmetry " 
                  f"-o{model_path}.sym.test {model_path}.sym.test.cpp")
    if ret != 0: return
    # build no sym model
    ret = run_cmd(f"{cc} -g -O2 -pthread " 
                  f"-o{model_path}.nosym.test {model_path}.sym.test.cpp")
    if ret != 0: return
    sym_t_dir = path.join(trace_dir,path.basename(model_path)+'_sym')
    no_sym_t_dir = path.join(trace_dir,path.basename(model_path)+'_nsym')
    # run sym model checker
    ret = run_cmd(f"{model_path}.sym.test {test_args} -t '{sym_t_dir}'")
    if ret != 0: return
    # run no sym model checker
    ret = run_cmd(f"{model_path}.nosym.test {test_args} -t '{no_sym_t_dir}'")
    if ret != 0: return
    
    print_results(process_data(trace_dir))
#? END def run_tests() -> None: 



def main() -> None:
    '''do the task'''
    i = len(argv) - 1
    cc = 'g++'
    if i == 5:
        cc = argv[i]
        i -= 1
    romp_path = 'build/romp/romp'
    if i == 4:
        romp_path = argv[i]
        i -= 1
    trace_dir = './traces'
    if i == 3:
        trace_dir = argv[i]
        i -= 1
    romp_args = None
    if i == 2:
        romp_args = argv[i] if argv[i] != '' else None
        i -= 1
    if i == 1:
        model_path = argv[i]
    else:
        print("ERROR no model was provided!")
        return
    
    run_tests(model_path,romp_args,trace_dir,romp_path,cc)
#? END def main() -> None


if __name__ == "__main__":
    main()



