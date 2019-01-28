/**
    MathWorks Interview Asn 2, Operations.h
    Purpose: Super and base classes realizing the operations; 
        insert, append, erase, eraseAt, replace, load, and save

    @author Ugur Koc
    @version 0.1 01/25/19 
*/

#include <regex>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <stack>

#include "HelperUtils.h"
#include "TextBuffer.h"

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
        arg_regex = new std::regex(".+\\.txt");// accepting any form of file name .txt -- to be evaluted by OS
    }
    ~Load(){ delete arg_regex; }
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
        arg_regex = new std::regex(".+\\.txt");
    }
    ~Save(){ delete arg_regex; }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to Save!");
        } else {
            std::ofstream file_stream (arg, std::ios::out | std::ios::binary);
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
        arg_regex = new std::regex("\\d+\\s.*"); // e.g. '3 too late'
    }

    ~Insert(){ delete arg_regex; }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to Insert!");
        } else {
            int indx = arg.find(" ");
            int pos = stoi(arg.substr(0, indx));
            if (pos > tb->getLength()) {
                HelperUtils::printWarning("Invalid insertion position!");
                validArgs = false;
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
        tb->erase(pos, insertion_len); // erase the insertion part
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
        arg_regex = new std::regex(".+"); // anything
    }
    ~Append(){ delete arg_regex; }

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
    std::stack<std::string> erases; // stack of erased parts ot be used by undo/redo
public:
    explicit EraseAt(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex("\\d+\\s\\d+"); // pairs of positive numbers
    }
    ~EraseAt(){ delete arg_regex; }

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
                erases.push(removal);
                if ((pos+erase_len)>tb->getLength()) {
                    HelperUtils::printWarning("Erased less than "+std::to_string(erase_len)+" characters!");
                }
                tb->erase(pos, erase_len);// the actual operation
            }
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        int pos = stoi(arg.substr(0, indx));
        std::string insertion = erases.top();
        erases.pop();
        tb->insert(pos, insertion);
    }
    void redo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        int pos = stoi(arg.substr(0, indx));
        int erase_len  = stoi(arg.substr(indx+1));
        std::string removal = tb->getSubString(pos, erase_len);
        erases.push(removal);
        tb->erase(pos, erase_len);// the actual operation
    }
};

class Erase: public Operation {
    std::stack<std::string> erases; // stack of erased parts ot be used by undo/redo
public:
    explicit Erase(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex("\\d+"); // any positive number
    }
    ~Erase(){ delete arg_regex; }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to Erase!");
        } else {
            int erase_len = stoi(arg);
            int pos = tb->getLength() - erase_len;
            if (pos < 0) {
                HelperUtils::printWarning("Erased less than "+std::to_string(erase_len)+" characters!");
                pos = 0;
            }
            std::string removal = tb->getSubString(pos, erase_len);
            erases.push(removal);
            tb->erase(pos, erase_len);// the actual operation
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        std::string insertion = erases.top();
        erases.pop();
        tb->append(insertion);
    }
    void redo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int erase_len = stoi(arg);
        int pos = tb->getLength() - erase_len;
        if (pos < 0) { pos = 0; }
        std::string removal = tb->getSubString(pos, erase_len);
        erases.push(removal);
        tb->erase(pos, erase_len);// the actual operation
    }
};

class Replace: public Operation {
    std::stack<std::string> replaces; // stack of replaces to be used by undo/redo
public:
    explicit Replace(TextBuffer * _tb) {
        tb = _tb;
        arg_regex = new std::regex("\\S+\\s.*"); // e.g. 'a b'
    }
    ~Replace(){ delete arg_regex; }

    bool execute(const std::string &arg) override {
        bool validArgs = std::regex_match(arg, *arg_regex);
        if (!validArgs) {
            HelperUtils::printWarning("Invalid arguments to Replace!");
        } else {
            int indx = arg.find(" ");
            std::string findStr = arg.substr(0, indx);
            std::string replaceStr = arg.substr(indx + 1);
            replaces.push(tb->getContent());
            tb->replace(findStr,replaceStr);// the actual operation
        }
        return validArgs;
    }

    void undo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        std::string prevContent = replaces.top();
        replaces.pop();
        tb->setContent(prevContent);// the actual operation
    }
    void redo(const std::string &arg) override {
        // arg retrieved from history, no need for validity check
        int indx = arg.find(" ");
        std::string findStr = arg.substr(0, indx);
        std::string replaceStr = arg.substr(indx + 1);
        replaces.push(tb->getContent());
        tb->replace(findStr,replaceStr);// the actual operation
    }
};