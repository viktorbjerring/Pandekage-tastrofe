ARCH?=host

ifeq (${ARCH},host)
BUILD_DIR=./build/host
BUILD_BIN=./bin/host
endif

ifeq (${ARCH},target)
CXX=arm-rpizw-g++
BUILD_DIR=./build/target
BUILD_BIN=./bin/target
endif

LDFLAGS= -pthread
SOURCES=main.cpp I2CData.cpp DispeKommu.cpp
OBJECTS=$(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.o))
DEPS=$(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.d))
EXE=prog
CXXFLAGS=-I.
CXX=g++
MKDIR_P =@mkdir -p

all: $(EXE)

$(EXE): $(DEPS) $(OBJECTS)
	$(MKDIR_P) $(BUILD_BIN)
	$(CXX) $(CXXFLAGS) -o $(BUILD_BIN)/$@ $(OBJECTS) $(LDFLAGS)

$(BUILD_DIR)/%.d: %.cpp
	$(MKDIR_P) $(BUILD_DIR)
	$(CXX) -MT$@ -MM $(CXXFLAGS) $< > $@
	$(CXX) -MT$(@:.d=.o) -MM $(CXXFLAGS) $< >> $@ 

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


.PHONY=clean
clean:
	-rm -rf bin/
	-rm -rf build/

run:
	@make all
	@echo "Running $(EXE)"
	@cd $(shell pwd)/bin/host/ && ./$(EXE)
		

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif