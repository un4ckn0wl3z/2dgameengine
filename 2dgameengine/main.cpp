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

int nativeCube(int n) {
    return (n * n * n);
}

void TestLua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    // bind c native function to lua
    lua["cube"] = nativeCube;

    lua.script_file("./assets/scripts/myscript.lua");

    int l_some_var = lua["some_var"];
    std::cout << "The value of some variable inside c++ is: " << l_some_var << std::endl;

    // get configs from lua script
    bool isFullScreen = lua["config"]["fullscreen"];
    sol::table config = lua["config"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];

    std::cout << "We read the value fullscreen: " << static_cast<bool>(isFullScreen) << std::endl;
    std::cout << "We read the value width: " << width << std::endl;
    std::cout << "We read the value height: " << height << std::endl;

    sol::function l_fac = lua["fac"];
    int l_fac_result = l_fac(5);

    std::cout << "5! from lua: " << l_fac_result << std::endl;


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