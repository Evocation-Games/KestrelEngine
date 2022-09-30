test_suite(Kestrel
    # Resources
        # ResourceNamespace
            resource_namespace_constructWithName
            resource_namespace_constructWithMultipleNames
            resource_namespace_constructWithNoName_adoptsUniversalName
            resource_namespace_primaryName_returnsExpectedValue
            resource_namespace_count_returnsExpectedValue
            resource_namespace_text_returnsExpectedValue
            resource_namespace_containsResources_returnsTrueIfNamespaceHasResources
            resource_namespace_containsResources_returnsFalseIfNamespaceHasNoResources
            resource_namespace_hasName_returnsExpectedResult
            resource_namespace_isGlobal_reportsExpectedValue
            resource_namespace_isUniversal_reportsExpectedValue
            resource_namespace_addNamespace_reportsExpectedTextValue
            resource_namespace_firstResourceOfType_returnsExpectedDescriptor
            resource_namespace_resourcesOfType_returnsExpectedDescriptors
            resource_namespace_resourceForTypeAndId_returnsExpectedDescriptor
            resource_namespace_addFileConstraint_correctlyAppliesConstraint_withSingleNamespace
            resource_namespace_addFileConstraint_correctlyAppliesConstraint_withMultipleNamespaces
            resource_namespace_addResourceIdentifier_returnsExpectedResource
            resource_namespace_addTypedResource_returnsExpectedResource
            resource_namespace_addNamedResource_returnsExpectedResource
            resource_namespace_addTypedResourceIdentifier_returnsExpectedResource
            resource_namespace_addNamedResourceIdentifier_returnsExpectedResource
            resource_namespace_addTypedNamedResource_returnsExpectedResource
            resource_namespace_addTypedNamedResourceIdentifier_returnsExpectedResource

        # ResourceDescriptor


    # Scripting
        # lua::vector
            lua_vector_construction
            lua_vector_luaEmpty
            lua_vector_luaSize
            lua_vector_luaAt
            lua_vector_at
            lua_vector_emplaceBack
            lua_vector_appendVector

    # Utilities
        # uid
            uid_random_doesNotGenerateRepeatValues
)