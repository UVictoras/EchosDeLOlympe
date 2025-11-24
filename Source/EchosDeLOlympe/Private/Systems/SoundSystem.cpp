// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

USoundSystem::USoundSystem()
{
}

void USoundSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	AudioComponentPool.Empty();
	MusicComponent = nullptr;
	MusicVolume = 1.0f;
}

void USoundSystem::Deinitialize()
{
	CleanupPool();

	if (MusicComponent)
	{
		MusicComponent->Stop();
		MusicComponent->DestroyComponent();
		MusicComponent = nullptr;
	}

	Super::Deinitialize();
}

FVector USoundSystem::GetPlayerLocation() const
{
	UWorld* World = GetWorld();
	if (!World) return FVector::ZeroVector;

	APlayerController* PC = World->GetFirstPlayerController();

	if (PC)
	{
		APawn* Pawn = PC->GetPawn();

		if (Pawn)
		{
			return Pawn->GetActorLocation();
		}

		APawn* Possessed = PC->GetPawn();

		if (Possessed) return Possessed->GetActorLocation();

		FVector Loc;
		FRotator Rot;
		PC->GetPlayerViewPoint(Loc, Rot);
		return Loc;
	}

	return FVector::ZeroVector;
}

UAudioComponent* USoundSystem::AcquirePooledAudioComponent(AActor* WorldContextActor, const FVector& Location)
{
	UWorld* World = (WorldContextActor ? WorldContextActor->GetWorld() : GetWorld());
	if (!World) return nullptr;

	for (int32 i = 0; i < AudioComponentPool.Num(); ++i)
	{
		UAudioComponent* AC = AudioComponentPool[i];

		if (AC && !AC->IsPlaying())
		{
			AC->SetWorldLocation(Location);
			AC->bAutoDestroy = false;
			return AC;
		}
	}

	if (AudioComponentPool.Num() < MaxPoolSize)
	{
		UAudioComponent* NewAC = NewObject<UAudioComponent>(World->GetWorldSettings());

		if (NewAC)
		{
			NewAC->RegisterComponentWithWorld(World);
			NewAC->SetAutoActivate(false);
			NewAC->bAutoDestroy = false;
			NewAC->OnAudioFinished.AddDynamic(this, &USoundSystem::OnPooledAudioFinished);
			AudioComponentPool.Add(NewAC);
			NewAC->SetWorldLocation(Location);

			return NewAC;
		}
	}

	UAudioComponent* Fallback = UGameplayStatics::SpawnSoundAtLocation(World, nullptr, Location);

	return Fallback;
}

void USoundSystem::ReleasePooledAudioComponent(UAudioComponent* Component)
{
	if (!Component) return;

	if (AudioComponentPool.Contains(Component))
	{
		Component->Stop();
		Component->SetRelativeLocation(FVector::ZeroVector);
		Component->SetWorldLocation(FVector::ZeroVector);
	}
	else
	{
		Component->Stop();
		Component->DestroyComponent();
	}
}

void USoundSystem::OnPooledAudioFinished()
{
	for (UAudioComponent* AC : AudioComponentPool)
	{
		if (!AC) continue;

		if (!AC->IsPlaying())
		{
			AC->Stop();
			AC->SetSound(nullptr);
			AC->AttenuationSettings = nullptr;
		};
	}
}

void USoundSystem::CleanupPool()
{
	for (UAudioComponent* AC : AudioComponentPool)
	{
		if (AC)
		{
			AC->Stop();
			AC->OnAudioFinished.RemoveAll(this);
			AC->DestroyComponent();
		}
	}

	AudioComponentPool.Empty();
}

#pragma region SFX API

void USoundSystem::PlaySoundAtPlayer(USoundBase* Sound, float Volume, float Pitch, USoundAttenuation* Attenuation)
{
	if (!Sound) return;

	FVector PlayerLoc = GetPlayerLocation();
	UWorld* World = GetWorld();

	if (!World) return;

	UAudioComponent* AC = AcquirePooledAudioComponent(nullptr, PlayerLoc);

	if (AC)
	{
		AC->SetSound(Sound);
		AC->SetPitchMultiplier(Pitch);
		AC->SetVolumeMultiplier(Volume);

		if (Attenuation)
		{
			AC->AttenuationSettings = Attenuation;
		}

		AC->SetWorldLocation(PlayerLoc);
		AC->Play(0.0f);
	}
	else
	{
		UGameplayStatics::SpawnSoundAtLocation(World, Sound, PlayerLoc, FRotator::ZeroRotator, Volume, Pitch, 0.0f, Attenuation);
	}
}

