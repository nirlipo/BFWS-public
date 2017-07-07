import os

debug = ARGUMENTS.get( 'debug', 0 )
common_env = Environment(ENV = os.environ)

platform = ARGUMENTS.get('OS', Platform())

if platform.name == 'darwin':
        lwaptk_base_dir = '/Users/nirlipo/Sandboxes/LAPKT-public'
else:
	lwaptk_base_dir = '/home/nirlipo/Sandboxes/LAPKT-public'
	#lwaptk_base_dir = '/media/sf_Sandboxes/LAPKT-public'


common_env.Append( CCFLAGS = [ '-Wall', '-std=c++11' ] )

if int(debug) == 1 :
        common_env.Append( CCFLAGS = ['-g',  '-DDEBUG' ] )
elif int(debug) == 2 :
        common_env.Append( CCFLAGS = ['-g',  '-DNDEBUG' ] )
else:
        common_env.Append( CCFLAGS = ['-O3', '-DNDEBUG'] )

common_env.Append( LINKFLAGS = [ '-static' ] )

Export( 'common_env' )

generic_objs = SConscript(os.path.join( lwaptk_base_dir,'src/SConscript.aptk'))
agnostic_objs = SConscript(os.path.join( lwaptk_base_dir,'interfaces/agnostic/SConscript.agnostic'))
ff_wrapper_objs = SConscript(os.path.join( lwaptk_base_dir,'interfaces/ff-wrapped/SConscript.ff'))
siw_objs = SConscript('src/SConscript')

common_env.Program( 'bfs_f_exp', siw_objs + generic_objs + agnostic_objs + ff_wrapper_objs ) 
