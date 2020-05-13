ARCH?=host
CC=gcc
ifeq (${ARCH},host)
BUILD_DIR=./build/host
BUILD_BIN="/home/pi/Pandekage-tastrofe/Moduler/Uimplementeret moduler/RPI_server/flaskServer_v5.2/"
endif

ifeq (${ARCH},target)
CC=arm-rpizw-gcc
BUILD_DIR=./build/target
BUILD_BIN=./bin/target
endif

LDFLAGS= -pthread
SOURCES= $(wildcard *.c)
OBJECTS=$(addprefix $(BUILD_DIR)/,$(SOURCES:.c=.o))
DEPS=$(addprefix $(BUILD_DIR)/,$(SOURCES:.c=.d))
EXE=prog
CCFLAGS=-I.
MKDIR_P =@mkdir -p

all: $(EXE)

$(EXE): $(DEPS) $(OBJECTS)
	$(MKDIR_P) $(BUILD_BIN)
	$(CC) $(CCFLAGS) -o $(BUILD_BIN)/$@ $(OBJECTS) $(LDFLAGS)

$(BUILD_DIR)/%.d: %.c
	$(MKDIR_P) $(BUILD_DIR)
	$(CC) -MT$@ -MM $(CCFLAGS) $< > $@
	$(CC) -MT$(@:.d=.o) -MM $(CCFLAGS) $< >> $@ 

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@


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
