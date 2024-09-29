// OpenAI, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Provider/Types/EmbeddingTypes.h"
#include "Provider/Types/CommonTypes.h"
#include "Provider/OpenAIProvider.h"
#include "TestUtils.h"

BEGIN_DEFINE_SPEC(FOpenAIProviderEmbedding, "OpenAI.Provider",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::HighPriority)
TObjectPtr<UOpenAIProvider> OpenAIProvider;
FOpenAIAuth Auth;
bool RequestCompleted{false};
END_DEFINE_SPEC(FOpenAIProviderEmbedding)

using namespace OpenAI::Tests;

void FOpenAIProviderEmbedding::Define()
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
                    RequestCompleted = false;
                });

            It("Embedding.CreateEmbeddingsRequestShouldResponseCorrectly",
                [this]()
                {
                    OpenAIProvider->OnCreateEmbeddingsCompleted().AddLambda(
                        [&](const FEmbeddingsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Model.Equals("text-embedding-ada-002"));
                            TestTrueExpr(Response.Usage.Prompt_Tokens == 6);
                            TestTrueExpr(Response.Usage.Total_Tokens == 6);
                            TestTrueExpr(Response.Object.Equals("list"));
                            TestTrueExpr(Response.Data.Num() == 1);

                            for (const auto& Data : Response.Data)
                            {
                                TestTrueExpr(Data.Object.Equals("embedding"));
                                TestTrueExpr(Data.Index == 0);
                                TestTrueExpr(Data.Embedding.Num() > 0);
                            }

                            RequestCompleted = true;
                        });

                    FEmbeddings Embeddings;
                    Embeddings.Input = {"Hello! How are you?"};
                    Embeddings.Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_Ada_002);
                    Embeddings.Encoding_Format = UOpenAIFuncLib::OpenAIEmbeddingsEncodingFormatToString(EEmbeddingsEncodingFormat::Float);
                    OpenAIProvider->CreateEmbeddings(Embeddings, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });

            It("Embedding.CreateEmbeddingsRequestWithOptionalFieldsShouldResponseCorrectly",
                [this]()
                {
                    const auto Model = UOpenAIFuncLib::OpenAIAllModelToString(EAllModelEnum::Text_Embedding_3_Small);
                    OpenAIProvider->OnCreateEmbeddingsCompleted().AddLambda(
                        [&, SaveModel = Model](const FEmbeddingsResponse& Response, const FOpenAIResponseMetadata& ResponseMetadata)
                        {
                            TestTrueExpr(Response.Model.Equals(SaveModel));
                            TestTrueExpr(Response.Usage.Prompt_Tokens == 6);
                            TestTrueExpr(Response.Usage.Total_Tokens == 6);
                            TestTrueExpr(Response.Object.Equals("list"));
                            TestTrueExpr(Response.Data.Num() == 1);

                            for (const auto& Data : Response.Data)
                            {
                                TestTrueExpr(Data.Object.Equals("embedding"));
                                TestTrueExpr(Data.Index == 0);
                                TestTrueExpr(Data.Embedding.Num() > 0);
                            }

                            RequestCompleted = true;
                        });

                    FEmbeddings Embeddings;
                    Embeddings.Input = {"Hello! How are you?"};
                    Embeddings.Model = Model;
                    Embeddings.Encoding_Format = UOpenAIFuncLib::OpenAIEmbeddingsEncodingFormatToString(EEmbeddingsEncodingFormat::Float);
                    Embeddings.User.Set("John Doe");
                    Embeddings.Dimensions.Set(2);
                    OpenAIProvider->CreateEmbeddings(Embeddings, Auth);

                    ADD_LATENT_AUTOMATION_COMMAND(FWaitForRequestCompleted(RequestCompleted));
                });
        });
}

#endif
