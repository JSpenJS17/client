#ifndef TODOERROR_CPP
#define TODOERROR_CPP

#include <iostream>

class ToDoError
{
public:
    ToDoError()
    {
        std::cerr << "Threw a ToDoError." << std::endl;
    }
};

#endif