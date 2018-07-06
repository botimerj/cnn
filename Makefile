
all: 

#beamform: beamform.c
#	icc -o beamform beamform.c -pthread -vec-report3

cnn: cnn.h mnist.cpp conv.cpp cnn.cpp
	g++ cnn.h mnist.cpp conv.cpp cnn.cpp -o cnn

cnn_load: cnn.h load_network.cpp conv.cpp cnn.cpp
	g++ cnn.h load_network.cpp conv.cpp cnn.cpp -o cnn_load

mnist: mnist.cpp 
	g++ mnist.cpp -o mnist 

conv_tb: conv.cpp tb/conv_tb.cpp
	g++ -g cnn.h conv.cpp tb/conv_tb.cpp -o tb/conv_tb 

clean:
	rm -f cnn mnist
