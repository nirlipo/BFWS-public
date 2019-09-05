/*
Lightweight Automated Planning Toolkit
Copyright (C) 2012
Miquel Ramirez <miquel.ramirez@rmit.edu.au>
Nir Lipovetzky <nirlipo@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>

/**
 * LAPKT includes
 */
#include <ff_to_aptk.hxx>
#include <strips_prob.hxx>
#include <fluent.hxx>
#include <action.hxx>
#include <cond_eff.hxx>
#include <strips_state.hxx>
#include <fwd_search_prob.hxx>

#include <novelty_partition.hxx>
#include <landmark_graph.hxx>
#include <landmark_graph_generator.hxx>
#include <landmark_graph_manager.hxx>
#include <h_2.hxx>
#include <h_1.hxx>
#include <layered_h_max.hxx>


#include <aptk/open_list.hxx>
#include <aptk/string_conversions.hxx>

#include <boost/program_options.hpp>

/**
 * Local includes
 */

//NIR: Files adapted from LAPKT
#include "novelty_partition_2.hxx"
#include "ff_rp_heuristic.hxx"
#include "rp_heuristic_bfws.hxx"
#include "landmark_count.hxx"

#include "bfws_4h.hxx"
#include "bfws_2h.hxx"
#include "bfws_2h_M.hxx"
#include "bfws_2h_consistency.hxx"
#include "bfws_2h_consistency_M.hxx"


#include "new_node_comparer.hxx"

namespace po = boost::program_options;

/**
 * NAMESPACES 
 */


//NIR: Model 
using	aptk::STRIPS_Problem;
using	aptk::agnostic::Fwd_Search_Problem;
using	aptk::Action;


//NIR: Heuristics
using 	aptk::agnostic::Landmarks_Graph;
using 	aptk::agnostic::Landmarks_Graph_Generator;
using   aptk::agnostic::Landmarks_Graph_Manager;
using 	aptk::agnostic::Landmarks_Count_Heuristic;

using 	aptk::agnostic::H2_Heuristic;
using 	aptk::agnostic::H1_Heuristic;
using   aptk::agnostic::Layered_H_Max;

using	aptk::agnostic::H_Add_Evaluation_Function;
using	aptk::agnostic::H_Max_Evaluation_Function;
using	aptk::agnostic::Relaxed_Plan_Heuristic;
using   aptk::agnostic::FF_Relaxed_Plan_Heuristic;


//NIR: Novelties
using 	aptk::agnostic::Novelty_Partition;
using 	aptk::agnostic::Novelty_Partition_2;


//NIR: Open List and evaluation functions
using 	aptk::search::Open_List;
using	aptk::search::Node_Comparer_4H;
using	aptk::search::Node_Comparer_2H;


//NIR: Search Engines
using	aptk::search::bfws_4h::BFWS_4H;
using	aptk::search::bfws_2h::BFWS_2H;
using	aptk::search::bfws_2h::BFWS_2H_M;
using	aptk::search::bfws_2h::BFWS_2H_Consistency;
using	aptk::search::bfws_2h::BFWS_2H_Consistency_M;

/**
 * DEFINITIONS
 */

//NIR: Heuristics
typedef         H2_Heuristic<Fwd_Search_Problem>                  H2_Fwd;
typedef         Landmarks_Graph_Generator<Fwd_Search_Problem>     Gen_Lms_Fwd;
typedef         Landmarks_Count_Heuristic<Fwd_Search_Problem>     H_Lmcount_Fwd;
typedef         Landmarks_Graph_Manager<Fwd_Search_Problem>       Land_Graph_Man;



// NIR: Node representations for each search algorithm
typedef		aptk::search::bfws_4h::Node< Fwd_Search_Problem, aptk::State >	Search_Node_4h;
typedef		aptk::search::bfws_2h::Node< Fwd_Search_Problem, aptk::State >	Search_Node_2h;

