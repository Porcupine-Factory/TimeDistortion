
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/TimeDistortionSystemComponent.h>

namespace TimeDistortion
{
    /// System component for TimeDistortion editor
    class TimeDistortionEditorSystemComponent
        : public TimeDistortionSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = TimeDistortionSystemComponent;
    public:
        AZ_COMPONENT_DECL(TimeDistortionEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        TimeDistortionEditorSystemComponent();
        ~TimeDistortionEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace TimeDistortion
