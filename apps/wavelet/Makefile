include ../support/Makefile.inc

HL_MODULES = daubechies_x.o inverse_daubechies_x.o haar_x.o inverse_haar_x.o

all: filter

wavelet: ../../ wavelet.cpp
	$(MAKE) -C ../../ $(LIB_HALIDE)
	$(CXX) wavelet.cpp -I ../../include $(LDFLAGS) ../../$(LIB_HALIDE) -o wavelet -lpthread -ldl -lz -g

$(HL_MODULES): wavelet
	./wavelet

filter.o: filter.cpp $(HL_MODULES)
	$(CXX) $(LIBPNG_CXX_FLAGS) -I../support -O3 -c filter.cpp

filter: filter.o
	$(CXX) filter.o $(HL_MODULES) $(PNGFLAGS) $(CUDA_LDFLAGS) $(OPENCL_LDFLAGS) -o filter -lpthread

clean:
	rm -f wavelet filter filter.o $(HL_MODULES) $(HL_MODULES:%.o) $(HL_MODULES:%.o=%.h)

test: filter
	./filter ../images/gray.png
