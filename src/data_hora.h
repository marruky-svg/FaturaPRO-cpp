#pragma once
#include <chrono>
#include <sstream>
#include <iomanip>

inline std::string dataAtual()
    {
        auto agora = std::chrono::system_clock::now();
        auto tempo = std::chrono::system_clock::to_time_t(agora);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&tempo),"%Y-%m-&d &H:&M:&S");
        return ss.str();
    }