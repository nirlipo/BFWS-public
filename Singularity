Bootstrap: docker
From: ubuntu:focal

%post
    export DEBIAN_FRONTEND=noninteractive
    ## Install all necessary dependencies.
    apt-get update
    apt-get install --no-install-recommends -y \
    build-essential \
    ca-certificates \
    xutils-dev \
    scons \
    gcc-multilib \
    flex \
    bison \
    libboost-dev \
    libjudy-dev \
    libboost-python-dev \
    libboost-program-options-dev \
    g++-multilib \
    g++ \
    git

    rm -rf /var/lib/apt/lists/*

    git clone https://github.com/nirlipo/BFWS-public.git /planner/BFWS
    git clone https://github.com/LAPKT-dev/LAPKT-public.git /planner/LAPKT-public

    ## Set up environment variables
    ##
    ## Uncomment if you want to get latest LAPKT version
    ## export LAPKT_URL=https://github.com/LAPKT-dev/LAPKT-public/archive/master.tar.gz \

    export LAPKT_PATH=/planner/LAPKT-public

    ## Download LAPKT-public
    ## curl -SL $LAPKT_URL | tar -xz
    ## mv LAPKT-* $LAPKT_PATH
    cd /planner
    #tar xjf LAPKT-public.tar.bz2
    cd $LAPKT_PATH
    cd external/libff
    make clean
    make depend
    make


    ## Build your planner
    cd /planner/BFWS/ff-version
    scons -j4
    cd /planner/BFWS/fd-version
    python3 build.py

    #clean object files
    rm -rf /planner/*.o

    apt-get purge -y  \
    build-essential \
    ca-certificates \
    xutils-dev \
    scons \
    gcc-multilib \
    flex \
    bison \
    libboost-dev \
    libjudy-dev \
    libboost-python-dev \
    libboost-program-options-dev \
    g++-multilib \
    g++ \
    git

    unset DEBIAN_FRONTEND
%runscript
    ## The runscript is called whenever the container is used to solve
    ## an instance.

    # PLANNER=$1
    # DOMAINFILE=$2
    # PROBLEMFILE=$3
    # PLANFILE=$4

    ## Call your planner using FD-parser.
    #/planner/BFWS/fd-version/bfws_anytime_fd_singularity.py $DOMAINFILE $PROBLEMFILE $PLANFILE
    #$PLANNER $DOMAINFILE $PROBLEMFILE $PLANFILE

    ## Use arguments, e.g. from planutils manifest.json
    $@

## Update the following fields with meta data about your submission.
## Please use the same field names and use only one line for each value.
%labels
Name        LAPKT-DUAL-BFWS
Description DUAL-BFWS implemented with LAPKT
Authors     Nir Lipovetzky <nir.lipovetzky@unimelb.edu.au> and Miquel Ramirez <miguel.ramirez@unimelb.edu.au> and Guillem Frances <guillem.frances@unibas.ch> and Hector Geffner <hector.geffner@upf.edu>
SupportsDerivedPredicates no
SupportsQuantifiedPreconditions yes
SupportsQuantifiedEffects yes
