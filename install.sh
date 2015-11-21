#!/bin/bash

# MUST be the same as in the Makefile
INSTALL_DIR=/opt/FEniCS

REPO=https://bitbucket.org/fenics-project/dolfin.git
SRC_DIR=dolfin-whale
WHALE_PATCH=whale_backend-1.6.0.patch
DOLFIN_BRANCH=dolfin-1.6.0
WHALE_BRANCH=$DOLFIN_BRANCH-whale

function detect_dolfin_repo {
    grep -q "project(DOLFIN)" $SRC_DIR/CMakeLists.txt
    if [ $? -ne 0 ]; then
        echo "Cannot detect FEniCS/DOLFIN repository"
        return 1 # error
    fi
    return 0 # success
}

function apply_patch {
    cd $SRC_DIR

    git checkout -q $DOLFIN_BRANCH
    # create a new branch to hold the patch, remove if already exists
    git branch -D $WHALE_BRANCH
    git checkout -b $WHALE_BRANCH
    git apply ../$WHALE_PATCH

    # include new files to the next commit
    git add ./dolfin
    # include modified files to the next commit
    git add ./CMakeLists.txt ./dolfin

    git commit -a -q -m"add Whale"

    cd -
}

function install_whale_lib {
    echo "Install Whale synamic library to $INSTALL_DIR ... "
    make clean all install
}

function install_patched_dolfin {
    echo "Configure FEniCS/DOLFIN with Whale support"
    cd $SRC_DIR
    mkdir -p ./build
    cd ./build
    rm -f CMakeCache.txt
    cmake -DCMAKE_SKIP_RPATH:BOOL=ON -D PYTHON_EXECUTABLE:FILEPATH=/usr/bin/python -D CMAKE_INSTALL_PREFIX:PATH=$INSTALL_DIR ../

    cd ..
    echo "Build FEniCS/DOLFIN with Whale support"
    make -C ./build

    echo "Install FEniCS/DOLFIN with Whale support"
    make -C ./build install

    cd -
}

###    Helper functions from dorsal

cecho() {
    # Display messages in a specified colour
    COL=$1; shift
    echo -e "${COL}$@\033[0m"
}

guess_architecture() {
  # Try to guess the architecture of the platform we are running on
    ARCH=unknown
    if [ -x /usr/bin/uname -o -x /bin/uname ]
    then
	ARCH=`uname -m`
    fi
}

default () {
    # Export a variable, if it is not already set
    VAR="${1%%=*}"
    VALUE="${1#*=}"
    eval "[[ \$$VAR ]] || export $VAR='$VALUE'"
}

# If the PYTHON_EXECUTABLE environment variable hasn't been set,
# set it to the default python executable
default PYTHON_EXECUTABLE=$(which python)

# If the platform doesn't override the system python by installing its
# own, figure out the version of of the existing python
default PYTHONVER=`${PYTHON_EXECUTABLE} -c "import sys; print sys.version[:3]"`

generate_fenics_conf() {
    # Generate configuration file for building against FEniCS installation

    mkdir -p $INSTALL_PATH/share/fenics
    CONFIG_FILE=$INSTALL_PATH/share/fenics/fenics.conf
    rm -f $CONFIG_FILE
    echo "
# Source this file to set up your environment for building against FEniCS

# Standard paths
export INSTALL_PATH=${INSTALL_PATH}
export PATH=\$INSTALL_PATH/bin:\$PATH
export PYTHONPATH=\$INSTALL_PATH/lib/python${PYTHONVER}/site-packages:\$PYTHONPATH
export LD_LIBRARY_PATH=\$INSTALL_PATH/lib:\$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=\$INSTALL_PATH/lib:\$DYLD_LIBRARY_PATH
export PKG_CONFIG_PATH=\$INSTALL_PATH/lib/pkgconfig:\$PKG_CONFIG_PATH
export MANPATH=\$INSTALL_PATH/share/man:\$MANPATH
export CPLUS_INCLUDE_PATH=\$INSTALL_PATH/include:\$CPLUS_INCLUDE_PATH
export CMAKE_PREFIX_PATH=\$INSTALL_PATH:\$CMAKE_PREFIX_PATH
" >> $CONFIG_FILE

    # Add some extra library paths for 64 bit machines
    guess_architecture
    if [ "$ARCH" == "x86_64" ]; then
        echo "
# 64 bit paths
export PYTHONPATH=\$INSTALL_PATH/lib64/python${PYTHONVER}/site-packages:\$PYTHONPATH
export LD_LIBRARY_PATH=\$INSTALL_PATH/lib64:\$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=\$INSTALL_PATH/lib64:\$DYLD_LIBRARY_PATH
" >> $CONFIG_FILE
    fi

#    for PACKAGE in ${PACKAGES[@]}
#    do
#	case ${PACKAGE} in
#            *boost) echo "
#export BOOST_DIR=\$INSTALL_PATH
#" >> $CONFIG_FILE;;
#	    *vtk | *vtkwithqt) echo "
# FIXME: This needs to be updated when the VTK version is changed
#export LD_LIBRARY_PATH=\$INSTALL_PATH/lib/vtk-5.8:\$LD_LIBRARY_PATH
#export DYLD_LIBRARY_PATH=\$INSTALL_PATH/lib/vtk-5.8:\$DYLD_LIBRARY_PATH
#" >> $CONFIG_FILE;;
#	esac
#    done

    echo
    echo "FEniCS has now been installed in"
    echo
    cecho ${GOOD} "    ${INSTALL_PATH}"
    echo
    echo "To update your environment variables, run the following command:"
    echo
    cecho ${GOOD} "    source $CONFIG_FILE"
    echo
    echo "For future reference, we recommend that you add this command to your"
    echo "configuration (.bashrc, .profile or similar)."
    echo

}

################################################################################

if [ -d $SRC_DIR ]; then
    echo "Directory $SRC_DIR already exists"
else
    echo "Create $SRC_DIR"
    mkdir $SRC_DIR
fi

if [ -z "$(ls -A $SRC_DIR)" ]; then
    echo "Clone FEniCS/DOLFIN repository into $SRC_DIR ... "
    git clone $REPO $SRC_DIR
elif [ -d $SRC_DIR/.git ]; then
    if ! detect_dolfin_repo ; then
        exit
    fi
    echo "Use local copy of FEniCS/DOLFIN"
else
    echo "Directory $SRC_DIR is not empty and does not contain FEniCS/DOLFIN repository  -  exit"
    exit
fi

apply_patch
install_whale_lib
install_patched_dolfin
generate_fenics_conf

echo "Installation Complete"
