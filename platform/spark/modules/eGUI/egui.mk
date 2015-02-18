
INCLUDE_DIRS += $(SRC_EGUI)/D4D

CPPSRC := $(filter-out %_Official_Demos%,$(CPPSRC))
CSRC := $(filter-out %_Official_Demos%,$(CSRC))