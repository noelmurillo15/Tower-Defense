/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#ifndef CREDITSSTATE_H
#define CREDITSSTATE_H

#include "IGameState.h"


class CreditsState : public IGameState {

public:
		//	Singleton:
	static CreditsState* GetInstance(void);

		// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources
	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu

private:
		// SINGLETON!
	CreditsState(void) = default;
	virtual ~CreditsState(void) = default;
	CreditsState(const CreditsState&) = delete;
	CreditsState& operator= (const CreditsState&) = delete;

	int		m_nCursor = 0;
};

#endif //CREDITSSTATE_H