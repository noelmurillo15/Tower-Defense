/***************************************************************
|	File:		GameplayState.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	GameplayState class initializes & runs the game logic
***************************************************************/

#ifndef GAMEPLAYSTATE_H
#define GAMEPLAYSTATE_H

#include "IGameState.h"							// uses IGameState
#include "../SGD Wrappers/SGD_Declarations.h"	// uses Message
#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include "../SGD Wrappers/SGD_String.h"

class Entity;
class EntityManager;


class GameplayState : public IGameState {

		//	Pause System
	struct Pause{
		bool isPaused = false;
		SGD::Rectangle m_PauseScreen;
		int m_nCursor = 1;
	};
		//	Win / Lose System
	struct EndGame{

		int WavesToBeat = 0;

		bool Losing = false;
		bool Winning = false;
		bool GameOver = false;
	};
		//	Interface System
	struct Interface{
		SGD::Rectangle m_FireMageRect;
		SGD::Rectangle m_AquaGoddessRect;
		SGD::Rectangle m_ZephyrRect;
		SGD::Rectangle m_SelectedHero;

		SGD::Rectangle m_HeroSpawnRect;

		unsigned int m_SelectedType;
		bool IsSelected = false;
	};
	Pause m_pause;
	EndGame m_endgame;
	Interface m_interface;

public:

		// Singleton Accessor:
	static GameplayState* GetInstance( void );

		// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources
	virtual void	Exit	( void )				override;	// unload resources
													
	virtual bool	Update	( float elapsedTime )	override;	// handle input & update entities
	virtual void	Render	( float elapsedTime )	override;	// render game entities / menus

private:
		// SINGLETON (not-dynamically allocated)
	GameplayState( void ) = default;	// default constructor
	virtual ~GameplayState( void )	= default;	// destructor
	GameplayState( const GameplayState& )				= delete;	// copy constructor
	GameplayState& operator= ( const GameplayState& )	= delete;	// assignment operator

		//	Game Entities
	EntityManager*			m_pEntities			= nullptr;

		//	Assets
	SGD::HTexture m_hTower = SGD::INVALID_HANDLE;
	SGD::HTexture m_hCharacters = SGD::INVALID_HANDLE;
	SGD::HTexture m_Plasma = SGD::INVALID_HANDLE;
	SGD::HTexture m_Fireball = SGD::INVALID_HANDLE;
	SGD::HTexture m_Windshuri = SGD::INVALID_HANDLE;
	SGD::HTexture m_EnemySleep = SGD::INVALID_HANDLE;

	SGD::HAudio		m_PlasmaSfx = SGD::INVALID_HANDLE;
	SGD::HAudio		m_win = SGD::INVALID_HANDLE;
	SGD::HAudio		m_lose = SGD::INVALID_HANDLE;

		//	Creation Methods
	Entity* CreateHero(SGD::HTexture img, unsigned int type) const;
	Entity* CreateEnemy(SGD::HTexture img, int type)const;

	Entity* CreateFireBall(SGD::Point pos, SGD::Vector, Entity* caster) const;
	Entity*	CreateWindShuriken(SGD::Point pos, SGD::Vector, Entity* caster)	const;
	Entity*	CreatePlasma(SGD::Point pos, SGD::Vector)	const;

		// Message Callback Function:
	static void MessageProc( const SGD::Message* pMsg );
};

#endif //GAMEPLAYSTATE_H
