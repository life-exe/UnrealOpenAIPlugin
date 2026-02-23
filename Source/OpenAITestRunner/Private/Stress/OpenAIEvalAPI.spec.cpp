// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/EvalTypes.h"
#include "Provider/Types/OpenAICommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"
#include "Algo/AnyOf.h"

BEGIN_DEFINE_SPEC(FOpenAIEvals, "OpenAI.Provider",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
FString EvalId;
TArray<FString> EvalIds;
END_DEFINE_SPEC(FOpenAIEvals)

using namespace OpenAI::Tests;

void FOpenAIEvals::Define()
{
    Describe("ActualAPI",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    Auth = UOpenAIFuncLib::LoadAPITokensFromFile(FPaths::ProjectDir().Append("OpenAIAuth.ini"));
                    OpenAIProvider = NewObject<UOpenAIProvider>();
                    OpenAIProvider->SetLogEnabled(true);
                    OpenAIProvider->OnRequestError().AddLambda(
                        [&](const FString& URL, const FString& Content)
                        {
                            RequestCompleted = true;
                            AddError(FString::Format(TEXT("URL: {0} Content: {1}"), {URL, Content}));
                        });
                    EvalIds.Empty();
                    RequestCompleted = false;
                });

            It("Evals.CreateDeleteEvalRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Name = TEXT("Test Eval ") + FGuid::NewGuid().ToString();
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_Mini);

                    OpenAIProvider->OnCreateEvalCompleted().AddLambda(
                        [&, Name](const FCreateEvalResponse& CreateEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(CreateEvalResponse.Name.Equals(Name));
                            TestTrueExpr(!CreateEvalResponse.Id.IsEmpty());
                            EvalId = CreateEvalResponse.Id;

                            // 2. delete eval
                            OpenAIProvider->DeleteEval(EvalId, Auth);
                        });

                    OpenAIProvider->OnDeleteEvalCompleted().AddLambda(
                        [&](const FDeleteEvalResponse& DeleteEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(DeleteEvalResponse.Eval_Id.Equals(EvalId));
                            TestTrueExpr(DeleteEvalResponse.Deleted);
                            RequestCompleted = true;
                        });

                    FCreateEval CreateEval;
                    CreateEval.Name = Name;
                    CreateEval.Data_Source_Config.Type = TEXT("custom");
                    CreateEval.Data_Source_Config.Item_Schema =
                        TEXT("{\"type\": \"object\", \"properties\": {\"input\": {\"type\": \"string\"}}}");

                    FEvalGrader Grader;
                    Grader.Type = TEXT("label_model");
                    Grader.Name = TEXT("Test Grader");
                    FEvalMessage Message;
                    Message.Role = TEXT("user");
                    Message.Content = TEXT("Is this {{item.input}} positive?");
                    Grader.Input.Add(Message);
                    Grader.Labels = {TEXT("yes"), TEXT("no")};
                    Grader.Model = Model;
                    Grader.Passing_Labels = {TEXT("yes")};
                    CreateEval.Testing_Criteria.Add(Grader);

                    // 1. create eval
                    OpenAIProvider->CreateEval(CreateEval, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Evals.RetrieveEvalRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Name = TEXT("Test Eval ") + FGuid::NewGuid().ToString();
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_Mini);

                    OpenAIProvider->OnCreateEvalCompleted().AddLambda(
                        [&, Name](const FCreateEvalResponse& CreateEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(CreateEvalResponse.Name.Equals(Name));
                            EvalId = CreateEvalResponse.Id;

                            // 2. retrieve eval
                            OpenAIProvider->RetrieveEval(EvalId, Auth);
                        });

                    OpenAIProvider->OnRetrieveEvalCompleted().AddLambda(
                        [&, Name](const FRetrieveEvalResponse& RetrieveEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(RetrieveEvalResponse.Name.Equals(Name));
                            TestTrueExpr(RetrieveEvalResponse.Id.Equals(EvalId));

                            // 3. delete eval
                            OpenAIProvider->DeleteEval(EvalId, Auth);
                        });

                    OpenAIProvider->OnDeleteEvalCompleted().AddLambda(
                        [&](const FDeleteEvalResponse& DeleteEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(DeleteEvalResponse.Eval_Id.Equals(EvalId));
                            TestTrueExpr(DeleteEvalResponse.Deleted);
                            RequestCompleted = true;
                        });

                    FCreateEval CreateEval;
                    CreateEval.Name = Name;
                    CreateEval.Data_Source_Config.Type = TEXT("custom");
                    CreateEval.Data_Source_Config.Item_Schema =
                        TEXT("{\"type\": \"object\", \"properties\": {\"input\": {\"type\": \"string\"}}}");

                    FEvalGrader Grader;
                    Grader.Type = TEXT("label_model");
                    Grader.Name = TEXT("Test Grader");
                    FEvalMessage Message;
                    Message.Role = TEXT("user");
                    Message.Content = TEXT("Is this {{item.input}} positive?");
                    Grader.Input.Add(Message);
                    Grader.Labels = {TEXT("yes"), TEXT("no")};
                    Grader.Model = Model;
                    Grader.Passing_Labels = {TEXT("yes")};
                    CreateEval.Testing_Criteria.Add(Grader);

                    // 1. create eval
                    OpenAIProvider->CreateEval(CreateEval, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Evals.ListEvalsRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Name = TEXT("Test Eval ") + FGuid::NewGuid().ToString();
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_Mini);

                    OpenAIProvider->OnCreateEvalCompleted().AddLambda(
                        [&, Name](const FCreateEvalResponse& CreateEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            EvalId = CreateEvalResponse.Id;
                            EvalIds.Add(EvalId);
                            if (EvalIds.Num() == 1)
                            {
                                // 1b. create eval 2 after eval 1 is confirmed
                                FCreateEval CreateEval2;
                                CreateEval2.Name = Name + TEXT("_2");
                                CreateEval2.Data_Source_Config.Type = TEXT("custom");
                                CreateEval2.Data_Source_Config.Item_Schema =
                                    TEXT("{\"type\": \"object\", \"properties\": {\"input\": {\"type\": \"string\"}}}");
                                FEvalGrader Grader2;
                                Grader2.Type = TEXT("label_model");
                                Grader2.Name = TEXT("Test Grader");
                                FEvalMessage Message2;
                                Message2.Role = TEXT("user");
                                Message2.Content = TEXT("Is this {{item.input}} positive?");
                                Grader2.Input.Add(Message2);
                                Grader2.Labels = {TEXT("yes"), TEXT("no")};
                                Grader2.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_Mini);
                                Grader2.Passing_Labels = {TEXT("yes")};
                                CreateEval2.Testing_Criteria.Add(Grader2);
                                OpenAIProvider->CreateEval(CreateEval2, Auth);
                            }
                            else if (EvalIds.Num() == 2)
                            {
                                // 2. list evals
                                OpenAIProvider->ListEvals(FEvalQueryParams{}, Auth);
                            }
                        });

                    OpenAIProvider->OnListEvalsCompleted().AddLambda(
                        [&](const FListEvalsResponse& ListEvalsResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(ListEvalsResponse.Data.Num() >= 2);
                            for (const auto& Id : EvalIds)
                            {
                                const bool Contains =
                                    Algo::AnyOf(ListEvalsResponse.Data, [&Id](const auto& Elem) { return Elem.Id.Equals(Id); });
                                TestTrueExpr(Contains);
                            }

                            // 3. delete eval 1
                            OpenAIProvider->DeleteEval(EvalIds[0], Auth);
                        });

                    OpenAIProvider->OnDeleteEvalCompleted().AddLambda(
                        [&](const FDeleteEvalResponse& DeleteEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            EvalIds.Remove(DeleteEvalResponse.Eval_Id);
                            if (EvalIds.IsEmpty())
                            {
                                RequestCompleted = true;
                            }
                            else
                            {
                                // 3. delete eval 2
                                OpenAIProvider->DeleteEval(EvalIds[0], Auth);
                            }
                        });

                    FCreateEval CreateEval;
                    CreateEval.Name = Name;
                    CreateEval.Data_Source_Config.Type = TEXT("custom");
                    CreateEval.Data_Source_Config.Item_Schema =
                        TEXT("{\"type\": \"object\", \"properties\": {\"input\": {\"type\": \"string\"}}}");

                    FEvalGrader Grader;
                    Grader.Type = TEXT("label_model");
                    Grader.Name = TEXT("Test Grader");
                    FEvalMessage Message;
                    Message.Role = TEXT("user");
                    Message.Content = TEXT("Is this {{item.input}} positive?");
                    Grader.Input.Add(Message);
                    Grader.Labels = {TEXT("yes"), TEXT("no")};
                    Grader.Model = Model;
                    Grader.Passing_Labels = {TEXT("yes")};
                    CreateEval.Testing_Criteria.Add(Grader);

                    // 1. create eval 1 (eval 2 is created sequentially inside OnCreateEvalCompleted)
                    OpenAIProvider->CreateEval(CreateEval, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Evals.UpdateEvalRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Name = TEXT("Test Eval ") + FGuid::NewGuid().ToString();
                    const FString UpdatedName = Name + TEXT(" Updated");
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O_Mini);

                    OpenAIProvider->OnCreateEvalCompleted().AddLambda(
                        [&, UpdatedName](const FCreateEvalResponse& CreateEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            EvalId = CreateEvalResponse.Id;

                            // 2. update eval
                            FUpdateEval UpdateEval;
                            UpdateEval.Name.IsSet = true;
                            UpdateEval.Name.Value = UpdatedName;
                            OpenAIProvider->UpdateEval(EvalId, UpdateEval, Auth);
                        });

                    OpenAIProvider->OnUpdateEvalCompleted().AddLambda(
                        [&, UpdatedName](const FUpdateEvalResponse& UpdateEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(UpdateEvalResponse.Name.Equals(UpdatedName));
                            TestTrueExpr(UpdateEvalResponse.Id.Equals(EvalId));

                            // 3. delete eval
                            OpenAIProvider->DeleteEval(EvalId, Auth);
                        });

                    OpenAIProvider->OnDeleteEvalCompleted().AddLambda(
                        [&](const FDeleteEvalResponse& DeleteEvalResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(DeleteEvalResponse.Eval_Id.Equals(EvalId));
                            TestTrueExpr(DeleteEvalResponse.Deleted);
                            RequestCompleted = true;
                        });

                    FCreateEval CreateEval;
                    CreateEval.Name = Name;
                    CreateEval.Data_Source_Config.Type = TEXT("custom");
                    CreateEval.Data_Source_Config.Item_Schema =
                        TEXT("{\"type\": \"object\", \"properties\": {\"input\": {\"type\": \"string\"}}}");

                    FEvalGrader Grader;
                    Grader.Type = TEXT("label_model");
                    Grader.Name = TEXT("Test Grader");
                    FEvalMessage Message;
                    Message.Role = TEXT("user");
                    Message.Content = TEXT("Is this {{item.input}} positive?");
                    Grader.Input.Add(Message);
                    Grader.Labels = {TEXT("yes"), TEXT("no")};
                    Grader.Model = Model;
                    Grader.Passing_Labels = {TEXT("yes")};
                    CreateEval.Testing_Criteria.Add(Grader);

                    // 1. create eval
                    OpenAIProvider->CreateEval(CreateEval, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif
