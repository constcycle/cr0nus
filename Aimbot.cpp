#include "sdk.h"
#include "Aimbot.h"
#include "global.h"
#include "Menu.h"
#include "Math.h"
#include "Resolver.h"
#include "GameUtils.h"
#include "Autowall.h"
#include "BacktrackingHelper.h"
#include "NoSpread.h"
#define TICK_INTERVAL			( g_pGlobals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )

template<class T, class U>
inline T clamp( T in, U low, U high )
{
    if( in <= low )
        return low;
    else if( in >= high )
        return high;
    else
        return in;
}
std::vector<Vector> MainHitbox( CBaseEntity* pTarget, VMatrix BoneMatrix[ 128 ] )
{
    std::vector<Vector> Points;
    int Aimspot = RagebotElements.Ragebot_Hitbox->SelectedIndex;

    //if (m_pPlayerlist[pTarget->Index()].bbAim)
    //Points = GameUtils::GetMultiplePointsForHitbox(Points, 3, pTarget);

    if( G::MainWeapon->WeaponID() == AWP && RagebotElements.Ragebot_BodyAWP->Checked ) {
        //Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 6, BoneMatrix);

        Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 3, BoneMatrix );

        //Points = GameUtils::GetMultiplePointsForHitbox(pTarget, 4, BoneMatrix);
    }
    else {
        switch( Aimspot ) {
        case 0:
            Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 0, BoneMatrix );
            break;
        case 1:
            Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 1, BoneMatrix );
            break;
        case 2:
            Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 6, BoneMatrix );
            break;
        case 3:
            Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 3, BoneMatrix );//pelvis
            break;
        case 4:
            Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 4, BoneMatrix );//stomach
            break;
        case 5:
            int iShots = G::resolver_ticks[ pTarget->GetIndex() ];
            if( iShots < 2 )
                Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 0, BoneMatrix );
            else
                Points = GameUtils::GetMultiplePointsForHitbox( pTarget, 3, BoneMatrix );
            break;
        }
    }
    return Points;
}
std::vector<int> GetHitboxesToScan( CBaseEntity* pTarget )
{
    std::vector<int> HitBoxesToScan;
    int HitScanMode = RagebotElements.Ragebot_Hitscan->SelectedIndex;
    int Aimspot = RagebotElements.Ragebot_Hitbox->SelectedIndex;




    switch( HitScanMode ) {
    case 0:
        break;
    case 1:
        // head/body
        if( Aimspot != 0 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Head );
        else if( Aimspot != 1 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Neck );
        else if( Aimspot != 2 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::UpperChest );
        else if( Aimspot != 3 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Pelvis );
        else if( Aimspot != 4 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Stomach );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::Chest );
        break;
    case 2:
        // basic +(arms, thighs)
        if( Aimspot != 0 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Head );
        else if( Aimspot != 1 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Neck );
        else if( Aimspot != 2 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::UpperChest );
        else if( Aimspot != 3 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Pelvis );
        else if( Aimspot != 4 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Stomach );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::Chest );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::LeftUpperArm );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::RightUpperArm );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::LeftThigh );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::RightThigh );
    case 3:
        // heaps ++(just all the random shit)
        if( Aimspot != 0 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Head );
        else if( Aimspot != 1 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Neck );
        else if( Aimspot != 2 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::UpperChest );
        else if( Aimspot != 3 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Pelvis );
        else if( Aimspot != 4 )
            HitBoxesToScan.push_back( (int)CSGOHitboxID::Stomach );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::Chest );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::LeftUpperArm );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::RightUpperArm );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::LeftThigh );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::RightThigh );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::LeftHand );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::RightHand );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::LeftFoot );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::RightFoot );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::LeftShin );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::RightShin );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::LeftLowerArm );
        HitBoxesToScan.push_back( (int)CSGOHitboxID::RightLowerArm );
    }



    return HitBoxesToScan;
}

