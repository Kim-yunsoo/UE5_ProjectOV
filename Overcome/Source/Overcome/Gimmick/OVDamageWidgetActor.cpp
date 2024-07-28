// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/OVDamageWidgetActor.h"

#include "Components/WidgetComponent.h"
#include "UI/OVAttackDamageWidget.h"

// Sets default values
AOVDamageWidgetActor::AOVDamageWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	DamageComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Damage"));
	static ConstructorHelpers::FClassFinder<UOVAttackDamageWidget> DamageRef(TEXT("/Game/UMG/WBP_DamageWidget.WBP_DamageWidget_C"));

	FVector RelativeLocation(40.f, 0.f, 0.f);
	FRotator RelativeRotation(0.f, -90.f, 0.f);

	DamageComponent->SetRelativeLocation(RelativeLocation);
	DamageComponent->SetRelativeRotation(RelativeRotation);

	if (DamageRef.Class)
	{
		DamageComponent->SetWidgetSpace(EWidgetSpace::Screen); //2Dº¯°æ
		DamageComponent->SetDrawSize(FVector2D(250.0f, 250.0f));
		DamageComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DamageComponent->SetWidgetClass(DamageRef.Class);
	}
}

// Called when the game starts or when spawned
void AOVDamageWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(2.f);
}

// Called every frame
void AOVDamageWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOVDamageWidgetActor::SetDamage(const float& InDamage)
{
	UOVAttackDamageWidget* Widget = Cast<UOVAttackDamageWidget>(DamageComponent->GetWidget());
	if (Widget)
	{
		Widget->SetDamage(InDamage);
	}
}

