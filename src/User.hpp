#pragma once

#include "includes.hpp"
#include <dpp/user.h>

namespace SqDiscord
{

	class User
	{
	public:
		std::string ID;
		std::string Username;
		std::string Discriminator;
		bool isBot = false;
		bool Mfa_enabled = false; // true if two-factor authentication is enabled
		bool Verified = false;	  // true if email has been verified

		User() = default;

		User(const dpp::user &user);

		User(const User &o) = default;

		User(User &&o) = default;

		User &operator=(const User &o) = default;

		User &operator=(User &&o) = default;

		~User() = default;

		std::string GetID() const;

		std::string GetUsername() const;

		std::string GetDiscriminator() const;

		bool IsBot() const;

		bool IsMfaEnabled() const;

		bool IsVerified() const;

		nlohmann::json User::ToJson() const;

		static void Register_User(Sqrat::Table &table);
	};
} // Namespace:: SqDiscord