// Copyright (c) 2023 Tom Hancocks
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

#pragma once

#include <string>
#include <memory>
#include <type_traits>
#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/languages/procedural/scalar.hpp>

namespace codegen::language::procedural
{
    template<type_support L>
    struct type_system
    {
        struct base
        {
            [[nodiscard]] virtual auto name() const -> std::string = 0;
        };
        typedef std::shared_ptr<struct base> type_id;

        struct internal_void_type : public base
        {
            [[nodiscard]] auto name() const -> std::string override { return L::void_type_name(); }
        };

        struct internal_error_type : public base
        {
            [[nodiscard]] auto name() const -> std::string override { return L::error_type_name(); }
        };

        struct internal_automatically_infered_type : public base
        {
            [[nodiscard]] auto name() const -> std::string override { return L::auto_inference_type_name(); }
        };

        struct internal_nested_type : public base
        {
            explicit internal_nested_type(const type_id& type) : m_nested(type) {}

            [[nodiscard]] auto name() const -> std::string override { return m_nested->name(); }

        private:
            type_id m_nested;
        };

        template<
            typename T,
            typename ...Args,
            typename std::enable_if<std::is_base_of<struct base, T>::value>::type* = nullptr
        >
        static auto make(Args&& ...args) -> type_id { return std::make_shared<T>(std::forward<Args>(args)...); }

        template<type::scalar S>
        struct internal_scalar_type : public base
        {
            [[nodiscard]] auto name() const -> std::string override { return L::scalar_type_name(S); }
        };

        struct internal_named_type : public base
        {
            explicit internal_named_type(const std::string& name) : m_name(name) {}
            [[nodiscard]] auto name() const -> std::string override { return m_name; }
        private:
            std::string m_name;
        };

        struct internal_ptr_type : public internal_nested_type
        {
            explicit internal_ptr_type(const type_id& type) : internal_nested_type(type) {}
            [[nodiscard]] auto name() const -> std::string override { return L::type_name_pointer(internal_nested_type::name()); }
        };

        struct internal_ref_type : public internal_nested_type
        {
            explicit internal_ref_type(const type_id& type) : internal_nested_type(type) {}
            [[nodiscard]] auto name() const -> std::string override { return L::type_name_reference(internal_nested_type::name()); }
        };

        struct internal_const_qualified_type : public internal_nested_type
        {
            explicit internal_const_qualified_type(const type_id& type) : internal_nested_type(type) {}
            [[nodiscard]] auto name() const -> std::string override { return L::const_qualified_type_name(internal_nested_type::name()); }
        };

        struct internal_template_specialized_type : public internal_nested_type
        {
            explicit internal_template_specialized_type(const type_id& type, const std::vector<type_id>& specializations = {})
                : internal_nested_type(type), m_specializations(specializations)
            {}

            [[nodiscard]] auto name() const -> std::string override
            {
                std::vector<std::string> specializations;
                for (const auto& specialization : m_specializations) {
                    specializations.emplace_back(specialization->name());
                }
                return L::template_specialized_type_name(internal_nested_type::name(), specializations);
            }

        private:
            std::vector<type_id> m_specializations;
        };
    };

    template<type_support L>
    static auto void_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::internal_void_type>();
    }

    template<type_support L>
    static auto error_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::internal_error_type>();
    }

    template<type_support L>
    static auto auto_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::internal_automatically_infered_type>();
    }

    template<type_support L>
    static auto named(const std::string& name) -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::internal_named_type>(name);
    }

    template<type_support L>
    static auto const_qualified(const typename type_system<L>::type_id& type) -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::internal_const_qualified_type>(type);
    }

    template<type_support L>
    static auto pointer(const typename type_system<L>::type_id& type) -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::internal_ptr_type>(type);
    }

    template<type_support L>
    static auto reference(const typename type_system<L>::type_id& type) -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::internal_ref_type>(type);
    }

    template<type_support L>
    static auto template_specailized(
        const typename type_system<L>::type_id& type,
        const std::vector<typename type_system<L>::type_id>& specializations
    ) -> typename type_system<L>::type_id {
        return type_system<L>::template make<typename type_system<L>::internal_template_specialized_type>(type, specializations);
    }

    // MARK: - Scalar Types
    template<type_support L>
    static auto bool_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::boolean_type>>();
    }

    template<type_support L>
    static auto char_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::char_type>>();
    }

    template<type_support L>
    static auto short_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::short_type>>();
    }

    template<type_support L>
    static auto long_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::long_type>>();
    }

    template<type_support L>
    static auto long_long_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::long_long_type>>();
    }

    template<type_support L>
    static auto uchar_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::uchar_type>>();
    }

    template<type_support L>
    static auto ushort_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::ushort_type>>();
    }

    template<type_support L>
    static auto ulong_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::ulong_type>>();
    }

    template<type_support L>
    static auto ulong_long_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::ulong_long_type>>();
    }

    template<type_support L>
    static auto int_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::int_type>>();
    }

    template<type_support L>
    static auto uint_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::uint_type>>();
    }

    template<type_support L>
    static auto float_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::float_type>>();
    }

    template<type_support L>
    static auto double_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::double_type>>();
    }

    template<type_support L>
    static auto long_double_type() -> typename type_system<L>::type_id
    {
        return type_system<L>::template make<typename type_system<L>::template internal_scalar_type<type::scalar::long_double_type>>();
    }

    // MARK: - Basic Types

    template<type_support L>
    static auto string_type() -> typename type_system<L>::type_id
    {
        return named<L>(L::string_type_name());
    }

}