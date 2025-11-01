#include "Game.h"
#include <iostream>

Game::Game() {
	std::cout << "Game contructor called" << std::endl;
};

Game::~Game() {
	std::cout << "Game decontructor called" << std::endl;
};

void Game::Initialize() {};

void Game::Run() {};

void Game::ProcesInput() {};

void Game::Update() {};

void Game::Render() {};

void Game::Destroy() {};
