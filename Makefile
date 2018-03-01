hc: hc.o
	c++ hc.o -o hc

hc.o:	hc.cpp
	c++ -c -g hc.cpp -o hc.o

clean:
	rm hc hc.o
