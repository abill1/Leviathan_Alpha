// Copyright 2021 Anthony Bill. All rights reserved.


#include "PlayerCharacter.h"

#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Weapon.h"
#include "Gun.h"
#include "PlayerHUD.h"

#define MY_LOG(x) UE_LOG(LogTemp, Warning, x);

APlayerCharacter::APlayerCharacter()
{
	// ----- Load Animation Data: Currently this does not do anything other than allow me to get the duration of clip. Everything else is done in blueprints
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage(TEXT("AnimMontage'/Game/ExternalAssets/ParagonLtBelica/Characters/Heroes/Belica/Animations/LevelStart_Montage.LevelStart_Montage'"));
	if (Montage.Succeeded())
		this->BeginPlayMontage = Montage.Object;

	this->InBeginPlay = true;
	//this->HUD = CreateDefaultSubobject<APlayerHUD>(TEXT("HUD"));
	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	check(PlayerController);
	check(PlayerController->PlayerCameraManager);
	PlayerController->PlayerCameraManager->ViewPitchMin = this->MaxPitch;
	PlayerController->PlayerCameraManager->ViewPitchMax = this->MinPitch;

	this->Primary = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	this->Equipped = this->Primary;
	this->pMesh->HideBoneByName(TEXT("Weapon"), EPhysBodyOp::PBO_None);
	this->Equipped->AttachToComponent(this->pMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("weaponSocket"));
	this->Equipped->SetOwner(this);

	this->IntroDuration = this->PlayAnimMontage(this->BeginPlayMontage, 1.0f, FName("LevelStart"));						// Returns the length of the montage and 0.0f upon failure
	((AGun*)this->Equipped)->Reload();
	
	// Reload the weapon at the start of play


}

/*
* A better method would be to have different states for the player character like
* Begin state -> Play State <-> Dead State
* Then we can remove the intro montage code from Tick in tha play state and reduce the 
* computation cost every tick.
* 
*/
void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (this->IntroDuration > 0.0f)
	{
		this->IntroDuration -= DeltaSeconds;
		//MY_LOG(TEXT("Montage Playing."));
	}
	else
	{
		this->InBeginPlay = false;
		
	}

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);																	// An assert to ensure the Input Componet exists

	// ----- Set Aiming controls
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &APlayerCharacter::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ABaseCharacter::ZoomOut);

	// ----- Enable Rotation around Character
	PlayerInputComponent->BindAction(TEXT("RotateCamera"), EInputEvent::IE_Pressed, this, &ABaseCharacter::EnableRotation);
	PlayerInputComponent->BindAction(TEXT("RotateCamera"), EInputEvent::IE_Released, this, &ABaseCharacter::DisableRotation);

	// ----- Set key for Jump Action
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);

	// ----- Set Attack Action
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Attack);

	// ----- Set Movement key bindings 
	PlayerInputComponent->BindAxis(TEXT("MoveFwd_Bwd"), this, &APlayerCharacter::MoveFwd_Bwd);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft_Right"), this, &APlayerCharacter::MoveLeft_Right);

	//----- Set Camera Movement bindings
	PlayerInputComponent->BindAxis(TEXT("LookLeft_Right"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp_Down"), this, &APlayerCharacter::LookUp_Down);

}

/*It would be nice if the AController class had a method to get a Rotator with just the
* Yaw rotation, or even a method to just get the Forward or right direction without having
* to create all these temporary variables.
*
*Note : I tried editing the class in the source code, but it created all kinds of glitches
* for the controls.
*
* ------
* Many of the Tutorials, and I think the sample code, use the following code to get the
* FRotator used to find the direction of the Character :
* const FRotator Rotation = this->Controller->GetControlRotation();					// Get the rotation
* const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
*
*It is unnecessary to create anoter rotator objec and it would be better practice to
* perform the math operations I use below. While one temporary object creation is not
*taxing on the cpu, a million do add up.This method still has more temporary variables
* than are ideal, but it is one step in the right direction.
* 
* Another possible solution is for the player to own a YawRotator object, then just get the 
* Yaw from the controller and overwrite that float value. Now we removed having to create 24
* bytes for every Move. 
*
* Preferably, we would just get the forward and right vector direction from the controller.
* The controller class has GetActorForward() but this is inaccessible and does not look like 
* it would do what I expected from the name. 
* -----
* 
* Could just write this method as: 
* if(!InBeginPlay)
*	Super::MoveFwd_Bwd
* 
* Some small performance considerations.
*
*/
void APlayerCharacter::MoveFwd_Bwd(const float _axisValue)
{
	check(this->Controller);
	if (!this->InBeginPlay && (_axisValue != 0.0f) && !EnableRotateCamera)
	{
		FRotator Rotation = this->Controller->GetControlRotation();
		Rotation.Pitch -= Rotation.Pitch;
		Rotation.Roll -= Rotation.Roll;
		
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);			// Create forward vector
		this->AddMovementInput(Direction * _axisValue);										// Update movement with the forward vector
		
	}

}

void APlayerCharacter::MoveLeft_Right(const float _axisValue)
{
	check(this->Controller);
	if (!this->InBeginPlay && (_axisValue != 0.0f) && !EnableRotateCamera)
	{
		FRotator Rotation = this->Controller->GetControlRotation();
		Rotation.Pitch -= Rotation.Pitch;
		Rotation.Roll -= Rotation.Roll;

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);			// Create right vector
		this->AddMovementInput(Direction * _axisValue);										// Update movement with the forward vector

	}

}

void APlayerCharacter::LookUp_Down(const float _axisValue)
{
	if (!this->InBeginPlay && _axisValue != 0.0f)
	{
		APlayerController* pCont = CastChecked<APlayerController>(GetController());
		if (pCont != nullptr && !pCont->IsLookInputIgnored())
		{
			float change = _axisValue * pCont->InputPitchScale;
			pCont->RotationInput.Pitch += change;
		}
		else
		{
			pCont->RotationInput.Pitch += 0.0f;

		}

	}

}

void APlayerCharacter::Jump()
{
	if (!this->InBeginPlay && !this->ZoomedIn && !this->EnableRotateCamera)
	{
		Super::Jump();
		this->IsInAir = true;

	}

}

/*
* The zoom in method will now position the camera so that the character appears
* in the bottom left corner of the screen. When the player rotates while zoomed
* in, the character remains in the corner and the camera track the character.
* This effect is accomplished by changing the socket offset of the spring arm.
*
*/
void APlayerCharacter::ZoomIn()
{
	USpringArmComponent* pArm = GetCameraBoom();
	check(pArm);
	if (!this->InBeginPlay && !this->EnableRotateCamera && !this->IsInAir)								// Cannot zoom in if currently rotating around character
	{
		pArm->TargetArmLength = SpringArmAimLength;
		pArm->SocketOffset = this->AimingOffset;													// Change the socket offset to give a zoom in effect
		this->ZoomedIn = true;

		check(this->pMesh);
		this->pMesh->AddLocalRotation(this->MeshRotatorForAim, true);								// Need to rotate the mesh when zoomed in so that the barrel lines up with crosshair

		MY_LOG(TEXT("Zoomed In."));

		check(this->pCharacterMovement);
		this->pCharacterMovement->MaxWalkSpeed = this->WalkSpeedWhileAiming;

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Zoomed In."));
	}

}

void APlayerCharacter::Attack()
{
	this->Equipped->Attack();
	MY_LOG(TEXT("Player Attack"));

}
