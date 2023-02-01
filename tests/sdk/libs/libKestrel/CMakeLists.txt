# Copyright (c) 2023 Tom Hancocks
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

test_suite(Kestrel)

    test_include_directory(${TESTS})

    test_case(ResourceNamespace)
        test(resource_namespace_constructWithName)
        test(resource_namespace_constructWithMultipleNames)
        test(resource_namespace_constructWithNoName_adoptsUniversalName)
        test(resource_namespace_primaryName_returnsExpectedValue)
        test(resource_namespace_count_returnsExpectedValue)
        test(resource_namespace_text_returnsExpectedValue)
        test(resource_namespace_containsResources_returnsTrueIfNamespaceHasResources)
        test(resource_namespace_containsResources_returnsFalseIfNamespaceHasNoResources)
        test(resource_namespace_hasName_returnsExpectedResult)
        test(resource_namespace_isGlobal_reportsExpectedValue)
        test(resource_namespace_isUniversal_reportsExpectedValue)
        test(resource_namespace_addNamespace_reportsExpectedTextValue)
        test(resource_namespace_firstResourceOfType_returnsExpectedDescriptor)
        test(resource_namespace_resourcesOfType_returnsExpectedDescriptors)
        test(resource_namespace_resourceForTypeAndId_returnsExpectedDescriptor)
        test(resource_namespace_addFileConstraint_correctlyAppliesConstraint_withSingleNamespace)
        test(resource_namespace_addFileConstraint_correctlyAppliesConstraint_withMultipleNamespaces)
        test(resource_namespace_addResourceIdentifier_returnsExpectedResource)
        test(resource_namespace_addTypedResource_returnsExpectedResource)
        test(resource_namespace_addNamedResource_returnsExpectedResource)
        test(resource_namespace_addTypedResourceIdentifier_returnsExpectedResource)
        test(resource_namespace_addNamedResourceIdentifier_returnsExpectedResource)
        test(resource_namespace_addTypedNamedResource_returnsExpectedResource)
        test(resource_namespace_addTypedNamedResourceIdentifier_returnsExpectedResource)
    end_test_case()

    test_case(ResourceDescriptor)
        test(resource_descriptor_fileConstrainedDescriptor)
        test(resource_descriptor_identifiedDescriptor)
        test(resource_descriptor_typedDescriptor)
        test(resource_descriptor_namedDescriptor)
        test(resource_descriptor_typedIdentifiedDescriptor)
        test(resource_descriptor_identifiedNamedDescriptor)
        test(resource_descriptor_typedNamedDescriptor)
        test(resource_descriptor_typedIdentifiedNamedDescriptor)
        test(resource_descriptor_identifier_descriptionHasExpectedValue)
        test(resource_descriptor_typed_descriptionHasExpectedValue)
        test(resource_descriptor_named_descriptionHasExpectedValue)
        test(resource_descriptor_typedIdentified_descriptionHasExpectedValue)
        test(resource_descriptor_typedNamed_descriptionHasExpectedValue)
        test(resource_descriptor_identifiedNamed_descriptionHasExpectedValue)
        test(resource_descriptor_typedIdentifiedNamed_descriptionHasExpectedValue)
        test(resource_descriptor_typedIdentifiedNamed_withNamespace_descriptionHasExpectedValue)
        test(resource_descriptor_hasType_expectedResult)
        test(resource_descriptor_hasId_expectedResult)
        test(resource_descriptor_hasName_expectedResult)
        test(resource_descriptor_isNamespaced_expectedResult)
    end_test_case()

    test_case(LuaScripting)
        test(lua_vector_construction)
        test(lua_vector_luaEmpty)
        test(lua_vector_luaSize)
        test(lua_vector_luaAt)
        test(lua_vector_at)
        test(lua_vector_emplaceBack)
        test(lua_vector_appendVector)
    end_test_case()

    test_case(PhysicsCollisions)
        test(triangle_triangle_hasCollision_whenOverlapping)
        test(triangle_triangle_hasCollision_whenNotOverlapping)
    end_test_case()

    test_case(UniversalIdentifiers)
        test(uid_random_doesNotGenerateRepeatValues)
    end_test_case()

end_test_suite()
