#include <iostream>
#include <string>
#include <vector>

struct key {
    std::string name;
    int mode;
    std::string function;
    std::string arg;

    bool multi;
};

std::vector<key> getKeymap(std::istream& config);
