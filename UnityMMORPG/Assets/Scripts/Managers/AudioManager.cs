using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Audio;

/// <summary>
/// Manages all audio in the game
/// </summary>
public class AudioManager : MonoBehaviour
{
    public static AudioManager Instance { get; private set; }
    
    [Header("Audio Settings")]
    public AudioMixer masterMixer;
    public AudioMixerGroup musicGroup;
    public AudioMixerGroup sfxGroup;
    public AudioMixerGroup voiceGroup;
    public AudioMixerGroup ambientGroup;
    
    [Header("Music")]
    public AudioClip[] backgroundMusic;
    public AudioClip[] zoneMusic;
    public AudioClip[] battleMusic;
    public AudioClip[] bossMusic;
    
    [Header("SFX")]
    public AudioClip[] playerFootsteps;
    public AudioClip[] playerAttack;
    public AudioClip[] playerAbility;
    public AudioClip[] playerHurt;
    public AudioClip[] playerDeath;
    public AudioClip[] playerJump;
    public AudioClip[] playerLand;
    
    public AudioClip[] enemyAttack;
    public AudioClip[] enemyHurt;
    public AudioClip[] enemyDeath;
    
    public AudioClip[] itemPickup;
    public AudioClip[] itemUse;
    public AudioClip[] itemEquip;
    public AudioClip[] itemDrop;
    
    public AudioClip[] uiClick;
    public AudioClip[] uiHover;
    public AudioClip[] uiOpen;
    public AudioClip[] uiClose;
    public AudioClip[] uiNotification;
    
    public AudioClip[] abilitySounds;
    public AudioClip[] spellSounds;
    public AudioClip[] weaponSounds;
    
    [Header("Voice")]
    public AudioClip[] playerVoices;
    public AudioClip[] npcVoices;
    public AudioClip[] bossVoices;
    
    [Header("Ambient")]
    public AudioClip[] ambientSounds;
    public AudioClip[] weatherSounds;
    public AudioClip[] environmentSounds;
    
    [Header("Synthesizer")]
    public AudioClip[] synthNotes;
    public AudioClip[] drumSamples;
    public AudioClip[] bassSamples;
    public AudioClip[] leadSamples;
    public AudioClip[] padSamples;
    
    private Dictionary<string, AudioClip> audioClipCache = new Dictionary<string, AudioClip>();
    private Dictionary<string, AudioSource> audioSourceCache = new Dictionary<string, AudioSource>();
    private List<AudioSource> activeSources = new List<AudioSource>();
    
    private AudioSource musicSource;
    private AudioSource ambientSource;
    private AudioSource voiceSource;
    
    private bool isInitialized = false;
    
    public override void Awake()
    {
        base.Awake();
        Instance = this;
        
        // Create audio sources
        CreateAudioSources();
        
        isInitialized = true;
    }
    
    private void CreateAudioSources()
    {
        // Create music source
        GameObject musicObj = new GameObject("MusicSource");
        musicObj.transform.SetParent(transform);
        musicSource = musicObj.AddComponent<AudioSource>();
        musicSource.outputAudioMixerGroup = musicGroup;
        musicSource.loop = true;
        musicSource.volume = 0.5f;
        
        // Create ambient source
        GameObject ambientObj = new GameObject("AmbientSource");
        ambientObj.transform.SetParent(transform);
        ambientSource = ambientObj.AddComponent<AudioSource>();
        ambientSource.outputAudioMixerGroup = ambientGroup;
        ambientSource.loop = true;
        ambientSource.volume = 0.3f;
        
        // Create voice source
        GameObject voiceObj = new GameObject("VoiceSource");
        voiceObj.transform.SetParent(transform);
        voiceSource = voiceObj.AddComponent<AudioSource>();
        voiceSource.outputAudioMixerGroup = voiceGroup;
        voiceSource.volume = 0.7f;
    }
    
    private void Update()
    {
        // Clean up finished audio sources
        CleanupFinishedSources();
    }
    
    private void CleanupFinishedSources()
    {
        for (int i = activeSources.Count - 1; i >= 0; i--)
        {
            if (activeSources[i] != null && !activeSources[i].isPlaying)
            {
                Destroy(activeSources[i].gameObject);
                activeSources.RemoveAt(i);
            }
        }
    }
    
    public void PlaySound(string soundName, AudioMixerGroup mixerGroup = null, float volume = 1f, bool loop = false)
    {
        if (!isInitialized) return;
        
        AudioClip clip = GetAudioClip(soundName);
        if (clip == null)
        {
            Debug.LogWarning($"Audio clip not found: {soundName}");
            return;
        }
        
        PlaySound(clip, mixerGroup, volume, loop);
    }
    
