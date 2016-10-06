/***************************************************************
|	File:		IGameState.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	IGameState class declares the interface that all
|				child states must implement
***************************************************************/

#ifndef IGAMESTATE_H
#define IGAMESTATE_H

class IGameState {

protected:

	IGameState( void )			= default;
	virtual ~IGameState( void )	= default;


public:

	// IGameState Interface:
	virtual void	Enter( void )				= 0;	// load resources
	virtual void	Exit ( void )				= 0;	// unload resources
	virtual bool	Update( float elapsedTime )	= 0;	// handle input & update entites
	virtual void	Render( float elapsedTime )	= 0;	// render entities / menu
};

#endif //IGAMESTATE_H
