.PHONY: clean

CXX = g++-14
CXXFLAGS = -std=c++20 -Wall
SOURCES = main.cc
OBJECTS = $(SOURCES:.cc=.o)
EXEC = myProg

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -Llibxlsxwriter/lib -lxlsxwriter -Wl,-rpath,'$$ORIGIN/libxlsxwriter/lib' -o $(EXEC)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)
