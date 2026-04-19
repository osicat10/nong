#include "NONG/input.h"

namespace NONG {
    const bool* Input::currentState = nullptr;
    std::vector<bool> Input::previousState;
    int Input::numKeys = 0;

    void Input::Update() 
    {
        if (currentState == nullptr) 
        {
            currentState = SDL_GetKeyboardState(&numKeys);
            previousState.resize(numKeys, false);
        }

        for (int i = 0; i < numKeys; i++) 
        {
            previousState[i] = currentState[i];
        }
    }

    bool Input::GetKey(SDL_Scancode key) 
    { 
        if (key < 0 || key >= numKeys) return false;
        return currentState[key]; 
    }

    bool Input::GetKeyPressed(SDL_Scancode key) 
    { 
        if (key < 0 || key >= numKeys) return false;
        return currentState[key] && !previousState[key]; 
    }

    bool Input::GetKeyReleased(SDL_Scancode key) 
    { 
        if (key < 0 || key >= numKeys) return false;
        return !currentState[key] && previousState[key]; 
    }

    void InputController::UpdateInput()
    {
        Input::Update();
    }
}