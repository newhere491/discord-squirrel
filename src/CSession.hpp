#pragma once

#include "includes.hpp"
#include <thread>
#include <mutex>
#include <memory>
#include <unordered_map>
#include "readerwriterqueue.h"

namespace SqDiscord
{

    struct Event
    {
        int connID;
        int type;
        std::vector<std::string> args;
    };

    class CSession
    {
    public:
        int connID;
        bool InternalCacheEnabled = false;

        CSession();
        ~CSession();

        void Connect(const std::string &token);
        void Message(const std::string &channelID, const std::string &content);
        void MessageEmbed(const std::string &channelID, const std::string &content, const dpp::embed &embed);
        std::string GetRoleName(dpp::snowflake guildID, dpp::snowflake roleID);

        static void Process();
        void Update();

        static void CSession::Register_CSession(Sqrat::Table tb);

        moodycamel::ReaderWriterQueue<Event> eventQueue;

    private:
        dpp::cluster *bot = nullptr;
        std::string token_;

        void SetupEvents();
        void FireEvent(int type, const std::vector<std::string> &args);
    };
};