#pragma once
namespace Math
{
	extern float NormalizeYaw(float value);
	extern void VectorAngles(const Vector&vecForward, Vector&vecAngles);
	extern void AngleVectors(const Vector angles, Vector& forward, Vector& right, Vector& up);
	extern void AngleVectors(const QAngle &angles, Vector* forward);
	extern void ClampAngles(QAngle& angles);
	extern float RandomFloat(float min, float max);
	extern void AngleVectors(const Vector &angles, Vector *forward, Vector *right, Vector *up);
}