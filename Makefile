revweb: serving.o revweb.o
	g++ -o revweb serving.o revweb.o
	
serving.o: serving.cc serving.h
	g++ -Wall -c serving.cc

revweb.o: revweb.cc serving.h
	g++ -Wall -c revweb.cc

.PHONY: clean

clean:
	rm -f *.o revweb

