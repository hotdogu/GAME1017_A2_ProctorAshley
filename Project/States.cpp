#include "States.h"
#include "Game.h"
#include "StateManager.h"
#include <iostream>
#include "Utilities.h"

void State::Update(float deltaTime)
{
	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_ESCAPE))
	{
		std::cout << "State Quit game" << std::endl;
		Game::GetInstance()->QuitGame();
	}
}

///////////////////
////begin title state
/////////////////////
void TitleState::Enter()
{
	std::cout << "Entering Title State" << std::endl;

	m_pBGText = nullptr;
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/space.jpg");
	m_pPlayText = IMG_LoadTexture(pRenderer, "Assets/Images/playButton.png");


	SDL_FRect bDest;
	bDest.h = 150;
	bDest.w = 350;
	bDest.x = 10;
	bDest.y = 400;

	playButton = new PlayButton(bDest, m_pPlayText);
}




void TitleState::Update(float deltaTime)
{
	State::Update(deltaTime);

	playButton->Update(deltaTime);


	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_N))
	{
		std::cout << "Titlestate changing to game state" << std::endl;
		StateManager::ChangeState(new GameState());
	}
}
void TitleState::Render()
{
	
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

	SDL_RenderCopyF(pRenderer, m_pBGText, NULL, NULL);
	playButton->Render();
}
void TitleState::Exit()
{
	delete playButton;

	std::cout << "Exiting Title State" << std::endl;
}
////////////////
//end title state
///////////////


