#include <iostream>
//#include "SDL.h"
//#include "SDL_image.h"
//#include "SDL_ttf.h"
//#include "SDL_mixer.h"
//#include "glm.hpp"
//#include "imgui.h"
//extern "C"
//{
//    #include "lua.h"
//    #include "lauxlib.h"
//    #include "lualib.h"
//}
//#include "sol.hpp"

#include "Game.h"

int main(int argc, char* argv[]){

    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();

    return 0;
}

// cont-> 82
