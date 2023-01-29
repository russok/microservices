#
# C compiler and options
#
BOOST=~/boost

ASIO_LIBS=	-lboost_coroutine  -lboost_thread -lboost_system -lboost_context -lpthread

# debug
#CC=	g++ -std=c++14 -g -Wall -I . -I $(BOOST)/include -L $(BOOST)/lib -DBOOST_ALL_NO_LIB=1

CC=	g++ -std=c++14 -O3 -Wall -I . -I $(BOOST)/include -L $(BOOST)/lib -DBOOST_ALL_NO_LIB=1


.cpp.o:
	$(CC) -c $< -o $@

#
# Compile
#


boost-beast: server.o $(OBJ)
	$(CC) -o $@ $^ $(ASIO_LIBS)

run: boost-beast
	LD_LIBRARY_PATH=$(BOOST)/lib ./$^ 0.0.0.0 3030 1
#     PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
#   58183 rsok      20   0  994744  62164   9136 S 231.9   0.4   2:46.35 target/release/load_test --no-reset-metrics --report-file=report.html
#   58045 rsok      20   0    8272   4668   4180 R  99.7   0.0   1:11.86 ./boost-beast 0.0.0.0 3030

runfast: boost-beast
	LD_LIBRARY_PATH=$(BOOST)/lib ./$^ 0.0.0.0 3030 3
#     PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
#    9817 rsok      20   0  994768  57836   9012 S 387.7   0.4   2:42.75 target/release/load_test --no-reset-metrics --report-file=report.html
#    9752 rsok      20   0   81504   4360   4072 R 200.0   0.0   1:27.11 ./boost-beast 0.0.0.0 3030 2
#   10122 rsok      20   0  155228   4404   4120 R 295.7   0.0   3:52.72 ./boost-beast 0.0.0.0 3030 3

#
# Clean out object files and the executable.
#
clean:
	rm *.o

