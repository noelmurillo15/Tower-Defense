/***************************************************************
|	File:		MainMenuState.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	MainMenuState class handles the main menu
***************************************************************/

#include "Game.h"
#include "OptionsState.h"
#include "MainMenuState.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

#include <iostream>
#include <fstream>

using namespace std;


OptionsState* OptionsState::GetInstance(void) {

	static OptionsState s_Instance;
	return &s_Instance;
}

void OptionsState::Enter(void) {

		// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 255 });

		//	Load audio
	m_sfxBoomShot = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/ANM_FireBlastGoBoom.wav");
	m_background = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/ANM_Background.xwm");

		//	Load Rects
	m_optionRect = { 200, 100, 1024 - 200, 768 - 100 };
	m_MastervolRect = { 400.0f, 200.0f, 600.0f, 220.0f };
	m_SFXvolRect = { 400.0f, 250.0f, 600.0f, 270.0f };
}

void OptionsState::Exit(void) {

	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();

		//	if any sounds are playing.. stop them
	if (audio->IsAudioPlaying(m_sfxBoomShot)){
		audio->StopAudio(m_sfxBoomShot);
	}
		//	unload sound
	audio->UnloadAudio(m_sfxBoomShot);
	audio->UnloadAudio(m_background);
}

bool OptionsState::Update(float elapsedTime) {

	SGD::GraphicsManager* graphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* audio = SGD::AudioManager::GetInstance();
	SGD::InputManager* input = SGD::InputManager::GetInstance();
	Game* self = Game::GetInstance();

	SGD::Point cursor = SGD::InputManager::GetInstance()->GetCursorPosition();
	SGD::Rectangle cursorrect = { cursor.x, cursor.y, cursor.x + 1, cursor.y + 1 };

	if (input->IsKeyPressed(SGD::Key::UpArrow) == true){
		if (m_nCursor == 0)
			m_nCursor = 1;
		else
			m_nCursor = 0;
	}
	else if (input->IsKeyPressed(SGD::Key::DownArrow) == true){
		if (m_nCursor == 1)
			m_nCursor = 0;
		else
			m_nCursor = 1;
	}

		//	arrow key input for sound
	if (input->IsKeyPressed(SGD::Key::LeftArrow) == true) {
		if (m_nCursor == 0){
			audio->SetMasterVolume(SGD::AudioGroup::Music,
				audio->GetMasterVolume(SGD::AudioGroup::Music) - 5);
		}
		else{
			audio->SetMasterVolume(SGD::AudioGroup::SoundEffects,
				audio->GetMasterVolume(SGD::AudioGroup::SoundEffects) - 5);

			if (m_sfxBoomShot != SGD::INVALID_HANDLE)
				audio->PlayAudio(m_sfxBoomShot);
		}

		ofstream fout;
		fout.open("sound.txt", ios_base::trunc);
		if (fout.is_open() && fout.good()){
			fout << audio->GetMasterVolume(SGD::AudioGroup::Music) 
				<< '\t' << audio->GetMasterVolume(SGD::AudioGroup::SoundEffects);
			fout.close();
		}

	}
	else if (input->IsKeyPressed(SGD::Key::RightArrow) == true) {
		if (m_nCursor == 0){
			audio->SetMasterVolume(SGD::AudioGroup::Music,
				audio->GetMasterVolume(SGD::AudioGroup::Music) + 5);
		}
		else{
			audio->SetMasterVolume(SGD::AudioGroup::SoundEffects,
				audio->GetMasterVolume(SGD::AudioGroup::SoundEffects) + 5);

			if (m_sfxBoomShot != SGD::INVALID_HANDLE)
				audio->PlayAudio(m_sfxBoomShot);
		}

		ofstream fout;
		fout.open("sound.txt", ios_base::trunc);
		if (fout.is_open() && fout.good()){
			fout << audio->GetMasterVolume(SGD::AudioGroup::Music)
				<< '\t' << audio->GetMasterVolume(SGD::AudioGroup::SoundEffects);
			fout.close();
		}
	}

		//	Cursor input for sounds
	if (cursorrect.IsIntersecting(m_MastervolRect) && input->IsKeyPressed(SGD::Key::MouseLeft) == true){
		audio->SetMasterVolume(SGD::AudioGroup::Music, (((int)cursor.x - 400) / 2));
		if (m_background != SGD::INVALID_HANDLE)
			if (audio->IsAudioPlaying(m_background) == false)
				audio->PlayAudio(m_background, false);

			ofstream fout;
			fout.open("sound.txt", ios_base::trunc);
			if (fout.is_open() && fout.good()){
				fout << audio->GetMasterVolume(SGD::AudioGroup::Music)
					<< '\t' << audio->GetMasterVolume(SGD::AudioGroup::SoundEffects);
				fout.close();
			}
	}
	else if (cursorrect.IsIntersecting(m_SFXvolRect) && input->IsKeyPressed(SGD::Key::MouseLeft) == true){
		audio->SetMasterVolume(SGD::AudioGroup::SoundEffects, (((int)cursor.x - 400) / 2));

		if (m_sfxBoomShot != SGD::INVALID_HANDLE)
			audio->PlayAudio(m_sfxBoomShot);

		ofstream fout;
		fout.open("sound.txt", ios_base::trunc);
		if (fout.is_open() && fout.good()){
			fout << audio->GetMasterVolume(SGD::AudioGroup::Music)
				<< '\t' << audio->GetMasterVolume(SGD::AudioGroup::SoundEffects);
			fout.close();
		}
	}

		// Press Escape to return to Main Menu
	if (input->IsKeyPressed(SGD::Key::Escape) == true) {
		self->ChangeState(MainMenuState::GetInstance());
		return true;
	}
	return true;		//	keep looping
}

