// Copyright (c) 2022 Tom Hancocks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <libTesting/testing.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libResourceCore/file.hpp>
#include "helpers/resource_manager.hpp"

using namespace kestrel::resource;

// MARK: - Construction

TEST(resource_descriptor_fileConstrainedDescriptor)
{

    auto file = new resource_core::file();
    auto desc = descriptor::file_constrained(file);
    test::not_null(desc.get());
    test::is_true(desc->file == file);
    test::equal(desc->id, INT64_MIN);
    test::is_true(desc->type.empty());
    test::is_true(desc->name.empty());
    test::is_true(desc->containers.empty());
}

TEST(resource_descriptor_identifiedDescriptor)
{
    auto desc = descriptor::identified(128);
    test::not_null(desc.get());
    test::is_null(desc->file);
    test::equal(desc->id, 128);
    test::is_true(desc->type.empty());
    test::is_true(desc->name.empty());
    test::is_true(desc->containers.empty());
}

TEST(resource_descriptor_typedDescriptor)
{
    auto desc = descriptor::typed("foo ");
    test::not_null(desc.get());
    test::is_null(desc->file);
    test::equal(desc->id, INT64_MIN);
    test::equal(desc->type, std::string("foo "));
    test::is_true(desc->name.empty());
    test::is_true(desc->containers.empty());
}

TEST(resource_descriptor_namedDescriptor)
{
    auto desc = descriptor::named("test");
    test::not_null(desc.get());
    test::is_null(desc->file);
    test::equal(desc->id, INT64_MIN);
    test::is_true(desc->type.empty());
    test::equal(desc->name, std::string("test"));
    test::is_true(desc->containers.empty());
}

TEST(resource_descriptor_typedIdentifiedDescriptor)
{
    auto desc = descriptor::typed_identified("foo ", 128);
    test::not_null(desc.get());
    test::is_null(desc->file);
    test::equal(desc->id, 128);
    test::equal(desc->type, std::string("foo "));
    test::is_true(desc->name.empty());
    test::is_true(desc->containers.empty());
}

TEST(resource_descriptor_identifiedNamedDescriptor)
{
    auto desc = descriptor::identified_named(128, "test");
    test::not_null(desc.get());
    test::is_null(desc->file);
    test::equal(desc->id, 128);
    test::is_true(desc->type.empty());
    test::equal(desc->name, std::string("test"));
    test::is_true(desc->containers.empty());
}

TEST(resource_descriptor_typedNamedDescriptor)
{
    auto desc = descriptor::typed_named("foo ", "test");
    test::not_null(desc.get());
    test::is_null(desc->file);
    test::equal(desc->id, INT64_MIN);
    test::equal(desc->type, std::string("foo "));
    test::equal(desc->name, std::string("test"));
    test::is_true(desc->containers.empty());
}

TEST(resource_descriptor_typedIdentifiedNamedDescriptor)
{
    auto desc = descriptor::typed_identified_named("foo ", 128, "test");
    test::not_null(desc.get());
    test::is_null(desc->file);
    test::equal(desc->id, 128);
    test::equal(desc->type, std::string("foo "));
    test::equal(desc->name, std::string("test"));
    test::is_true(desc->containers.empty());
}

// MARK: - Description

TEST(resource_descriptor_identifier_descriptionHasExpectedValue)
{
    auto desc = descriptor::identified(128);
    test::equal(desc->description(), std::string("#128"));
}

TEST(resource_descriptor_typed_descriptionHasExpectedValue)
{
    auto desc = descriptor::typed("foo ");
    test::equal(desc->description(), std::string("foo "));
}

TEST(resource_descriptor_named_descriptionHasExpectedValue)
{
    auto desc = descriptor::named("test");
    test::equal(desc->description(), std::string("test"));
}

TEST(resource_descriptor_typedIdentified_descriptionHasExpectedValue)
{
    auto desc = descriptor::typed_identified("foo ", 128);
    test::equal(desc->description(), std::string("foo  #128"));
}

TEST(resource_descriptor_typedNamed_descriptionHasExpectedValue)
{
    auto desc = descriptor::typed_named("foo ", "test");
    test::equal(desc->description(), std::string("foo  test"));
}

TEST(resource_descriptor_identifiedNamed_descriptionHasExpectedValue)
{
    auto desc = descriptor::identified_named(128, "test");
    test::equal(desc->description(), std::string("#128 test"));
}

TEST(resource_descriptor_typedIdentifiedNamed_descriptionHasExpectedValue)
{
    auto desc = descriptor::typed_identified_named("foo ", 128, "test");
    test::equal(desc->description(), std::string("foo  #128 test"));
}

TEST(resource_descriptor_typedIdentifiedNamed_withNamespace_descriptionHasExpectedValue)
{
    auto desc = container("bar").typed_identified_named_resource("foo ", 128, "test");
    test::equal(desc->description(), std::string("foo  #128 test container<bar>"));
}

// MARK: - Has Characteristics

TEST(resource_descriptor_hasType_expectedResult)
{
    test::is_true(descriptor::typed("foo ")->has_type());
    test::is_false(descriptor::identified(128)->has_type());
}

TEST(resource_descriptor_hasId_expectedResult)
{
    test::is_true(descriptor::identified(128)->has_id());
    test::is_false(descriptor::typed("foo ")->has_id());
}

TEST(resource_descriptor_hasName_expectedResult)
{
    test::is_true(descriptor::named("test")->has_name());
    test::is_false(descriptor::identified(128)->has_name());
}

TEST(resource_descriptor_isNamespaced_expectedResult)
{
    test::is_true(container("foo").typed_resource("bar")->has_container());
    test::is_false(descriptor::typed("bar")->has_container());
}

// TODO: Fill out further test cases from Resource Descriptor.