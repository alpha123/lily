#ifndef LILY_SEED_H
# define LILY_SEED_H

typedef enum {
    dyna_class,
    dyna_function,
    dyna_var,
    dyna_exception,
    dyna_enum,
    dyna_variant
} dyna_type;

typedef const struct lily_base_seed_ {
    const void *next;
    char *name;
    uint64_t seed_type;
} lily_base_seed;

typedef const struct {
    const void *next;
    char *name;
    uint64_t seed_type;
    char *type;
} lily_var_seed;

typedef const struct {
    const void *next;
    char *name;
    uint64_t seed_type;
    char *func_definition;
    lily_foreign_func func;
} lily_func_seed;

typedef const struct {
    const void *next;
    char *name;
    uint64_t seed_type;
    uint32_t is_refcounted;
    uint32_t generic_count;
    const void *dynaload_table;
    class_destroy_func destroy_func;
} lily_class_seed;

typedef const struct {
    const void *next;
    char *name;
    uint64_t seed_type;
    uint32_t generic_count;
    uint32_t pad;
    const void *dynaload_table;
    /* Enums use tuple's versions of mark, eq, and destroy, and thus do not need
       them specified here. */
} lily_enum_seed;

typedef const struct {
    const void *next;
    char *name;
    uint64_t seed_type;
    char *body;
    char *enum_name;
} lily_variant_seed;

#endif
