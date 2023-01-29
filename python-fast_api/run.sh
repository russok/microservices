#!/bin/bash
. .venv/bin/activate
uvicorn demo_server:app --log-level error --port 3030

#     PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
#   61847 rsok      20   0  150040  37876  16112 R 100.0   0.2   0:41.75 .venv/bin/python3.8 .venv/bin/uvicorn demo_server:app --log-level critical --port 3030
#   61906 rsok      20   0  972368  24980   9132 S  58.8   0.2   0:23.59 target/release/load_test --no-reset-metrics --report-file=report.html
#   19064 rsok      20   0  152896  41496  17348 R 100.0   0.3   1:41.54 .venv/bin/python3.11 .venv/bin/uvicorn demo_server:app --log-level error --port 3030
