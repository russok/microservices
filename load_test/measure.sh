#!/bin/bash

#for example, 
# bash ./measure.sh --report-file=report-asp.net.html

cargo run --release --  --no-reset-metrics "$@"
