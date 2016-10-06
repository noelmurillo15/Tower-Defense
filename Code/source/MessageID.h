/***************************************************************
|	File:		MessageID.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	MessageID enum declares the message enumerators
***************************************************************/

#ifndef MESSAGEID_H
#define MESSAGEID_H


enum class MessageID 
{  
	MSG_DESTROY_ENTITY,
	MSG_CREATE_ENEMY,
	MSG_CREATE_PLASMA,
	MSG_CREATE_FIREBALL,
	MSG_CREATE_WIND,
};

#endif //MESSAGEID_H