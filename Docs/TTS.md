# Text to speech

Text to speech [(TTS)](https://platform.openai.com/docs/guides/text-to-speech) is now available with all parameters:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/speech.png)

Notes: 
- The `WMFCodecs` plugin should be enabled (go to `Edit -> Plugins -> WMFCodecs`).
- Only MP3 and AAC formats are supported.
- Audio files are saved in the plugin's `Saved` directory. You can control the saving options with the `SpeechSettings` structure:

![](https://github.com/life-exe/UnrealOpenAIPlugin/blob/master/Media/TTS/settings.png)
