#ifndef ACTUATOR_HPP
#define  ACTUATOR_HPP

class Actuator {
public:
    virtual void halt() = 0;
    virtual void getValues(int* _values) = 0;
    virtual void setValues(int* _values) = 0;
};

#endif