// NIR: Novelty functions for each node type. Novelty partition expects class
// node to define partition() function. '_2' version expects partition2() function.  
typedef         Novelty_Partition<Fwd_Search_Problem, Search_Node_4h>               H_Novel_Fwd_4h;
typedef         Novelty_Partition_2<Fwd_Search_Problem, Search_Node_4h>             H_Novel_2_Fwd_4h;

typedef         Novelty_Partition<Fwd_Search_Problem, Search_Node_2h>               H_Novel_Fwd_2h;



// NIR: Then we define the type of the tie-breaking algorithm
// for the open list we are going to use
typedef		Node_Comparer_4H< Search_Node_4h >				Tie_Breaking_Algorithm_4h;
typedef		Node_Comparer_2H< Search_Node_2h >	        		Tie_Breaking_Algorithm_2h;


// NIR: Now we define the Open List type by combining the types we have defined before
typedef		Open_List< Tie_Breaking_Algorithm_4h, Search_Node_4h >	          	        BFS_Open_List_4h;
typedef		Open_List< Tie_Breaking_Algorithm_2h, Search_Node_2h >	                        BFS_Open_List_2h;



// NIR: Now we define the heuristics
typedef		H1_Heuristic<Fwd_Search_Problem, H_Add_Evaluation_Function>	H_Add_Fwd;
typedef		Relaxed_Plan_Heuristic< Fwd_Search_Problem, H_Add_Fwd >		H_Add_Rp_Fwd;

typedef         Layered_H_Max< Fwd_Search_Problem >				      Alt_H_Max;
typedef         FF_Relaxed_Plan_Heuristic< Fwd_Search_Problem, Alt_H_Max, unsigned >     Classic_FF_H_Max;

// NIR: Now we're ready to define the BFS algorithm we're going to use, H_Lmcount can be used only with goals,
// or with landmarks computed from s0
 typedef	BFWS_2H< Fwd_Search_Problem, H_Novel_Fwd_2h, H_Lmcount_Fwd, H_Add_Rp_Fwd,  BFS_Open_List_2h>                       k_BFWS;
 typedef	BFWS_2H_M< Fwd_Search_Problem, H_Novel_Fwd_2h, H_Lmcount_Fwd, H_Add_Rp_Fwd,  BFS_Open_List_2h >                    k_BFWS_M;
 typedef        BFWS_4H< Fwd_Search_Problem, H_Novel_Fwd_4h, H_Lmcount_Fwd, H_Novel_2_Fwd_4h, H_Add_Rp_Fwd,  BFS_Open_List_4h >    BFWS_w_hlm_hadd;

// NIR: Consistency Search variants
 typedef	BFWS_2H_Consistency< Fwd_Search_Problem, H_Novel_Fwd_2h, H_Lmcount_Fwd, H_Add_Rp_Fwd,  BFS_Open_List_2h >    k_BFWS_Consistency;
 typedef	BFWS_2H_Consistency_M< Fwd_Search_Problem, H_Novel_Fwd_2h, H_Lmcount_Fwd, H_Add_Rp_Fwd, BFS_Open_List_2h >   k_BFWS_Consistency_M;




template <typename Search_Engine>
void bfws_options( Fwd_Search_Problem&	search_prob, Search_Engine& bfs_engine, unsigned& max_novelty, Landmarks_Graph& graph){

	bfs_engine.set_max_novelty( max_novelty );
	bfs_engine.set_use_novelty( true );
	bfs_engine.rel_fl_h().ignore_rp_h_value(true);

	//NIR: engine doesn't own the pointer, need to free at the end
	Land_Graph_Man* lgm = new Land_Graph_Man( search_prob, &graph);
	bfs_engine.use_land_graph_manager( lgm );

	//NIR: Approximate the domain of #r counter, so we can initialize the novelty table, making sure we've got
	//     space for novelty > 1 tuples 
	H_Add_Rp_Fwd hadd( search_prob );
	float h_init=0;
	const aptk::State* s_0 = search_prob.init();
	hadd.eval( *s_0, h_init );
	
	bfs_engine.set_arity( max_novelty, graph.num_landmarks()*h_init );

	
}

