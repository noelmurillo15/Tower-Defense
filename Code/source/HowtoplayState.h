/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#ifndef HOWTOPLAYSTATE_H
#define HOWTOPLAYSTATE_H

#include "IGameState.h"

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"


class HowtoplayState : public IGameState {

	SGD::HTexture m_next = SGD::INVALID_HANDLE;
	SGD::Rectangle m_nextRect;

	SGD::HTexture m_Objective;
	SGD::HTexture m_Controls;
	SGD::HTexture m_Player;
	SGD::HTexture m_Enemy;

	unsigned int Page = 1;

public:

	static HowtoplayState* GetInstance(void);

		// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources
	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu

private:
		// SINGLETON!
	HowtoplayState(void) = default;
	virtual ~HowtoplayState(void) = default;
	HowtoplayState(const HowtoplayState&) = delete;
	HowtoplayState& operator= (const HowtoplayState&) = delete;

	int		m_nCursor = 0;
};

#endif //HOWTOPLAYSTATE_H