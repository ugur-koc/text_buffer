CC=clang++-mp-6.0 
CFLAGS=-c
LDFLAGS=
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=text_buffer

TEST_SOURCES = tests/UnitTests.cpp
TEST_OBJECTS=$(TEST_SOURCES:.cpp=.o)
TEST_EXE = run_tests

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

test: $(TEST_SOURCES) $(TEST_EXE)

$(TEST_EXE): $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) $(TEST_EXE)
	rm -f $(OBJECTS) $(TEST_OBJECTS)