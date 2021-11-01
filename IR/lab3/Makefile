indexer: main.o indexer.o
	g++ --std=c++17 -lboost_system -lboost_filesystem -lboost_serialization -o indexer main.o indexer.o

main.o: main.cpp indexer.h
	g++ --std=c++17  -c main.cpp

indexer.o: indexer.cpp indexer.h
	g++ --std=c++17  -c indexer.cpp

clean:
	rm -rf *.o indexer