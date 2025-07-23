#include "CSession.hpp"
extern HSQUIRRELVM v;

// Global
namespace SqDiscord
{
    static std::unordered_map<int, CSession *> sessions;
    static int nextConnID = 1;

    CSession::CSession()
    {
        connID = nextConnID++;
        sessions[connID] = this;
    }

    CSession::~CSession()
    {
        if (bot)
            delete bot;
        sessions.erase(connID);
    }

    void CSession::Connect(const std::string &token)
    {
        token_ = token;
        bot = new dpp::cluster(token);

        SetupEvents();
        bot->start(dpp::st_wait);
    }

    void CSession::Message(const std::string &channelID, const std::string &content)
    {
        if (!bot)
            return;
        bot->message_create(dpp::message(channelID, content));
    }
    void Process()
    {
        for (auto &[id, session] : sessions)
        {
            if (session)
                session->Update();
        }
    }

    void CSession::Update()
    {
        Event evt;
        while (eventQueue.try_dequeue(evt))
        {

            Sqrat::Function listener = Sqrat::RootTable(v).GetFunction(_SC("onDiscordUpdate"));
            if (!listener.IsNull())
            {
                try
                {
                    listener.Execute(evt.connID, evt.type, evt.args);
                }
                catch (Sqrat::Exception &e)
                {
                    printf("Squirrel Exception: %s\n", e.Message().c_str());
                }
            }
        }
    }

    void CSession::FireEvent(int type, const std::vector<std::string> &args)
    {
        Event evt;
        evt.connID = connID;
        evt.type = type;
        evt.args = args;

        eventQueue.enqueue(evt);
    }

    void CSession::SetupEvents()
    {
        bot->on_ready([this](const dpp::ready_t &)
                      { FireEvent(0, {}); });

        bot->on_message_create([this](const dpp::message_create_t &ev)
                               { FireEvent(1, {ev.msg.content}); });

        bot->on_log([this](const dpp::log_t &log)
                    {
        if (log.severity >= dpp::ll_error) {
            FireEvent(2, {
                std::to_string(log.severity),
                log.message
            });
        }

        // Optional: detect disconnect from log
        if (log.message.find("websocket closed") != std::string::npos ||
            log.message.find("connection lost") != std::string::npos) {
            FireEvent(3, {});
        } });
    }

    void CSession::Register_CSession(Sqrat::Table tb)
    {
        using namespace Sqrat;
        tb.Bind(_SC("CSession"),
                Class<CSession, NoCopy<CSession>>(tb.GetVM(), _SC("CSession"))
                    .Var(_SC("ConnID"), &CSession::connID)
                    //                    .Var(_SC("InternalCacheEnabled"), &CSession::InternalCacheEnabled)
                    .Func(_SC("Connect"), &CSession::Connect)
                    .Func(_SC("Message"), &CSession::Message));
    }

}