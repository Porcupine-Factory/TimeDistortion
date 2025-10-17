
#include "TimeDistortionSystemComponent.h"
#include <TimeDistortion/TimeDistortionTypeIds.h>
#include <TimeDistortionModuleInterface.h>

namespace TimeDistortion
{
    class TimeDistortionModule : public TimeDistortionModuleInterface
    {
    public:
        AZ_RTTI(TimeDistortionModule, TimeDistortionModuleTypeId, TimeDistortionModuleInterface);
        AZ_CLASS_ALLOCATOR(TimeDistortionModule, AZ::SystemAllocator);
    };
} // namespace TimeDistortion

AZ_DECLARE_MODULE_CLASS(Gem_TimeDistortion, TimeDistortion::TimeDistortionModule)
