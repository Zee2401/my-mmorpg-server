pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "AudioSystem.generated.h"

// Forward declarations
class AMusicMMORPGCharacter;

// Note types
UENUM(BlueprintType)
enum class ENoteType : uint8
{
    C4 UMETA(DisplayName = "C4"),
    CSharp4 UMETA(DisplayName = "C#4"),
    D4 UMETA(DisplayName = "D4"),
    DSharp4 UMETA(DisplayName = "D#4"),
    E4 UMETA(DisplayName = "E4"),
    F4 UMETA(DisplayName = "F4"),
    FSharp4 UMETA(DisplayName = "F#4"),
    G4 UMETA(DisplayName = "G4"),
    GSharp4 UMETA(DisplayName = "G#4"),
    A4 UMETA(DisplayName = "A4"),
    ASharp4 UMETA(DisplayName = "A#4"),
    B4 UMETA(DisplayName = "B4"),
    C5 UMETA(DisplayName = "C5"),
    MAX UMETA(Hidden)
};

// Drum types
UENUM(BlueprintType)
enum class EDrumType : uint8
{
    Kick UMETA(DisplayName = "Kick"),
    Snare UMETA(DisplayName = "Snare"),
    HiHat UMETA(DisplayName = "Hi-Hat"),
    SubBass UMETA(DisplayName = "Sub-Bass"),
    Crash UMETA(DisplayName = "Crash"),
    Ride UMETA(DisplayName = "Ride"),
    Tom UMETA(DisplayName = "Tom"),
    MAX UMETA(Hidden)
};

// Instrument types
UENUM(BlueprintType)
enum class EInstrumentType : uint8
{
    Guitar UMETA(DisplayName = "Guitar"),
    Bass UMETA(DisplayName = "Bass"),
    Piano UMETA(DisplayName = "Piano"),
    Drums UMETA(DisplayName = "Drums"),
    Vocals UMETA(DisplayName = "Vocals"),
    Synthesizer UMETA(DisplayName = "Synthesizer"),
    MAX UMETA(Hidden)
};

// Sound effect types
UENUM(BlueprintType)
enum class ESoundEffectType : uint8
{
    Hit UMETA(DisplayName = "Hit"),
    Miss UMETA(DisplayName = "Miss"),
    Perfect UMETA(DisplayName = "Perfect"),
    Great UMETA(DisplayName = "Great"),
    Ability UMETA(DisplayName = "Ability"),
    LevelUp UMETA(DisplayName = "Level Up"),
    Damage UMETA(DisplayName = "Damage"),
    Heal UMETA(DisplayName = "Heal"),
    Death UMETA(DisplayName = "Death"),
    Victory UMETA(DisplayName = "Victory"),
    Defeat UMETA(DisplayName = "Defeat"),
    MAX UMETA(Hidden)
};

/**
 * Audio System for Music MMORPG
 * Handles all audio functionality including sound synthesis, music playback, and sound effects
 */
UCLASS(Blueprintable)
class MUSICMMORPG_API UAudioSystem : public UObject
{
    GENERATED_BODY()

public:
    UAudioSystem();
    
    virtual void Initialize();
    virtual void Tick(float DeltaTime);
    virtual void Shutdown();
    
    // Sound synthesizer
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayNote(ENoteType Note, float Duration = 0.5f, float Volume = 1.0f, EInstrumentType Instrument = EInstrumentType::Piano);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayDrum(EDrumType Drum, float Duration = 0.5f, float Volume = 1.0f);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayChord(const TArray<ENoteType>& Notes, float Duration = 0.5f, float Volume = 1.0f, EInstrumentType Instrument = EInstrumentType::Piano);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayScale(ENoteType StartNote, int32 NumNotes, float NoteDuration = 0.2f, float Volume = 1.0f, EInstrumentType Instrument = EInstrumentType::Piano);
    
    // Sound effects
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySoundEffect(ESoundEffectType Effect, float Volume = 1.0f);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayHitSound(float Accuracy, float Volume = 1.0f);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayAbilitySound(int32 AbilityIndex, float Volume = 1.0f);
    
