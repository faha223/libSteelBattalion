CXX = g++
FLAGS = -Wall -c

default: test32 test64

# The 32-bit version of the Steel Battalion Controller interface library
libSteelBattalion32.so: SteelBattalion.h SteelBattalion.cpp Buttons.h
	$(CXX) $(FLAGS) -m32 -shared SteelBattalion.cpp -o libSteelBattalion32.so

# The 64-bit version of the Steel Battalion Controller interface library
libSteelBattalion64.so: SteelBattalion.h SteelBattalion.cpp Buttons.h
	$(CXX) $(FLAGS) -m64 -shared SteelBattalion.cpp -o libSteelBattalion64.so

# A 64-bit test application that uses the controller interface class
test64: main.cpp libSteelBattalion64.so
	$(CXX) -Wall -lusb-1.0 -lpthread -L./ -lSteelBattalion64 main.cpp -o test64

# A 32-bit test application that uses the controller interface class
test32: main.cpp libSteelBattalion32.so
	$(CXX) -Wall -m32 -L/usr/lib32 -lusb -lpthread -L./ -lSteelBattalion32 main.cpp -o test32

clean:
	rm -f test32
	rm -f test64
	rm -f libSteelBattalion32.so
	rm -f libSteelBattalion64.so
