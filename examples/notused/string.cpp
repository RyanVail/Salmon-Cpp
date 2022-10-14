// This is the string/str data type
struct string || str
{ 
	char[?] self; // This is the value of the string itself which is a vector<char>
	u32 size; // The size of the string

	// This finds and returns the index of a certain character in the string
	fn find(char char_to_find) -> int
	{
		for (int i=0; i < self.size(); i++)
		{
			// Parentheses aren't required
			if self[i] == char_to_find
			{
				return i;
			}
		}
		return -1; // -1 means we weren't able to find the character
	}

	// This means the function is called when a string is added to another string
	#[on_add_with_string]
	fn on_add_with_string(&string self, &string other_string)
	{

	}

	// This is how this type is defined when it's set equal to a char[]
	#[on_define_with_char_array]
	fn on_define_with_char_array()
	{

	}
}

fn main()
{
	string test_string = "asdf"; // Defining the test string
	test_string += 'd'; // Adding a char to it
	println!("{}", test_string); // Printing it
}