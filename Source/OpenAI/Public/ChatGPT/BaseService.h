// OpenAI, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Dom/JsonObject.h"
#include "Provider/Types/Chat/ChatCommonTypes.h"
#include "BaseService.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnServiceDataRecieved, const FMessage&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnServiceDataError, const FString&);

UCLASS()
class OPENAI_API UBaseService : public UObject
{
    GENERATED_BODY()

public:
    /*
      You can check some necessary conditions to enable the service, if there is no such condition just always return true.
    */
    virtual bool Init(const OpenAI::ServiceSecrets& Secrets);

    /*
      In most cases you won't need to override this function.
    */
    virtual FFunctionRequest Function() const;

    /*
      Description of your function for the ChatGPT.
    */
    virtual FString Description() const;

    /*
      Name of your function that ChatGPT will call.
    */
    virtual FString FunctionName() const;

    /*
      Actual call of your function.
    */
    virtual void Call(const TSharedPtr<FJsonObject>& Args, const FString& ToolID);

    /*
      Don't create long names. They will be truncated to [NameMaxLength = 8] in the UI. Use Description for more words.
    */
    virtual FString Name() const;

    /*
      Comes to UI tooltip, may be empty.
    */
    virtual FString TooltipDescription() const;

    FOnServiceDataRecieved& OnServiceDataRecieved();
    FOnServiceDataError& OnServiceDataError();

protected:
    /*
      Call this delegate with a data that returns your function.
    */
    FOnServiceDataRecieved ServiceDataRecieved;

    /*
      Call this delegate when an error occurs during your function calculations.
    */
    FOnServiceDataError ServiceDataError;

    FString ToolID{};

    /*
      Return a JSON with parameters that your function has.

      Please use MakeFunctionsString to wrap your JSON object at the end:

      TSharedPtr<FJsonObject> ParamsObj = MakeShareable(new FJsonObject());
      ...
      return UOpenAIFuncLib::MakeFunctionsString(ParamsObj);
    */
    virtual FString MakeFunction() const;

    /*
      Helper function that can be used in the child classes.
    */
    virtual FMessage MakeMessage(const FString& Content) const;
};