//////////////
///begin game state
////////////////
void GameState::Enter()
{
	std::cout << "Entering Game State" << std::endl;

	//Setting Variables NEW
	m_shouldClearEnemies = false;
	m_shouldClearEnemyBullets = false;
	m_canShoot = true;
	
	//bg stuff
	m_pBGText = nullptr;
	m_pBGText2 = nullptr;
	m_pBGText3 = nullptr;
	m_pBGText4 = nullptr;
	m_pBGText5 = nullptr;
	m_pBGText6 = nullptr;
	
	
	m_pSpriteTexture = nullptr;
	m_pPlayer = nullptr;
	m_enemySpawnTime = 20;
	m_enemySpawnMaxTime = 10.5f;
	m_pivot = { 0,0 };
	m_pMusic = nullptr;


	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

	/// //////////
	/// BG STUFF
	/// //////////

	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/BG1.png");
	m_pBGText2 = IMG_LoadTexture(pRenderer, "Assets/Images/BG2.png");
	m_pBGText3 = IMG_LoadTexture(pRenderer, "Assets/Images/BG3.png");
	m_pBGText4 = IMG_LoadTexture(pRenderer, "Assets/Images/BG4.png");
	m_pBGText5 = IMG_LoadTexture(pRenderer, "Assets/Images/BG5.png");
	m_pBGText5 = IMG_LoadTexture(pRenderer, "Assets/Images/BG6.png");
	m_pSpriteTexture = IMG_LoadTexture(pRenderer, "Assets/Images/sprites.png");

	m_bgArray[0] = { {0, 0, kWidth, kHeight}, {0, 0, kWidth, kHeight} };
	m_bgArray[1] = { {0, 0, kWidth, kHeight}, {kWidth, 0, kWidth, kHeight} };

	m_bgArray2[0] = { {0, 0, kWidth, kHeight}, {0, 0, kWidth, kHeight} };
	m_bgArray2[1] = { {0, 0, kWidth, kHeight}, {kWidth, 0, kWidth, kHeight} };

	m_bgArray3[0] = { {0, 0, kWidth, kHeight}, {0, 0, kWidth, kHeight} };
	m_bgArray3[1] = { {0, 0, kWidth, kHeight}, {kWidth, 0, kWidth, kHeight} };

	m_bgArray4[0] = { {0, 0, kWidth, kHeight}, {0, 0, kWidth, kHeight} };
	m_bgArray4[1] = { {0, 0, kWidth, kHeight}, {kWidth, 0, kWidth, kHeight} };

	m_bgArray5[0] = { {0, 0, kWidth, kHeight}, {0, 0, kWidth, kHeight} };
	m_bgArray5[1] = { {0, 0, kWidth, kHeight}, {kWidth, 0, kWidth, kHeight} };

	m_bgArray6[0] = { {0, 0, kWidth, kHeight}, {0, 0, kWidth, kHeight} };
	m_bgArray6[1] = { {0, 0, kWidth, kHeight}, {kWidth, 0, kWidth, kHeight} };

	/// /////////
	/// BG STUFFF
	/// /////////

	m_pMusic = Mix_LoadMUS("Assets/Audio/game.mp3"); // Load the music track.
	if (m_pMusic == nullptr)
	{
		std::cout << "Failed to load music: " << Mix_GetError() << std::endl;
	}

	m_sounds.reserve(4);
	Mix_Chunk* sound = Mix_LoadWAV("Assets/Audio/enemy.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/laser.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/die.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/Jump.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	m_pPlayer = new Player({ kPlayerSourceXPosition, kPlayerSourceYPosition, kPlayerWidth, kPlayerHeight }, // Image Source
		{ kWidth / 4 ,kHeight / 2 - kPlayerHeight / 2,kPlayerWidth,kPlayerHeight }, m_sounds[2], m_sounds[3], { 0, 250, kPlayerWidth, kPlayerHeight }, 
		{ kPlayerSourceXPosition, kPlayerSourceYPosition, kPlayerWidth, kPlayerHeight }, 
		{ 0, 497, kPlayerWidth, kPlayerHeight }); // Window Destination

	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(20); // 0-MIX_MAX_VOLUME (128).
}
void GameState::Update(float deltaTime)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	



	State::Update(deltaTime);
	

	////////// BG STUFF!
	// Scroll the background 1. 
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray[0].GetDestinationTransform()->x = 0;
		m_bgArray[1].GetDestinationTransform()->x = kWidth;
	}
	// Scroll the background 2.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray2[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed3 * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray2[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray2[0].GetDestinationTransform()->x = 0;
		m_bgArray2[1].GetDestinationTransform()->x = kWidth;
	}
	// Scroll the background 3.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray3[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed2 * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray3[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray3[0].GetDestinationTransform()->x = 0;
		m_bgArray3[1].GetDestinationTransform()->x = kWidth;
	}

	// Scroll the background 4.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray4[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed3 * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray4[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray4[0].GetDestinationTransform()->x = 0;
		m_bgArray4[1].GetDestinationTransform()->x = kWidth;
	}

	// Scroll the background 5.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray5[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed4 * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray5[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray5[0].GetDestinationTransform()->x = 0;
		m_bgArray5[1].GetDestinationTransform()->x = kWidth;
	}

	// Scroll the background 6.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray6[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray6[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray6[0].GetDestinationTransform()->x = 0;
		m_bgArray6[1].GetDestinationTransform()->x = kWidth;
	}
	///////BG STFF

	// Player animation/movement.
	m_pPlayer->Update(deltaTime);

	//Check for Ground
	float btmFloor = 850;
	if (m_pPlayer->m_ducking)
	{
		btmFloor += 40;
	}
	if (m_pPlayer->GetDestinationTransform()->y > btmFloor)
	{
		m_pPlayer->StopY();
		m_pPlayer->SetGrounded(true);
		m_pPlayer->SetY(btmFloor);
	}

	m_pPlayer->Animate(deltaTime); 

	/*if (Game::GetInstance()->KeyDown(SDL_SCANCODE_A) && m_pPlayer->GetDestinationTransform()->x > m_pPlayer->GetDestinationTransform()->h)
	{
		m_pPlayer->GetDestinationTransform()->x -= kPlayerSpeed * deltaTime;
	}
	else if (Game::GetInstance()->KeyDown(SDL_SCANCODE_D) && m_pPlayer->GetDestinationTransform()->x < kWidth / 2)
	{
		m_pPlayer->GetDestinationTransform()->x += kPlayerSpeed * deltaTime;
	}
	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_W) && m_pPlayer->GetDestinationTransform()->y > 0)
	{
		m_pPlayer->GetDestinationTransform()->y -= kPlayerSpeed * deltaTime;
	}
	else if (Game::GetInstance()->KeyDown(SDL_SCANCODE_S) && m_pPlayer->GetDestinationTransform()->y < kHeight - m_pPlayer->GetDestinationTransform()->w)
	{
		m_pPlayer->GetDestinationTransform()->y += kPlayerSpeed * deltaTime;
	}*/

	// Enemy animation/movement.
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Update(deltaTime);
		if (m_enemies[i]->GetDestinationTransform()->x < -m_enemies[i]->GetDestinationTransform()->h)
		{
			delete m_enemies[i];
			m_enemies[i] = nullptr;
			m_shouldClearEnemies = true;
		}
	}
	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	// Update enemy spawns.
	m_enemySpawnTime += deltaTime;
	if (m_enemySpawnTime > m_enemySpawnMaxTime)
	{
		int rap =  rand() % 3;
		if (rap == 0) {
			// Randomizing enemy bullet spawn rate
			float bulletSpawnRate = 0.5f + (rand() % 3) / 0.2f;
			// Random starting y location
			float yEnemyLocation = kHeight;
			m_enemies.push_back(
				new Enemy({ kEnemySourceXPosition, kEnemySourceYPosition, kEnemyWidth, kEnemyHeight },
					{ kWidth, yEnemyLocation, kEnemyWidth, kEnemyHeight },
					&m_enemyBullets,
					m_sounds[0],
					bulletSpawnRate, true, 4));

			m_enemySpawnTime = (float)(rand() % ((int)m_enemySpawnMaxTime * 100)) / 100;
		}
		else if (rap == 1)
		{
			// Random starting y location
			float yEnemyLocation = kHeight - 80;
			m_enemies.push_back(
				new Enemy({ 0, 425, 50, 65 },
					{ kWidth, yEnemyLocation, 50, 65 },
					nullptr,
					m_sounds[0],
					0, false, 3));

			m_enemySpawnTime = (float)(rand() % ((int)m_enemySpawnMaxTime * 100)) / 100;
		}
		else
		{
			// Random starting y location
			float yEnemyLocation = kHeight;
			m_enemies.push_back(
				new Enemy({ kEnemySourceXPosition, 330, 60, 100 },
					{ kWidth, yEnemyLocation, 60, 100 },
					nullptr,
					m_sounds[0],
					0, false, 1));

			m_enemySpawnTime = (float)(rand() % ((int)m_enemySpawnMaxTime * 100)) / 100;
		}
	}


	// Now enemy bullets.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		m_enemyBullets[i]->Update(deltaTime);
		if (m_enemyBullets[i]->GetDestinationTransform()->x < -m_enemyBullets[i]->GetDestinationTransform()->w)
		{
			delete m_enemyBullets[i];
			m_enemyBullets[i] = nullptr;
			m_shouldClearEnemyBullets = true;
		}
	}

	if (m_shouldClearEnemyBullets)
	{
		CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemyBullets);

	}

	if (!dying)
	{
		CheckCollision();
	}
	else
	{
		DiePause(deltaTime);
	}

	//Pausing
	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_P))
	{
		StateManager::PushState(new PauseState);
	}
}
void GameState::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

	// Render stuff. Background first. BG stuff
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(pRenderer, m_pBGText, m_bgArray[i].GetSourceTransform(), m_bgArray[i].GetDestinationTransform());
	}
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(pRenderer, m_pBGText2, m_bgArray2[i].GetSourceTransform(), m_bgArray2[i].GetDestinationTransform());
	}
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(pRenderer, m_pBGText3, m_bgArray3[i].GetSourceTransform(), m_bgArray3[i].GetDestinationTransform());
	}
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(pRenderer, m_pBGText4, m_bgArray4[i].GetSourceTransform(), m_bgArray4[i].GetDestinationTransform());
	}
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(pRenderer, m_pBGText5, m_bgArray5[i].GetSourceTransform(), m_bgArray5[i].GetDestinationTransform());
	}
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(pRenderer, m_pBGText6, m_bgArray6[i].GetSourceTransform(), m_bgArray6[i].GetDestinationTransform());
	}

	// Player.
	SDL_RenderCopyExF(pRenderer,
		m_pSpriteTexture,
		m_pPlayer->GetSourceTransform(),
		m_pPlayer->GetDestinationTransform(),
		m_pPlayer->GetAngle(),
		&m_pivot,
		SDL_FLIP_NONE);


	//std::cout << m_pPlayer->GetDestinationTransform()->x << " " << m_pPlayer->GetDestinationTransform()->y << std::endl;

	// Enemies.
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		SDL_RenderCopyExF(pRenderer, m_pSpriteTexture, m_enemies[i]->GetSourceTransform(), m_enemies[i]->GetDestinationTransform(), -90, &m_pivot, SDL_FLIP_NONE);
	}

	// Enemy bullets.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		SDL_RenderCopyF(pRenderer, m_pSpriteTexture, m_enemyBullets[i]->GetSourceTransform(), m_enemyBullets[i]->GetDestinationTransform());
	}
}

