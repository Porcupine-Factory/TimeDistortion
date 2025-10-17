#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/RTTI/BehaviorContext.h>

namespace TimeDistortion
{
    class TimeDistortionComponentRequests : public AZ::ComponentBus
    {
    public:
        ~TimeDistortionComponentRequests() override = default;

        virtual float GetTimeDistortionFactor() const = 0;
        virtual void SetTimeDistortionFactor(const float&) = 0;
        virtual bool GetTimestepBasedOnRefreshRate() const = 0;
        virtual void SetTimestepBasedOnRefreshRate(const bool&) = 0;
        virtual float GetRefreshRate() const = 0;
        virtual float GetDefaultFixedTimestep() const = 0;
        virtual void SetDefaultFixedTimestep(const float&) = 0;
        virtual void ApplyDefaultFixedTimestep() const = 0;
        virtual bool GetPhysicsEnabled() const = 0;
        virtual void SetPhysicsEnabled(const bool&) = 0;
        virtual bool GetPaused() const = 0;
        virtual void SetPaused(const bool&) = 0;
    };

    using TimeDistortionComponentRequestBus = AZ::EBus<TimeDistortionComponentRequests>;

    class TimeDistortionNotifications : public AZ::ComponentBus
    {
    public:
        virtual void OnTimeDistortionChanged() = 0;
    };

    using TimeDistortionNotificationBus = AZ::EBus<TimeDistortionNotifications>;

    class TimeDistortionNotificationHandler
        : public TimeDistortionNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER(
            TimeDistortionNotificationHandler, "{814559c9-032e-44ff-85fa-fc1a8ef95068}", AZ::SystemAllocator, OnTimeDistortionChanged);

        void OnTimeDistortionChanged() override
        {
            Call(FN_OnTimeDistortionChanged);
        }
    };
} // namespace TimeDistortion
