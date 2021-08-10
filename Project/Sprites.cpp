#include "Sprites.h"
#include "Game.h"
#include "EventManager.h"

#include <iostream>

////////////////////////////////////////////////////////
// Begin Sprite Class
////////////////////////////////////////////////////////
Sprite::Sprite()
	: m_sourceTransform{ 0, 0, 0, 0 }
	, m_destinationTransform{ 0, 0, 0, 0 }
	, m_angle(0)
{

}

Sprite::Sprite(SDL_Rect source, SDL_FRect destination, int angle) 
	: m_sourceTransform(source)
	, m_destinationTransform(destination)
	, m_angle(angle)
{

}


////////////////////////////////////////////////////////
// Begin AnimatedSprite Class
////////////////////////////////////////////////////////
AnimatedSprite::AnimatedSprite(int angle, float frameTime, int maxSprites, 
	SDL_Rect sourceTransform, SDL_FRect destinationTransform)
	: Sprite(sourceTransform, destinationTransform, angle)
	, m_currentSpriteIndex(0)
	, m_maxSprites(maxSprites)
	, m_currentTime(0)
	, m_frameRate(frameTime)

{

}

void AnimatedSprite::Animate(float deltaTime)
{
	m_currentTime += deltaTime;
	if (m_currentTime > m_frameRate)
	{
		m_currentTime = m_frameRate - m_currentTime;
		m_currentSpriteIndex++;
		if (m_currentSpriteIndex == m_maxSprites)
		{
			m_currentSpriteIndex = 0;
		}
	}
	m_sourceTransform.x = m_sourceTransform.w * m_currentSpriteIndex;
}

////////////////////////////////////////////////////////
// Begin Player Class
////////////////////////////////////////////////////////


const float Player::s_kAccelerationX = 650;
const float Player::s_kGravity = 20;
const float Player::s_kJumpForce = 5000;
const float Player::s_kMaxVelocityX = 1000;
const float Player::s_kMaxVelocityY = 10000;
const float Player::s_kDragX = 0.8f;

Player::Player(SDL_Rect sourceTransform, SDL_FRect destinationTransform, Mix_Chunk* pDieSound, Mix_Chunk* pJumpSound, SDL_Rect jumpSourceTransform, SDL_Rect moveSourceTransform, SDL_Rect duckSourceTransform)
	: AnimatedSprite(90, 0.2f, 4, sourceTransform, destinationTransform)
	, m_grounded(false)
	, m_facingLeft(false)
	, m_accelX(0)
	, m_accelY(0)
	, m_velX(0)
	, m_velY(0)
	, m_state(PlayerState::kIdle)
	, jumpSourceTransform(jumpSourceTransform)
	, moveSourceTransform(moveSourceTransform)
	, duckSourceTransform(duckSourceTransform)
	

{
	m_pJumpSound = pJumpSound;
	m_pDieSound = pDieSound;
}


