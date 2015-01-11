
$(MAKECMDGOALS) %: run_make

.PHONY: run_make

run_make:
	@$(MAKE) -s -C ../../../spark-firmware/main $(MAKECMDGOALS) APPDIR=../../../firmware/platform/spark TARGET_FILE=brewpi
