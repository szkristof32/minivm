SILENT = @

CC=gcc
LD=gcc

SRC_DIR=src
INT_DIR=../int/minivm-assembler
TARGET=minivm-assembler.exe

INCLUDES=-Isrc
CFLAGS=-Wall -g -MMD
LDFLAGS=

VM_SOURCES=$(wildcard $(SRC_DIR)/*.c)
VM_OBJECTS=$(subst $(SRC_DIR),$(INT_DIR),$(VM_SOURCES:.c=.o))

CLEAN=
ALWAYS=

ifeq ($(SHELL), cmd)
	CLEAN+=$(SILENT) if exist "$(INT_DIR)" (rmdir /S /Q "$(INT_DIR)") &
	CLEAN+=$(SILENT) if exist $(TARGET) (del $(TARGET))
	ALWAYS+=$(SILENT) if not exist "$(INT_DIR)" (mkdir "$(INT_DIR)")
else
	CLEAN+=$(SILENT) rm -rf $(INT_DIR)
	CLEAN+=$(SILENT) rm $(TARGET)
	ALWAYS+=$(SILENT) mkdir -p $(INT_DIR)
endif

.PHONY: always

vm: $(TARGET) always

$(TARGET): $(VM_OBJECTS)
	$(SILENT) echo Linking minivm-assembler...
	$(SILENT) $(LD) -o $@ $^ $(LDFLAGS)

$(INT_DIR)/%.o: $(SRC_DIR)/%.c always
	$(SILENT) echo minivm-assembler/$<
	$(SILENT) $(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

clean:
	$(SILENT) echo Cleaning minivm-assembler...
	$(CLEAN)

always:
	$(ALWAYS)

-include $(VM_OBJECTS:.o=.d)
