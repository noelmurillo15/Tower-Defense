	//***********************************************************************
	//	File:		CellAnimation.cpp
	//	Author:		Allan Murillo
	//	Course:		SGD 1409
	//	Purpose:	CellAnimation class renders a fixed-size animation
	//				based on the elapsed time
	//***********************************************************************

#include "CellAnimation.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>


	//***********************************************************************
	// Initialize
	//	- set up the animation
	//	- should load from a file
	//	- hardcoded to use SGD_Anim_Explosion.png
	void CellAnimation::Initialize(int type)
	{
		// Load the image
		if (type == 9)
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/ANM_Tower_Plasma_Anim.png");
		else if (type == 10)
			m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/ANM_Fireball_Animation.png");
		else if (type == 11)
			m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/ANM_AquaPrison.png");
		else if (type == 12)
			m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/ANM_IceBlast.png");

		if (type == 11){
			m_nNumCols = 6;
			m_nFrameWidth = 85;
			m_nFrameHeight = 128;

			m_nCurrentFrame = 0;		// index: 0 -> 5
			m_nNumFrames = 6;

			m_fTimeWaited = 0.0f;
			m_fTimePerFrame = 0.16f;		// 1/10th of a second
			m_fSpeed = 1.0f;
		}
		else if (type == 12){
			m_nNumCols = 5;
			m_nFrameWidth = 102;
			m_nFrameHeight = 64;

			m_nCurrentFrame = 0;		// index: 0 -> 2
			m_nNumFrames = 10;

			m_fTimeWaited = 0.0f;
			m_fTimePerFrame = 0.1f;		// 2/10th of a second
			m_fSpeed = 1.0f;
		}
		else{
			m_nNumCols = 3;
			m_nFrameWidth = 170;
			m_nFrameHeight = 128;

			m_nCurrentFrame = 0;		// index: 0 -> 2
			m_nNumFrames = 3;

			m_fTimeWaited = 0.0f;
			m_fTimePerFrame = 0.2f;		// 2/10th of a second
			m_fSpeed = 1.0f;
		}

		m_bPlaying = false;
		m_bLooping = false;
		m_bFinished = false;
	}

	//***********************************************************************
	// Terminate
	//	- unload resources
	void CellAnimation::Terminate(void)
	{
		// Unload the image
		SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	}


	//***********************************************************************
	// Update
	//	- run through the animation frame-by-frame based on elapsed time
	void CellAnimation::Update(float elapsedTime)
	{
		// Is the animation paused?
		if (m_bPlaying == false)
			return;


		// Increase the timer
		m_fTimeWaited += elapsedTime * m_fSpeed;

		// Is time for the next frame?
		if (m_fTimeWaited >= m_fTimePerFrame)
		{
			// Move to the next frame
			m_nCurrentFrame++;
			m_fTimeWaited = 0.0f;


			// Have we reached the end?
			if (m_nCurrentFrame == m_nNumFrames)
			{
				// Loop to the beginning?
				if (m_bLooping == true)
					m_nCurrentFrame = 0;
				else
				{
					// Stop on the last valid frame
					m_nCurrentFrame--;
					m_bPlaying = false;
					m_bFinished = true;
				}
			}
		}
	}

	//***********************************************************************
	// Render
	//	- draw the current frame to the specified position
	void CellAnimation::Render(SGD::Point position, float scale, SGD::Color color)
	{
		// Validate the image
		assert(m_hImage != SGD::INVALID_HANDLE
			&& "CellAnimation::Render - image was not initialized");

		// Check the parameters
		if (scale == 0.0f
			|| color.alpha == 0)
			return;


		// Calculate the source rect using the Cell Algorithm
		SGD::Rectangle frame;

		frame.left = (float)((m_nCurrentFrame % m_nNumCols) * m_nFrameWidth);
		frame.top = (float)((m_nCurrentFrame / m_nNumCols) * m_nFrameHeight);

		frame.right = frame.left + m_nFrameWidth;
		frame.bottom = frame.top + m_nFrameHeight;

		// Draw the frame
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(
			m_hImage,
			position,
			frame,
			0.0f, {},
			color,
			{ scale, scale });

	}


	//***********************************************************************
	// Restart
	//	- start the animation at frame 0
	void CellAnimation::Restart(float speed, bool looping)
	{
		// Store the parameters
		m_fSpeed = speed;
		m_bLooping = looping;

		// Start at frame 0
		m_nCurrentFrame = 0;
		m_fTimeWaited = 0.0f;
		m_bPlaying = true;
		m_bFinished = false;
	}


	//***********************************************************************
	// Pause
	//	- pause / resume the animation
	void CellAnimation::Pause(bool pause)
	{
		m_bPlaying = !pause;
		//if( pause == true )
		//	m_bPlaying = false;
		//else 
		//	m_bPlaying = true;
	}
