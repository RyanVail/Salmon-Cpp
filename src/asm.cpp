/*
  This file handles turning lines into ASM - Ryan Vail 2022 Sep. 15th
*/

struct asm_settings
{
  debug = false; // If the asm has debug things attachted
  std::vector<int> useable = {0,1,2,3} // The R0-R3 registers that are useable
  int main[] = [0,1] // The main two registers used of doing computations
}

// TODO: Find a better way of doing this
struct asm_single_expression
{
  int computation; // If this node is a computation otherwise 0 and 1 is reserved for commas in function inputs and list creation
  variable var; // The variable of this node
  function function_call; // The function we are calling
  int const_value; // The value of this node if it is constant
}

// This puts together the assembly of some computation
// TODO: This function can only do operations between u32 and char
// TODO: It would be very hard to do multiline optimizations with this function find a better way of doing this
void expression_to_asm(global &globals, std::vector<std::string> &file, int location)
{
  // This goes through and counts parentheses until it reaches a ';'
  std::vector<std::pair<int,int>> paretheses_counter; // The location of the paretheses then the counter
  int counter = 0;
  for (int i=location; i < file.size() || file[i] == ";"; i++)
  {
    if (file[i]) == "(" 
    { 
      counter++;
      paretheses_counter.push_back(std::make_pair(i, counter));
    }
    if (file[i]) == ")" { counter--; }
    if (counter < 0) { throw std::runtime_error("Uneven amount of closing and opening parentheses."); }
  }
  
  for (int i=0; i < paretheses_counter; i++)
  {
    // This goes through and finds the value with the greatest count
    pair<int,int> best;
    best.first = -1;
    best.second = 65536; // TODO: find a better way of doing this
    for (int x=0; x < paretheses_counter; x++)
    {
      if (paretheses_counter[x].second < best.second) { best = paretheses_counter[x]; }
    }
    if (best.first == -1) { throw std::runtime_error("Something went wrong in the function expression_to_asm in the file asm.cpp."); }
    
    // This evaluates what's between those parentheses first
    location = best.first;
    
	// EX. With badly optimized ASM:
    // u32 a = return_this_value(3 + 3) + 5;
		// 3 + 3 -> return_this_value(6) -> 6 + 5
			// mov r0, #3
	  		// mov r1, #3 
	 		// add r0, r1
	  		// bl return_this_value
    		// mov r1, #5
	  		// add r0, r1
	  
    // This should unwrap the expression removing paretheses and put it in the correct order using asm_single_expression
    // TODO: THIS SHOULD ALSO CHECK FOR BRACKETS AS WELL AS PARETHESES!
	  
    // If the variable has a '(' right after it without an operation seperating the two were calling a function
     	 // If the variable isn't just letters we send an error
    // If the variable has a '[' right after it we are getting a certain index of an array
      	// We should check if that array itself and the array index is valid ie. in scope and of the valid length
	// If the block we are working on only have commas we look for a function to the left of the '(' if we don't find one we send an error
  }
  
  /*
  bool had_last_value = false;
	int last_value = 0;
	for (int i=location; i < file.size(); i++)
	{
		if (!is_str_num(file[i]))
		{
			if (file[i] == "]") { return last_value; }
			if (file[i] == "(" || file[i] == ")") { throw std::runtime_error("parentheses not yet supported sorry. TODO: add parenthese support. There, now it's your problem."); }
			else if (check_expression(file[i], file[i+1]) == -1) { throw std::runtime_error("Unknow expression."); }
			else
			{
				if (!had_last_value) { throw std::runtime_error("Expected const before expression."); }
				last_value = compute_expression(last_value, check_expression(file[i], file[i+1]), get_str_num(file[i+1+(check_expression(file[i], file[i+1]) > 6)]));
				i += 1 + (check_expression(file[i], file[i+1]) > 6);
			}
		}
		else
		{
			if (had_last_value) { throw std::runtime_error("Expected an expression."); }
			had_last_value = true;
			last_value = get_str_num(file[i]);
		}
	}
	return last_value;
  */
}
