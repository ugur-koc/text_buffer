/**
    MathWorks Interview Asn 2, UnitTests.cpp
    Purpose: Unit test cases for available operations

    @author Ugur Koc
    @version 0.1 01/25/19 
*/


#include <assert.h>

#include "../core/Operations.h"

void test_load_good(){
	std::string str;
	TextBuffer tb(str);
	Load op(&tb);
	
	// simple load with existing file
	std::string cmd_args = "aaa.txt";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I love you");

	op.undo(cmd_args); // does nothing
	assert(tb.getContent() == "I love you");
	op.redo(cmd_args); // does nothing
	assert(tb.getContent() == "I love you");
}

void test_load_bad(){
	std::string str;
	TextBuffer tb(str);
	Load op(&tb);

	// load with non existing file
	assert(!op.execute("bbb.txt"));

	// load with non .txt file
	assert(!op.execute("main.cpp"));

}


void test_save_good(){
	std::string str="I love you";
	TextBuffer tb(str);
	Save op(&tb);
	TextBuffer tb2("");
	Load op2(&tb2);

	// simple save with existing file
	std::string cmd_args = "tests/ccc.txt";
	assert(op.execute(cmd_args));
	assert(op2.execute(cmd_args));
	assert(tb2.getContent() == "I love you");

	op.undo(cmd_args); // does nothing
	assert(tb.getContent() == "I love you");
	op.redo(cmd_args); // does nothing
	assert(tb.getContent() == "I love you");
}

void test_save_bad(){
	std::string str="I love you";
	TextBuffer tb(str);
	Save op(&tb);

	// save to non txt extension file
	assert(!op.execute("main.cpp")); // print out warning

	// save to .txt file
	assert(!op.execute(".txt")); // print out warning
}

void test_insert_good(){
	std::string str = "I love you";
	TextBuffer tb(str);
	Insert op(&tb);
	
	// simple insertion with valid arguments
	std::string cmd_args = "2 do ";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I do love you");

	// undo and redo previous insertion
	op.undo(cmd_args);
	assert(tb.getContent() == str);
	op.redo(cmd_args);
	assert(tb.getContent() == "I do love you");
	op.undo(cmd_args); //revert to orig
	
	// insertion to end
	cmd_args = "10  my girl";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I love you my girl");
	
	// insertion to the begining
	cmd_args = "0 Why do ";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "Why do I love you my girl");
}

void test_insert_bad(){
	std::string str = "I love you";
	TextBuffer tb(str);
	Insert op(&tb);
	
	// insertion with negative position
	assert(!op.execute("-2 do ")); // print out warning message
	assert(tb.getContent() == str);

	// insertion with position bigger than buffer lenght
	assert(!op.execute("200 do ")); // print out warning message
	assert(tb.getContent() == str);

	// insertion with non integer position
	assert(!op.execute("pos do ")); // print out warning message
	assert(tb.getContent() == str);

	// insertion with no insertion text
	assert(!op.execute("2")); // print out warning message
	assert(tb.getContent() == str);
}

void test_append_good(){
	std::string str = "I love you";
	TextBuffer tb(str);
	Append op(&tb);
	
	// simple append with valid arguments
	std::string cmd_args = " my girl";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I love you my girl");

	// undo and redo previous append
	op.undo(cmd_args);
	assert(tb.getContent() == str);
	op.redo(cmd_args);
	assert(tb.getContent() == "I love you my girl");
	op.undo(cmd_args); //revert to orig

	// append space
	cmd_args = " ";
	assert(op.execute(cmd_args)); // print out warning message
	assert(tb.getContent() == "I love you ");
	op.undo(cmd_args); //revert to orig
}

void test_append_bad(){
	std::string str = "I love you";
	TextBuffer tb(str);
	Append op(&tb);

	// append empty text
	std::string cmd_args = "";
	assert(!op.execute(cmd_args)); // print out warning message
	assert(tb.getContent() == "I love you");

}

