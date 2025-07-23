#include "ServerMember.hpp"

namespace SqDiscord
{

	ServerMember::ServerMember(const dpp::guild_member &member, const dpp::user &userObj)
	{
		ID = std::to_string(userObj.id);
		User = DUser(userObj);
		Nick = member.get_nickname();
		JoinedAt = member.joined_at;
		isDeaf = member.is_deaf();
		isMute = member.is_muted();

		// Convert roles
		for (const auto &role_id : member.get_roles())
		{
			Roles.push_back(std::to_string(role_id));
		}
	}

	void ServerMember::UpdateRoles(const std::vector<dpp::snowflake> &roles)
	{
		Roles.clear();
		for (const auto &role : roles)
		{
			Roles.push_back(std::to_string(role));
		}
	}

	std::string ServerMember::GetID() const
	{
		return ID;
	}

	nlohmann::json ServerMember::GetUser()
	{
		// Assume DUser::ToSquirrelObject() or similar exists
		return User.ToJson();
	}

	std::string ServerMember::GetNick() const
	{
		return Nick;
	}

	int ServerMember::GetRoleCount() const
	{
		return static_cast<int>(Roles.size());
	}

	Array ServerMember::GetRoles()
	{
		Array arr;
		for (const auto &r : Roles)
		{
			arr.Append(r);
		}
		return arr;
	}

	std::string ServerMember::GetJoinedAt() const
	{
		return JoinedAt;
	}

	bool ServerMember::IsDeaf() const
	{
		return isDeaf;
	}

	bool ServerMember::IsMute() const
	{
		return isMute;
	}

	// Register_ServerMember implementation here if needed

	void ServerMember::Register_ServerMember(Table &table)
	{
		table.Bind(_SC("ServerMember"),
				   Class<ServerMember>(table.GetVM(), "ServerMember")
					   .Prop(_SC("ID"), &ServerMember::GetID)
					   .Prop(_SC("User"), &ServerMember::GetUser)
					   .Prop(_SC("Nick"), &ServerMember::GetNick)
					   .Prop(_SC("RoleCount"), &ServerMember::GetRoleCount)
					   .Prop(_SC("Roles"), &ServerMember::GetRoles)
					   .Prop(_SC("JoinedAt"), &ServerMember::GetJoinedAt)
					   .Prop(_SC("IsDeaf"), &ServerMember::IsDeaf)
					   .Prop(_SC("IsMute"), &ServerMember::IsMute));
	}
} // Namespace:: SqDiscord