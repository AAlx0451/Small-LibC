CC = clang
AR = ar
ARFLAGS = rcs
INCDIR = ../include
OBJDIR = obj
TARGET_LIB = libSystem.a

ifneq ($(VERBOSE), 1)
  Q = @
endif

MAIN_CFLAGS = -I$(INCDIR) -O2 -fPIC -I/usr/share/llvm/lib/clang/6.0.1/include -w -std=c99

CFLAGS = $(MAIN_CFLAGS) -nostdinc

SYSCALL_CFLAGS = $(MAIN_CFLAGS) -marm

REGULAR_SOURCES = $(wildcard c/stdlib/*.c c/string/*.c c/unistd/*.c c/stdio/*.c c/runtime/*.c)
REGULAR_OBJECTS = $(patsubst %.c, $(OBJDIR)/%.o, $(REGULAR_SOURCES))
SPECIAL_OBJECTS = $(OBJDIR)/arm/syscall.o
OBJECTS = $(REGULAR_OBJECTS) $(SPECIAL_OBJECTS)

.PHONY: all
all: $(TARGET_LIB)

$(TARGET_LIB): $(OBJECTS)
	@echo "  AR  $@"
	$(Q)$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)/arm/syscall.o: arm/syscall.c
	@mkdir -p $(dir $@)
	@echo "  CC  $<"
	$(Q)$(CC) $(SYSCALL_CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "  CC  $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@echo "  CLEAN"
	$(Q)rm -rf $(TARGET_LIB) $(OBJDIR) ../ext/printf/tmp
