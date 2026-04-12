// Copyright (c), Firelight Technologies Pty, Ltd. 2012-2026.

#include "FMODOpenXRExtension.h"
#include "FMODStudioOpenXR.h"
#include "Features/IModularFeatures.h"
#include "FMODBlueprintStatics.h"
#include <fmod_studio.hpp>
#include <OpenXRCore.h>
#include <vector>

DEFINE_LOG_CATEGORY_STATIC(LogFMODStudioOpenXR, Log, All);

FMOD_RESULT(*so_FMOD_Haptics_OpenXrFocused)(void* session, void* instance, void* action);

FFMODOpenXRExtension::FFMODOpenXRExtension()
{
    RegisterOpenXRExtensionModularFeature();
}

FFMODOpenXRExtension::~FFMODOpenXRExtension()
{
    DestroyActionSet();
}

void FFMODOpenXRExtension::DestroyActionSet()
{
    if (ActionSet != XR_NULL_HANDLE)
    {
        xrDestroyActionSet(ActionSet);
        ActionSet = XR_NULL_HANDLE;
    }
    Session = XR_NULL_HANDLE;
}

void FFMODOpenXRExtension::PostCreateInstance(XrInstance InInstance)
{
    UE_LOG(LogFMODStudioOpenXR, Log, TEXT("OpenXR instance set - OpenXR plugin is enabled and loaded"));

    ensure(InInstance != XR_NULL_HANDLE);
    Instance = InInstance;
}

bool FFMODOpenXRExtension::GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
    OutExtensions.Add(XR_FB_HAPTIC_PCM_EXTENSION_NAME);

    return true;
}

void FFMODOpenXRExtension::PostCreateSession(XrSession InSession)
{
    UE_LOG(LogFMODStudioOpenXR, Log, TEXT("OpenXR session started"));

    ensure(InSession != XR_NULL_HANDLE);
    Session = InSession;

    XrActionSetCreateInfo info = { .type = XR_TYPE_ACTION_SET_CREATE_INFO };
    FCStringAnsi::Strcpy(info.actionSetName, XR_MAX_ACTION_SET_NAME_SIZE, "fmodinteractioncontext");
    FCStringAnsi::Strcpy(info.localizedActionSetName, XR_MAX_ACTION_SET_NAME_SIZE, "FMOD Interaction Context");

    xrCreateActionSet(Instance, &info, &ActionSet);
    XrActionCreateInfo aci = { XR_TYPE_ACTION_CREATE_INFO };
    aci.actionType = XR_ACTION_TYPE_VIBRATION_OUTPUT;
    FCStringAnsi::Strcpy(aci.actionName, XR_MAX_ACTION_NAME_SIZE, "fmodaction");
    FCStringAnsi::Strcpy(aci.localizedActionName, XR_MAX_ACTION_NAME_SIZE, "haptic_action");
    xrCreateAction(ActionSet, &aci, &Action);

    void* Lib = FPlatformProcess::GetDllHandle(TEXT("libfmod_haptics.so"));
    if (Lib != NULL)
    {
        so_FMOD_Haptics_OpenXrFocused = (FMOD_RESULT(*)(void*, void*, void*))FPlatformProcess::GetDllExport(Lib, TEXT("FMOD_Haptics_OpenXrFocused"));

        if (Session && Instance && Action && so_FMOD_Haptics_OpenXrFocused)
        {
            so_FMOD_Haptics_OpenXrFocused((void*)Session, (void*)Instance, (void*)Action);
            UE_LOG(LogFMODStudioOpenXR, Log, TEXT("so_FMOD_Haptics_OpenXrFocused called with XrSession: 0x%X XrInstance: 0x%X XrAction: 0x%X"), Session, Instance, Action);
        }
        else
        {
            UE_LOG(LogFMODStudioOpenXR, Warning, TEXT("Failed to call FMOD_Haptics_OpenXrFocused.  XrSession: 0x%X0 XrInstance: 0x%X XrAction: 0x%X so_FMOD_Haptics_OpenXrFocused: 0x%x"), Session, Instance, Action, so_FMOD_Haptics_OpenXrFocused);
        }
    }
}

void FFMODOpenXRExtension::AttachActionSets(TSet<XrActionSet>& OutActionSets)
{
    if (ActionSet != XR_NULL_HANDLE)
    {
        OutActionSets.Add(ActionSet);
    }
}

void FFMODOpenXRExtension::GetActiveActionSetsForSync(TArray<XrActiveActionSet>& OutActiveSets)
{
    if (ActionSet != XR_NULL_HANDLE)
    {
        const XrActiveActionSet activeActionSet{ ActionSet, XR_NULL_PATH };
        OutActiveSets.Push(activeActionSet);
    }
}

void FFMODOpenXRExtension::BindExtensionPluginDelegates(IOpenXRExtensionPluginDelegates& OpenXRHMD)
{
    UE_LOG(LogFMODStudioOpenXR, Log, TEXT("Extension plugin delegate set - OpenXR plugin is used as the HMD module"));
    Delegate = &OpenXRHMD;
}

bool FFMODOpenXRExtension::GetSuggestedBindings(XrPath InInteractionProfile, TArray<XrActionSuggestedBinding>& OutBindings)
{
    XrPath touchInteractionProfile = XR_NULL_PATH;
    XrPath leftHand = XR_NULL_PATH;
    XrPath rightHand = XR_NULL_PATH;
    xrStringToPath(Instance, "/interaction_profiles/oculus/touch_controller", &touchInteractionProfile);
    xrStringToPath(Instance, "/user/hand/left/output/haptic", &leftHand);
    xrStringToPath(Instance, "/user/hand/right/output/haptic", &rightHand);
    OutBindings.Add({ Action, leftHand });
    OutBindings.Add({ Action, rightHand });

    return true;
}