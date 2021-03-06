// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorManager.h"

UActorManager::UActorManager()
{

}

void UActorManager::Init()
{
	m_mapActors.Add(TEXT("Lance"), LoadClass<ABaseActor>(this, TEXT("Blueprint'/Game/SoulHunter/Actor/Weapons/BPLancer.BPLancer_C'")));
	m_mapActors.Add(TEXT("Spear"), LoadClass<ABaseActor>(this, TEXT("Blueprint'/Game/SoulHunter/Actor/Weapons/BPSpear.BPSpear_C'")));
	m_mapActors.Add(TEXT("Shield"), LoadClass<ABaseActor>(this, TEXT("Blueprint'/Game/SoulHunter/Actor/Weapons/BPShield.BPShield_C'")));
	m_mapActors.Add(TEXT("Sphere"), LoadClass<ABaseActor>(this, TEXT("Blueprint'/Game/SoulHunter/Actor/Bullet/BPSphere.BPSphere_C'")));
}

ABaseActor* UActorManager::CreateActor(const FName& Name, AActor* Owner) const
{
	return CreateActor(Name, Owner, FVector::ZeroVector, FRotator::ZeroRotator);
}

ABaseActor* UActorManager::CreateActor(const FName& Name, AActor* Owner, const FVector& Location, const FRotator& Rotator) const
{
	FActorSpawnParameters param;

	param.Owner = Owner;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;	

	return CreateActor(Name, Owner, Location, Rotator, param);
}

ABaseActor* UActorManager::CreateActor(const FName& Name, AActor* Owner, const FVector& Location, const FRotator& Rotator, const FActorSpawnParameters& param) const
{
	const TSubclassOf<ABaseActor>* SubclassOf = m_mapActors.Find(Name);

	if (nullptr != SubclassOf
		&& IsValid(*SubclassOf))
	{
		return GetWorld()->SpawnActor<ABaseActor>(*SubclassOf, Location, Rotator, param);
	}
	else
	{
		LOG(TEXT("Find Actor Failed.. <%s>"), *Name.ToString());
	}

	return nullptr;
}