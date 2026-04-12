// Copyright (c), Firelight Technologies Pty, Ltd. 2012-2026.

#pragma once

#include "IOpenXRExtensionPlugin.h"
#include "Misc/EngineVersionComparison.h"

class FFMODOpenXRExtension : public IOpenXRExtensionPlugin
{
public:
    FFMODOpenXRExtension();
    ~FFMODOpenXRExtension();

private:
    void DestroyActionSet();

    /** IOpenXRExtensionPlugin implementation */
    virtual void PostCreateInstance(XrInstance InInstance) override;
    virtual void PostCreateSession(XrSession InSession) override;
    bool GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
    virtual void AttachActionSets(TSet<XrActionSet>& OutActionSets) override;
    virtual void GetActiveActionSetsForSync(TArray<XrActiveActionSet>& OutActiveSets) override;
    virtual void BindExtensionPluginDelegates(class IOpenXRExtensionPluginDelegates& OpenXRHMD) override;
    virtual bool GetSuggestedBindings(XrPath InInteractionProfile, TArray<XrActionSuggestedBinding>& OutBindings) override;

    XrInstance Instance = XR_NULL_HANDLE;
    XrSession Session = XR_NULL_HANDLE;
    XrActionSet ActionSet = XR_NULL_HANDLE;
    XrAction Action = XR_NULL_HANDLE;

    bool bConfigured = false;
    IOpenXRExtensionPluginDelegates* Delegate = nullptr;
};