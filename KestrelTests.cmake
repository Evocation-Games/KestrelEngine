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
            resource_descriptor_fileConstrainedDescriptor
            resource_descriptor_identifiedDescriptor
            resource_descriptor_typedDescriptor
            resource_descriptor_namedDescriptor
            resource_descriptor_typedIdentifiedDescriptor
            resource_descriptor_identifiedNamedDescriptor
            resource_descriptor_typedNamedDescriptor
            resource_descriptor_typedIdentifiedNamedDescriptor
            resource_descriptor_identifier_descriptionHasExpectedValue
            resource_descriptor_typed_descriptionHasExpectedValue
            resource_descriptor_named_descriptionHasExpectedValue
            resource_descriptor_typedIdentified_descriptionHasExpectedValue
            resource_descriptor_typedNamed_descriptionHasExpectedValue
            resource_descriptor_identifiedNamed_descriptionHasExpectedValue
            resource_descriptor_typedIdentifiedNamed_descriptionHasExpectedValue
            resource_descriptor_typedIdentifiedNamed_withNamespace_descriptionHasExpectedValue
            resource_descriptor_hasType_expectedResult
            resource_descriptor_hasId_expectedResult
            resource_descriptor_hasName_expectedResult
            resource_descriptor_isNamespaced_expectedResult

    # Scripting
        # lua::vector
            lua_vector_construction
            lua_vector_luaEmpty
            lua_vector_luaSize
            lua_vector_luaAt
            lua_vector_at
            lua_vector_emplaceBack
            lua_vector_appendVector

    # Physics
        # Collisions
            triangle_triangle_hasCollision_whenOverlapping
            triangle_triangle_hasCollision_whenNotOverlapping

    # Utilities
        # uid
            uid_random_doesNotGenerateRepeatValues
)