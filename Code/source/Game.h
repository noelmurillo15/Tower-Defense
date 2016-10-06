/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo	
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#ifndef GAME_H
#define GAME_H

	// Forward class declaration
class BitmapFont;
class IGameState;
class Entity;
class EntityManager;

#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio


class Game {

public:
	
		// Singleton Accessors:
	static Game*	GetInstance		( void );
	static void		DeleteInstance	( void );
	
		// Setup, Play, Cleanup:
	bool Initialize	( float width, float height );
	int	 Update		( void );
	void Terminate	( void );
	
		// Screen Size Accessors:
	float				GetScreenWidth	( void ) const	{	return m_fScreenWidth;	}
	float				GetScreenHeight ( void ) const	{	return m_fScreenHeight;	}
	bool				GetFullScreen()const			{ return m_fullScreenMode; }
	void				SetFullScreen(bool boo)			{ m_fullScreenMode = boo; }

		// Font Accessor: (#include "BitmapFont.h" to use!)
	const BitmapFont*	GetFont			( void ) const	{	return m_pFont;			}

		// Game State Machine:
		//	- can ONLY be called by the state's Input, Update, or Render methods!!!
	void ChangeState( IGameState* pNewState );

private:
		// Singleton Object:
	static Game*	s_pInstance;

	Game( void )	= default;		// default constructor
	~Game( void )	= default;		// destructor
	Game( const Game& )				= delete;	// copy constructor
	Game& operator= ( const Game& )	= delete;	// assignment operator

	
		// Screen Size
	float					m_fScreenWidth	= 1;
	float					m_fScreenHeight = 1;	
		// Game Font
	BitmapFont*				m_pFont			= nullptr;
		// Current Game State
	IGameState*				m_pCurrState	= nullptr;
		// Game Time
	unsigned long			m_ulGameTime	= 0;

	bool m_fullScreenMode = false;

	SGD::HAudio m_backgroundtheme = SGD::INVALID_HANDLE;
};

#endif //GAME_H