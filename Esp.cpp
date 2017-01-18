#include "Esp.h"
#include "CDrawManager.h"

void Esp::PaintTraverse()
{
	Vector world, screen;
	for(int i = 1; i < gInts.EntList->GetHighestEntityIndex(); i++)
	{
		CBaseEntity *baseEntity = GetBaseEntity(i);

		if(baseEntity == nullptr)
			continue;

		if(baseEntity->IsDormant())
			continue;

		if(baseEntity->IsAlive() == false)
			continue;

		if(baseEntity->GetClientClass()->iClassID == classId::CTFPlayer)
		{
			// this is a player
			
			baseEntity->GetWorldSpaceCenter(world);
			
			if(gDrawManager.WorldToScreen(world, screen))
			{
				player_info_t info;
				gInts.Engine->GetPlayerInfo(i, &info);

				float yPos = screen.y;
				
				gDrawManager.DrawString(screen.x, yPos, 0xFFFFFFFF, "%s", info.name);
				yPos+=gDrawManager.GetESPHeight();
				gDrawManager.DrawString(screen.x, screen.y, 0xFFFFFFFF, "%i", baseEntity->GetHealth());
				yPos+=gDrawManager.GetESPHeight();
			}
		}
	}
}
