#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PrototypeXCharacter.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class TEAMPROJECT_API APrototypeXCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APrototypeXCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCapsuleComponent> UCapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	// Mouse ==========================
	float Inter_LookAmountX;
	float Inter_LookAmountY;
	float Inter_FinalX;
	float Inter_FinalY;

	float MouseSensibiliy;
	// ================================
	float Max_Speed;

protected:
	virtual void BeginPlay() override;
	void Move_Start(const FInputActionValue& value);
	void Move_Stop(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void Inter_Look(float DeltaTime);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
