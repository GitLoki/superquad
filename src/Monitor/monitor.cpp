Monitor::Monitor(): target(3), location(3), lights(false){};


void Monitor::set_target(std::vector<float> values){
    target = values;
}

void Monitor::set_location(std::vector<float> values){
    location = values;
}

void Monitor::lightswitch(){
    lights = !lights;
}


void Monitor::get_target(std::vector<float> values){
    values = target;
}

void Monitor::get_location(std::vector<float> values){
    values = location;
}

bool Monitor::get_light(){
    return lights;
}
