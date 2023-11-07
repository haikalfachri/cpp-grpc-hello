#include "env_reader.h"

using namespace std;  

EnvReader::EnvReader(const std::string& filePath) {
    std::ifstream file(filePath);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                envMap[key] = value;
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filePath << std::endl;
    }
}

std::string EnvReader::get(const std::string& key) const {
    auto it = envMap.find(key);
    if (it != envMap.end()) {
        return it->second;
    } else {
        return ""; 
    }
}
