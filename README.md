# BFWS
Best First Width Search Planner

### Getting Started ###

You first need to install LAPKT by following this instructions: https://lapkt-dev.github.io/docs/gettingStarted/

Yoy also need to define LAPKT_PATH as an enviromnent variable. Add the following line to your .bash or .profile file or simply execute it in your terminal:
```
  export LAPKT_PATH = /Absolute-path-to-LAPKT-folder
```
BFWS can run using either FF or FD parser.

### FF-parser version ###

The Scons script included in FF-parser-version folder knows which modules from the LAPKT toolkit it needs to recompile.

To compile type
```
  scons 
```  
### FD-parser version ###

Go to FD-parser-version folder and type to compile
```
  ./build.py 
```
# Running BFWS #

These are BFWS options
```
  ./bfws --help

Options::
  --help                                Show help message. 
  --domain arg                          Input PDDL domain description
  --problem arg                         Input PDDL problem description
  --output arg                          Output file for plan
  --max_novelty arg (=2)                Max width w for novelty (default 2)

Search Algorithms::
  --DUAL-BFWS arg (=1)                  1-BFWS first, then BFWS using h_ff and h_landcount as in AAAI-17 paper
  --DUAL-C-BFWS arg (=0)                1-C-BFWS first, then BFWS using h_ff and h_landcount
  --BFWS-f5 arg (=0)                    BFWS(w,#g), w_{#r,#g}, as in BFWS(f5) AAAI-17 paper
  --BFWS-f5-initstate-relevant arg (=0) BFWS(f5) but computing relevant fluents only once from s0
  --BFWS-f5-landmarks arg (=0)          BFWS(w,h_landcount), w = w_{#r,h_landcount} 
  --BFWS-goalcount-only arg (=0)        BFWS(w,#g), w = w_{#g}, no relevant fluents count

Polynomial Search Algorithms::
  --1-BFWS arg (=0)                     1-BFWS(w,#g), w_{#r,#g}, pruning w > 1 
  --1-C-BFWS arg (=0)                   1-BFWS using consistency to refine goal counting
  --k-BFWS arg (=0)                     k-BFWS(w,#g), w_{#r,#g}, pruning w > k, where k = bound() argument, default 2
  --k-C-BFWS arg (=0)                   k-BFWS with goal consistency count
  --k-M-BFWS arg (=0)                   Allowing (M) nodes > novelty bound() for each node with novelty <= bound()
  --k-M-C-BFWS arg (=0)                 k-M-C-BFWS with goal consistency

```
The command to run the FF-parser-version of BFWS, computing novelty 1,2, and greater than 2, and pruning nodes with novelty greater than 2
```
  ./bfws --domain domain.pddl --problem prob.pddl --max_novelty 2 --k-BFWS true
```
to use FD-parser version, go to the correct folder and run the same options with the following command
```
  ./bfws.py domain.pddl prob.pddl k-BFWS
```
FD-version uses the same options but do not uses tags. To change the default max_novelty 2 and M values, edit bfws.py file

# Credits #

This project is a joint work by Nir Lipovetzky, and Hector Geffner.

### Paper ###
You can read more about it in the [AAAI 2017 paper](http://people.eng.unimelb.edu.au/nlipovetzky/papers/aaai17-BFWS-novelty-exploration.pdf) and [ICAPS 2017 paper](http://people.eng.unimelb.edu.au/nlipovetzky/papers/icaps17-polytime-BFWS.pdf)
  
