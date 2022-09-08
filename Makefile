all: build

build:
	mkdir -p build && cd build && cmake .. && make -j$(shell nproc)
.PHONY: build

test: build
	./build/mossetest

clean:
	cd build && make clean
.PHONY: clean
