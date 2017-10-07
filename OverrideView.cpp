#include "hooks.h"
#include "global.h"
#include "Menu.h"
void __fastcall Hooks::OverrideView(void* _this, void* _edx, CViewSetup* setup)
{
		if (G::LocalPlayer && !G::LocalPlayer->IsScoped() && G::LocalPlayer->GetHealth() > 0)
		{
			float fov = MiscElements.Misc_General_FOV->value;

			if (fov < 90)
				fov = 90.f;

			if (fov != setup->fov)
				setup->fov = fov;


		}


		clientmodeVMT->GetOriginalMethod<OverrideViewFn>(18)(_this, setup);
	
}