void GameState::Exit()
{
	std::cout << "Exiting Game State" << std::endl;

	std::cout << "Clean ..." << std::endl;

	// Delete Player
	delete m_pPlayer;
	m_pPlayer = nullptr;

	// Destroy Enemies
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		delete m_enemies[i];
		m_enemies[i] = nullptr;
	}
	m_enemies.clear();

	// Destroy Enemy Bullets
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		delete m_enemyBullets[i];
		m_enemyBullets[i] = nullptr;
	}
	m_enemyBullets.clear();

	// Clean sounds up
	for (size_t i = 0; i < m_sounds.size(); i++)
	{
		Mix_FreeChunk(m_sounds[i]);
	}
	m_sounds.clear();

	//Mix_FreeMusic(m_pMusic);
	//m_pMusic = NULL;
}

void GameState::CheckCollision()
{
	// Player vs. Enemy.
	SDL_Rect playerCollisionRect = { (int)m_pPlayer->GetDestinationTransform()->x - kPlayerHeight, (int)m_pPlayer->GetDestinationTransform()->y, kPlayerHeight, kPlayerWidth };
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		SDL_Rect enemyCollisionRect = { (int)m_enemies[i]->GetDestinationTransform()->x, (int)m_enemies[i]->GetDestinationTransform()->y - kEnemyWidth, kEnemyHeight, kEnemyWidth };
		if (SDL_HasIntersection(&playerCollisionRect, &enemyCollisionRect))
		{
			// Game over!
			std::cout << "Player goes boom!" << std::endl;
			Mix_PlayChannel(-1, m_sounds[2], 0);
			delete m_enemies[i];
			m_enemies[i] = nullptr;
			m_shouldClearEnemies = true;
			dying = true;
			CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
			m_pPlayer->Die();
			return;
		}
	}

	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}


	// Enemy bullets vs. player.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		SDL_Rect enemyBulletCollisionRect = { (int)m_enemyBullets[i]->GetDestinationTransform()->x, (int)m_enemyBullets[i]->GetDestinationTransform()->y, (int)m_enemyBullets[i]->GetDestinationTransform()->w, (int)m_enemyBullets[i]->GetDestinationTransform()->h };
		if (SDL_HasIntersection(&playerCollisionRect, &enemyBulletCollisionRect))
		{
			// Game over!
			std::cout << "Player goes boom!" << std::endl;
			Mix_PlayChannel(-1, m_sounds[2], 0);
			delete m_enemyBullets[i];
			m_enemyBullets[i] = nullptr;
			m_shouldClearEnemyBullets = true;
			dying = true;
			CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemies);
			m_pPlayer->Die();
			return;
		}
	}

	if (m_shouldClearEnemyBullets)
	{
		CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemyBullets);
	}
}