    public void PlaySound(AudioClip clip, AudioMixerGroup mixerGroup = null, float volume = 1f, bool loop = false)
    {
        if (clip == null) return;
        
        if (mixerGroup == null)
        {
            mixerGroup = sfxGroup;
        }
        
        GameObject sourceObj = new GameObject($"AudioSource_{clip.name}");
        sourceObj.transform.SetParent(transform);
        AudioSource source = sourceObj.AddComponent<AudioSource>();
        
        source.clip = clip;
        source.outputAudioMixerGroup = mixerGroup;
        source.volume = volume;
        source.loop = loop;
        
        source.Play();
        
        activeSources.Add(source);
        
        if (!loop)
        {
            Destroy(sourceObj, clip.length);
        }
    }
    
    public void PlayMusic(string musicName, bool loop = true, float fadeDuration = 1f)
    {
        AudioClip clip = GetAudioClip(musicName);
        if (clip == null)
        {
            Debug.LogWarning($"Music clip not found: {musicName}");
            return;
        }
        
        PlayMusic(clip, loop, fadeDuration);
    }
    
    public void PlayMusic(AudioClip clip, bool loop = true, float fadeDuration = 1f)
    {
        if (musicSource == null || clip == null) return;
        
        StartCoroutine(FadeMusic(clip, loop, fadeDuration));
    }
    
    private System.Collections.IEnumerator FadeMusic(AudioClip newClip, bool loop, float fadeDuration)
    {
        float startVolume = musicSource.volume;
        
        // Fade out current music
        if (musicSource.isPlaying)
        {
            float t = 0f;
            while (t < fadeDuration)
            {
                t += Time.deltaTime;
                musicSource.volume = Mathf.Lerp(startVolume, 0f, t / fadeDuration);
                yield return null;
            }
            musicSource.Stop();
        }
        
        // Set new clip
        musicSource.clip = newClip;
        musicSource.loop = loop;
        musicSource.volume = 0f;
        musicSource.Play();
        
        // Fade in new music
        float t2 = 0f;
        while (t2 < fadeDuration)
        {
            t2 += Time.deltaTime;
            musicSource.volume = Mathf.Lerp(0f, startVolume, t2 / fadeDuration);
            yield return null;
        }
        musicSource.volume = startVolume;
    }
    
    public void StopMusic(float fadeDuration = 1f)
    {
        if (musicSource == null || !musicSource.isPlaying) return;
        
        StartCoroutine(FadeOutMusic(fadeDuration));
    }
    
    private System.Collections.IEnumerator FadeOutMusic(float fadeDuration)
    {
        float startVolume = musicSource.volume;
        float t = 0f;
        
        while (t < fadeDuration)
        {
            t += Time.deltaTime;
            musicSource.volume = Mathf.Lerp(startVolume, 0f, t / fadeDuration);
            yield return null;
        }
        musicSource.Stop();
        musicSource.volume = startVolume;
    }
    
    public void PlayAmbient(string ambientName, bool loop = true)
    {
        AudioClip clip = GetAudioClip(ambientName);
        if (clip == null)
        {
            Debug.LogWarning($"Ambient clip not found: {ambientName}");
            return;
        }
        
        PlayAmbient(clip, loop);
    }
    
    public void PlayAmbient(AudioClip clip, bool loop = true)
    {
        if (ambientSource == null || clip == null) return;
        
        ambientSource.clip = clip;
        ambientSource.loop = loop;
        ambientSource.Play();
    }
    
    public void StopAmbient()
    {
        if (ambientSource != null && ambientSource.isPlaying)
        {
            ambientSource.Stop();
        }
    }
    
    public void PlayVoice(string voiceName)
    {
        AudioClip clip = GetAudioClip(voiceName);
        if (clip == null)
        {
            Debug.LogWarning($"Voice clip not found: {voiceName}");
            return;
        }
        
        PlayVoice(clip);
    }
    
    public void PlayVoice(AudioClip clip)
    {
        if (voiceSource == null || clip == null) return;
        
        voiceSource.clip = clip;
        voiceSource.Play();
    }
    
    public void StopVoice()
    {
        if (voiceSource != null && voiceSource.isPlaying)
        {
            voiceSource.Stop();
        }
    }
    
