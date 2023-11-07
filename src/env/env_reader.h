#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>

class EnvReader {
private:
    std::unordered_map<std::string, std::string> envMap;

public:
    EnvReader(const std::string& filePath);
    std::string get(const std::string& key) const;
};