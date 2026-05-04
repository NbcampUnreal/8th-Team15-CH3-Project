// BattleSystemComponent.cpp


#include "BattleSystemComponent.h"

UBattleSystemComponent::UBattleSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UBattleSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBattleSystemComponent::DamageCalculation(float attackerATK)
{
	if (!isDead)
	{
		HP = HP - attackerATK;

		if (HP <= 0)
		{
			HP = 0;
			isDead = true; // TODO: 사망 델리게이트 Broadcast, GameMode에게 전달
		}
	}
}

void UBattleSystemComponent::Heal()
{
	HP = FMath::Min(HP + ATK * 0.45f, MaxHP);
}





