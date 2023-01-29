#!/bin/bash
java -XX:ActiveProcessorCount=1 -jar target/java-microservice-1.0-SNAPSHOT-jar-with-dependencies.jar
#java -jar target/java-microservice-1.0-SNAPSHOT-jar-with-dependencies.jar

#    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
#  45129 rsok      20   0  994744  70576   8872 S 324.8   0.4   4:39.69 target/release/load_test --no-reset-metrics --report-file=report.html
#  44581 rsok      20   0 6450576 134620  27484 S 100.3   0.8   1:25.07 java -XX:ActiveProcessorCount=1 -jar target/java-microservice-1.0-SNAPSHOT-jar-with-dependencies.jar
#  16950 rsok      20   0 8438220 270052  28140 S 233.6   1.7   1:59.87 java -jar target/java-microservice-1.0-SNAPSHOT-jar-with-dependencies.jar
