build: ./src/*.cpp ./src/*.h
	mkdir -p build
	g++ -g -Wall -std=c++2a ./src/*.cpp ./src/lib/*.c -lSDL2 -o ./build/renderer

run:
	./build/renderer

clean:
	rm -rf ./build
