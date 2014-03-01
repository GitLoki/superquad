#include <iostream>
#include "SDL.h"

#define LEFT_THUMBSTICK_HORIZONTAL 1
#define LEFT_THUMBSTICK_VERTICAL 2
#define RIGHT_THUMBSTICK_HORIZONTAL 3
#define RIGHT_THUMBSTICK_VERTICAL 4
#define BUTTON_PRESSED 5
#define BUTTON_RELEASED 6


class XBoxControllerManager{
    private:
        bool initialize_sdl();
        SDL_Joystick *xbox_controller;
        bool controller_ready;
        SDL_Event event;
    public:
        XBoxControllerManager();
        ~XBoxControllerManager();
        bool controller_is_ready(){ return controller_ready; }
        bool initialize();
        int* get_next_event();
        void monitorEvents();
};
