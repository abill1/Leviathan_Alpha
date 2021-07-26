// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#define MY_LOG(x) UE_LOG(LogTemp, Warning, x);

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CapsuleRadius = 42.0f;															// These should be dependent on the size of the mesh of the character
	CapsuleHalfHeight = 96.0f;
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);		// Initialize the collision capsule around the character pawn

	// ----- Set Character Characteristics
	UCharacterMovementComponent* pMovement = GetCharacterMovement();
	pMovement->bOrientRotationToMovement = true;									// This will make the character move in the direction of the rotation input
	pMovement->RotationRate = FRotator(0.0f, 540.0f, 0.0f);							// The Rate at which the character will rotate in the direction of the input
	pMovement->JumpZVelocity = 600.0f;												// The initial velocity of the character's jump
	pMovement->AirControl = 0.2f;													// When falling, amount of lateral movement control available to the character. How much the character can move around the x,y plane while in the air.
	pMovement->MaxWalkSpeed = 600.0f;
	WalkingSpeed = 600.0f;
	WalkSpeedWhileAiming = 300.0f;
	ZoomRate = 20.0f;
	ZoomedIn = false;
	MaxPitch = -55.0f;
	MinPitch = 10.0f;
	EnableRotateCamera = false;

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
		MY_LOG(TEXT("Spring Arm null."));

	this->Camera = (UCameraComponent*)GetDefaultSubobjectByName(TEXT("Camera"));					// Need to reconnect the pointer
	if (this->Camera == nullptr)
		MY_LOG(TEXT("Camera is null."));

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (PlayerController->PlayerCameraManager)
		{
			PlayerController->PlayerCameraManager->ViewPitchMin = this->MaxPitch; 
			PlayerController->PlayerCameraManager->ViewPitchMax = this->MinPitch;

		}
	}

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

	// ----- Enable Rotation around Character
	PlayerInputComponent->BindAction(TEXT("RotateCamera"), EInputEvent::IE_Pressed, this, &ABaseCharacter::EnableRotation);
	PlayerInputComponent->BindAction(TEXT("RotateCamera"), EInputEvent::IE_Released, this, &ABaseCharacter::DisableRotation);

	// ----- Set key for Jump Action
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	// ----- Set Movement key bindings 
	PlayerInputComponent->BindAxis(TEXT("MoveFwd_Bwd"), this, &ABaseCharacter::MoveFwd_Bwd);
	PlayerInputComponent->BindAxis(TEXT("MoveLeft_Right"), this, &ABaseCharacter::MoveLeft_Right);

	//----- Set Camera Movement bindings
	PlayerInputComponent->BindAxis(TEXT("LookLeft_Right"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp_Down"), this, &ABaseCharacter::LookUp_Down);

}

void ABaseCharacter::MoveFwd_Bwd(const float _axisValue)
{
	if ((this->Controller != nullptr) && (_axisValue != 0.0f) && !EnableRotateCamera)
	{
		const FRotator Rotation = this->Controller->GetControlRotation();					// Get the rotation
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);		// Create forward vector
		this->AddMovementInput(Direction * _axisValue);										// Update movement with the forward vector
		this->privDebugCamAndPlayerPosition();

	}
	
}

void ABaseCharacter::MoveLeft_Right(const float _axisValue)
{
	if ((this->Controller != nullptr) && (_axisValue != 0.0f) && !EnableRotateCamera)
	{
		const FRotator Rotation = this->Controller->GetControlRotation();					// Get the rotation
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);		// Create right vector
		this->AddMovementInput(Direction * _axisValue);										// Update movement with the forward vector
		this->privDebugCamAndPlayerPosition();

	}

}

