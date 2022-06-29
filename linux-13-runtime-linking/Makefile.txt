all: exe 

exe: solution.c 
	gcc -o solution solution.c -ldl -fPIC '-Wl,-rpath=.'
