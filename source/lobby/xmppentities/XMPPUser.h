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

#ifndef XMPPUSER_H
#define XMPPUSER_H

#include "lobby/xmppentities/IXMPPEntity.h"

class XMPPUser final : IXMPPEntity<XMPPUser>
{
public:
	XMPPUser(const std::string& jid);
	~XMPPUser() override;

	bool operator ==(const XMPPUser& other) const { return m_jid == other.m_jid; }
	bool operator !=(const XMPPUser& other) const { return m_jid != other.m_jid; }

	static std::string ConstructJID(const std::string& username);

	const std::string& GetJID() const { return m_jid; }
	const std::string& GetUsername() const { return m_username; }

	JS::Value ToJSVal(const ScriptRequest& rq) override;

	void Trace(JSTracer*) override;

	void SetAttribute(const std::string& attribute, const std::string& value) override;

	bool Sync(IXMPPClient2&) override;

private:
	bool m_dirty;
	std::string m_jid;
	std::string m_username;
	std::map<std::string, std::string> m_userData;

	JS::Heap<JS::Value> m_jsValue;
	const ScriptInterface* m_scriptInterface;
};

#endif // XMPPUSER_H
