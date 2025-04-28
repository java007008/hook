#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include "cef_include.h"  // 包含CEF头文件

// 模拟发送消息函数
void SendMessageToChat(const std::string& message) {
    // 模拟通过CEF发送消息（你需要根据实际的CEF实现来填写具体发送代码）
    std::cout << "发送消息: " << message << std::endl;
}

// 检查聊天内容并自动回复
void CheckAndReply(const std::string& message, const std::string& groupName) {
    std::vector<std::string> triggerWords = { "有问题", "打不开" };
    std::string replyMessage = "1";

    // 如果是特定群组并且包含触发关键词，则回复
    if (groupName == "需求讨论") {
        for (const auto& word : triggerWords) {
            if (message.find(word) != std::string::npos) {
                SendMessageToChat(replyMessage);
                break;
            }
        }
    }
}

// CEF事件监听函数：监听JS中的聊天内容
void OnChatMessageReceived(const std::string& message, const std::string& groupName) {
    CheckAndReply(message, groupName);
}