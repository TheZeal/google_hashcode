hc: hc.o
	c++ hc.o -o hc

hc.o:	hc.cpp
	c++ -c -O3 hc.cpp -o hc.o
