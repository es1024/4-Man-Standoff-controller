#!/bin/bash
printf -v CLASSPATH `pwd`'/%s:' players/*
export CLASSPATH="$CLASSPATH."
time ./standoff.out 2>x
