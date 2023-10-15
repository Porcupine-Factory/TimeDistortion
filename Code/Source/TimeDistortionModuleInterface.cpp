
#include "TimeDistortionModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <TimeDistortion/TimeDistortionTypeIds.h>

#include <Clients/TimeDistortionSystemComponent.h>
#include <Clients/TimeDistortionComponent.h>

namespace TimeDistortion
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(TimeDistortionModuleInterface,
        "TimeDistortionModuleInterface", TimeDistortionModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(TimeDistortionModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(TimeDistortionModuleInterface, AZ::SystemAllocator);

    TimeDistortionModuleInterface::TimeDistortionModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            TimeDistortionSystemComponent::CreateDescriptor(),
            TimeDistortionComponent::CreateDescriptor()
            });
    }

    AZ::ComponentTypeList TimeDistortionModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<TimeDistortionSystemComponent>(),
        };
    }
} // namespace TimeDistortion
