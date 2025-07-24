#include "Logger.h"
#include <chrono>
#include <format>
#include <fstream>

#include <debugapi.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

void Log(const std::string& _message)
{
	OutputDebugStringA(_message.c_str());
	return;
}

void Logger::Initialize()
{
    pJsonio_ = JSONIO::GetInstance();

    /// ルートを作成
    jsonValue_ = json::object();

    /// Logs配列を作成
    jsonValue_["Logs"] = json::array();


    /// ファイル名を設定
    auto now = std::chrono::system_clock::now();
    auto now_sec = std::chrono::floor<std::chrono::seconds>(now);
    fileName_ = std::format("{:%Y%m%d_%H%M%S}", now_sec);

    return;
}

void Logger::Save()
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto now = std::chrono::system_clock::now();

    std::string filepath = folderPath_ + fileName_;
    pJsonio_->Save(filepath + ".json", jsonValue_);

    std::string filePayload;
    for (auto& log : logData_)
    {
        filePayload += "[" + log.status + "] ";
        filePayload += log.date + " ";
        filePayload += log.time + ", ";
        filePayload += log.filename + ", ";
        filePayload += log.action + ", ";
        filePayload += log.message + "\n";
    }

    /// 改行を削除
    filePayload.erase(filePayload.end() - 1);

    std::ofstream ofs(filepath + ".log");
    ofs << filePayload;
    ofs.close();

    logPayload_ = filePayload;

    return;
}

void Logger::DrawUI()
{
#ifdef _DEBUG

    ImGuiWindowFlags flag = {};
    flag |= ImGuiWindowFlags_HorizontalScrollbar;
    if ( enableAutoScroll_ )
    {
        flag = ImGuiWindowFlags_NoScrollWithMouse;
        flag |= ImGuiWindowFlags_NoScrollbar;
    }

    if ( ImGui::Begin("Log", nullptr, flag) )
    {
        ImGui::Checkbox("Auto Scroll", &enableAutoScroll_);

        ImGui::BeginChild("LogChild", ImVec2(-1, -1), ImGuiChildFlags_Border, flag);

        ImGui::TextUnformatted(logPayload_.data());

        if ( enableAutoScroll_ )
        {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
    }
    ImGui::End();

#endif // _DEBUG
}

void Logger::LogError(const std::string& _filename, const std::string& _action, const std::string& _message)
{
    Log("Error", _filename, _action, _message);
}

void Logger::LogWarning(const std::string& _filename, const std::string& _action, const std::string& _message)
{
    Log("Warning", _filename, _action, _message);
}

void Logger::LogInfo(const std::string& _filename, const std::string& _action, const std::string& _message)
{
    Log("Info", _filename, _action, _message);
}

void Logger::LogForOutput(const std::string& _message)
{
    OutputDebugStringA(_message.c_str());
}

void Logger::Log(const std::string& _status, const std::string& _filename, const std::string& _action, const std::string& _message)
{
    auto now = std::chrono::system_clock::now();
    auto now_sec = std::chrono::floor<std::chrono::seconds>(now);


    std::string date = std::format("{:%Y/%m/%d}", now_sec);
    std::string time = std::format("{:%H:%M:%S}", now_sec);

    LogData data = {};
    data.date = date;
    data.time = time;
    data.status = _status;
    data.filename = _filename;
    data.action = _action;
    data.message = _message;


    {
        std::lock_guard<std::mutex> lock(mutex_);
        logData_.emplace_back(data);
        LogJson(date, time, _status, _filename, _action, _message);
    }

    OutputDebugStringA(std::format("[{}] {} {}, {}, {}, {}\n", _status, date, time, _filename, _action, _message).c_str());

    this->Save();
}

void Logger::LogJson(const std::string& _date, const std::string& _time, const std::string& _status, const std::string& _filename, const std::string& _action, const std::string& _message)
{
    auto object = json::object();
    object["Date"] = _date;
    object["Time"] = _time;
    object["Status"] = _status;
    object["ClassName"] = _filename;
    object["Action"] = _action;
    object["Message"] = _message;

    jsonValue_["Logs"].push_back(object);

    return;
}
