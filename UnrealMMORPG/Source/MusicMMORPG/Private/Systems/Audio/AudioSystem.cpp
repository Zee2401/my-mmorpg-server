#include "AudioSystem.h"
#include "MusicMMORPGCharacter.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Engine/World.h"

// Initialize note frequency map
const TMap<ENoteType, float> UAudioSystem::NoteFrequencyMap = {
    {ENoteType::C4, 261.63f},
    {ENoteType::CSharp4, 277.18f},
    {ENoteType::D4, 293.66f},
    {ENoteType::DSharp4, 311.13f},
    {ENoteType::E4, 329.63f},
    {ENoteType::F4, 349.23f},
    {ENoteType::FSharp4, 369.99f},
    {ENoteType::G4, 392.00f},
    {ENoteType::GSharp4, 415.30f},
    {ENoteType::A4, 440.00f},
    {ENoteType::ASharp4, 466.16f},
    {ENoteType::B4, 493.88f},
    {ENoteType::C5, 523.25f}
};

UAudioSystem::UAudioSystem()
{
    // Initialize audio components
    BackgroundMusicComponent = nullptr;
    BattleMusicComponent = nullptr;
    
    // Initialize volume settings
    MasterVolume = 1.0f;
    MusicVolume = 0.5f;
    SFXVolume = 1.0f;
    VoiceVolume = 1.0f;
}

void UAudioSystem::Initialize()
{
    // Initialize audio system
    CleanupAudioComponents();
}

void UAudioSystem::Tick(float DeltaTime)
{
    // Clean up finished audio components
    CleanupAudioComponents();
}

void UAudioSystem::Shutdown()
{
    // Stop all audio
    StopBackgroundMusic();
    StopBattleMusic();
    
    // Clean up all audio components
    CleanupAudioComponents();
}

void UAudioSystem::PlayNote(ENoteType Note, float Duration, float Volume, EInstrumentType Instrument)
{
    if (const USoundBase* Sound = NoteSounds.Find(Note))
    {
        UAudioComponent* AudioComponent = PlaySound(*Sound, Volume * SFXVolume * MasterVolume);
        if (AudioComponent)
        {
            ActiveAudioComponents.Add(AudioComponent);
            OnNotePlayed.Broadcast(Note);
        }
    }
}

void UAudioSystem::PlayDrum(EDrumType Drum, float Duration, float Volume)
{
    if (const USoundBase* Sound = DrumSounds.Find(Drum))
    {
        UAudioComponent* AudioComponent = PlaySound(*Sound, Volume * SFXVolume * MasterVolume);
        if (AudioComponent)
        {
            ActiveAudioComponents.Add(AudioComponent);
            OnDrumPlayed.Broadcast(Drum);
        }
    }
}

void UAudioSystem::PlayChord(const TArray<ENoteType>& Notes, float Duration, float Volume, EInstrumentType Instrument)
{
    for (ENoteType Note : Notes)
    {
        PlayNote(Note, Duration, Volume, Instrument);
    }
}

void UAudioSystem::PlayScale(ENoteType StartNote, int32 NumNotes, float NoteDuration, float Volume, EInstrumentType Instrument)
{
    int32 StartIndex = static_cast<int32>(StartNote);
    for (int32 i = 0; i < NumNotes; i++)
    {
        int32 NoteIndex = (StartIndex + i) % static_cast<int32>(ENoteType::MAX);
        ENoteType Note = static_cast<ENoteType>(NoteIndex);
        PlayNote(Note, NoteDuration, Volume, Instrument);
    }
}

void UAudioSystem::PlaySoundEffect(ESoundEffectType Effect, float Volume)
{
    if (const USoundBase* Sound = SoundEffectMap.Find(Effect))
    {
        UAudioComponent* AudioComponent = PlaySound(*Sound, Volume * SFXVolume * MasterVolume);
        if (AudioComponent)
        {
            ActiveAudioComponents.Add(AudioComponent);
            OnSoundEffectPlayed.Broadcast(Effect);
        }
    }
}

