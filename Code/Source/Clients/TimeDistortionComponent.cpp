#include <Clients/TimeDistortionComponent.h>

#include <Atom/RPI.Public/ViewportContext.h>
#include <Atom/RPI.Public/ViewportContextBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Time/ITime.h>
#include <AzFramework/Physics/SystemBus.h>
#include <PhysX/Configuration/PhysXConfiguration.h>
#include <System/PhysXSystem.h>
#include <limits>

namespace TimeDistortion
{
    void TimeDistortionComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<TimeDistortionComponent, AZ::Component>()
                ->Field("Time Distortion Factor", &TimeDistortionComponent::m_timeDistortionFactor)
                ->Field("Set Timestep Based on Refresh Rate", &TimeDistortionComponent::m_timestepBasedOnRefreshRate)
                ->Version(1);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                using namespace AZ::Edit::Attributes;
                ec->Class<TimeDistortionComponent>("Time Distortion", "Distort time by making everything faster or slower")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(Category, "Time Distortion")
                    ->Attribute(AutoExpand, false)
                    ->DataElement(
                        nullptr,
                        &TimeDistortionComponent::m_timeDistortionFactor,
                        "Time Distortion Factor",
                        "Factor applied to the assumed tick and physics timestep.")
                    ->DataElement(
                        nullptr,
                        &TimeDistortionComponent::m_timestepBasedOnRefreshRate,
                        "Set Timestep Based on Refresh Rate",
                        "When enabled, the physics timestep will be set based on the monitor's refresh rate.");
            }
        }

        if (auto bc = azrtti_cast<AZ::BehaviorContext*>(rc))
        {
            bc->EBus<TimeDistortionNotificationBus>("TimeDistortionNotificationBus")->Handler<TimeDistortionNotificationHandler>();

            bc->EBus<TimeDistortionComponentRequestBus>("TimeDistortionComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "effects")
                ->Attribute(AZ::Script::Attributes::Category, "Time Distortion")
                ->Event("Get Time Distortion Factor", &TimeDistortionComponentRequests::GetTimeDistortionFactor)
                ->Event("Set Time Distortion Factor", &TimeDistortionComponentRequests::SetTimeDistortionFactor)
                ->Event("Get Timestep Based On Refresh Rate", &TimeDistortionComponentRequests::GetTimestepBasedOnRefreshRate)
                ->Event("Set Timestep Based On Refresh Rate", &TimeDistortionComponentRequests::SetTimestepBasedOnRefreshRate)
                ->Event("Get Refresh Rate", &TimeDistortionComponentRequests::GetRefreshRate)
                ->Event("Get Default Fixed Timestep", &TimeDistortionComponentRequests::GetDefaultFixedTimestep)
                ->Event("Set Default Fixed Timestep", &TimeDistortionComponentRequests::SetDefaultFixedTimestep)
                ->Event("Apply Default Fixed Timestep", &TimeDistortionComponentRequests::ApplyDefaultFixedTimestep)
                ->Event("Get Physics Enabled", &TimeDistortionComponentRequests::GetPhysicsEnabled)
                ->Event("Set Physics Enabled", &TimeDistortionComponentRequests::SetPhysicsEnabled)
                ->Event("Get Paused", &TimeDistortionComponentRequests::GetPaused)
                ->Event("Set Paused", &TimeDistortionComponentRequests::SetPaused);

            bc->Class<TimeDistortionComponent>()->RequestBus("TimeDistortionComponentRequestBus");
        }
    }

    void TimeDistortionComponent::Activate()
    {
        if (m_timestepBasedOnRefreshRate)
        {
            AzFramework::NativeWindowHandle windowHandle = nullptr;
            windowHandle = AZ::RPI::ViewportContextRequests::Get()->GetDefaultViewportContext()->GetWindowHandle();
            if (windowHandle)
            {
                float refreshRate = 60.f;
                AzFramework::WindowRequestBus::EventResult(
                    refreshRate, windowHandle, &AzFramework::WindowRequestBus::Events::GetDisplayRefreshRate);

                AzPhysics::SystemConfiguration* config =
                    const_cast<AzPhysics::SystemConfiguration*>(AZ::Interface<AzPhysics::SystemInterface>::Get()->GetConfiguration());

                config->m_fixedTimestep = 1.f / refreshRate;

                AZ::Interface<AzPhysics::SystemInterface>::Get()->UpdateConfiguration(config, true);
            }
        }

        // Apply the default time distortion factor that's entered in the component's text field
        if (auto* timeSystem = AZ::Interface<AZ::ITime>::Get())
            timeSystem->SetSimulationTickScale(m_timeDistortionFactor);
        PhysX::PhysXSystemConfiguration prevConfig;
        auto* physicsSystem = AZ::Interface<AzPhysics::SystemInterface>::Get();
        if (const auto* config = azdynamic_cast<const PhysX::PhysXSystemConfiguration*>(physicsSystem->GetConfiguration()))
            prevConfig = *config;
        PhysX::PhysXSystemConfiguration modifiedConfig = prevConfig;

        // Get the default PhysX Fixed Timestep
        m_defaultFixedTimestep = modifiedConfig.m_fixedTimestep;

        if (m_timeDistortionFactor != 1.f && m_timeDistortionFactor >= 0.f)
        {
            modifiedConfig.m_fixedTimestep *= m_timeDistortionFactor;
            physicsSystem->UpdateConfiguration(&modifiedConfig);
        }

        // Connect the handler to the request bus
        TimeDistortionComponentRequestBus::Handler::BusConnect(GetEntityId());
    }

    void TimeDistortionComponent::Deactivate()
    {
        // Reset the time distortion factor
        if (auto* timeSystem = AZ::Interface<AZ::ITime>::Get())
            timeSystem->SetSimulationTickScale(1.f);
        PhysX::PhysXSystemConfiguration prevConfig;
        auto* physicsSystem = AZ::Interface<AzPhysics::SystemInterface>::Get();
        if (const auto* config = azdynamic_cast<const PhysX::PhysXSystemConfiguration*>(physicsSystem->GetConfiguration()))
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

    void TimeDistortionComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("TimeDistortionService"));
    }

    // Event Notification methods for use in scripts
    void TimeDistortionComponent::OnTimeDistortionChanged()
    {
    }

    // Request Bus getter and setter methods for use in scripts
    float TimeDistortionComponent::GetTimeDistortionFactor() const
    {
        return m_timeDistortionFactor;
    }
    void TimeDistortionComponent::SetTimeDistortionFactor(const float& new_timeDistortionFactor)
    {
        if (new_timeDistortionFactor >= 0.f)
        {
            // Set the new time distortion factor
            m_timeDistortionFactor = new_timeDistortionFactor;

            // Apply the new time distortion factor
            if (auto* timeSystem = AZ::Interface<AZ::ITime>::Get())
                timeSystem->SetSimulationTickScale(m_timeDistortionFactor);
            PhysX::PhysXSystemConfiguration prevConfig;
            auto* physicsSystem = AZ::Interface<AzPhysics::SystemInterface>::Get();
            if (const auto* config = azdynamic_cast<const PhysX::PhysXSystemConfiguration*>(physicsSystem->GetConfiguration()))
                prevConfig = *config;
            PhysX::PhysXSystemConfiguration modifiedConfig = prevConfig;
            modifiedConfig.m_fixedTimestep = m_defaultFixedTimestep * m_timeDistortionFactor;
            physicsSystem->UpdateConfiguration(&modifiedConfig);

            // Broadcast a notification event letting listeners know that the time distortion factor changed
            TimeDistortionNotificationBus::Broadcast(&TimeDistortionNotificationBus::Events::OnTimeDistortionChanged);
        }
        else
            AZ_Warning("TimeDistortionComponent", false, "The Time Distortion Factor cannot be negative.");
    }
    bool TimeDistortionComponent::GetTimestepBasedOnRefreshRate() const
    {
        return m_timestepBasedOnRefreshRate;
    }
    void TimeDistortionComponent::SetTimestepBasedOnRefreshRate(const bool& new_timestepBasedOnRefreshRate)
    {
        m_timestepBasedOnRefreshRate = new_timestepBasedOnRefreshRate;

        if (m_timestepBasedOnRefreshRate)
        {
            AzFramework::NativeWindowHandle windowHandle = nullptr;
            windowHandle = AZ::RPI::ViewportContextRequests::Get()->GetDefaultViewportContext()->GetWindowHandle();
            if (windowHandle)
            {
                float refreshRate = 60.f;
                AzFramework::WindowRequestBus::EventResult(
                    refreshRate, windowHandle, &AzFramework::WindowRequestBus::Events::GetDisplayRefreshRate);

                AzPhysics::SystemConfiguration* config =
                    const_cast<AzPhysics::SystemConfiguration*>(AZ::Interface<AzPhysics::SystemInterface>::Get()->GetConfiguration());

                config->m_fixedTimestep = 1.f / refreshRate;

                AZ::Interface<AzPhysics::SystemInterface>::Get()->UpdateConfiguration(config, true);
            }
        }
    }
    float TimeDistortionComponent::GetRefreshRate() const
    {
        AzFramework::NativeWindowHandle windowHandle = nullptr;
        windowHandle = AZ::RPI::ViewportContextRequests::Get()->GetDefaultViewportContext()->GetWindowHandle();
        if (windowHandle)
        {
            float refreshRate = 60.f;
            AzFramework::WindowRequestBus::EventResult(
                refreshRate, windowHandle, &AzFramework::WindowRequestBus::Events::GetDisplayRefreshRate);

            return refreshRate;
        }
        else
        {
            // Return NaN when the window handle wasn't obtained
            return std::numeric_limits<float>::quiet_NaN();
        }
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
        if (const auto* config = azdynamic_cast<const PhysX::PhysXSystemConfiguration*>(physicsSystem->GetConfiguration()))
            prevConfig = *config;
        PhysX::PhysXSystemConfiguration modifiedConfig = prevConfig;
        modifiedConfig.m_fixedTimestep = m_defaultFixedTimestep;
        physicsSystem->UpdateConfiguration(&modifiedConfig);
    }
    bool TimeDistortionComponent::GetPhysicsEnabled() const
    {
        return m_physicsEnabled;
    }
    void TimeDistortionComponent::SetPhysicsEnabled(const bool& new_physicsEnabled)
    {
        m_physicsEnabled = new_physicsEnabled;

        AzPhysics::SceneHandle sceneHandle;
        Physics::DefaultWorldBus::BroadcastResult(sceneHandle, &Physics::DefaultWorldRequests::GetDefaultSceneHandle);
        PhysX::PhysXSystem* physXSystem = PhysX::GetPhysXSystem();

        physXSystem->GetScene(sceneHandle)->SetEnabled(m_physicsEnabled);
    }
    bool TimeDistortionComponent::GetPaused() const
    {
        return m_paused;
    }
    void TimeDistortionComponent::SetPaused(const bool& new_paused)
    {
        m_paused = new_paused;

        if (m_paused)
        {
            SetPhysicsEnabled(false);
            m_timeDistortionFactorBeforePause = GetTimeDistortionFactor();
            SetTimeDistortionFactor(0.f);
        }
        else
        {
            SetTimeDistortionFactor(m_timeDistortionFactorBeforePause);
            SetPhysicsEnabled(true);
        }
    }
} // namespace TimeDistortion
