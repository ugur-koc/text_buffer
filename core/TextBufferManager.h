/**
    MathWorks Interview Asn 2, TextBufferManager.h
    Purpose: Main algorithm interacting with user and performing the 
        operations issued by user

    @author Ugur Koc
    @version 0.1 01/25/19 
*/

#include <iostream>
#include <string>
#include <stack>
#include <map>

#include "Operations.h"

#define NUM_OP 7

class TextBufferManager {
public:
    
    // The main loop, used by main.cpp only
    void run() {
        TextBuffer tb;
        std::string cmd, arg, op_code, undo_cmd = "undo", redo_cmd = "redo";
        std::stack<std::string> cmd_hist, undones;
        std::stack<std::string>* cmd_stacks[2] = {&cmd_hist, &undones};
        std::map<std::string, Operation*> op_map;
        std::string op_codes[NUM_OP] = {"insert", "append", "eraseAt", "erase", "replace", "load", "save"};
        Insert insert(&tb);
        Append append(&tb);
        EraseAt eraseAt(&tb);
        Erase erase(&tb);
        Replace replace(&tb);
        Load load(&tb);
        Save save(&tb);

        Operation* operations[NUM_OP] = {&insert,&append,&eraseAt,&erase,&replace,&load,&save};
        for (int i = 0; i < NUM_OP; ++i) { // initialize op_map
            op_map[op_codes[i]] = operations[i];
        }

        HelperUtils::printOperations(op_codes);

        while (1) {
            tb.printContent();
            getline(std::cin, cmd);
            std::tie(op_code, arg) = HelperUtils::parseCmd(cmd);
            if (op_code == undo_cmd || op_code == redo_cmd) {
                if (arg != "") { // undo and redo do not take any arguments
                    HelperUtils::printWarning("Arguments to " + op_code + " are ignored!");
                }
                int indx = op_code != undo_cmd; // select the stack to pop from
                if (cmd_stacks[indx]->empty()) {
                    HelperUtils::printWarning("Nothing to " + op_code);
                } else {
                    cmd = cmd_stacks[indx]->top();
                    cmd_stacks[indx]->pop();
                    cmd_stacks[(indx+1)%2]->push(cmd); // push the cmd to the other stack
                    std::tie(op_code, arg) = HelperUtils::parseCmd(cmd);
                    if (indx == 1){
                        op_map[op_code]->redo(arg);
                    } else {
                        op_map[op_code]->undo(arg);
                    }
                }
            } else if (cmd == "help") {
                HelperUtils::printUsage(op_codes);
            } else if (cmd == "exit") {
                break;
            } else if (op_map.find(op_code) == op_map.end()) {
                HelperUtils::printWarning("Unknown operation code!"); // op_code is not in the map
            } else if (op_map[op_code]->execute(arg)) { // false implies a warning message was displayed
                if (op_code != "load" && op_code != "save") {
                    cmd_hist.push(cmd);
                }
                // there should be nothing to redo after a fresh operation
                while(!undones.empty()) undones.pop(); 
            }
        }
        op_map.clear();
    }
};