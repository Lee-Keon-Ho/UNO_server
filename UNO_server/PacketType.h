#pragma once

enum ePacketType
{
	CS_PT_LOGIN = 1,
	CS_PT_CREATEROOM,
	CS_PT_USERLIST,
	CS_PT_ROOMLIST,
	CS_PT_INROOM,
	CS_PT_OUTROOM,
	CS_PT_ROOMSTATE,
	CS_PT_MAX
};