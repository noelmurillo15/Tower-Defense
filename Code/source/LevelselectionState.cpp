/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Game.h"

#include "LevelselectionState.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


LevelselectionState* LevelselectionState::GetInstance(void) {

	static LevelselectionState s_Instance;
	return &s_Instance;
}

void LevelselectionState::Enter(void) {

	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 155, 0, 255 });
}

void LevelselectionState::Exit(void) {

}

bool LevelselectionState::Update(float elapsedTime) {

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

		// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());


		// Move cursor
	if (pInput->IsKeyPressed(SGD::Key::Down) == true) {

		++m_nCursor;

		if (m_nCursor > 2)
			m_nCursor = 0;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true) {

		--m_nCursor;

		if (m_nCursor < 0)
			m_nCursor = 2;
	}

	if (pInput->IsKeyPressed(SGD::Key::Enter) == true) {

			// Which level?
		if (m_nCursor == 0)		// Easy
			m_nLevel = 1;

		else if (m_nCursor == 1)	// Medium
			m_nLevel = 2;
		
		else if (m_nCursor == 2)	// Hard
			m_nLevel = 3;
		
		Game::GetInstance()->ChangeState(GameplayState::GetInstance());
		return true;	//	play in curr state
	}
	return true;	// keep looping
}

void LevelselectionState::Render(float elapsedTime) {

		// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

		// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();


		// Display the Level select centered at 2x scale
	pFont->Draw("Level Select", { (width - ( 1.5f * 4 * 32 * 4.0f)) / 2, 50 },
		2.0f, { 255, 0, 0 });

		// Display the level options centered at 1x scale
	pFont->Draw("Easy", { (width - (4 * 32)) / 2, 300 },
		1.0f, { 255, 0, 0 });
	pFont->Draw("Medium", { (width - (4 * 32)) / 2, 350 },
		1.0f, { 255, 0, 0 });
	pFont->Draw("Hard", { (width - (4 * 32)) / 2, 400 },
		1.0f, { 255, 0, 0 });


	if (m_nCursor == 0)
		pFont->Draw("=     =", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 255, 0 });

	else if (m_nCursor == 1)
		pFont->Draw("=       =", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 255, 0 });

	else if (m_nCursor == 2)
		pFont->Draw("=     =", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 255, 0 });

}