Vector RunAimScan( CBaseEntity* pTarget, float &simtime, Vector& origin )
{
    Vector vEyePos = G::LocalPlayer->GetEyePosition();
    auto wpn_data = G::WeaponData;

    float minimum_damage = ( pTarget->GetHealth() < RagebotElements.Ragebot_MinDamage->value ) ? pTarget->GetHealth() : RagebotElements.Ragebot_MinDamage->value;

    bool found = false;

    int index = pTarget->Index();



    VMatrix BoneMatrix[ 128 ];
    if( RagebotElements.Ragebot_Corrections_PositionAdjustment->Checked ) {




        std::vector<tick_record> lby_records, trash_records;

        /*fix for hitscan and multipoints only on the last records*/
        float latest_time = 0.0f;
        tick_record latest_record;

        //do it that way to use the latest nonfakelagging record
        for( int j = g_BacktrackHelper->PlayerRecord[ index ].records.size() - 1; j >= 0; j-- ) //works pretty good for nospread
        {
            tick_record record = g_BacktrackHelper->PlayerRecord[ index ].records.at( j );


            float lerptime = g_BacktrackHelper->GetLerpTime();
            float desired_time = record.m_flSimulationTime + lerptime;
            float estimated_time = g_BacktrackHelper->GetEstimateServerTime();



            float SV_MAXUNLAG = 1.0f;


            float latency = g_BacktrackHelper->GetNetworkLatency();
            float m_flLerpTime = g_BacktrackHelper->GetLerpTime();
            float correct = clamp<float>( latency + m_flLerpTime, 0.0f, SV_MAXUNLAG );

            float deltaTime = correct - ( estimated_time + lerptime - desired_time );


            if( fabs( deltaTime ) > 0.2f )
                continue;



            if( record.m_flSimulationTime == g_BacktrackHelper->PlayerRecord[ index ].records.back().m_flSimulationTime ) {
                latest_time = g_BacktrackHelper->PlayerRecord[ index ].records.at( j ).m_flSimulationTime;
                latest_record = g_BacktrackHelper->PlayerRecord[ index ].records.at( j );
            }

            //removed as i test with animtime
            if( record.needs_extrapolation )
                continue;



            if( record.bLowerBodyYawUpdated )
                lby_records.emplace_back( record );
            else if( j != g_BacktrackHelper->PlayerRecord[ index ].records.size() - 1 )
                trash_records.emplace_back( record );

        }
        /*add records to the list, before we reverse the lby record for hardcore taps*/

        /*reverse lby vectors to tap them at their lby from old to new*/
        //std::reverse(lby_records.begin(), lby_records.end());

        /*check for shit to aim at lby, yay*/

        //check if we need to extrapolate, and just fucking do it, as nothing gets extrapoalted otherwise

        bool was_latest_checked_in_lby = false;

        for( int i = 0; i < lby_records.size(); i++ ) {
            tick_record record = lby_records.at( i );//total_valid_records.at(j);

            if( record.needs_extrapolation )
                continue;

            simtime = record.m_flSimulationTime;
            origin = record.m_vecOrigin;


            bool is_latest_record = record.m_flSimulationTime == latest_record.m_flSimulationTime;

            if( is_latest_record )
                was_latest_checked_in_lby = true;

            int count = 0; //fix that only center gets autowalled

            for( auto HitBox : MainHitbox( pTarget, record.boneMatrix ) ) {
                int hitgroup = -1;

                //fix that only latest record gets multipointed OR use the center of the hitbox to do this when using backtrack records
                if( is_latest_record || count == 0 ) {

                    if( g_pEngineTrace->IsVisible( G::LocalPlayer, vEyePos, HitBox, pTarget, hitgroup ) ) {
                        float modified_damage = wpn_data->iDamage * (float)pow( wpn_data->flRangeModifier, wpn_data->flRange * 0.002 );

                        ScaleDamage( hitgroup, pTarget, wpn_data->flArmorRatio, modified_damage );

                        if( HitBox != Vector( 0, 0, 0 ) && modified_damage >= minimum_damage )
                            return HitBox;
                    }
                }

                if( is_latest_record && count == 0 ) //neded to comment it, as i can't backtrack autowall atm
                {
                    if( RagebotElements.Ragebot_Autowall->Checked && g_Autowall->PenetrateWall( pTarget, HitBox ) ) {
                        if( HitBox != Vector( 0, 0, 0 ) )
                            return HitBox;
                    }
                }
                count++;
            }
        }

        //only do autowall & hitscan for the latest lagrecord, but do that before we use other records that ain't good
        if( !was_latest_checked_in_lby && !latest_record.needs_extrapolation ) {
            int count = 0; //fix that only center gets autowalled

            was_latest_checked_in_lby = true;

            simtime = latest_record.m_flSimulationTime;
            origin = latest_record.m_vecOrigin;

            for( auto HitBox : MainHitbox( pTarget, latest_record.boneMatrix ) ) {
                int hitgroup = -1;

                //fix that only latest record gets multipointed OR use the center of the hitbox to do this when using backtrack records

                if( g_pEngineTrace->IsVisible( G::LocalPlayer, vEyePos, HitBox, pTarget, hitgroup ) ) {
                    float modified_damage = wpn_data->iDamage * (float)pow( wpn_data->flRangeModifier, wpn_data->flRange * 0.002 );

                    ScaleDamage( hitgroup, pTarget, wpn_data->flArmorRatio, modified_damage );

                    if( HitBox != Vector( 0, 0, 0 ) && modified_damage >= minimum_damage )
                        return HitBox;
                }


                if( count == 0 ) //neded to comment it, as i can't backtrack autowall atm
                {
                    if( RagebotElements.Ragebot_Autowall->Checked && g_Autowall->PenetrateWall( pTarget, HitBox ) ) {
                        if( HitBox != Vector( 0, 0, 0 ) )
                            return HitBox;
                    }
                }
                count++;
            }
        }
        //hitscan and autowall only for latest record before trashrecords
        if( !latest_record.needs_extrapolation ) {
            Vector vPoint;
            simtime = latest_record.m_flSimulationTime;
            origin = latest_record.m_vecOrigin;
            for( auto HitboxID : GetHitboxesToScan( pTarget ) ) {
                std::vector<Vector> Points = GameUtils::GetMultiplePointsForHitbox( pTarget, HitboxID, latest_record.boneMatrix );
                for( int k = 0; k < Points.size(); k++ ) {

                    vPoint = Points.at( k );
                    float damage = 0.f;

                    int hitgroup = -1;
                    if( g_pEngineTrace->IsVisible( G::LocalPlayer, vEyePos, vPoint, pTarget, hitgroup ) ) {
                        float modified_damage = wpn_data->iDamage * (float)pow( wpn_data->flRangeModifier, wpn_data->flRange * 0.002 );

                        ScaleDamage( hitgroup, pTarget, wpn_data->flArmorRatio, modified_damage );



                        if( vPoint != Vector( 0, 0, 0 ) && modified_damage >= minimum_damage )
                            return vPoint;
                    }

                    if( k == 0 ) //fixes that only the center gets autowall
                    {
                        if( RagebotElements.Ragebot_Autowall->Checked && RagebotElements.Ragebot_Autowall_Hitscan->Checked && g_Autowall->PenetrateWall( pTarget, vPoint ) ) {
                            if( vPoint != Vector( 0, 0, 0 ) )
                                return vPoint;
                        }
                    }
                }
            }
        }


        //check for other records
        for( int j = 0; j < trash_records.size(); j++ ) {
            tick_record record = trash_records.at( j );//total_valid_records.at(j);

            if( record.needs_extrapolation )
                continue;

            //*(Vector*)((DWORD)pTarget + 0x110) = record.m_vecVelocity;


            simtime = record.m_flSimulationTime;
            origin = record.m_vecOrigin;

            int count = 0; //fix that only center gets autowalled

                           //don't aim at lby records, as we tested for them before

            for( auto HitBox : MainHitbox( pTarget, record.boneMatrix ) ) {
                int hitgroup = -1;

                //fix that only latest record gets multipointed OR use the center of the hitbox to do this when using backtrack records
                if( count == 0 ) {
                    if( g_pEngineTrace->IsVisible( G::LocalPlayer, vEyePos, HitBox, pTarget, hitgroup ) ) {
                        float modified_damage = wpn_data->iDamage * (float)pow( wpn_data->flRangeModifier, wpn_data->flRange * 0.002 );

                        ScaleDamage( hitgroup, pTarget, wpn_data->flArmorRatio, modified_damage );

                        if( HitBox != Vector( 0, 0, 0 ) && modified_damage >= minimum_damage )
                            return HitBox;
                    }
                }
                count++;
            }
        }

        if( latest_record.needs_extrapolation ) {
            Vector position = latest_record.m_vecOrigin;
            float old_simtime = latest_record.m_flSimulationTime;;
            Vector extr_position = position;
            simtime = latest_record.m_flSimulationTime;
            //retunrs correct simtime without lerptime
            g_BacktrackHelper->ExtrapolatePosition( pTarget, extr_position, simtime, latest_record.m_vecVelocity );

            Msg( "Extrapolating... OldVec(%f, %f, %f) - NewVec(%f, %f, %f)\n", position.x, position.y, position.z, extr_position.x, extr_position.y, extr_position.z );

            int count = 0;

            for( auto HitBox : MainHitbox( pTarget, /*BoneMatrix*/latest_record.boneMatrix ) ) {
                int hitgroup = -1;



                HitBox -= position;
                HitBox += extr_position;


                if( g_pEngineTrace->IsVisible( G::LocalPlayer, vEyePos, HitBox, pTarget, hitgroup ) ) {
                    //float modified_damage = wpn_data->iDamage * (float)pow(wpn_data->flRangeModifier, wpn_data->flRange * 0.002);

                    //ScaleDamage(hitgroup, pTarget, wpn_data->flArmorRatio, modified_damage);

                    if( HitBox != Vector( 0, 0, 0 )/* && modified_damage >= minimum_damage*/ )
                        return HitBox;
                }


                //if (count == 0/*fix that only center gets autowalled*/) //neded to comment it, as i can't backtrack autowall atm
                //{
                //if (RagebotElements.Ragebot_Autowall->Checked && g_Autowall->PenetrateWall(pTarget, HitBox))
                //{
                //	if (HitBox != Vector(0, 0, 0))
                //			return HitBox;
                //	}
                //}
                count++;
            }
        }
    }
    else {
        int* array = reinterpret_cast<int*>( offys.dwOcclusionArray );
        *(int*)( (uintptr_t)pTarget + offys.nWriteableBones ) = 0;
        *(int*)( (uintptr_t)pTarget + offys.bDidCheckForOcclusion ) = array[ 1 ];



        pTarget->SetupBones( BoneMatrix, 128, 0x100, g_pGlobals->curtime );
        simtime = pTarget->GetSimulationTime();

        origin = pTarget->GetOrigin();

        for( auto HitBox : MainHitbox( pTarget, BoneMatrix ) ) {
            if( g_pEngineTrace->IsVisible( G::LocalPlayer, vEyePos, HitBox, pTarget ) ) {
                if( HitBox != Vector( 0, 0, 0 ) )
                    return HitBox;
            }
            if( RagebotElements.Ragebot_Autowall->Checked && g_Autowall->PenetrateWall( pTarget, HitBox ) ) {
                if( HitBox != Vector( 0, 0, 0 ) )
                    return HitBox;
            }
        }
        for( auto HitboxID : GetHitboxesToScan( pTarget ) ) {

            Vector vPoint;

            std::vector<Vector> Points = GameUtils::GetMultiplePointsForHitbox( pTarget, HitboxID, BoneMatrix );
            for( int k = 0; k < Points.size(); k++ ) {

                vPoint = Points.at( k );
                float damage = 0.f;

                int hitgroup = -1;
                if( g_pEngineTrace->IsVisible( G::LocalPlayer, vEyePos, vPoint, pTarget, hitgroup ) ) {
                    //float curdamage = pLocal->GetWeapon()->GetCSWpnData()->m_iDamage;
                    //ScaleDamage(hitgroup, pTarget, pLocal->GetWeapon()->GetCSWpnData()->m_flArmorRatio, curdamage);
                    if( vPoint != Vector( 0, 0, 0 )/* && curdamage >= RagebotElements.Ragebot_MinDamage->value*/ )
                        return vPoint;
                }

                //if (HitboxID == (int)CSGOHitboxID::Head || HitboxID == (int)CSGOHitboxID::Stomach || HitboxID == (int)CSGOHitboxID::Chest || HitboxID == (int)CSGOHitboxID::UpperChest || HitboxID == (int)CSGOHitboxID::LowerChest)
                if( RagebotElements.Ragebot_Autowall->Checked && RagebotElements.Ragebot_Autowall_Hitscan->Checked && g_Autowall->PenetrateWall( pTarget, vPoint ) ) {
                    if( vPoint != Vector( 0, 0, 0 ) )
                        return vPoint;
                }
            }
        }
    }
    return Vector( 0, 0, 0 );
}


