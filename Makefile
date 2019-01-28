CC=clang++-mp-6.0 
CFLAGS=-c
LDFLAGS=
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=text_buffer

TEST_SOURCES = tests/UnitTests.cpp
TEST_OBJECTS=$(TEST_SOURCES:.cpp=.o)
TEST_EXE = tests/run_unit_tests
TESTS = load_save load_insert_replace load_insert_eraseAt load_insert_erase \
		load_undo load_redo load_erase_undo load_append_undo \
		insert_undo_insert_redo insert_append_save insert_append_replace_undo_undo \
		bad_args_undo bad_args_redo bad_op_code \

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

test: $(TEST_SOURCES) $(TEST_EXE)
	./$(TEST_EXE) > tests/unittests.tmp 2>&1;\
	tests/clear_color_codes.pl < tests/unittests.tmp > tests/unittests.out 2>&1; \
    diff tests/unittests.expected tests/unittests.out > /dev/null || \
	echo Test unittests failed >&2;\
	for t in $(TESTS); do \
        ./$(EXECUTABLE) < tests/$$t.test > tests/$$t.tmp 2>&1; \
        tests/clear_color_codes.pl < tests/$$t.tmp > tests/$$t.out 2>&1; \
        diff tests/$$t.expected tests/$$t.out > /dev/null || \
            echo Test $$t failed >&2; \
    done

$(TEST_EXE): $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean test

clean:
	rm -f $(EXECUTABLE) $(TEST_EXE) $(OBJECTS) $(TEST_OBJECTS)
	rm -f tests/*.out tests/*.tmp tests/*.txt