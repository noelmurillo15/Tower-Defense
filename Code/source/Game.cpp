/***************************************************************
|	File:		Game.cpp
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Game class controls the SGD wrappers
|				& runs the game state machine
***************************************************************/

#include "Game.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"

#include "BitmapFont.h"
#include "IGameState.h"
#include "MainMenuState.h"
#include "Enemies.h"
#include "EntityManager.h"

#include <ctime>
#include <cstdlib>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>



Game* Game::s_pInstance = nullptr;

Game* Game::GetInstance( void ) {

	if( s_pInstance == nullptr )
		s_pInstance = new Game;

	return s_pInstance;
}

void Game::DeleteInstance( void ) {

	delete s_pInstance;
	s_pInstance = nullptr;
}

bool Game::Initialize( float width, float height ) {

		// Seed First!
	srand( (unsigned int)time( nullptr ) );
	rand();

		// Initialize the wrappers
	if( SGD::AudioManager::GetInstance()->Initialize() == false 
		|| SGD::GraphicsManager::GetInstance()->Initialize( false ) == false
		|| SGD::InputManager::GetInstance()->Initialize() == false )
		return false;

	m_backgroundtheme = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/ANM_Background.xwm");

		// Store the size parameters
	m_fScreenWidth	= width;
	m_fScreenHeight = height;

		// Allocate & initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize();

		// Start the game in the Main Menu state
	ChangeState( MainMenuState::GetInstance() );
	
		// Store the current time (in milliseconds)
	m_ulGameTime	= GetTickCount();

	if (m_backgroundtheme != SGD::INVALID_HANDLE)
		SGD::AudioManager::GetInstance()->PlayAudio(m_backgroundtheme, true);

	return true;	// success!
}

int Game::Update( void ) {

	// Update the wrappers
	if( SGD::AudioManager::GetInstance()->Update() == false 
		|| SGD::GraphicsManager::GetInstance()->Update() == false
		|| SGD::InputManager::GetInstance()->Update() == false )
		return -10;		// exit FAILURE!

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	
		// Calculate the elapsed time between frames
	unsigned long now = GetTickCount();					// current time in milliseconds
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;	// convert to fraction of a second
	m_ulGameTime = now;									// store the current time

		// Cap the elapsed time to 1/8th of a second
	if( elapsedTime > 0.125f )
		elapsedTime = 0.125f;
	
		// Let the current state handle input
	if( m_pCurrState->Update( elapsedTime ) == false )
		return 1;	// exit success!	

	if (pInput->IsKeyDown(SGD::Key::Alt) == true && pInput->IsKeyPressed(SGD::Key::Enter) == true && !Game::GetInstance()->GetFullScreen()){
		SGD::GraphicsManager::GetInstance()->Resize(SGD::Size(Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight()), false);
		Game::GetInstance()->SetFullScreen(true);
	}
	else if (pInput->IsKeyDown(SGD::Key::Alt) == true && pInput->IsKeyPressed(SGD::Key::Enter) == true && Game::GetInstance()->GetFullScreen()){
		SGD::GraphicsManager::GetInstance()->Resize(SGD::Size(Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight()), true);
		Game::GetInstance()->SetFullScreen(false);
	}
	
		// Let the current state render
	m_pCurrState->Render( elapsedTime );
	
	return 0;		// keep playing!
}

void Game::Terminate( void ) {

		// Exit the current state
	ChangeState( nullptr );

	SGD::AudioManager::GetInstance()->UnloadAudio(m_backgroundtheme);

		// Terminate & deallocate the font
	m_pFont->Terminate();
	delete m_pFont;
	m_pFont = nullptr;

		// Terminate the core SGD wrappers
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();

	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();

	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();
}

void Game::ChangeState( IGameState* pNewState ) {

		// Exit the old state
	if( m_pCurrState != nullptr )
		m_pCurrState->Exit();

		// Store the new state
	m_pCurrState = pNewState;

		// Enter the new state
	if( m_pCurrState != nullptr )
		m_pCurrState->Enter();
}