#!/usr/bin/env python
import sys
import os

#planner = "/planner/ff-version/bfws"
planner = "./bfws"
domain = sys.argv[1]
problem = sys.argv[2]
plan = sys.argv[3]

"""
" Run 1-BFWS
"""

cmd = "{} --domain {}  --problem {} --output {} --1-BFWS 1".format(planner,domain,problem,plan)
os.system(cmd)
print(cmd)

"""
" Run 2-C-BFWS if not solved
"""
if not os.path.exists(plan):
    cmd = "{} --domain {}  --problem {} --output {} --k-C-BFWS 1".format(planner,domain,problem,plan)
    os.system(cmd)

    
"""
" Run 2-M-BFWS, M=1,2,4,8,16,32
"""
if not os.path.exists(plan):
    for exponent in range(0,6):
        cmd = "{} --domain {}  --problem {} --output {} --k-M-BFWS {}".format(planner,domain,problem,plan, 2 ** exponent)
        os.system(cmd)
        if os.path.exists(plan):
            break
        
    

                
