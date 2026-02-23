// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/OpenAIOptional.h"
#include "EvalTypes.generated.h"

USTRUCT(BlueprintType)
struct FEvalDataSourceConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type;

    // Custom
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Item_Schema;  // JSON string

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FOptionalBool Include_Sample_Schema;

    // Logs / Stored_Completions
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    TMap<FString, FString> Metadata;
};

USTRUCT(BlueprintType)
struct FEvalMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Content;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Role;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type{"message"};
};

USTRUCT(BlueprintType)
struct FEvalGrader
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Type;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Name;

    // LabelModelGrader
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    TArray<FEvalMessage> Input;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    TArray<FString> Labels;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Model;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    TArray<FString> Passing_Labels;

    // StringCheckGrader
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Input_String;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Operation;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Reference;

    // TextSimilarityGrader / PythonGrader / ScoreModelGrader
    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FOptionalFloat Pass_Threshold;
};

USTRUCT(BlueprintType)
struct FCreateEval
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FEvalDataSourceConfig Data_Source_Config;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    TArray<FEvalGrader> Testing_Criteria;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FString Name;
};

USTRUCT(BlueprintType)
struct FEval
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    int32 Created_At{};

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FEvalDataSourceConfig Data_Source_Config;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TMap<FString, FString> Metadata;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FEvalGrader> Testing_Criteria;
};

USTRUCT(BlueprintType)
struct FCreateEvalResponse : public FEval
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FListEvalsResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    TArray<FEval> Data;

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
struct FRetrieveEvalResponse : public FEval
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FUpdateEvalResponse : public FEval
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FEvalQueryParams
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FOptionalString After;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FOptionalInt Limit;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FOptionalString Order;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FOptionalString Order_By;

    FString ToQuery() const
    {
        TArray<FString> Params;
        if (After.IsSet) Params.Add(FString::Format(TEXT("after={0}"), {After.Value}));
        if (Limit.IsSet) Params.Add(FString::Format(TEXT("limit={0}"), {FString::FromInt(Limit.Value)}));
        if (Order.IsSet) Params.Add(FString::Format(TEXT("order={0}"), {Order.Value}));
        if (Order_By.IsSet) Params.Add(FString::Format(TEXT("order_by={0}"), {Order_By.Value}));
        return Params.Num() > 0 ? "?" + FString::Join(Params, TEXT("&")) : "";
    }
};

USTRUCT(BlueprintType)
struct FUpdateEval
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    FOptionalString Name;

    UPROPERTY(BlueprintReadWrite, Category = "OpenAI")
    TMap<FString, FString> Metadata;
};

USTRUCT(BlueprintType)
struct FDeleteEvalResponse
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Eval_Id;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    FString Object;

    UPROPERTY(BlueprintReadOnly, Category = "OpenAI")
    bool Deleted{false};
};
