/**
    MathWorks Interview Asn 2, HelperUtils.h
    Purpose: Helper functions used by Operations and TextBufferManager

    @author Ugur Koc
    @version 0.1 01/25/19 
*/

#include <iostream>
#include <string>
#include <sstream>
#include <tuple>

#define NUM_OP 7

class HelperUtils {
public:
    static void printWarning(const std::string &warning) {
        std::cout << "\033[1;31m" << warning << "\033[0m\n" << std::endl;
    }

    static void printOperations(const  std::string operations[]) {
	    std::cout << "Available Operations: ";
	    for (int i = 0; i < NUM_OP; ++i) {
	        std::cout << "\033[1;34m" << operations[i] << "\033[0m ";
	    }
	    std::cout << "\033[1;34mhelp exit\033[0m "<<std::endl;
	}

    static void printUsage(const  std::string operations[]) {
	    std::cout << "Usage: "<<std::endl;
	    std::cout << "\033[1;34m" << operations[0] << " position insertion_text\033[0m (inserts the insertion_text starting from position)"<<std::endl;
	    std::cout << "\033[1;34m" << operations[1] << " addendum_text\033[0m (appends addendum_text to the end)"<<std::endl;
	    std::cout << "\033[1;34m" << operations[2] << " position length \033[0m (erases length character forward from position)"<<std::endl;
	    std::cout << "\033[1;34m" << operations[3] << " length \033[0m (erases length character from end)"<<std::endl;
	    std::cout << "\033[1;34m" << operations[4] << " substr replacestr \033[0m (replaces all occurance of substr with replacestr)"<<std::endl;
	    std::cout << "\033[1;34mundo\033[0m "<<std::endl;
	    std::cout << "\033[1;34mredo\033[0m "<<std::endl;
	    std::cout << "\033[1;34m" << operations[5] << " filename \033[0m "<<std::endl;
	    std::cout << "\033[1;34m" << operations[6] << " filename \033[0m "<<std::endl;
	    std::cout << "\033[1;34mexit\033[0m "<<std::endl;
	}

	static std::tuple<std::string, std::string> parseCmd(const std::string &cmd) {
	    int indx = cmd.find(" ");
	    if (indx == std::string::npos) {
	        return std::make_tuple(cmd, "");
	    }
	    return std::make_tuple(cmd.substr(0, indx), cmd.substr(indx + 1));
	}
};