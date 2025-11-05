#pragma once
#include "Shape.h"

class Rectangle : public Shape
{
public:
	int w, h;

	Rectangle(int x, int y, int w, int h) : Shape(x, y), w(w), h(h) {}
};