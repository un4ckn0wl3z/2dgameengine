#include <iostream>
#include "SDL.h"

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
