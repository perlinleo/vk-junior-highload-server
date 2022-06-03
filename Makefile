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


## run-nginx: Run nginx server in docker on 8888 port
run-nginx:
	docker run -d --rm -p 8888:8888 --name nginx -t nginx

build-nginx:
	docker build -t nginx ./nginx-test

check:
	cppcheck --enable=all --language=c */*.c */*/*.h
	cpplint --extensions=c */*.c

.PHONY: all help build
all: help 
help: Makefile
	@echo
	@echo " Choose a command to run:"
	@echo
	@sed -n 's/^##//p' $< | column -t -s ':' |  sed -e 's/^/ /'
	@echo