// rev-a7b31e-20260827 ChatEngine.h
#pragma once
#include <string>
#include <vector>
#include <utility>

class ChatEngine {
private:
    std::vector<std::pair<std::string, std::string>> m_history;
    ChatEngine() = default;
    std::string GenerateWithModel(const std::string& prompt, int maxTokens, float temp, float topP);
public:
    static ChatEngine& GetInstance();
    std::string Generate(const std::string& prompt);
    void ClearHistory();
};
