#include "Esp.h"
#include "CDrawManager.h"

void Esp::PaintTraverse()
{
	Vector world, screen;
	for(int i = 1; i < gInts.EntList->GetHighestEntityIndex(); i++)
	{
		CBaseEntity *baseEntity = GetBaseEntity(i);

		if(baseEntity == NULL)
			continue;

		if(baseEntity->IsDormant())
			continue;

		if(baseEntity->IsAlive() == false)
			continue;
			
		Vector world, screen;
		Vector min, max, origin, localOrigin;

		baseEntity->GetRenderBounds(min, max);

		origin = baseEntity->GetAbsOrigin();

		baseEntity->GetWorldSpaceCenter(world);

		if(!gDrawManager.WorldToScreen(world, screen))
			continue;
		
		float yPos = screen.y+16;
		
		if(baseEntity->GetClientClass()->iClassID == classId::CTFPlayer)
		{
			// this is a player
			
			player_info_t info;
			gInts.Engine->GetPlayerInfo(i, &info);

			gDrawManager.DrawString(screen.x, yPos, gDrawManager.dwGetTeamColor(baseEntity->GetTeam()), "%s", info.name);
			yPos+=gDrawManager.GetESPHeight();
			gDrawManager.DrawString(screen.x, yPos, gDrawManager.dwGetTeamColor(baseEntity->GetTeam()), "(%s)", baseEntity->GetClassName());
			yPos+=gDrawManager.GetESPHeight();
			gDrawManager.DrawString(screen.x, screen.y, gDrawManager.dwGetTeamColor(baseEntity->GetTeam()), "%i", baseEntity->GetHealth());
			yPos+=gDrawManager.GetESPHeight();
		} else {
			ClientClass* bec = baseEntity->GetClientClass();
			if((bec->iClassID== classId::CObjectDispenser || bec->iClassID== classId::CObjectSapper || bec->iClassID== classId::CObjectSentrygun || bec->iClassID== classId::CObjectTeleporter)) {
				gDrawManager.DrawString(screen.x, screen.y, COLORCODE(0,0,0,255), "%s", bec->chName);
			}
		}
	}
}