template <typename Search_Engine>
float do_search( Search_Engine& engine, STRIPS_Problem& plan_prob, std::ofstream& plan_stream, bool &found_plan ) {

	std::ofstream	details( "execution.details" );	
	engine.start();

	std::vector< aptk::Action_Idx > plan;
	float				cost = -1;

	float ref = aptk::time_used();
	float t0 = aptk::time_used();

	unsigned expanded_0 = engine.expanded();
	unsigned generated_0 = engine.generated();

	found_plan = engine.find_solution( cost, plan );
	
	if ( found_plan  ) {
		details << "Plan found with cost: " << cost << std::endl;
		for ( unsigned k = 0; k < plan.size(); k++ ) {
			details << k+1 << ". ";
			const aptk::Action& a = *(plan_prob.actions()[ plan[k] ]);
			details << a.signature();
			details << std::endl;
			plan_stream << a.signature() << std::endl;
		}
		float tf = aptk::time_used();
		unsigned expanded_f = engine.expanded();
		unsigned generated_f = engine.generated();
		details << "Time: " << tf - t0 << std::endl;
		details << "Generated: " << generated_f - generated_0 << std::endl;
		details << "Expanded: " << expanded_f - expanded_0 << std::endl;
		t0 = tf;
		expanded_0 = expanded_f;
		generated_0 = generated_f;
		plan.clear();

		float total_time = aptk::time_used() - ref;
		std::cout << "Total time: " << total_time << std::endl;
		std::cout << "Nodes generated during search: " << engine.generated() << std::endl;
		std::cout << "Nodes expanded during search: " << engine.expanded() << std::endl;
		std::cout << "Plan found with cost: " << cost << std::endl;
		details.close();
		return total_time;
	}
	else{
		float tf = aptk::time_used();
		unsigned expanded_f = engine.expanded();
		unsigned generated_f = engine.generated();
		details << "Time: " << tf - t0 << std::endl;
		details << "Generated: " << generated_f - generated_0 << std::endl;
		details << "Expanded: " << expanded_f - expanded_0 << std::endl;
		t0 = tf;
		expanded_0 = expanded_f;
		generated_0 = generated_f;

	 	float total_time = aptk::time_used() - ref;	
		std::cout << "Total time: " << total_time << std::endl;
		std::cout << "Nodes generated during search: " << engine.generated() << std::endl;
		std::cout << "Nodes expanded during search: " << engine.expanded() << std::endl;
		std::cout << "Plan found with cost: NOTFOUND" << std::endl;
		details.close();
		return total_time;
	}				      
}


