// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleRadius = 42.0f;														// These should be dependent on the size of the mesh of the character
	CapsuleHalfHeight = 96.0f;
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);	// Initialize the collision capsule around the character pawn

	// ----- Set Character Characteristics
	UCharacterMovementComponent* pMovement = GetCharacterMovement();
	pMovement->bOrientRotationToMovement = true;									// This will make the character move in the direction of the rotation input
	pMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);							// The Rate at which the character will rotate in the direction of the input
	pMovement->JumpZVelocity = 600.0f;												// The initial velocity of the character's jump
	pMovement->AirControl = 0.2f;													// When falling, amount of lateral movement control available to the character. How much the character can move around the x,y plane while in the air.
	pMovement->MaxWalkSpeed = 600.0f;
	WalkingSpeed = 600.0f;
	WalkAndAimSpeed = 300.0f;
	ZoomRate = 20.0f;
	ZoomedIn = false;

	// ----- Camera Setup
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	this->Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	
	// ----- Setup Spring Arm Properties
	if (SpringArm != nullptr)
	{
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->SetWorldLocation(RootComponent->GetComponentLocation());
		SpringArm->TargetArmLength = 300.0f;										// The distance at which to place the spring arm behind the character
		SpringArm->bUsePawnControlRotation = true;									// Rotate the spring arm given the controller input
		SpringArmFollowLength = 300.0f;
		SpringArmAimLength = 100.0f;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spring Arm is nullptr."));
	}

	// ----- Setup Camera Properties
	if (Camera != nullptr)
	{
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);		// Attach the camera to the spring arm
		Camera->bUsePawnControlRotation = false;									// The camera's rotation will be driven by the spring arm
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cam is nullptr."));
	}

	// ----- Rotation Properties: These will just have the camera rotate around the character and not rotate the character pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();	
	this->SpringArm = (USpringArmComponent*)GetDefaultSubobjectByName(TEXT("CameraBoom"));			// Need to reconnect the pointer
	if(this->SpringArm == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Spring Arm null."));

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);																	// An assert to ensure the Input Componet exists
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ----- Set Aiming controls
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &ABaseCharacter::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ABaseCharacter::ZoomOut);

	// ----- Set Movement key bindings 
	PlayerInputComponent->BindAxis(TEXT("MoveFwd_Bwd"), this, &ABaseCharacter::MoveFwd_Bwd);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft_Right"), this, &ABaseCharacter::MoveLeft_Right);

	//----- Set Camera Movement bindings
	PlayerInputComponent->BindAxis(TEXT("LookLeft_Right"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp_Down"), this, &APawn::AddControllerPitchInput);

}

void ABaseCharacter::MoveFwd_Bwd(const float _axisValue)
{
	if ((this->Controller != nullptr) && (_axisValue != 0.0f))
	{
		//if (!ZoomedIn)
		//{
			const FRotator Rotation = this->Controller->GetControlRotation();					// Get the rotation
			const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);		// Create forward vector
			this->AddMovementInput(Direction * _axisValue);										// Update movement with the forward vector
		
		//}
		//else
		//{
			//this->AddMovementInput(this->GetActorForwardVector() * _axisValue);

		//}

		//UE_LOG(LogTemp, Warning, TEXT("Axis Should Not equal Zeror : %f."), _axisValue);
		// this->GetActorForwardVector() => would this be faster to call; need to look to see how this method is different from above
	
	}
	
}

void ABaseCharacter::MoveLeft_Right(const float _axisValue)
{
	if ((this->Controller != nullptr) && (_axisValue != 0.0f))
	{
		//if(!ZoomedIn)
		//{
			const FRotator Rotation = this->Controller->GetControlRotation();					// Get the rotation
			const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// Create right vector
			this->AddMovementInput(Direction * _axisValue);										// Update movement with the forward vector
		
		//}
		//else
		//{
			//this->AddMovementInput(this->GetActorRightVector() * _axisValue);

		//}

		// this->GetActorForwardVector() => would this be faster to call; need to look to see how this method is different from above
	}

}

void ABaseCharacter::ZoomIn()
{
	USpringArmComponent* pArm = GetCameraBoom();
	if (pArm != nullptr)
	{
		FVector forward = GetActorForwardVector();
		UE_LOG(LogTemp, Warning, TEXT("Forward: %f, %f, %f."),forward.X, forward.Y, forward.Z);
		pArm->TargetArmLength = SpringArmAimLength;
		pArm->TargetOffset = FVector(0.0f, 60.0f, 70.0f);
		this->ZoomedIn = true;

		UE_LOG(LogTemp, Warning, TEXT("Zoomed In."));
		UCharacterMovementComponent* pMovement = GetCharacterMovement();
		if (pMovement)
		{
			pMovement->MaxWalkSpeed = WalkAndAimSpeed;

		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Zoomed In."));
	}

}

void ABaseCharacter::ZoomOut()
{
	USpringArmComponent* pArm = GetCameraBoom();
	if (pArm != nullptr)
	{
		pArm->TargetArmLength = SpringArmFollowLength;
		pArm->TargetOffset = FVector(0.0f, 0.0f, 0.0f);
		this->ZoomedIn = false;
		

		UE_LOG(LogTemp, Warning, TEXT("Zoomed Out."));
		UCharacterMovementComponent* pMovement = GetCharacterMovement();
		if (pMovement)
		{
			pMovement->MaxWalkSpeed = WalkingSpeed;

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Zoomed Out."));
	}

}

