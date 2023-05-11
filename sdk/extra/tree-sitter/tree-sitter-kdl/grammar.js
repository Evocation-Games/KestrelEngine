const PREC = {
  INCLUSIVE_OR: 0,
  EXCLUSIVE_OR: 1,
  BITWISE_AND: 2,
  SHIFT: 3,
  ADD: 4,
  MULTIPLY: 5,
  UNARY: 6,
  CALL: 7,
};

module.exports = grammar({
    name: 'KDL',
    rules: {
        source_file: $ => repeat($._top_level),
        
        _top_level: $ => choice(
            $.comment,
            $.directive,
            $.type_definition,
            $.resource_declaration,
            $.component,
            $.scene
        ),
        
        binary_type: $ => choice(
            'CSTR', 'PSTR', 'CHAR',
            'DBYT', 'DWRD', 'DLNG', 'DQWD',
            'HBYT', 'HWRD', 'HLNG', 'HQWD',
            'BOOL', 'BBIT', 'RECT',
            'HEXD', 'RSRC',
            'LSTB', 'LSTC', 'LSTE', 'OCNT',
            /C[0-9A-Fa-f]{3}/,
            seq(
                field('name', choice('Nested', 'Reference')),
                '<',
                field('type', $.identifier_literal),
                '>'
            )
        ),

        // Comments
        comment: $ => seq('`', /(`(.|\r?\n)|[^`\n])*/),
        
        // Decorators
        decorator: $ => seq(
            field('name', /@[A-Za-z_][A-Za-z0-9_]*/),
            optional(seq(
                '(',
                choice(
                    seq(
                        choice($.value, $.identifier_literal),
                        repeat(seq(',', choice($.value, $.identifier_literal)))
                    ),
                    choice($.value, $.identifier_literal)
                ),
                ')'
            )),
        ),
        
        // Directives
        directive: $ => seq(choice(
            $.out_directive,
            $.format_directive,
            $.metadata_directive,
            $.import_directive,
            $.variable_directive,
            $.function_directive,
        ), ';'),
        
        out_directive: $ => seq(
            '@out', repeat($.string_literal)
        ),
        
        metadata_directive: $ => seq(
            choice(
                '@project', '@module', '@author', '@copyright', '@version', '@license', '@email'
            ), $.string_literal
        ),
        
        format_directive: $ => seq(
            '@format', 
            choice('requires', 'set'),
            choice('rsrc', 'classic', 'extended', 'kestrel', 'rez'),
        ),
        
        import_directive: $ => seq(
            '@import',
            choice(
                field('source_file', $.string_literal),
                field('module', $.identifier_literal)
            ),
        ),
        
        variable_directive: $ => seq(
            choice('@var', '@const'),
            field('name', choice($.variable, $.identifier_literal)),
            '=',
            $.expression
        ),
        
        function_directive: $ => seq(
            '@function',
            field('name', $.identifier_literal),
            '=',
            $.expression
        ),
        
        // Type Definitions
        type_definition: $ => seq(
            optional(field('decorators', repeat($.decorator))),
            'type',
            field('name', $.identifier_literal), ':', field('code', $.string_literal),
            field('definition', $.type_definition_body),
            ';'
        ),
        
        type_definition_body: $ => seq(
            '{',
            repeat(choice(
                $.binary_template,
                $.field_definition,
                $.type_constructor
            )),
            '}'
        ),
        
        binary_template: $ => seq(
            'template', '{',
            repeat($.binary_template_field),
            '}', ';'
        ),
        
        binary_template_field: $ => seq(
            field('type', $.binary_type),
            field('label', $.identifier_literal),
            ';'
        ),

        type_constructor: $ => seq(
            'constructor', '(',
            optional(seq(
                $.identifier_literal,
                repeat(seq(',', $.identifier_literal))
            )),
            ')',
            $.type_constructor_body
        ),

        type_constructor_body: $ => seq(
            '{',
            repeat(
                seq(
                    field('lhs', choice(
                        seq('template', '.', $.identifier_literal),
                        $.expression
                    )),
                    '=',
                    $.expression,
                    ';'
                )
            ),
            '};'
        ),
        
        field_definition: $ => seq(
            optional(repeat($.decorator)),
            'field', '(', field('name', $.string_literal), ')',
            optional($.field_repeatable),
            optional(choice(
                field('values', $.field_definition_values),
                $.field_value_default_value,
            )),
            ';'
        ),

        field_repeatable: $ => seq(
            'repeatable', '<',
            choice(
                seq($.integer_literal),
                seq($.integer_literal, ',', $.integer_literal),
                seq($.identifier_literal),
                seq($.integer_literal, ',', $.identifier_literal),
                seq($.integer_literal, ',', $.integer_literal, ',',  $.identifier_literal)
            ),
            '>'
        ),
        
        field_explicit_type: $ => seq(
            'as',
            optional(seq(
                field('explicit_type', $.identifier_literal),
                optional(seq(
                    '<',
                    choice(
                        choice($.value, $.identifier_literal),
                        seq(choice($.value, $.identifier_literal), repeat(seq(
                            ',', choice($.value, $.identifier_literal)
                        )))
                    ),
                    '>'
                ))
            )),
            optional(field('is_reference', '&')),
        ),
        
        field_definition_values: $ => seq(
            '{',
            repeat(seq(
                optional(repeat($.decorator)),
                field('name', $.identifier_literal),
                optional($.field_explicit_type),
                optional($.field_value_default_value),
                optional($.field_definition_value_symbol_list),
                ';'
            )),
            '}'
        ),

        field_value_default_value: $ => seq(
            '=',
            field('default_value', choice($.value, $.identifier_literal)),
        ),
        
        field_definition_value_symbol_list: $ => seq(
            '[',
            repeat(seq(
                optional(repeat($.decorator)),
                field('name', $.identifier_literal),
                '=',
                field('value', $.value),
                optional(',')
            )),
            ']'
        ),
        
        // Resource Declarations
        resource_declaration: $ => seq(
            optional(repeat($.decorator)),
            'declare', field('type', $.type_identifier),
            $.resource_declaration_body,
            ';'
        ),
        
        type_identifier: $ => seq(
            optional(seq(field('namespace', $.identifier_literal), '.')),
            field('type', $.identifier_literal),
        ),
        
        resource_declaration_body: $ => seq(
            '{',
            repeat(seq(
                optional(repeat($.decorator)),
                choice(
                    $.new_resource,
                    $.override_resource,
                    $.duplicate_resource
                )
            )),
            '}'
        ),
        
        new_resource: $ => seq(
            'new',
            optional($.argument_list),
            $.resource_declaration_values,
            ';'
        ),
        
        override_resource: $ => seq(
            'override',
            $.argument_list,
            $.resource_declaration_values,
            ';'
        ),
        
        duplicate_resource: $ => seq(
            'duplicate',
            '(',
                field('source', $.resource_reference_literal),
                'as',
                field('copy', $.resource_reference_literal),
                optional(field('name', $.string_literal)),
            ')',
            $.resource_declaration_values,
            ';'
        ),
        
        resource_declaration_values: $ => seq(
            '{',
            repeat(
                $.resource_declaration_field_value
            ),
            '}'
        ),
        
        resource_declaration_field_value: $ => seq(
            optional(repeat($.decorator)),
            field('field', $.identifier_literal), '=',
            choice(
                $.resource_declaration_field_value_import,
                choice($.new_resource, seq($.expression, repeat(seq(',', $.expression))))
            ),
            ';'
        ),

        resource_declaration_field_value_import: $ => seq('import', repeat($.string_literal)),
        
        // Components
        component: $ => seq(
            optional(repeat($.decorator)),
            'component',
            $.component_configuration,
            field('name', $.identifier_literal),
            $.component_body,
            ';'
        ),

        component_configuration: $ => seq(
            '<',
            optional(seq(
                field('ns', $.identifier_literal), '.'
            )),
            field('type', $.identifier_literal), ',',
            field('id', $.expression),
            '>'
        ),

        component_body: $ => seq(
            '{',
            repeat(seq(choice(
                $.component_files,
                $.component_types
            ), ';')),
            '}'
        ),

        component_types: $ => seq(
            'api_export', '{',
                repeat($.component_type_reference),
            '}'
        ),

        component_files: $ => seq(
            'files',
            '(', field('path', $.string_literal), ')', '{',
                repeat($.component_file_reference),
            '}'
        ),

        component_file_reference: $ => seq(
            $.named_file_reference,
            optional($.file_reference_attributes),
            ';'
        ),

        component_type_reference: $ => seq(
            field('type', $.identifier_literal), ';'
        ),

        named_file_reference: $ => seq(
            field('file', $.string_literal),
            optional(seq(
                '->',
                field('name', $.string_literal)
            ))
        ),

        file_reference_attributes: $ => seq(
            '(',
            $.key_value_pair,
            optional(repeat(seq(',', $.key_value_pair))),
            ')'
        ),

        key_value_pair: $ => seq(
            field('key', $.identifier_literal),
            optional(seq(
                '=',
                field('value', $.expression)
            ))
        ),

        // Scenes
        scene: $ => seq(
            optional(repeat($.decorator)),
            choice('scene', 'dialog'), optional(seq('<', field('id', choice($.resource_reference_literal, $.identifier_literal)), '>')),
            field('name', $.identifier_literal),
            $.scene_body,
            ';'
        ),

        scene_body: $ => seq(
          '{',
          repeat(seq(choice(
              $.scene_property,
              $.scene_element,
              $.scene_background
          ), ';')),
          '}'
        ),

        scene_property: $ => seq(
            field('name', choice('Title', 'Flags', 'Frame', 'Size')),
            '=',
            field('value', seq($.expression, repeat(seq(',', $.expression))))
        ),

        scene_background: $ => seq(
            'Background', '=',
            choice($.expression, seq(
                '{',
                repeat(seq($.scene_background_component, ';')),
                '}'
            ))
        ),

        scene_background_component: $ => seq(
            field('name', choice('Top', 'Fill', 'Bottom')),
            '=',
            field('value', $.expression)
        ),

        scene_element: $ => seq(
            field('type', choice(
                'Button', 'Label', 'Checkbox', 'PopupButton', 'Canvas', 'Image', 'Sprite',
                'TextField', 'TextArea', 'Grid', 'List', 'Separator', 'Slider', 'RadioButton',
                'TimedTrigger', 'KeyTrigger', 'VerticalSpacer', 'HorizontalSpacer', 'Position',
                'ScrollArea'
            )),
            '(', field('name', $.string_literal), ')',
            $.scene_element_body,
            ';'
        ),

        scene_element_body: $ => seq(
            '{',
            repeat(seq($.scene_element_property, ';')),
            '}'
        ),

        scene_element_property: $ => seq(
            field('name', choice(
                'ID', 'Frame', 'Y', 'X', 'Size', 'Color', 'BackgroundColor', 'SelectionColor', 'BorderColor',
                'SecondaryColor', 'Font', 'FontSize', 'Hidden', 'Disabled', 'Weight', 'HorizontalAlignment',
                'VerticalAlignment', 'AxisOrigin', 'Action', 'Value'
            )),
            '=',
            field('value', seq($.expression, repeat(seq(',', $.expression))))
        ),

        // Values
        value: $ => choice(
            $.hex_literal,
            $.string_literal,
            $.integer_literal,
            $.percentage_literal,
            $.resource_reference_literal,
            $.variable,
        ),
        
        variable: $ => /\$[A-Za-z0-9_]*/,
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
        
        // Expressions
        expression: $ => choice(
            $.binary_expression,
            $.unary_expression,
            $.call_expression,
            $.string_literal,
            $.integer_literal,
            $.percentage_literal,
            $.resource_reference_literal,
            $.identifier_literal,
            $.variable,
            $.parenthesized_expression,
        ),
        
        unary_expression: $ => prec.left(PREC.UNARY, seq(
            field('operator', choice('~')),
            field('argument', $.expression)
        )),
        
        binary_expression: $ => {
            const table = [
                ['+', PREC.ADD],
                ['-', PREC.ADD],
                ['*', PREC.MULTIPLY],
                ['/', PREC.MULTIPLY],
                ['^', PREC.EXCLUSIVE_OR],
                ['|', PREC.INCLUSIVE_OR],
                ['&', PREC.BITWISE_AND],
                ['<<', PREC.SHIFT],
                ['>>', PREC.SHIFT],
            ];
            
            return choice(...table.map(([operator, precedence]) => {
                return prec.left(precedence, seq(
                    field('lhs', $.expression),
                    field('operator', operator),
                    field('rhs', $.expression)
                ))
            }));
        },
        
        call_expression: $ => prec(PREC.CALL, seq(
            field('function', $.identifier_literal),
            field('arguments', $.argument_list)
        )),
        
        parenthesized_expression: $ => seq(
            '(',
            choice($.expression, $.comma_expression),
            ')'
        ),
        
        comma_expression: $ => seq(
            field('left', $.expression),
            ',',
            field('right', choice($.expression, $.comma_expression))
        ),
        
        argument_list: $ => seq(
            '(',
            choice(
                $.expression,
                seq($.expression, repeat(seq(
                    ',', $.expression
                )))
            ),
            ')'
        ),
        
    }
});
