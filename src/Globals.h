#ifndef Globals_HPP
#define Globals_HPP

#pragma once
#include <vector>
//#include <winsock2.h> // Include-Reihenfolge wichtig!
//#include <Ws2tcpip.h>
#include <Windows.h> // windows.h muss als letztes
#include <string.h>
#include <iostream>

// Aktuell nur getestet unter Windows 10
#ifdef WIN32
#define ASSETS "../../assets/"
#define SHADERS "../../assets/shaders/"
#define WORLEY_AMOUNT 30
#define MAX_TEXTURE_ARR_SIZE 50

#define DEBUG_MESSAGES true

#define ASPECT_RATIO 16.0f/9.0f
#define ASPECT_WIDTH 1920
#define ASPECT_HEIGHT 1080

#define NC "\033[0m"
#define RED "\033[31m"
#define GRN "\033[92m"
#define CYN "\033[96m"
#define BOLD "\033[1m"
#define NF "\033[0m"

#pragma once
#include <iomanip>
template<typename T>
static void print(const char* title, T msg, bool error = false, int tabs = 0)
{
    if (!DEBUG_MESSAGES) return;


    std::string space = "";
    for (int i = 0; i < tabs; i++) space += "  ";
    if (error)
    {
        std::cout << space << std::left << std::setw(30) << RED << title << NC <<
            std::setw(30) << std::right << BOLD << msg << NF << "\n"; // red print
    }
    else
    {
        std::cout << space << std::left << std::setw(30) << title <<
            std::right << msg<< "\n"; // red print
    }
}

static void printDivider(int size = 45)
{
    std::cout << std::setw(size) << std::setfill('-') << "\n" << std::setfill(' ');
}


#else
// Kompatibilitaet fuer andere Betriebssysteme ueberpruefen
#define ASSETS "../assets/"
#endif


#endif // Globals_HPP