void test_eraseAt_good(){
	std::string str = "I love you";
	TextBuffer tb(str);
	EraseAt op(&tb);
	
	// simple eraseAt with valid arguments
	std::string cmd_args = "2 5";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I you");

	// undo and redo previous eraseAt
	op.undo(cmd_args);
	assert(tb.getContent() == str);
	op.redo(cmd_args);
	assert(tb.getContent() == "I you");
	op.undo(cmd_args); //revert to orig
	
	// eraseAt end
	cmd_args = "6 4";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I love");
	op.undo(cmd_args); //revert to orig

	// eraseAt the begining
	cmd_args = "0 2";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "love you");
}

void test_eraseAt_bad(){
	std::string str = "I love you";
	TextBuffer tb(str);
	EraseAt op(&tb);
	
	// eraseAt negative position
	assert(!op.execute("-2 3")); // print out warning message
	assert(tb.getContent() == str);

	// eraseAt with position bigger than buffer lenght
	assert(!op.execute("200 3")); // print out warning message
	assert(tb.getContent() == str);

	// eraseAt with non integer position and length
	assert(!op.execute("pos do ")); // print out warning message
	assert(tb.getContent() == str);

	// eraseAt with no length
	assert(!op.execute("2")); // print out warning message
	assert(tb.getContent() == str);

	// eraseAt end shorther lenght
	assert(op.execute("6 5")); // print out warning message
	assert(tb.getContent() == "I love");
}

void test_erase_good(){
	std::string str = "I love you";
	TextBuffer tb(str);
	Erase op(&tb);
	
	// simple erase with valid arguments
	std::string cmd_args = "4";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I love");

	// undo and redo previous eraseAt
	op.undo(cmd_args);
	assert(tb.getContent() == str);
	op.redo(cmd_args);
	assert(tb.getContent() == "I love");
	op.undo(cmd_args); //revert to orig
}

void test_erase_bad(){
	std::string str = "I love you";
	TextBuffer tb(str);
	Erase op(&tb);
	
	// erase negative position
	assert(!op.execute("-2")); // print out warning message
	assert(tb.getContent() == str);

	// erase with non integer position and length
	assert(!op.execute("pos")); // print out warning message
	assert(tb.getContent() == str);

	// erase with no length
	assert(!op.execute("")); // print out warning message
	assert(tb.getContent() == str);

	// erase with position bigger than buffer length
	assert(op.execute("200")); // print out warning message
	assert(tb.getContent() == "");
}

void test_replace_good(){
	std::string str = "I love you";
	TextBuffer tb(str);
	Replace op(&tb);
	
	// simple replace with valid arguments
	std::string cmd_args = "love like";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I like you");

	// undo and redo previous replace
	op.undo(cmd_args);
	assert(tb.getContent() == str);
	op.redo(cmd_args);
	assert(tb.getContent() == "I like you");
	op.undo(cmd_args); //revert to orig

	// replace non-occurring substr
	cmd_args = "her you";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I love you");
	
	// replace multi-occurring substr
	cmd_args = "o a";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I lave yau");
	op.undo(cmd_args); //revert to orig

	// replace with space
	cmd_args = "love  ";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I   you");
	op.undo(cmd_args); //revert to orig
	assert(tb.getContent() == "I love you");

	// replace with nothing
	cmd_args = "love ";
	assert(op.execute(cmd_args));
	assert(tb.getContent() == "I  you");
	op.undo(cmd_args); //revert to orig
	assert(tb.getContent() == "I love you");
}

void test_replace_bad(){
	std::string str = "I love you";
	TextBuffer tb(str);
	Replace op(&tb);
	
	// replace no argument
	assert(!op.execute("")); // print out warning message
	assert(tb.getContent() == str);

	// replace single argument
	assert(!op.execute("love")); // print out warning message
	assert(tb.getContent() == str);

	// replace space
	assert(!op.execute("  love")); // print out warning message
	assert(tb.getContent() == str);
}

int main() {
	test_load_good();
	test_load_bad();
	test_save_good();
	test_save_bad();
	test_insert_good();
	test_insert_bad();
	test_append_good();
	test_append_bad();
	test_eraseAt_good();
	test_eraseAt_bad();
	test_erase_good();
	test_erase_bad();
	test_replace_good();
	test_replace_bad();
	return 0;
}
