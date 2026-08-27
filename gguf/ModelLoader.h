// rev-a7b31e-20260827 ModelLoader.h
#pragma once
#include <string>
#include <unordered_map>

struct GGUFHeader {
    uint32_t version;
    uint64_t tensorCount;
    uint64_t kvCount;
};

class ModelLoader {
private:
    std::string m_modelPath;
    bool m_isLoaded = false;
    GGUFHeader m_header{};
    std::unordered_map<std::string, std::string> m_metadata;
    ModelLoader() = default;
public:
    static ModelLoader& GetInstance();
    bool LoadModel(const std::string& path);
    bool IsLoaded() const { return m_isLoaded; }
    std::string GetModelInfo() const;
    const std::string& GetModelPath() const { return m_modelPath; }
};
