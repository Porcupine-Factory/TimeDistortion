
#include <AzCore/Serialization/SerializeContext.h>
#include "TimeDistortionEditorSystemComponent.h"

#include <TimeDistortion/TimeDistortionTypeIds.h>

namespace TimeDistortion
{
    AZ_COMPONENT_IMPL(TimeDistortionEditorSystemComponent, "TimeDistortionEditorSystemComponent",
        TimeDistortionEditorSystemComponentTypeId, BaseSystemComponent);

    void TimeDistortionEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TimeDistortionEditorSystemComponent, TimeDistortionSystemComponent>()
                ->Version(0);
        }
    }

    TimeDistortionEditorSystemComponent::TimeDistortionEditorSystemComponent() = default;

    TimeDistortionEditorSystemComponent::~TimeDistortionEditorSystemComponent() = default;

    void TimeDistortionEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("TimeDistortionEditorService"));
    }

    void TimeDistortionEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("TimeDistortionEditorService"));
    }

    void TimeDistortionEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void TimeDistortionEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void TimeDistortionEditorSystemComponent::Activate()
    {
        TimeDistortionSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void TimeDistortionEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        TimeDistortionSystemComponent::Deactivate();
    }

} // namespace TimeDistortion
