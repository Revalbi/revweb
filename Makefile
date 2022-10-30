CXXFLAGS = -Wall -std=c++20

revweb: serving.o revweb.o
	g++ -o revweb serving.o revweb.o
	
serving.o: serving.cc serving.h
	g++ $(CXXFLAGS) -c serving.cc

revweb.o: revweb.cc serving.h
	g++ $(CXXFLAGS) -c revweb.cc

.PHONY: clean

clean:
	rm -f *.o revweb

