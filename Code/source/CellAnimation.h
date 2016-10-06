//***********************************************************************
//	File:		CellAnimation.h
//	Author:		Allan Murillo
//	Course:		SGD 1409
//	Purpose:	CellAnimation class renders a fixed-size animation
//				based on the elapsed time
//***********************************************************************

#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"


//***********************************************************************
// CellAnimation class
//	- runs animation using an image of fixed-size frames
//	- image MUST have a size power-of-2 (e.g. 64, 128, 256, 512)
class CellAnimation
{
public:
	//*******************************************************************
	// Default Constructor & Destructor
	CellAnimation(void) = default;
	~CellAnimation(void) = default;


	//*******************************************************************
	// Initialize & Terminate
	void	Initialize(int type);	// should load from a file
	void	Terminate(void);


	//*******************************************************************
	// Controls:
	void	Update(float elapsedTime);
	void	Render(SGD::Point position, float scale, SGD::Color color);

	void	Restart(float speed, bool looping);
	void	Pause(bool pause);

	//*******************************************************************
	// Accessors:
	bool	IsPlaying(void) const	{ return m_bPlaying; }
	bool	IsFinished(void) const	{ return m_bFinished; }

private:
	//*******************************************************************
	// image
	SGD::HTexture	m_hImage = SGD::INVALID_HANDLE;

	// cell algorithm data
	int				m_nNumCols = 0;
	int				m_nFrameWidth = 0;
	int				m_nFrameHeight = 0;

	// animation data
	int				m_nCurrentFrame = 0;
	int				m_nNumFrames = 0;

	float			m_fTimeWaited = 0.0f;
	float			m_fTimePerFrame = 0.0f;
	float			m_fSpeed = 1.0f;	// 1 = normal, 2 = twice as fast

	bool			m_bPlaying = false;
	bool			m_bLooping = false;
	bool			m_bFinished = false;
};

