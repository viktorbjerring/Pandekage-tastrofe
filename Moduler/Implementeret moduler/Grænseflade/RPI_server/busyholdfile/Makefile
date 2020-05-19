CXX=g++
CXXFLAGS=-I. -pthread
EXE=prog
SOURCES=main.cpp
DEPS=$(SOURCES:.cpp=.d)
OBJECTS=${SOURCES:.cpp=.o}

all: $(EXE)

$(EXE): ${DEPS} ${OBJECTS}
	$(CXX) -o $(EXE) ${OBJECTS} ${CXXFLAGS}

%.o: %.cpp
	$(CXX) -c -o $@ $^ ${CXXFLAGS}

# Rule that describes how a .d (dependency) file is created from a .cpp file
# Similar to the assigment that you just completed %.cpp -> %.o

%.d: %.cpp
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $^ > $@

clean: 
	@rm -f *.o *.d $(EXE)
	@echo "Så er der rent! :o)"

.PHONY: clean all
