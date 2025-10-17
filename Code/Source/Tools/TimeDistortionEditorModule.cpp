
#include "TimeDistortionEditorSystemComponent.h"
#include <TimeDistortion/TimeDistortionTypeIds.h>
#include <TimeDistortionModuleInterface.h>

namespace TimeDistortion
{
    class TimeDistortionEditorModule : public TimeDistortionModuleInterface
    {
    public:
        AZ_RTTI(TimeDistortionEditorModule, TimeDistortionEditorModuleTypeId, TimeDistortionModuleInterface);
        AZ_CLASS_ALLOCATOR(TimeDistortionEditorModule, AZ::SystemAllocator);

        TimeDistortionEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    TimeDistortionEditorSystemComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<TimeDistortionEditorSystemComponent>(),
            };
        }
    };
} // namespace TimeDistortion

AZ_DECLARE_MODULE_CLASS(Gem_TimeDistortion, TimeDistortion::TimeDistortionEditorModule)