void UAudioSystem::PlayHitSound(float Accuracy, float Volume)
{
    if (Accuracy >= 0.9f)
    {
        PlaySoundEffect(ESoundEffectType::Perfect, Volume);
    }
    else if (Accuracy >= 0.7f)
    {
        PlaySoundEffect(ESoundEffectType::Great, Volume);
    }
    else if (Accuracy >= 0.3f)
    {
        PlaySoundEffect(ESoundEffectType::Hit, Volume);
    }
    else
    {
        PlaySoundEffect(ESoundEffectType::Miss, Volume);
    }
}

void UAudioSystem::PlayAbilitySound(int32 AbilityIndex, float Volume)
{
    PlaySoundEffect(ESoundEffectType::Ability, Volume);
}

void UAudioSystem::PlayBackgroundMusic(const FString& MusicName, float Volume)
{
    // Stop current background music
    StopBackgroundMusic();
    
    // Find music by name
    for (const auto& Pair : ZoneMusicMap)
    {
        if (Pair.Key.ToString() == MusicName)
        {
            BackgroundMusicComponent = PlaySound(Pair.Value, Volume * MusicVolume * MasterVolume, 1.0f, 0.0f);
            if (BackgroundMusicComponent)
            {
                BackgroundMusicComponent->bLoop = true;
                OnBackgroundMusicStarted.Broadcast(MusicName);
            }
            return;
        }
    }
    
    // If not found in zone music, try to play by name
    if (USoundBase* Sound = LoadObject<USoundBase>(nullptr, *MusicName))
    {
        BackgroundMusicComponent = PlaySound(Sound, Volume * MusicVolume * MasterVolume, 1.0f, 0.0f);
        if (BackgroundMusicComponent)
        {
            BackgroundMusicComponent->bLoop = true;
            OnBackgroundMusicStarted.Broadcast(MusicName);
        }
    }
}

void UAudioSystem::StopBackgroundMusic()
{
    if (BackgroundMusicComponent)
    {
        BackgroundMusicComponent->Stop();
        BackgroundMusicComponent = nullptr;
        OnBackgroundMusicStopped.Broadcast();
    }
}

void UAudioSystem::PlayZoneMusic(FName ZoneName)
{
    if (const USoundBase* Music = ZoneMusicMap.Find(ZoneName))
    {
        PlayBackgroundMusic(Music->GetName(), MusicVolume);
    }
}

void UAudioSystem::PlayBossMusic(FName BossName)
{
    if (const USoundBase* Music = BossMusicMap.Find(BossName))
    {
        PlayBackgroundMusic(Music->GetName(), MusicVolume);
    }
}

void UAudioSystem::PlayBattleMusic()
{
    if (BattleMusic)
    {
        StopBackgroundMusic();
        BattleMusicComponent = PlaySound(BattleMusic, MusicVolume * MasterVolume, 1.0f, 0.0f);
        if (BattleMusicComponent)
        {
            BattleMusicComponent->bLoop = true;
            OnBattleMusicStarted.Broadcast();
        }
    }
}

void UAudioSystem::StopBattleMusic()
{
    if (BattleMusicComponent)
    {
        BattleMusicComponent->Stop();
        BattleMusicComponent = nullptr;
        OnBattleMusicStopped.Broadcast();
    }
}

void UAudioSystem::PlayCharacterSound(AMusicMMORPGCharacter* Character, ESoundEffectType Effect, float Volume)
{
    if (Character)
    {
        PlaySoundEffect(Effect, Volume);
    }
}

void UAudioSystem::PlayFootstepSound(AMusicMMORPGCharacter* Character, float Volume)
{
    if (Character)
    {
        // Play footstep sound based on character's movement
        PlaySoundEffect(ESoundEffectType::Hit, Volume * 0.5f);
    }
}

void UAudioSystem::PlayAttackSound(AMusicMMORPGCharacter* Character, float Volume)
{
    if (Character)
    {
        // Play attack sound based on character's class
        ECharacterClass CharacterClass = Character->CharacterClass;
        
        // For now, just play a generic ability sound
        PlaySoundEffect(ESoundEffectType::Ability, Volume);
    }
}

