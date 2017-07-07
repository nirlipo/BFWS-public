#! /usr/bin/env python
import fd.grounding
import sys
import os
from libbfws import BFWS 
# MRJ: Profiler imports
#from prof import profiler_start, profiler_stop

def main( domain_file, problem_file, search_alg ) :
	task = BFWS( )

        task.ignore_action_costs = True
        
	fd.grounding.default( domain_file, problem_file, task )

	#NIR: Uncomment to check what actions are being loaded
	#for i in range( 0, task.num_actions() ) :
	#	task.print_action( i )

	# NIR: Setting planner parameters is as easy as setting the values
	# of Python object attributes
	
	# NIR: log filename set
	task.log_filename = 'bfws.log'

	# NIR: search alg
	task.search = search_alg

        # NIR: Set Max novelty to 2
        task.max_novelty = 2

        # NIR: Set M to 32
        task.m_value = 32
        
	# NIR: Comment line below to deactivate profiling
	#profiler_start( 'bfws.prof' )

	# NIR: We call the setup method in SIW_Planner
	task.setup()

	# NIR: And then we're ready to go
	task.solve()

	#NIR: Comment lines below to deactivate profile
	#profiler_stop()	

	#rv = os.system( 'google-pprof --pdf libbfws.so bfws.prof > bfws.pdf' )
	#if rv != 0 :
	#	print >> sys.stderr, "An error occurred while translating google-perftools profiling information into valgrind format"


def debug() :
	main( "/Users/nirlipo/Sandboxes/trapper/trapper/examples/domain.pddl",
              "/Users/nirlipo/Sandboxes/trapper/trapper/examples/prob3.pddl",
              "fast" )

if __name__ == "__main__":
	main( sys.argv[1], sys.argv[2], sys.argv[3] )

