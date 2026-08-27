// rev-a7b31e-20260827 ModelLoader.cpp
#include "ModelLoader.h"
#include <fstream>
#include <sstream>

ModelLoader& ModelLoader::GetInstance() { static ModelLoader i; return i; }

bool ModelLoader::LoadModel(const std::string& path) {
    m_modelPath = path;
    m_isLoaded = false;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    // Check GGUF magic
    char magic[4];
    file.read(magic, 4);
    if (magic[0] != 'G' || magic[1] != 'G' || magic[2] != 'U' || magic[3] != 'F') {
        return false;
    }

    // Read GGUF header
    file.read((char*)&m_header.version, sizeof(m_header.version));
    file.read((char*)&m_header.tensorCount, sizeof(m_header.tensorCount));
    file.read((char*)&m_header.kvCount, sizeof(m_header.kvCount));

    // Read metadata
    for (uint64_t i = 0; i < m_header.kvCount; i++) {
        uint64_t keyLen;
        file.read((char*)&keyLen, sizeof(keyLen));
        std::string key(keyLen, '\0');
        file.read(key.data(), keyLen);

        uint32_t type;
        file.read((char*)&type, sizeof(type));

        switch (type) {
            case 0: { uint8_t v; file.read((char*)&v, 1); m_metadata[key] = std::to_string(v); break; }
            case 1: { int8_t v; file.read((char*)&v, 1); m_metadata[key] = std::to_string(v); break; }
            case 2: { uint16_t v; file.read((char*)&v, 2); m_metadata[key] = std::to_string(v); break; }
            case 3: { int16_t v; file.read((char*)&v, 2); m_metadata[key] = std::to_string(v); break; }
            case 4: { uint32_t v; file.read((char*)&v, 4); m_metadata[key] = std::to_string(v); break; }
            case 5: { int32_t v; file.read((char*)&v, 4); m_metadata[key] = std::to_string(v); break; }
            case 6: { float v; file.read((char*)&v, 4); m_metadata[key] = std::to_string(v); break; }
            case 7: { bool v; file.read((char*)&v, 1); m_metadata[key] = v ? "true" : "false"; break; }
            case 8: {
                uint64_t len; file.read((char*)&len, sizeof(len));
                std::string v(len, '\0'); file.read(v.data(), len);
                m_metadata[key] = v; break;
            }
            default: return false;
        }
    }

    m_isLoaded = true;
    return true;
}

std::string ModelLoader::GetModelInfo() const {
    std::stringstream ss;
    ss << "DeepSeek V4 Flash Local 0731\n";
    ss << "Architecture: Mixture of Experts\n";
    ss << "Total parameters: 236B\n";
    ss << "Active parameters: 37B\n";
    ss << "Context window: 1M tokens\n";
    ss << "Attention: CSA + HCA\n";
    for (const auto& [k, v] : m_metadata) {
        ss << k << ": " << v << "\n";
    }
    return ss.str();
}
