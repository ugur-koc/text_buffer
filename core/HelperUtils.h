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