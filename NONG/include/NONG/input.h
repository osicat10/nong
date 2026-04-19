#ifndef NONG_INPUT_H
#define NONG_INPUT_H

#include <SDL3/SDL.h>
#include <vector>

namespace NONG {

    class InputController
    {
    private:
        InputController();
    public:
        static void UpdateInput();
    };

    class Input 
    {
    private:
        Input();
        
        static const bool* currentState; 
        
        static std::vector<bool> previousState; 
        static int numKeys;

        static void Update(); 
    public:
        static bool GetKey(SDL_Scancode key);
        static bool GetKeyPressed(SDL_Scancode key);
        static bool GetKeyReleased(SDL_Scancode key);   

        friend class InputController;
    };
}

#endif