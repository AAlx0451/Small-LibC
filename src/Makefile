CC = clang
INCDIR = ../include
OBJDIR = obj
TARGET_LIB = libc.a

GENERIC_SRCS := $(wildcard sysindep/*/*.c sysindep/*/src/*.c)
MARM_SRCS := sysdep/syscall.c $(wildcard sysdep/unistd/*.c)

ifneq ($(VERBOSE), 1)
  Q = @
endif

.PHONY: all clean
all: $(TARGET_LIB)

ifeq ($(CROSS), 1)
  ifndef SDK
    $(error SDK path must be set for cross-compilation)
  endif
	
  LLVM_PREFIX = llvm-
  AR = $(LLVM_PREFIX)ar
  LIPO = $(LLVM_PREFIX)lipo
  
  ARCHES ?= armv7
  comma := ,
  empty :=
  space := $(empty) $(empty)
  ARCH_LIST = $(subst $(comma),$(space),$(ARCHES))
  PIC = -fPIC
  BASE_CFLAGS = -I$(INCDIR) -Os $(PIC) -std=c99 -isysroot $(SDK)

  define BUILD_ARCH
    OBJS_GENERIC_$(1) := $$(patsubst %.c,$(OBJDIR)/$(1)/%.o,$$(GENERIC_SRCS))
    OBJS_MARM_$(1) := $$(patsubst %.c,$(OBJDIR)/$(1)/%.o,$$(MARM_SRCS))
    LIB_$(1) := $(OBJDIR)/libSystem-$(1).a
    
    ARCH_LIBS += $$(LIB_$(1))

    $$(LIB_$(1)): $$(OBJS_GENERIC_$(1)) $$(OBJS_MARM_$(1))
	@echo "  AR   [$(1)] $$@"
	@mkdir -p $$(dir $$@)
	$$(Q)$(AR) rcs $$@ $$^

    $$(OBJS_GENERIC_$(1)): $(OBJDIR)/$(1)/%.o: %.c
	@echo "  CC   [$(1)] $$<"
	@mkdir -p $$(dir $$@)
	$$(Q)$(CC) $$(BASE_CFLAGS) -target $(1)-apple-ios6.1 -c $$< -o $$@

    $$(OBJS_MARM_$(1)): $(OBJDIR)/$(1)/%.o: %.c
	@echo "  CC   [$(1)] $$<"
	@mkdir -p $$(dir $$@)
	$$(Q)$(CC) $$(BASE_CFLAGS) -target $(1)-apple-ios6.1 -marm -c $$< -o $$@
  endef

  $(foreach arch,$(ARCH_LIST),$(eval $(call BUILD_ARCH,$(arch))))

  $(TARGET_LIB): $(ARCH_LIBS)
	@echo "  LIPO $@"
	$(Q)$(LIPO) -create -output $@ $^

else
  AR = ar
  PIC = -fPIC
  BASE_CFLAGS = -I$(INCDIR) -Os $(PIC) -std=c99 -nostdinc -I/usr/share/llvm/lib/clang/6.0.1/include

  OBJS_GENERIC := $(patsubst %.c,$(OBJDIR)/%.o,$(GENERIC_SRCS))
  OBJS_MARM := $(patsubst %.c,$(OBJDIR)/%.o,$(MARM_SRCS))

  $(TARGET_LIB): $(OBJS_GENERIC) $(OBJS_MARM)
	@echo "  AR   $@"
	$(Q)$(AR) rcs $@ $^

  $(OBJS_GENERIC): $(OBJDIR)/%.o: %.c
	@echo "  CC   $<"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(BASE_CFLAGS) -c $< -o $@

  $(OBJS_MARM): $(OBJDIR)/%.o: %.c
	@echo "  CC   $<"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(BASE_CFLAGS) -marm -c $< -o $@

endif

clean:
	@echo "  CLEAN"
	$(Q)rm -rf $(TARGET_LIB) $(OBJDIR)
