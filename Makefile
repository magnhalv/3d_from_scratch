build:
	mkdir -p build
	g++ -g -Wall -std=c++2a ./src/*.cpp -lSDL2 -o ./build/renderer

run:
	./build/renderer

clean:
	rm -rf ./build
