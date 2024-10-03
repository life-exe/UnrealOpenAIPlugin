// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "AssistantTypes.generated.h"

UENUM(BlueprintType)
enum class EAssistantToolType : uint8
{
    CodeInterpreter = 0,
    FileSearch,
    Function
};

USTRUCT(BlueprintType)
struct FAssistantTool
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Type;

    // @todo: FileSearchTool

    // @todo: FunctionTool
};

USTRUCT(BlueprintType)
struct FAssistantToolResources
{
    GENERATED_BODY()
};

///////////////////////////////////////////////////////
//                 REQUEST TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FCreateAssistant
{
    GENERATED_BODY()

    /**
      ID of the model to use.
      You can use the List models API to see all of your available models,
      or see our Model overview for descriptions of them.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Model;

    /**
      The name of the assistant. The maximum length is 256 characters.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Name;

    /**
      The description of the assistant. The maximum length is 512 characters.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Description;

    /**
      The system instructions that the assistant uses. The maximum length is 256,000 characters.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Instructions;

    /**
      A list of tool enabled on the assistant.
      There can be a maximum of 128 tools per assistant.
      Tools can be of types code_interpreter, file_search, or function.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TArray<FAssistantTool> Tools;

    /**
      A set of resources that are used by the assistant's tools.
      The resources are specific to the type of tool. For example,
      the code_interpreter tool requires a list of file IDs,
      while the file_search tool requires a list of vector store IDs.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FAssistantToolResources Tool_Resources;

    /**
      Set of 16 key-value pairs that can be attached to an object.
      This can be useful for storing additional information about the object in a structured format.
      Keys can be a maximum of 64 characters long and values can be a maxium of 512 characters long.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    TMap<FString, FString> Metadata;

    /**
      What sampling temperature to use, between 0 and 2.
      Higher values like 0.8 will make the output more random,
      while lower values like 0.2 will make it more focused and deterministic.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Temperature{1.0f};

    /**
      An alternative to sampling with temperature, called nucleus sampling,
      where the model considers the results of the tokens with top_p probability mass.
      So 0.1 means only the tokens comprising the top 10% probability mass are considered.
      We generally recommend altering this or temperature but not both.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    float Top_P{1.0f};

    /**
      Specifies the format that the model must output.
      Compatible with GPT-4o, GPT-4 Turbo, and all GPT-3.5 Turbo models since gpt-3.5-turbo-1106.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Response_Format;
};

USTRUCT(BlueprintType)
struct FListAssistants
{
    GENERATED_BODY()

    /**
      A limit on the number of objects to be returned.
      Limit can range between 1 and 100, and the default is 20.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    int32 Limit{20};

    /**
      Sort order by the created_at timestamp of the objects.
      asc for ascending order and desc for descending order.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FString Order{"desc"};  // todo enum

    /**
      A cursor for use in pagination. after is an object ID that defines your place in the list.
      For instance, if you make a list request and receive 100 objects, ending with obj_foo,
      your subsequent call can include after=obj_foo in order to fetch the next page of the list.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    /**
      A cursor for use in pagination. before is an object ID that defines your place in the list.
      For instance, if you make a list request and receive 100 objects,
      ending with obj_foo, your subsequent call can include before=obj_foo in order to fetch the previous page of the list.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Before;

    FString ToQuery() const
    {
        FString Query{"?"};

        Query.Append("limit=").Append(FString::FromInt(Limit)).Append("&");
        Query.Append("order=").Append(Order).Append("&");

        if (After.IsSet)
        {
            Query.Append("after=").Append(After.Value).Append("&");
        }

        if (Before.IsSet)
        {
            Query.Append("before=").Append(Before.Value).Append("&");
        }

        return Query.LeftChop(1);
    }
};

USTRUCT(BlueprintType)
struct FModifyAssistant : public FCreateAssistant
{
    GENERATED_BODY()
};

///////////////////////////////////////////////////////
//                 RESPONSE TYPES
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FDeleteAssistantResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;  //"assistant.deleted"

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted;
};

/**
  Represents an assistant that can call the model and use tools.
*/
USTRUCT(BlueprintType)
struct FAssistantObjectResponse
{
    GENERATED_BODY()

    /**
      The identifier, which can be referenced in API endpoints.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    /**
      The object type, which is always assistant.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    /**
      The Unix timestamp (in seconds) for when the assistant was created.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At;

    /**
      The name of the assistant. The maximum length is 256 characters.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Name;

    /**
      ID of the model to use.
      You can use the List models API to see all of your available models,
      or see our Model overview for descriptions of them.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Model;

    /**
      The system instructions that the assistant uses. The maximum length is 256,000 characters.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Instructions;

    /**
      A list of tool enabled on the assistant.
      There can be a maximum of 128 tools per assistant.
      Tools can be of types code_interpreter, file_search, or function.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FAssistantTool> Tools;

    /**
      A set of resources that are used by the assistant's tools.
      The resources are specific to the type of tool. For example,
      the code_interpreter tool requires a list of file IDs,
      while the file_search tool requires a list of vector store IDs.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FAssistantToolResources Tool_Resources;

    /**
      Set of 16 key-value pairs that can be attached to an object.
      This can be useful for storing additional information about the object in a structured format.
      Keys can be a maximum of 64 characters long and values can be a maxium of 512 characters long.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TMap<FString, FString> Metadata;

    /**
      What sampling temperature to use, between 0 and 2.
      Higher values like 0.8 will make the output more random,
      while lower values like 0.2 will make it more focused and deterministic.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    float Temperature;

    /**
      An alternative to sampling with temperature, called nucleus sampling,
      where the model considers the results of the tokens with top_p probability mass.
      So 0.1 means only the tokens comprising the top 10% probability mass are considered.
      We generally recommend altering this or temperature but not both.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    float Top_P;

    /**
      Specifies the format that the model must output.
      Compatible with GPT-4o, GPT-4 Turbo, and all GPT-3.5 Turbo models since gpt-3.5-turbo-1106.
    */
    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Response_Format;
};

USTRUCT(BlueprintType)
struct FListAssistantsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FAssistantObjectResponse> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{};
};