#ifndef MACH_VM_STATISTICS_H
#define MACH_VM_STATISTICS_H

#include <features.h>

#if defined(_MACH_SOURCE)

# include <mach/machine/vm_types.h>

# if defined(_C_SOURCE)

struct vm_statistics {
    natural_t free_count;
    natural_t active_count;
    natural_t inactive_count;
    natural_t wire_count;
    natural_t zero_fill_count;
    natural_t reactivations;
    natural_t pageins;
    natural_t pageouts;
    natural_t faults;
    natural_t cow_faults;
    natural_t lookups;
    natural_t hits;
    natural_t purgeable_count;
    natural_t purges;
    natural_t speculative_count;
};

typedef struct vm_statistics *vm_statistics_t;
typedef struct vm_statistics vm_statistics_data_t;

struct vm_statistics64 {
    natural_t free_count;
    natural_t active_count;
    natural_t inactive_count;
    natural_t wire_count;
    uint64_t zero_fill_count;
    uint64_t reactivations;
    uint64_t pageins;
    uint64_t pageouts;
    uint64_t faults;
    uint64_t cow_faults;
    uint64_t lookups;
    uint64_t hits;
    uint64_t purges;
    natural_t purgeable_count;
    natural_t speculative_count;

} __attribute__((aligned(8)));

typedef struct vm_statistics64 *vm_statistics64_t;
typedef struct vm_statistics64 vm_statistics64_data_t;

#  define VM_STATISTICS_TRUNCATE_TO_32_BIT(value)                                                  \
      ((uint32_t)(((value) > UINT32_MAX) ? UINT32_MAX : (value)))

struct vm_extmod_statistics {
    int64_t task_for_pid_count;
    int64_t task_for_pid_caller_count;
    int64_t thread_creation_count;
    int64_t thread_creation_caller_count;
    int64_t thread_set_state_count;
    int64_t thread_set_state_caller_count;
} __attribute__((aligned(8)));

typedef struct vm_extmod_statistics *vm_extmod_statistics_t;
typedef struct vm_extmod_statistics vm_extmod_statistics_data_t;

# endif /* _C_SOURCE */

# define VM_PAGE_QUERY_PAGE_PRESENT      0x1
# define VM_PAGE_QUERY_PAGE_FICTITIOUS   0x2
# define VM_PAGE_QUERY_PAGE_REF          0x4
# define VM_PAGE_QUERY_PAGE_DIRTY        0x8
# define VM_PAGE_QUERY_PAGE_PAGED_OUT    0x10
# define VM_PAGE_QUERY_PAGE_COPIED       0x20
# define VM_PAGE_QUERY_PAGE_SPECULATIVE  0x40
# define VM_PAGE_QUERY_PAGE_EXTERNAL     0x80
# define VM_PAGE_QUERY_PAGE_CS_VALIDATED 0x100
# define VM_PAGE_QUERY_PAGE_CS_TAINTED   0x200

# define VM_FLAGS_FIXED     0x0000
# define VM_FLAGS_ANYWHERE  0x0001
# define VM_FLAGS_PURGABLE  0x0002
# define VM_FLAGS_NO_CACHE  0x0010
# define VM_FLAGS_OVERWRITE 0x4000

# define VM_FLAGS_SUPERPAGE_MASK  0x70000
# define VM_FLAGS_SUPERPAGE_SHIFT 16

# define SUPERPAGE_NONE              0
# define SUPERPAGE_SIZE_ANY          1
# define VM_FLAGS_SUPERPAGE_NONE     (SUPERPAGE_NONE << VM_FLAGS_SUPERPAGE_SHIFT)
# define VM_FLAGS_SUPERPAGE_SIZE_ANY (SUPERPAGE_SIZE_ANY << VM_FLAGS_SUPERPAGE_SHIFT)
# define SUPERPAGE_SIZE_2MB          2
# define VM_FLAGS_SUPERPAGE_SIZE_2MB (SUPERPAGE_SIZE_2MB << VM_FLAGS_SUPERPAGE_SHIFT)

