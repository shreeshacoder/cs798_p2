
PROTO_PATH = ../proto/

all:
	g++ `pkg-config --cflags --libs fuse3` -c wrapper.cc -o wrapper.o
	g++ -Wall  `pkg-config --cflags --libs fuse3` -c client.cc -o client.o
	g++ -std=c++11 `pkg-config --cflags protobuf grpc` -c -o nfsClient.o nfsClient.cc

	g++ $(PROTO_PATH)schema.pb.o $(PROTO_PATH)schema.grpc.pb.o client.o wrapper.o nfsClient.o -L/usr/local/lib `pkg-config --libs --cflags fuse3 protobuf grpc++` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o client	
	




clean: 
	rm wrapper.o client client.o nfsClient.o