void Player::Update(float deltaTime)
{

	// Checking states.
	switch (m_state)
	{
	case PlayerState::kIdle:
		// Transition to run.
		if (Game::GetInstance()->KeyDown(SDL_SCANCODE_A) || Game::GetInstance()->KeyDown(SDL_SCANCODE_D))
		{
			m_state = PlayerState::kRunning;
		}
		// Transition to jump.
		else if (Game::GetInstance()->KeyDown(SDL_SCANCODE_SPACE) && m_grounded)
		{
			Jump();
		}
		else if (Game::GetInstance()->KeyDown(SDL_SCANCODE_S))
		{
			Duck(true);
		}
		break;
	case PlayerState::kRunning:
		// Move left and right.

		if (Game::GetInstance()->KeyDown(SDL_SCANCODE_S))
		{
			Duck(true);
		}
		if (Game::GetInstance()->KeyDown(SDL_SCANCODE_A) && m_destinationTransform.x > 0)
		{
			m_accelX = -s_kAccelerationX;
		}
		else if (Game::GetInstance()->KeyDown(SDL_SCANCODE_D) && m_destinationTransform.x < Game::kWidth - m_destinationTransform.w)
		{
			m_accelX = s_kAccelerationX;
		}
		// Transition to jump.
		if (Game::GetInstance()->KeyDown(SDL_SCANCODE_SPACE) && m_grounded)
		{
			Jump();
		}


		// Transition to idle.
		if (!Game::GetInstance()->KeyDown(SDL_SCANCODE_A) && !Game::GetInstance()->KeyDown(SDL_SCANCODE_D))
		{
			m_state = PlayerState::kIdle;
		}
		break;
	case PlayerState::kJumping:
		// Move in mid-air.
		if (Game::GetInstance()->KeyDown(SDL_SCANCODE_A) && m_destinationTransform.x > 0)
		{
			m_accelX = -s_kAccelerationX;
			if (!m_facingLeft)
				m_facingLeft = true;
		}
		else if (Game::GetInstance()->KeyDown(SDL_SCANCODE_D) && m_destinationTransform.x < Game::kWidth - m_destinationTransform.w)
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
	case PlayerState::kDucking:

		if (!Game::GetInstance()->KeyDown(SDL_SCANCODE_S))
		{
			Duck(false);
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

void Player::Jump()
{
	m_accelY = -s_kJumpForce;
	m_grounded = false;
	m_state = PlayerState::kJumping;
	m_sourceTransform = jumpSourceTransform;
	m_maxSprites = 1;
	m_currentSpriteIndex = 0;
	Mix_PlayChannel(-1, m_pJumpSound, 0);
}

void Player::Duck(bool duckin)
{
	if (duckin)
	{
		m_state = PlayerState::kDucking;
		m_sourceTransform = duckSourceTransform;
		m_maxSprites = 1;
		m_currentSpriteIndex = 0;
		m_destinationTransform.y -= 40;
		m_ducking = true;
	}
	else
	{
		m_state = PlayerState::kRunning;
		m_sourceTransform = moveSourceTransform;
		m_maxSprites = 4;
		m_currentSpriteIndex = 0;
		m_destinationTransform.y += 40;
		m_ducking = false;
	}
}

void Player::SetGrounded(bool g) 
{ 
	if (g && !m_grounded && m_state != PlayerState::kDying)
	{
		m_sourceTransform = moveSourceTransform;
		m_maxSprites = 4;
		m_currentSpriteIndex = 0;
	}

	m_grounded = g; 
}
void Player::Stop()
{
	StopX();
	StopY();
}

void Player::Die()
{
	m_state = PlayerState::kDying;
	m_currentSpriteIndex = 0;
	m_maxSprites = 4;
	m_sourceTransform = { 0, 150, 98, 100 };
}

////////////////////////////////////////////////////////
// Begin Bullet Class
////////////////////////////////////////////////////////
Bullet::Bullet(SDL_Rect sourceTransform, SDL_FRect destinationTransform, float speed)
	: Sprite(sourceTransform, destinationTransform)
	, m_speed(speed)
	, m_isActive(true)
{

}

void Bullet::Update(float deltaTime)
{
	m_destinationTransform.x += m_speed * deltaTime;
}


////////////////////////////////////////////////////////
// Begin Enemy Class
////////////////////////////////////////////////////////
Enemy::Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform, std::vector<Bullet*>* pBullets, Mix_Chunk* pShootSound, float bulletSpawnRate, bool pshoots, int frames)
	: AnimatedSprite(-90, 0.1f, frames, sourceTransform, destinationTransform)
	, m_bulletTimer(0)
	, m_timerMax(bulletSpawnRate)
	, m_pBullets(pBullets)
	, m_pShootSound(pShootSound)
	, shoots(pshoots)
{

}

void Enemy::Update(float deltaTime)
{
	Animate(deltaTime);
	m_destinationTransform.x -= kEnemySpeed * deltaTime;

	if (shoots)
	{


	m_bulletTimer += deltaTime;
	if (m_bulletTimer > m_timerMax)
	{
		m_bulletTimer = 0;
		m_pBullets->push_back(
			new Bullet({ kBulletSourceXPosition, kBulletSourceYPosition, kBulletWidth, kBulletHeight },
						{ m_destinationTransform.x, m_destinationTransform.y - kBulletHeight * 2, kBulletWidth, kBulletHeight },
						kBulletSpeed));
		Mix_PlayChannel(-1, m_pShootSound, 0);
	}
	}
}