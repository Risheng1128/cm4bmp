# Generic Makefile
PWD = $(shell pwd)
OUT ?= $(PWD)/build
BIN ?= main

# c source file
INPUT_C_SRC ?= $(PWD)/test/hello-world/main.c
INPUT_C_INC ?= 

# cm4bm project path
CM4BM_DIR = cm4bm
CM4BM_DIR_BUILD = $(CM4BM_DIR)/Makefile

all: $(OUT)/$(BIN_NAME)

$(CM4BM_DIR_BUILD):
	git submodule update --init $(dir $@)

$(OUT)/$(BIN_NAME): $(CM4BM_DIR_BUILD)
	$(MAKE) -C $(CM4BM_DIR) OUT=$(OUT) BIN=$(BIN) INPUT_C_SRC=$(INPUT_C_SRC) INPUT_C_INC=$(INPUT_C_INC)

debug:
	$(MAKE) -C $(CM4BM_DIR) debug

upload: $(OUT)/$(BIN_NAME)
	$(MAKE) -C $(CM4BM_DIR) upload OUT=$(OUT) BIN=$(BIN)

clean:
	$(RM) -r $(OUT)