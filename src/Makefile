CC ?= clang
INCDIR = ../include
OBJDIR = obj
CFLAGS := -Os

TARGET_LIBC       = libc.a
TARGET_LIBRT      = librt.a
TARGET_LIBM       = libm.a
TARGET_LIBMACH    = libmach.a
TARGET_LIBM_DYLIB = libm.dylib

C_SRCS    := $(shell find c asm -name '*.c' 2>/dev/null)
RT_SRCS   := $(shell find runtime -name '*.c' 2>/dev/null)
MATH_SRCS := $(shell find math -name '*.c' 2>/dev/null)
MACH_SRCS := $(shell find mach -name '*.c' 2>/dev/null)

MATHERR_WORDS := $(subst |, ,$(MATHERR))
HAS_ERRNO := $(filter errno,$(MATHERR_WORDS))
HAS_EXCEPT := $(filter except,$(MATHERR_WORDS))
HAS_BOTH := $(filter both,$(MATHERR_WORDS))

ifneq ($(HAS_BOTH),)
  MATH_CFLAGS := -Dmath_errhandling="(MATH_ERRNO | MATH_ERREXCEPT)"
else ifeq ($(and $(HAS_ERRNO),$(HAS_EXCEPT)),)
  ifeq ($(HAS_ERRNO),errno)
    MATH_CFLAGS := -Dmath_errhandling="(MATH_ERRNO)"
  else ifeq ($(HAS_EXCEPT),except)
    MATH_CFLAGS := -Dmath_errhandling="(MATH_ERREXCEPT)"
  endif
else
  MATH_CFLAGS := -Dmath_errhandling="(MATH_ERRNO | MATH_ERREXCEPT)"
endif

Q = $(if $(filter 1,$(VERBOSE)),,@)

.PHONY: all clean install uninstall

all: $(TARGET_LIBC) $(TARGET_LIBRT) $(TARGET_LIBM) $(TARGET_LIBMACH)

ifeq ($(CROSS), 1)
  ifndef SDK
    $(error SDK path must be set for cross-compilation when CROSS=1)
  endif

  AR = llvm-ar
  LIPO = llvm-lipo
  comma := ,
  ARCH_LIST = $(subst $(comma), ,$(ARCHES))
  ARCHES ?= armv7

  BASE_CFLAGS = $(CPPFLAGS) $(CFLAGS) -isystem $(INCDIR) -fPIC -std=gnu11 -isysroot $(SDK)

  define BUILD_ARCH
    OBJS_$(1) := $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(C_SRCS) $(RT_SRCS) $(MATH_SRCS) $(MACH_SRCS))
    
    $(OBJDIR)/$(1)/math/%.o: BASE_CFLAGS += $(MATH_CFLAGS)

    $(OBJDIR)/$(1)/%.o: %.c
		@printf "\tCC\t[$(1)] %%s\n" "$$<"
		@mkdir -p $$(dir $$@)
		$(Q)$(CC) $$(BASE_CFLAGS) -target $(1)-apple-ios6.1 -c $$< -o $$@

    $(OBJDIR)/$(1)/libc.a: $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(C_SRCS))
    $(OBJDIR)/$(1)/librt.a: $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(RT_SRCS))
    $(OBJDIR)/$(1)/libm.a: $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(MATH_SRCS))
    $(OBJDIR)/$(1)/libmach.a: $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(MACH_SRCS))

    $(OBJDIR)/$(1)/%.a:
		@printf "\tAR\t[$(1)] %%s\n" "$$@"
		@mkdir -p $$(dir $$@)
		$(Q)$(AR) rcs $$@ $$^

    ARCH_LIBS_C    += $(OBJDIR)/$(1)/libc.a
    ARCH_LIBS_RT   += $(OBJDIR)/$(1)/librt.a
    ARCH_LIBS_M    += $(OBJDIR)/$(1)/libm.a
    ARCH_LIBS_MACH += $(OBJDIR)/$(1)/libmach.a
  endef

  $(foreach arch,$(ARCH_LIST),$(eval $(call BUILD_ARCH,$(arch))))

  $(TARGET_LIBC): $(ARCH_LIBS_C)
	@printf "\tLIPO\t%s\n" "$@"
	$(Q)$(LIPO) -create -output $@ $^

  $(TARGET_LIBRT): $(ARCH_LIBS_RT)
	@printf "\tLIPO\t%s\n" "$@"
	$(Q)$(LIPO) -create -output $@ $^

  $(TARGET_LIBM): $(ARCH_LIBS_M)
	@printf "\tLIPO\t%s\n" "$@"
	$(Q)$(LIPO) -create -output $@ $^

  $(TARGET_LIBMACH): $(ARCH_LIBS_MACH)
	@printf "\tLIPO\t%s\n" "$@"
	$(Q)$(LIPO) -create -output $@ $^

else
  AR = ar
  BASE_CFLAGS = $(CPPFLAGS) $(CFLAGS) -isystem $(INCDIR) -fPIC -std=gnu11 -nostdinc \
		-isystem /usr/share/llvm/lib/clang/7.1.0/include \
		-isystem /usr/share/llvm/lib/clang/6.0.1/include

  OBJS_C    = $(patsubst %.c,$(OBJDIR)/%.o,$(C_SRCS))
  OBJS_RT   = $(patsubst %.c,$(OBJDIR)/%.o,$(RT_SRCS))
  OBJS_M    = $(patsubst %.c,$(OBJDIR)/%.o,$(MATH_SRCS))
  OBJS_MACH = $(patsubst %.c,$(OBJDIR)/%.o,$(MACH_SRCS))

  $(OBJDIR)/math/%.o: BASE_CFLAGS += $(MATH_CFLAGS)

  $(OBJDIR)/%.o: %.c
	@printf "\tCC\t%s\n" "$<"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(BASE_CFLAGS) -c $< -o $@

  $(TARGET_LIBC): $(OBJS_C)
	@printf "\tAR\t%s\n" "$@"
	$(Q)$(AR) rcs $@ $^

  $(TARGET_LIBRT): $(OBJS_RT)
	@printf "\tAR\t%s\n" "$@"
	$(Q)$(AR) rcs $@ $^

  $(TARGET_LIBM): $(OBJS_M)
	@printf "\tAR\t%s\n" "$@"
	$(Q)$(AR) rcs $@ $^

  $(TARGET_LIBM_DYLIB): $(OBJS_M)
	@printf "\tLD\t%s\n" "$@"
	$(Q)$(CC) -dynamiclib -o $@ $^

  $(TARGET_LIBMACH): $(OBJS_MACH)
	@printf "\tAR\t%s\n" "$@"
	$(Q)$(AR) rcs $@ $^
endif

clean:
	@printf "\tCLEAN\n"
	$(Q)rm -rf $(TARGET_LIBC) $(TARGET_LIBRT) $(TARGET_LIBM) $(TARGET_LIBMACH) $(TARGET_LIBM_DYLIB) $(OBJDIR)

install: all
	@echo "This lib can't be installed..."

uninstall: clean