void process_command_line_options( int ac, char** av, po::variables_map& vars ) {
	po::options_description desc( "Options:",135,260 );
	
	desc.add_options()
		( "help", "Show help message. " )
		( "domain", po::value<std::string>(), "Input PDDL domain description" )
		( "problem", po::value<std::string>(), "Input PDDL problem description" )
		( "output", po::value<std::string>(), "Output file for plan" )
		( "max_novelty", po::value<int>()->default_value(2), "Max width w for novelty (default 2)")
		( "ignore_costs", po::value<bool>()->default_value(true), "Ignore action costs")
		;
	po::options_description desc_search( "Search Algorithms:",135,260 );
	
	desc_search.add_options()
		( "DUAL-BFWS", po::value<bool>()->default_value(true), "1-BFWS first, then BFWS using h_ff and h_landcount as in AAAI-17 paper")
		( "DUAL-C-BFWS", po::value<bool>()->default_value(false), "1-C-BFWS first, then BFWS using h_ff and h_landcount")
		( "BFWS-f5", po::value<bool>()->default_value(false),
  		        "BFWS(w,#g), w_{#r,#g}, as in BFWS(f5) AAAI-17 paper" )
		( "BFWS-f5-initstate-relevant", po::value<bool>()->default_value(false),
			"BFWS(f5) but computing relevant fluents only once from s0" )
		( "BFWS-f5-landmarks", po::value<bool>()->default_value(false),
			"BFWS(w,h_landcount), w = w_{#r,h_landcount} " )	  
		( "BFWS-goalcount-only", po::value<bool>()->default_value(false),
			"BFWS(w,#g), w = w_{#g}, no relevant fluents count" )	  
		;

	po::options_description desc_poly_search( "Polynomial Search Algorithms:",135,260 );
	
	desc_poly_search.add_options()
		( "1-BFWS", po::value<bool>()->default_value(false),
			"1-BFWS(w,#g), w_{#r,#g}, pruning w > 1 " )
		( "1-C-BFWS", po::value<bool>()->default_value(false),
			"1-BFWS using consistency to refine goal counting" )
		( "k-BFWS", po::value<bool>()->default_value(false),
			"k-BFWS(w,#g), w_{#r,#g}, pruning w > k, where k = bound() argument, default 2" )
		( "k-C-BFWS", po::value<bool>()->default_value(false),
		         "k-BFWS with goal consistency count" )
		( "k-M-BFWS", po::value<int>()->default_value(0),
			"Allowing (M) nodes > novelty bound() for each node with novelty <= bound()" )
		( "k-M-C-BFWS", po::value<int>()->default_value(0),
			"k-M-C-BFWS with goal consistency" )
		;

	po::options_description all_desc;
	all_desc.add(desc).add(desc_search).add(desc_poly_search);
	try {
		po::store( po::parse_command_line( ac, av, all_desc ), vars );	       
		po::notify( vars );
	}
	catch ( std::exception& e ) {
		std::cerr << "Error: " << e.what() << std::endl;
		std::exit(1);
	}
	catch ( ... ) {
		std::cerr << "Exception of unknown type!" << std::endl;
		std::exit(1);
	}

	if ( vars.count("help") ) {
		std::cout << all_desc << std::endl;
		std::exit(0);
	}

}

void report_no_solution( std::string reason, std::ofstream& plan_stream ) {
	plan_stream << ";; No solution found" << std::endl;
	plan_stream << ";; " << reason << std::endl;
	plan_stream.close();
}

