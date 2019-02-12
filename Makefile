

schema :
	cd proto/ && make 

utils :
	cd utils/ && make

server :
	cd server/ && make 

client :
	cd client/ && make 

all :
	make schema
	make utils
	make server
	make client

clean :
	cd proto/ && make clean
	cd utils/ && make clean
	cd server/ && make clean
	cd client/ && make clean
