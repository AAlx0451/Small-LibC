CC = clang
AR = ar

ROOT_DIR = ..
INC_DIR  = $(ROOT_DIR)/include
SRC_LIB_DIR = $(ROOT_DIR)/src
LLVM_INC = /usr/share/llvm/lib/clang/7.1.0/include

OBJ_DIR = obj
LIB_OUT_DIR = lib

CFLAGS = -I$(INC_DIR) -I$(LLVM_INC) -nostdinc -Wall -Wextra -g -O0
LDFLAGS = -static -nostdlib -L$(LIB_OUT_DIR) -L$(SRC_LIB_DIR)
BASE_LIBS = -lc -lrt -lmach

CT_SRCS  = $(wildcard asm/*.c asm/*/*.c c/*/*.c c/*/*/*.c)
CT_OBJS  = $(patsubst %.c, $(OBJ_DIR)/%.o, $(CT_SRCS))

RTT_SRCS = $(wildcard runtime/asm/*.c runtime/c/*.c)
RTT_OBJS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(RTT_SRCS))

MT_SRCS  = $(wildcard math/*.c)
MT_OBJS  = $(patsubst %.c, $(OBJ_DIR)/%.o, $(MT_SRCS))

.PHONY: all clean

all: ctest rttest mtest

$(LIB_OUT_DIR)/libct.a: $(CT_OBJS)
	@mkdir -p $(dir $@)
	@printf "\tAR\t%s\n" $@
	@$(AR) rcs $@ $^

ctest: suite/runtests.c $(LIB_OUT_DIR)/libct.a
	@printf "\tLD\t%s\n" $@
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -lct $(BASE_LIBS)

$(LIB_OUT_DIR)/librtt.a: $(RTT_OBJS)
	@mkdir -p $(dir $@)
	@printf "\tAR\t%s\n" $@
	@$(AR) rcs $@ $^

rttest: runtime/suite/runtests.c $(LIB_OUT_DIR)/librtt.a
	@printf "\tLD\t%s\n" $@
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -lrtt $(BASE_LIBS)

$(LIB_OUT_DIR)/libmt.a: $(MT_OBJS)
	@mkdir -p $(dir $@)
	@printf "\tAR\t%s\n" $@
	@$(AR) rcs $@ $^

mtest: math/suite/runtests.c $(LIB_OUT_DIR)/libmt.a
	@printf "\tLD\t%s\n" $@
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -lmt $(BASE_LIBS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "\tCC\t%s\n" $<
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "\tCLEAN\n"
	@rm -rf $(OBJ_DIR) $(LIB_OUT_DIR)
	@rm -f ctest rttest mtest
