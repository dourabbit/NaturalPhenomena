# $Id: gfx-config.in 343 2008-09-13 18:34:59Z garland $

CXX = g++
CXXFLAGS = -g -O2 -Wall -Wno-sign-compare -Iinclude -DHAVE_CONFIG_H 
OBJS = StableFluids.o ScalarField.o VectorField.o imageio.o

project2: $(OBJS)
	$(CXX) -o $@ $^ -lglut -lpng
clean:
	rm $(OBJS) project2
