
#pragma once

namespace TimeDistortion
{
    // System Component TypeIds
    inline constexpr const char* TimeDistortionSystemComponentTypeId = "{4DDDE50B-F458-461F-B36C-120F68771A92}";
    inline constexpr const char* TimeDistortionEditorSystemComponentTypeId = "{3031D7E4-9B31-4056-82AA-06A359545580}";

    // Module derived classes TypeIds
    inline constexpr const char* TimeDistortionModuleInterfaceTypeId = "{546D328B-CCAF-467C-8C46-560A5ACD2152}";
    inline constexpr const char* TimeDistortionModuleTypeId = "{A6F48D36-194B-4ADB-9963-606DF16A16D4}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* TimeDistortionEditorModuleTypeId = TimeDistortionModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* TimeDistortionRequestsTypeId = "{2F34A6BC-9A91-4AC6-917E-48366BE4B26C}";
} // namespace TimeDistortion
