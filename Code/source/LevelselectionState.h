/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#ifndef LEVELSELECTIONSTATE_H
#define LEVELSELECTIONSTATE_H

#include "IGameState.h"


class LevelselectionState : public IGameState {

public:

	static LevelselectionState* GetInstance(void);

	int GetLevel(){ return m_nLevel; }

	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources
	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu


private:

	// SINGLETON!
	LevelselectionState(void) = default;
	virtual ~LevelselectionState(void) = default;
	LevelselectionState(const LevelselectionState&) = delete;
	LevelselectionState& operator= (const LevelselectionState&) = delete;

	int		m_nCursor = 0;
	int		m_nLevel = 0;
};

#endif //LEVELSELECTIONSTATE_H