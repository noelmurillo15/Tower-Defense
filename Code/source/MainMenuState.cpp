/***************************************************************
|	File:		MainMenuState.cpp
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	MainMenuState class handles the main menu
***************************************************************/

#include "MainMenuState.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "HowtoplayState.h"
#include "OptionsState.h"
#include "CreditsState.h"
#include "LevelselectionState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"

#include <iostream>
#include <fstream>
using namespace std;


MainMenuState* MainMenuState::GetInstance( void ) {

	static MainMenuState s_Instance;
	return &s_Instance;
}


void MainMenuState::Enter(void) {

	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 50, 50, 50 });	// dark gray

		//	Load Master Volumes from sound file
	ifstream fin;
	fin.open("sound.txt");

	if (fin.is_open()) {
	
		int musicvol, sfxvol;

		fin >> musicvol;
		fin.ignore(INT_MAX, '\t');
		fin >> sfxvol;

		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, musicvol);
		SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, sfxvol);

		fin.close();
	}
}

void MainMenuState::Exit( void ) {

}

bool MainMenuState::Update( float elapsedTime ) {

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	

		// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
		m_nCursor = 4;



		// Move cursor
	if( pInput->IsKeyPressed( SGD::Key::Down ) == true ) {

		++m_nCursor;

		if( m_nCursor > 4 )		// should enumerate the options
			m_nCursor = 0;
	}
	else if( pInput->IsKeyPressed( SGD::Key::Up ) == true ) {

		--m_nCursor;

		if( m_nCursor < 0 )
			m_nCursor = 4;
	}

	if (pInput->IsKeyPressed(SGD::Key::Enter) == true && pInput->IsKeyDown(SGD::Key::Alt) == false)
	{
		// Which option?
		if( m_nCursor == 0 ) {		// PLAY
		
			Game::GetInstance()->ChangeState( LevelselectionState::GetInstance() );
			return true;	// keep playing in the new state
		}
		else if (m_nCursor == 1) {	// How To Play
		
			Game::GetInstance()->ChangeState(HowtoplayState::GetInstance());
			return true;	
		}
		else if (m_nCursor == 2) {	// Options
		
			Game::GetInstance()->ChangeState(OptionsState::GetInstance());
			return true;
		}
		else if (m_nCursor == 3) {	// Credits
		
			Game::GetInstance()->ChangeState(CreditsState::GetInstance());
			return true;
		}
		else if( m_nCursor == 4) {	// EXIT
			return false;	// quit the game
		}
	}

	return true;	// keep looping
}

void MainMenuState::Render( float elapsedTime ) {

		// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

		// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();


		// Display the game title centered at 2x scale
	pFont->Draw( "Tower Defense", { (width - ( 1.5f * 4 * 32 * 4.0f))/2, 50 }, 
				 2.0f, { 255, 0, 0 } );

		// Display the menu options centered at 1x scale
	pFont->Draw( "Play", { (width - (4 * 32))/2, 300 }, 
				 1.0f, {255, 0, 0} );
	pFont->Draw("How To Play", { (width - (4 * 32)) / 2, 350 },
		1.0f, { 255, 0, 0 });
	pFont->Draw("Options", { (width - (4 * 32)) / 2, 400 },
		1.0f, { 255, 0, 0 });
	pFont->Draw("Credits", { (width - (4 * 32)) / 2, 450 },
		1.0f, { 255, 0, 0 });
	pFont->Draw( "Exit", { (width - (4 * 32))/2, 500 }, 
				 1.0f, {255, 0, 0} );

	if (m_nCursor == 0)
		pFont->Draw("=     =", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 255, 0 });

	else if (m_nCursor == 1)
		pFont->Draw("=            =", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 255, 0 });
	
	else if (m_nCursor == 2)
		pFont->Draw("=        =", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 255, 0 });
	
	else if (m_nCursor == 3)
		pFont->Draw("=        =", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 255, 0 });
	
	else if (m_nCursor == 4)
		pFont->Draw("=     =", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			1.0f, { 0, 255, 0 });
}
