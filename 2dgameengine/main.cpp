#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "glm.hpp"

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    glm::vec2 velocity = glm::vec2(5.0, -2.0);
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
