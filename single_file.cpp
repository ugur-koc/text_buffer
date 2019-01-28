#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <regex>
#include <fstream>
#include <streambuf>
#include <sstream>

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

class TextBuffer {
    std::string content;
public:
    TextBuffer(){
        
    }

    explicit TextBuffer(const std::string &_content): content(_content){}

    void setContent(const std::string  &_content){
        content= _content;
    }

    std::string getContent() const {
        return content;
    }

    std::string getSubString(int start) const {
        return content.substr(start);
    }
    
    std::string getSubString(int start, int length) const {
        return content.substr(start, length);
    }

    void printContent() const {
        std::cout << "Text Buffer: " << "\033[1;32m" << content << "\033[0m" << std::endl;
    }

    const void insert(int pos, const std::string &insertion) {
        content.insert(pos, insertion);
    }
    
    const void append(const std::string &addition) {
        content.append(addition);
    }
    
    void replace(const std::string &findStr, const std::string &replaceStr) {
        size_t pos = 0;
        while ((pos = content.find(findStr, pos)) != std::string::npos) {
            content.replace(pos, findStr.length(), replaceStr);
            pos += replaceStr.length();
        }
    }

    const void erase(int pos, int length) {
        content.erase(pos, length);
    }
    
    const int getLength() const {
        return content.length();
    }

};

class Operation {
public:
    TextBuffer *tb;
    std::regex *arg_regex;
    virtual bool execute(const std::string &args) = 0 ;
    virtual void undo(const std::string &args) = 0;
    virtual void redo(const std::string &args) = 0; // better to have to skip the validity checks for args
};

class Load: public Operation { 
public:
    explicit Load(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex(".+");
    }
    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to Load!");
        } else {
            std::ifstream file_stream(arg);
            if (!file_stream) {
                HelperUtils::printWarning("Error in openning file " + arg + "!");
                validArgs = false;
            } else {
                std::stringstream buffer;
                buffer << file_stream.rdbuf();
                tb->setContent(buffer.str());    
            }
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {} // not in use
    void redo(const std::string &arg) override {} // not in use
};

class Save: public Operation { 
public:
    explicit Save(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex(".+");
    }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to Save!");
        } else {
            std::ofstream file_stream (arg, std::ios::out | std::ios::app | std::ios::binary);
            if (!file_stream.is_open()) {
                HelperUtils::printWarning("Error in openning file " + arg + "!");
                validArgs = false;
            } else {
                file_stream << tb->getContent();
                file_stream.close();
            }
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {} // not in use
    void redo(const std::string &arg) override {} // not in use
};

class Insert: public Operation {
public:
    explicit Insert(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex("\\d+\\s.*");
    }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to insert!");
        } else {
            int indx = arg.find(" ");
            int pos = stoi(arg.substr(0, indx));
            if (pos > tb->getLength()) {
                validArgs = false;
                HelperUtils::printWarning("Invalid insertion position!");
            } else {
                std::string insertion = arg.substr(indx + 1); // test 'insert 1 '
                tb->insert(pos, insertion); // the actual operation
            }
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        int pos = stoi(arg.substr(0, indx));
        int insertion_len = arg.length() - indx - 1;
        tb->erase(pos, insertion_len);
    }
    void redo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        int pos = stoi(arg.substr(0, indx));
        std::string insertion = arg.substr(indx + 1);
        tb->insert(pos, insertion);
    }
};

class Append: public Operation {
public:
    explicit Append(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex(".+");
    }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Nothing to append!");
        } else {
            tb->append(arg);// the actual operation
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int insertion_len = arg.length();
        tb->erase(tb->getLength() - insertion_len, insertion_len);
    }
    void redo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        tb->append(arg);
    }
};

class EraseAt: public Operation {
    std::stack<std::string> *erases;
public:
    explicit EraseAt(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex("\\d+\\s\\d+");
        erases = new std::stack<std::string>();
    }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to EraseAt!");
        } else {
            int indx = arg.find(" ");
            int pos = stoi(arg.substr(0, indx));
            if (pos > tb->getLength()) {
                validArgs = false;
                HelperUtils::printWarning("Invalid EraseAt position!");
            } else {
                int erase_len  = stoi(arg.substr(indx+1));
                std::string removal = tb->getSubString(pos, erase_len);
                erases->push(removal);
                tb->erase(pos, erase_len);// the actual operation
                if ((pos+erase_len)>tb->getLength()) {
                    HelperUtils::printWarning("Erased less then "+std::to_string(erase_len)+" characthers!");
                }
            }
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        int pos = stoi(arg.substr(0, indx));
        std::string insertion = erases->top();
        erases->pop();
        tb->insert(pos, insertion);
    }
    void redo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        int pos = stoi(arg.substr(0, indx));
        int erase_len  = stoi(arg.substr(indx+1));
        std::string removal = tb->getSubString(pos, erase_len);
        erases->push(removal);
        tb->erase(pos, erase_len);// the actual operation
    }
};

