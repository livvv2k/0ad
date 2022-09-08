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

#include "XMPPUser.h"

#include "lobby/IXMPPClient2.h"
#include "lobby/glooxwrapper/glooxwrapper.h"
#include "lobby/stanzaextensions/StanzaExtensions.h"
#include "lobby/xmppentities/XMPPUser.h"
#include "ps/ConfigDB.h"
#include "scriptinterface/Object.h"
#include "scriptinterface/ScriptInterface.h"

XMPPUser::XMPPUser(const std::string& jid)
{
	// TODO
}

XMPPUser::~XMPPUser()
{
	if (m_scriptInterface)
		JS_RemoveExtraGCRootsTracer(m_scriptInterface->GetGeneralJSContext(), IXMPPEntity::Trace, this);
}

JS::Value XMPPUser::ToJSVal(const ScriptRequest& rq)
{
	const ScriptInterface* scriptInterface = &rq.GetScriptInterface();
	if (m_scriptInterface == scriptInterface)
		return m_jsValue;

	JS_RemoveExtraGCRootsTracer(m_scriptInterface->GetGeneralJSContext(), IXMPPEntity::Trace, this);
	m_scriptInterface = scriptInterface;

	// This object has constant properties
	JS::RootedValue jsVal(rq.cx);
	Script::SetProperty(rq, jsVal, "jid", m_jid, true);
	Script::SetProperty(rq, jsVal, "username", m_username, true);
	for (const auto& [attribute, value] : m_userData)
		Script::SetProperty(rq, jsVal, attribute, value, true);

	JS_SetReservedSlot(&jsVal.toObject(), 0, JS::PrivateValue(this));
	m_jsValue = JS::Heap<JS::Value>(jsVal);

	JS_AddExtraGCRootsTracer(m_scriptInterface->GetGeneralJSContext(), IXMPPEntity::Trace, this);

	return m_jsValue;
}

void XMPPUser::SetAttribute(const std::string& attribute, const std::string& value)
{
	// noop as JS object is constant
}

bool XMPPUser::Sync(IXMPPClient2& xmpp)
{
	// Readonly sync called periodically upon roster change.
	// TODO: Do that
	return true;
}

void XMPPUser::Trace(JSTracer* tracer)
{
	JS::TraceEdge(tracer, &m_jsValue, "XMPPUser::m_jsValue");
}
