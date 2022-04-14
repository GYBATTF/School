#! /bin/bash

## https://stackoverflow.com/a/31605674
export GOPATH="$(cd "$(dirname "$1")"; pwd)/$(basename "$1")"
PACKAGE="legv8emul"
LOG="build.log"

if [ -f $PACKAGE ]
then
    echo "Removing previous build..."
    rm $PACKAGE
    if [ -f $LOG ] 
    then
        rm $LOG
    fi
fi

echo "Building..."
go install $PACKAGE 2>&1 | tee $LOG

if [ -f "bin/${PACKAGE}" ]
then
    mv bin/$PACKAGE .
    FINISH_MSG="Built $PACKAGE successfully!"
else
    FINISH_MSG="Failed to build ${PACKAGE}, errors located in ${LOG}."
    LOG=""
fi

echo "Cleaning up..."
rm -rf bin pkg $LOG
echo $FINISH_MSG
