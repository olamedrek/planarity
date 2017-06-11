#!/bin/bash

for filename in ./test/*.in; do
    python main.py -path $filename
done