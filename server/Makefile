
PROTO_PATH = ../proto/

all :
	g++ -std=c++11 `pkg-config --cflags protobuf grpc` -c -o nfsServer.o nfsServer.cc
	g++ -std=c++11 `pkg-config --cflags protobuf grpc` -c -o server.o server.cc
	g++ $(PROTO_PATH)schema.pb.o $(PROTO_PATH)schema.grpc.pb.o nfsServer.o server.o -L/usr/local/lib `pkg-config --libs protobuf grpc++` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o server

clean:
	rm nfsServer.o server.o server