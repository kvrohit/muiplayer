#!/bin/bash

echo -e "--- Building MUI ---\n"

echo -e "--- Building cfmod library ---\n"
cd src/cfmod
make linux_lib
if [ $? -ne 0 ]
then
    echo -e "--- Error building cfmod ---\n"
    exit
fi
echo -e "--- cfmod library built ---\n"

echo -e "--- Building cplaylist library ---\n"
cd ../cplaylist
make linux_lib

if[ $? -ne 0]
then
	echo -e "--- Error building cplaylist ---\n"
	exit
fi
echo -e "--- cplaylist library built ---\n"

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
