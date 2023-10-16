#include <Clients/TimeDistortionComponent.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Component/Entity.h>
#include <System/PhysXSystem.h>
#include <AzCore/Time/ITime.h>
#include <PhysX/Configuration/PhysXConfiguration.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace TimeDistortion
{
    void TimeDistortionComponent::Reflect(AZ::ReflectContext* rc)
    {
        if(auto sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<TimeDistortionComponent, AZ::Component>()
              ->Field("Time Distortion Factor", &TimeDistortionComponent::m_timeDistortionFactor)
              ->Version(1);

            if(AZ::EditContext* ec = sc->GetEditContext())
            {
                using namespace AZ::Edit::Attributes;
                ec->Class<TimeDistortionComponent>("Time Distortion",
                    "Distort time by making everything faster or slower")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(Category, "Time Distortion")
                    ->Attribute(AutoExpand, false)
                    ->DataElement(nullptr,
                        &TimeDistortionComponent::m_timeDistortionFactor,
                        "Time Distortion Factor", "Factor applied to the assumed tick and physics timestep.");
            }
        }

        if(auto bc = azrtti_cast<AZ::BehaviorContext*>(rc))
        {
            bc->EBus<TimeDistortionNotificationBus>("TimeDistortionNotificationBus")
                ->Handler<TimeDistortionNotificationHandler>();

            bc->EBus<TimeDistortionComponentRequestBus>("TimeDistortionComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "effects")
                ->Attribute(AZ::Script::Attributes::Category, "Time Distortion")
                ->Event("Get Time Distortion Factor", &TimeDistortionComponentRequests::GetTimeDistortionFactor)
                ->Event("Set Time Distortion Factor", &TimeDistortionComponentRequests::SetTimeDistortionFactor)
                ->Event("Get Default Fixed Timestep", &TimeDistortionComponentRequests::GetDefaultFixedTimestep)
                ->Event("Set Default Fixed Timestep", &TimeDistortionComponentRequests::SetDefaultFixedTimestep)
                ->Event("Apply Default Fixed Timestep", &TimeDistortionComponentRequests::ApplyDefaultFixedTimestep);

            bc->Class<TimeDistortionComponent>()->RequestBus("TimeDistortionComponentRequestBus");
        }
    }

    void TimeDistortionComponent::Activate()
    {
        // Apply the default time distortion factor that's entered in the component's text field
        if(auto* timeSystem = AZ::Interface<AZ::ITime>::Get())
            timeSystem->SetSimulationTickScale(m_timeDistortionFactor);
        PhysX::PhysXSystemConfiguration prevConfig;
        auto* physicsSystem = AZ::Interface<AzPhysics::SystemInterface>::Get();
        if(const auto* config = azdynamic_cast<const PhysX::PhysXSystemConfiguration*>(physicsSystem->GetConfiguration()))
            prevConfig = *config;
        PhysX::PhysXSystemConfiguration modifiedConfig = prevConfig;

        // Get the default PhysX Fixed Time Step
        m_defaultFixedTimestep = modifiedConfig.m_fixedTimestep;

        modifiedConfig.m_fixedTimestep *= m_timeDistortionFactor;
        physicsSystem->UpdateConfiguration(&modifiedConfig);

        // Connect the handler to the request bus
        TimeDistortionComponentRequestBus::Handler::BusConnect(GetEntityId());
    }

    void TimeDistortionComponent::Deactivate()
    {
        // Reset the time distortion factor
        if(auto* timeSystem = AZ::Interface<AZ::ITime>::Get())
            timeSystem->SetSimulationTickScale(1.f);
        PhysX::PhysXSystemConfiguration prevConfig;
        auto* physicsSystem = AZ::Interface<AzPhysics::SystemInterface>::Get();
        if(const auto* config = azdynamic_cast<const PhysX::PhysXSystemConfiguration*>(physicsSystem->GetConfiguration()))
            prevConfig = *config;
        PhysX::PhysXSystemConfiguration modifiedConfig = prevConfig;
        modifiedConfig.m_fixedTimestep = m_defaultFixedTimestep;
        physicsSystem->UpdateConfiguration(&modifiedConfig);

        // Disconnect the handler from the request bus
        TimeDistortionComponentRequestBus::Handler::BusDisconnect();
    }

    void TimeDistortionComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void TimeDistortionComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("TimeDistortionService"));
    }

    // Event Notification methods for use in scripts
    void TimeDistortionComponent::OnTimeDistortionChanged(){}

    // Request Bus getter and setter methods for use in scripts
    float TimeDistortionComponent::GetTimeDistortionFactor() const
    {
        return m_timeDistortionFactor;
    }
    void TimeDistortionComponent::SetTimeDistortionFactor(const float& new_timeDistortionFactor)
    {
        // Set the new time distortion factor
        m_timeDistortionFactor = new_timeDistortionFactor;

        // Apply the new time distortion factor
        if(auto* timeSystem = AZ::Interface<AZ::ITime>::Get())
            timeSystem->SetSimulationTickScale(m_timeDistortionFactor);
        PhysX::PhysXSystemConfiguration prevConfig;
        auto* physicsSystem = AZ::Interface<AzPhysics::SystemInterface>::Get();
        if(const auto* config = azdynamic_cast<const PhysX::PhysXSystemConfiguration*>(physicsSystem->GetConfiguration()))
            prevConfig = *config;
        PhysX::PhysXSystemConfiguration modifiedConfig = prevConfig;
        modifiedConfig.m_fixedTimestep = m_defaultFixedTimestep * m_timeDistortionFactor;
        physicsSystem->UpdateConfiguration(&modifiedConfig);

        // Broadcast a notification event letting listeners know that the time distortion factor changed
        TimeDistortionNotificationBus::Broadcast(&TimeDistortionNotificationBus::Events::OnTimeDistortionChanged);
    }
    float TimeDistortionComponent::GetDefaultFixedTimestep() const
    {
        return m_defaultFixedTimestep;
    }
    void TimeDistortionComponent::SetDefaultFixedTimestep(const float& new_defaultFixedTimestep)
    {
        m_defaultFixedTimestep = new_defaultFixedTimestep;
    }
    void TimeDistortionComponent::ApplyDefaultFixedTimestep() const
    {
        // Apply the default PhysX Fixed Time Step
        PhysX::PhysXSystemConfiguration prevConfig;
        auto* physicsSystem = AZ::Interface<AzPhysics::SystemInterface>::Get();
        if(const auto* config = azdynamic_cast<const PhysX::PhysXSystemConfiguration*>(physicsSystem->GetConfiguration()))
            prevConfig = *config;
        PhysX::PhysXSystemConfiguration modifiedConfig = prevConfig;
        modifiedConfig.m_fixedTimestep = m_defaultFixedTimestep;
        physicsSystem->UpdateConfiguration(&modifiedConfig);
    }
}
