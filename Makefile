CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
TARGET = code

$(TARGET): main.cpp game.h
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET)