UAudioComponent* USoundSystem::PlaySoundAtLocation(AActor* WorldContextActor, USoundBase* Sound, const FVector& Location, float Volume, float Pitch, USoundAttenuation* Attenuation)
{
	if (!Sound) return nullptr;

	UWorld* World = (WorldContextActor ? WorldContextActor->GetWorld() : GetWorld());

	if (!World) return nullptr;

	UAudioComponent* AC = AcquirePooledAudioComponent(WorldContextActor, Location);

	if (AC)
	{
		AC->SetSound(Sound);
		AC->SetPitchMultiplier(Pitch);
		AC->SetVolumeMultiplier(Volume);
		AC->AttenuationSettings = Attenuation;
		AC->SetWorldLocation(Location);
		AC->Play(0.0f);

		return AC;
	}

	return UGameplayStatics::SpawnSoundAtLocation(World, Sound, Location, FRotator::ZeroRotator, Volume, Pitch, 0.0f, Attenuation);
}

UAudioComponent* USoundSystem::PlaySound2D(USoundBase* Sound, float Volume, float Pitch)
{
	if (!Sound) return nullptr;

	UWorld* World = GetWorld();
	 
	if (!World) return nullptr;

	return UGameplayStatics::SpawnSound2D(World, Sound, Volume, Pitch);
}

#pragma endregion

#pragma region Music API

void USoundSystem::CreateMusicComponentIfNeeded()
{
	if (MusicComponent && MusicComponent->IsValidLowLevel()) return;

	UWorld* World = GetWorld();

	if (!World) return;

	AWorldSettings* WS = World->GetWorldSettings();

	if (!WS) return;

	MusicComponent = NewObject<UAudioComponent>(WS ? WS : (UObject*)World);

	if (MusicComponent)
	{
		MusicComponent->bAutoActivate = false;
		MusicComponent->bIsUISound = false;
		MusicComponent->bAllowSpatialization = false;
		MusicComponent->SetVolumeMultiplier(MusicVolume);
		MusicComponent->RegisterComponentWithWorld(World);
	}
}

void USoundSystem::PlayMusic(USoundBase* Music, float Volume, float Pitch, float FadeInTime, bool bLoop)
{
	if (!Music) return;

	CreateMusicComponentIfNeeded();

	if (!MusicComponent) return;

	if (MusicComponent->IsPlaying())
	{
		MusicComponent->FadeOut(0.2f, 0.2f);
	}

	MusicComponent->SetSound(Music);
	MusicVolume = FMath::Clamp(Volume, 0.0f, 2.0f);
	MusicComponent->SetVolumeMultiplier(0.0f);
	MusicComponent->SetPitchMultiplier(Pitch);
	MusicComponent->Play(0.0f);

	if (FadeInTime > 0.0f)
	{
		MusicComponent->FadeIn(FadeInTime, MusicVolume, 0.0f);
	}
	else
	{
		MusicComponent->SetVolumeMultiplier(MusicVolume);
	}
}

void USoundSystem::StopMusic(float FadeOutTime)
{
	if (!MusicComponent) return;

	if (FadeOutTime > 0.0001f)
	{
		MusicComponent->FadeOut(FadeOutTime, 0.0f);

		FTimerHandle Th;

		if (UWorld* W = GetWorld())
		{
			W->GetTimerManager().SetTimer(Th, FTimerDelegate::CreateLambda([this]()
			{
				if (MusicComponent)
				{
					MusicComponent->Stop();
				}
			}), FadeOutTime + 0.02f, false);
		}
	}
	else
	{
		MusicComponent->Stop();
	}
}

void USoundSystem::FadeMusicTo(float TargetVolume, float FadeTime)
{
	if (!MusicComponent) return;

	TargetVolume = FMath::Clamp(TargetVolume, 0.0f, 2.0f);
	float CurrentVolume = MusicComponent->VolumeMultiplier;

	if (FadeTime <= 0.001f)
	{
		MusicComponent->SetVolumeMultiplier(TargetVolume);
		return;
	}

	if (!MusicComponent->IsPlaying())
	{
		MusicComponent->Play(0.0f);
	}

	MusicComponent->FadeIn(FadeTime, TargetVolume, 0.0f);
	MusicVolume = TargetVolume;
}

#pragma endregion