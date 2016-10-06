/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#ifndef OPTIONSSTATE_H
#define OPTIONSSTATE_H

#include "IGameState.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_String.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include <xaudio2.h>

class OptionsState : public IGameState {

	SGD::Rectangle m_optionRect;
	SGD::Rectangle m_MastervolRect;
	SGD::Rectangle m_SFXvolRect;

	SGD::HAudio m_sfxBoomShot = SGD::INVALID_HANDLE;
	SGD::HAudio m_background = SGD::INVALID_HANDLE;
	
public:

	static OptionsState* GetInstance(void);

	// IGameState Interface:
	virtual void	Enter(void)				override;	// load resources
	virtual void	Exit(void)				override;	// unload resources
	virtual bool	Update(float elapsedTime)	override;	// handle input & update entites
	virtual void	Render(float elapsedTime)	override;	// render entities / menu


private:
		// SINGLETON!
	OptionsState(void) = default;
	virtual ~OptionsState(void) = default;
	OptionsState(const OptionsState&) = delete;
	OptionsState& operator= (const OptionsState&) = delete;

	int		m_nCursor = 0;
};

#endif //OptionsState_H