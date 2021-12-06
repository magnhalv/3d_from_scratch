build:
	mkdir -p build
	gcc -Wall -std=c99 ./src/*.c -o ./build/renderer

run:
	./build/renderer

clean:
	rm -rf ./build