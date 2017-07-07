#include <bfws.hxx>
using namespace boost::python;

BOOST_PYTHON_MODULE( libbfws )
{
    class_<BFWS>("BFWS")
	.def( init< std::string, std::string >() )
	.def( "add_atom", &BFWS::add_atom )
	.def( "add_action", &BFWS::add_action )
	.def( "add_mutex_group", &BFWS::add_mutex_group )
	.def( "num_atoms", &BFWS::n_atoms )
	.def( "num_actions", &BFWS::n_actions )
	.def( "get_atom_name", &BFWS::get_atom_name )
	.def( "get_domain_name", &BFWS::get_domain_name )
	.def( "get_problem_name", &BFWS::get_problem_name )
	.def( "add_precondition", &BFWS::add_precondition )
	.def( "add_effect", &BFWS::add_effect )
	.def( "add_cond_effect", &BFWS::add_cond_effect )
	.def( "set_cost", &BFWS::set_cost )
	.def( "notify_negated_conditions", &BFWS::notify_negated_conditions )
	.def( "create_negated_fluents", &BFWS::create_negated_fluents )
	.def( "set_init", &BFWS::set_init )
	.def( "set_goal", &BFWS::set_goal )
	.def( "set_domain_name", &BFWS::set_domain_name )
	.def( "set_problem_name", &BFWS::set_problem_name )
	.def( "write_ground_pddl", &BFWS::write_ground_pddl )
	.def( "print_action", &BFWS::print_action )
	.def( "setup", &BFWS::setup )
	.def( "solve", &BFWS::solve )
	.def_readwrite( "parsing_time", &BFWS::m_parsing_time )
	.def_readwrite( "ignore_action_costs", &BFWS::m_ignore_action_costs )
	.def_readwrite( "log_filename", &BFWS::m_log_filename )
	.def_readwrite( "search", &BFWS::m_search_alg )
        .def_readwrite( "m_value", &BFWS::m_M )
	.def_readwrite( "max_novelty", &BFWS::m_max_novelty )
	;
}

