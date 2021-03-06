// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseActor.h"

// Sets default values
ABaseActor::ABaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_Root);

	m_Body = CreateDefaultSubobject<UBoxComponent>(TEXT("Body"));
	m_Body->SetupAttachment(m_Root);

	m_ActorType = EActorType::None;

	//m_Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	//m_Particle->SetupAttachment(m_Body);
}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();

	AddIgnoreActorWhenMoving(this);
	AddIgnoreActorWhenMoving(GetOwner());

	m_BodyCollisionProfileName = m_Body->GetCollisionProfileName();

	SetEnableCollision(false);

	m_Body->OnComponentHit.AddDynamic(this, &ABaseActor::OnHit);
	m_Body->OnComponentBeginOverlap.AddDynamic(this, &ABaseActor::OnOverlapBegin);
	m_Body->OnComponentEndOverlap.AddDynamic(this, &ABaseActor::OnOverlapEnd);
}

void ABaseActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_Body->IsCollisionEnabled())
	{
		DrawDebugBox(GetWorld(), GetActorLocation(), m_Body->GetScaledBoxExtent(), GetActorRotation().Quaternion(), FColor::Blue);
	}
}

float ABaseActor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (m_TakeDamageCallback.IsBound())
	{
		m_TakeDamageCallback.Execute(this, Damage, DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}

	return Damage;
}

void ABaseActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
	, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* pActor = Hit.GetActor();

	if (IsValid(pActor))
	{
		if (m_HitCallback.IsBound())
		{
			m_HitCallback.Execute(this, Hit);
		}
	}
}

void ABaseActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (this != SweepResult.Actor)
		{
			OnOverlapBeginFromBody(OverlappedComp, SweepResult);

			if (m_OverlapBeginCallback.IsBound())
			{
				m_OverlapBeginCallback.Execute(this, SweepResult);
			}
		}
	}
}

void ABaseActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		if (this != OtherActor)
		{
			if (m_OverlapEndCallback.IsBound())
			{
				m_OverlapEndCallback.Execute(this, OtherActor);
			}
		}
	}
}

void ABaseActor::OnOverlapBeginFromBody(UPrimitiveComponent* OverlappedComp, const FHitResult& HitResult)
{

}

void ABaseActor::OnOverlapEndFromBody(UPrimitiveComponent* OverlappedComp, AActor* OverlapEndActor)
{

}

void ABaseActor::AddIgnoreActorWhenMoving(AActor* pActor)
{
	m_Body->IgnoreActorWhenMoving(pActor, true);
}

void ABaseActor::ClearIgnoreActorWhenMoving()
{
	m_Body->ClearMoveIgnoreActors();
}

void ABaseActor::SetEnableCollision(bool Enable)
{
	if (false == Enable)
	{
		m_Body->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
	else
	{
		m_Body->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		m_Body->SetCollisionProfileName(m_BodyCollisionProfileName);
	}
}

FVector ABaseActor::GetBodyExtent() const
{
	return m_Body->GetScaledBoxExtent();
}

EActorType ABaseActor::GetActorType() const
{
	return m_ActorType;
}

bool ABaseActor::IsEnableCollision() const
{
	return m_Body->IsCollisionEnabled();
}