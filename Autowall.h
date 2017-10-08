#pragma once
struct FireBulletData
{
    FireBulletData( const Vector &eye_pos ) : src( eye_pos )
    {
    }

    Vector						src;
    trace_t       enter_trace;
    Vector						direction;
    CTraceFilter  filter;
    float						trace_length;
    float						trace_length_remaining;
    float						current_damage;
    int							penetrate_count;
};
class CAutowall
{
public:
    //Old Awall
    bool HandleBulletPenetration( CSWeaponInfo* pWeaponData, FireBulletData& BulletData );
    bool SimulateFireBullet( CBaseEntity* pBaseEntity, FireBulletData& BulletData );
    bool PenetrateWall( CBaseEntity* pBaseEntity, const Vector& vecPoint );






}; extern CAutowall* g_Autowall;
extern void ScaleDamage( int hitgroup, CBaseEntity *enemy, float weapon_armor_ratio, float &current_damage );
extern void UTIL_ClipTraceToPlayers( const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr );

