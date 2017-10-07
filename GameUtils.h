#pragma once
namespace GameUtils
{
	bool WorldToScreen(const Vector& in, Vector& position);
	Vector GetHitboxVector(CBaseEntity* pBaseEntity, int iHitbox, matrix3x4 BoneMatrixArray[128]);
	std::vector<Vector> GetMultiplePointsForHitbox(CBaseEntity* pBaseEntity, int iHitbox, VMatrix BoneMatrix[128]);
	Vector GetBonePosition(CBaseEntity* pPlayer, int Bone, matrix3x4 MatrixArray[128]);
	float GetFoV(QAngle qAngles, Vector vecSource, Vector vecDestination, bool bDistanceBased);
	QAngle CalculateAngle(Vector vecOrigin, Vector vecOther);
	void TraceLine(Vector& vecAbsStart, Vector& vecAbsEnd, unsigned int mask, CBaseEntity* ignore, trace_t* ptr);
	bool IsVisible_Fix(Vector vecOrigin, Vector vecOther, unsigned int mask, CBaseEntity* pCBaseEntity, CBaseEntity* pIgnore, int& hitgroup);
	bool IsAbleToShoot();
	bool IsBreakableEntity(CBaseEntity* pBaseEntity);
}