#include <iostream>
#include "CRTMatrix.h"

int main()
{
    CRTVector v(2.0f, 3.0f, 1.0f);  

    CRTMatrix M({
    {1.0f, 4.0f, 7.0f},  
    {2.0f, 5.0f, 8.0f},  
    {3.0f, 6.0f, 9.0f}   
        });

    CRTVector result = v * M;  
    std::cout << result;
}

