#include "Player.h"
#include "Game.h"
#include "EventManager.h"
#include <cmath>

const float PlatformPlayer::s_kAccelerationX = 250;
const float PlatformPlayer::s_kGravity = 50;
const float PlatformPlayer::s_kJumpForce = 2000;
const float PlatformPlayer::s_kMaxVelocityX = 350;
const float PlatformPlayer::s_kMaxVelocityY = 2000;
const float PlatformPlayer::s_kDragX = 0.8f;

PlatformPlayer::PlatformPlayer(SDL_Rect sourceTransform, SDL_FRect destinationTransform)
	: m_grounded(false)
	, m_facingLeft(false)
	, m_accelX(0)
	, m_accelY(0)
	, m_velX(0)
	, m_velY(0)
	, m_state(PlayerState::kIdle)

{

}

PlatformPlayer::~PlatformPlayer()
{

}

void PlatformPlayer::Update(float deltaTime)
{
	// Checking states.
	switch (m_state)
	{
	case PlayerState::kIdle:
		// Transition to run.
		if (EventManager::KeyPressed(SDL_SCANCODE_A) || EventManager::KeyPressed(SDL_SCANCODE_D))
		{
			m_state = PlayerState::kRunning;
		}
		// Transition to jump.
		else if (EventManager::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			Jump();
		}
		break;
	case PlayerState::kRunning:
		// Move left and right.
		if (EventManager::KeyHeld(SDL_SCANCODE_A) && m_destinationTransform.x > 0)
		{
			m_accelX = -s_kAccelerationX;
		}
		else if (EventManager::KeyHeld(SDL_SCANCODE_D) && m_destinationTransform.x < Game::kWidth - m_destinationTransform.w)
		{
			m_accelX = s_kAccelerationX;
		}
		// Transition to jump.
		if (EventManager::KeyPressed(SDL_SCANCODE_SPACE) && m_grounded)
		{
			Jump();
		}
		// Transition to idle.
		if (!EventManager::KeyHeld(SDL_SCANCODE_A) && !EventManager::KeyHeld(SDL_SCANCODE_D))
		{
			m_state = PlayerState::kIdle;
		}
		break;
	case PlayerState::kJumping:
		// Move in mid-air.
		if (EventManager::KeyHeld(SDL_SCANCODE_A) && m_destinationTransform.x > 0)
		{
			m_accelX = -s_kAccelerationX;
			if (!m_facingLeft)
				m_facingLeft = true;
		}
		else if (EventManager::KeyHeld(SDL_SCANCODE_D) && m_destinationTransform.x < Game::kWidth - m_destinationTransform.w)
		{
			m_accelX = s_kAccelerationX;
			if (m_facingLeft)
				m_facingLeft = false;
		}
		// Hit the ground, transition to run.
		if (m_grounded)
		{
			m_state = PlayerState::kRunning;
		}
		break;
	}
	// Player movement. X axis first.
	m_velX += m_accelX;
	m_velX *= (m_grounded ? s_kDragX : 1.0f);
	m_velX = std::min(std::max(m_velX, -s_kMaxVelocityX), s_kMaxVelocityX);
	m_destinationTransform.x += m_velX * deltaTime;

	m_velY += m_accelY + s_kGravity;
	m_velY = std::min(std::max(m_velY, -s_kMaxVelocityY), s_kMaxVelocityY);
	m_destinationTransform.y += m_velY * deltaTime;

	// Resetting accel every frame.
	m_accelX = 0.0f;
	m_accelY = 0.0f;

}

void PlatformPlayer::Render()
{
}


void PlatformPlayer::Jump()
{
	m_accelY = -s_kJumpForce;
	m_grounded = false;
	m_state = PlayerState::kJumping;
}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}