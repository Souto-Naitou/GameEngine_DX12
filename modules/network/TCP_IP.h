#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <vector>



namespace TCP
{
    class Client
    {
    public:
        Client() = default;
        ~Client() = default;

        void                    Initialize(const WSADATA& _wsaData);

        bool                    Connect(const std::string& _ip, const std::string& _port);
        void                    Close() const;

        bool                    Send(const std::string& _data);
        std::string             Receive();


    private:
        SOCKET                  clientSocket_;
        WSADATA                 wsaData_;
        addrinfo*               result_;
        addrinfo                hints_;
        int                     iResult_ = 0;
    };


    class Host
    {
    public:
        Host() = default;
        ~Host() = default;

        void                    Initialize(const WSADATA& _wsaData);

        void                    Open(const std::string& _port);
        void                    Close() const;

        bool                    Send(uint32_t _handle, const char* _data, int _size);
        std::string             Receive(uint32_t _handle) const;

        uint32_t                ListenAndAccept();


    private:
        SOCKET                  serverSocket_;
        WSADATA                 wsaData_;
        addrinfo*               result_;
        addrinfo                hints_;
        std::string             port_;
        int                     iResult_ = 0;
        std::vector<SOCKET>     clientSockets_;
    };
}

std::string GetErrorMessage(DWORD _errorCode);