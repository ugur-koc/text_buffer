#include <iostream>
#include <string>
#include <stack>
#include <map>

#include "core/Operations.h"

#define NUM_OP 7

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