#include "LJW/Character/PrototypeXCharacter.h"
#include "TeamProject/LJW/GameUtilHeader/GameUtil.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputComponent.h"
#include "LJW/Controller/PrototypeXPlayerController.h"
APrototypeXCharacter::APrototypeXCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//GetCapsuleComponent()->SetRelativeRotation(FRotator(0.f, 45.f, 0.f));
	FVector PivotLocation = FVector(0.f, 0.f, -50.f);
	FRotator PivotRotation = FRotator(0.f, -90.f, 0.f);
	UCapsuleComponent = GetCapsuleComponent();
	SkeletalMeshComponent = GetMesh();
	SkeletalMeshComponent->SetRelativeRotation(PivotRotation);
	SkeletalMeshComponent->SetRelativeLocation(PivotLocation);
	SpringArmComponent = GameUtil::CreateComponent<USpringArmComponent>(this);
	SpringArmComponent->TargetArmLength = 300.f;
	CameraComponent = GameUtil::CreateComponent<UCameraComponent>(this);
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	Inter_LookAmountX = 0;
	Inter_LookAmountY = 0;
	Inter_FinalX = 0;
	Inter_FinalY = 0;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;  // 마우스 좌우에 따라 캐릭터가 회전하길 원하면 true
	bUseControllerRotationRoll = false;

	// 2. SpringArm이 컨트롤러의 회전(마우스 입력값)을 그대로 따르도록 설정
	SpringArmComponent->bUsePawnControlRotation = true; // 핵심: 이게 true여야 컨트롤러 값을 가져옵니다.
	SpringArmComponent->bInheritPitch = true;           // 컨트롤러의 Pitch 반영
	SpringArmComponent->bInheritYaw = true;             // 컨트롤러의 Yaw 반영
	SpringArmComponent->bInheritRoll = false;
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationRoll = false;

	SpringArmComponent->bEnableCameraRotationLag = true; // 회전 지연 활성화
	SpringArmComponent->CameraRotationLagSpeed = 10.f;

	MouseSensibiliy = 0.5f;
	//bUseControllerRotationYaw = true;
	//SpringArmComponent->bUsePawnControlRotation = true;
	//SpringArmComponent->bInheritPitch = true; // 컨트롤러의 Pitch를 받아야 카메라가 위아래로 움직임
	//SpringArmComponent->bInheritYaw = true;   // 캐릭터가 회전할 때 카메라도 같이 회전
	//SpringArmComponent->bInheritRoll = false;
	// 
	//bUseControllerRotationYaw = true;
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationRoll = false;
	//GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->RotationRate.Yaw = 540.f;
	//bUseControllerRotationYaw = false;
	//SpringArmComponent->bInheritYaw = false;
	//SpringArmComponent->bInheritPitch = false;
	//SpringArmComponent->bInheritRoll = false;
	//SpringArmComponent->bUsePawnControlRotation = true;
	//CameraComponent->bUsePawnControlRotation = true;
	Max_Speed = 400.f;
	GetCharacterMovement()->MaxWalkSpeed = Max_Speed;

}

void APrototypeXCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpringArmComponent->bUsePawnControlRotation = true;
}

void APrototypeXCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Mouse_Interp
	//Inter_Look(DeltaTime);
	

}

void APrototypeXCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput =
		Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APrototypeXPlayerController* PlayerController =
			Cast<APrototypeXPlayerController>(GetController()))
		{
			if (PlayerController->IA_Move)
			{
				EnhancedInput->BindAction(
					PlayerController->IA_Move,
					ETriggerEvent::Triggered,
					this,
					&APrototypeXCharacter::Move_Start
				);

				EnhancedInput->BindAction(
					PlayerController->IA_Move,
					ETriggerEvent::Completed,
					this,
					&APrototypeXCharacter::Move_Stop
				);
			}

			if (PlayerController->IA_Look)
			{
				EnhancedInput->BindAction(
					PlayerController->IA_Look,
					ETriggerEvent::Triggered,
					this,
					&APrototypeXCharacter::Look
				);
			}
		}
	}
}

void APrototypeXCharacter::Move_Start(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveAmount = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveAmount.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveAmount.X * MouseSensibiliy);
	}
	if (!FMath::IsNearlyZero(MoveAmount.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveAmount.Y);
	}
}

void APrototypeXCharacter::Move_Stop(const FInputActionValue& value)
{

}

void APrototypeXCharacter::Look(const FInputActionValue& value)
{
	const FVector2D LookAmount = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(LookAmount.X))
	{
		//Inter_LookAmountX += LookAmount.X;
		AddControllerYawInput(LookAmount.X);
	}
	if (!FMath::IsNearlyZero(LookAmount.Y))
	{
		AddControllerPitchInput(-LookAmount.Y);
		//Inter_LookAmountY += LookAmount.Y;
		//Inter_LookAmountY = FMath::Clamp(Inter_LookAmountY, -80, 60);

	}
}

void APrototypeXCharacter::Inter_Look(float DeltaTime)
{
	if (Inter_FinalX != Inter_LookAmountX || Inter_FinalY != Inter_LookAmountY)
	{
		Inter_FinalX = FMath::FInterpTo(Inter_FinalX, Inter_LookAmountX, DeltaTime, 12.f);
		Inter_FinalY = FMath::FInterpTo(Inter_FinalY, Inter_LookAmountY, DeltaTime, 12.f);

		SpringArmComponent->SetRelativeRotation(FRotator(Inter_FinalY, Inter_FinalX, 0.f));
		if (FMath::IsNearlyEqual(Inter_FinalX, Inter_LookAmountX))
		{
			Inter_FinalX = Inter_LookAmountX;
		}
		if (FMath::IsNearlyEqual(Inter_FinalY, Inter_LookAmountY))
		{
			Inter_FinalY = Inter_LookAmountY;
		}
	}
}

