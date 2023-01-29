#!/bin/bash

cargo run --release

#     PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
#   56977 rsok      20   0  994768  57788   8836 S 455.6   0.4   4:51.09 target/release/load_test --no-reset-metrics --report-file=report.html
#   56067 rsok      20   0  817708   3244   2940 S 155.0   0.0   1:39.72 target/release/warp_server