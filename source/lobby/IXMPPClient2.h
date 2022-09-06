/* Copyright (C) 2022 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IXMPPCLIENT2_H
#define IXMPPCLIENT2_H

class XMPPUser {
private:
    std::string m_jid;
    std::string m_username;
    u16 m_rating;
    u16 m_gamesPlayed;
    u16 m_gamesWon;
    u16 m_gamesLost;
};

class XMPPGame {
private:
    std::string m_hostJID;
    std::map<std::string, std::string> m_gameData;
};

class IXMPPClient2
{
public:
    virtual IXMPPClient2::~IXMPPClient2() = default;

    // Allow unauthenticated
    virtual bool Login(const std::string& username, const std::string& password) = 0;
    virtual bool Register(const std::string& username, const std::string& password) = 0;
    
    // Requires being authenticated
    virtual void Logout() = 0;
    virtual bool ChangePassword(const std::string& newPassword) = 0;

    // Restrict clients to join only one MUC at a time.
    virtual bool JoinMUC(const std::string& room) = 0;
    virtual bool LeaveMUC() = 0;

    // Basic XMPP functions
    virtual bool SendMessage(const std::string& toUsername, const std::string& message) = 0;
    virtual bool SendMessageToMUC(const std::string& message) = 0;
    virtual bool SetNick(const std::string& nick) = 0;
    virtual bool SetPresence(const std::string& status) = 0;

    // Member info
    virtual const XMPPUser& GetUser(const std::string& username) const = 0;
    virtual const std::list<XMPPUser>& GetUsers() const = 0;

    // Pyrogenesis functions
    virtual const std::list<XMPPUser>& GetLeaderboard() const = 0;
    virtual bool RegisterGame(const XMPPGame&) = 0;
    virtual bool UnRegisterGame(const XMPPGame&) = 0;
    virtual bool ReportGame(const XMPPGame&) = 0;
    virtual const XMPPGame& GetGame(const std::string& hostUsername) const = 0;
    virtual const std::list<XMPPGame>& GetGames() const = 0;
};

#endif // XMPPCLIENT2_H
