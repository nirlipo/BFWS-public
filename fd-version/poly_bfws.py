#! /usr/bin/env python
import fd.grounding
import sys
import os
from libbfws import BFWS 
# NIR: Profiler imports
#import yep

def main( domain_file, problem_file, output ) :
	task = BFWS( )

        #NIR: Costs are parsed, but the search ignores action costs. If set to True, it will report plan length        
        task.ignore_action_costs = False
        
	fd.grounding.default( domain_file, problem_file, task )

	#NIR: Uncomment to check what actions are being loaded
	#for i in range( 0, task.num_actions() ) :
	#	task.print_action( i )

	# NIR: Setting planner parameters is as easy as setting the values
	# of Python object attributes
	
	# NIR: log and plan filename set
	task.log_filename = 'bfws.log'
        task.plan_filename = output

	# NIR: search alg
	task.search = "POLY-BFWS"

        # NIR: Set Max novelty to 2 (Default: 2)
        #task.max_novelty = 2

        # NIR: Set M to 32 (Default: 32)
        #task.m_value = 32
        
	# NIR: Comment line below to deactivate profiling
	#yep.start( 'bfws.prof' )
        
	# NIR: We call the setup method in SIW_Planner
	task.setup()

	# NIR: And then we're ready to go
	task.solve()

	#NIR: Comment lines below to deactivate profile
	#yep.stop()	

	#rv = os.system( 'google-pprof --pdf libbfws.so bfws.prof > bfws.pdf' )
	#if rv != 0 :
	#	print >> sys.stderr, "An error occurred while translating google-perftools profiling information into valgrind format"


def debug() :
	main( "domain.pddl",
              "p01.pddl",
              "solution" )

if __name__ == "__main__":
	main( sys.argv[1], sys.argv[2], sys.argv[3] )