void OptionsState::Render(float elapsedTime) {

		//	Draw options rect
	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_optionRect,
		SGD::Color(80, 80, 80),
		SGD::Color(0,0,0));

		//	Set rects for volume levels
	SGD::Rectangle m_MastervolLevelRect = {400.0f, 200.0f,
		400.0f + (SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) * 2.0f) + 1.0f,
		220.0f
	};
	SGD::Rectangle m_SFXvolLevelRect = {400.0f, 250.0f,
		400.0f + (SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) * 2.0f) + 1.0f,
		270.0f
	};

		// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

		//	Ostring display volume levels
	SGD::OStringStream musiclevel;
	SGD::OStringStream sfxlevel;
	musiclevel << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
	sfxlevel << SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);

	pFont->Draw("Options", { 200, 50 },
		1.5f, { 0, 0, 0 });

	if (m_nCursor == 0){
		pFont->Draw("- Music Volume : ", { 370, 175 }, 0.5f, SGD::Color(0, 0, 255));
		pFont->Draw(musiclevel.str().c_str(), { 630, 175 }, 0.5f, SGD::Color(0, 0, 255));
	}
	else{
		pFont->Draw("Music Volume : ", { 400, 175 }, 0.5f, SGD::Color(0, 0, 0));
		pFont->Draw(musiclevel.str().c_str(), { 630, 175 }, 0.5f, SGD::Color(0, 0, 0));
	}
	if (m_nCursor == 1){
		pFont->Draw("- Sfx Volume : ", { 370, 225 }, 0.5f, SGD::Color(0, 0, 255));
		pFont->Draw(sfxlevel.str().c_str(), { 600, 225 }, 0.5f, SGD::Color(0, 0, 255));
	}
	else{
		pFont->Draw("Sfx Volume : ", { 400, 225 }, 0.5f, SGD::Color(0, 0, 0));
		pFont->Draw(sfxlevel.str().c_str(), { 600, 225 }, 0.5f, SGD::Color(0, 0, 0));
	}
	
		//	Draw Music Rect
	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_MastervolRect, SGD::Color(255, 0, 0, 0),
		SGD::Color(0, 0, 0), 1);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_MastervolLevelRect, SGD::Color(100, 0, 0, 255));

		//	Draw Sfx Rect
	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_SFXvolRect, SGD::Color(255, 0, 0, 0),
		SGD::Color(0, 0, 0), 1);

	SGD::GraphicsManager::GetInstance()->DrawRectangle(m_SFXvolLevelRect, SGD::Color(100, 0, 0, 255));
}