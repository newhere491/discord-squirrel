#include "User.hpp"

namespace SqDiscord
{

	User::User(const dpp::user &user)
	{
		ID = user.id;
		Username = user.username;
		Discriminator = user.discriminator;
		isBot = user.is_bot();
		Mfa_enabled = user.is_mfa_enabled();
		Verified = user.is_verified();
	}

	std::string User::GetID() const
	{
		return ID;
	}

	std::string User::GetUsername() const
	{
		return Username;
	}

	std::string User::GetDiscriminator() const
	{
		return Discriminator;
	}

	bool User::IsBot() const
	{
		return isBot;
	}

	bool User::IsMfaEnabled() const
	{
		return Mfa_enabled;
	}

	bool User::IsVerified() const
	{
		return Verified;
	}

	nlohmann::json User::ToJson() const
	{
		nlohmann::json json;

		json["id"] = ID;
		json["username"] = Username;
		json["discriminator"] = Discriminator;
		json["isBot"] = isBot;
		json["mfa_enabled"] = Mfa_enabled;
		json["verified"] = Verified;

		return json;
	}

	void User::Register_User(Table &table)
	{
		table.Bind(_SC("User"),
				   Class<User>(table.GetVM(), "User")
					   .Prop(_SC("ID"), &User::GetID)
					   .Prop(_SC("Username"), &User::GetUsername)
					   .Prop(_SC("Discriminator"), &User::GetDiscriminator)
					   .Prop(_SC("IsBot"), &User::IsBot)
					   .Prop(_SC("Mfa_enabled"), &User::IsMfaEnabled)
					   .Prop(_SC("Verified"), &User::IsVerified));
	}
} // Namespace:: SqDiscord