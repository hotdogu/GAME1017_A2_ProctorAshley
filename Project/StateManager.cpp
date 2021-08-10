#include "StateManager.h"
#include "States.h"

std::vector<State*> StateManager::s_states;
bool StateManager::s_stateChange = false;

void  StateManager::Update(float deltaTime)
{
	s_stateChange = false;
	if (!s_states.empty())
	{
		s_states.back()->Update(deltaTime);
	}
}
void  StateManager::Render()
{
	if (!s_states.empty())
	{
		s_states.back()->Render();
	}
}

void  StateManager::ChangeState(State* pState)
{
	s_stateChange = true;
	if (!s_states.empty())
	{
		s_states.back()->Exit();
		delete s_states.back();
		s_states.back() = nullptr;
		s_states.pop_back();

	}
	s_states.push_back(pState);
	s_states.back()->Enter();
	
	
}


void  StateManager::PushState(State* pState)
{
	s_states.back()->Pause();
	s_states.push_back(pState);
	s_states.back()->Enter();
}
void  StateManager::PopState()
{
	s_stateChange = true;
	if (s_states.size() <= 1)
		return;

	if (!s_states.empty())
	{
		s_states.back()->Exit();
		delete s_states.back();
		s_states.back() = nullptr;
		s_states.pop_back();
	}
	s_states.back()->Resume();
}

void  StateManager::Quit()
{
	while (!s_states.empty())
	{
		s_states.back()->Exit();
		delete s_states.back();
		s_states.back() = nullptr;
		s_states.pop_back();
		s_states.clear();
	}
}