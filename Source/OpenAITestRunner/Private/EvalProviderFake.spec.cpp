// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "OpenAIProviderFake.h"
#include "Provider/Types/EvalTypes.h"

DEFINE_SPEC(FEvalProviderFake, "OpenAI.Provider.Eval",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

void FEvalProviderFake::Define()
{
    Describe("Fake",
        [this]()
        {
            It("CreateEvalShouldBeParsedCorrectly",
                [this]()
                {
                    FCreateEvalResponse CreateEvalResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnCreateEvalCompleted().AddLambda(
                        [&](const FCreateEvalResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        { CreateEvalResponse = Response; });

                    const FString ResponseJSON = R"(
                    {
                        "id": "eval-123",
                        "object": "eval",
                        "created_at": 1700000000,
                        "name": "My Eval",
                        "data_source_config": {
                            "type": "custom",
                            "schema": { "type": "object" }
                        },
                        "testing_criteria": [
                            {
                                "type": "label_model",
                                "name": "Grader 1"
                            }
                        ]
                    }
                    )";

                    OpenAIProvider->SetResponse(ResponseJSON);
                    OpenAIProvider->CreateEval(FCreateEval{}, FOpenAIAuth{});

                    TestTrueExpr(CreateEvalResponse.Id.Equals("eval-123"));
                    TestTrueExpr(CreateEvalResponse.Object.Equals("eval"));
                    TestTrueExpr(CreateEvalResponse.Created_At == 1700000000);
                    TestTrueExpr(CreateEvalResponse.Name.Equals("My Eval"));
                    TestTrueExpr(CreateEvalResponse.Data_Source_Config.Type.Equals("custom"));
                    TestTrueExpr(CreateEvalResponse.Testing_Criteria.Num() == 1);
                    TestTrueExpr(CreateEvalResponse.Testing_Criteria[0].Name.Equals("Grader 1"));
                });

            It("ListEvalsShouldBeParsedCorrectly",
                [this]()
                {
                    FListEvalsResponse ListEvalsResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnListEvalsCompleted().AddLambda(
                        [&](const FListEvalsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        { ListEvalsResponse = Response; });

                    const FString ResponseJSON = R"(
                    {
                        "object": "list",
                        "data": [
                            {
                                "id": "eval-1",
                                "object": "eval",
                                "created_at": 1700000001,
                                "name": "Eval 1"
                            },
                            {
                                "id": "eval-2",
                                "object": "eval",
                                "created_at": 1700000002,
                                "name": "Eval 2"
                            }
                        ],
                        "first_id": "eval-1",
                        "last_id": "eval-2",
                        "has_more": false
                    }
                    )";

                    OpenAIProvider->SetResponse(ResponseJSON);
                    OpenAIProvider->ListEvals(FEvalQueryParams{}, FOpenAIAuth{});

                    TestTrueExpr(ListEvalsResponse.Object.Equals("list"));
                    TestTrueExpr(ListEvalsResponse.Data.Num() == 2);
                    TestTrueExpr(ListEvalsResponse.Data[0].Id.Equals("eval-1"));
                    TestTrueExpr(ListEvalsResponse.Data[1].Id.Equals("eval-2"));
                    TestFalseExpr(ListEvalsResponse.Has_More);
                });

            It("RetrieveEvalShouldBeParsedCorrectly",
                [this]()
                {
                    FRetrieveEvalResponse RetrieveEvalResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnRetrieveEvalCompleted().AddLambda(
                        [&](const FRetrieveEvalResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        { RetrieveEvalResponse = Response; });

                    const FString ResponseJSON = R"(
                    {
                        "id": "eval-123",
                        "object": "eval",
                        "created_at": 1700000000,
                        "name": "My Eval"
                    }
                    )";

                    OpenAIProvider->SetResponse(ResponseJSON);
                    OpenAIProvider->RetrieveEval("eval-123", FOpenAIAuth{});

                    TestTrueExpr(RetrieveEvalResponse.Id.Equals("eval-123"));
                    TestTrueExpr(RetrieveEvalResponse.Name.Equals("My Eval"));
                });

            It("UpdateEvalShouldBeParsedCorrectly",
                [this]()
                {
                    FUpdateEvalResponse UpdateEvalResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnUpdateEvalCompleted().AddLambda(
                        [&](const FUpdateEvalResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        { UpdateEvalResponse = Response; });

                    const FString ResponseJSON = R"(
                    {
                        "id": "eval-123",
                        "object": "eval",
                        "created_at": 1700000000,
                        "name": "Updated Eval"
                    }
                    )";

                    OpenAIProvider->SetResponse(ResponseJSON);
                    OpenAIProvider->UpdateEval("eval-123", FUpdateEval{}, FOpenAIAuth{});

                    TestTrueExpr(UpdateEvalResponse.Name.Equals("Updated Eval"));
                });

            It("DeleteEvalShouldBeParsedCorrectly",
                [this]()
                {
                    FDeleteEvalResponse DeleteEvalResponse;
                    auto* OpenAIProvider = NewObject<UOpenAIProviderFake>();
                    OpenAIProvider->OnDeleteEvalCompleted().AddLambda(
                        [&](const FDeleteEvalResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        { DeleteEvalResponse = Response; });

                    const FString ResponseJSON = R"(
                    {
                        "eval_id": "eval-123",
                        "object": "eval.deleted",
                        "deleted": true
                    }
                    )";

                    OpenAIProvider->SetResponse(ResponseJSON);
                    OpenAIProvider->DeleteEval("eval-123", FOpenAIAuth{});

                    TestTrueExpr(DeleteEvalResponse.Eval_Id.Equals("eval-123"));
                    TestTrueExpr(DeleteEvalResponse.Deleted);
                });
        });
}

#endif
