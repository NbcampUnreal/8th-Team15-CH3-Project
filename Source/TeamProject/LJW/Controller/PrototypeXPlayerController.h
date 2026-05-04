#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PrototypeXPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class TEAMPROJECT_API APrototypeXPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	APrototypeXPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	TObjectPtr<UInputMappingContext> IMC_Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	TObjectPtr<UInputAction> IA_Look;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	TObjectPtr<UInputAction> IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	TObjectPtr<UInputAction> IA_Sprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inputs")
	TObjectPtr<UInputAction> IA_Roll;


protected:
	virtual void BeginPlay() override;

};
