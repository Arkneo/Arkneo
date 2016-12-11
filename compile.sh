#!/bin/bash
# File to use CMake in an easier fashion.

BUILD_FOLDER=build
BUILD_TEST_RULE=BUILD_TESTS

# Testing CMake
echo -en '\033[36mTesting cmake... \033[0m'
cmake > /dev/null 2>/tmp/testing_cmake
TEST=$(</tmp/testing_cmake)
echo -e '\033[32mDone !\033[0m'
if [ -n "$TEST" ]
then
    echo -e '\n\033[31mError with cmake :\033[0m'
    echo -e '\033[31m=========================\033[0m'
    echo "$TEST"
    echo -e '\033[31m=========================\033[0m'
    echo -e '\033[31mPlease, install CMake and try again. Exiting with error...\033[0m'
    exit 1
else
    echo -e '\033[32mCMake was successfully found !\033[0m'
fi

cd $(dirname "$0")
# Testing build folder
echo -en "\033[36mLooking for build folder ($BUILD_FOLDER)... \033[0m"
if [ -d "$BUILD_FOLDER" ]
then
    echo -e '\033[32mFound !\033[0m'
    echo -en '\033[36mCleaning build folder... \033[0m'
    rm -rf $BUILD_FOLDER/*
    echo -e '\033[32mDone !\033[0m'
else
    echo -e '\033[33mNot found !\033[0m'
    echo -en '\033[36mCreating build folder... \033[0m'
    mkdir $BUILD_FOLDER
    echo -e '\033[32mDone !\033[0m'
fi

echo "--------------------------------------------------------------------------------"
echo "--------------------------------------------------------------------------------"
# Testing if args
if [ "$#" -gt 0 ]
then
    # Command test
    if [ "$1" = "test" ]
    then
    	echo -en "\033[36mRunning [cmake -H. -B$BUILD_FOLDER -D$BUILD_TEST_RULE:BOOL=true -G "Unix Makefiles"]... \033[0m"
    	cmake -H. -B$BUILD_FOLDER -D$BUILD_TEST_RULE:BOOL=true -G "Unix Makefiles"
    	if [ "$?" != "0" ]
    	then
    	    echo -e '\033[31mErrors !\033[0m'
    	else
    	    echo -e '\033[32mDone !\033[0m'
    	    make -C $BUILD_FOLDER
    	    if [ "$?" = "0" ]
    	    then
    		  make test -C $BUILD_FOLDER
    	    fi
    	fi
    elif [ "$1" = "usage" ]
    then
       echo
	   echo "$(basename "$0") [usage|test|clean]"
       echo
       echo 'usage      Displays this message'
       echo 'test       Builds tests and run them'
       echo 'clean      Removes the build folder'
       echo
    elif [ "$1" = "clean" ]
    then
    	echo -en '\033[36mRemoving build folder...\033[0m'
    	rm -rf $BUILD_FOLDER
    	if [ "$?" != "0" ]
    	then
    	    echo -e '\033[31mErrors !\033[0m'
    	else
    	    echo -e '\033[32mDone !\033[0m'
    	fi
    else
	   echo -e '\033[31mBad argument\033[0m'
    fi
else
    echo -en "\033[36mRunning [cmake -H. -B$BUILD_FOLDER -G "Unix Makefiles"]... \033[0m"
    cmake -H. -B$BUILD_FOLDER -G "Unix Makefiles"
    if [ "$?" != "0" ]
    then
	   echo -e '\033[31mErrors !\033[0m'
    else
    	echo -e '\033[32mDone !\033[0m'
    	make -C $BUILD_FOLDER
    fi
fi
cd -
