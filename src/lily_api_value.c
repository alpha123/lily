#include "lily_value_structs.h"
#include "lily_vm.h"

#include "lily_api_value_ops.h"

#define DEFINE_SETTERS(name, action, ...) \
void lily_##name##_boolean(__VA_ARGS__, int v) \
{ lily_move_boolean(source->action, v); } \
void lily_##name##_double(__VA_ARGS__, double v) \
{ lily_move_double(source->action, v); } \
void lily_##name##_empty_variant(__VA_ARGS__, lily_instance_val * v) \
{ lily_move_enum_f(MOVE_SHARED_NO_GC, source->action, v); } \
void lily_##name##_file(__VA_ARGS__, lily_file_val * v) \
{ lily_move_file(source->action, v); } \
void lily_##name##_foreign(__VA_ARGS__, lily_foreign_val * v) \
{ lily_move_foreign_f(MOVE_DEREF_SPECULATIVE, source->action, v); } \
void lily_##name##_filled_variant(__VA_ARGS__, lily_instance_val * v) \
{ lily_move_enum_f(MOVE_DEREF_SPECULATIVE, source->action, v); } \
void lily_##name##_hash(__VA_ARGS__, lily_hash_val * v) \
{ lily_move_hash_f(MOVE_DEREF_SPECULATIVE, source->action, v); } \
void lily_##name##_instance(__VA_ARGS__, lily_instance_val * v) \
{ lily_move_instance_f(MOVE_DEREF_SPECULATIVE, source->action, v); } \
void lily_##name##_integer(__VA_ARGS__, int64_t v) \
{ lily_move_integer(source->action, v); } \
void lily_##name##_list(__VA_ARGS__, lily_list_val * v) \
{ lily_move_list_f(MOVE_DEREF_SPECULATIVE, source->action, v); } \
void lily_##name##_string(__VA_ARGS__, lily_string_val * v) \
{ lily_move_string(source->action, v); } \
void lily_##name##_tuple(__VA_ARGS__, lily_list_val * v) \
{ lily_move_tuple_f(MOVE_DEREF_SPECULATIVE, source->action, v); } \
void lily_##name##_value(__VA_ARGS__, lily_value * v) \
{ lily_assign_value(source->action, v); } \

DEFINE_SETTERS(variant_set, values[index], lily_instance_val *source, int index)
DEFINE_SETTERS(dynamic_set, inner_value, lily_dynamic_val *source)
DEFINE_SETTERS(return, call_chain->prev->return_target, lily_vm_state *source)


void lily_return_tag_dynamic(lily_vm_state *vm, lily_dynamic_val *v)
{
    lily_move_dynamic(vm->call_chain->prev->return_target, v);
    lily_tag_value(vm, vm->call_chain->prev->return_target);
}

void lily_return_value_noref(lily_vm_state *vm, lily_value *v)
{
    lily_assign_value_noref(vm->call_chain->prev->return_target, v);
}

void lily_result_return(lily_vm_state *vm)
{
    lily_value *r = vm->regs_from_main[vm->num_registers - 1];
    lily_assign_value_noref(vm->call_chain->prev->return_target, r);
    r->flags = 0;
    vm->num_registers--;
}


int lily_arg_count(lily_vm_state *vm)
{
    return vm->call_chain->regs_used;
}

double lily_arg_double(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.doubleval;
}

lily_file_val *lily_arg_file(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.file;
}

FILE *lily_arg_file_raw(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.file->inner_file;
}

lily_function_val *lily_arg_function(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.function;
}

lily_hash_val *lily_arg_hash(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.hash;
}

lily_generic_val *lily_arg_generic(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.generic;
}

lily_instance_val *lily_arg_instance(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.instance;
}

int64_t lily_arg_integer(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.integer;
}

lily_list_val *lily_arg_list(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.list;
}

lily_string_val *lily_arg_string(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.string;
}

const char *lily_arg_string_raw(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index]->value.string->string;
}

lily_value *lily_arg_value(lily_vm_state *vm, int index)
{
    return vm->vm_regs[index];
}


lily_value *lily_result_get(lily_vm_state *vm)
{
    return vm->call_chain->return_target;
}

int lily_result_get_boolean(lily_vm_state *vm)
{
    return vm->call_chain->return_target->value.integer;
}


void lily_drop_value(lily_vm_state *vm)
{
    vm->num_registers--;
    lily_value *z = vm->regs_from_main[vm->num_registers];
    lily_deref(z);
    z->flags = 0;
}
