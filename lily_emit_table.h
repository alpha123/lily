#ifndef LILY_EMIT_TABLE_H
# define LILY_EMIT_TABLE_H

# include "lily_expr_op.h"
# include "lily_opcode.h"

/* The emitter table is used to get the opcode for 'generic' binary expressions.
   So far, this just means plus.
   This depends on a couple assumptions:
   * The expr_assign opcode is last.
   * Integer, number, and str have id 0, 1, and 2 respectively.
   Access it by [opcode][lhs id][rhs id]. -1 means the operation is not
   supported. */
static const int generic_binop_table[2][3][3] =
{
    {
        {o_integer_add, o_number_add, -1},
        {o_number_add, o_number_add, -1},
        {-1, -1, -1}
    },
    {
        {o_integer_minus, o_number_minus, -1},
        {o_number_minus, o_number_minus, -1},
        {-1, -1, -1}
    }
};
#endif