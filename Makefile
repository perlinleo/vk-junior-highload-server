PROJECT_DIR := ${CURDIR}


## build: rebuild
build:
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make -j4 
## clean: delete build folder
clean:
	rm -rf build

## rebuild: delete current build and create a new one from scratch
rebuild: clean build

dev-deps:
	sudo apt -y -q install cppcheck
	sudo apt -y -q install python3-pip
	sudo apt -y -q install cmake
	sudo pip3 install cpplint


check:
	cppcheck --enable=all --language=c */*.c */*/*.h
	cpplint --extensions=c */*.c

.PHONY: all run help build
all: help run
help: Makefile
	@echo
	@echo " Choose a command to run:"
	@echo
	@sed -n 's/^##//p' $< | column -t -s ':' |  sed -e 's/^/ /'
	@echo