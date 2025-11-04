SILENT = @

.PHONY: all always

all: vm assembler

vm:
	$(SILENT) $(MAKE) --no-print-directory -C minivm -f minivm.make vm

assembler:
	$(SILENT) $(MAKE) --no-print-directory -C minivm-assembler -f minivm-assembler.make vm

clean:
	$(SILENT) $(MAKE) --no-print-directory -C minivm -f minivm.make clean
	$(SILENT) $(MAKE) --no-print-directory -C minivm-assembler -f minivm-assembler.make clean

always:
	$(SILENT) $(MAKE) --no-print-directory -C minivm -f minivm.make always
	$(SILENT) $(MAKE) --no-print-directory -C minivm-assembler -f minivm-assembler.make always
