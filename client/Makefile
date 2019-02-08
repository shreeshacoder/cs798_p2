
PROTO_PATH = ../proto/

all:
	g++ `pkg-config --cflags --libs fuse3` -c wrapper.cc -o wrapper.o
	g++ -Wall client.cc `pkg-config --cflags --libs fuse3` wrapper.o -o client


	# g++ -std=c++11 `pkg-config --cflags protobuf grpc` -c -o nfsClient.o nfsClient.cc
	# g++ $(PROTO_PATH)schema.pb.o $(PROTO_PATH)schema.grpc.pb.o nfsClient.o -L/usr/local/lib `pkg-config --libs protobuf grpc++` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o nfsClient	


clean: 
	rm wrapper.o
	rm client