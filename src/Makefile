CC = clang
INCDIR = ../include
OBJDIR = obj
TARGET_LIB = libSystem.a

REGULAR_SOURCES = $(wildcard c/stdlib/*.c c/string/*.c c/unistd/*.c c/stdio/*.c c/runtime/*.c c/sys/mman/*.c c/ctype/*.c)
SPECIAL_SOURCES = arm/syscall.c

ifneq ($(VERBOSE), 1)
  Q = @
endif

.PHONY: all
all: $(TARGET_LIB)

ifeq ($(CROSS), 1)
  ifndef SDK
    $(error SDK path must be set for cross-compilation. e.g. make CROSS=1 SDK=~/iPhoneOS6.1.sdk)
  endif

  ARCHES ?= armv7
  comma := ,
  empty :=
  space := $(empty) $(empty)
  ARCH_LIST = $(subst $(comma),$(space),$(ARCHES))

  ALL_OBJECTS := $(foreach arch, $(ARCH_LIST), \
    $(patsubst %.c,$(OBJDIR)/$(arch)/%.o,$(REGULAR_SOURCES)) \
    $(patsubst %.c,$(OBJDIR)/$(arch)/%.o,$(SPECIAL_SOURCES)) \
  )

  AR = llvm-ar
  LIPO = llvm-lipo
  ARFLAGS = rcs
  ARCH_LIBS = $(foreach arch, $(ARCH_LIST), $(OBJDIR)/libSystem-$(arch).a)

  .PHONY: all-objects
  all-objects: $(ALL_OBJECTS)

  $(TARGET_LIB): $(ARCH_LIBS)
	@echo "  LIPO Creating universal library $@"
	$(Q)$(LIPO) -create -output $@ $^

  define ARCH_RULES
    REGULAR_OBJECTS_$(1) = $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(REGULAR_SOURCES))
    SPECIAL_OBJECTS_$(1) = $(patsubst %.c,$(OBJDIR)/$(1)/%.o,$(SPECIAL_SOURCES))
    OBJECTS_$(1) = $$(REGULAR_OBJECTS_$(1)) $$(SPECIAL_OBJECTS_$(1))

    $(OBJDIR)/libSystem-$(1).a: $$(OBJECTS_$(1)) | all-objects
	    @echo "  AR   [$(1)] $$@"
	    $$(Q)$(AR) $(ARFLAGS) $$@ $$^

    CFLAGS_$(1) = -I$(INCDIR) -O2 -fPIC -std=c99 -isysroot $(SDK) -target $(1)-apple-ios6.1
    SYSCALL_CFLAGS_$(1) = $$(CFLAGS_$(1)) -marm

    $(OBJDIR)/$(1)/%.o: %.c
	    @mkdir -p $$(dir $$@)
	    @echo "  CC   [$(1)] $$<"
	    $$(Q)$(CC) $$(CFLAGS_$(1)) -c $$< -o $$@

    $(OBJDIR)/$(1)/arm/syscall.o: arm/syscall.c
	    @mkdir -p $$(dir $$@)
	    @echo "  CC   [$(1)] $$<"
	    $$(Q)$(CC) $$(SYSCALL_CFLAGS_$(1)) -c $$< -o $$@
  endef
  $(foreach arch,$(ARCH_LIST),$(eval $(call ARCH_RULES,$(arch))))

else	
  AR = ar
  ARFLAGS = rcs
  MAIN_CFLAGS = -I$(INCDIR) -O2 -fPIC -I/usr/share/llvm/lib/clang/6.0.1/include -std=c99
  CFLAGS = $(MAIN_CFLAGS) -nostdinc
  SYSCALL_CFLAGS = $(MAIN_CFLAGS) -marm
  REGULAR_OBJECTS = $(patsubst %.c, $(OBJDIR)/%.o, $(REGULAR_SOURCES))
  SPECIAL_OBJECTS = $(OBJDIR)/arm/syscall.o
  OBJECTS = $(REGULAR_OBJECTS) $(SPECIAL_OBJECTS)
  
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
endif

.PHONY: clean
clean:
	@echo "  CLEAN"
	$(Q)rm -rf $(TARGET_LIB) $(OBJDIR) ../ext/printf/tmp
