// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "ToolsTypes.generated.h"

USTRUCT(BlueprintType)
struct FFunctionCommon
{
    GENERATED_BODY()

    /**
      The name of the function to call.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Name;

    /**
      The arguments to call the function with, as generated by the model in JSON format.
      Note that the model does not always generate valid JSON,
      and may hallucinate parameters not defined by your function schema.
      Validate the arguments in your code before calling your function.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Arguments;
};

USTRUCT(BlueprintType)
struct FToolCalls
{
    GENERATED_BODY()

    /**
      The ID of the tool call.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString ID;

    /**
      The type of the tool. Currently, only function is supported.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type{"function"};

    /**
     The function that the model called.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FFunctionCommon Function;
};

USTRUCT(BlueprintType)
struct FToolCallsDelta : public FToolCalls
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Index{};
};

USTRUCT(BlueprintType)
struct FFunctionRequestBase
{
    GENERATED_BODY()

    /**
      The name of the function to be called. Must be a-z, A-Z, 0-9, or contain underscores and dashes,
      with a maximum length of 64.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Name;
};

USTRUCT(BlueprintType)
struct FFunctionRequest : public FFunctionRequestBase
{
    GENERATED_BODY()

    /**
      A description of what the function does, used by the model to choose when and how to call the function.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Description;

    /**
      The parameters the functions accepts, described as a JSON Schema object.
      See the guide for examples, and the JSON Schema reference for documentation about the format.

      To describe a function that accepts no parameters, provide the value {"type": "object", "properties": {}}.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Parameters;  // @todo: object

    /**
      Whether to enable strict schema adherence when generating the function call.
      If set to true, the model will follow the exact schema defined in the parameters field.
      Only a subset of JSON Schema is supported when strict is true.
      Learn more about Structured Outputs in the function calling guide.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Strict;
};

USTRUCT(BlueprintType)
struct FTools
{
    GENERATED_BODY()

    /**
      The type of the tool. Currently, only function is supported.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Type{"function"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FFunctionRequest Function;
};

USTRUCT(BlueprintType)
struct FToolChoice
{
    GENERATED_BODY()

    /**
      The type of the tool. Currently, only function is supported.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Type{"function"};

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FFunctionRequestBase Function;
};
