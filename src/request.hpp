#pragma once
#include <vector>
#include <functional>
#include "includes.hpp"

namespace sqdiscord
{

    class Request
    {
    public:
        Request();
        Request(const std::string &method);
        ~Request();

        Request &setURL(const std::string &url);
        Request &setHeader(const std::string &key, const std::string &value);

        Request &setBody(const std::string &body);
        void send(Sqrat::Function callback);

        static void Register_sqdiscord(Sqrat::Table tb);

    private:
        std::string method_;
        std::string url_;
        std::vector<std::string> headers_;
        std::string body_;
        HSQUIRRELVM vm_;

        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output);
    };

    // Factory functions
    Request *GET();
    Request *POST();
    Request *PUT();
    Request *DELETE_();
}
