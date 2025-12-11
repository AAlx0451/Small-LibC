CC = clang
INCDIR = ../include
OBJDIR = obj
TARGET_LIBC = libc.a
TARGET_LIBRT = librt.a
TARGET_LIBM = libm.a

CFLAGS ?=
CPPFLAGS ?=

# Поиск исходников согласно вашему дереву каталогов
GENERIC_SRCS := $(wildcard sysindep/*/*.c sysindep/*/src/*.c)
SYSDEP_SRCS := sysdep/syscall.c $(wildcard sysdep/*/*.c)
RT_SRCS := $(wildcard runtime/*/*.c)
MATH_SRCS := $(wildcard math/*.c)

ifneq ($(VERBOSE), 1)
	Q = @
endif

.PHONY: all clean
all: $(TARGET_LIBC) $(TARGET_LIBRT) $(TARGET_LIBM)

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
	BASE_CFLAGS = $(CPPFLAGS) $(CFLAGS) -I$(INCDIR) -Os $(PIC) -std=gnu99 -isysroot $(SDK)
	ARCH_LIBS_C :=
	ARCH_LIBS_RT :=
	ARCH_LIBS_M :=

	define BUILD_ARCH
		OBJS_GENERIC_$(1) := $$(patsubst %.c,$(OBJDIR)/$(1)/%.o,$$(GENERIC_SRCS))
		OBJS_SYSDEP_$(1) := $$(patsubst %.c,$(OBJDIR)/$(1)/%.o,$$(SYSDEP_SRCS))
		OBJS_RT_$(1) := $$(patsubst %.c,$(OBJDIR)/$(1)/%.o,$$(RT_SRCS))
		OBJS_MATH_$(1) := $$(patsubst %.c,$(OBJDIR)/$(1)/%.o,$$(MATH_SRCS))
		
		LIB_C_$(1) := $(OBJDIR)/libSystem-$(1).a
		LIB_RT_$(1) := $(OBJDIR)/librt-$(1).a
		LIB_M_$(1) := $(OBJDIR)/libm-$(1).a
		
		ARCH_LIBS_C += $$(LIB_C_$(1))
		ARCH_LIBS_RT += $$(LIB_RT_$(1))
		ARCH_LIBS_M += $$(LIB_M_$(1))
		
		$$(LIB_C_$(1)): $$(OBJS_GENERIC_$(1)) $$(OBJS_SYSDEP_$(1))
			@echo "  AR   [$(1)] $$@"
			@mkdir -p $$(dir $$@)
			$$(Q)$(AR) rcs $$@ $$^

		$$(LIB_RT_$(1)): $$(OBJS_RT_$(1))
			@echo "  AR   [$(1)] $$@"
			@mkdir -p $$(dir $$@)
			$$(Q)$(AR) rcs $$@ $$^

		$$(LIB_M_$(1)): $$(OBJS_MATH_$(1))
			@echo "  AR   [$(1)] $$@"
			@mkdir -p $$(dir $$@)
			$$(Q)$(AR) rcs $$@ $$^

		$$(OBJS_GENERIC_$(1)) $$(OBJS_RT_$(1)) $$(OBJS_MATH_$(1)): $(OBJDIR)/$(1)/%.o: %.c
			@echo "  CC   [$(1)] $$<"
			@mkdir -p $$(dir $$@)
			$$(Q)$(CC) $$(BASE_CFLAGS) -target $(1)-apple-ios6.1 -c $$< -o $$@

		$$(OBJS_SYSDEP_$(1)): $(OBJDIR)/$(1)/%.o: %.c
			@echo "  CC   [$(1)] $$<"
			@mkdir -p $$(dir $$@)
			$$(Q)$(CC) $$(BASE_CFLAGS) -target $(1)-apple-ios6.1 -c $$< -o $$@
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
# --- NATIVE BUILD ---
# Обратите внимание: присвоения переменных БЕЗ отступов табами
AR = ar
PIC = -fPIC
# Путь к include clang может потребоваться скорректировать под вашу систему
BASE_CFLAGS = $(CPPFLAGS) $(CFLAGS) -I$(INCDIR) -Os $(PIC) -std=gnu99 -nostdinc -I/usr/share/llvm/lib/clang/6.0.1/include

OBJS_GENERIC := $(patsubst %.c,$(OBJDIR)/%.o,$(GENERIC_SRCS))
OBJS_SYSDEP := $(patsubst %.c,$(OBJDIR)/%.o,$(SYSDEP_SRCS))
OBJS_RT := $(patsubst %.c,$(OBJDIR)/%.o,$(RT_SRCS))
OBJS_MATH := $(patsubst %.c,$(OBJDIR)/%.o,$(MATH_SRCS))

$(TARGET_LIBC): $(OBJS_GENERIC) $(OBJS_SYSDEP)
	@echo "  AR   $@"
	$(Q)$(AR) rcs $@ $^

$(TARGET_LIBRT): $(OBJS_RT)
	@echo "  AR   $@"
	$(Q)$(AR) rcs $@ $^

$(TARGET_LIBM): $(OBJS_MATH)
	@echo "  AR   $@"
	$(Q)$(AR) rcs $@ $^

# Правило для math, runtime и sysindep
$(OBJS_GENERIC) $(OBJS_RT) $(OBJS_MATH): $(OBJDIR)/%.o: %.c
	@echo "  CC   $<"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(BASE_CFLAGS) -c $< -o $@

# Правило для sysdep
$(OBJS_SYSDEP): $(OBJDIR)/%.o: %.c
	@echo "  CC   $<"
	@mkdir -p $(dir $@)
	$(Q)$(CC) $(BASE_CFLAGS) -c $< -o $@

endif

clean:
	@echo "  CLEAN"
	$(Q)rm -rf $(TARGET_LIBC) $(TARGET_LIBRT) $(TARGET_LIBM) $(OBJDIR)
