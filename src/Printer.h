#ifndef Printer_h
#define Printer_h

#include <iostream>


class Printer
{
	float delta = 0.01f;
	float current = 0;
public:
	Printer() : delta(1 / 165) {};
	Printer(int hz) : delta((float)1 / (float)hz) {};

	template<typename T>
	void print(const char* error, T msg)
	{
		std::cout << current;
		if (current >= 1) {
			std::cout << "[" << error << "] - " << msg << "\n";
			current = 0;
		}
		current += delta;
	}

};

#endif