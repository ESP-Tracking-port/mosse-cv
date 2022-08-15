all: build

build:
	mkdir -p build && cd build && cmake .. && make -j$(shell nproc)
.PHONY: build

clean:
	rm -rf build
.PHONY: clean
