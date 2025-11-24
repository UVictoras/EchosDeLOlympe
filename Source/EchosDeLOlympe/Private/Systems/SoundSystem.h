// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "SoundSystem.generated.h"

UCLASS()
class USoundSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USoundSystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// --- SFX API ---

	UFUNCTION(BlueprintCallable, Category = "SoundSystem|SFX")
	void PlaySoundAtPlayer(USoundBase* Sound, float Volume = 1.0f, float Pitch = 1.0f, USoundAttenuation* Attenuation = nullptr);

	UFUNCTION(BlueprintCallable, Category = "SoundSystem|SFX")
	UAudioComponent* PlaySoundAtLocation(AActor* WorldContextActor, USoundBase* Sound, const FVector& Location, float Volume = 1.0f, float Pitch = 1.0f, USoundAttenuation* Attenuation = nullptr);

	UFUNCTION(BlueprintCallable, Category = "SoundSystem|SFX")
	UAudioComponent* PlaySound2D(USoundBase* Sound, float Volume = 1.0f, float Pitch = 1.0f);

	// --- Music API ---

	UFUNCTION(BlueprintCallable, Category = "SoundSystem|Music")
	void PlayMusic(USoundBase* Music, float Volume = 1.0f, float Pitch = 1.0f, float FadeInTime = 0.5f, bool bLoop = true);

	UFUNCTION(BlueprintCallable, Category = "SoundSystem|Music")
	void StopMusic(float FadeOutTime = 0.5f);

	UFUNCTION(BlueprintCallable, Category = "SoundSystem|Music")
	void FadeMusicTo(float TargetVolume, float FadeTime);

	UFUNCTION(BlueprintCallable, Category = "SoundSystem|Music")
	void SetMusicVolume(float NewVolume) { MusicVolume = FMath::Clamp(NewVolume, 0.0f, 2.0f); if (MusicComponent) MusicComponent->SetVolumeMultiplier(MusicVolume); }

	UFUNCTION(BlueprintCallable, Category = "SoundSystem|Music")
	UAudioComponent* GetMusicComponent() const { return MusicComponent; }

protected:

	UAudioComponent* AcquirePooledAudioComponent(AActor* WorldContextActor, const FVector& Location);

	void ReleasePooledAudioComponent(UAudioComponent* Component);

	UFUNCTION()
	void OnPooledAudioFinished();

private:

	UPROPERTY(Transient)
	TArray<UAudioComponent*> AudioComponentPool;

	UPROPERTY(EditDefaultsOnly, Category = "SoundSystem|Pooling")
	int32 MaxPoolSize = 16;

	UPROPERTY(Transient)
	UAudioComponent* MusicComponent = nullptr;

	UPROPERTY(Transient)
	float MusicVolume = 1.0f;

	FVector GetPlayerLocation() const;

	void CreateMusicComponentIfNeeded();

	void CleanupPool();
};
