/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Game.h"

#include "CreditsState.h"
#include "MainMenuState.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


CreditsState* CreditsState::GetInstance(void) {

	static CreditsState s_Instance;
	return &s_Instance;
}

void CreditsState::Enter(void) {

	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 200, 200, 200 });
}

void CreditsState::Exit(void) {

}

bool CreditsState::Update(float elapsedTime) {
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

		// Press Escape to return to Main Menu
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true) {

		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;	// keep playing in the new state
	}
	return true;		//	Keep looping
}

void CreditsState::Render(float elapsedTime) {

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();


	// Display the game title centered at 2x scale
	pFont->Draw("Credits : ", { (width - (1.5f * 4 * 32 * 4.0f)) / 2, 50 },
		1.5f, { 0, 0, 0 });

	// Display the menu options centered at 1x scale
	pFont->Draw("Developed by : ", { (width - (2 * 32)) / 2, 300 },
		1.0f, { 0, 0, 0 });
	pFont->Draw("Allan Murillo,", { (width - (2 * 32)) / 2, 350 },
		0.8f, { 0, 0, 0 });
	pFont->Draw("Structure of Game design at Full Sail University,", { (width - (2 * 32)) / 2, 400 },
		0.3f, { 0, 0, 0 });
	pFont->Draw("Mario", { (width - (2 * 32)) / 2, 450 },
		0.8f, { 0, 0, 0 });

}