# define VM_FLAGS_ALIAS_MASK              0xFF000000
# define VM_GET_FLAGS_ALIAS(flags, alias) (alias) = ((flags) & VM_FLAGS_ALIAS_MASK) >> 24
# define VM_SET_FLAGS_ALIAS(flags, alias)                                                          \
     (flags) = (((flags) & ~VM_FLAGS_ALIAS_MASK) | (((alias) & ~VM_FLAGS_ALIAS_MASK) << 24))

# define VM_FLAGS_USER_ALLOCATE                                                                    \
     (VM_FLAGS_FIXED | VM_FLAGS_ANYWHERE | VM_FLAGS_PURGABLE | VM_FLAGS_NO_CACHE |                 \
      VM_FLAGS_OVERWRITE | VM_FLAGS_SUPERPAGE_MASK | VM_FLAGS_ALIAS_MASK)
# define VM_FLAGS_USER_MAP   VM_FLAGS_USER_ALLOCATE
# define VM_FLAGS_USER_REMAP (VM_FLAGS_FIXED | VM_FLAGS_ANYWHERE | VM_FLAGS_OVERWRITE)

# define VM_MEMORY_MALLOC                              1
# define VM_MEMORY_MALLOC_SMALL                        2
# define VM_MEMORY_MALLOC_LARGE                        3
# define VM_MEMORY_MALLOC_HUGE                         4
# define VM_MEMORY_SBRK                                5
# define VM_MEMORY_REALLOC                             6
# define VM_MEMORY_MALLOC_TINY                         7
# define VM_MEMORY_MALLOC_LARGE_REUSABLE               8
# define VM_MEMORY_MALLOC_LARGE_REUSED                 9
# define VM_MEMORY_ANALYSIS_TOOL                       10
# define VM_MEMORY_MACH_MSG                            20
# define VM_MEMORY_IOKIT                               21
# define VM_MEMORY_STACK                               30
# define VM_MEMORY_GUARD                               31
# define VM_MEMORY_SHARED_PMAP                         32
# define VM_MEMORY_DYLIB                               33
# define VM_MEMORY_OBJC_DISPATCHERS                    34
# define VM_MEMORY_UNSHARED_PMAP                       35
# define VM_MEMORY_APPKIT                              40
# define VM_MEMORY_FOUNDATION                          41
# define VM_MEMORY_COREGRAPHICS                        42
# define VM_MEMORY_CORESERVICES                        43
# define VM_MEMORY_CARBON                              VM_MEMORY_CORESERVICES
# define VM_MEMORY_JAVA                                44
# define VM_MEMORY_ATS                                 50
# define VM_MEMORY_LAYERKIT                            51
# define VM_MEMORY_CGIMAGE                             52
# define VM_MEMORY_TCMALLOC                            53
# define VM_MEMORY_COREGRAPHICS_DATA                   54
# define VM_MEMORY_COREGRAPHICS_SHARED                 55
# define VM_MEMORY_COREGRAPHICS_FRAMEBUFFERS           56
# define VM_MEMORY_COREGRAPHICS_BACKINGSTORES          57
# define VM_MEMORY_COREGRAPHICS_MISC                   VM_MEMORY_COREGRAPHICS
# define VM_MEMORY_DYLD                                60
# define VM_MEMORY_DYLD_MALLOC                         61
# define VM_MEMORY_SQLITE                              62
# define VM_MEMORY_JAVASCRIPT_CORE                     63
# define VM_MEMORY_JAVASCRIPT_JIT_EXECUTABLE_ALLOCATOR 64
# define VM_MEMORY_JAVASCRIPT_JIT_REGISTER_FILE        65
# define VM_MEMORY_GLSL                                66
# define VM_MEMORY_OPENCL                              67
# define VM_MEMORY_COREIMAGE                           68
# define VM_MEMORY_WEBCORE_PURGEABLE_BUFFERS           69
# define VM_MEMORY_IMAGEIO                             70
# define VM_MEMORY_COREPROFILE                         71
# define VM_MEMORY_ASSETSD                             72
# define VM_MEMORY_APPLICATION_SPECIFIC_1              240
# define VM_MEMORY_APPLICATION_SPECIFIC_16             255

# define VM_MAKE_TAG(tag) ((tag) << 24)

#endif /* _MACH_SOURCE */

#endif /* !MACH_VM_STATISTICS_H */
