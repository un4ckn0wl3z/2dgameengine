#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "glm.hpp"
#include "imgui.h"
extern "C"
{
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}
#include "sol.hpp"

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    glm::vec2 velocity = glm::vec2(5.0, -2.0);
    std::cout << "Hello, world!" << std::endl;
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    return 0;
}
