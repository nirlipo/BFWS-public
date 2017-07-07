#!/usr/bin/python
import os
import sys

LAPKT_ROOT = os.environ.get("LAPKT_PATH")

def main() :

        if LAPKT_ROOT is None:
                sys.exit("ERROR: define Environmental Variable!\n export LAPKT_PATH = \"LAPKT_root_folder_location\"")

	# 1. Copy FD python parsing files
	os.system( 'rm -rf fd' )
	rv = os.system( 'cp -Ra %(LAPKT_ROOT)s/external/fd .'%globals() )
	if rv != 0 :
		print >> sys.stderr, "Could not copy FD parser files!"
		sys.exit(1)

	# 2. Call scons to build
        if len(sys.argv) > 1:
                rv = os.system( "scons %s" % sys.argv[1] )
        else:
	        rv = os.system( 'scons -j 8' )
	if rv != 0 :
		print >> sys.stderr, "Build failed!"
		sys.exit(1)

if __name__ == '__main__' :
	main()
