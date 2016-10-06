/***************************************************************
|	File:		MainMenuState.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	MainMenuState class handles the main menu
***************************************************************/

#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "IGameState.h"

class MainMenuState : public IGameState {

public:

		// Singleton Accessor
	static MainMenuState* GetInstance( void );

		// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources
	virtual void	Exit	( void )				override;	// unload resources
	virtual bool	Update	( float elapsedTime )	override;	// handle input & update entites
	virtual void	Render	( float elapsedTime )	override;	// render entities / menu

private:
		
		// SINGLETON!
	MainMenuState( void )			= default;
	virtual ~MainMenuState( void )	= default;
	MainMenuState( const MainMenuState& )				= delete;	
	MainMenuState& operator= ( const MainMenuState& )	= delete;

		// Cursor Index
	int		m_nCursor = 0;
};

#endif //MAINMENUSTATE_H
