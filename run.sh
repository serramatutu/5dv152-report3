#!/bin/bash

TARGET=$1

if [ "$TARGET" = "" ]; then
    echo "Must specify a target as first argument."
    exit 1
fi

# compile target
make $TARGET

# list all programs produced by target
PROGRAMS=$(ls bin/$TARGET)
# comma separated list of programs
PROGRAM_COMMAS=$(echo "$PROGRAMS" | paste -sd ",")

# create results file and add CSV header to it
mkdir -p -- results
RESULTS_FILE="results/$TARGET.csv"
rm $RESULTS_FILE
touch $RESULTS_FILE
echo "input_file,repetition,$PROGRAM_COMMAS" > $RESULTS_FILE

# run experiments with all input files
REPETITIONS=3

export OMP_NUM_THREADS=4

for INPUT_FILE in `ls -v in/$TARGET`; do
    for REP in `eval echo {1..$REPETITIONS..1}`; do
        RESULTS=""

        for PROGRAM in $PROGRAMS; do
            PROGRAM_RESULT=$(./bin/$TARGET/$PROGRAM < ./in/$TARGET/$INPUT_FILE | tail -1)
            RESULTS="$RESULTS,$PROGRAM_RESULT"
        done

        echo "$INPUT_FILE,$REP$RESULTS" >> $RESULTS_FILE
    done
    echo "Finished running for file 'in/$TARGET/$INPUT_FILE'."
done


# create plots
mkdir -p -- plots
python3 plot.py $RESULTS_FILE plots/$TARGET

echo "Done."
