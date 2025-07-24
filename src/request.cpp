#include "request.hpp"
#include <curl/curl.h>
#include <thread>

namespace SqDiscord
{

    Request::Request(const std::string &method)
        : method_(method), vm_(nullptr) {}

    Request::Request() : method_("GET"), vm_(nullptr) {}

    Request::~Request() {}

    Request &Request::setURL(const std::string &url)
    {
        url_ = url;
        return *this;
    }

    Request &Request::setHeader(const std::string &key, const std::string &value)
    {
        headers_.push_back(key + ": " + value);
        return *this;
    }

    Request &Request::setBody(const std::string &body)
    {
        body_ = body;
        return *this;
    }

    size_t Request::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
    {
        size_t totalSize = size * nmemb;
        output->append((char *)contents, totalSize);
        return totalSize;
    }

    void Request::send(Sqrat::Function callback)
    {
        vm_ = callback.GetVM();

        std::thread([=, callback = callback]() mutable
                    {
        CURL *curl = curl_easy_init();
        std::string response;
        long responseCode = -1;

        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());

            if (method_ == "POST")
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
            else if (method_ == "PUT")
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
            else if (method_ == "DELETE")
                curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

            if (!body_.empty())
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_.c_str());

            struct curl_slist *chunk = nullptr;
            for (const auto &h : headers_)
                chunk = curl_slist_append(chunk, h.c_str());

            if (chunk)
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);

            if (res == CURLE_OK)
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);

            curl_easy_cleanup(curl);
            if (chunk)
                curl_slist_free_all(chunk);
        }

        // Execute the callback with both responseCode and responseBody
        callback.Execute(responseCode, response); })
            .detach();
    }

    // Factory methods
    Request *GET() { return new Request("GET"); }
    Request *POST() { return new Request("POST"); }
    Request *PUT() { return new Request("PUT"); }
    Request *DELETE_() { return new Request("DELETE"); }

    void SqDiscord::Request::Register_SqDiscord(Sqrat::Table tb)
    {
        using namespace Sqrat;

        tb.Func(_SC("GET"), &GET);
        tb.Func(_SC("POST"), &POST);
        tb.Func(_SC("PUT"), &PUT);
        tb.Func(_SC("DELETE"), &DELETE_);

        tb.Bind(_SC("Request"),
                Class<Request>(v, _SC("Request"))
                    .Func(_SC("setURL"), &Request::setURL)
                    .Func(_SC("setHeader"), &Request::setHeader)
                    .Func(_SC("setBody"), &Request::setBody)
                    .Func(_SC("send"), &Request::send));
    }
}
