#!/bin/bash

# Format all hpp and cpp files within the project with clang-format

HPPFILES=$(find . -type f -name "*.hpp")
CPPFILES=$(find . -type f -name "*.cpp")

for FILE in $HPPFILES; do
    STATUS="$( git status --short $FILE | tr -d ' ' )"
    if [ $STATUS ]
    then
	echo "clang-format -i ${FILE}"
	clang-format -i $FILE
    fi
done

for FILE in $CPPFILES; do
    STATUS="$( git status --short $FILE | tr -d ' ' )"
    if [ $STATUS ]
    then
	echo "clang-format -i ${FILE}"
	clang-format -i $FILE
    fi
done
