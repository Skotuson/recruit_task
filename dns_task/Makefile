CXX      = g++
LD       = g++
CXXFLAGS = -g -std=c++17 -Wall -pedantic
OUTPUT   = dns.out

SOURCES = $(wildcard *.cpp */*.cpp)
OBJS    = $(SOURCES:.cpp=.o)


all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(LD) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o */*.o $(OUTPUT)
 
run: $(OUTPUT)
	./$(OUTPUT)