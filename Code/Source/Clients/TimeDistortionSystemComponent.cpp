
#include "TimeDistortionSystemComponent.h"

#include <TimeDistortion/TimeDistortionTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace TimeDistortion
{
    AZ_COMPONENT_IMPL(TimeDistortionSystemComponent, "TimeDistortionSystemComponent", TimeDistortionSystemComponentTypeId);

    void TimeDistortionSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<TimeDistortionSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void TimeDistortionSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("TimeDistortionService"));
    }

    void TimeDistortionSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("TimeDistortionService"));
    }

    void TimeDistortionSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void TimeDistortionSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    TimeDistortionSystemComponent::TimeDistortionSystemComponent()
    {
        if (TimeDistortionInterface::Get() == nullptr)
        {
            TimeDistortionInterface::Register(this);
        }
    }

    TimeDistortionSystemComponent::~TimeDistortionSystemComponent()
    {
        if (TimeDistortionInterface::Get() == this)
        {
            TimeDistortionInterface::Unregister(this);
        }
    }

    void TimeDistortionSystemComponent::Init()
    {
    }

    void TimeDistortionSystemComponent::Activate()
    {
        TimeDistortionRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void TimeDistortionSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        TimeDistortionRequestBus::Handler::BusDisconnect();
    }

    void TimeDistortionSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace TimeDistortion
