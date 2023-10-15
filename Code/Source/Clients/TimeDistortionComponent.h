#pragma once

#include <TimeDistortion/TimeDistortionComponentBus.h>

#include <AzCore/Component/Component.h>

namespace TimeDistortion
{
    class TimeDistortionComponent
        : public AZ::Component
        , public TimeDistortionComponentRequestBus::Handler
    {
    public:
        AZ_COMPONENT(TimeDistortionComponent, "{1ebc61d4-e90d-4ad5-b24e-7631d75e39be}");

        static void Reflect(AZ::ReflectContext* rc);

        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;

        static void GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);

        // TimeDistortionRequestBus
        void SetTimeDistortionFactor(const float& new_timeDistortionFactor) override;
        float GetTimeDistortionFactor() const override;

    private:
        // TimeDistortionNotificationBus
        void OnTimeDistortionChanged();

        // Time Distortion Factor
        float m_timeDistortionFactor = 0.5f;
    };
}
