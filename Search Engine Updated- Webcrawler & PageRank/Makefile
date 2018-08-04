all: search
search: search.cpp webpage.cpp
	g++ -g -std=c++11 -Wall webpage.cpp search.cpp -o search

webpage.o: webpage.cpp
	g++ -g -std=c++11 -Wall -c webpage.cpp

crawler: crawler.cpp
	g++ -g -std=c++11 -Wall webpage.cpp crawler.cpp -o crawler