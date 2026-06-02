CXX := g++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -pedantic -Iinclude
TARGET := prng_blue_plots
SRC := src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC) include/*.h
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -rf plots

.PHONY: all run clean
