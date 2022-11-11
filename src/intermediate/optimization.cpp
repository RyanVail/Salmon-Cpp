/* ==== Optimization pipeline ====
 *
 * 1. This makes another pass through the intermediate stack and if there is an
 * operation that can be done in compile time it does it.
 */

#include<intermediate/optimization.hpp>
#include<vector>
#include<intermediate/intermediate.hpp>
#include<typedefs.hpp>
#include<stack>

/* ==== Current optimiztions ====
 * Constant evuations.
 * Dead if and while statment removal.
 * Always do constant if.
 */

/* ==== To add ====
 * Dead code removal.
 * Peephole shifting in place of multiplication.
 * Peephole "MEM_ADDRS" then "GET" removal.
 * Register substitution.
 * Const variables.
 */

// ==== Statics ====
static std::vector<inter> *file;
// TODO: It would be better to replace the file with the output file then 
// remove the extra.
static std::vector<inter> output_file;
static std::stack<inter*> operand_stack;
// This is a stack of the intermediate ids of statments to remove like "IF_END".
static std::stack<u8> statments_to_remove;

namespace intermediate_optimization
{
    // ==== Helper functions ====
    inline u8 get_operation_count(inter &_inter);
    inline i32 do_double_operation(i32 _first_value, i32 _second_value, u8 _inter_id);
    inline bool is_const(inter _inter);
    inline i32 do_single_operation(i32 _value, u8 _inter_id);
    inline void on_operation(inter *inter_a, inter *inter_b, inter *operation);

    // ==== Main function ====
    void optimzize(std::vector<inter> *_file)
    {
        file = _file;

        for (inter current_inter = *_file.begin(); current_inter != *_file.end(); current_inter++)
        {
            u8 _count = get_operation_count(*current_inter);
            if (_count)
            {
                // If we don't have a constant we save it how it is.
                if (!is_const(*operand_stack.top()))
                    if (_count == 2)
                        goto save_operands_and_inter;
                    goto save_operand_and_inter;

                if (_count == 2)
                {
                    inter _first_inter = operand_stack.top();
                    operand_stack.pop();

                    if (!is_const(*operand_stack.top()))
                    {
                        operand_stack.push(_first_inter);
                        goto save_operand_and_inter
                    }

                    on_operation(_first_inter, operand_stack.top(), current_inter);
                    operand_stack.pop();
                }
                if (_count == 1)
                {
                    on_operation(operand_stack.top(), nullptr, current_inter);
                    operand_stack.pop();
                }
                continue;
            }
            if (current_inter->id == FUNC_CALL)
            {
                // TODO: Add some logic
            }

            // If we hit a statment and the top of the stack is a const we compute
            // it here instead of during runtime.
            if (current_inter->id == WHILE_BEGIN || current_inter->id == IF_BEGIN)
            {
                if (!is_const(*operand_stack.top()))
                    goto save_operands_and_inter;

                // If we are always doing an if statment we don't add the
                // "IF_BEGIN" and add the "IF_END" to the statments to remove.
                if (operand_stack.top()->value >= 1)
                {
                    if (current_inter->id == IF_BEGIN)
                        statments_to_remove.push(current_inter->id + 1);

                    goto save_inter;
                }
                else
                {
                    u8 _end_statment_id = current_inter->id + 1;
                    // If we are not doing the if or while statment we skip to
                    // the end statment
                    while (true)
                    {
                        if (current_inter == file.end())
                            error::send_error("Error: reached the end of the file while removing a dead statment.");
                        if (current_inter->id == _end_statment_id)
                            break;
                    }
                    continue;
                }

            }

            goto save_inter;

            // ==== Clean ups ====
            save_operands_and_inter:
                output_file.push_back(*operand_stack.top());
                operand_stack.pop();
            save_operand_and_inter:
                output_file.push_back(*operand_stack.top());
                operand_stack.pop();
            save_inter:
                output_file.push_back(current_inter);
        }
    }

    // ==== Helper function definitions  ====
    // This returns the number of values the inter is popping off the stack.
    inline u8 get_operation_count(inter &_inter)
    {
        if (_inter.id >= NOT && _inter.id <= DECRAMENT)
            return 1;
        if (_inter.id >= AND && _inter.id <= MUL)
            return 2;
        return 0;
    }
    // This precomputes an operation with two opearnds
    inline i32 do_double_operation(i32 _first_value, i32 _second_value, u8 _inter_id)
    {
        switch(_inter_id)
        {
        case AND:
            return _first_value & _second_value;
            break;
        case OR:
            return _first_value | _second_value;
            break;
        case XOR:
            return _first_value ^ _second_value;
            break;
        case LSL:
            return _first_value << _second_value;
            break;
        case LSR:
            return _first_value >> _second_value;
            break;
        case IS_EQUAL:
            return _first_value == _second_value;
            break;
        case LESS:
            return _first_value < _second_value;
            break;
        case LESS_EQUAL:
            return _first_value <= _second_value;
            break;
        case GREATER:
            return _first_value > _second_value;
            break;
        case GREATER_EQUAL:
            return _first_value >= _second_value;
            break;
        case ADD:
            return _first_value + _second_value;
            break;
        case SUB:
            return _first_value - _second_value;
            break;
        case MUL:
            return _first_value * _second_value;
        }
    }
    inline bool is_const(inter _inter)
    {
        if (_inter.id == CONST)
            return true;
        return false;
    }
    // This precomputes an operation with one operand
    inline i32 do_single_operation(i32 _value, u8 _inter_id)
    {
        switch(_inter_id)
        {
        case NOT:
            return !_value;
            break;
        case INCRAMENT:
            return _value++;
            break;
        case DECRAMENT:
            return _value--;
        }
    }
    // This is called when there is an operation
    inline void on_operation(inter *inter_a, inter *inter_b, inter *operation)
    {
        i32 _result;

        if (inter_b == nullptr)
            _result = do_single_operation(inter_a->value, operation->id);
        else
            _result = do_double_operation(inter_a->value, inter_b->value, operation->id);
        
        output_file.push_back(inter(CONST, _result, 8));
    }
}
