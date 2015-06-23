all: build run

build:
	docker build -t deberg:linux .
run:
	docker run -i -e "CXX=g++" -v `pwd`/..:/home/penny/deberg -t deberg:linux deberg/docker/test.sh
