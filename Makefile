all: clean
	g++ -O3 rule.cpp -o run
	
debug: clean
	g++ -O3 rule.cpp -o run -ggdb
	
clean:
	rm ./run
