#pragma once
#include <SDL.h>
#include "Sprites.h"
#include "PlayButton.h"

class State
{
public:
	virtual void Enter() = 0;
	virtual void Update(float deltaTime);
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void Pause() { }
	virtual void Resume() { }
};

class TitleState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;

	SDL_Texture* m_pBGText; // For the bg.
	SDL_Texture* m_pPlayText; // For the play button
	PlayButton* playButton;
};

class GameState : public State
{
public:
	static const int kWidth = 1880; //Take these out
	static const int kHeight = 950;
	static const int kPlayerSpeed = 400;

private: 
	static const int kPlayerSourceXPosition = 0;
	static const int kPlayerSourceYPosition = 0;
	static const int kPlayerWidth = 94;
	static const int kPlayerHeight = 100;

	static const int kEnemySourceXPosition = 0;
	static const int kEnemySourceYPosition = 100;
	static const int kEnemyWidth = 40;
	static const int kEnemyHeight = 46;
	// BG Stuff
	static const int kBackgroundScrollSpeed = 30;
	static const int kBackgroundScrollSpeed2 = 70;
	static const int kBackgroundScrollSpeed3 = 0;
	static const int kBackgroundScrollSpeed4 = 100;
	static const int kNumberOfBackgrounds = 2;

	// These three flags check if we need to clear the respective vectors of nullptrs.
	bool m_shouldClearEnemies;
	bool m_shouldClearPlayerBullets;
	bool m_shouldClearEnemyBullets;

	bool m_canShoot; // This restricts the player from firing again unless they release the Spacebar.
	

	SDL_Texture* m_pBGText; // For the bg.
	Sprite m_bgArray[kNumberOfBackgrounds];

	SDL_Texture* m_pBGText2; // For the bg2.
	Sprite m_bgArray2[kNumberOfBackgrounds];
	
	SDL_Texture* m_pBGText3; // For the bg3.
	Sprite m_bgArray3[kNumberOfBackgrounds];

	SDL_Texture* m_pBGText4; // For the bg4.
	Sprite m_bgArray4[kNumberOfBackgrounds];

	SDL_Texture* m_pBGText5; // For the bg5.
	Sprite m_bgArray5[kNumberOfBackgrounds];

	SDL_Texture* m_pBGText6; // For the bg6.
	Sprite m_bgArray6[kNumberOfBackgrounds];

	SDL_Texture* m_pSpriteTexture; // For the sprites.
	Player* m_pPlayer;

	// The enemy spawn frame timer properties.
	float m_enemySpawnTime;
	float m_enemySpawnMaxTime;

	SDL_FPoint m_pivot;

	Mix_Music* m_pMusic;
	std::vector<Mix_Chunk*> m_sounds;

	std::vector<Enemy*> m_enemies;
	std::vector<Bullet*> m_playerBullets;
	std::vector<Bullet*> m_enemyBullets;

public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
	virtual void Pause() override;
	virtual void Resume() override;
	void CheckCollision();
	bool dying = false;
	float dyingTimer = 0.0f;
	float maxDie = 1.0f;
	void DiePause(float deltaTime);
};

class PauseState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;

	SDL_Texture* m_pPText; // For the resume butt
	PButton* pButton;
	SDL_Texture* m_pMMText; // For the main menu button
	MMButton* mmButton;
	SDL_Texture* m_pBGText;


};

class LoseState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;

	SDL_Texture* m_pMMText; // For the main menu button
	MMButton* mmButton;
	SDL_Texture* m_pBGText;
};

class WinState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;

	SDL_Texture* m_pMMText; // For the main menu button
	MMButton* mmButton;
	SDL_Texture* m_pBGText;
};