int main( int argc, char** argv ) {

	po::variables_map vm;

	process_command_line_options( argc, argv, vm );

	
	if ( !vm.count( "domain" ) ) {
		std::cerr << "No PDDL domain was specified!" << std::endl;
		std::exit(1);
	}

	if ( !vm.count( "problem" ) ) {
		std::cerr << "No PDDL problem was specified!" << std::endl;
		std::exit(1);
	}

	std::ofstream	plan_stream;
	
	if ( !vm.count( "output" ) ) {
		std::cerr << "No output plan file specified, defaulting to 'plan.ipc'" << std::endl;
		plan_stream.open( "plan.ipc" );
	}
	else
		plan_stream.open( vm["output"].as<std::string>() );


	STRIPS_Problem	prob;
	bool ignore_costs = vm["ignore_costs"].as<bool>();

	aptk::FF_Parser::get_problem_description( vm["domain"].as<std::string>(), vm["problem"].as<std::string>(), prob, ignore_costs  );
	std::cout << "PDDL problem description loaded: " << std::endl;
	std::cout << "\tDomain: " << prob.domain_name() << std::endl;
	std::cout << "\tProblem: " << prob.problem_name() << std::endl;
	std::cout << "\t#Actions: " << prob.num_actions() << std::endl;
	std::cout << "\t#Fluents: " << prob.num_fluents() << std::endl;

	Fwd_Search_Problem	search_prob( &prob );

	prob.compute_edeletes();	

	Gen_Lms_Fwd    gen_lms( search_prob );
	Landmarks_Graph graph( prob );

	gen_lms.set_only_goals( true );
	//gen_lms.set_goal_ordering( false );
	
	gen_lms.compute_lm_graph_set_additive( graph );
	
	std::cout << "Goals found: " << graph.num_landmarks() << std::endl;
	std::cout << "Goals_Edges found: " << graph.num_landmarks_and_edges() << std::endl;

	//graph.print( std::cout );       


	bool found_plan = false;
	
	unsigned max_novelty  = vm["max_novelty"].as<int>();
		
	if(vm["BFWS-f5-landmarks"].as<bool>()){
		
		std::cout << "Starting search with BFWS-f5-landmarks..." << std::endl;
			
		k_BFWS bfs_engine( search_prob );	
		
		/**
		 * Use landmark count instead of goal count
		 */
		Gen_Lms_Fwd    gen_lms( search_prob );
		gen_lms.set_only_goals( false );
		Landmarks_Graph graph1( prob );
		gen_lms.compute_lm_graph_set_additive( graph1 );

		bfws_options( search_prob, bfs_engine, max_novelty, graph1 );
				
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		plan_stream.close();

		return 0;
	}
	else 	if(vm["BFWS-goalcount-only"].as<bool>()){
		
		std::cout << "Starting search with BFWS(w_(#G), #G)..." << std::endl;
			
		k_BFWS bfs_engine( search_prob );		      
		
		bfws_options( search_prob, bfs_engine, max_novelty, graph );
				
		//Do not use #rp
		bfs_engine.set_use_rp(false);
		
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		plan_stream.close();
		
		return 0;

	}
	else 	if(vm["BFWS-f5"].as<bool>()){
		
		std::cout << "Starting search with BFWS-f5..." << std::endl;
			
		k_BFWS bfs_engine( search_prob );	

		bfws_options( search_prob, bfs_engine, max_novelty, graph );
		
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		return 0;

	}
	else 	if(vm["BFWS-f5-initstate-relevant"].as<bool>()){
		
		std::cout << "Starting search with BFWS-f5... R computed once from s0" << std::endl;
			
		k_BFWS bfs_engine( search_prob );
		
		bfws_options( search_prob, bfs_engine, max_novelty, graph );
		
		bfs_engine.set_use_rp_from_init_only( true );
		
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		return 0;

	}
	else 	if(vm["k-BFWS"].as<bool>()){
		
		std::cout << "Starting search with k-BFWS..." << std::endl;
			
		k_BFWS bfs_engine( search_prob );	
		
		bfws_options( search_prob, bfs_engine, max_novelty, graph );
		bfs_engine.set_use_novelty_pruning( true );
		
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		plan_stream.close();
		
		return 0;
	}
	else 	if(vm["k-M-BFWS"].as<int>()){
		
		std::cout << "Starting search with k-M-BFWS..." << std::endl;     

		k_BFWS_M bfs_engine( search_prob );	

		unsigned m_value = vm["k-M-BFWS"].as<int>();

		bfws_options( search_prob, bfs_engine, max_novelty, graph );

		bfs_engine.set_use_novelty_pruning( true );
		bfs_engine.set_M( m_value );
		
		std::cout << "New M-Value: " << m_value << std::endl;
			
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
			
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		plan_stream.close();
		
		return 0;
	}
	else 	if(vm["k-M-C-BFWS"].as<int>()){
		
		std::cout << "Starting search with k-M-C-BFWS..." << std::endl;
		
		k_BFWS_Consistency_M bfs_engine( search_prob );	

		unsigned m_value = vm["k-M-C-BFWS"].as<int>();

		bfws_options( search_prob, bfs_engine, max_novelty, graph );
		
		bfs_engine.set_use_novelty_pruning( true );
		bfs_engine.set_M( m_value );
	
		std::cout << "New M-Value: " << m_value << std::endl;
			
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
			
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;
		
		plan_stream.close();
		
		return 0;
	}
	else 	if(vm["k-C-BFWS"].as<bool>()){
		
		std::cout << "Starting search with k-C-BFWS..." << std::endl;
			
		k_BFWS_Consistency bfs_engine( search_prob );	

		bfws_options( search_prob, bfs_engine, max_novelty, graph );

		bfs_engine.set_use_novelty_pruning( true );	       

		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		plan_stream.close();
		
		return 0;
	}else 	if(vm["1-C-BFWS"].as<bool>()){
		
		std::cout << "Starting search with 1-C-BFWS..." << std::endl;
			
		k_BFWS_Consistency bfs_engine( search_prob );	

		max_novelty = 1;

		bfws_options( search_prob, bfs_engine, max_novelty, graph );

		bfs_engine.set_use_novelty_pruning( true );
		
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		plan_stream.close();
		
		return 0;
	}
	else if(vm["1-BFWS"].as<bool>()){
	        std::cout << "Starting search with 1-BFWS..." << std::endl;
	
		k_BFWS bfs_engine( search_prob );	

		max_novelty = 1;

		bfws_options( search_prob, bfs_engine, max_novelty, graph );

		bfs_engine.set_use_novelty_pruning( true );
		
		float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
		std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;

		plan_stream.close();
		
		return 0;
        }
 

	


    //Fast First Consistency
    if( vm["DUAL-C-BFWS"].as<bool>() and !found_plan)	{
	    std::cout << "Starting search with 1-C-BFWS..." << std::endl;
	
	    k_BFWS_Consistency bfs_engine( search_prob );	

	    max_novelty = 1;
	    
	    bfws_options( search_prob, bfs_engine, max_novelty, graph );

	    bfs_engine.set_use_novelty_pruning( true );
	    	
	    float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
		
	    std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;
    }
    //Fast First (FOR AAAI-17)
    if( vm["DUAL-BFWS"].as<bool>() and !found_plan)	{
            std::cout << "Starting search with 1-BFWS..." << std::endl;
	
	    k_BFWS bfs_engine( search_prob );	
	    
	    max_novelty = 1;

	    bfws_options( search_prob, bfs_engine, max_novelty, graph );

	    bfs_engine.set_use_novelty_pruning( true );
	    
	    float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
	    
	    std::cout << "Fast-BFS search completed in " << bfs_t << " secs" << std::endl;
    }
	
	
    if(!found_plan && (vm["DUAL-BFWS"].as<bool>() or vm["DUAL-C-BFWS"].as<bool>() ) ){
	    std::cout << "Starting search with BFWS(novel,land,h_ff)..." << std::endl;

	    BFWS_w_hlm_hadd bfs_engine( search_prob );	
	    bfs_engine.h4().ignore_rp_h_value(true);

	    /**
	     * Use landmark count instead of goal count
	     */
	    Gen_Lms_Fwd    gen_lms( search_prob );
	    gen_lms.set_only_goals( false );	   
	    Landmarks_Graph graph1( prob );
	    gen_lms.compute_lm_graph_set_additive( graph1 );
	 
	    Land_Graph_Man lgm( search_prob, &graph1);
	    bfs_engine.use_land_graph_manager( &lgm );

	    std::cout << "Landmarks found: " << graph1.num_landmarks() << std::endl;
	    std::cout << "Landmarks_Edges found: " << graph1.num_landmarks_and_edges() << std::endl;

	    max_novelty = vm["max_novelty"].as<int>(); 
			
	    bfs_engine.set_arity( max_novelty, graph1.num_landmarks_and_edges() );

	    //NIR: Approximate the domain of #r counter, so we can initialize the novelty table, making sure we've got
	    //     space for novelty > 1 tuples 
	    H_Add_Rp_Fwd hadd( search_prob );
	    float h_init=0;
	    const aptk::State* s_0 = search_prob.init();
	    hadd.eval( *s_0, h_init );
	    
	    bfs_engine.set_arity_2( max_novelty,  h_init );

	    found_plan = false;
	    float bfs_t = do_search( bfs_engine, prob, plan_stream, found_plan );
			
	    std::cout << "BFS search completed in " << bfs_t << " secs" << std::endl;	
	
    }	
	
	plan_stream.close();

	return 0;
}
