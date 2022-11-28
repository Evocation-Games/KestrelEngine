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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/resource/namespace.hpp>
#include <libKestrel/resource/descriptor.hpp>

#include "helpers/resource_manager.hpp"

using namespace kestrel::resource;

// MARK: - Construction

TEST(resource_namespace_constructWithName)
{
    resource_namespace ns("test");
    test::equal(ns.count(), 1);
    test::equal(ns.text(), std::string("test"));
}

TEST(resource_namespace_constructWithMultipleNames)
{
    resource_namespace ns(std::vector<std::string>({"test", "foo" }));
    test::equal(ns.count(), 2);
    test::equal(ns.text(), std::string("test foo"));
}

TEST(resource_namespace_constructWithNoName_adoptsUniversalName)
{
    resource_namespace ns(std::vector<std::string>({}));
    test::equal(ns.count(), 1);
    test::equal(ns.text(), std::string("*"));
}

// MARK: - Accessors

TEST(resource_namespace_primaryName_returnsExpectedValue)
{
    resource_namespace ns(std::vector<std::string>({ "test", "foo" }));
    test::equal(ns.primary_name(), std::string("test"));
}

TEST(resource_namespace_count_returnsExpectedValue)
{
    resource_namespace ns(std::vector<std::string>({ "test", "foo" }));
    test::equal(ns.count(), 2);
}

TEST(resource_namespace_text_returnsExpectedValue)
{
    resource_namespace ns(std::vector<std::string>({ "test", "foo", "bar" }));
    test::equal(ns.text(), std::string("test foo bar"));
}

TEST(resource_namespace_containsResources_returnsTrueIfNamespaceHasResources)
{
    test::resource_manager::setup()
        .add_file()
            .add_resource("test", 128, "", "example");

    test::is_true(resource_namespace("example").contains_resources());
}

TEST(resource_namespace_containsResources_returnsFalseIfNamespaceHasNoResources)
{
    test::resource_manager::setup()
        .add_file()
            .add_resource("test", 128, "", "foo");

    test::is_false(resource_namespace("example").contains_resources());
}

// MARK: - Querying

TEST(resource_namespace_hasName_returnsExpectedResult)
{
    test::is_true(resource_namespace("test").has_name("test"));
    test::is_false(resource_namespace("test").has_name("foo"));
}

TEST(resource_namespace_isGlobal_reportsExpectedValue)
{
    test::is_true(resource_namespace::global()->is_global());
    test::is_false(resource_namespace("foo").is_global());
}

TEST(resource_namespace_isUniversal_reportsExpectedValue)
{
    test::is_true(resource_namespace::universal()->is_universal());
    test::is_false(resource_namespace("foo").is_universal());
}

// MARK: - Modification

TEST(resource_namespace_addNamespace_reportsExpectedTextValue)
{
    resource_namespace ns("example");
    test::equal(ns.text(), std::string("example"));

    ns.add_namespace(resource_namespace("foo"));
    test::equal(ns.text(), std::string("example foo"));
}

// MARK: - Look Up

TEST(resource_namespace_firstResourceOfType_returnsExpectedDescriptor)
{
    test::resource_manager::setup()
        .add_file()
            .add_resource("type", 128, "first", "example")
            .add_resource("type", 129, "second", "example")
            .add_resource("type", 130, "third", "example")
            .add_resource("type", 127, "fourth", "example");

    // TODO: Assess whether this is the correct behaviour to expect. Should we be ordered on ID, or creation?
    auto descriptor = resource_namespace("example").first_resource_of("type");
    test::equal(descriptor->id, 128);
    test::equal(descriptor->name, std::string("first"));
}

TEST(resource_namespace_resourcesOfType_returnsExpectedDescriptors)
{
    test::resource_manager::setup()
        .add_file()
            .add_resource("type", 128, "first", "example")
            .add_resource("type", 129, "second", "example")
            .add_resource("food", 128, "bread", "example")
            .add_resource("food", 129, "soup", "example")
            .add_resource("type", 130, "third", "example")
            .add_resource("type", 127, "fourth", "example");

    auto descriptors = resource_namespace("example").resources_of_type("type");
    test::equal(descriptors.size(), 4);
    test::equal(descriptors.at(0)->id, 128);
    test::equal(descriptors.at(1)->id, 129);
    test::equal(descriptors.at(2)->id, 130);
    test::equal(descriptors.at(3)->id, 127);
}


