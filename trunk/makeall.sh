#!/bin/bash

echo -e "--- building project MUI ---\n"

echo -e "--- building cfmod library ---\n"
cd src/cfmod
make -f makefile.linux
if [ $? -ne 0 ]
then
    echo -e "--- error building cfmod ---\n"
    exit
fi
echo -e "--- cfmod library built ---\n"

echo -e "--- building MUI ---\n"
cd ../MUI
qmake
make
if [ $? -ne 0 ]
then
    echo -e "--- error building MUI ---\n"
    exit
fi
echo -e "--- MUI built ---\n"
