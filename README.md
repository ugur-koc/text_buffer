# Text Buffer Manager

## Compilation
`make`

Creates **text_buffer** executable.

## Usage: 
`./text_buffer`

Then `help` will display following message:

`insert position insertion_text` (inserts the insertion_text starting from position)
`append addendum_text` (appends addendum_text to the end)
`eraseAt position length`  (erases length character forward from position)
`erase length`  (erases length character from end)
`replace substr replacestr` (replaces all occurance of substr with replacestr)
`undo` 
`redo` 
`load filename`  
`save filename`  
`exit` 

## Running Tests
`make test`

(only after `make`)
Creates **tests/test_name.tmp** and **tests/test_name.out** files and checks **tests/test_name.out** files agains **tests/test_name.expected**

## Content

* **core/\*.h** and **main.cpp** project source code files
* **tests/UnitTests.cpp** contains functions that test operations in isolation
* **tests/test_name.test** files are test cases used my the **test** target in the make file
* **tests/test_name.expected** files are expected test case result used my the make file
* **tests/clear_color_codes.pl** is a helper script that post process test result to remove color codes.
* **aaa.txt** is used by some of the test cases (do not remove)

## Implementation Notes

Insert can insert empty string
There will be nothing to redo after a new/fresh operation
Replace cannot have empty find string to replace
Load and Save accepts .txt extensions only
Load and Save do not have undo and redo
Text buffer initializes as empty string
c++11 features used
Compilation is tested with clang++ 6.0

## Credits

This program is developed by Ugur Koc for the MathWorks interview.