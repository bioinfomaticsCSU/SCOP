CC=g++

CPPFLAGS = -g -Wall -O3


SCOP:	main.o generateGraph.o executeScaffolding.o
	$(CC) -o $@ $^ ./lp/liblpsolve55.a -lm -ldl -I $(BAMTOOLS_HOME)/include -L $(BAMTOOLS_HOME)/lib -lbamtools -lpython2.7 -lpthread -lm -ldl -lutil -std=gnu++98

main.o: main.cpp generateGraph.h executeScaffolding.h
	$(CC) -c main.cpp -lm -ldl -I $(BAMTOOLS_HOME)/include -L $(BAMTOOLS_HOME)/lib -lpython2.7 -lpthread -lm -ldl -lutil  -std=gnu++98
	
generateGraph.o: generateGraph.cpp generateGraph.h
	$(CC) -c generateGraph.cpp -lm -ldl -I $(BAMTOOLS_HOME)/include -L $(BAMTOOLS_HOME)/lib -lbamtools -lpython2.7 -lpthread -lm -ldl -lutil -std=gnu++98
	
executeScaffolding.o: executeScaffolding.cpp generateGraph.h executeScaffolding.h
	$(CC) -c executeScaffolding.cpp -lm -ldl -I $(BAMTOOLS_HOME)/include -L $(BAMTOOLS_HOME)/lib -lbamtools -lpython2.7 -lpthread -lm -ldl -lutil -std=gnu++98
	

all: SCOP
	rm -f *.o

clean:
	rm -f *.o
	rm SCOP