CAimbot* g_Aimbot = new CAimbot;
void CAimbot::DropTarget()
{
    target_index = -1;
    best_distance = 99999.f;
    aimbotted_in_current_tick = false;
    fired_in_that_tick = false;
    current_aim_position = Vector();
    pTarget = nullptr;
}
ConVar* weapon_accuracy_nospread;
void CAimbot::Run()
{

    if( !RagebotElements.Ragebot_Active->Checked )
        return;

    QAngle view; g_pEngine->GetViewAngles( view );
    CBaseCombatWeapon* pWeapon = G::LocalPlayer->GetWeapon();
    if( !pWeapon || pWeapon->Clip1() == 0 || pWeapon->IsMiscWeapon() || !GameUtils::IsAbleToShoot() )
        return;

    for( int i = 1; i < 64; ++i ) {
        if( i == g_pEngine->GetLocalPlayer() )
            continue;

        CBaseEntity* target = g_pEntitylist->GetClientEntity( i );
        if( !target || target->IsDormant() || target->GetHealth() < 1 || target->GunGameImmunity() )
            continue;

        if( target->GetTeamNum() == G::LocalPlayer->GetTeamNum() && !RagebotElements.Ragebot_AtFriendly->Checked )
            continue;

        float fov = GameUtils::GetFoV( view, G::LocalPlayer->GetEyePosition(), target->GetEyePosition(), false );
        if( fov > RagebotElements.Ragebot_FOV->value )
            continue;


        float simtime = 0;
        Vector minus_origin = Vector( 0, 0, 0 );

        Vector aim_position = RunAimScan( target, simtime, minus_origin );

        if( aim_position == Vector( 0, 0, 0 ) )
            continue;

        float selection_value = 0;
        switch( RagebotElements.Ragebot_SelectBy->SelectedIndex ) {
        case 0:
            selection_value = fov;
            break;
        case 2:
            selection_value = Math::RandomFloat( 0, 100 );
            break;
        case 3:
            selection_value = target->GetVelocity().Length();
            break;
        case 4:
            selection_value = target->GetHealth();
            break;

        default:
            break;
        }

        if( best_distance >= selection_value && aim_position != Vector( 0, 0, 0 ) ) {
            best_distance = selection_value;


            target_index = i;
            current_aim_position = aim_position;
            pTarget = target;
            current_aim_simulationtime = simtime;
            current_aim_player_origin = minus_origin;
        }
    }
    if( target_index != -1 && current_aim_position != Vector( 0, 0, 0 ) && pTarget ) {
        aimbotted_in_current_tick = true;
        QAngle aim = GameUtils::CalculateAngle( G::LocalPlayer->GetEyePosition(), current_aim_position );
        aim.y = Math::NormalizeYaw( aim.y );


        G::UserCmd->viewangles = aim;

        if( RagebotElements.Ragebot_Type->SelectedIndex == 0 )
            g_pEngine->SetViewAngles( G::UserCmd->viewangles );





        if( RagebotElements.Ragebot_AutoScope->Checked && !G::LocalPlayer->IsScoped() && pWeapon->IsScopeable() )
            G::UserCmd->buttons |= IN_ATTACK2;
        else if( RagebotElements.Ragebot_AutoShoot->Checked ) {
            auto HitChance = []( Vector Point, Vector minus_org, CBaseEntity* ent, float chance ) -> bool
            {
                if( !weapon_accuracy_nospread )
                    weapon_accuracy_nospread = g_pCvar->FindVar( XorStr( "weapon_accuracy_nospread" ) );

                if( weapon_accuracy_nospread->GetInt() == 1 )
                    return true;
                auto pLocal = G::LocalPlayer;





                if( pLocal->GetVelocity().Length() > 0.1 ) {
                    auto is_crouching = pLocal->GetFlags() & FL_DUCKING;

                    float SpreadCone = G::MainWeapon->GetAccuracyPenalty() * 256.0f / M_PI + ( is_crouching ? G::WeaponData->flInaccuracyCrouch : G::WeaponData->flInaccuracyMove ) * pLocal->GetVelocity().Length() / 3000.0f;



                    float a = ( Point - pLocal->GetEyePosition() ).Length();
                    float b = sqrt( tan( SpreadCone * M_PI / 180.0f ) * a );
                    if( 2.2f > b )
                        return true;

                    return ( ( 2.2f / fmax( b, 2.2f ) ) * 100.0f ) > chance / 1.5;
                }
                else {
                    float spread = G::MainWeapon->GetAccuracyPenalty();
                    if( spread <= ( ( ( 101 - chance ) / 20 ) / ( 100.f ) ) )
                        return true;
                    else
                        return false;
                }

                /*int iHits = 0;
                int iHitsNeed = static_cast<int>(256.f * (static_cast<float>(chance) / 100.f));

                Vector aimpoint = Point - minus_org + ent->GetOrigin();
                QAngle angle = GameUtils::CalculateAngle(aimpoint, G::LocalPlayer->GetEyePosition());

                Vector fwd, right, up;
                Math::AngleVectors(angle + (G::LocalPlayer->GetPunchAngle()* 2.f), &fwd, &right, &up);

                G::MainWeapon->UpdateAccuracyPenalty();
                auto weapon_spread = G::MainWeapon->GetSpread();
                auto weapon_cone = G::MainWeapon->GetInaccuracy();

                Vector eye_pos = G::LocalPlayer->GetEyePosition();

                bool bHitchance = false;

                trace_t tr;
                Ray_t ray;
                CTraceCBaseEntity filter;
                filter.pHit = ent;

                for (int i = 0; i < 256; i++)
                {
                RandomSeed(i + 1);

                float rand_a = RandomFloat(0.f, 1.f);
                float rand_b = RandomFloat(0.f, 2.f * PI);

                float rand_c = RandomFloat(0.f, 1.f);
                float rand_d = RandomFloat(0.f, 2.f * PI);

                float spread = rand_a * weapon_spread;
                float cone = rand_c * weapon_cone;

                Vector vec_spread((cos(rand_b) * spread) + (cos(rand_d) * cone), (sin(rand_b) * spread) + (sin(rand_d) * cone), 0), dir;

                dir.x = fwd.x + (right.x * vec_spread.x) + (up.x * vec_spread.y);
                dir.y = fwd.y + (right.y * vec_spread.x) + (up.y * vec_spread.y);
                dir.z = fwd.z + (right.z * vec_spread.x) + (up.z * vec_spread.y);

                dir.NormalizeInPlace();

                QAngle spreaded_view;
                Math::VectorAngles(dir, spreaded_view);
                spreaded_view.NormalizeInPlace();

                Vector end;
                Math::AngleVectors(angle - (spreaded_view - angle), &end);

                ray.Init(eye_pos, eye_pos + (end*G::WeaponData->flRange));

                g_pEngineTrace->TraceRay_NEW(ray, MASK_SHOT, &filter, &tr);

                if (tr.m_pEnt == ent)
                iHits++;

                if (static_cast<int>((static_cast<float>(iHits) / 256.f) * 100.f) >= chance) {
                bHitchance = true;
                break;
                }

                if ((255 - i + iHits) < iHitsNeed)//can we still win?
                break;
                }
                return bHitchance;*/
            };


            if( !RagebotElements.Ragebot_Hitchance->Checked || RagebotElements.Ragebot_Hitchance_Slider->value == 0 || HitChance( current_aim_position, current_aim_player_origin, pTarget, RagebotElements.Ragebot_Hitchance_Slider->value ) ) {
                if( pWeapon->WeaponID() == REVOLVER && RagebotElements.Ragebot_SecondaryRevolver->Checked )
                    G::UserCmd->buttons |= IN_ATTACK2;
                else
                    G::UserCmd->buttons |= IN_ATTACK;

                this->fired_in_that_tick = true;
            }
        }

        if( RagebotElements.Ragebot_AutoStop->Checked ) {
            //pCmd->buttons = NULL;
            //if (G::LocalPlayer->GetVelocity().Length2DSqr() <= 90)
            //{
            G::UserCmd->sidemove = 0.00f;
            G::UserCmd->forwardmove = 0.00f;
            /*}
            else
            {
            G::UserCmd->sidemove = -(G::UserCmd->sidemove);
            G::UserCmd->forwardmove = -(G::UserCmd->forwardmove);
            }*/

        }

        if( RagebotElements.Ragebot_Corrections_PositionAdjustment->Checked ) {


            G::UserCmd->tick_count = TIME_TO_TICKS( current_aim_simulationtime ) + TIME_TO_TICKS( g_BacktrackHelper->GetLerpTime() );// for backtracking

        }


    }
}

