
$(MAKECMDGOALS) %: run_make

.PHONY: run_make

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))


PARTICLE_FIRMWARE=firmware

APPDIR = $(abspath $(current_dir)/../../..)
SPARKDIR = $(APPDIR)/platform/spark
APP ?= controller
APP_MAKEFILE = app/$(APP)/build.mk
# include exported definitions for the app now
-include $(APPDIR)/app/$(APP)/env.mk

run_make:
	@$(MAKE) -e -C $(PARTICLE_FIRMWARE)/main APPDIR=$(APPDIR) \
	USER_MAKEFILE=$(APP_MAKEFILE) \
	TARGET_DIR=$(SPARKDIR)/target/$(APP)-$(PLATFORM)  BUILD_PATH_BASE=$(SPARKDIR)/target $(MAKEOVERRIDES) $(MAKECMDGOALS) 
