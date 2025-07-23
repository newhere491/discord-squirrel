#pragma once

#include <vector>
#include "includes.hpp"

#include "User.hpp"

namespace SqDiscord
{

	typedef User DUser;

	class ServerMember
	{
	public:
		std::string ID;
		DUser User;
		std::string Nick;
		std::vector<std::string> Roles;
		std::string JoinedAt;
		bool isDeaf = false;
		bool isMute = false;

		ServerMember() = default;

		ServerMember(const dpp::guild_member &member, const dpp::user &userObj);

		ServerMember(const ServerMember &o) = default;

		ServerMember(ServerMember &&o) = default;

		ServerMember &operator=(const ServerMember &o) = default;

		ServerMember &operator=(ServerMember &&o) = default;

		~ServerMember() = default;

		void UpdateRoles(const std::vector<dpp::snowflake> &roles);

		std::string GetID() const;

		nlohmann::json GetUser();

		std::string GetNick() const;

		int GetRoleCount() const;

		Array GetRoles();

		std::string GetJoinedAt() const;

		bool IsDeaf() const;

		bool IsMute() const;

		static void Register_ServerMember(Table &table);
	};

} // namespace SqDiscord
