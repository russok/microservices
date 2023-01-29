#!/bin/bash

dotnet run -c release

#     PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
#   47631 rsok      20   0  263.1g 253468  64360 S 327.9   1.6   6:44.65 /home/rsok/rust/asp.net/bin/release/net7.0/asp.net
#   48251 rsok      20   0  981980  42112   8908 S 230.9   0.3   0:44.85 target/release/load_test --no-reset-metrics --report-file=report.html