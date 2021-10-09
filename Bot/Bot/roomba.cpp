// roomba.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "screenReader.h"
#include "farmSequence.h"
#include "screenClicker.h"

int main()
{
    std::cout << "Initializing\n";

    g_screen.init();
    g_sequence.init();

    std::cout << "Roomba start\n";

    bool done = false;
    while (!done)
    {
        if (g_screen.pReadback && g_screen.pReadback->windowPosX > 0)
        {
            g_sequence.tick();
        }
        g_screen.tickCompute();
    }

    g_screen.terminate();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
