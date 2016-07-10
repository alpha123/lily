#ifndef LILY_API_VALUE_OPS_H
# define LILY_API_VALUE_OPS_H

# include "lily_core_types.h"

/** General value operations **/

void lily_deref(lily_value *);
void lily_assign_value(lily_value *, lily_value *);
void lily_assign_value_noref(lily_value *, lily_value *);
lily_value *lily_copy_value(lily_value *);
int lily_eq_value(struct lily_vm_state_ *, lily_value *, lily_value *);

/** The functions and the MOVE_* flags form the move api. This api is used to
    push values back into lily_values so that they're visible in Lily space. **/

void lily_move_boolean(lily_value *, int64_t);
void lily_move_double(lily_value *, double);
void lily_move_dynamic(lily_value *, lily_dynamic_val *);
void lily_move_enum_f(uint32_t, lily_value *, lily_instance_val *);
void lily_move_file(lily_value *, lily_file_val *);
void lily_move_foreign_f(uint32_t, lily_value *, lily_foreign_val *);
void lily_move_function_f(uint32_t, lily_value *, lily_function_val *);
void lily_move_hash_f(uint32_t, lily_value *, lily_hash_val *);
void lily_move_instance_f(uint32_t, lily_value *, lily_instance_val *);
void lily_move_integer(lily_value *, int64_t);
void lily_move_list_f(uint32_t, lily_value *, lily_list_val *);
void lily_move_string(lily_value *, lily_string_val *);
void lily_move_tuple_f(uint32_t, lily_value *, lily_list_val *);

/* The flags of a value include what kind of value that it is. The above moves
   that have _f as a suffix take a flag as their first parameter to determine
   what gc/deref information a value should have.
   Tagging is done independent of move, so values that are going to be tagged
   should be moved no gc prior to tagging. That will ensure that the garbage
   collector has only one state to deal with at a time (either speculative or
   tagged, but never both).
   Generally speaking, it is preferable to use the moves above in combination
   with the values below to shuffle values to and fro within the interpreter. */

/* This value can be ref'd/deref'd, but does not contain any tagged data inside
   of it. */
#define MOVE_DEREF_NO_GC          VAL_IS_DEREFABLE
/* This value can be ref'd/deref'd, and may contain tagged data inside. Use this
   if you're not sure (such as with a `List[A]`. */
#define MOVE_DEREF_SPECULATIVE    (VAL_IS_DEREFABLE | VAL_IS_GC_SPECULATIVE)
/* This value should not be ref'd/deref'd. This is typically for the default
   value of a variant. Use this if you're certain that the type of the value
   has instances where it could have sweepable data. Examples of this include
   `None` for values like `List[Option[Integer]]`, because `Some(Integer)`
   cannot have circular data inside. */
#define MOVE_SHARED_NO_GC         0
/* This value should not be ref'd/deref'd. This is the more common case for
   default values of a variant. Use this if you're not sure that the type might
   have sweepable data. `List[Option[A]]` is an example where the `None` is not
   interesting, but might be replaced with `Some(Dynamic)` which needs to be
   swept through. */
#define MOVE_SHARED_SPECULATIVE   (VAL_IS_GC_SPECULATIVE)

/** These functions create new raw values. Since these are raw values, Lily has
    no internal knowledge that they exist. Newly-made values must be put into
    Lily space using the above-mentioned move functions. **/

lily_value *lily_new_empty_value(void);

lily_dynamic_val *lily_new_dynamic_val(void);

lily_file_val *lily_new_file_val(FILE *, const char *);

/* These three functions are a special case. The first two are meant to be used
   at parse-time (they may not work at vm-time). The last may work at vm-time,
   but is used primarily for closure allocation. */
lily_function_val *lily_new_native_function_val(char *, char *);
lily_function_val *lily_new_foreign_function_val(lily_foreign_func,
        const char *, const char *);
lily_function_val *lily_new_function_copy(lily_function_val *);

lily_list_val *lily_new_list_val(void);
lily_hash_val *lily_new_hash_val(void);
lily_instance_val *lily_new_instance_val(void);

