SILENT = @

CC=gcc
LD=gcc

SRC_DIR=src
INT_DIR=../int/minivm
TARGET=minivm.exe

INCLUDES=-Isrc
CFLAGS=-Wall -g -MMD
LDFLAGS=

VM_SOURCES=$(wildcard $(SRC_DIR)/*.c)
VM_OBJECTS=$(subst $(SRC_DIR),$(INT_DIR),$(VM_SOURCES:.c=.o))

.PHONY: always

vm: $(TARGET) always

$(TARGET): $(VM_OBJECTS)
	$(SILENT) echo Linking minivm...
	$(SILENT) $(LD) -o $@ $^ $(LDFLAGS)

$(INT_DIR)/%.o: $(SRC_DIR)/%.c always
	$(SILENT) echo minivm/$<
	$(SILENT) $(CC) -c $< -o $@ $(INCLUDES) $(CFLAGS)

clean:
	$(SILENT) echo Cleaning minivm...
	$(SILENT) rmdir /S /Q "$(INT_DIR)"
	$(SILENT) del $(TARGET)

always:
	$(SILENT) if not exist "$(INT_DIR)" (mkdir "$(INT_DIR)")

-include $(VM_OBJECTS:.o=.d)
