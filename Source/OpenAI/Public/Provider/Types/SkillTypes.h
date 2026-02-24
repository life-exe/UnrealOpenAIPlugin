// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "SkillTypes.generated.h"

///////////////////////////////////////////////////////
//                 SKILLS
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FSkillResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Default_Version;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Latest_Version;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

using FCreateSkillResponse = FSkillResponse;
using FRetrieveSkillResponse = FSkillResponse;
using FUpdateSkillResponse = FSkillResponse;

USTRUCT(BlueprintType)
struct FCreateSkill
{
    GENERATED_BODY()

    /**
      Skill files to upload (directory upload) or a single zip file.
      We will use this as a path to a file or directory.
    */
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Files;
};

USTRUCT(BlueprintType)
struct FUpdateSkill
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Default_Version;
};

USTRUCT(BlueprintType)
struct FListSkillsParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Limit;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Order;

    FString ToQuery() const
    {
        FString Query{"?"};
        if (After.IsSet) Query.Append("after=").Append(After.Value).Append("&");
        if (Limit.IsSet) Query.Append("limit=").Append(FString::FromInt(Limit.Value)).Append("&");
        if (Order.IsSet) Query.Append("order=").Append(Order.Value).Append("&");
        return Query.LeftChop(1);
    }
};

USTRUCT(BlueprintType)
struct FListSkillsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FSkillResponse> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{false};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

USTRUCT(BlueprintType)
struct FDeleteSkillResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted{false};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

///////////////////////////////////////////////////////
//                 VERSIONS
///////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FSkillVersionResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{0};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Skill_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Version;
};

using FCreateSkillVersionResponse = FSkillVersionResponse;
using FRetrieveSkillVersionResponse = FSkillVersionResponse;

USTRUCT(BlueprintType)
struct FCreateSkillVersion
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Required")
    FString Files;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalBool Default;
};

USTRUCT(BlueprintType)
struct FListSkillVersionsParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalInt Limit;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI | Optional")
    FOptionalString Order;

    FString ToQuery() const
    {
        FString Query{"?"};
        if (After.IsSet) Query.Append("after=").Append(After.Value).Append("&");
        if (Limit.IsSet) Query.Append("limit=").Append(FString::FromInt(Limit.Value)).Append("&");
        if (Order.IsSet) Query.Append("order=").Append(Order.Value).Append("&");
        return Query.LeftChop(1);
    }
};

USTRUCT(BlueprintType)
struct FListSkillVersionsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FSkillVersionResponse> Data;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString First_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Has_More{false};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Last_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;
};

USTRUCT(BlueprintType)
struct FDeleteSkillVersionResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted{false};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Version;
};

USTRUCT(BlueprintType)
struct FSkillContentResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<uint8> Bytes;
};