    public void PlaySynthNote(string note, string waveType = "sine", float duration = 0.2f, float volume = 0.5f)
    {
        // Map note to audio clip
        string clipName = $"Synth_{note}_{waveType}";
        AudioClip clip = GetAudioClip(clipName);
        
        if (clip == null)
        {
            // Try to find a matching note
            foreach (var synthClip in synthNotes)
            {
                if (synthClip.name.Contains(note))
                {
                    clip = synthClip;
                    break;
                }
            }
        }
        
        if (clip != null)
        {
            PlaySound(clip, sfxGroup, volume, false);
        }
        else
        {
            Debug.LogWarning($"Synth note not found: {note}");
        }
    }
    
    public void PlayDrumSample(string drumType, float volume = 0.5f)
    {
        // Map drum type to audio clip
        string clipName = $"Drum_{drumType}";
        AudioClip clip = GetAudioClip(clipName);
        
        if (clip == null)
        {
            // Try to find a matching drum sample
            foreach (var drumClip in drumSamples)
            {
                if (drumClip.name.Contains(drumType))
                {
                    clip = drumClip;
                    break;
                }
            }
        }
        
        if (clip != null)
        {
            PlaySound(clip, sfxGroup, volume, false);
        }
        else
        {
            Debug.LogWarning($"Drum sample not found: {drumType}");
        }
    }
    
    public void PlayHitSound(bool isPerfect)
    {
        string soundName = isPerfect ? "Hit_Perfect" : "Hit_Great";
        PlaySound(soundName);
    }
    
    public void PlayMissSound()
    {
        PlaySound("Hit_Miss");
    }
    
    public void PlayAbilitySound()
    {
        PlaySound("Ability_Special");
    }
    
    public void PlayFootstepSound()
    {
        if (playerFootsteps.Length > 0)
        {
            AudioClip clip = playerFootsteps[UnityEngine.Random.Range(0, playerFootsteps.Length)];
            PlaySound(clip, sfxGroup, 0.3f, false);
        }
    }
    
    public void PlayPlayerAttackSound()
    {
        if (playerAttack.Length > 0)
        {
            AudioClip clip = playerAttack[UnityEngine.Random.Range(0, playerAttack.Length)];
            PlaySound(clip, sfxGroup, 0.5f, false);
        }
    }
    
    public void PlayPlayerHurtSound()
    {
        if (playerHurt.Length > 0)
        {
            AudioClip clip = playerHurt[UnityEngine.Random.Range(0, playerHurt.Length)];
            PlaySound(clip, sfxGroup, 0.7f, false);
        }
    }
    
    public void PlayPlayerDeathSound()
    {
        if (playerDeath.Length > 0)
        {
            AudioClip clip = playerDeath[UnityEngine.Random.Range(0, playerDeath.Length)];
            PlaySound(clip, sfxGroup, 0.8f, false);
        }
    }
    
    public void PlayEnemyAttackSound()
    {
        if (enemyAttack.Length > 0)
        {
            AudioClip clip = enemyAttack[UnityEngine.Random.Range(0, enemyAttack.Length)];
            PlaySound(clip, sfxGroup, 0.5f, false);
        }
    }
    
    public void PlayEnemyHurtSound()
    {
        if (enemyHurt.Length > 0)
        {
            AudioClip clip = enemyHurt[UnityEngine.Random.Range(0, enemyHurt.Length)];
            PlaySound(clip, sfxGroup, 0.4f, false);
        }
    }
    
    public void PlayEnemyDeathSound()
    {
        if (enemyDeath.Length > 0)
        {
            AudioClip clip = enemyDeath[UnityEngine.Random.Range(0, enemyDeath.Length)];
            PlaySound(clip, sfxGroup, 0.6f, false);
        }
    }
    
    public void PlayItemPickupSound()
    {
        if (itemPickup.Length > 0)
        {
            AudioClip clip = itemPickup[UnityEngine.Random.Range(0, itemPickup.Length)];
            PlaySound(clip, sfxGroup, 0.4f, false);
        }
    }
    
    public void PlayItemUseSound()
    {
        if (itemUse.Length > 0)
        {
            AudioClip clip = itemUse[UnityEngine.Random.Range(0, itemUse.Length)];
            PlaySound(clip, sfxGroup, 0.4f, false);
        }
    }
    
    public void PlayUIClickSound()
    {
        if (uiClick.Length > 0)
        {
            AudioClip clip = uiClick[UnityEngine.Random.Range(0, uiClick.Length)];
            PlaySound(clip, sfxGroup, 0.3f, false);
        }
    }
    
    public void PlayUIHoverSound()
    {
        if (uiHover.Length > 0)
        {
            AudioClip clip = uiHover[UnityEngine.Random.Range(0, uiHover.Length)];
            PlaySound(clip, sfxGroup, 0.2f, false);
        }
    }
    
