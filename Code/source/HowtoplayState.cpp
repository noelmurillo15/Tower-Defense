/***************************************************************
|	File:		Game.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Game.h"

#include "HowtoplayState.h"
#include "MainMenuState.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"


HowtoplayState* HowtoplayState::GetInstance(void) {

	static HowtoplayState s_Instance;
	return &s_Instance;
}

void HowtoplayState::Enter(void) {

	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 140, 140, 140 });

	m_next = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_HTP_Next.png");
	m_Objective = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_Objectives.png");;
	m_Controls = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_Controls.png");;
	m_Player = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_Heroes.png");;
	m_Enemy = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/ANM_Enemies.png");;

	m_nextRect = {
		1024 - 200, 768 - 140, 1024 - 80, 768 - 40
	};
}

void HowtoplayState::Exit(void) {
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_next);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Objective);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Controls);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Player);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_Enemy);

	Page = 1;
}

bool HowtoplayState::Update(float elapsedTime) {
	SGD::InputManager* input = SGD::InputManager::GetInstance();
	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };

	if (input->IsKeyPressed(SGD::Key::MouseLeft) == true && cursorrect.IsIntersecting(m_nextRect) && Page != 4){
		Page++;
	}
	else if (Page == 4 && input->IsKeyPressed(SGD::Key::MouseLeft) == true){
		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;	// keep playing in the new state
	}

		// Press Escape to return to Main Menu
	if (input->IsKeyPressed(SGD::Key::Escape) == true) {

		Game::GetInstance()->ChangeState(MainMenuState::GetInstance());
		return true;	// keep playing in the new state
	}
	return true;		// keep looping
}

void HowtoplayState::Render(float elapsedTime) {

	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();

	//	cursor pos
	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };
	
	if (cursorrect.IsIntersecting(m_nextRect))
		graphics->DrawRectangle(m_nextRect,
			SGD::Color(100, 100, 100));

	graphics->DrawTexture(m_next, SGD::Point(1024 - 200, 768 - 150),
		0.0f,
		{},
		SGD::Color(255,255,255,255),
		SGD::Size(0.25f, 0.25f));

	if (Page == 1){
		graphics->DrawTexture(m_Objective, SGD::Point(0, 0));
	}
	else if (Page == 2){
		graphics->DrawTexture(m_Controls, SGD::Point(0, 0));
	}
	else if (Page == 3){
		graphics->DrawTexture(m_Player, SGD::Point(0, 0));
	}
	else if (Page == 4){
		graphics->DrawTexture(m_Enemy, SGD::Point(0, 0));
	}
}