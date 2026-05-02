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
	
	bUseControllerRotationYaw = false; // = 컨트롤러의 회전을 캐릭터가 상속을 하겠느냐
	bUseControllerRotationPitch = false; // = False > 따로따로 회전 적용
	bUseControllerRotationRoll = false;

	// 2. 이동 방향으로 캐릭터가 자동으로 회전하도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true; // 핵심: 입력 방향으로 몸을 돌림
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // 회전 속도
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	// 3. 스프링암 설정 (카메라만 컨트롤러 회전을 따름)
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;

	//GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	
	//===========================================================================================
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = true;  // 마우스 좌우에 따라 캐릭터가 회전하길 원하면 true
	//bUseControllerRotationRoll = false;

	//// 2. SpringArm이 컨트롤러의 회전(마우스 입력값)을 그대로 따르도록 설정
	//SpringArmComponent->bUsePawnControlRotation = true; // 핵심: 이게 true여야 컨트롤러 값을 가져옵니다.
	//SpringArmComponent->bInheritPitch = true;           // 컨트롤러의 Pitch 반영
	//SpringArmComponent->bInheritYaw = true;             // 컨트롤러의 Yaw 반영
	//SpringArmComponent->bInheritRoll = false;

	//SpringArmComponent->bEnableCameraRotationLag = true; // 회전 지연 활성화
	//SpringArmComponent->CameraRotationLagSpeed = 10.f;
	//============================================================================================
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
	Normal_Speed = 500.f;
	Sprint_Speed = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = Normal_Speed;

	Normal_Jump_Speed = 500.f;
	Max_Jump_Speed = 500.f;
	Min_Jump_Speed = Normal_Jump_Speed;

	GetCharacterMovement()->JumpZVelocity = Normal_Jump_Speed;
	//bIsOnAir = false;
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

			if (PlayerController->IA_Jump)
			{
				EnhancedInput->BindAction(
					PlayerController->IA_Jump,
					ETriggerEvent::Triggered,
					this,
					&APrototypeXCharacter::Jump_Start
				);

				EnhancedInput->BindAction(
					PlayerController->IA_Jump,
					ETriggerEvent::Completed,
					this,
					&APrototypeXCharacter::Jump_Stop
				);
			}

			if (PlayerController->IA_Sprint)
			{
				EnhancedInput->BindAction(
					PlayerController->IA_Sprint,
					ETriggerEvent::Triggered,
					this,
					&APrototypeXCharacter::Sprint_Start
				);

				EnhancedInput->BindAction(
					PlayerController->IA_Sprint,
					ETriggerEvent::Completed,
					this,
					&APrototypeXCharacter::Sprint_Stop
				);
			}
		}
	}
}

void APrototypeXCharacter::Move_Start(const FInputActionValue& value)
{
	if (!Controller) return;
	const FVector2D MoveAmount = value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0); // 컨트롤러의 Yaw(Z축) 추출

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// 컨트롤러의 틀어진Yaw축에서 XYZ기즈모를 얻고 X축의 기즈모를 얻음(방향벡터라 크기1)
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// 컨트롤러의 틀어진Yaw축에서 XYZ기즈모를 얻고 Y축의 기즈모를 얻음(방향벡터라 크기1)

	if (!FMath::IsNearlyZero(MoveAmount.X))
	{
		//AddMovementInput(GetActorForwardVector(), MoveAmount.X);
		AddMovementInput(ForwardDirection, MoveAmount.X);
	}
	if (!FMath::IsNearlyZero(MoveAmount.Y))
	{
		//AddMovementInput(GetActorRightVector(), MoveAmount.Y);
		AddMovementInput(RightDirection, MoveAmount.Y);
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

void APrototypeXCharacter::Jump_Start(const FInputActionValue& value)
{
	bIsOnJumpping = true;
	//FVector Velocity = GetCharacterMovement()->Velocity;
	//float Speed = Velocity.Size2D();
	//GetCharacterMovement()->JumpZVelocity = 
	//	FMath::Clamp(Normal_Jump_Speed * (Speed / Normal_Speed), Min_Jump_Speed, Max_Jump_Speed);
	Jump();
}

void APrototypeXCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bIsOnJumpping = false;

}

void APrototypeXCharacter::Jump_Stop(const FInputActionValue& value)
{
	//bIsOnAir = false;
	StopJumping();
	GetCharacterMovement()->JumpZVelocity = Normal_Jump_Speed;
}

void APrototypeXCharacter::Sprint_Start(const FInputActionValue& value)
{
	GetCharacterMovement()->MaxWalkSpeed = Sprint_Speed;
}

void APrototypeXCharacter::Sprint_Stop(const FInputActionValue& value)
{
	GetCharacterMovement()->MaxWalkSpeed = Normal_Speed;
}

