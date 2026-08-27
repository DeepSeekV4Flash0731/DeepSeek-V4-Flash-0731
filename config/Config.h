// rev-a7b31e-20260827 Config.h
#pragma once
#include <string>
#include <unordered_map>

class Config {
private:
    std::string m_path;
    std::unordered_map<std::string, std::string> m_values;
    Config() = default;
public:
    static Config& GetInstance();
    void Load(const std::string& path);
    std::string GetString(const std::string& key, const std::string& def) const;
    int GetInt(const std::string& key, int def) const;
    float GetFloat(const std::string& key, float def) const;
};