class Erase: public Operation {
    std::stack<std::string> *erases;
public:
    explicit Erase(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex("\\d+");
        erases = new std::stack<std::string>();
    }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to Erase!");
        } else {
            int erase_len = stoi(arg);
            int pos = tb->getLength() - erase_len;
            if (pos < 0) {
                HelperUtils::printWarning("Erased less then "+std::to_string(erase_len)+" characthers!");
                pos = 0;
            }
            std::string removal = tb->getSubString(pos, erase_len);
            erases->push(removal);
            tb->erase(pos, erase_len);// the actual operation
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        std::string insertion = erases->top();
        erases->pop();
        tb->append(insertion);
    }
    void redo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int erase_len = stoi(arg);
        int pos = tb->getLength() - erase_len;
        if (pos < 0) { pos = 0; }
        std::string removal = tb->getSubString(pos, erase_len);
        erases->push(removal);
        tb->erase(pos, erase_len);// the actual operation
    }
};

class Replace: public Operation {
public:
    explicit Replace(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex(".+\\s.*");
    }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to Replace!");
        } else {
            int indx = arg.find(" ");
            std::string findStr = arg.substr(0, indx);
            std::string replaceStr = arg.substr(indx + 1);
            tb->replace(findStr,replaceStr);// the actual operation
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        std::string findStr = arg.substr(0, indx);
        std::string replaceStr = arg.substr(indx + 1);
        tb->replace(replaceStr,findStr);// the actual operation
    }
    void redo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        std::string findStr = arg.substr(0, indx);
        std::string replaceStr = arg.substr(indx + 1);
        tb->replace(findStr,replaceStr);// the actual operation
    }
};

std::map<std::string, Operation*> initOperationMap(TextBuffer *tb){
    std::map<std::string, Operation*> op_map;
    std::string op_codes[NUM_OP] = {"insert", "append", "eraseAt", "erase", "replace", "load", "save"};
    Operation *operations[NUM_OP] = {new Insert(tb), new Append(tb), new EraseAt(tb), new Erase(tb), new Replace(tb), new Load(tb), new Save(tb)};
    for (int i = 0; i < NUM_OP; ++i) { 
        op_map[op_codes[i]] = operations[i];
    }
    HelperUtils::printOperations(op_codes);
    return op_map;
}


int main() {
    TextBuffer *tb = new TextBuffer();
    std::string cmd, arg, op_code, undo_cmd = "undo", redo_cmd = "redo";
    std::stack<std::string> cmd_hist, undones;
    std::stack<std::string>* cmd_stacks[] = {&cmd_hist, &undones};
    std::map<std::string, Operation*> op_map = initOperationMap(tb);

    while (1) {
        tb->printContent();
        getline(std::cin, cmd);
        std::tie(op_code, arg) = HelperUtils::parseCmd(cmd);
        if (cmd == undo_cmd || cmd == redo_cmd) {
            if (arg != "") {
                HelperUtils::printWarning("Arguments to " + op_code + " are ignored!");
            }
            int indx = op_code != undo_cmd;
            if (cmd_stacks[indx]->empty()) {
                HelperUtils::printWarning("Nothing to " + op_code);
            } else {
                cmd = cmd_stacks[indx]->top();
                cmd_stacks[indx]->pop();
                cmd_stacks[(indx+1)%2]->push(cmd);
                std::tie(op_code, arg) = HelperUtils::parseCmd(cmd);
                if (indx == 1){
                    op_map[op_code]->redo(arg);
                } else {
                    op_map[op_code]->undo(arg);
                }
            }
        } else if (cmd == "exit") {
            break;
        } else if (op_map.find(op_code) == op_map.end()) {
            HelperUtils::printWarning("Unknown operation code!");
        } else if (op_map[op_code]->execute(arg)) { // false implies a warning message was displayed
            if (op_code != "load" && op_code != "save") {
                cmd_hist.push(cmd);
            }
            // there should be nothing to redo after a fresh operation
            while(!undones.empty()) undones.pop(); 
        }
    }
    return 0;
}