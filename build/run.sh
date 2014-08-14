#!/bin/bash
# java classpath
printf -v CLASSPATH `pwd`'/%s:' players/*
export CLASSPATH="$CLASSPATH."
time ./standoff.out $1 $2 1>"stdout-$1-$2.log" 2>"stderr-$1-$2.log"
