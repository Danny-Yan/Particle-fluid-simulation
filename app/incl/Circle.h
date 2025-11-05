#pragma once
#include "Shape.h"

class Circle : public Shape
{
public:
	int r;

	Circle(int x, int y, int r) : Shape(x, y), r(r){}
};

