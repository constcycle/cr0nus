#include "sdk.h"
#include "global.h"

bool G::Aimbotting = false;
QAngle G::LastAngle = QAngle();
QAngle G::AAAngle = QAngle();
QAngle G::StrafeAngle = QAngle();
bool G::Return = true;
bool G::ForceRealAA = false;
int G::resolver_ticks[64];
bool G::SendPacket = true;
int G::ChokedPackets = 0;
bool G::InThirdperson;
CScreen G::Screen;
CBaseEntity* G::LocalPlayer = nullptr;
CBaseCombatWeapon* G::MainWeapon = nullptr;
CSWeaponInfo* G::WeaponData = nullptr;
CUserCmd*	G::UserCmd = nullptr;
CUserCmd* G::UserCmdForBacktracking = nullptr;
float G::lby_update_end_time;
QAngle		G::RealAngle;
QAngle		G::FakeAngle;