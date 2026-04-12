// Copyright (c), Firelight Technologies Pty, Ltd. 2012-2026.

#include "FMODStudioOpenXR.h"
#include "FMODOpenXRExtension.h"
#include "Misc/Paths.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FFMODStudioOpenXRModule"

void FFMODStudioOpenXRModule::StartupModule()
{
    IPluginManager::Get().FindPlugin(TEXT("FMODStudioOpenXR"))->GetBaseDir();
    FMODOpenXRExtension.Reset(new FFMODOpenXRExtension());
}

void FFMODStudioOpenXRModule::ShutdownModule()
{
    FMODOpenXRExtension.Reset();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FFMODStudioOpenXRModule, FMODStudioOpenXR)