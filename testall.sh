#!/usr/bin/env bash
for file in $(find -name "*.bf"); do
    syntax="$(cat $file)"
    ./brainfuck "$syntax"
done
