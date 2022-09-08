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

#include "precompiled.h"

#include "XMPPGame.h"

#include "lobby/IXMPPClient2.h"
#include "lobby/glooxwrapper/glooxwrapper.h"
#include "lobby/stanzaextensions/StanzaExtensions.h"
#include "lobby/xmppentities/XMPPUser.h"
#include "ps/ConfigDB.h"
#include "scriptinterface/Object.h"
#include "scriptinterface/ScriptInterface.h"

XMPPGame::~XMPPGame()
{
	if (m_scriptInterface)
		JS_RemoveExtraGCRootsTracer(m_scriptInterface->GetGeneralJSContext(), IXMPPEntity::Trace, this);
}

JS::Value XMPPGame::ToJSVal(const ScriptRequest& rq)
{
	const ScriptInterface* scriptInterface = &rq.GetScriptInterface();
	if (m_scriptInterface == scriptInterface)
		return m_jsValue;

	JS_RemoveExtraGCRootsTracer(m_scriptInterface->GetGeneralJSContext(), IXMPPEntity::Trace, this);
	m_scriptInterface = scriptInterface;

	JS::RootedValue jsVal(rq.cx);
	for (const auto& [attribute, value] : m_gameData)
		Script::SetProperty(rq, jsVal, attribute, value);

	JS_SetReservedSlot(&jsVal.toObject(), 0, JS::PrivateValue(this));
	m_jsValue = JS::Heap<JS::Value>(jsVal);

	JS_AddExtraGCRootsTracer(m_scriptInterface->GetGeneralJSContext(), IXMPPEntity::Trace, this);

	return m_jsValue;
}

void XMPPGame::SetAttribute(const std::string& attribute, const std::string& value)
{
	m_gameData.insert_or_assign(attribute, value);
	m_dirty = true;
}

bool XMPPGame::Sync(IXMPPClient2& xmpp)
{
	if (xmpp.GetSelf() != m_host)
		return false;

	if (!m_registered) {
		// Gloox take ownership of the raw ptr so smart_ptrs are not that useful as it needs to be released anyway
		GameListQuery* query = new GameListQuery();
		query->m_Command = "register";
		glooxwrapper::Tag* game = glooxwrapper::Tag::allocate("game");

		for (const auto& [attribute, value] : m_gameData)
			game->addAttribute(attribute, value);

		// Overwrite some attributes to make it slightly less trivial to do bad things,
		// and explicit some invariants.

		// The JID must point to ourself.
		game->addAttribute("hostJID", m_host.GetJID());
		query->m_GameList.emplace_back(game);

		m_registered = xmpp.SendCustomStanza(xmpp.GetUser(XMPPUser::ConstructJID(xmpp.GetServerDetails().m_xpartamupp)), query);
		m_dirty = !m_registered;
		return m_registered;
	}

	if (m_dirty) {
		GameListQuery* query = new GameListQuery();
		query->m_Command = "changestate";
		glooxwrapper::Tag* game = glooxwrapper::Tag::allocate("game");
		game->addAttribute("nbp", m_gameData["nbp"]);
		game->addAttribute("players", m_gameData["players"]);
		query->m_GameList.emplace_back(game);
		m_dirty = !xmpp.SendCustomStanza(xmpp.GetUser(XMPPUser::ConstructJID(xmpp.GetServerDetails().m_xpartamupp)), query);
		return !m_dirty;
	}

	return true;
}

void XMPPGame::Trace(JSTracer* tracer)
{
	JS::TraceEdge(tracer, &m_jsValue, "XMPPGame::m_jsValue");
}
