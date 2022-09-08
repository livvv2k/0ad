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

#ifndef XMPPGAME_H
#define XMPPGAME_H

#include "lobby/xmppentities/IXMPPEntity.h"

#include <map>
#include <string>

class ScriptInterface;
class XMPPUser;

class XMPPGame final : IXMPPEntity<XMPPGame>
{
public:
	XMPPGame(const XMPPUser& host, bool already_registered = false) : m_host(host), m_registered(true), m_dirty(false) {};
	~XMPPGame() override;

	void SetAttribute(const std::string& attribute, const std::string& value) override;

	JS::Value ToJSVal(const ScriptRequest& rq) override;

	void Trace(JSTracer*) override;

	bool Sync(IXMPPClient2&) override;

private:
	bool m_dirty;
	bool m_registered;
	const XMPPUser& m_host;
	std::map<std::string, std::string> m_gameData;

	JS::Heap<JS::Value> m_jsValue;
	const ScriptInterface* m_scriptInterface;
};

#endif // XMPPGAME_H
