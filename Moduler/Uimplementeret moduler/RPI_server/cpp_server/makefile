ARCH ?= host

SOURCES = $(wildcard *.cpp)
OBJECTS = $(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.o))
DEPS = $(addprefix $(BUILD_DIR)/,$(SOURCES:.cpp=.d))
EXE = $(addprefix $(BIN_DIR)/, server)
CCFLAGS = -I.


#Making for host
# > make ARCH=host
ifeq (${ARCH},host)
CC = g++
BUILD_DIR = ./build/host
BIN_DIR = ./bin/host
endif

#Making for target
# > make ARCH=target
ifeq (${ARCH},target)
CC = arm-rpizw-g++
BUILD_DIR = ./build/target
BIN_DIR = ./bin/target
endif

#Making Directory for all the bins
$(EXE): $(DEPS) $(OBJECTS)     # << Check the $(DEPS) new dependency
	mkdir -p $(BIN_DIR)
	$(CC) $(CCFLAGS) -o $@ $(OBJECTS)

#Rule that describes how a .d (dependency) file is created from a .cpp file
#Similar to the assignment that you just completed %.cpp -> %.o

#Making Directory for the .d files
$(BUILD_DIR)/%.d: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) -MT$(@:.d=.o) -MM $(CCFLAGS) $^ > $@

#Making Directory for the .o files
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CCFLAGS)

.PHONY = clean all
clean:
	-rm -rf bin/
	-rm -rf build/

ifneq ($(MAKECMDGOALS)),clean)
-include $(DEPS)
endif
