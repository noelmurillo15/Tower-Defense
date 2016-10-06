/***************************************************************
|	File:		Entity.h
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Entity class stores the shared data members
|				for all child game entities
***************************************************************/

#ifndef ENTITY_H
#define ENTITY_H


#include "IEntity.h"						// IEntity type
#include "../SGD Wrappers/SGD_Handle.h"		// HTexture type


//	- parent class of all game entities, stores the shared data members
//	- velocity-based movement
class Entity : public IEntity {

	// MUST have a virtual destructor to allow upcasted pointers
	// to trigger children destructors at deallocation
public:		Entity( void )			= default;	// default constructor
protected:	virtual ~Entity( void );			// VIRTUAL destructor

public:

		// Entity Types:
	enum EntityType { ENT_BASE, E_GHAST, E_UNDEAD, E_GOBLIN, E_SPIRE,
			P_AQUAGODDESS, P_FIREMAGE, P_GUARDIAN, P_ZEPHYR,
			ENT_PLASMA, ENT_FIREBALL, ENT_AQUABOUND, ENT_WINDSTYLEBLAST
	};

	//	Ghast - Shoots Plasma
	//	Undead - Punches
	//	Goblin - Punches
	//	Spire - Shoots fire

	//	Water Mage - Traps enemies in aqua prison
	//	FireMage - Shoots fire, extra dmg over time
	//	UndeadGuardian - Sycthe attack
	//	FrostMage - Shoots ice, slows for some amount of time
	

		//	- virtual functions for children classes to override
	virtual void	Update			( float elapsedTime )		override;
	virtual void	Render			( void )					override;
	virtual void	HandleCollision	( const IEntity* pOther )	override;

	virtual int		GetType(void)	const			override	{ return ENT_BASE; }

	virtual SGD::Rectangle GetRect(void)	const			override;
	
		// Children classes CANNOT override a 'final' method.
	virtual void	AddRef			( void )					final;
	virtual void	Release			( void )					final;

	
		// Accessors:
	SGD::HTexture		GetImage	( void ) const			{	return m_hImage;		}
	SGD::Point			GetPosition	( void ) const			{	return m_ptPosition;	}
	SGD::Vector			GetVelocity	( void ) const			{	return m_vtVelocity;	}
	SGD::Size			GetSize		( void ) const			{	return m_szSize;		}
	float				GetRotation	( void ) const			{	return m_fRotation;		}
		// Mutators:
	void SetImage		( SGD::HTexture	img  )	{	m_hImage		= img;	}
	void SetPosition	( SGD::Point	pos  ) 	{	m_ptPosition	= pos;	}
	void SetVelocity	( SGD::Vector	vel	 ) 	{	m_vtVelocity	= vel;	}
	void SetSize		( SGD::Size		size ) 	{	m_szSize		= size;	}
	void SetRotation	( float			rad  )	{	m_fRotation		= rad;	}

protected:

		// members:
	SGD::HTexture		m_hImage		= SGD::INVALID_HANDLE;	// image handle
	SGD::Point			m_ptPosition	= SGD::Point{0, 0};		// 2D position
	SGD::Vector			m_vtVelocity	= SGD::Vector{0, 0};	// 2D velocity
	SGD::Size			m_szSize		= SGD::Size{0, 0};		// 2D size
	float				m_fRotation		= 0.0f;					// angle in radians

private:
		// reference count
	unsigned int		m_unRefCount	= 1;	// calling 'new' gives the prime reference
};

#endif //ENTITY_H
