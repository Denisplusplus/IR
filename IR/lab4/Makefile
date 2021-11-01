searcher: main.o search.o query.o
	g++ --std=c++17 -lboost_system -lboost_filesystem -lboost_serialization -o searcher main.o search.o query.o

main.o: main.cpp search.h query.h
	g++ --std=c++17  -c main.cpp

search.o: search.cpp search.h
	g++ --std=c++17  -c search.cpp

query.o: query.cpp query.h
	g++ --std=c++17  -c query.cpp


clean:
	rm -rf *.o searcher