    // Music playback
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayBackgroundMusic(const FString& MusicName, float Volume = 0.5f);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void StopBackgroundMusic();
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayZoneMusic(FName ZoneName);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayBossMusic(FName BossName);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayBattleMusic();
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void StopBattleMusic();
    
    // Character sounds
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayCharacterSound(AMusicMMORPGCharacter* Character, ESoundEffectType Effect, float Volume = 1.0f);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayFootstepSound(AMusicMMORPGCharacter* Character, float Volume = 1.0f);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayAttackSound(AMusicMMORPGCharacter* Character, float Volume = 1.0f);
    
    // Voice chat
    UFUNCTION(BlueprintCallable, Category = "Audio")
    bool StartVoiceChat();
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void StopVoiceChat();
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    bool IsVoiceChatEnabled() const;
    
    // Audio settings
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetMasterVolume(float Volume);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetMusicVolume(float Volume);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetSFXVolume(float Volume);
    
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetVoiceVolume(float Volume);
    
    UFUNCTION(BlueprintPure, Category = "Audio")
    float GetMasterVolume() const;
    
    UFUNCTION(BlueprintPure, Category = "Audio")
    float GetMusicVolume() const;
    
    UFUNCTION(BlueprintPure, Category = "Audio")
    float GetSFXVolume() const;
    
    UFUNCTION(BlueprintPure, Category = "Audio")
    float GetVoiceVolume() const;
    
    // Audio state
    UFUNCTION(BlueprintPure, Category = "Audio")
    bool IsPlayingBackgroundMusic() const;
    
    UFUNCTION(BlueprintPure, Category = "Audio")
    bool IsPlayingBattleMusic() const;
    
    // Audio components
    UPROPERTY(BlueprintReadOnly, Category = "Audio")
    TArray<UAudioComponent*> ActiveAudioComponents;
    
    UPROPERTY(BlueprintReadOnly, Category = "Audio")
    UAudioComponent* BackgroundMusicComponent;
    
    UPROPERTY(BlueprintReadOnly, Category = "Audio")
    UAudioComponent* BattleMusicComponent;
    
    // Sound assets (should be set in the editor)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    TMap<ENoteType, USoundBase*> NoteSounds;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    TMap<EDrumType, USoundBase*> DrumSounds;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    TMap<ESoundEffectType, USoundBase*> SoundEffectMap;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    TMap<FName, USoundBase*> ZoneMusicMap;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    TMap<FName, USoundBase*> BossMusicMap;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    USoundBase* BattleMusic;
    
    // Volume settings
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    float MasterVolume;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    float MusicVolume;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    float SFXVolume;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    float VoiceVolume;
    
    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotePlayedSignature, ENoteType, Note);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDrumPlayedSignature, EDrumType, Drum);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundEffectPlayedSignature, ESoundEffectType, Effect);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBackgroundMusicStartedSignature, FString, MusicName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackgroundMusicStoppedSignature);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleMusicStartedSignature);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattleMusicStoppedSignature);
    
    UPROPERTY(BlueprintAssignable, Category = "Audio")
    FOnNotePlayedSignature OnNotePlayed;
    
    UPROPERTY(BlueprintAssignable, Category = "Audio")
    FOnDrumPlayedSignature OnDrumPlayed;
    
    UPROPERTY(BlueprintAssignable, Category = "Audio")
    FOnSoundEffectPlayedSignature OnSoundEffectPlayed;
    
    UPROPERTY(BlueprintAssignable, Category = "Audio")
    FOnBackgroundMusicStartedSignature OnBackgroundMusicStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Audio")
    FOnBackgroundMusicStoppedSignature OnBackgroundMusicStopped;
    
    UPROPERTY(BlueprintAssignable, Category = "Audio")
    FOnBattleMusicStartedSignature OnBattleMusicStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Audio")
    FOnBattleMusicStoppedSignature OnBattleMusicStopped;
    
protected:
    // Helper functions
    UAudioComponent* PlaySound(USoundBase* Sound, float Volume = 1.0f, float Pitch = 1.0f, float StartTime = 0.0f);
    
    void CleanupAudioComponents();
    
    // Note frequency map
    static const TMap<ENoteType, float> NoteFrequencyMap;
};
