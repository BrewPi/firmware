
$(MAKECMDGOALS) %: run_make

.PHONY: run_make

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))


PARTICLE_FIRMWARE=firmware

APPDIR = $(abspath $(current_dir)/../../..)
SPARKDIR = $(APPDIR)/platform/spark
APP ?= controller

run_make:
	@$(MAKE) -e -C $(PARTICLE_FIRMWARE)/main APPDIR=$(APPDIR) \
	USER_MAKEFILE=app/$(APP)/build.mk \
	TARGET_FILE=brewpi TARGET_DIR=$(SPARKDIR)/target  BUILD_PATH_BASE=$(SPARKDIR)/target $(MAKEOVERRIDES) $(MAKECMDGOALS) 
