#pragma once
class Vector2D
{
public:

	float x, y;

	Vector2D::Vector2D()
	{

	}
	Vector2D(float X, float Y)
	{
		x = X;
		y = Y;
	}



	inline Vector2D& Vector2D::operator=(const Vector2D& vOther)
	{
		x = vOther.x; y = vOther.y;
		return *this;
	}
};