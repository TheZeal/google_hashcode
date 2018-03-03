hcd: hcd.o
	c++ hcd.o -o hcd

hcd.o:	hcd.cpp
	c++ -c -g hcd.cpp -o hcd.o

hc: hc.o

	c++ hc.o -o hc

hc.o:	hc.cpp
	c++ -c -g hc.cpp -o hc.o

clean:
	rm hc hc.o hcd hcd.o
