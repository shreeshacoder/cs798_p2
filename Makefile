


all :
	cd proto/ && make 
	cd utils/ && make
	cd server/ && make 
	cd client/ && make 

clean :
	cd proto/ && make clean
	cd utils/ && make clean
	cd server/ && make clean
	cd client/ && make clean