    public void PlayUINotificationSound()
    {
        if (uiNotification.Length > 0)
        {
            AudioClip clip = uiNotification[UnityEngine.Random.Range(0, uiNotification.Length)];
            PlaySound(clip, sfxGroup, 0.4f, false);
        }
    }
    
    public void SetMasterVolume(float volume)
    {
        if (masterMixer != null)
        {
            masterMixer.SetFloat("MasterVolume", Mathf.Log10(Mathf.Clamp(volume, 0.0001f, 1f)) * 20f);
        }
    }
    
    public void SetMusicVolume(float volume)
    {
        if (masterMixer != null)
        {
            masterMixer.SetFloat("MusicVolume", Mathf.Log10(Mathf.Clamp(volume, 0.0001f, 1f)) * 20f);
        }
    }
    
    public void SetSFXVolume(float volume)
    {
        if (masterMixer != null)
        {
            masterMixer.SetFloat("SFXVolume", Mathf.Log10(Mathf.Clamp(volume, 0.0001f, 1f)) * 20f);
        }
    }
    
    public void SetVoiceVolume(float volume)
    {
        if (masterMixer != null)
        {
            masterMixer.SetFloat("VoiceVolume", Mathf.Log10(Mathf.Clamp(volume, 0.0001f, 1f)) * 20f);
        }
    }
    
    public void SetAmbientVolume(float volume)
    {
        if (masterMixer != null)
        {
            masterMixer.SetFloat("AmbientVolume", Mathf.Log10(Mathf.Clamp(volume, 0.0001f, 1f)) * 20f);
        }
    }
    
    public void MuteAll(bool mute)
    {
        SetMasterVolume(mute ? 0f : 1f);
    }
    
    public void PauseAll()
    {
        AudioListener.pause = true;
    }
    
    public void ResumeAll()
    {
        AudioListener.pause = false;
    }
    
    public void StopAll()
    {
        // Stop all active sources
        foreach (var source in activeSources)
        {
            if (source != null && source.isPlaying)
            {
                source.Stop();
            }
        }
        
        // Stop persistent sources
        if (musicSource != null && musicSource.isPlaying)
        {
            musicSource.Stop();
        }
        
        if (ambientSource != null && ambientSource.isPlaying)
        {
            ambientSource.Stop();
        }
        
        if (voiceSource != null && voiceSource.isPlaying)
        {
            voiceSource.Stop();
        }
    }
    
    public AudioClip GetAudioClip(string clipName)
    {
        if (audioClipCache.TryGetValue(clipName, out AudioClip clip))
        {
            return clip;
        }
        
        // Search in all loaded clips
        AudioClip foundClip = null;
        
        // Check background music
        foreach (var musicClip in backgroundMusic)
        {
            if (musicClip != null && musicClip.name.Equals(clipName, StringComparison.OrdinalIgnoreCase))
            {
                foundClip = musicClip;
                break;
            }
        }
        
        if (foundClip == null)
        {
            // Check zone music
            foreach (var musicClip in zoneMusic)
            {
                if (musicClip != null && musicClip.name.Equals(clipName, StringComparison.OrdinalIgnoreCase))
                {
                    foundClip = musicClip;
                    break;
                }
            }
        }
        
        if (foundClip == null)
        {
            // Check battle music
            foreach (var musicClip in battleMusic)
            {
                if (musicClip != null && musicClip.name.Equals(clipName, StringComparison.OrdinalIgnoreCase))
                {
                    foundClip = musicClip;
                    break;
                }
            }
        }
        
        if (foundClip == null)
        {
            // Check SFX
            foreach (var sfxClip in playerFootsteps)
            {
                if (sfxClip != null && sfxClip.name.Equals(clipName, StringComparison.OrdinalIgnoreCase))
                {
                    foundClip = sfxClip;
                    break;
                }
            }
        }
        
        if (foundClip != null)
        {
            audioClipCache[clipName] = foundClip;
        }
        
        return foundClip;
    }
    
    public void LoadAudioClips()
    {
        // Load all audio clips from Resources
        AudioClip[] clips = Resources.LoadAll<AudioClip>("Audio");
        
        foreach (var clip in clips)
        {
            if (clip != null && !audioClipCache.ContainsKey(clip.name))
            {
                audioClipCache[clip.name] = clip;
            }
        }
    }
    
    public void UnloadUnusedClips()
    {
        // Clear cache
        audioClipCache.Clear();
        
        // Force garbage collection
        System.GC.Collect();
    }
}
