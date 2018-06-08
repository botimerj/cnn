
all: 

#beamform: beamform.c
#	icc -o beamform beamform.c -pthread -vec-report3

cnn: conv.cpp cnn.cpp
	g++ cnn.h conv.cpp cnn.cpp -o cnn

conv_tb: conv.cpp tb/conv_tb.cpp
	g++ -g cnn.h conv.cpp tb/conv_tb.cpp -o tb/conv_tb 

clean:
	rm -f cnn
