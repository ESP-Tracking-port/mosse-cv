all: build

build:
	mkdir -p build && cd build && cmake .. && make -j$(shell nproc)
.PHONY: build

test: build
	./build/test/test

clean:
	rm -rf build
.PHONY: clean
