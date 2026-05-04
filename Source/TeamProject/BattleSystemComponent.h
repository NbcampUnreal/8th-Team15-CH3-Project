//BattleSystemComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BattleSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMPROJECT_API UBattleSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattleSystemComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	float HP;
	UPROPERTY(EditDefaultsOnly)
	float MaxHP;
	UPROPERTY(EditDefaultsOnly)
	float ATK;
	UPROPERTY()
	bool isDead;

public:
	UFUNCTION(BlueprintCallable)
	void DamageCalculation(float attackerATK);
	UFUNCTION(BlueprintCallable)
	void Heal();
};
