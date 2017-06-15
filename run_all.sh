#!/bin/bash

for in_path in ./test/*.in; do
    OUT=$(python run.py -exe $1 -input $in_path)
    exp_path=${in_path//.in/.exp}
    EXP=$(cat < ${exp_path})
    if [ "$OUT" == "$EXP" ]
    then
        echo $in_path OK
    else
        echo $in_path WA
    fi
done