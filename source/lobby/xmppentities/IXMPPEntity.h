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

#ifndef IXMPPENTITY_H
#define IXMPPENTITY_H

#include "scriptinterface/ScriptTypes.h"

#include <type_traits>

class IXMPPClient2;
class ScriptRequest;

template<typename ConcreteType>
class IXMPPEntity
{
public:
	virtual ~IXMPPEntity() = default;

	/**
	 * Gets the corresponding JSValue for this entity.
	 */
	virtual JS::Value ToJSVal(const ScriptRequest& rq) = 0;

	/**
	 * Push or pull new changes to the entity to the server.
	 */
	virtual bool Sync(IXMPPClient2&) = 0;

	virtual void Trace(JSTracer*) = 0;

	virtual void SetAttribute(const std::string& attribute, const std::string& value) = 0;


	static bool SetAttribute(JSContext* cx, uint argc, JS::Value* vp)
	{
		JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
		args.rval().setUndefined();

		// Check parameter count
		if (argc > 2 || argc < 2)
			return false;

		// Check parameter type
		if (!args[0].isString() || !args[1].isString())
			return false;

		JS::HandleValue jsThis = args.thisv();

		// TODO: safe ref?
		ConcreteType* concreteType = static_cast<ConcreteType*>(JS_GetReservedSlot(&jsThis.toObject(), 0).toPrivate());

		ScriptRequest rq(cx);
		std::string attribute;
		std::string value;
		Script::FromJSVal(rq, args[0], attribute);
		Script::FromJSVal(rq, args[1], value);

		concreteType->SetAttribute(attribute, value);

		return true;
	}

	static void Trace(JSTracer* tracer, void* context)
	{
		static_cast<ConcreteType*>(context)->Trace(tracer);
	}
};

#endif // IXMPPENTITY_H
