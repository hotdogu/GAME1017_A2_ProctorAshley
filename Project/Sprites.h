#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>

////////////////////////////////////////////////////////
// Begin Sprite Class
////////////////////////////////////////////////////////
class Sprite
{
protected:
	SDL_Rect m_sourceTransform; // Rectangle for source pixels from image.
	SDL_FRect m_destinationTransform; // Rectangle for destination window.
	int m_angle;		// Angle of sprite.
public:
	Sprite();
	Sprite(SDL_Rect source, SDL_FRect destination, int angle = 0);
	SDL_Rect* GetSourceTransform() { return &m_sourceTransform; }
	SDL_FRect* GetDestinationTransform() { return &m_destinationTransform; }
	double GetAngle() { return m_angle; }
};

////////////////////////////////////////////////////////
// Begin AnimatedSprite Class
////////////////////////////////////////////////////////
class AnimatedSprite : public Sprite
{
protected:
	int m_currentSpriteIndex;	// Which sprite to display for animation.
	int m_maxSprites;			// How many sprites total.
	float m_currentTime;		// Time counter.
	float m_frameRate;			// When to change to next sprite.

public:
	AnimatedSprite(int angle, float frameTime, int maxSprites, 
		SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	void Animate(float deltaTime);
};

////////////////////////////////////////////////////////
// Begin Player Class
////////////////////////////////////////////////////////
class Player : public AnimatedSprite
{
	enum class PlayerState { kIdle, kRunning, kJumping, kDucking, kDying };

	bool m_grounded;
	bool m_facingLeft;
	float m_accelX;
	float m_accelY;
	float m_velX;
	float m_velY;
	PlayerState m_state;
	SDL_Rect jumpSourceTransform;
	SDL_Rect moveSourceTransform;
	SDL_Rect duckSourceTransform;

	static const float s_kAccelerationX;
	static const float s_kGravity;
	static const float s_kJumpForce;
	static const float s_kMaxVelocityX;
	static const float s_kMaxVelocityY;
	static const float s_kDragX;

public:

	Player(SDL_Rect sourceTransform, SDL_FRect destinationTransform, Mix_Chunk* pDieSound, Mix_Chunk* pJumpSound, SDL_Rect jumpSourceTransform, SDL_Rect moveSourceTransform, SDL_Rect duckSourceTransform);
	void Update(float deltaTime);
	Mix_Chunk* m_pDieSound;
	Mix_Chunk* m_pJumpSound;
	bool m_ducking = false;
	void Render();
	void Jump();
	void Stop();
	void Die();
	void Duck(bool duckin);

	void StopX() { m_velX = 0.0f; }
	void StopY() { m_velY = 0.0f; }
	void SetAccelX(float a) { m_accelX = a; }
	void SetAccelY(float a) { m_accelY = a; }
	bool IsGrounded() { return m_grounded; }
	void SetGrounded(bool g);
	float GetVelX() { return m_velX; }
	float GetVelY() { return m_velY; }
	void SetX(float x) { m_destinationTransform.x = x; }
	void SetY(float y) { m_destinationTransform.y = y; }
};

////////////////////////////////////////////////////////
// Begin Bullet Class
////////////////////////////////////////////////////////
class Bullet : public Sprite
{
private:
	float m_speed;
	bool m_isActive;
public:
	Bullet(SDL_Rect sourceTransform, SDL_FRect destinationTransform, float speed);
	void Update(float deltaTime);
};

////////////////////////////////////////////////////////
// Begin Enemy Class
////////////////////////////////////////////////////////
class Enemy : public AnimatedSprite
{
	static const int kEnemySpeed = 150;
	static const int kBulletSpeed = -300;

	static const int kBulletSourceXPosition = 160;
	static const int kBulletSourceYPosition = 100;
	static const int kBulletWidth = 14;
	static const int kBulletHeight = 14;

private:
	float m_bulletTimer;
	float m_timerMax;
	bool shoots;
	std::vector<Bullet*>* m_pBullets;
	Mix_Chunk* m_pShootSound;
public:
	Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform, std::vector<Bullet*>* pBullets, Mix_Chunk* pShootSound, float bulletSpawnRate, bool pshoots, int frames);
	void Update(float deltaTime);
};

