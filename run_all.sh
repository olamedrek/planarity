#!/bin/bash

for filename in ./test/*.in; do
    python run.py -exe $1 -input $filename
done