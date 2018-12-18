$(MAKECMDGOALS) : run_make

.PHONY: run_make

current_dir := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
APPDIR := $(realpath $(current_dir)/..)
SPARKDIR := $(APPDIR)/platform/spark/firmware

APP ?= brewblox
APP_MAKEFILE := app/$(APP)/build.mk
WARNINGS_AS_ERRORS ?=n
USE_PRINTF_FLOAT ?=n

run_make:
	echo "Building app $(APP) from $(APP_MAKEFILE)"
	@$(MAKE) -s -C $(SPARKDIR)/main \
	APPDIR=$(APPDIR) \
	USER_MAKEFILE=$(APP_MAKEFILE) \
	USE_PRINTF_FLOAT=$(USE_PRINTF_FLOAT) \
	WARNINGS_AS_ERRORS=$(WARNINGS_AS_ERRORS) \
	TARGET_DIR=$(current_dir)/target/$(APP)-$(PLATFORM) \
	BUILD_PATH_BASE=$(current_dir)/target \
	PARTICLE_DEVELOP=y \
	$(MAKEOVERRIDES) $(MAKECMDGOALS)
