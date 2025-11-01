#pragma once
class Game
{
private:
public:
	Game();
	~Game();
	void Initialize();
	void Run();
	void ProcesInput();
	void Update();
	void Render();
	void Destroy();
};

