// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Provider/Types/VectorStoreTypes.h"

namespace OpenAI
{
class OPENAI_API VectorStoreParser
{
public:
    static FString CreateVectorStoreToJsonRepresentation(const FCreateVectorStore& CreateVectorStore);
    static FString UpdateVectorStoreToJsonRepresentation(const FUpdateVectorStore& UpdateVectorStore);
    static FString CreateVectorStoreFileBatchToJsonRepresentation(const FCreateVectorStoreFileBatch& CreateBatch);
    static FString SearchVectorStoreToJsonRepresentation(const FVectorStoreSearch& Search);
};
}  // namespace OpenAI