lily_value *lily_new_string(const char *);
lily_value *lily_new_string_take(char *);
lily_string_val *lily_new_raw_string(const char *);
lily_string_val *lily_new_raw_string_sized(const char *, int);

#define lily_arg_value(vm, code, index) vm->vm_regs[code[index]]
#define lily_arg_double(vm, code, index) vm->vm_regs[code[index]]->value.doubleval
#define lily_arg_file(vm, code, index) vm->vm_regs[code[index]]->value.file
#define lily_arg_file_raw(vm, code, index) vm->vm_regs[code[index]]->value.file->inner_file
#define lily_arg_hash(vm, code, index) vm->vm_regs[code[index]]->value.hash
#define lily_arg_instance(vm, code, index) vm->vm_regs[code[index]]->value.instance
#define lily_arg_integer(vm, code, index) vm->vm_regs[code[index]]->value.integer
#define lily_arg_list(vm, code, index) vm->vm_regs[code[index]]->value.list
#define lily_arg_string(vm, code, index) vm->vm_regs[code[index]]->value.string
#define lily_arg_string_raw(vm, code, index) vm->vm_regs[code[index]]->value.string->string

#define lily_return_boolean(vm, b) lily_move_boolean(vm->vm_regs[code[0]], b)
#define lily_return_double(vm, d) lily_move_double(vm->vm_regs[code[0]], d)
#define lily_return_tag_dynamic(vm, d) { lily_move_dynamic(vm->vm_regs[code[0]], d); lily_tag_value(vm, vm->vm_regs[code[0]]); }
#define lily_return_hash(vm, h) lily_move_hash_f(MOVE_DEREF_SPECULATIVE, vm->vm_regs[code[0]], h)
#define lily_return_integer(vm, i) lily_move_integer(vm->vm_regs[code[0]], i)
#define lily_return_file(vm, f) lily_move_file(vm->vm_regs[code[0]], f)
#define lily_return_filled_variant(vm, e) lily_move_enum_f(MOVE_DEREF_SPECULATIVE, vm->vm_regs[code[0]], e)
#define lily_return_list(vm, l) lily_move_list_f(MOVE_DEREF_SPECULATIVE, vm->vm_regs[code[0]], l)
#define lily_return_string(vm, str) lily_move_string(vm->vm_regs[code[0]], str)
#define lily_return_tuple(vm, t) lily_move_tuple_f(MOVE_DEREF_SPECULATIVE, vm->vm_regs[code[0]], t)
#define lily_return_value(vm, v) lily_assign_value(vm->vm_regs[code[0]], v)
#define lily_return_value_noref(vm, v) lily_assign_value_noref(vm->vm_regs[code[0]], v)
#define lily_return_empty_variant(vm, v) \
lily_move_enum_f(MOVE_SHARED_NO_GC, vm->vm_regs[code[0]], v)

char *lily_bytestring_get_raw(lily_string_val *);
int lily_bytestring_length(lily_string_val *);

char *lily_string_get_raw(lily_string_val *);
int lily_string_length(lily_string_val *);

lily_value *lily_instance_get(lily_instance_val *, int);
lily_list_val *lily_new_list_of_n(int);

void lily_list_set_string(lily_list_val *, int, lily_string_val *);

void lily_variant_set(lily_instance_val *, int, lily_value *);
void lily_variant_set_integer(lily_instance_val *, int, int64_t);
void lily_variant_set_string(lily_instance_val *, int, lily_string_val *);

lily_instance_val *lily_build_new_some(void);
lily_instance_val *lily_build_new_left(void);
lily_instance_val *lily_build_new_right(void);

/* An id is assigned to every variant within an enum. That id is used along with
   the id of an enum for printing the variant. The values below are used to
   create variants of Option and Either, respectively. */

#define SOME_VARIANT_ID 0
#define NONE_VARIANT_ID 1

lily_instance_val *lily_new_some(lily_value *);
lily_instance_val *lily_get_none(struct lily_vm_state_ *);

#define RIGHT_VARIANT_ID 0
#define LEFT_VARIANT_ID  1

lily_instance_val *lily_new_left(lily_value *);
lily_instance_val *lily_new_right(lily_value *);

#endif