bool UAudioSystem::StartVoiceChat()
{
    // In a real implementation, this would start voice chat
    return false;
}

void UAudioSystem::StopVoiceChat()
{
    // In a real implementation, this would stop voice chat
}

bool UAudioSystem::IsVoiceChatEnabled() const
{
    return false;
}

void UAudioSystem::SetMasterVolume(float Volume)
{
    MasterVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
    // Update all active audio components
    for (UAudioComponent* Component : ActiveAudioComponents)
    {
        if (Component)
        {
            Component->SetVolumeMultiplier(MasterVolume);
        }
    }
    
    if (BackgroundMusicComponent)
    {
        BackgroundMusicComponent->SetVolumeMultiplier(MasterVolume * MusicVolume);
    }
    
    if (BattleMusicComponent)
    {
        BattleMusicComponent->SetVolumeMultiplier(MasterVolume * MusicVolume);
    }
}

void UAudioSystem::SetMusicVolume(float Volume)
{
    MusicVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
    
    if (BackgroundMusicComponent)
    {
        BackgroundMusicComponent->SetVolumeMultiplier(MasterVolume * MusicVolume);
    }
    
    if (BattleMusicComponent)
    {
        BattleMusicComponent->SetVolumeMultiplier(MasterVolume * MusicVolume);
    }
}

void UAudioSystem::SetSFXVolume(float Volume)
{
    SFXVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void UAudioSystem::SetVoiceVolume(float Volume)
{
    VoiceVolume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

float UAudioSystem::GetMasterVolume() const
{
    return MasterVolume;
}

float UAudioSystem::GetMusicVolume() const
{
    return MusicVolume;
}

float UAudioSystem::GetSFXVolume() const
{
    return SFXVolume;
}

float UAudioSystem::GetVoiceVolume() const
{
    return VoiceVolume;
}

bool UAudioSystem::IsPlayingBackgroundMusic() const
{
    return BackgroundMusicComponent != nullptr && BackgroundMusicComponent->IsPlaying();
}

bool UAudioSystem::IsPlayingBattleMusic() const
{
    return BattleMusicComponent != nullptr && BattleMusicComponent->IsPlaying();
}

UAudioComponent* UAudioSystem::PlaySound(USoundBase* Sound, float Volume, float Pitch, float StartTime)
{
    if (!Sound) return nullptr;
    
    if (UWorld* World = GetOuter()->GetWorld())
    {
        UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(
            World,
            Sound,
            FVector::ZeroVector,
            FRotator::ZeroRotator,
            Volume,
            Pitch,
            StartTime
        );
        
        if (AudioComponent)
        {
            AudioComponent->bAutoDestroy = true;
            AudioComponent->SetVolumeMultiplier(Volume);
            return AudioComponent;
        }
    }
    
    return nullptr;
}

void UAudioSystem::CleanupAudioComponents()
{
    // Remove finished audio components
    for (int32 i = ActiveAudioComponents.Num() - 1; i >= 0; i--)
    {
        UAudioComponent* Component = ActiveAudioComponents[i];
        if (!Component || !Component->IsPlaying())
        {
            if (Component)
            {
                Component->Stop();
                Component->DestroyComponent();
            }
            ActiveAudioComponents.RemoveAt(i);
        }
    }
    
    // Clean up background music component if it's not playing
    if (BackgroundMusicComponent && !BackgroundMusicComponent->IsPlaying())
    {
        BackgroundMusicComponent->Stop();
        BackgroundMusicComponent->DestroyComponent();
        BackgroundMusicComponent = nullptr;
    }
    
    // Clean up battle music component if it's not playing
    if (BattleMusicComponent && !BattleMusicComponent->IsPlaying())
    {
        BattleMusicComponent->Stop();
        BattleMusicComponent->DestroyComponent();
        BattleMusicComponent = nullptr;
    }
}
