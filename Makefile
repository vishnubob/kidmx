CPP_FLAGS = -ggdb -Isrc

kidmx: build build/kinet.o build/kidmx.o build/rpidmx.o
	g++ ${CPP_FLAGS} build/kidmx.o build/kinet.o build/rpidmx.o -o kidmx -lrt -lpigpio -lpthread

build/kinet.o: build src/kinet.h src/kinet.cpp
	g++ ${CPP_FLAGS} -c src/kinet.cpp -o build/kinet.o

build/kidmx.o: build src/kidmx.cpp
	g++ ${CPP_FLAGS} -c src/kidmx.cpp -o build/kidmx.o

build/rpidmx.o: build src/rpidmx.h src/rpidmx.cpp
	g++ ${CPP_FLAGS} -c src/rpidmx.cpp -o build/rpidmx.o

build:
	mkdir build

clean:
	rm -rf build kidmx

.PHONY: clean
