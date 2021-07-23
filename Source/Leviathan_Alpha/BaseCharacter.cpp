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

	// ----- Rotation Properties: These will just have the camera rotate around the character and not rotate the character pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// ----- Set Character Movement Characteristics
	UCharacterMovementComponent* pMovement = GetCharacterMovement();
	pMovement->bOrientRotationToMovement = true;									// This will make the character move in the direction of the rotation input
	pMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);							// The Rate at which the character will rotate in the direction of the input
	pMovement->JumpZVelocity = 600.0f;												// The initial velocity of the character's jump
	pMovement->AirControl = 0.2f;													// When falling, amount of lateral movement control available to the character. How much the character can move around the x,y plane while in the air.
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));

	// ----- Setup Spring Arm Properties
	if (SpringArm != nullptr)
	{
		SpringArm->SetupAttachment(RootComponent);
		SpringArm->SetWorldLocation(RootComponent->GetComponentLocation());
		SpringArm->TargetArmLength = 300.0f;										// The distance at which to place the spring arm behind the character
		SpringArm->bUsePawnControlRotation = true;									// Rotate the spring arm given the controller input
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

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();	

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
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
		const FRotator Rotation = this->Controller->GetControlRotation();					// Get the rotation
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);								
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);		// Create forward vector
		this->AddMovementInput(Direction * _axisValue);										// Update movement with the forward vector

		// this->GetActorForwardVector() => would this be faster to call; need to look to see how this method is different from above
	}
	
}

void ABaseCharacter::MoveLeft_Right(const float _axisValue)
{
	if ((this->Controller != nullptr) && (_axisValue != 0.0f))
	{
		const FRotator Rotation = this->Controller->GetControlRotation();					// Get the rotation
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);								 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// Create right vector
		this->AddMovementInput(Direction * _axisValue);										// Update movement with the forward vector

		// this->GetActorForwardVector() => would this be faster to call; need to look to see how this method is different from above
	}

}


