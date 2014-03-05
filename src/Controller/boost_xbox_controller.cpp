#include "../../include/Controller/boost_xbox_controller.hpp"

bool XBoxControllerManager::initialize_sdl(){
    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0){
        return false;
    }
    SDL_JoystickEventState(SDL_ENABLE);
    return true;
}

XBoxControllerManager::XBoxControllerManager(){
    controller_ready = false;
}

XBoxControllerManager::~XBoxControllerManager(){
    SDL_JoystickClose(this->xbox_controller);
    SDL_Quit();
}

bool XBoxControllerManager::initialize(){
    bool success = initialize_sdl();
    if (!success){
	std::cout << "Failed to initialise SDL" << std::endl;
        return false;
    }
    SDL_JoystickEventState(SDL_ENABLE);
    this->xbox_controller = SDL_JoystickOpen(0);
    if(this->xbox_controller == NULL){
	std::cout << "Failed to open controller" << std::endl;
        return false;
    }
    this->controller_ready = true;
    return true;
}
/*
int* XBoxControllerManager::get_next_event(){
    if(!this->controller_ready){
        return items_to_tuple(0, 0);
    }
    bool event_exists = SDL_PollEvent(&this->event);
    if (!event_exists){
        return items_to_tuple(0, 0);
    }
    switch(event.type){
        case SDL_JOYAXISMOTION:  // Handle Joystick Motion 
            if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) ){
                int joystick_value = event.jaxis.value;
                if( event.jaxis.axis == 0) {
                    return items_to_tuple(LEFT_THUMBSTICK_HORIZONTAL, joystick_value);
                }
                else if( event.jaxis.axis == 1) {
                    return items_to_tuple(LEFT_THUMBSTICK_VERTICAL, joystick_value);
                }
                else if( event.jaxis.axis == 2){
                    return items_to_tuple(RIGHT_THUMBSTICK_HORIZONTAL, joystick_value);
                }
                else if( event.jaxis.axis == 3){
                    return items_to_tuple(RIGHT_THUMBSTICK_VERTICAL, joystick_value);
                }
            }
            break;
        case SDL_JOYBUTTONUP:
            return items_to_tuple(BUTTON_RELEASED, event.jbutton.button);
            break;
        case SDL_JOYBUTTONDOWN:  // Handle Joystick Button Presses 
            return items_to_tuple(BUTTON_PRESSED, event.jbutton.button);
            break;
    }
    return items_to_tuple(0, 0);
}
*/
void XBoxControllerManager::monitorEvents(){
    SDL_Event event;
    bool event_exists;

    if (!this->controller_ready){
	return;
    }
    event_exists = SDL_PollEvent(&this->event);
    if (event_exists) {
	if (event.type == SDL_JOYAXISMOTION) {
	    std::cout << "Axis :: " << std::endl;//(unsigned int) event.jaxis.axis << std::endl;
	    std::cout << "Value :: " << std::endl;//(signed int) event.jaxis.value << std::endl << std::endl;
	}
	else if (event.type == SDL_JOYBUTTONDOWN)
	    std::cout << "Button :: " << std::endl;//(unsigned int) event.jbutton.button << std::endl << std::endl;
    }
}

// If you want to run a program from C++ your main() function MUST have the following params
/*
int main(int argc, char *argv[]){
    XBoxControllerManager cont;
    cont.initialize();
    while(true) 
	cont.monitorEvents();
    return 1;
}
*/
// Note to self - the controller is not opening. Google this in morning.
