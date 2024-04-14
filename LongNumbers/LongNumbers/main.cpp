#include <iostream>
#include "LongNumber.h"

int main()
{
    LongNumber n1("1");
    LongNumber n2("2");

    LongNumber n3 = n1 - n2;

    std::string s = n3.getString();

    std::cout << "number: " << s << std::endl;

}


