#include <iostream>
#include <string>

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