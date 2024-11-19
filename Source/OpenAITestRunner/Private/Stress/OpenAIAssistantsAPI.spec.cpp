// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/AssistantTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"
#include "Algo/AnyOf.h"

BEGIN_DEFINE_SPEC(FOpenAIAssistants, "OpenAI.Provider",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
FString AssistantId;
TArray<FString> AssistantIds;
END_DEFINE_SPEC(FOpenAIAssistants)

using namespace OpenAI::Tests;

void FOpenAIAssistants::Define()
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
                    AssistantIds.Empty();
                    RequestCompleted = false;
                });

            It("Assistants.CreateDeleteAssistantRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O);
                    const FString Name = "Math Tutor";
                    const FString Instructions =
                        "You are a personal math tutor. When asked a question, write and run Python code to answer the question.";
                    const FString ToolType = UOpenAIFuncLib::OpenAIAssistantToolTypeToString(EAssistantToolType::CodeInterpreter);

                    OpenAIProvider->OnCreateAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FAssistantObjectResponse& AssistantObjectResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(AssistantObjectResponse.Model.Equals(Model));
                            TestTrueExpr(AssistantObjectResponse.Instructions.Equals(Instructions));
                            TestTrueExpr(AssistantObjectResponse.Name.Equals(Name));
                            AssistantId = AssistantObjectResponse.Id;

                            // 2. delete assistant
                            OpenAIProvider->DeleteAssistant(AssistantId, Auth);
                        });

                    OpenAIProvider->OnDeleteAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FDeleteAssistantResponse& DeleteAssistantResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(DeleteAssistantResponse.Id.Equals(AssistantId));
                            TestTrueExpr(DeleteAssistantResponse.Deleted);
                            RequestCompleted = true;
                        });

                    FCreateAssistant CreateAssistant;
                    CreateAssistant.Model = Model;
                    CreateAssistant.Instructions = Instructions;
                    CreateAssistant.Name = Name;

                    FAssistantTool AssistantTool;
                    AssistantTool.Type = ToolType;
                    CreateAssistant.Tools.Add(AssistantTool);

                    // 1. create assistant
                    OpenAIProvider->CreateAssistant(CreateAssistant, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Assistants.RetrieveAssistantRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O);
                    const FString Name = "Math Tutor";
                    const FString Instructions =
                        "You are a personal math tutor. When asked a question, write and run Python code to answer the question.";
                    const FString ToolType = UOpenAIFuncLib::OpenAIAssistantToolTypeToString(EAssistantToolType::CodeInterpreter);

                    OpenAIProvider->OnCreateAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FAssistantObjectResponse& AssistantObjectResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(AssistantObjectResponse.Model.Equals(Model));
                            TestTrueExpr(AssistantObjectResponse.Instructions.Equals(Instructions));
                            TestTrueExpr(AssistantObjectResponse.Name.Equals(Name));
                            AssistantId = AssistantObjectResponse.Id;

                            // 2. retrieve assistant
                            OpenAIProvider->RetrieveAssistant(AssistantId, Auth);
                        });

                    OpenAIProvider->OnRetrieveAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FAssistantObjectResponse& AssistantObjectResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(AssistantObjectResponse.Model.Equals(Model));
                            TestTrueExpr(AssistantObjectResponse.Instructions.Equals(Instructions));
                            TestTrueExpr(AssistantObjectResponse.Name.Equals(Name));
                            TestTrueExpr(AssistantObjectResponse.Id.Equals(AssistantId));

                            // 3. delete assistant
                            OpenAIProvider->DeleteAssistant(AssistantId, Auth);
                        });

                    OpenAIProvider->OnDeleteAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FDeleteAssistantResponse& DeleteAssistantResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(DeleteAssistantResponse.Id.Equals(AssistantId));
                            TestTrueExpr(DeleteAssistantResponse.Deleted);
                            RequestCompleted = true;
                        });

                    FCreateAssistant CreateAssistant;
                    CreateAssistant.Model = Model;
                    CreateAssistant.Instructions = Instructions;
                    CreateAssistant.Name = Name;

                    FAssistantTool AssistantTool;
                    AssistantTool.Type = ToolType;
                    CreateAssistant.Tools.Add(AssistantTool);

                    // 1. create assistant
                    OpenAIProvider->CreateAssistant(CreateAssistant, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Assistants.ListAssistantsRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O);
                    const FString Name = "Math Tutor";
                    const FString Instructions =
                        "You are a personal math tutor. When asked a question, write and run Python code to answer the question.";
                    const FString ToolType = UOpenAIFuncLib::OpenAIAssistantToolTypeToString(EAssistantToolType::CodeInterpreter);

                    OpenAIProvider->OnCreateAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FAssistantObjectResponse& AssistantObjectResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(AssistantObjectResponse.Model.Equals(Model));
                            TestTrueExpr(AssistantObjectResponse.Instructions.Equals(Instructions));
                            TestTrueExpr(AssistantObjectResponse.Name.Equals(Name));
                            AssistantId = AssistantObjectResponse.Id;
                            AssistantIds.Add(AssistantId);
                            AddInfo(FString::Format(TEXT("Assistant created: id={0}"), {AssistantId}));
                            if (AssistantIds.Num() == 2)
                            {
                                AddInfo(FString::Format(TEXT("All assistants created: count={0}"), {AssistantIds.Num()}));
                                // 2. list assistants
                                OpenAIProvider->ListAssistants(FListAssistants{}, Auth);
                            }
                        });

                    OpenAIProvider->OnListAssistantsCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FListAssistantsResponse& ListAssistantsResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            AddInfo(TEXT("List assistants completed"));

                            TestTrueExpr(ListAssistantsResponse.Data.Num() >= 2);
                            for (const auto& Id : AssistantIds)
                            {
                                const bool Contains =
                                    Algo::AnyOf(ListAssistantsResponse.Data, [&Id](const auto& Elem) { return Elem.Id.Equals(Id); });
                                TestTrueExpr(Contains);
                            }

                            // 3. delete assistant 1
                            OpenAIProvider->DeleteAssistant(AssistantIds[0], Auth);
                        });

                    OpenAIProvider->OnDeleteAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FDeleteAssistantResponse& DeleteAssistantResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            AddInfo(FString::Format(TEXT("Assistant deleted: id={0}"), {DeleteAssistantResponse.Id}));

                            AssistantIds.Remove(DeleteAssistantResponse.Id);
                            if (AssistantIds.IsEmpty())
                            {
                                RequestCompleted = true;
                            }
                            else
                            {
                                // 3. delete assistant 2
                                OpenAIProvider->DeleteAssistant(AssistantIds[0], Auth);
                            }
                        });

                    FCreateAssistant CreateAssistant;
                    CreateAssistant.Model = Model;
                    CreateAssistant.Instructions = Instructions;
                    CreateAssistant.Name = Name;

                    FAssistantTool AssistantTool;
                    AssistantTool.Type = ToolType;
                    CreateAssistant.Tools.Add(AssistantTool);

                    // 1. create assistant 1
                    OpenAIProvider->CreateAssistant(CreateAssistant, Auth);
                    // 1. create assistant 2
                    OpenAIProvider->CreateAssistant(CreateAssistant, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Assistants.ModifyAssistantRequestShouldResponseCorrectly",
                [this]()
                {
                    const FString Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::GPT_4O);
                    const FString Name = "Math Tutor";
                    const FString Instructions =
                        "You are a personal math tutor. When asked a question, write and run Python code to answer the question.";
                    const FString ToolType1 = UOpenAIFuncLib::OpenAIAssistantToolTypeToString(EAssistantToolType::CodeInterpreter);
                    const FString ToolType2 = UOpenAIFuncLib::OpenAIAssistantToolTypeToString(EAssistantToolType::FileSearch);

                    FCreateAssistant CreateAssistant;
                    CreateAssistant.Model = Model;
                    CreateAssistant.Instructions = Instructions;
                    CreateAssistant.Name = Name;

                    FAssistantTool AssistantTool;
                    AssistantTool.Type = ToolType1;
                    CreateAssistant.Tools.Add(AssistantTool);

                    OpenAIProvider->OnCreateAssistantCompleted().AddLambda(
                        [&, CreateAssistant, Model, Instructions, Name, ToolType1, ToolType2](
                            const FAssistantObjectResponse& AssistantObjectResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(AssistantObjectResponse.Model.Equals(Model));
                            TestTrueExpr(AssistantObjectResponse.Instructions.Equals(Instructions));
                            TestTrueExpr(AssistantObjectResponse.Name.Equals(Name));
                            TestTrueExpr(AssistantObjectResponse.Tools[0].Type.Equals(ToolType1));
                            AssistantId = AssistantObjectResponse.Id;

                            // 2. modify assistant
                            FModifyAssistant ModifyAssistant{CreateAssistant};
                            FAssistantTool AssistantTool;
                            AssistantTool.Type = ToolType2;
                            ModifyAssistant.Tools.Empty();
                            ModifyAssistant.Tools.Add(AssistantTool);
                            OpenAIProvider->ModifyAssistant(AssistantId, ModifyAssistant, Auth);
                        });

                    OpenAIProvider->OnModifyAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name, ToolType2](
                            const FAssistantObjectResponse& AssistantObjectResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(AssistantObjectResponse.Model.Equals(Model));
                            TestTrueExpr(AssistantObjectResponse.Instructions.Equals(Instructions));
                            TestTrueExpr(AssistantObjectResponse.Name.Equals(Name));
                            TestTrueExpr(AssistantObjectResponse.Tools[0].Type.Equals(ToolType2));

                            // 3. delete assistant
                            OpenAIProvider->DeleteAssistant(AssistantId, Auth);
                        });

                    OpenAIProvider->OnDeleteAssistantCompleted().AddLambda(
                        [&, Model, Instructions, Name](
                            const FDeleteAssistantResponse& DeleteAssistantResponse, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(DeleteAssistantResponse.Id.Equals(AssistantId));
                            TestTrueExpr(DeleteAssistantResponse.Deleted);
                            RequestCompleted = true;
                        });

                    // 1. create assistant
                    OpenAIProvider->CreateAssistant(CreateAssistant, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif
