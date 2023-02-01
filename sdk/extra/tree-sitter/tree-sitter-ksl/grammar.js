module.exports = grammar({
    name: 'KSL',
    rules: {
        source_file: $ => repeat(
            $.top_level_statement
        ),

        top_level_statement: $ => seq(choice(
            $.import_directive,
            $.definition,
        ), ';'),

        // Literals
        identifier_literal: $ => /[A-Za-z_][A-Za-z0-9_]*/,
        integer_literal: $ => /-?[0-9]+/,
        hex_literal: $ => /0x[0-9A-Fa-f]+/,
        percentage_literal: $ => /-?[0-9]+%/,

        resource_reference_literal: $ => choice('#auto', seq(
            '#',
            optional(seq(choice(
                seq(field('namespace', $.identifier_literal), '.', field('type', $.identifier_literal)),
                field('type', $.identifier_literal),
            ), '.')),
            field('id', $.integer_literal),
        )),

        string_literal: $ => seq(
            '"',
            repeat(choice(
                token.immediate(prec(1, /[^\\"\n]+/)),
            )),
            '"'
        ),

        // General Types
        type: $ => seq(
            field('name', choice(
                'Self',
                $.built_in_types,
                $.identifier_literal
            )),
            optional(field('is_reference', '&'))
        ),

        // Built in types
        built_in_types: $ => choice(
            $.built_in_array_type,
            $.built_in_map_type,
            'String', 'Integer', 'Boolean'
        ),

        built_in_array_type: $ => seq(
            'Array', '<', field('type', $.built_in_types), '>'
        ),
        built_in_map_type: $ => seq(
            'Map', '<', field('key_type', $.type), ',', field('value_type', $.type), '>'
        ),

        // Import
        kestrel_module_name: $ => choice('Kestrel'),
        import_directive: $ => seq(
            'import', choice(
                field('module', $.kestrel_module_name),
                field('reference', $.resource_reference_literal)
            )
        ),

        // Definitions
        definition: $ => choice(
            $.variable_definition,
            $.class_definition
        ),

        variable_definition: $ => seq(
            choice('var', 'val'),
            field('name', $.identifier_literal),
            optional(seq(':', field('type', $.type))),
            repeat(choice(
                field('get', $.variable_getter),
                field('set', $.variable_setter)
            ))
        ),
        variable_getter: $ => seq(
            'get', $.code_block
        ),
        variable_setter: $ => seq(
            'set', $.code_block
        ),

        class_definition: $ => seq(
            choice('model', 'class', 'namespace'),
            field('name', $.identifier_literal),
            $.class_body
        ),
        class_body: $ => seq(
            '{',
            repeat(seq(choice(
                $.variable_definition
            ), ';')),
            '}'
        ),

        // Code Blocks
        code_block: $ => seq(
            '{',
            repeat(seq($.expression, ';')),
            '}'
        ),

        expression: $ => prec.left(1, choice(
            $.identifier_literal,
            $.integer_literal,
            $.hex_literal,
            $.string_literal,
            $.percentage_literal,
            $.resource_reference_literal,
            $.return_expression,
            $.key_path,
            $.call_expression
        )),
        return_expression: $ => seq(
            'return', $.expression
        ),
        key_path: $ => prec.left(2, seq(
            field('member', choice(
                'self', $.identifier_literal, $.call_expression
            )),
            optional(seq(
                '.', $.key_path
            ))
        )),
        call_expression: $ => seq(
            $.key_path, '(', ')'
        )
    }
});
