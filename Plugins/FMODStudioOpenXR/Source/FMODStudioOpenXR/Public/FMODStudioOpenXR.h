// Copyright (c), Firelight Technologies Pty, Ltd. 2012-2026.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FFMODOpenXRExtension;

class FFMODStudioOpenXRModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    TUniquePtr<FFMODOpenXRExtension> FMODOpenXRExtension;
};
