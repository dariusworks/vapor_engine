/************************************************************************
*
* vapor3D Server � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Protocol/API.h"
#include "Protocol/UserMessages.h"
#include "Network/MessageHandlers.h"

NAMESPACE_PROTOCOL_BEGIN

//-----------------------------------//

REFLECT_ENUM(UserAuthType)
	ENUM(None)
	ENUM(Password)
	ENUM(Certificate)
REFLECT_ENUM_END()

REFLECT_ENUM(UserMessageIds)
	ENUM(UserAuth)
	ENUM(UserAuthStatus)
	ENUM(UserChangeName)
	ENUM(UserJoin)
	ENUM(UserLeave)
	ENUM(UserNotifyName)
REFLECT_ENUM_END()

REFLECT_CHILD_CLASS(UserAuthMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, string, name)
	FIELD_ENUM(1, UserAuthType, type)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(UserAuthStatusMessage, MessageDefinition)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(UserJoinMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, uint8, user)
	FIELD_PRIMITIVE(1, string, name)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(UserLeaveMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, uint8, user)
REFLECT_CLASS_END()

REFLECT_CHILD_CLASS(UserChangeNameMessage, MessageDefinition)
	FIELD_PRIMITIVE(0, uint8, user)
	FIELD_PRIMITIVE(1, string, name)
REFLECT_CLASS_END()

//-----------------------------------//

IMPLEMENT_HANDLER_REF(User, UserAuth)
IMPLEMENT_HANDLER_REF(User, UserAuthStatus)
IMPLEMENT_HANDLER_REF(User, UserChangeName)
IMPLEMENT_HANDLER_REF(User, UserJoin)
IMPLEMENT_HANDLER_REF(User, UserLeave)
//IMPLEMENT_HANDLER_REF(User, UserNotifyName)

PROTOCOL_MESSAGE_HANDLERS(User)
	HANDLER_REF(User, UserAuth, ClientToServer)
	HANDLER_REF(User, UserAuthStatus, ServerToClient)
	HANDLER_REF(User, UserChangeName, ClientToServer)
	HANDLER_REF(User, UserJoin, ServerToClient)
	HANDLER_REF(User, UserLeave, ServerToClient)
	//HANDLER_REF(User, UserNotifyName, ServerToClient)
PROTOCOL_MESSAGE_HANDLERS_END()

//-----------------------------------//

REFLECT_ABSTRACT_CHILD_CLASS(UserMessageHandler, MessageHandler)
REFLECT_CLASS_END()

//-----------------------------------//

const MessagesTable& UserMessageHandler::getMessagesTable()
{
	static MessagesTable gs_UserMessages(gs_UserRawMessages, gs_UserRawMessages + ARRAY_SIZE(gs_UserRawMessages));
	return gs_UserMessages;
}

//-----------------------------------//

Enum* UserMessageHandler::getMessagesEnum()
{
	return ReflectionGetType(UserMessageIds);
}

//-----------------------------------//

NAMESPACE_PROTOCOL_END