void ABaseCharacter::LookUp_Down(const float _axisValue)
{
	if (_axisValue != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Axis Val: %f."), _axisValue);
		APlayerController* pCont = CastChecked<APlayerController>(GetController());
		if (pCont != nullptr && !pCont->IsLookInputIgnored())
		{
			float change = _axisValue * pCont->InputPitchScale;
			UE_LOG(LogTemp, Warning, TEXT("PitchScale: %f."), pCont->InputPitchScale);
			UE_LOG(LogTemp, Warning, TEXT("Change: %f."), change);
			pCont->RotationInput.Pitch += change;
			UE_LOG(LogTemp, Warning, TEXT("New Pitch: %f."), pCont->RotationInput.Pitch);
			UE_LOG(LogTemp, Warning, TEXT("---------------"));

		}
		else
		{
			pCont->RotationInput.Pitch += 0.0f;

		}

	}
	
}

/*
* The zoom in method will now position the camera so that the character appears 
* in the bottom left corner of the screen. When the player rotates while zoomed 
* in, the character remains in the corner and the camera track the character. 
* This effect is accomplished by changing the socket offset of the spring arm. 
* 
*/
void ABaseCharacter::ZoomIn()
{
	USpringArmComponent* pArm = GetCameraBoom();
	if (pArm != nullptr && !this->EnableRotateCamera)										// Cannot zoom in if currently rotating around character
	{

		FVector forward = GetActorForwardVector();
		pArm->TargetArmLength = SpringArmAimLength;
		pArm->SocketOffset = FVector(0.0f, 60.0f, 70.0f);									// Change the socket offset to give a zoom in effect
		this->ZoomedIn = true;

		UE_LOG(LogTemp, Warning, TEXT("Forward: %f, %f, %f."), forward.X, forward.Y, forward.Z);
		MY_LOG(TEXT("Zoomed In."));

		UCharacterMovementComponent* pMovement = GetCharacterMovement();
		if (pMovement)
		{
			pMovement->MaxWalkSpeed = this->WalkSpeedWhileAiming;
			pMovement->bOrientRotationToMovement = false;									// Changing this to false while zoomed prevents the character model from doing a small rotation in the direction of the input

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
		pArm->TargetArmLength = this->SpringArmFollowLength;
		pArm->SocketOffset = FVector(0.0f, 0.0f, 0.0f);
		this->ZoomedIn = false;

		UE_LOG(LogTemp, Warning, TEXT("Zoomed Out."));
		UCharacterMovementComponent* pMovement = GetCharacterMovement();
		if (pMovement)
		{
			pMovement->MaxWalkSpeed = this->WalkingSpeed;
			pMovement->bOrientRotationToMovement = true;									// Reset to allow the model to rotate with movement input

		}
	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed Zoomed Out."));
	}

}

void ABaseCharacter::EnableRotation()
{
	if (!this->ZoomedIn)																	// Cannot rotate around character if zoomed in	
	{
		bUseControllerRotationYaw = false;													// This boolean enables the camera to rotate around the character
		this->EnableRotateCamera = true;
		MY_LOG(TEXT("Start Rotation."));
		this->privDebugCamAndPlayerPosition();

	}
}

/*
* This function still needs some work. Ideally, the camera will 
* rotate back around the character so that its forward vector is
* the same as the character. Currently, the character will rotate
* so that its forward vector matches the camera. 
* 
*/
void ABaseCharacter::DisableRotation()
{
	bUseControllerRotationYaw = true;													// Prevents the camera from rotating around the character
	this->EnableRotateCamera = false;
	this->privDebugCamAndPlayerPosition();
	MY_LOG(TEXT("Stop Rotation."));

}

void ABaseCharacter::privDebugCamAndPlayerPosition() const
{
	FVector characterLocation = this->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Charcter Pos: X: %f; Y: %f; Z:%f"), characterLocation.X, characterLocation.Y, characterLocation.Z);

	USpringArmComponent* pArm = GetCameraBoom();
	if (pArm)
	{
		FVector camPosition = pArm->GetComponentLocation();
		UE_LOG(LogTemp, Warning, TEXT("Cam Pos: X: %f; Y: %f; Z:%f"), camPosition.X, camPosition.Y, camPosition.Z);

	}

}