void GameState::DiePause(float deltaTime)
{
	dyingTimer += deltaTime;
	m_pPlayer->Animate(deltaTime);

	if (dyingTimer >= maxDie)
	{
		//end the game
		StateManager::ChangeState(new LoseState());
	}
}

void GameState::Pause()
{
	std::cout << "pausing" << std::endl;
}

void GameState::Resume()
{
	std::cout << "Resuming" << std::endl;
}
////////////
//end game state
//////////////


///////////////////
////begin Pause state
/////////////////////
void PauseState::Enter()
{
	std::cout << "Entering Pause State" << std::endl;
	
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/Pause.jpg");
	m_pPText = IMG_LoadTexture(pRenderer, "Assets/Images/PButton.png");
	m_pMMText = IMG_LoadTexture(pRenderer, "Assets/Images/MMButton.png");


	SDL_FRect bDest;
	bDest.h = 200;
	bDest.w = 400;
	bDest.x = 312;
	bDest.y = 284;
	pButton = new PButton(bDest, m_pPText);


	SDL_FRect aDest{};
	bDest.h = 200;
	bDest.w = 400;
	bDest.x = 12;
	bDest.y = 84;
	mmButton = new MMButton(aDest, m_pMMText);
	
	
}
void PauseState::Update(float deltaTime)
{
	State::Update(deltaTime);
	pButton->Update(deltaTime);
	mmButton->Update(deltaTime);

	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_R))
	{
		std::cout << "unpausing" << std::endl;
		StateManager::PopState();
	}
}
void PauseState::Render()
{
		StateManager::GetStates().front()->Render();
		SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

		SDL_SetRenderDrawBlendMode(Game::GetInstance()->GetRenderer(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(Game::GetInstance()->GetRenderer(), 128, 128, 128, 128);
		SDL_Rect bg{ 256,128, 512, 512 };
		SDL_RenderFillRect(Game::GetInstance()->GetRenderer(), &bg);
		//SDL_RenderCopyF(pRenderer, m_pBGText, NULL, NULL);
		pButton->Render();
		mmButton->Render();


		
}
void PauseState::Exit()
{
	std::cout << "Exiting Pause State" << std::endl;
	delete mmButton;
	delete pButton;
}
////////////////
//end pause state
///////////////

///////////////////
////begin Lose state
/////////////////////
void LoseState::Enter()
{
	std::cout << "Entering lose State" << std::endl;

	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/Lose.jpg");
	m_pMMText = IMG_LoadTexture(pRenderer, "Assets/Images/MMButton.png");


	SDL_FRect bDest;
	bDest.h = 200;
	bDest.w = 200;
	bDest.x = 50;
	bDest.y = 50;

	mmButton = new MMButton(bDest, m_pMMText);
}
void LoseState::Update(float deltaTime)
{
	State::Update(deltaTime);
	mmButton->Update(deltaTime);

	
}
void LoseState::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer, 128, 128, 128, 50);
	SDL_RenderClear(pRenderer);
	SDL_RenderCopyF(pRenderer, m_pBGText, NULL, NULL);
	mmButton->Render();



}
void LoseState::Exit()
{
	std::cout << "Exiting lose State" << std::endl;
	delete mmButton;
}
////////////////
//end lose state
///////////////

///////////////////
////begin win state
/////////////////////
void WinState::Enter()
{
	std::cout << "Entering win State" << std::endl;

	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/win.jpg");
	m_pMMText = IMG_LoadTexture(pRenderer, "Assets/Images/MMButton.png");


	SDL_FRect bDest;
	bDest.h = 200;
	bDest.w = 200;
	bDest.x = 50;
	bDest.y = 50;

	mmButton = new MMButton(bDest, m_pMMText);
}
void WinState::Update(float deltaTime)
{
	State::Update(deltaTime);
	mmButton->Update(deltaTime);


}
void WinState::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();


	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer, 128, 128, 128, 50);
	SDL_RenderClear(pRenderer);
	SDL_RenderCopyF(pRenderer, m_pBGText, NULL, NULL);
	mmButton->Render();



}
void WinState::Exit()
{
	delete mmButton;
	std::cout << "Exiting win State" << std::endl;
}
////////////////
//end win state
///////////////
