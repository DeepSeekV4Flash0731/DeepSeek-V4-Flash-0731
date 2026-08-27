// rev-a7b31e-20260827 ChatEngine.cpp
#include "ChatEngine.h"
#include "ModelLoader.h"
#include "Config.h"
#include <sstream>

ChatEngine& ChatEngine::GetInstance() { static ChatEngine i; return i; }

std::string ChatEngine::Generate(const std::string& prompt) {
    if (!ModelLoader::GetInstance().IsLoaded()) {
        return "Error: No model loaded.";
    }

    Config& config = Config::GetInstance();
    int maxTokens = config.GetInt("max_tokens", 2048);
    float temperature = config.GetFloat("temperature", 0.7f);
    float topP = config.GetFloat("top_p", 0.9f);

    std::string response = GenerateWithModel(prompt, maxTokens, temperature, topP);
    m_history.push_back({"user", prompt});
    m_history.push_back({"assistant", response});
    return response;
}

std::string ChatEngine::GenerateWithModel(const std::string& prompt, int maxTokens, float temp, float topP) {
    // This would interface with llama.cpp or similar inference engine
    // For now, return a placeholder indicating the model is ready
    std::stringstream ss;
    ss << "[DeepSeek V4 Flash Local 0731 - Local Inference]\n";
    ss << "Model loaded and ready. Prompt received (" << prompt.length() << " chars).\n";
    ss << "Max tokens: " << maxTokens << ", Temperature: " << temp << ", Top-p: " << topP << "\n";
    ss << "Note: Connect to llama.cpp or Ollama backend for actual inference.";
    return ss.str();
}

void ChatEngine::ClearHistory() { m_history.clear(); }
