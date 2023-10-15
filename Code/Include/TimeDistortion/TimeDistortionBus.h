
#pragma once

#include <TimeDistortion/TimeDistortionTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace TimeDistortion
{
    class TimeDistortionRequests
    {
    public:
        AZ_RTTI(TimeDistortionRequests, TimeDistortionRequestsTypeId);
        virtual ~TimeDistortionRequests() = default;
        // Put your public methods here
    };

    class TimeDistortionBusTraits
        : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using TimeDistortionRequestBus = AZ::EBus<TimeDistortionRequests, TimeDistortionBusTraits>;
    using TimeDistortionInterface = AZ::Interface<TimeDistortionRequests>;

} // namespace TimeDistortion