TEST(resource_namespace_resourceForTypeAndId_returnsExpectedDescriptor)
{
    test::resource_manager::setup()
        .add_file()
            .add_resource("type", 128, "first", "example")
            .add_resource("type", 129, "second", "example")
            .add_resource("type", 130, "third", "example")
            .add_resource("type", 127, "fourth", "example");

    auto descriptor = resource_namespace("example").resource_for_id("type", 130);
    test::equal(descriptor->id, 130);
    test::equal(descriptor->name, std::string("third"));
}

// MARK: - Constraints / Arguments

TEST(resource_namespace_addFileConstraint_correctlyAppliesConstraint_withSingleNamespace)
{
    auto file = new graphite::rsrc::file();
    resource_namespace ns("example");

    auto descriptor = ns.file_constraint(file);
    test::equal(descriptor->namespaces.size(), 1);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
}

TEST(resource_namespace_addFileConstraint_correctlyAppliesConstraint_withMultipleNamespaces)
{
    auto file = new graphite::rsrc::file();
    resource_namespace ns(std::vector<std::string>({ "example", "foo", "bar" }));

    auto descriptor = ns.file_constraint(file);
    test::equal(descriptor->namespaces.size(), 3);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
    test::equal(descriptor->namespaces.at(1), std::string("foo"));
    test::equal(descriptor->namespaces.at(2), std::string("bar"));
}

TEST(resource_namespace_addResourceIdentifier_returnsExpectedResource)
{
    auto descriptor = resource_namespace("example").identified_resource(128);
    test::equal(descriptor->id, 128);
    test::equal(descriptor->namespaces.size(), 1);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
    test::is_true(descriptor->name.empty());
    test::is_true(descriptor->type.empty());
}

TEST(resource_namespace_addTypedResource_returnsExpectedResource)
{
    auto descriptor = resource_namespace("example").typed_resource("foo ");
    test::equal(descriptor->id, INT64_MIN); // TODO: We should be using a dedicated constant/symbol for unused resource id.
    test::equal(descriptor->namespaces.size(), 1);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
    test::is_true(descriptor->name.empty());
    test::equal(descriptor->type, std::string("foo "));
}

TEST(resource_namespace_addNamedResource_returnsExpectedResource)
{
    auto descriptor = resource_namespace("example").named_resource("test");
    test::equal(descriptor->id, INT64_MIN);
    test::equal(descriptor->namespaces.size(), 1);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
    test::equal(descriptor->name, std::string("test"));
    test::is_true(descriptor->type.empty());
}

TEST(resource_namespace_addTypedResourceIdentifier_returnsExpectedResource)
{
    auto descriptor = resource_namespace("example").typed_identified_resource("foo ", 128);
    test::equal(descriptor->id, 128);
    test::equal(descriptor->namespaces.size(), 1);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
    test::is_true(descriptor->name.empty());
    test::equal(descriptor->type, std::string("foo "));
}

TEST(resource_namespace_addNamedResourceIdentifier_returnsExpectedResource)
{
    auto descriptor = resource_namespace("example").identified_named_resource(128, "test");
    test::equal(descriptor->id, 128);
    test::equal(descriptor->namespaces.size(), 1);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
    test::equal(descriptor->name, std::string("test"));
    test::is_true(descriptor->type.empty());
}

TEST(resource_namespace_addTypedNamedResource_returnsExpectedResource)
{
    auto descriptor = resource_namespace("example").typed_named_resource("foo ", "test");
    test::equal(descriptor->id, INT64_MIN);
    test::equal(descriptor->namespaces.size(), 1);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
    test::equal(descriptor->name, std::string("test"));
    test::equal(descriptor->type, std::string("foo "));
}

TEST(resource_namespace_addTypedNamedResourceIdentifier_returnsExpectedResource)
{
    auto descriptor = resource_namespace("example").typed_identified_named_resource("foo ", 128, "test");
    test::equal(descriptor->id, 128);
    test::equal(descriptor->namespaces.size(), 1);
    test::equal(descriptor->namespaces.at(0), std::string("example"));
    test::equal(descriptor->name, std::string("test"));
    test::equal(descriptor->type, std::string("foo "));
}