void CAimbot::CompensateInaccuracies()
{
    if( this->fired_in_that_tick ) {


        if( RagebotElements.Ragebot_NoSpread->Checked )
            G::UserCmd->viewangles = g_NoSpread->SpreadFactor( G::UserCmd->random_seed );

        if( RagebotElements.Ragebot_NoRecoil->Checked ) {
            G::MainWeapon->UpdateAccuracyPenalty();
            QAngle punch = G::LocalPlayer->GetPunchAngle();
            float recoil_value = 2;
            if( g_pCvar->FindVar( XorStr( "weapon_recoil_scale" ) ) )
                recoil_value = g_pCvar->FindVar( XorStr( "weapon_recoil_scale" ) )->GetValue();
            G::UserCmd->viewangles -= punch*recoil_value;
        }
    }
}
void CAimbot::AutoRevolver()
{
    if( RagebotElements.Ragebot_AutoShoot->Checked && G::MainWeapon->WeaponID() == ItemDefinitionIndex::REVOLVER ) {
        G::UserCmd->buttons |= IN_ATTACK;
        float flPostponeFireReady = G::MainWeapon->GetPostponeFireReadyTime();
        if( flPostponeFireReady > 0 && flPostponeFireReady < g_pGlobals->curtime ) {
            G::UserCmd->buttons &= ~IN_ATTACK;
        }
    }
}