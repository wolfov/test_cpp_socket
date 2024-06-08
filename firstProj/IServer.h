#pragma once
#include <string>

    class IServer {
    public:
        virtual void Run() = 0;
        virtual void Close() = 0;
        virtual void AcceptNewListiners() = 0;
        virtual void SendForAll(std::string& message) = 0;
    };
