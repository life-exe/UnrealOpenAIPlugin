// OpenAI, Copyright LifeEXE. All Rights Reserved.

/*
 ___       ___  ________ _______
|\  \     |\  \|\  _____\\  ___ \
\ \  \    \ \  \ \  \__/\ \   __/|
 \ \  \    \ \  \ \   __\\ \  \_|/__
  \ \  \____\ \  \ \  \_| \ \  \_|\ \
   \ \_______\ \__\ \__\   \ \_______\
    \|_______|\|__|\|__|    \|_______|



 _______      ___    ___ _______
|\  ___ \    |\  \  /  /|\  ___ \
\ \   __/|   \ \  \/  / | \   __/|
 \ \  \_|/__  \ \    / / \ \  \_|/__
  \ \  \_|\ \  /     \/   \ \  \_|\ \
   \ \_______\/  /\   \    \ \_______\
    \|_______/__/ /\ __\    \|_______|
             |__|/ \|__|
*/

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FOpenAITestRunnerModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
