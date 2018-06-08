
all: 

#beamform: beamform.c
#	icc -o beamform beamform.c -pthread -vec-report3

cnn: conv.cpp cnn.cpp
	g++ conv.cpp cnn.cpp -o cnn

clean:
	rm -f cnn
