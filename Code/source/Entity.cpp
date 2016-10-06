/***************************************************************
|	File:		Entity.cpp
|	Author:		Allan Murillo
|	Course:		SGD 1409
|	Purpose:	Entity class stores the shared data members
|				for all child game entities
***************************************************************/

#include "Entity.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"

#include <cassert>


Entity::~Entity( void ) {

	//assert( m_unRefCount == 0 && "~Entity - entity was destroyed while references remain!" );
}

void Entity::Update( float elapsedTime ) {

	m_ptPosition += m_vtVelocity * elapsedTime;
}

void Entity::Render( void ) {

		// Verify the image
	assert( m_hImage != SGD::INVALID_HANDLE && "Entity::Render - image was not set!" );
	
		// Draw the image
	SGD::GraphicsManager::GetInstance()->DrawTexture( 
		m_hImage, m_ptPosition,
		m_fRotation, m_szSize/2 );
}

SGD::Rectangle Entity::GetRect( void ) const {

	return SGD::Rectangle{
		this->GetPosition().x,
		this->GetPosition().y,
		this->GetPosition().x + this->GetSize().width,
		this->GetPosition().y + this->GetSize().height
	};
}

void Entity::HandleCollision( const IEntity* pOther ) {

		/* DO NOTHING */
	(void)pOther;		// unused parameter
}

void Entity::AddRef( void ) {

	assert( m_unRefCount != 0xFFFFFFFF && "Entity::AddRef - maximum reference count has been exceeded" );
	++m_unRefCount;
}

void Entity::Release( void ) {

	--m_unRefCount;

	if( m_unRefCount == 0 )
		delete this;
}
