#include <assert.h>

#include "../core/Operations.h"

void test_insert_good(){
	std::string str = "I love you";
	TextBuffer *tb = new TextBuffer(str);
	Operation *op = new Insert(tb);
	
	// simple insertion with valid arguments
	std::string cmd_args = "2 do ";
	assert(op->execute(cmd_args));
	assert(tb->getContent() == "I do love you");

	// undo and redo previous insertion
	op->undo(cmd_args);
	assert(tb->getContent() == str);
	op->redo(cmd_args);
	assert(tb->getContent() == "I do love you");
	op->undo(cmd_args); //revert to orig

	// undo, a new insertion, and redo previous insertion
	cmd_args = "2 did ";
	assert(op->execute(cmd_args));
	assert(tb->getContent() == "I did love you");
	op->undo(cmd_args); //revert to orig
	
	// insertion to end
	cmd_args = "10  my girl";
	assert(op->execute(cmd_args));
	assert(tb->getContent() == "I love you my girl");
	
	// insertion to the begining
	cmd_args = "0 Why do ";
	assert(op->execute(cmd_args));
	assert(tb->getContent() == "Why do I love you my girl");
}

void test_insert_bad(){
	std::string str = "I love you";
	TextBuffer *tb = new TextBuffer(str);
	Operation *op = new Insert(tb);
	
	// insertion with negative position
	assert(!op->execute("-2 do ")); // print out warning message
	assert(tb->getContent() == str);

	// insertion with position bigger than buffer lenght
	assert(!op->execute("200 do ")); // print out warning message
	assert(tb->getContent() == str);

	// insertion with non integer position
	assert(!op->execute("pos do ")); // print out warning message
	assert(tb->getContent() == str);

	// insertion with no insertion text
	assert(!op->execute("2")); // print out warning message
	assert(tb->getContent() == str);
}

int main() {
	test_insert_good();
	test_insert_bad();

	return 0;
}