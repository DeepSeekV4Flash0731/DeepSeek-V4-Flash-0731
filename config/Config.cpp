// rev-a7b31e-20260827 Config.cpp
#include "Config.h"
#include <fstream>
#include <sstream>

Config& Config::GetInstance() { static Config i; return i; }

void Config::Load(const std::string& path) {
    m_path = path;
    m_values["max_tokens"] = "2048";
    m_values["temperature"] = "0.7";
    m_values["top_p"] = "0.9";
    m_values["context_length"] = "1048576";
    m_values["gpu_layers"] = "auto";
    m_values["threads"] = "auto";
    m_values["batch_size"] = "512";

    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        val.erase(0, val.find_first_not_of(" \t"));
        val.erase(val.find_last_not_of(" \t") + 1);
        m_values[key] = val;
    }
}

std::string Config::GetString(const std::string& key, const std::string& def) const {
    auto it = m_values.find(key);
    return it != m_values.end() ? it->second : def;
}

int Config::GetInt(const std::string& key, int def) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) return std::stoi(it->second);
    return def;
}

float Config::GetFloat(const std::string& key, float def) const {
    auto it = m_values.find(key);
    if (it != m_values.end()) return std::stof(it->second);
    return def;
}
