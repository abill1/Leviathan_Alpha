// Copyright 2021 Anthony Bill. All rights reserved.


#include "PlayerHUD.h"
#include "PlayerCharacter.h"

#define MY_LOG(x) UE_LOG(LogTemp, Warning, x);

APlayerHUD::APlayerHUD()
{
	// ----- Load UAsset Files
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainTextureOb(TEXT("/Game/HUD/HUDMain"));
	
	// ----- Get the Textures
	this->pHUDMainTexture = HUDMainTextureOb.Object;
	if (this->pHUDMainTexture == nullptr)
		UE_LOG(LogTemp, Warning, TEXT("Main Texture Not Loaded."));

	this->mCrosshair = UCanvas::MakeIcon(this->pHUDMainTexture, 44.0f, 374.0f, 66.0f, 66.0f);
	
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	GEngine->GameViewport->GetViewportSize(this->mViewPortSize);
	UE_LOG(LogTemp, Warning, TEXT("ViewX: %f; ViewY: %f."), this->mViewPortSize.X, this->mViewPortSize.Y);

	if (this->Canvas != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Canvas Exists here."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO Canvas."));
	}
}

void APlayerHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
	
	if (this->Canvas)
	{
		
		this->privGetPlayerPawn();
		this->DrawCrosshair();
		UE_LOG(LogTemp, Warning, TEXT("Drawing HUD."));
	}
	else
	{
		MY_LOG(TEXT("NO Canvas"));
	}
}

void APlayerHUD::DrawCrosshair()
{
	float centerx;
	float centery;
	this->Canvas->GetCenter(centerx, centery); // temporay until get gun
	this->Canvas->SetDrawColor(FColor::Red);
	this->Canvas->DrawIcon(this->mCrosshair, centerx - 5.0f, centery - 5.0f, 0.5f);

}

void APlayerHUD::privGetPlayerPawn()
{
	this->pPlayer = Cast<APlayerCharacter>(GetOwningPawn());
	if (this->pPlayer == nullptr)
	{
		MY_LOG(TEXT("No Player Owns HUD"));
	}

}
