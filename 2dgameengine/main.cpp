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

#include "lua.hpp"
#include "sol.hpp"

#include "Game.h"

void TestLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script_file("./assets/scripts/myscript.lua");

    int l_some_val = lua["some_val"];
    std::cout << "The valur of some variable inside c++ is: " << l_some_val << std::endl;

}

int main(int argc, char* argv[]){

    //Game game;
    //game.Initialize();
    //game.Run();
    //game.Destroy();

    
    TestLua();

    return 0;
}

// cont-> 133.