
svr :
	g++ -std=c++11 `pkg-config --cflags protobuf grpc` -c -o nfsServer.o nfsServer.cc
	g++ proto/schema.pb.o proto/schema.grpc.pb.o nfsServer.o -L/usr/local/lib `pkg-config --libs protobuf grpc++` -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed -ldl -o nfsServer

schema :
	cd proto/ && make 


all :
	g++ server.cc -o server "-lstdc++fs" 

clean :
	rm schema.grpc.pb.cc schema.grpc.pb.h
	rm schema.pb.cc schema.pb.h
	rm schema.pb.o
	rm schema.grpc.pb.o
