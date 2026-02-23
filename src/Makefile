CC = clang
INCDIR = ../include
OBJDIR = obj
CFLAGS := -Os

TARGET_LIBC = libc.a
TARGET_LIBRT = librt.a
TARGET_LIBM = libm.a

GENERIC_SRCS := $(wildcard sysindep/*/*.c sysindep/*/src/*.c)
SYSDEP_SRCS  := sysdep/syscall.c $(wildcard sysdep/*/*.c sysdep/*.c)
RT_SRCS      := $(wildcard runtime/*/*.c)
MATH_SRCS    := $(wildcard math/*.c)

Q = $(if $(filter 1,$(VERBOSE)),,@)

.PHONY: all clean install uninstall

all: $(TARGET_LIBC) $(TARGET_LIBRT) $(TARGET_LIBM)

ifeq ($(CROSS), 1)
  ifndef SDK
    $(error SDK path must be set for cross-compilation)
  endif

  AR = llvm-ar
  LIPO = llvm-lipo
  comma := ,
  ARCH_LIST = $(subst $(comma), ,$(ARCHES))
  ARCHES ?= armv7

  BASE_CFLAGS = $(CPPFLAGS) $(CFLAGS) -I$(INCDIR) -fPIC -std=gnu11 -isysroot $(SDK)

  define BUILD_ARCH
    OBJS_$(1) := $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(GENERIC_SRCS) $(SYSDEP_SRCS) $(RT_SRCS) $(MATH_SRCS))
    
    $(OBJDIR)/$(1)/%.o: %.c
	@echo "  CC   [$(1)] $$<"
	@mkdir -p $$(dir $$@)
	$(Q)$(CC) $(BASE_CFLAGS) -target $(1)-apple-ios6.1 -c $$< -o $$@

    $(OBJDIR)/$(1)/libc.a: $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(GENERIC_SRCS) $(SYSDEP_SRCS))
    $(OBJDIR)/$(1)/librt.a: $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(RT_SRCS))
    $(OBJDIR)/$(1)/libm.a: $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(MATH_SRCS))

    $(OBJDIR)/$(1)/%.a:
	@echo "  AR   [$(1)] $$@"
	@mkdir -p $$(dir $$@)
	$(Q)$(AR) rcs $$@ $$^

    ARCH_LIBS_C  += $(OBJDIR)/$(1)/libc.a
    ARCH_LIBS_RT += $(OBJDIR)/$(1)/librt.a
    ARCH_LIBS_M  += $(OBJDIR)/$(1)/libm.a
  endef

  $(foreach arch,$(ARCH_LIST),$(eval $(call BUILD_ARCH,$(arch))))

  $(TARGET_LIBC): $(ARCH_LIBS_C)
	@echo "  LIPO $@"
	$(Q)$(LIPO) -create -output $@ $^

  $(TARGET_LIBRT): $(ARCH_LIBS_RT)
	@echo "  LIPO $@"
	$(Q)$(LIPO) -create -output $@ $^

  $(TARGET_LIBM): $(ARCH_LIBS_M)
	@echo "  LIPO $@"
	$(Q)$(LIPO) -create -output $@ $^

else
  AR = ar
  BASE_CFLAGS = $(CPPFLAGS) $(CFLAGS) -isystem $(INCDIR) -fPIC -std=gnu11 -nostdinc \
		-isystem /usr/share/llvm/lib/clang/7.1.0/include \
		-isystem /usr/share/llvm/lib/clang/6.0.1/include

  OBJS_C  = $(patsubst %.c,$(OBJDIR)/%.o,$(GENERIC_SRCS) $(SYSDEP_SRCS))
  OBJS_RT = $(patsubst %.c,$(OBJDIR)/%.o,$(RT_SRCS))
  OBJS_M  = $(patsubst %.c,$(OBJDIR)/%.o,$(MATH_SRCS))

  $(OBJDIR)/%.o: %.c
	@echo "  CC   $<"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(BASE_CFLAGS) -c $< -o $@

  $(TARGET_LIBC): $(OBJS_C)
	@echo "  AR   $@"
	$(Q)$(AR) rcs $@ $^

  $(TARGET_LIBRT): $(OBJS_RT)
	@echo "  AR   $@"
	$(Q)$(AR) rcs $@ $^

  $(TARGET_LIBM): $(OBJS_M)
	@echo "  AR   $@"
	$(Q)$(AR) rcs $@ $^
endif

clean:
	@echo "  CLEAN"
	$(Q)rm -rf $(TARGET_LIBC) $(TARGET_LIBRT) $(TARGET_LIBM) $(OBJDIR)

install: all
	@echo "This lib can't be installed..."

uninstall: clean
