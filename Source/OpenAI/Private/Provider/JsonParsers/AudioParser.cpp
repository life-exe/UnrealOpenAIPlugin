// OpenAI Sample, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "Provider/JsonParsers/AudioParser.h"

using namespace OpenAI;

bool AudioParser::IsVerboseResponse(const FString& ResponseString)
{
    return ResponseString.Find("segments") != INDEX_NONE;
}
