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
 *
 *
 */

/* ==== To add ====
 * Peephole shifting in place of multiplication.
 */

// ==== Statics ====
static std::vector<inter> *file;
static std::stack<inter*> operand_stack;

namespace intermediate_optimization
{
    // ==== Main function ====
    // This takes in a pointer to the static intermediate file and optimizes it.
    void optimzize_file(std::vector<inter> *_file)
    {
        file = _file;
    }
    
    // ==== Helper inline functions ====
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
            break;
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
        
        // This should replace the operation with the result and remove the
        // operands.
    }
}
