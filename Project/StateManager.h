#pragma once
#include <vector>

class State;

class StateManager
{
private:
	StateManager() {}
	static std::vector<State*> s_states;
	static bool s_stateChange;

public:
	static void Update(float deltaTime);
	static void Render();

	static void ChangeState(State* pState);


	static void PushState(State* pState);
	static void PopState();

	static void Quit();

	static std::vector<State*>& GetStates() { return s_states; }
	static bool StateChanging() { return s_stateChange; }
};