// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

// https://platform.openai.com/docs/api-reference

namespace OpenAI
{
const FString API_URL = "https://api.openai.com";

namespace V1
{
const FString ModelsURL = API_URL + "/v1/models";
const FString CompletionURL = API_URL + "/v1/completions";
const FString ChatCompletionURL = API_URL + "/v1/chat/completions";
const FString EditsURL = API_URL + "/v1/edits";
const FString ImageGenerationsURL = API_URL + "/v1/images/generations";
const FString ImageEditsURL = API_URL + "/v1/images/edits";
const FString ImageVariationsURL = API_URL + "/v1/images/variations";
const FString EmbeddingsURL = API_URL + "/v1/embeddings";
const FString AudioTranscriptionsURL = API_URL + "/v1/audio/transcriptions";
const FString AudioTranslationsURL = API_URL + "/v1/audio/translations";
const FString FilesURL = API_URL + "/v1/files";
const FString FineTunesURL = API_URL + "/v1/fine-tunes";
const FString ModerationsURL = API_URL + "/v1/moderations";
}  // namespace V1
}  // namespace OpenAI
