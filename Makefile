all: build
MOSSE_DEF_ASSERT := -DMOSSE_PORT_USE_CASSERT=1
MOSSE_DEF_PORT := -DMOSSE_PORTABLE=1
PORTABLE := 1

ifeq ($(PORTABLE), 1)
	MAIN:= build_port
else
	MAIN := build
endif

primary: $(MAIN)

build_port:
	mkdir -p build && cd build && cmake $(MOSSE_DEF_ASSERT) $(MOSSE_DEF_PORT) .. && make -j$(shell nproc)
.PHONY: build_port

build:
	mkdir -p build && cd build && cmake .. && make -j$(shell nproc)
.PHONY: build

test: build
	./build/mossetest

clean:
	cd build && make clean
.PHONY: clean
