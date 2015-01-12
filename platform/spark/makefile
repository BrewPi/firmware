
$(MAKECMDGOALS) %: run_make

.PHONY: run_make

APPDIR = ../../firmware
SPARKDIR = $(APPDIR)/platform/spark

run_make:
	@$(MAKE) -s -e -C ../../../spark-firmware/main APPDIR=$(APPDIR) TARGET_FILE=brewpi TARGET_DIR=$(SPARKDIR)/target $(MAKEOVERRIDES) $(MAKECMDGOALS) 
