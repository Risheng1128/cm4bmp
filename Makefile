# Generic Makefile
export BIN = $(shell pwd)/test/hello-world/main
export OUT = $(shell pwd)/build

CM4BM_DIR = cm4bm
CM4BM_DIR_BUILD = $(CM4BM_DIR)/Makefile
BIN_NAME = $(notdir $(BIN))

# FIXME: Compile other dependencies via cm4bm
# ifeq ($(TARGET), Task_Scheduler/main)
# 	C_SOURCES += Task_Scheduler/myscheduler.c
# 	C_INCLUDES += -ITask_Scheduler/myscheduler.h
# endif

all: $(OUT)/$(BIN_NAME)

$(CM4BM_DIR_BUILD):
	git submodule update --init $(dir $@)

$(OUT)/$(BIN_NAME): $(CM4BM_DIR_BUILD)
	$(MAKE) -C $(CM4BM_DIR) BIN=$(BIN) OUT=$(OUT)

debug:
	$(MAKE) -C $(CM4BM_DIR) debug

upload: $(OUT)/$(BIN_NAME)
	openocd -f interface/stlink-v2-1.cfg -f target/stm32f3x.cfg -c " program $< verify exit "

clean:
	$(RM) -r $(OUT)