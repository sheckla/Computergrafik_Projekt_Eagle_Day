/*
 * Globale Utility Funktionen, Includes und Definitionen
 */
#ifndef Globals_HPP
#define Globals_HPP

// Globale Includes
#pragma once
#include <vector>
#include <winsock2.h> // Include-Reihenfolge wichtig!
#include <Ws2tcpip.h>
#include <Windows.h> // windows.h muss als letztes
#include <string.h>
#include <iostream>

// Windows includes
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#include "freeimage.h"
#define _USE_MATH_DEFINES
#include <math.h>

// Directories
#define ASSETS "../../assets/"
#define SHADERS "../../assets/shaders/"

// Shader
#define WORLEY_AMOUNT 30

// Eagle Day Color Scheme
#define COL_DARK Color(0.38, 0.18, 0.12)
#define COL_LIGHT Color(0.6,0.5,0.3)
#define COL_VERY_LIGHT Color(0.6*1.3,0.5*1.3,0.3*1.3)

// Bildschirminformationen
#define ASPECT_RATIO 16.0f/9.0f
static float ASPECT_WIDTH = 1920;
static float ASPECT_HEIGHT = 1080;

// Debug-print Farben
#define DEBUG_MESSAGES true
#define NC "\033[0m"
#define RED "\033[31m"
#define GRN "\033[92m"
#define CYN "\033[96m"
#define BOLD "\033[1m"
#define NF "\033[0m"

//Quality
#define QUALITY 2;

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
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#define ASSETS "../assets/"
#endif


#endif // Globals_HPP
