#include <iostream>
#include "LongNumber.h"

int main()
{
    LongNumber n1("-56");
    LongNumber n2("27");

    LongNumber n3 = n1 * n2;

    std::string s = n3.getString();

    std::cout << "number: " << s << std::endl;

}


