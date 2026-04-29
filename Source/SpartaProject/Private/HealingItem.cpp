#include "HealingItem.h"
#include "SpartaCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
        {
            // 캐릭터의 체력을 회복
            PlayerCharacter->AddHealth(HealAmount);

            // %f 에러 방지를 위해 (float) 캐스팅 확인
            UE_LOG(LogTemp, Warning, TEXT("Healing Item Used: +%f HP"), (float)HealAmount);
        }

        DestroyItem();
    }
}