CC = gcc
CFLAGS = -Wall -O2

all:
	@echo "Use 'make sequential', 'make openmp', or 'make openmpi' after branching."

clean:
	rm -f sequential openmp openmpi