#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#ifdef _MSC_VER
#pragma optimize("", off)
#elif defined(__clang__)
#pragma clang optimize off
#elif defined(__GNUC__)
#pragma GCC optimize ("O0")
#endif

#define LANGUAGE_VERSION 14
#define STATE_COUNT 427
#define LARGE_STATE_COUNT 2
#define SYMBOL_COUNT 199
#define ALIAS_COUNT 0
#define TOKEN_COUNT 122
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 30
#define MAX_ALIAS_SEQUENCE_LENGTH 9
#define PRODUCTION_ID_COUNT 59

enum {
  anon_sym_CSTR = 1,
  anon_sym_PSTR = 2,
  anon_sym_CHAR = 3,
  anon_sym_DBYT = 4,
  anon_sym_DWRD = 5,
  anon_sym_DLNG = 6,
  anon_sym_DQAD = 7,
  anon_sym_HBYT = 8,
  anon_sym_HWRD = 9,
  anon_sym_HLNG = 10,
  anon_sym_HQAD = 11,
  anon_sym_BOOL = 12,
  anon_sym_BBIT = 13,
  anon_sym_RECT = 14,
  anon_sym_HEXD = 15,
  anon_sym_RSRC = 16,
  anon_sym_LSTB = 17,
  anon_sym_LSTC = 18,
  anon_sym_LSTE = 19,
  anon_sym_OCNT = 20,
  aux_sym_binary_type_token1 = 21,
  anon_sym_Nested = 22,
  anon_sym_Reference = 23,
  anon_sym_LT = 24,
  anon_sym_GT = 25,
  sym_comment = 26,
  aux_sym_decorator_token1 = 27,
  anon_sym_LPAREN = 28,
  anon_sym_COMMA = 29,
  anon_sym_RPAREN = 30,
  anon_sym_SEMI = 31,
  anon_sym_ATout = 32,
  anon_sym_ATproject = 33,
  anon_sym_ATmodule = 34,
  anon_sym_ATauthor = 35,
  anon_sym_ATcopyright = 36,
  anon_sym_ATversion = 37,
  anon_sym_ATlicense = 38,
  anon_sym_ATemail = 39,
  anon_sym_ATformat = 40,
  anon_sym_requires = 41,
  anon_sym_set = 42,
  anon_sym_rsrc = 43,
  anon_sym_classic = 44,
  anon_sym_extended = 45,
  anon_sym_kestrel = 46,
  anon_sym_rez = 47,
  anon_sym_ATimport = 48,
  anon_sym_ATvar = 49,
  anon_sym_ATconst = 50,
  anon_sym_EQ = 51,
  anon_sym_ATfunction = 52,
  anon_sym_type = 53,
  anon_sym_COLON = 54,
  anon_sym_LBRACE = 55,
  anon_sym_RBRACE = 56,
  anon_sym_template = 57,
  anon_sym_constructor = 58,
  anon_sym_DOT = 59,
  anon_sym_RBRACE_SEMI = 60,
  anon_sym_field = 61,
  anon_sym_repeatable = 62,
  anon_sym_as = 63,
  anon_sym_AMP = 64,
  anon_sym_LBRACK = 65,
  anon_sym_RBRACK = 66,
  anon_sym_declare = 67,
  anon_sym_new = 68,
  anon_sym_override = 69,
  anon_sym_duplicate = 70,
  anon_sym_import = 71,
  anon_sym_component = 72,
  anon_sym_api_export = 73,
  anon_sym_files = 74,
  anon_sym_DASH_GT = 75,
  anon_sym_scene = 76,
  anon_sym_Title = 77,
  anon_sym_Flags = 78,
  anon_sym_Size = 79,
  anon_sym_Button = 80,
  anon_sym_Label = 81,
  anon_sym_TextArea = 82,
  anon_sym_Image = 83,
  anon_sym_TextField = 84,
  anon_sym_Checkbox = 85,
  anon_sym_List = 86,
  anon_sym_ScrollArea = 87,
  anon_sym_Grid = 88,
  anon_sym_LabeledList = 89,
  anon_sym_Canvas = 90,
  anon_sym_Sprite = 91,
  anon_sym_PopupButton = 92,
  anon_sym_Slider = 93,
  anon_sym_Table = 94,
  anon_sym_Box = 95,
  anon_sym_Radio = 96,
  anon_sym_TabBar = 97,
  anon_sym_Separator = 98,
  anon_sym_Spacer = 99,
  anon_sym_Position = 100,
  anon_sym_Frame = 101,
  anon_sym_Value = 102,
  anon_sym_Action = 103,
  sym_variable = 104,
  sym_identifier_literal = 105,
  sym_integer_literal = 106,
  sym_hex_literal = 107,
  sym_percentage_literal = 108,
  anon_sym_POUNDauto = 109,
  anon_sym_POUND = 110,
  anon_sym_DQUOTE = 111,
  aux_sym_string_literal_token1 = 112,
  anon_sym_TILDE = 113,
  anon_sym_PLUS = 114,
  anon_sym_DASH = 115,
  anon_sym_STAR = 116,
  anon_sym_SLASH = 117,
  anon_sym_CARET = 118,
  anon_sym_PIPE = 119,
  anon_sym_LT_LT = 120,
  anon_sym_GT_GT = 121,
  sym_source_file = 122,
  sym__top_level = 123,
  sym_binary_type = 124,
  sym_decorator = 125,
  sym_directive = 126,
  sym_out_directive = 127,
  sym_metadata_directive = 128,
  sym_format_directive = 129,
  sym_import_directive = 130,
  sym_variable_directive = 131,
  sym_function_directive = 132,
  sym_type_definition = 133,
  sym_type_definition_body = 134,
  sym_binary_template = 135,
  sym_binary_template_field = 136,
  sym_type_constructor = 137,
  sym_type_constructor_body = 138,
  sym_field_definition = 139,
  sym_field_repeatable = 140,
  sym_field_explicit_type = 141,
  sym_field_definition_values = 142,
  sym_field_definition_value_symbol_list = 143,
  sym_resource_declaration = 144,
  sym_type_identifier = 145,
  sym_resource_declaration_body = 146,
  sym_new_resource = 147,
  sym_override_resource = 148,
  sym_duplicate_resource = 149,
  sym_resource_declaration_values = 150,
  sym_resource_declaration_field_value = 151,
  sym_resource_declaration_field_value_import = 152,
  sym_component = 153,
  sym_component_configuration = 154,
  sym_component_body = 155,
  sym_component_types = 156,
  sym_component_files = 157,
  sym_component_file_reference = 158,
  sym_component_type_reference = 159,
  sym_named_file_reference = 160,
  sym_file_reference_attributes = 161,
  sym_key_value_pair = 162,
  sym_scene = 163,
  sym_scene_body = 164,
  sym_scene_property = 165,
  sym_scene_element = 166,
  sym_scene_element_body = 167,
  sym_scene_element_property = 168,
  sym_value = 169,
  sym_resource_reference_literal = 170,
  sym_string_literal = 171,
  sym_expression = 172,
  sym_unary_expression = 173,
  sym_binary_expression = 174,
  sym_call_expression = 175,
  sym_parenthesized_expression = 176,
  sym_comma_expression = 177,
  sym_argument_list = 178,
  aux_sym_source_file_repeat1 = 179,
  aux_sym_decorator_repeat1 = 180,
  aux_sym_out_directive_repeat1 = 181,
  aux_sym_type_definition_repeat1 = 182,
  aux_sym_type_definition_body_repeat1 = 183,
  aux_sym_binary_template_repeat1 = 184,
  aux_sym_type_constructor_repeat1 = 185,
  aux_sym_type_constructor_body_repeat1 = 186,
  aux_sym_field_definition_values_repeat1 = 187,
  aux_sym_field_definition_value_symbol_list_repeat1 = 188,
  aux_sym_resource_declaration_body_repeat1 = 189,
  aux_sym_resource_declaration_values_repeat1 = 190,
  aux_sym_resource_declaration_field_value_repeat1 = 191,
  aux_sym_component_body_repeat1 = 192,
  aux_sym_component_types_repeat1 = 193,
  aux_sym_component_files_repeat1 = 194,
  aux_sym_file_reference_attributes_repeat1 = 195,
  aux_sym_scene_body_repeat1 = 196,
  aux_sym_scene_element_body_repeat1 = 197,
  aux_sym_string_literal_repeat1 = 198,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [anon_sym_CSTR] = "CSTR",
  [anon_sym_PSTR] = "PSTR",
  [anon_sym_CHAR] = "CHAR",
  [anon_sym_DBYT] = "DBYT",
  [anon_sym_DWRD] = "DWRD",
  [anon_sym_DLNG] = "DLNG",
  [anon_sym_DQAD] = "DQAD",
  [anon_sym_HBYT] = "HBYT",
  [anon_sym_HWRD] = "HWRD",
  [anon_sym_HLNG] = "HLNG",
  [anon_sym_HQAD] = "HQAD",
  [anon_sym_BOOL] = "BOOL",
  [anon_sym_BBIT] = "BBIT",
  [anon_sym_RECT] = "RECT",
  [anon_sym_HEXD] = "HEXD",
  [anon_sym_RSRC] = "RSRC",
  [anon_sym_LSTB] = "LSTB",
  [anon_sym_LSTC] = "LSTC",
  [anon_sym_LSTE] = "LSTE",
  [anon_sym_OCNT] = "OCNT",
  [aux_sym_binary_type_token1] = "binary_type_token1",
  [anon_sym_Nested] = "Nested",
  [anon_sym_Reference] = "Reference",
  [anon_sym_LT] = "<",
  [anon_sym_GT] = ">",
  [sym_comment] = "comment",
  [aux_sym_decorator_token1] = "decorator_token1",
  [anon_sym_LPAREN] = "(",
  [anon_sym_COMMA] = ",",
  [anon_sym_RPAREN] = ")",
  [anon_sym_SEMI] = ";",
  [anon_sym_ATout] = "@out",
  [anon_sym_ATproject] = "@project",
  [anon_sym_ATmodule] = "@module",
  [anon_sym_ATauthor] = "@author",
  [anon_sym_ATcopyright] = "@copyright",
  [anon_sym_ATversion] = "@version",
  [anon_sym_ATlicense] = "@license",
  [anon_sym_ATemail] = "@email",
  [anon_sym_ATformat] = "@format",
  [anon_sym_requires] = "requires",
  [anon_sym_set] = "set",
  [anon_sym_rsrc] = "rsrc",
  [anon_sym_classic] = "classic",
  [anon_sym_extended] = "extended",
  [anon_sym_kestrel] = "kestrel",
  [anon_sym_rez] = "rez",
  [anon_sym_ATimport] = "@import",
  [anon_sym_ATvar] = "@var",
  [anon_sym_ATconst] = "@const",
  [anon_sym_EQ] = "=",
  [anon_sym_ATfunction] = "@function",
  [anon_sym_type] = "type",
  [anon_sym_COLON] = ":",
  [anon_sym_LBRACE] = "{",
  [anon_sym_RBRACE] = "}",
  [anon_sym_template] = "template",
  [anon_sym_constructor] = "constructor",
  [anon_sym_DOT] = ".",
  [anon_sym_RBRACE_SEMI] = "};",
  [anon_sym_field] = "field",
  [anon_sym_repeatable] = "repeatable",
  [anon_sym_as] = "as",
  [anon_sym_AMP] = "&",
  [anon_sym_LBRACK] = "[",
  [anon_sym_RBRACK] = "]",
  [anon_sym_declare] = "declare",
  [anon_sym_new] = "new",
  [anon_sym_override] = "override",
  [anon_sym_duplicate] = "duplicate",
  [anon_sym_import] = "import",
  [anon_sym_component] = "component",
  [anon_sym_api_export] = "api_export",
  [anon_sym_files] = "files",
  [anon_sym_DASH_GT] = "->",
  [anon_sym_scene] = "scene",
  [anon_sym_Title] = "Title",
  [anon_sym_Flags] = "Flags",
  [anon_sym_Size] = "Size",
  [anon_sym_Button] = "Button",
  [anon_sym_Label] = "Label",
  [anon_sym_TextArea] = "TextArea",
  [anon_sym_Image] = "Image",
  [anon_sym_TextField] = "TextField",
  [anon_sym_Checkbox] = "Checkbox",
  [anon_sym_List] = "List",
  [anon_sym_ScrollArea] = "ScrollArea",
  [anon_sym_Grid] = "Grid",
  [anon_sym_LabeledList] = "LabeledList",
  [anon_sym_Canvas] = "Canvas",
  [anon_sym_Sprite] = "Sprite",
  [anon_sym_PopupButton] = "PopupButton",
  [anon_sym_Slider] = "Slider",
  [anon_sym_Table] = "Table",
  [anon_sym_Box] = "Box",
  [anon_sym_Radio] = "Radio",
  [anon_sym_TabBar] = "TabBar",
  [anon_sym_Separator] = "Separator",
  [anon_sym_Spacer] = "Spacer",
  [anon_sym_Position] = "Position",
  [anon_sym_Frame] = "Frame",
  [anon_sym_Value] = "Value",
  [anon_sym_Action] = "Action",
  [sym_variable] = "variable",
  [sym_identifier_literal] = "identifier_literal",
  [sym_integer_literal] = "integer_literal",
  [sym_hex_literal] = "hex_literal",
  [sym_percentage_literal] = "percentage_literal",
  [anon_sym_POUNDauto] = "#auto",
  [anon_sym_POUND] = "#",
  [anon_sym_DQUOTE] = "\"",
  [aux_sym_string_literal_token1] = "string_literal_token1",
  [anon_sym_TILDE] = "~",
  [anon_sym_PLUS] = "+",
  [anon_sym_DASH] = "-",
  [anon_sym_STAR] = "*",
  [anon_sym_SLASH] = "/",
  [anon_sym_CARET] = "^",
  [anon_sym_PIPE] = "|",
  [anon_sym_LT_LT] = "<<",
  [anon_sym_GT_GT] = ">>",
  [sym_source_file] = "source_file",
  [sym__top_level] = "_top_level",
  [sym_binary_type] = "binary_type",
  [sym_decorator] = "decorator",
  [sym_directive] = "directive",
  [sym_out_directive] = "out_directive",
  [sym_metadata_directive] = "metadata_directive",
  [sym_format_directive] = "format_directive",
  [sym_import_directive] = "import_directive",
  [sym_variable_directive] = "variable_directive",
  [sym_function_directive] = "function_directive",
  [sym_type_definition] = "type_definition",
  [sym_type_definition_body] = "type_definition_body",
  [sym_binary_template] = "binary_template",
  [sym_binary_template_field] = "binary_template_field",
  [sym_type_constructor] = "type_constructor",
  [sym_type_constructor_body] = "type_constructor_body",
  [sym_field_definition] = "field_definition",
  [sym_field_repeatable] = "field_repeatable",
  [sym_field_explicit_type] = "field_explicit_type",
  [sym_field_definition_values] = "field_definition_values",
  [sym_field_definition_value_symbol_list] = "field_definition_value_symbol_list",
  [sym_resource_declaration] = "resource_declaration",
  [sym_type_identifier] = "type_identifier",
  [sym_resource_declaration_body] = "resource_declaration_body",
  [sym_new_resource] = "new_resource",
  [sym_override_resource] = "override_resource",
  [sym_duplicate_resource] = "duplicate_resource",
  [sym_resource_declaration_values] = "resource_declaration_values",
  [sym_resource_declaration_field_value] = "resource_declaration_field_value",
  [sym_resource_declaration_field_value_import] = "resource_declaration_field_value_import",
  [sym_component] = "component",
  [sym_component_configuration] = "component_configuration",
  [sym_component_body] = "component_body",
  [sym_component_types] = "component_types",
  [sym_component_files] = "component_files",
  [sym_component_file_reference] = "component_file_reference",
  [sym_component_type_reference] = "component_type_reference",
  [sym_named_file_reference] = "named_file_reference",
  [sym_file_reference_attributes] = "file_reference_attributes",
  [sym_key_value_pair] = "key_value_pair",
  [sym_scene] = "scene",
  [sym_scene_body] = "scene_body",
  [sym_scene_property] = "scene_property",
  [sym_scene_element] = "scene_element",
  [sym_scene_element_body] = "scene_element_body",
  [sym_scene_element_property] = "scene_element_property",
  [sym_value] = "value",
  [sym_resource_reference_literal] = "resource_reference_literal",
  [sym_string_literal] = "string_literal",
  [sym_expression] = "expression",
  [sym_unary_expression] = "unary_expression",
  [sym_binary_expression] = "binary_expression",
  [sym_call_expression] = "call_expression",
  [sym_parenthesized_expression] = "parenthesized_expression",
  [sym_comma_expression] = "comma_expression",
  [sym_argument_list] = "argument_list",
  [aux_sym_source_file_repeat1] = "source_file_repeat1",
  [aux_sym_decorator_repeat1] = "decorator_repeat1",
  [aux_sym_out_directive_repeat1] = "out_directive_repeat1",
  [aux_sym_type_definition_repeat1] = "type_definition_repeat1",
  [aux_sym_type_definition_body_repeat1] = "type_definition_body_repeat1",
  [aux_sym_binary_template_repeat1] = "binary_template_repeat1",
  [aux_sym_type_constructor_repeat1] = "type_constructor_repeat1",
  [aux_sym_type_constructor_body_repeat1] = "type_constructor_body_repeat1",
  [aux_sym_field_definition_values_repeat1] = "field_definition_values_repeat1",
  [aux_sym_field_definition_value_symbol_list_repeat1] = "field_definition_value_symbol_list_repeat1",
  [aux_sym_resource_declaration_body_repeat1] = "resource_declaration_body_repeat1",
  [aux_sym_resource_declaration_values_repeat1] = "resource_declaration_values_repeat1",
  [aux_sym_resource_declaration_field_value_repeat1] = "resource_declaration_field_value_repeat1",
  [aux_sym_component_body_repeat1] = "component_body_repeat1",
  [aux_sym_component_types_repeat1] = "component_types_repeat1",
  [aux_sym_component_files_repeat1] = "component_files_repeat1",
  [aux_sym_file_reference_attributes_repeat1] = "file_reference_attributes_repeat1",
  [aux_sym_scene_body_repeat1] = "scene_body_repeat1",
  [aux_sym_scene_element_body_repeat1] = "scene_element_body_repeat1",
  [aux_sym_string_literal_repeat1] = "string_literal_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [anon_sym_CSTR] = anon_sym_CSTR,
  [anon_sym_PSTR] = anon_sym_PSTR,
  [anon_sym_CHAR] = anon_sym_CHAR,
  [anon_sym_DBYT] = anon_sym_DBYT,
  [anon_sym_DWRD] = anon_sym_DWRD,
  [anon_sym_DLNG] = anon_sym_DLNG,
  [anon_sym_DQAD] = anon_sym_DQAD,
  [anon_sym_HBYT] = anon_sym_HBYT,
  [anon_sym_HWRD] = anon_sym_HWRD,
  [anon_sym_HLNG] = anon_sym_HLNG,
  [anon_sym_HQAD] = anon_sym_HQAD,
  [anon_sym_BOOL] = anon_sym_BOOL,
  [anon_sym_BBIT] = anon_sym_BBIT,
  [anon_sym_RECT] = anon_sym_RECT,
  [anon_sym_HEXD] = anon_sym_HEXD,
  [anon_sym_RSRC] = anon_sym_RSRC,
  [anon_sym_LSTB] = anon_sym_LSTB,
  [anon_sym_LSTC] = anon_sym_LSTC,
  [anon_sym_LSTE] = anon_sym_LSTE,
  [anon_sym_OCNT] = anon_sym_OCNT,
  [aux_sym_binary_type_token1] = aux_sym_binary_type_token1,
  [anon_sym_Nested] = anon_sym_Nested,
  [anon_sym_Reference] = anon_sym_Reference,
  [anon_sym_LT] = anon_sym_LT,
  [anon_sym_GT] = anon_sym_GT,
  [sym_comment] = sym_comment,
  [aux_sym_decorator_token1] = aux_sym_decorator_token1,
  [anon_sym_LPAREN] = anon_sym_LPAREN,
  [anon_sym_COMMA] = anon_sym_COMMA,
  [anon_sym_RPAREN] = anon_sym_RPAREN,
  [anon_sym_SEMI] = anon_sym_SEMI,
  [anon_sym_ATout] = anon_sym_ATout,
  [anon_sym_ATproject] = anon_sym_ATproject,
  [anon_sym_ATmodule] = anon_sym_ATmodule,
  [anon_sym_ATauthor] = anon_sym_ATauthor,
  [anon_sym_ATcopyright] = anon_sym_ATcopyright,
  [anon_sym_ATversion] = anon_sym_ATversion,
  [anon_sym_ATlicense] = anon_sym_ATlicense,
  [anon_sym_ATemail] = anon_sym_ATemail,
  [anon_sym_ATformat] = anon_sym_ATformat,
  [anon_sym_requires] = anon_sym_requires,
  [anon_sym_set] = anon_sym_set,
  [anon_sym_rsrc] = anon_sym_rsrc,
  [anon_sym_classic] = anon_sym_classic,
  [anon_sym_extended] = anon_sym_extended,
  [anon_sym_kestrel] = anon_sym_kestrel,
  [anon_sym_rez] = anon_sym_rez,
  [anon_sym_ATimport] = anon_sym_ATimport,
  [anon_sym_ATvar] = anon_sym_ATvar,
  [anon_sym_ATconst] = anon_sym_ATconst,
  [anon_sym_EQ] = anon_sym_EQ,
  [anon_sym_ATfunction] = anon_sym_ATfunction,
  [anon_sym_type] = anon_sym_type,
  [anon_sym_COLON] = anon_sym_COLON,
  [anon_sym_LBRACE] = anon_sym_LBRACE,
  [anon_sym_RBRACE] = anon_sym_RBRACE,
  [anon_sym_template] = anon_sym_template,
  [anon_sym_constructor] = anon_sym_constructor,
  [anon_sym_DOT] = anon_sym_DOT,
  [anon_sym_RBRACE_SEMI] = anon_sym_RBRACE_SEMI,
  [anon_sym_field] = anon_sym_field,
  [anon_sym_repeatable] = anon_sym_repeatable,
  [anon_sym_as] = anon_sym_as,
  [anon_sym_AMP] = anon_sym_AMP,
  [anon_sym_LBRACK] = anon_sym_LBRACK,
  [anon_sym_RBRACK] = anon_sym_RBRACK,
  [anon_sym_declare] = anon_sym_declare,
  [anon_sym_new] = anon_sym_new,
  [anon_sym_override] = anon_sym_override,
  [anon_sym_duplicate] = anon_sym_duplicate,
  [anon_sym_import] = anon_sym_import,
  [anon_sym_component] = anon_sym_component,
  [anon_sym_api_export] = anon_sym_api_export,
  [anon_sym_files] = anon_sym_files,
  [anon_sym_DASH_GT] = anon_sym_DASH_GT,
  [anon_sym_scene] = anon_sym_scene,
  [anon_sym_Title] = anon_sym_Title,
  [anon_sym_Flags] = anon_sym_Flags,
  [anon_sym_Size] = anon_sym_Size,
  [anon_sym_Button] = anon_sym_Button,
  [anon_sym_Label] = anon_sym_Label,
  [anon_sym_TextArea] = anon_sym_TextArea,
  [anon_sym_Image] = anon_sym_Image,
  [anon_sym_TextField] = anon_sym_TextField,
  [anon_sym_Checkbox] = anon_sym_Checkbox,
  [anon_sym_List] = anon_sym_List,
  [anon_sym_ScrollArea] = anon_sym_ScrollArea,
  [anon_sym_Grid] = anon_sym_Grid,
  [anon_sym_LabeledList] = anon_sym_LabeledList,
  [anon_sym_Canvas] = anon_sym_Canvas,
  [anon_sym_Sprite] = anon_sym_Sprite,
  [anon_sym_PopupButton] = anon_sym_PopupButton,
  [anon_sym_Slider] = anon_sym_Slider,
  [anon_sym_Table] = anon_sym_Table,
  [anon_sym_Box] = anon_sym_Box,
  [anon_sym_Radio] = anon_sym_Radio,
  [anon_sym_TabBar] = anon_sym_TabBar,
  [anon_sym_Separator] = anon_sym_Separator,
  [anon_sym_Spacer] = anon_sym_Spacer,
  [anon_sym_Position] = anon_sym_Position,
  [anon_sym_Frame] = anon_sym_Frame,
  [anon_sym_Value] = anon_sym_Value,
  [anon_sym_Action] = anon_sym_Action,
  [sym_variable] = sym_variable,
  [sym_identifier_literal] = sym_identifier_literal,
  [sym_integer_literal] = sym_integer_literal,
  [sym_hex_literal] = sym_hex_literal,
  [sym_percentage_literal] = sym_percentage_literal,
  [anon_sym_POUNDauto] = anon_sym_POUNDauto,
  [anon_sym_POUND] = anon_sym_POUND,
  [anon_sym_DQUOTE] = anon_sym_DQUOTE,
  [aux_sym_string_literal_token1] = aux_sym_string_literal_token1,
  [anon_sym_TILDE] = anon_sym_TILDE,
  [anon_sym_PLUS] = anon_sym_PLUS,
  [anon_sym_DASH] = anon_sym_DASH,
  [anon_sym_STAR] = anon_sym_STAR,
  [anon_sym_SLASH] = anon_sym_SLASH,
  [anon_sym_CARET] = anon_sym_CARET,
  [anon_sym_PIPE] = anon_sym_PIPE,
  [anon_sym_LT_LT] = anon_sym_LT_LT,
  [anon_sym_GT_GT] = anon_sym_GT_GT,
  [sym_source_file] = sym_source_file,
  [sym__top_level] = sym__top_level,
  [sym_binary_type] = sym_binary_type,
  [sym_decorator] = sym_decorator,
  [sym_directive] = sym_directive,
  [sym_out_directive] = sym_out_directive,
  [sym_metadata_directive] = sym_metadata_directive,
  [sym_format_directive] = sym_format_directive,
  [sym_import_directive] = sym_import_directive,
  [sym_variable_directive] = sym_variable_directive,
  [sym_function_directive] = sym_function_directive,
  [sym_type_definition] = sym_type_definition,
  [sym_type_definition_body] = sym_type_definition_body,
  [sym_binary_template] = sym_binary_template,
  [sym_binary_template_field] = sym_binary_template_field,
  [sym_type_constructor] = sym_type_constructor,
  [sym_type_constructor_body] = sym_type_constructor_body,
  [sym_field_definition] = sym_field_definition,
  [sym_field_repeatable] = sym_field_repeatable,
  [sym_field_explicit_type] = sym_field_explicit_type,
  [sym_field_definition_values] = sym_field_definition_values,
  [sym_field_definition_value_symbol_list] = sym_field_definition_value_symbol_list,
  [sym_resource_declaration] = sym_resource_declaration,
  [sym_type_identifier] = sym_type_identifier,
  [sym_resource_declaration_body] = sym_resource_declaration_body,
  [sym_new_resource] = sym_new_resource,
  [sym_override_resource] = sym_override_resource,
  [sym_duplicate_resource] = sym_duplicate_resource,
  [sym_resource_declaration_values] = sym_resource_declaration_values,
  [sym_resource_declaration_field_value] = sym_resource_declaration_field_value,
  [sym_resource_declaration_field_value_import] = sym_resource_declaration_field_value_import,
  [sym_component] = sym_component,
  [sym_component_configuration] = sym_component_configuration,
  [sym_component_body] = sym_component_body,
  [sym_component_types] = sym_component_types,
  [sym_component_files] = sym_component_files,
  [sym_component_file_reference] = sym_component_file_reference,
  [sym_component_type_reference] = sym_component_type_reference,
  [sym_named_file_reference] = sym_named_file_reference,
  [sym_file_reference_attributes] = sym_file_reference_attributes,
  [sym_key_value_pair] = sym_key_value_pair,
  [sym_scene] = sym_scene,
  [sym_scene_body] = sym_scene_body,
  [sym_scene_property] = sym_scene_property,
  [sym_scene_element] = sym_scene_element,
  [sym_scene_element_body] = sym_scene_element_body,
  [sym_scene_element_property] = sym_scene_element_property,
  [sym_value] = sym_value,
  [sym_resource_reference_literal] = sym_resource_reference_literal,
  [sym_string_literal] = sym_string_literal,
  [sym_expression] = sym_expression,
  [sym_unary_expression] = sym_unary_expression,
  [sym_binary_expression] = sym_binary_expression,
  [sym_call_expression] = sym_call_expression,
  [sym_parenthesized_expression] = sym_parenthesized_expression,
  [sym_comma_expression] = sym_comma_expression,
  [sym_argument_list] = sym_argument_list,
  [aux_sym_source_file_repeat1] = aux_sym_source_file_repeat1,
  [aux_sym_decorator_repeat1] = aux_sym_decorator_repeat1,
  [aux_sym_out_directive_repeat1] = aux_sym_out_directive_repeat1,
  [aux_sym_type_definition_repeat1] = aux_sym_type_definition_repeat1,
  [aux_sym_type_definition_body_repeat1] = aux_sym_type_definition_body_repeat1,
  [aux_sym_binary_template_repeat1] = aux_sym_binary_template_repeat1,
  [aux_sym_type_constructor_repeat1] = aux_sym_type_constructor_repeat1,
  [aux_sym_type_constructor_body_repeat1] = aux_sym_type_constructor_body_repeat1,
  [aux_sym_field_definition_values_repeat1] = aux_sym_field_definition_values_repeat1,
  [aux_sym_field_definition_value_symbol_list_repeat1] = aux_sym_field_definition_value_symbol_list_repeat1,
  [aux_sym_resource_declaration_body_repeat1] = aux_sym_resource_declaration_body_repeat1,
  [aux_sym_resource_declaration_values_repeat1] = aux_sym_resource_declaration_values_repeat1,
  [aux_sym_resource_declaration_field_value_repeat1] = aux_sym_resource_declaration_field_value_repeat1,
  [aux_sym_component_body_repeat1] = aux_sym_component_body_repeat1,
  [aux_sym_component_types_repeat1] = aux_sym_component_types_repeat1,
  [aux_sym_component_files_repeat1] = aux_sym_component_files_repeat1,
  [aux_sym_file_reference_attributes_repeat1] = aux_sym_file_reference_attributes_repeat1,
  [aux_sym_scene_body_repeat1] = aux_sym_scene_body_repeat1,
  [aux_sym_scene_element_body_repeat1] = aux_sym_scene_element_body_repeat1,
  [aux_sym_string_literal_repeat1] = aux_sym_string_literal_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [anon_sym_CSTR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PSTR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_CHAR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DBYT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DWRD] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DLNG] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DQAD] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_HBYT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_HWRD] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_HLNG] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_HQAD] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_BOOL] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_BBIT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RECT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_HEXD] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RSRC] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LSTB] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LSTC] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LSTE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_OCNT] = {
    .visible = true,
    .named = false,
  },
  [aux_sym_binary_type_token1] = {
    .visible = false,
    .named = false,
  },
  [anon_sym_Nested] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Reference] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_GT] = {
    .visible = true,
    .named = false,
  },
  [sym_comment] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_decorator_token1] = {
    .visible = false,
    .named = false,
  },
  [anon_sym_LPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COMMA] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_SEMI] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATout] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATproject] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATmodule] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATauthor] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATcopyright] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATversion] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATlicense] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATemail] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATformat] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_requires] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_set] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_rsrc] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_classic] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_extended] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_kestrel] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_rez] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATimport] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATvar] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATconst] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_EQ] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ATfunction] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_type] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COLON] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LBRACE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RBRACE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_template] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_constructor] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DOT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RBRACE_SEMI] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_field] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_repeatable] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_as] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_AMP] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LBRACK] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RBRACK] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_declare] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_new] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_override] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_duplicate] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_import] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_component] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_api_export] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_files] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DASH_GT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_scene] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Title] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Flags] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Size] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Button] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Label] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_TextArea] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Image] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_TextField] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Checkbox] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_List] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_ScrollArea] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Grid] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LabeledList] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Canvas] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Sprite] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PopupButton] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Slider] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Table] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Box] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Radio] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_TabBar] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Separator] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Spacer] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Position] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Frame] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Value] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Action] = {
    .visible = true,
    .named = false,
  },
  [sym_variable] = {
    .visible = true,
    .named = true,
  },
  [sym_identifier_literal] = {
    .visible = true,
    .named = true,
  },
  [sym_integer_literal] = {
    .visible = true,
    .named = true,
  },
  [sym_hex_literal] = {
    .visible = true,
    .named = true,
  },
  [sym_percentage_literal] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_POUNDauto] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_POUND] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DQUOTE] = {
    .visible = true,
    .named = false,
  },
  [aux_sym_string_literal_token1] = {
    .visible = false,
    .named = false,
  },
  [anon_sym_TILDE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PLUS] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_DASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_STAR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_SLASH] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_CARET] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PIPE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LT_LT] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_GT_GT] = {
    .visible = true,
    .named = false,
  },
  [sym_source_file] = {
    .visible = true,
    .named = true,
  },
  [sym__top_level] = {
    .visible = false,
    .named = true,
  },
  [sym_binary_type] = {
    .visible = true,
    .named = true,
  },
  [sym_decorator] = {
    .visible = true,
    .named = true,
  },
  [sym_directive] = {
    .visible = true,
    .named = true,
  },
  [sym_out_directive] = {
    .visible = true,
    .named = true,
  },
  [sym_metadata_directive] = {
    .visible = true,
    .named = true,
  },
  [sym_format_directive] = {
    .visible = true,
    .named = true,
  },
  [sym_import_directive] = {
    .visible = true,
    .named = true,
  },
  [sym_variable_directive] = {
    .visible = true,
    .named = true,
  },
  [sym_function_directive] = {
    .visible = true,
    .named = true,
  },
  [sym_type_definition] = {
    .visible = true,
    .named = true,
  },
  [sym_type_definition_body] = {
    .visible = true,
    .named = true,
  },
  [sym_binary_template] = {
    .visible = true,
    .named = true,
  },
  [sym_binary_template_field] = {
    .visible = true,
    .named = true,
  },
  [sym_type_constructor] = {
    .visible = true,
    .named = true,
  },
  [sym_type_constructor_body] = {
    .visible = true,
    .named = true,
  },
  [sym_field_definition] = {
    .visible = true,
    .named = true,
  },
  [sym_field_repeatable] = {
    .visible = true,
    .named = true,
  },
  [sym_field_explicit_type] = {
    .visible = true,
    .named = true,
  },
  [sym_field_definition_values] = {
    .visible = true,
    .named = true,
  },
  [sym_field_definition_value_symbol_list] = {
    .visible = true,
    .named = true,
  },
  [sym_resource_declaration] = {
    .visible = true,
    .named = true,
  },
  [sym_type_identifier] = {
    .visible = true,
    .named = true,
  },
  [sym_resource_declaration_body] = {
    .visible = true,
    .named = true,
  },
  [sym_new_resource] = {
    .visible = true,
    .named = true,
  },
  [sym_override_resource] = {
    .visible = true,
    .named = true,
  },
  [sym_duplicate_resource] = {
    .visible = true,
    .named = true,
  },
  [sym_resource_declaration_values] = {
    .visible = true,
    .named = true,
  },
  [sym_resource_declaration_field_value] = {
    .visible = true,
    .named = true,
  },
  [sym_resource_declaration_field_value_import] = {
    .visible = true,
    .named = true,
  },
  [sym_component] = {
    .visible = true,
    .named = true,
  },
  [sym_component_configuration] = {
    .visible = true,
    .named = true,
  },
  [sym_component_body] = {
    .visible = true,
    .named = true,
  },
  [sym_component_types] = {
    .visible = true,
    .named = true,
  },
  [sym_component_files] = {
    .visible = true,
    .named = true,
  },
  [sym_component_file_reference] = {
    .visible = true,
    .named = true,
  },
  [sym_component_type_reference] = {
    .visible = true,
    .named = true,
  },
  [sym_named_file_reference] = {
    .visible = true,
    .named = true,
  },
  [sym_file_reference_attributes] = {
    .visible = true,
    .named = true,
  },
  [sym_key_value_pair] = {
    .visible = true,
    .named = true,
  },
  [sym_scene] = {
    .visible = true,
    .named = true,
  },
  [sym_scene_body] = {
    .visible = true,
    .named = true,
  },
  [sym_scene_property] = {
    .visible = true,
    .named = true,
  },
  [sym_scene_element] = {
    .visible = true,
    .named = true,
  },
  [sym_scene_element_body] = {
    .visible = true,
    .named = true,
  },
  [sym_scene_element_property] = {
    .visible = true,
    .named = true,
  },
  [sym_value] = {
    .visible = true,
    .named = true,
  },
  [sym_resource_reference_literal] = {
    .visible = true,
    .named = true,
  },
  [sym_string_literal] = {
    .visible = true,
    .named = true,
  },
  [sym_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_unary_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_binary_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_call_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_parenthesized_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_comma_expression] = {
    .visible = true,
    .named = true,
  },
  [sym_argument_list] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_source_file_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_decorator_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_out_directive_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_type_definition_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_type_definition_body_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_binary_template_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_type_constructor_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_type_constructor_body_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_field_definition_values_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_field_definition_value_symbol_list_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_resource_declaration_body_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_resource_declaration_values_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_resource_declaration_field_value_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_component_body_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_component_types_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_component_files_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_file_reference_attributes_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_scene_body_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_scene_element_body_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_string_literal_repeat1] = {
    .visible = false,
    .named = false,
  },
};

enum {
  field_argument = 1,
  field_arguments = 2,
  field_code = 3,
  field_copy = 4,
  field_decorators = 5,
  field_default_value = 6,
  field_definition = 7,
  field_explicit_type = 8,
  field_field = 9,
  field_file = 10,
  field_function = 11,
  field_id = 12,
  field_is_reference = 13,
  field_key = 14,
  field_label = 15,
  field_left = 16,
  field_lhs = 17,
  field_module = 18,
  field_name = 19,
  field_namespace = 20,
  field_ns = 21,
  field_operator = 22,
  field_path = 23,
  field_rhs = 24,
  field_right = 25,
  field_source = 26,
  field_source_file = 27,
  field_type = 28,
  field_value = 29,
  field_values = 30,
};

static const char * const ts_field_names[] = {
  [0] = NULL,
  [field_argument] = "argument",
  [field_arguments] = "arguments",
  [field_code] = "code",
  [field_copy] = "copy",
  [field_decorators] = "decorators",
  [field_default_value] = "default_value",
  [field_definition] = "definition",
  [field_explicit_type] = "explicit_type",
  [field_field] = "field",
  [field_file] = "file",
  [field_function] = "function",
  [field_id] = "id",
  [field_is_reference] = "is_reference",
  [field_key] = "key",
  [field_label] = "label",
  [field_left] = "left",
  [field_lhs] = "lhs",
  [field_module] = "module",
  [field_name] = "name",
  [field_namespace] = "namespace",
  [field_ns] = "ns",
  [field_operator] = "operator",
  [field_path] = "path",
  [field_rhs] = "rhs",
  [field_right] = "right",
  [field_source] = "source",
  [field_source_file] = "source_file",
  [field_type] = "type",
  [field_value] = "value",
  [field_values] = "values",
};

static const TSFieldMapSlice ts_field_map_slices[PRODUCTION_ID_COUNT] = {
  [1] = {.index = 0, .length = 1},
  [2] = {.index = 1, .length = 1},
  [3] = {.index = 2, .length = 1},
  [4] = {.index = 3, .length = 1},
  [5] = {.index = 4, .length = 1},
  [6] = {.index = 5, .length = 1},
  [7] = {.index = 6, .length = 2},
  [8] = {.index = 8, .length = 1},
  [9] = {.index = 9, .length = 2},
  [10] = {.index = 11, .length = 2},
  [11] = {.index = 13, .length = 1},
  [12] = {.index = 14, .length = 1},
  [13] = {.index = 15, .length = 2},
  [14] = {.index = 17, .length = 3},
  [15] = {.index = 20, .length = 3},
  [16] = {.index = 23, .length = 1},
  [17] = {.index = 24, .length = 2},
  [18] = {.index = 26, .length = 2},
  [19] = {.index = 28, .length = 4},
  [20] = {.index = 32, .length = 3},
  [21] = {.index = 35, .length = 3},
  [22] = {.index = 38, .length = 2},
  [23] = {.index = 40, .length = 1},
  [24] = {.index = 41, .length = 2},
  [25] = {.index = 43, .length = 2},
  [26] = {.index = 45, .length = 1},
  [27] = {.index = 46, .length = 1},
  [28] = {.index = 47, .length = 1},
  [29] = {.index = 48, .length = 3},
  [30] = {.index = 51, .length = 2},
  [31] = {.index = 53, .length = 1},
  [32] = {.index = 54, .length = 2},
  [33] = {.index = 56, .length = 2},
  [34] = {.index = 58, .length = 2},
  [35] = {.index = 60, .length = 2},
  [36] = {.index = 62, .length = 4},
  [37] = {.index = 66, .length = 2},
  [38] = {.index = 68, .length = 2},
  [39] = {.index = 70, .length = 3},
  [40] = {.index = 73, .length = 1},
  [41] = {.index = 74, .length = 2},
  [42] = {.index = 76, .length = 2},
  [43] = {.index = 78, .length = 1},
  [44] = {.index = 79, .length = 1},
  [45] = {.index = 80, .length = 1},
  [46] = {.index = 81, .length = 2},
  [47] = {.index = 83, .length = 2},
  [48] = {.index = 85, .length = 2},
  [49] = {.index = 87, .length = 2},
  [50] = {.index = 89, .length = 4},
  [51] = {.index = 93, .length = 2},
  [52] = {.index = 95, .length = 3},
  [53] = {.index = 98, .length = 2},
  [54] = {.index = 100, .length = 2},
  [55] = {.index = 102, .length = 2},
  [56] = {.index = 104, .length = 2},
  [57] = {.index = 106, .length = 2},
  [58] = {.index = 108, .length = 2},
};

static const TSFieldMapEntry ts_field_map_entries[] = {
  [0] =
    {field_name, 0},
  [1] =
    {field_module, 1},
  [2] =
    {field_source_file, 1},
  [3] =
    {field_type, 0},
  [4] =
    {field_id, 1},
  [5] =
    {field_name, 1},
  [6] =
    {field_namespace, 0},
    {field_type, 2},
  [8] =
    {field_type, 1},
  [9] =
    {field_arguments, 1},
    {field_function, 0},
  [11] =
    {field_argument, 1},
    {field_operator, 0},
  [13] =
    {field_name, 2},
  [14] =
    {field_type, 2},
  [15] =
    {field_id, 3},
    {field_type, 1},
  [17] =
    {field_lhs, 0},
    {field_operator, 1},
    {field_rhs, 2},
  [20] =
    {field_code, 3},
    {field_definition, 4},
    {field_name, 1},
  [23] =
    {field_name, 3},
  [24] =
    {field_left, 0},
    {field_right, 2},
  [26] =
    {field_id, 2},
    {field_name, 4},
  [28] =
    {field_code, 4},
    {field_decorators, 0},
    {field_definition, 5},
    {field_name, 2},
  [32] =
    {field_id, 5},
    {field_namespace, 1},
    {field_type, 3},
  [35] =
    {field_id, 5},
    {field_ns, 1},
    {field_type, 3},
  [38] =
    {field_id, 3},
    {field_name, 5},
  [40] =
    {field_field, 0},
  [41] =
    {field_name, 0},
    {field_value, 2},
  [43] =
    {field_label, 1},
    {field_type, 0},
  [45] =
    {field_field, 1},
  [46] =
    {field_path, 2},
  [47] =
    {field_file, 0},
  [48] =
    {field_name, 0},
    {field_value, 2},
    {field_value, 3},
  [51] =
    {field_name, 0},
    {field_type, 2},
  [53] =
    {field_lhs, 1, .inherited = true},
  [54] =
    {field_lhs, 0, .inherited = true},
    {field_lhs, 1, .inherited = true},
  [56] =
    {field_name, 2},
    {field_values, 4},
  [58] =
    {field_copy, 4},
    {field_source, 2},
  [60] =
    {field_default_value, 1, .inherited = true},
    {field_name, 1, .inherited = true},
  [62] =
    {field_default_value, 0, .inherited = true},
    {field_default_value, 1, .inherited = true},
    {field_name, 0, .inherited = true},
    {field_name, 1, .inherited = true},
  [66] =
    {field_name, 2},
    {field_values, 5},
  [68] =
    {field_name, 3},
    {field_values, 5},
  [70] =
    {field_copy, 4},
    {field_name, 5},
    {field_source, 2},
  [73] =
    {field_key, 0},
  [74] =
    {field_file, 0},
    {field_name, 2},
  [76] =
    {field_name, 2},
    {field_type, 0},
  [78] =
    {field_lhs, 0},
  [79] =
    {field_is_reference, 1},
  [80] =
    {field_explicit_type, 1},
  [81] =
    {field_name, 3},
    {field_values, 6},
  [83] =
    {field_default_value, 2},
    {field_name, 0},
  [85] =
    {field_explicit_type, 1},
    {field_is_reference, 2},
  [87] =
    {field_name, 1, .inherited = true},
    {field_value, 1, .inherited = true},
  [89] =
    {field_name, 0, .inherited = true},
    {field_name, 1, .inherited = true},
    {field_value, 0, .inherited = true},
    {field_value, 1, .inherited = true},
  [93] =
    {field_key, 0},
    {field_value, 2},
  [95] =
    {field_lhs, 0},
    {field_lhs, 1},
    {field_lhs, 2},
  [98] =
    {field_default_value, 3},
    {field_name, 0},
  [100] =
    {field_default_value, 3},
    {field_name, 1},
  [102] =
    {field_name, 1},
    {field_value, 3},
  [104] =
    {field_default_value, 4},
    {field_name, 1},
  [106] =
    {field_explicit_type, 1},
    {field_is_reference, 5},
  [108] =
    {field_explicit_type, 1},
    {field_is_reference, 6},
};

static const TSSymbol ts_alias_sequences[PRODUCTION_ID_COUNT][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [0] = {0},
};

static const uint16_t ts_non_terminal_alias_map[] = {
  0,
};

static const TSStateId ts_primary_state_ids[STATE_COUNT] = {
  [0] = 0,
  [1] = 1,
  [2] = 2,
  [3] = 3,
  [4] = 4,
  [5] = 5,
  [6] = 6,
  [7] = 7,
  [8] = 8,
  [9] = 9,
  [10] = 10,
  [11] = 11,
  [12] = 12,
  [13] = 13,
  [14] = 14,
  [15] = 15,
  [16] = 16,
  [17] = 17,
  [18] = 18,
  [19] = 19,
  [20] = 20,
  [21] = 21,
  [22] = 22,
  [23] = 23,
  [24] = 24,
  [25] = 25,
  [26] = 26,
  [27] = 27,
  [28] = 28,
  [29] = 29,
  [30] = 30,
  [31] = 31,
  [32] = 32,
  [33] = 33,
  [34] = 34,
  [35] = 35,
  [36] = 36,
  [37] = 37,
  [38] = 38,
  [39] = 39,
  [40] = 40,
  [41] = 41,
  [42] = 42,
  [43] = 43,
  [44] = 44,
  [45] = 45,
  [46] = 46,
  [47] = 47,
  [48] = 48,
  [49] = 49,
  [50] = 50,
  [51] = 51,
  [52] = 52,
  [53] = 53,
  [54] = 54,
  [55] = 55,
  [56] = 56,
  [57] = 57,
  [58] = 58,
  [59] = 59,
  [60] = 60,
  [61] = 61,
  [62] = 62,
  [63] = 63,
  [64] = 64,
  [65] = 65,
  [66] = 66,
  [67] = 67,
  [68] = 68,
  [69] = 69,
  [70] = 70,
  [71] = 71,
  [72] = 72,
  [73] = 73,
  [74] = 74,
  [75] = 75,
  [76] = 76,
  [77] = 77,
  [78] = 78,
  [79] = 79,
  [80] = 80,
  [81] = 81,
  [82] = 82,
  [83] = 83,
  [84] = 84,
  [85] = 85,
  [86] = 86,
  [87] = 77,
  [88] = 88,
  [89] = 89,
  [90] = 90,
  [91] = 91,
  [92] = 92,
  [93] = 93,
  [94] = 94,
  [95] = 95,
  [96] = 96,
  [97] = 97,
  [98] = 98,
  [99] = 99,
  [100] = 100,
  [101] = 101,
  [102] = 102,
  [103] = 103,
  [104] = 104,
  [105] = 105,
  [106] = 106,
  [107] = 107,
  [108] = 108,
  [109] = 109,
  [110] = 110,
  [111] = 111,
  [112] = 112,
  [113] = 113,
  [114] = 114,
  [115] = 115,
  [116] = 116,
  [117] = 117,
  [118] = 118,
  [119] = 119,
  [120] = 120,
  [121] = 121,
  [122] = 122,
  [123] = 123,
  [124] = 124,
  [125] = 125,
  [126] = 126,
  [127] = 127,
  [128] = 128,
  [129] = 129,
  [130] = 130,
  [131] = 131,
  [132] = 132,
  [133] = 133,
  [134] = 134,
  [135] = 135,
  [136] = 136,
  [137] = 137,
  [138] = 138,
  [139] = 139,
  [140] = 140,
  [141] = 141,
  [142] = 142,
  [143] = 143,
  [144] = 144,
  [145] = 145,
  [146] = 146,
  [147] = 147,
  [148] = 148,
  [149] = 149,
  [150] = 150,
  [151] = 151,
  [152] = 152,
  [153] = 153,
  [154] = 154,
  [155] = 155,
  [156] = 156,
  [157] = 157,
  [158] = 158,
  [159] = 159,
  [160] = 160,
  [161] = 161,
  [162] = 162,
  [163] = 163,
  [164] = 164,
  [165] = 165,
  [166] = 166,
  [167] = 167,
  [168] = 90,
  [169] = 169,
  [170] = 170,
  [171] = 33,
  [172] = 172,
  [173] = 30,
  [174] = 29,
  [175] = 175,
  [176] = 32,
  [177] = 177,
  [178] = 178,
  [179] = 179,
  [180] = 180,
  [181] = 181,
  [182] = 182,
  [183] = 183,
  [184] = 184,
  [185] = 185,
  [186] = 186,
  [187] = 187,
  [188] = 188,
  [189] = 189,
  [190] = 190,
  [191] = 191,
  [192] = 192,
  [193] = 193,
  [194] = 194,
  [195] = 195,
  [196] = 196,
  [197] = 197,
  [198] = 198,
  [199] = 199,
  [200] = 200,
  [201] = 201,
  [202] = 202,
  [203] = 203,
  [204] = 204,
  [205] = 205,
  [206] = 206,
  [207] = 207,
  [208] = 208,
  [209] = 209,
  [210] = 210,
  [211] = 211,
  [212] = 212,
  [213] = 213,
  [214] = 214,
  [215] = 215,
  [216] = 216,
  [217] = 217,
  [218] = 218,
  [219] = 219,
  [220] = 220,
  [221] = 221,
  [222] = 222,
  [223] = 223,
  [224] = 95,
  [225] = 225,
  [226] = 226,
  [227] = 227,
  [228] = 228,
  [229] = 229,
  [230] = 230,
  [231] = 231,
  [232] = 232,
  [233] = 233,
  [234] = 234,
  [235] = 189,
  [236] = 236,
  [237] = 237,
  [238] = 238,
  [239] = 217,
  [240] = 240,
  [241] = 241,
  [242] = 242,
  [243] = 243,
  [244] = 244,
  [245] = 245,
  [246] = 246,
  [247] = 247,
  [248] = 101,
  [249] = 249,
  [250] = 250,
  [251] = 103,
  [252] = 252,
  [253] = 253,
  [254] = 254,
  [255] = 255,
  [256] = 256,
  [257] = 257,
  [258] = 258,
  [259] = 259,
  [260] = 260,
  [261] = 261,
  [262] = 246,
  [263] = 263,
  [264] = 264,
  [265] = 265,
  [266] = 266,
  [267] = 267,
  [268] = 268,
  [269] = 269,
  [270] = 270,
  [271] = 271,
  [272] = 272,
  [273] = 273,
  [274] = 274,
  [275] = 245,
  [276] = 276,
  [277] = 277,
  [278] = 278,
  [279] = 279,
  [280] = 280,
  [281] = 281,
  [282] = 282,
  [283] = 283,
  [284] = 284,
  [285] = 285,
  [286] = 286,
  [287] = 287,
  [288] = 288,
  [289] = 289,
  [290] = 290,
  [291] = 291,
  [292] = 292,
  [293] = 293,
  [294] = 294,
  [295] = 295,
  [296] = 296,
  [297] = 297,
  [298] = 298,
  [299] = 299,
  [300] = 300,
  [301] = 301,
  [302] = 302,
  [303] = 303,
  [304] = 304,
  [305] = 305,
  [306] = 306,
  [307] = 307,
  [308] = 308,
  [309] = 309,
  [310] = 310,
  [311] = 311,
  [312] = 312,
  [313] = 313,
  [314] = 314,
  [315] = 315,
  [316] = 316,
  [317] = 317,
  [318] = 318,
  [319] = 319,
  [320] = 320,
  [321] = 321,
  [322] = 322,
  [323] = 323,
  [324] = 324,
  [325] = 325,
  [326] = 326,
  [327] = 327,
  [328] = 328,
  [329] = 329,
  [330] = 330,
  [331] = 331,
  [332] = 332,
  [333] = 333,
  [334] = 334,
  [335] = 335,
  [336] = 336,
  [337] = 337,
  [338] = 338,
  [339] = 339,
  [340] = 340,
  [341] = 341,
  [342] = 342,
  [343] = 343,
  [344] = 344,
  [345] = 345,
  [346] = 346,
  [347] = 347,
  [348] = 348,
  [349] = 349,
  [350] = 350,
  [351] = 351,
  [352] = 352,
  [353] = 353,
  [354] = 354,
  [355] = 355,
  [356] = 356,
  [357] = 357,
  [358] = 358,
  [359] = 359,
  [360] = 360,
  [361] = 361,
  [362] = 362,
  [363] = 363,
  [364] = 364,
  [365] = 365,
  [366] = 366,
  [367] = 367,
  [368] = 368,
  [369] = 369,
  [370] = 370,
  [371] = 371,
  [372] = 372,
  [373] = 373,
  [374] = 374,
  [375] = 375,
  [376] = 376,
  [377] = 377,
  [378] = 378,
  [379] = 379,
  [380] = 380,
  [381] = 381,
  [382] = 382,
  [383] = 383,
  [384] = 384,
  [385] = 385,
  [386] = 386,
  [387] = 387,
  [388] = 388,
  [389] = 389,
  [390] = 390,
  [391] = 391,
  [392] = 392,
  [393] = 393,
  [394] = 394,
  [395] = 395,
  [396] = 396,
  [397] = 397,
  [398] = 398,
  [399] = 399,
  [400] = 400,
  [401] = 401,
  [402] = 402,
  [403] = 403,
  [404] = 404,
  [405] = 405,
  [406] = 406,
  [407] = 407,
  [408] = 408,
  [409] = 409,
  [410] = 410,
  [411] = 411,
  [412] = 412,
  [413] = 413,
  [414] = 414,
  [415] = 415,
  [416] = 416,
  [417] = 417,
  [418] = 418,
  [419] = 419,
  [420] = 420,
  [421] = 421,
  [422] = 422,
  [423] = 395,
  [424] = 424,
  [425] = 373,
  [426] = 331,
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(302);
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '#') ADVANCE(492);
      if (lookahead == '$') ADVANCE(471);
      if (lookahead == '&') ADVANCE(429);
      if (lookahead == '(') ADVANCE(392);
      if (lookahead == ')') ADVANCE(394);
      if (lookahead == '*') ADVANCE(500);
      if (lookahead == '+') ADVANCE(497);
      if (lookahead == ',') ADVANCE(393);
      if (lookahead == '-') ADVANCE(499);
      if (lookahead == '.') ADVANCE(424);
      if (lookahead == '/') ADVANCE(501);
      if (lookahead == '0') ADVANCE(290);
      if (lookahead == ':') ADVANCE(418);
      if (lookahead == ';') ADVANCE(395);
      if (lookahead == '<') ADVANCE(327);
      if (lookahead == '=') ADVANCE(415);
      if (lookahead == '>') ADVANCE(328);
      if (lookahead == '@') ADVANCE(62);
      if (lookahead == 'A') ADVANCE(85);
      if (lookahead == 'B') ADVANCE(17);
      if (lookahead == 'C') ADVANCE(34);
      if (lookahead == 'D') ADVANCE(18);
      if (lookahead == 'F') ADVANCE(172);
      if (lookahead == 'G') ADVANCE(230);
      if (lookahead == 'H') ADVANCE(22);
      if (lookahead == 'I') ADVANCE(185);
      if (lookahead == 'L') ADVANCE(48);
      if (lookahead == 'N') ADVANCE(147);
      if (lookahead == 'O') ADVANCE(24);
      if (lookahead == 'P') ADVANCE(49);
      if (lookahead == 'R') ADVANCE(31);
      if (lookahead == 'S') ADVANCE(89);
      if (lookahead == 'T') ADVANCE(67);
      if (lookahead == 'V') ADVANCE(63);
      if (lookahead == '[') ADVANCE(430);
      if (lookahead == ']') ADVANCE(431);
      if (lookahead == '^') ADVANCE(502);
      if (lookahead == '`') ADVANCE(329);
      if (lookahead == 'a') ADVANCE(216);
      if (lookahead == 'c') ADVANCE(176);
      if (lookahead == 'd') ADVANCE(125);
      if (lookahead == 'e') ADVANCE(293);
      if (lookahead == 'f') ADVANCE(153);
      if (lookahead == 'i') ADVANCE(184);
      if (lookahead == 'k') ADVANCE(149);
      if (lookahead == 'n') ADVANCE(107);
      if (lookahead == 'o') ADVANCE(287);
      if (lookahead == 'r') ADVANCE(108);
      if (lookahead == 's') ADVANCE(92);
      if (lookahead == 't') ADVANCE(148);
      if (lookahead == '{') ADVANCE(419);
      if (lookahead == '|') ADVANCE(503);
      if (lookahead == '}') ADVANCE(420);
      if (lookahead == '~') ADVANCE(496);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(0)
      END_STATE();
    case 1:
      if (lookahead == '\n') SKIP(8)
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(494);
      if (lookahead != 0 &&
          lookahead != '\\') ADVANCE(495);
      END_STATE();
    case 2:
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '#') ADVANCE(492);
      if (lookahead == '$') ADVANCE(471);
      if (lookahead == '&') ADVANCE(429);
      if (lookahead == '(') ADVANCE(392);
      if (lookahead == '-') ADVANCE(11);
      if (lookahead == ';') ADVANCE(395);
      if (lookahead == '<') ADVANCE(326);
      if (lookahead == '=') ADVANCE(415);
      if (lookahead == '[') ADVANCE(430);
      if (lookahead == 'i') ADVANCE(477);
      if (lookahead == 'n') ADVANCE(473);
      if (lookahead == '~') ADVANCE(496);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(2)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(488);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 3:
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '#') ADVANCE(492);
      if (lookahead == '$') ADVANCE(471);
      if (lookahead == '(') ADVANCE(392);
      if (lookahead == '-') ADVANCE(297);
      if (lookahead == 't') ADVANCE(475);
      if (lookahead == '}') ADVANCE(9);
      if (lookahead == '~') ADVANCE(496);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(3)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(488);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 4:
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '#') ADVANCE(492);
      if (lookahead == '$') ADVANCE(471);
      if (lookahead == '(') ADVANCE(392);
      if (lookahead == '-') ADVANCE(297);
      if (lookahead == '~') ADVANCE(496);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(4)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(488);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 5:
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '#') ADVANCE(492);
      if (lookahead == '$') ADVANCE(471);
      if (lookahead == '-') ADVANCE(297);
      if (lookahead == '0') ADVANCE(487);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(5)
      if (('1' <= lookahead && lookahead <= '9')) ADVANCE(488);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 6:
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '&') ADVANCE(429);
      if (lookahead == '(') ADVANCE(392);
      if (lookahead == ')') ADVANCE(394);
      if (lookahead == '*') ADVANCE(500);
      if (lookahead == '+') ADVANCE(497);
      if (lookahead == ',') ADVANCE(393);
      if (lookahead == '-') ADVANCE(499);
      if (lookahead == '/') ADVANCE(501);
      if (lookahead == ';') ADVANCE(395);
      if (lookahead == '<') ADVANCE(10);
      if (lookahead == '=') ADVANCE(415);
      if (lookahead == '>') ADVANCE(328);
      if (lookahead == '@') ADVANCE(301);
      if (lookahead == '[') ADVANCE(430);
      if (lookahead == ']') ADVANCE(431);
      if (lookahead == '^') ADVANCE(502);
      if (lookahead == '{') ADVANCE(419);
      if (lookahead == '|') ADVANCE(503);
      if (lookahead == '}') ADVANCE(420);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(6)
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 7:
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '&') ADVANCE(429);
      if (lookahead == '(') ADVANCE(392);
      if (lookahead == ')') ADVANCE(394);
      if (lookahead == '*') ADVANCE(500);
      if (lookahead == '+') ADVANCE(497);
      if (lookahead == ',') ADVANCE(393);
      if (lookahead == '-') ADVANCE(498);
      if (lookahead == '/') ADVANCE(501);
      if (lookahead == ';') ADVANCE(395);
      if (lookahead == '<') ADVANCE(10);
      if (lookahead == '=') ADVANCE(415);
      if (lookahead == '>') ADVANCE(328);
      if (lookahead == '@') ADVANCE(301);
      if (lookahead == '[') ADVANCE(430);
      if (lookahead == '^') ADVANCE(502);
      if (lookahead == 'a') ADVANCE(244);
      if (lookahead == 'c') ADVANCE(199);
      if (lookahead == 'd') ADVANCE(125);
      if (lookahead == 'f') ADVANCE(163);
      if (lookahead == 'n') ADVANCE(107);
      if (lookahead == 'o') ADVANCE(287);
      if (lookahead == 's') ADVANCE(91);
      if (lookahead == 't') ADVANCE(148);
      if (lookahead == '{') ADVANCE(419);
      if (lookahead == '|') ADVANCE(503);
      if (lookahead == '}') ADVANCE(420);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(7)
      END_STATE();
    case 8:
      if (lookahead == '"') ADVANCE(493);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(8)
      END_STATE();
    case 9:
      if (lookahead == ';') ADVANCE(425);
      END_STATE();
    case 10:
      if (lookahead == '<') ADVANCE(504);
      END_STATE();
    case 11:
      if (lookahead == '>') ADVANCE(442);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(488);
      END_STATE();
    case 12:
      if (lookahead == 'A') ADVANCE(26);
      END_STATE();
    case 13:
      if (lookahead == 'A') ADVANCE(42);
      END_STATE();
    case 14:
      if (lookahead == 'A') ADVANCE(29);
      END_STATE();
    case 15:
      if (lookahead == 'A') ADVANCE(238);
      if (lookahead == 'F') ADVANCE(166);
      END_STATE();
    case 16:
      if (lookahead == 'A') ADVANCE(242);
      END_STATE();
    case 17:
      if (lookahead == 'B') ADVANCE(35);
      if (lookahead == 'O') ADVANCE(41);
      if (lookahead == 'o') ADVANCE(291);
      if (lookahead == 'u') ADVANCE(267);
      END_STATE();
    case 18:
      if (lookahead == 'B') ADVANCE(59);
      if (lookahead == 'L') ADVANCE(38);
      if (lookahead == 'Q') ADVANCE(12);
      if (lookahead == 'W') ADVANCE(45);
      END_STATE();
    case 19:
      if (lookahead == 'B') ADVANCE(319);
      if (lookahead == 'C') ADVANCE(320);
      if (lookahead == 'E') ADVANCE(321);
      END_STATE();
    case 20:
      if (lookahead == 'B') ADVANCE(75);
      if (lookahead == 'l') ADVANCE(114);
      END_STATE();
    case 21:
      if (lookahead == 'B') ADVANCE(286);
      END_STATE();
    case 22:
      if (lookahead == 'B') ADVANCE(60);
      if (lookahead == 'E') ADVANCE(58);
      if (lookahead == 'L') ADVANCE(39);
      if (lookahead == 'Q') ADVANCE(14);
      if (lookahead == 'W') ADVANCE(47);
      END_STATE();
    case 23:
      if (lookahead == 'C') ADVANCE(318);
      END_STATE();
    case 24:
      if (lookahead == 'C') ADVANCE(40);
      END_STATE();
    case 25:
      if (lookahead == 'C') ADVANCE(55);
      END_STATE();
    case 26:
      if (lookahead == 'D') ADVANCE(309);
      END_STATE();
    case 27:
      if (lookahead == 'D') ADVANCE(307);
      END_STATE();
    case 28:
      if (lookahead == 'D') ADVANCE(317);
      END_STATE();
    case 29:
      if (lookahead == 'D') ADVANCE(313);
      END_STATE();
    case 30:
      if (lookahead == 'D') ADVANCE(311);
      END_STATE();
    case 31:
      if (lookahead == 'E') ADVANCE(25);
      if (lookahead == 'S') ADVANCE(46);
      if (lookahead == 'a') ADVANCE(106);
      if (lookahead == 'e') ADVANCE(150);
      END_STATE();
    case 32:
      if (lookahead == 'G') ADVANCE(308);
      END_STATE();
    case 33:
      if (lookahead == 'G') ADVANCE(312);
      END_STATE();
    case 34:
      if (lookahead == 'H') ADVANCE(13);
      if (lookahead == 'S') ADVANCE(56);
      if (lookahead == 'a') ADVANCE(193);
      if (lookahead == 'h') ADVANCE(127);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'F') ||
          ('b' <= lookahead && lookahead <= 'f')) ADVANCE(300);
      END_STATE();
    case 35:
      if (lookahead == 'I') ADVANCE(51);
      END_STATE();
    case 36:
      if (lookahead == 'L') ADVANCE(314);
      END_STATE();
    case 37:
      if (lookahead == 'L') ADVANCE(167);
      END_STATE();
    case 38:
      if (lookahead == 'N') ADVANCE(32);
      END_STATE();
    case 39:
      if (lookahead == 'N') ADVANCE(33);
      END_STATE();
    case 40:
      if (lookahead == 'N') ADVANCE(54);
      END_STATE();
    case 41:
      if (lookahead == 'O') ADVANCE(36);
      END_STATE();
    case 42:
      if (lookahead == 'R') ADVANCE(305);
      END_STATE();
    case 43:
      if (lookahead == 'R') ADVANCE(303);
      END_STATE();
    case 44:
      if (lookahead == 'R') ADVANCE(304);
      END_STATE();
    case 45:
      if (lookahead == 'R') ADVANCE(27);
      END_STATE();
    case 46:
      if (lookahead == 'R') ADVANCE(23);
      END_STATE();
    case 47:
      if (lookahead == 'R') ADVANCE(30);
      END_STATE();
    case 48:
      if (lookahead == 'S') ADVANCE(50);
      if (lookahead == 'a') ADVANCE(82);
      if (lookahead == 'i') ADVANCE(250);
      END_STATE();
    case 49:
      if (lookahead == 'S') ADVANCE(57);
      if (lookahead == 'o') ADVANCE(214);
      END_STATE();
    case 50:
      if (lookahead == 'T') ADVANCE(19);
      END_STATE();
    case 51:
      if (lookahead == 'T') ADVANCE(315);
      END_STATE();
    case 52:
      if (lookahead == 'T') ADVANCE(306);
      END_STATE();
    case 53:
      if (lookahead == 'T') ADVANCE(310);
      END_STATE();
    case 54:
      if (lookahead == 'T') ADVANCE(322);
      END_STATE();
    case 55:
      if (lookahead == 'T') ADVANCE(316);
      END_STATE();
    case 56:
      if (lookahead == 'T') ADVANCE(43);
      END_STATE();
    case 57:
      if (lookahead == 'T') ADVANCE(44);
      END_STATE();
    case 58:
      if (lookahead == 'X') ADVANCE(28);
      END_STATE();
    case 59:
      if (lookahead == 'Y') ADVANCE(52);
      END_STATE();
    case 60:
      if (lookahead == 'Y') ADVANCE(53);
      END_STATE();
    case 61:
      if (lookahead == '_') ADVANCE(128);
      END_STATE();
    case 62:
      if (lookahead == 'a') ADVANCE(387);
      if (lookahead == 'c') ADVANCE(360);
      if (lookahead == 'e') ADVANCE(352);
      if (lookahead == 'f') ADVANCE(363);
      if (lookahead == 'i') ADVANCE(353);
      if (lookahead == 'l') ADVANCE(344);
      if (lookahead == 'm') ADVANCE(361);
      if (lookahead == 'o') ADVANCE(388);
      if (lookahead == 'p') ADVANCE(372);
      if (lookahead == 'v') ADVANCE(331);
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('b' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 63:
      if (lookahead == 'a') ADVANCE(173);
      END_STATE();
    case 64:
      if (lookahead == 'a') ADVANCE(151);
      END_STATE();
    case 65:
      if (lookahead == 'a') ADVANCE(449);
      END_STATE();
    case 66:
      if (lookahead == 'a') ADVANCE(454);
      END_STATE();
    case 67:
      if (lookahead == 'a') ADVANCE(81);
      if (lookahead == 'e') ADVANCE(294);
      if (lookahead == 'i') ADVANCE(263);
      END_STATE();
    case 68:
      if (lookahead == 'a') ADVANCE(249);
      END_STATE();
    case 69:
      if (lookahead == 'a') ADVANCE(152);
      END_STATE();
    case 70:
      if (lookahead == 'a') ADVANCE(187);
      END_STATE();
    case 71:
      if (lookahead == 'a') ADVANCE(93);
      if (lookahead == 'r') ADVANCE(165);
      END_STATE();
    case 72:
      if (lookahead == 'a') ADVANCE(84);
      END_STATE();
    case 73:
      if (lookahead == 'a') ADVANCE(239);
      END_STATE();
    case 74:
      if (lookahead == 'a') ADVANCE(247);
      END_STATE();
    case 75:
      if (lookahead == 'a') ADVANCE(227);
      END_STATE();
    case 76:
      if (lookahead == 'a') ADVANCE(271);
      END_STATE();
    case 77:
      if (lookahead == 'a') ADVANCE(277);
      END_STATE();
    case 78:
      if (lookahead == 'a') ADVANCE(275);
      END_STATE();
    case 79:
      if (lookahead == 'a') ADVANCE(236);
      END_STATE();
    case 80:
      if (lookahead == 'a') ADVANCE(276);
      END_STATE();
    case 81:
      if (lookahead == 'b') ADVANCE(20);
      END_STATE();
    case 82:
      if (lookahead == 'b') ADVANCE(136);
      END_STATE();
    case 83:
      if (lookahead == 'b') ADVANCE(204);
      END_STATE();
    case 84:
      if (lookahead == 'b') ADVANCE(183);
      END_STATE();
    case 85:
      if (lookahead == 'c') ADVANCE(265);
      END_STATE();
    case 86:
      if (lookahead == 'c') ADVANCE(169);
      END_STATE();
    case 87:
      if (lookahead == 'c') ADVANCE(407);
      END_STATE();
    case 88:
      if (lookahead == 'c') ADVANCE(408);
      END_STATE();
    case 89:
      if (lookahead == 'c') ADVANCE(224);
      if (lookahead == 'e') ADVANCE(218);
      if (lookahead == 'i') ADVANCE(296);
      if (lookahead == 'l') ADVANCE(156);
      if (lookahead == 'p') ADVANCE(71);
      END_STATE();
    case 90:
      if (lookahead == 'c') ADVANCE(180);
      END_STATE();
    case 91:
      if (lookahead == 'c') ADVANCE(109);
      END_STATE();
    case 92:
      if (lookahead == 'c') ADVANCE(109);
      if (lookahead == 'e') ADVANCE(256);
      END_STATE();
    case 93:
      if (lookahead == 'c') ADVANCE(141);
      END_STATE();
    case 94:
      if (lookahead == 'c') ADVANCE(122);
      END_STATE();
    case 95:
      if (lookahead == 'c') ADVANCE(80);
      END_STATE();
    case 96:
      if (lookahead == 'c') ADVANCE(279);
      END_STATE();
    case 97:
      if (lookahead == 'd') ADVANCE(455);
      END_STATE();
    case 98:
      if (lookahead == 'd') ADVANCE(426);
      END_STATE();
    case 99:
      if (lookahead == 'd') ADVANCE(324);
      END_STATE();
    case 100:
      if (lookahead == 'd') ADVANCE(409);
      END_STATE();
    case 101:
      if (lookahead == 'd') ADVANCE(451);
      END_STATE();
    case 102:
      if (lookahead == 'd') ADVANCE(37);
      END_STATE();
    case 103:
      if (lookahead == 'd') ADVANCE(140);
      END_STATE();
    case 104:
      if (lookahead == 'd') ADVANCE(138);
      END_STATE();
    case 105:
      if (lookahead == 'd') ADVANCE(120);
      END_STATE();
    case 106:
      if (lookahead == 'd') ADVANCE(159);
      END_STATE();
    case 107:
      if (lookahead == 'e') ADVANCE(289);
      END_STATE();
    case 108:
      if (lookahead == 'e') ADVANCE(220);
      if (lookahead == 's') ADVANCE(231);
      END_STATE();
    case 109:
      if (lookahead == 'e') ADVANCE(195);
      END_STATE();
    case 110:
      if (lookahead == 'e') ADVANCE(446);
      END_STATE();
    case 111:
      if (lookahead == 'e') ADVANCE(417);
      END_STATE();
    case 112:
      if (lookahead == 'e') ADVANCE(468);
      END_STATE();
    case 113:
      if (lookahead == 'e') ADVANCE(450);
      END_STATE();
    case 114:
      if (lookahead == 'e') ADVANCE(461);
      END_STATE();
    case 115:
      if (lookahead == 'e') ADVANCE(444);
      END_STATE();
    case 116:
      if (lookahead == 'e') ADVANCE(469);
      END_STATE();
    case 117:
      if (lookahead == 'e') ADVANCE(443);
      END_STATE();
    case 118:
      if (lookahead == 'e') ADVANCE(458);
      END_STATE();
    case 119:
      if (lookahead == 'e') ADVANCE(432);
      END_STATE();
    case 120:
      if (lookahead == 'e') ADVANCE(435);
      END_STATE();
    case 121:
      if (lookahead == 'e') ADVANCE(421);
      END_STATE();
    case 122:
      if (lookahead == 'e') ADVANCE(325);
      END_STATE();
    case 123:
      if (lookahead == 'e') ADVANCE(436);
      END_STATE();
    case 124:
      if (lookahead == 'e') ADVANCE(427);
      END_STATE();
    case 125:
      if (lookahead == 'e') ADVANCE(90);
      if (lookahead == 'u') ADVANCE(217);
      END_STATE();
    case 126:
      if (lookahead == 'e') ADVANCE(197);
      END_STATE();
    case 127:
      if (lookahead == 'e') ADVANCE(86);
      END_STATE();
    case 128:
      if (lookahead == 'e') ADVANCE(295);
      END_STATE();
    case 129:
      if (lookahead == 'e') ADVANCE(243);
      END_STATE();
    case 130:
      if (lookahead == 'e') ADVANCE(235);
      END_STATE();
    case 131:
      if (lookahead == 'e') ADVANCE(246);
      END_STATE();
    case 132:
      if (lookahead == 'e') ADVANCE(99);
      END_STATE();
    case 133:
      if (lookahead == 'e') ADVANCE(175);
      END_STATE();
    case 134:
      if (lookahead == 'e') ADVANCE(175);
      if (lookahead == 'l') ADVANCE(131);
      END_STATE();
    case 135:
      if (lookahead == 'e') ADVANCE(198);
      END_STATE();
    case 136:
      if (lookahead == 'e') ADVANCE(170);
      END_STATE();
    case 137:
      if (lookahead == 'e') ADVANCE(248);
      END_STATE();
    case 138:
      if (lookahead == 'e') ADVANCE(100);
      END_STATE();
    case 139:
      if (lookahead == 'e') ADVANCE(194);
      END_STATE();
    case 140:
      if (lookahead == 'e') ADVANCE(225);
      END_STATE();
    case 141:
      if (lookahead == 'e') ADVANCE(226);
      END_STATE();
    case 142:
      if (lookahead == 'e') ADVANCE(171);
      END_STATE();
    case 143:
      if (lookahead == 'e') ADVANCE(76);
      END_STATE();
    case 144:
      if (lookahead == 'e') ADVANCE(178);
      END_STATE();
    case 145:
      if (lookahead == 'e') ADVANCE(65);
      END_STATE();
    case 146:
      if (lookahead == 'e') ADVANCE(66);
      END_STATE();
    case 147:
      if (lookahead == 'e') ADVANCE(255);
      END_STATE();
    case 148:
      if (lookahead == 'e') ADVANCE(188);
      if (lookahead == 'y') ADVANCE(221);
      END_STATE();
    case 149:
      if (lookahead == 'e') ADVANCE(252);
      END_STATE();
    case 150:
      if (lookahead == 'f') ADVANCE(130);
      END_STATE();
    case 151:
      if (lookahead == 'g') ADVANCE(245);
      END_STATE();
    case 152:
      if (lookahead == 'g') ADVANCE(113);
      END_STATE();
    case 153:
      if (lookahead == 'i') ADVANCE(134);
      END_STATE();
    case 154:
      if (lookahead == 'i') ADVANCE(61);
      END_STATE();
    case 155:
      if (lookahead == 'i') ADVANCE(97);
      END_STATE();
    case 156:
      if (lookahead == 'i') ADVANCE(103);
      END_STATE();
    case 157:
      if (lookahead == 'i') ADVANCE(202);
      END_STATE();
    case 158:
      if (lookahead == 'i') ADVANCE(95);
      END_STATE();
    case 159:
      if (lookahead == 'i') ADVANCE(201);
      END_STATE();
    case 160:
      if (lookahead == 'i') ADVANCE(88);
      END_STATE();
    case 161:
      if (lookahead == 'i') ADVANCE(205);
      END_STATE();
    case 162:
      if (lookahead == 'i') ADVANCE(240);
      END_STATE();
    case 163:
      if (lookahead == 'i') ADVANCE(133);
      END_STATE();
    case 164:
      if (lookahead == 'i') ADVANCE(105);
      END_STATE();
    case 165:
      if (lookahead == 'i') ADVANCE(274);
      END_STATE();
    case 166:
      if (lookahead == 'i') ADVANCE(144);
      END_STATE();
    case 167:
      if (lookahead == 'i') ADVANCE(254);
      END_STATE();
    case 168:
      if (lookahead == 'i') ADVANCE(280);
      END_STATE();
    case 169:
      if (lookahead == 'k') ADVANCE(83);
      END_STATE();
    case 170:
      if (lookahead == 'l') ADVANCE(448);
      END_STATE();
    case 171:
      if (lookahead == 'l') ADVANCE(410);
      END_STATE();
    case 172:
      if (lookahead == 'l') ADVANCE(64);
      if (lookahead == 'r') ADVANCE(70);
      END_STATE();
    case 173:
      if (lookahead == 'l') ADVANCE(284);
      END_STATE();
    case 174:
      if (lookahead == 'l') ADVANCE(16);
      END_STATE();
    case 175:
      if (lookahead == 'l') ADVANCE(98);
      END_STATE();
    case 176:
      if (lookahead == 'l') ADVANCE(68);
      if (lookahead == 'o') ADVANCE(186);
      END_STATE();
    case 177:
      if (lookahead == 'l') ADVANCE(158);
      END_STATE();
    case 178:
      if (lookahead == 'l') ADVANCE(101);
      END_STATE();
    case 179:
      if (lookahead == 'l') ADVANCE(174);
      END_STATE();
    case 180:
      if (lookahead == 'l') ADVANCE(79);
      END_STATE();
    case 181:
      if (lookahead == 'l') ADVANCE(78);
      END_STATE();
    case 182:
      if (lookahead == 'l') ADVANCE(115);
      END_STATE();
    case 183:
      if (lookahead == 'l') ADVANCE(124);
      END_STATE();
    case 184:
      if (lookahead == 'm') ADVANCE(215);
      END_STATE();
    case 185:
      if (lookahead == 'm') ADVANCE(69);
      END_STATE();
    case 186:
      if (lookahead == 'm') ADVANCE(219);
      if (lookahead == 'n') ADVANCE(253);
      END_STATE();
    case 187:
      if (lookahead == 'm') ADVANCE(112);
      END_STATE();
    case 188:
      if (lookahead == 'm') ADVANCE(222);
      END_STATE();
    case 189:
      if (lookahead == 'n') ADVANCE(470);
      END_STATE();
    case 190:
      if (lookahead == 'n') ADVANCE(447);
      END_STATE();
    case 191:
      if (lookahead == 'n') ADVANCE(467);
      END_STATE();
    case 192:
      if (lookahead == 'n') ADVANCE(459);
      END_STATE();
    case 193:
      if (lookahead == 'n') ADVANCE(288);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'F') ||
          ('a' <= lookahead && lookahead <= 'f')) ADVANCE(299);
      END_STATE();
    case 194:
      if (lookahead == 'n') ADVANCE(260);
      END_STATE();
    case 195:
      if (lookahead == 'n') ADVANCE(117);
      END_STATE();
    case 196:
      if (lookahead == 'n') ADVANCE(139);
      END_STATE();
    case 197:
      if (lookahead == 'n') ADVANCE(104);
      END_STATE();
    case 198:
      if (lookahead == 'n') ADVANCE(94);
      END_STATE();
    case 199:
      if (lookahead == 'o') ADVANCE(186);
      END_STATE();
    case 200:
      if (lookahead == 'o') ADVANCE(491);
      END_STATE();
    case 201:
      if (lookahead == 'o') ADVANCE(463);
      END_STATE();
    case 202:
      if (lookahead == 'o') ADVANCE(189);
      END_STATE();
    case 203:
      if (lookahead == 'o') ADVANCE(190);
      END_STATE();
    case 204:
      if (lookahead == 'o') ADVANCE(292);
      END_STATE();
    case 205:
      if (lookahead == 'o') ADVANCE(191);
      END_STATE();
    case 206:
      if (lookahead == 'o') ADVANCE(233);
      END_STATE();
    case 207:
      if (lookahead == 'o') ADVANCE(179);
      END_STATE();
    case 208:
      if (lookahead == 'o') ADVANCE(192);
      END_STATE();
    case 209:
      if (lookahead == 'o') ADVANCE(228);
      END_STATE();
    case 210:
      if (lookahead == 'o') ADVANCE(229);
      END_STATE();
    case 211:
      if (lookahead == 'o') ADVANCE(196);
      END_STATE();
    case 212:
      if (lookahead == 'o') ADVANCE(234);
      END_STATE();
    case 213:
      if (lookahead == 'p') ADVANCE(21);
      END_STATE();
    case 214:
      if (lookahead == 'p') ADVANCE(282);
      if (lookahead == 's') ADVANCE(168);
      END_STATE();
    case 215:
      if (lookahead == 'p') ADVANCE(206);
      END_STATE();
    case 216:
      if (lookahead == 'p') ADVANCE(154);
      if (lookahead == 's') ADVANCE(428);
      END_STATE();
    case 217:
      if (lookahead == 'p') ADVANCE(177);
      END_STATE();
    case 218:
      if (lookahead == 'p') ADVANCE(73);
      END_STATE();
    case 219:
      if (lookahead == 'p') ADVANCE(211);
      END_STATE();
    case 220:
      if (lookahead == 'p') ADVANCE(143);
      if (lookahead == 'q') ADVANCE(283);
      if (lookahead == 'z') ADVANCE(411);
      END_STATE();
    case 221:
      if (lookahead == 'p') ADVANCE(111);
      END_STATE();
    case 222:
      if (lookahead == 'p') ADVANCE(181);
      END_STATE();
    case 223:
      if (lookahead == 'p') ADVANCE(212);
      END_STATE();
    case 224:
      if (lookahead == 'r') ADVANCE(207);
      END_STATE();
    case 225:
      if (lookahead == 'r') ADVANCE(460);
      END_STATE();
    case 226:
      if (lookahead == 'r') ADVANCE(466);
      END_STATE();
    case 227:
      if (lookahead == 'r') ADVANCE(464);
      END_STATE();
    case 228:
      if (lookahead == 'r') ADVANCE(465);
      END_STATE();
    case 229:
      if (lookahead == 'r') ADVANCE(423);
      END_STATE();
    case 230:
      if (lookahead == 'r') ADVANCE(155);
      END_STATE();
    case 231:
      if (lookahead == 'r') ADVANCE(87);
      END_STATE();
    case 232:
      if (lookahead == 'r') ADVANCE(285);
      END_STATE();
    case 233:
      if (lookahead == 'r') ADVANCE(259);
      END_STATE();
    case 234:
      if (lookahead == 'r') ADVANCE(261);
      END_STATE();
    case 235:
      if (lookahead == 'r') ADVANCE(135);
      END_STATE();
    case 236:
      if (lookahead == 'r') ADVANCE(119);
      END_STATE();
    case 237:
      if (lookahead == 'r') ADVANCE(142);
      END_STATE();
    case 238:
      if (lookahead == 'r') ADVANCE(145);
      END_STATE();
    case 239:
      if (lookahead == 'r') ADVANCE(77);
      END_STATE();
    case 240:
      if (lookahead == 'r') ADVANCE(137);
      END_STATE();
    case 241:
      if (lookahead == 'r') ADVANCE(164);
      END_STATE();
    case 242:
      if (lookahead == 'r') ADVANCE(146);
      END_STATE();
    case 243:
      if (lookahead == 'r') ADVANCE(241);
      END_STATE();
    case 244:
      if (lookahead == 's') ADVANCE(428);
      END_STATE();
    case 245:
      if (lookahead == 's') ADVANCE(445);
      END_STATE();
    case 246:
      if (lookahead == 's') ADVANCE(441);
      END_STATE();
    case 247:
      if (lookahead == 's') ADVANCE(457);
      END_STATE();
    case 248:
      if (lookahead == 's') ADVANCE(405);
      END_STATE();
    case 249:
      if (lookahead == 's') ADVANCE(251);
      END_STATE();
    case 250:
      if (lookahead == 's') ADVANCE(257);
      END_STATE();
    case 251:
      if (lookahead == 's') ADVANCE(160);
      END_STATE();
    case 252:
      if (lookahead == 's') ADVANCE(278);
      END_STATE();
    case 253:
      if (lookahead == 's') ADVANCE(270);
      END_STATE();
    case 254:
      if (lookahead == 's') ADVANCE(262);
      END_STATE();
    case 255:
      if (lookahead == 's') ADVANCE(272);
      END_STATE();
    case 256:
      if (lookahead == 't') ADVANCE(406);
      END_STATE();
    case 257:
      if (lookahead == 't') ADVANCE(453);
      END_STATE();
    case 258:
      if (lookahead == 't') ADVANCE(15);
      END_STATE();
    case 259:
      if (lookahead == 't') ADVANCE(437);
      END_STATE();
    case 260:
      if (lookahead == 't') ADVANCE(439);
      END_STATE();
    case 261:
      if (lookahead == 't') ADVANCE(440);
      END_STATE();
    case 262:
      if (lookahead == 't') ADVANCE(456);
      END_STATE();
    case 263:
      if (lookahead == 't') ADVANCE(182);
      END_STATE();
    case 264:
      if (lookahead == 't') ADVANCE(200);
      END_STATE();
    case 265:
      if (lookahead == 't') ADVANCE(157);
      END_STATE();
    case 266:
      if (lookahead == 't') ADVANCE(203);
      END_STATE();
    case 267:
      if (lookahead == 't') ADVANCE(266);
      END_STATE();
    case 268:
      if (lookahead == 't') ADVANCE(126);
      END_STATE();
    case 269:
      if (lookahead == 't') ADVANCE(208);
      END_STATE();
    case 270:
      if (lookahead == 't') ADVANCE(232);
      END_STATE();
    case 271:
      if (lookahead == 't') ADVANCE(72);
      END_STATE();
    case 272:
      if (lookahead == 't') ADVANCE(132);
      END_STATE();
    case 273:
      if (lookahead == 't') ADVANCE(269);
      END_STATE();
    case 274:
      if (lookahead == 't') ADVANCE(118);
      END_STATE();
    case 275:
      if (lookahead == 't') ADVANCE(121);
      END_STATE();
    case 276:
      if (lookahead == 't') ADVANCE(123);
      END_STATE();
    case 277:
      if (lookahead == 't') ADVANCE(209);
      END_STATE();
    case 278:
      if (lookahead == 't') ADVANCE(237);
      END_STATE();
    case 279:
      if (lookahead == 't') ADVANCE(210);
      END_STATE();
    case 280:
      if (lookahead == 't') ADVANCE(161);
      END_STATE();
    case 281:
      if (lookahead == 'u') ADVANCE(264);
      END_STATE();
    case 282:
      if (lookahead == 'u') ADVANCE(213);
      END_STATE();
    case 283:
      if (lookahead == 'u') ADVANCE(162);
      END_STATE();
    case 284:
      if (lookahead == 'u') ADVANCE(116);
      END_STATE();
    case 285:
      if (lookahead == 'u') ADVANCE(96);
      END_STATE();
    case 286:
      if (lookahead == 'u') ADVANCE(273);
      END_STATE();
    case 287:
      if (lookahead == 'v') ADVANCE(129);
      END_STATE();
    case 288:
      if (lookahead == 'v') ADVANCE(74);
      END_STATE();
    case 289:
      if (lookahead == 'w') ADVANCE(433);
      END_STATE();
    case 290:
      if (lookahead == 'x') ADVANCE(298);
      END_STATE();
    case 291:
      if (lookahead == 'x') ADVANCE(462);
      END_STATE();
    case 292:
      if (lookahead == 'x') ADVANCE(452);
      END_STATE();
    case 293:
      if (lookahead == 'x') ADVANCE(268);
      END_STATE();
    case 294:
      if (lookahead == 'x') ADVANCE(258);
      END_STATE();
    case 295:
      if (lookahead == 'x') ADVANCE(223);
      END_STATE();
    case 296:
      if (lookahead == 'z') ADVANCE(110);
      END_STATE();
    case 297:
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(488);
      END_STATE();
    case 298:
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'F') ||
          ('a' <= lookahead && lookahead <= 'f')) ADVANCE(489);
      END_STATE();
    case 299:
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'F') ||
          ('a' <= lookahead && lookahead <= 'f')) ADVANCE(323);
      END_STATE();
    case 300:
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'F') ||
          ('a' <= lookahead && lookahead <= 'f')) ADVANCE(299);
      END_STATE();
    case 301:
      if (('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 302:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 303:
      ACCEPT_TOKEN(anon_sym_CSTR);
      END_STATE();
    case 304:
      ACCEPT_TOKEN(anon_sym_PSTR);
      END_STATE();
    case 305:
      ACCEPT_TOKEN(anon_sym_CHAR);
      END_STATE();
    case 306:
      ACCEPT_TOKEN(anon_sym_DBYT);
      END_STATE();
    case 307:
      ACCEPT_TOKEN(anon_sym_DWRD);
      END_STATE();
    case 308:
      ACCEPT_TOKEN(anon_sym_DLNG);
      END_STATE();
    case 309:
      ACCEPT_TOKEN(anon_sym_DQAD);
      END_STATE();
    case 310:
      ACCEPT_TOKEN(anon_sym_HBYT);
      END_STATE();
    case 311:
      ACCEPT_TOKEN(anon_sym_HWRD);
      END_STATE();
    case 312:
      ACCEPT_TOKEN(anon_sym_HLNG);
      END_STATE();
    case 313:
      ACCEPT_TOKEN(anon_sym_HQAD);
      END_STATE();
    case 314:
      ACCEPT_TOKEN(anon_sym_BOOL);
      END_STATE();
    case 315:
      ACCEPT_TOKEN(anon_sym_BBIT);
      END_STATE();
    case 316:
      ACCEPT_TOKEN(anon_sym_RECT);
      END_STATE();
    case 317:
      ACCEPT_TOKEN(anon_sym_HEXD);
      END_STATE();
    case 318:
      ACCEPT_TOKEN(anon_sym_RSRC);
      END_STATE();
    case 319:
      ACCEPT_TOKEN(anon_sym_LSTB);
      END_STATE();
    case 320:
      ACCEPT_TOKEN(anon_sym_LSTC);
      END_STATE();
    case 321:
      ACCEPT_TOKEN(anon_sym_LSTE);
      END_STATE();
    case 322:
      ACCEPT_TOKEN(anon_sym_OCNT);
      END_STATE();
    case 323:
      ACCEPT_TOKEN(aux_sym_binary_type_token1);
      END_STATE();
    case 324:
      ACCEPT_TOKEN(anon_sym_Nested);
      END_STATE();
    case 325:
      ACCEPT_TOKEN(anon_sym_Reference);
      END_STATE();
    case 326:
      ACCEPT_TOKEN(anon_sym_LT);
      END_STATE();
    case 327:
      ACCEPT_TOKEN(anon_sym_LT);
      if (lookahead == '<') ADVANCE(504);
      END_STATE();
    case 328:
      ACCEPT_TOKEN(anon_sym_GT);
      if (lookahead == '>') ADVANCE(505);
      END_STATE();
    case 329:
      ACCEPT_TOKEN(sym_comment);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(329);
      END_STATE();
    case 330:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'a') ADVANCE(345);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('b' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 331:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'a') ADVANCE(369);
      if (lookahead == 'e') ADVANCE(371);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('b' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 332:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'a') ADVANCE(382);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('b' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 333:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'c') ADVANCE(339);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 334:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'c') ADVANCE(386);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 335:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'c') ADVANCE(384);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 336:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'd') ADVANCE(389);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 337:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'e') ADVANCE(398);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 338:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'e') ADVANCE(402);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 339:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'e') ADVANCE(359);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 340:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'e') ADVANCE(335);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 341:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'g') ADVANCE(342);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 342:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'h') ADVANCE(385);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 343:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'h') ADVANCE(367);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 344:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'i') ADVANCE(333);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 345:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'i') ADVANCE(350);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 346:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'i') ADVANCE(341);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 347:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'i') ADVANCE(364);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 348:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'i') ADVANCE(365);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 349:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'j') ADVANCE(340);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 350:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'l') ADVANCE(403);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 351:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'l') ADVANCE(337);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 352:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'm') ADVANCE(330);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 353:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'm') ADVANCE(368);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 354:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'm') ADVANCE(332);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 355:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'n') ADVANCE(377);
      if (lookahead == 'p') ADVANCE(390);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 356:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'n') ADVANCE(401);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 357:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'n') ADVANCE(416);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 358:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'n') ADVANCE(334);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 359:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'n') ADVANCE(378);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 360:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'o') ADVANCE(355);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 361:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'o') ADVANCE(336);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 362:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'o') ADVANCE(349);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 363:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'o') ADVANCE(375);
      if (lookahead == 'u') ADVANCE(358);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 364:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'o') ADVANCE(356);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 365:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'o') ADVANCE(357);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 366:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'o') ADVANCE(374);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 367:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'o') ADVANCE(370);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 368:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'p') ADVANCE(366);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 369:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'r') ADVANCE(413);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 370:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'r') ADVANCE(399);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 371:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'r') ADVANCE(376);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 372:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'r') ADVANCE(362);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 373:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'r') ADVANCE(346);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 374:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'r') ADVANCE(383);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 375:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'r') ADVANCE(354);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 376:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 's') ADVANCE(347);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 377:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 's') ADVANCE(381);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 378:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 's') ADVANCE(338);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 379:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 't') ADVANCE(343);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 380:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 't') ADVANCE(396);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 381:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 't') ADVANCE(414);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 382:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 't') ADVANCE(404);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 383:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 't') ADVANCE(412);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 384:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 't') ADVANCE(397);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 385:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 't') ADVANCE(400);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 386:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 't') ADVANCE(348);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 387:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'u') ADVANCE(379);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 388:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'u') ADVANCE(380);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 389:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'u') ADVANCE(351);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 390:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (lookahead == 'y') ADVANCE(373);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 391:
      ACCEPT_TOKEN(aux_sym_decorator_token1);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 392:
      ACCEPT_TOKEN(anon_sym_LPAREN);
      END_STATE();
    case 393:
      ACCEPT_TOKEN(anon_sym_COMMA);
      END_STATE();
    case 394:
      ACCEPT_TOKEN(anon_sym_RPAREN);
      END_STATE();
    case 395:
      ACCEPT_TOKEN(anon_sym_SEMI);
      END_STATE();
    case 396:
      ACCEPT_TOKEN(anon_sym_ATout);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 397:
      ACCEPT_TOKEN(anon_sym_ATproject);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 398:
      ACCEPT_TOKEN(anon_sym_ATmodule);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 399:
      ACCEPT_TOKEN(anon_sym_ATauthor);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 400:
      ACCEPT_TOKEN(anon_sym_ATcopyright);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 401:
      ACCEPT_TOKEN(anon_sym_ATversion);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 402:
      ACCEPT_TOKEN(anon_sym_ATlicense);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 403:
      ACCEPT_TOKEN(anon_sym_ATemail);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 404:
      ACCEPT_TOKEN(anon_sym_ATformat);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 405:
      ACCEPT_TOKEN(anon_sym_requires);
      END_STATE();
    case 406:
      ACCEPT_TOKEN(anon_sym_set);
      END_STATE();
    case 407:
      ACCEPT_TOKEN(anon_sym_rsrc);
      END_STATE();
    case 408:
      ACCEPT_TOKEN(anon_sym_classic);
      END_STATE();
    case 409:
      ACCEPT_TOKEN(anon_sym_extended);
      END_STATE();
    case 410:
      ACCEPT_TOKEN(anon_sym_kestrel);
      END_STATE();
    case 411:
      ACCEPT_TOKEN(anon_sym_rez);
      END_STATE();
    case 412:
      ACCEPT_TOKEN(anon_sym_ATimport);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 413:
      ACCEPT_TOKEN(anon_sym_ATvar);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 414:
      ACCEPT_TOKEN(anon_sym_ATconst);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 415:
      ACCEPT_TOKEN(anon_sym_EQ);
      END_STATE();
    case 416:
      ACCEPT_TOKEN(anon_sym_ATfunction);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(391);
      END_STATE();
    case 417:
      ACCEPT_TOKEN(anon_sym_type);
      END_STATE();
    case 418:
      ACCEPT_TOKEN(anon_sym_COLON);
      END_STATE();
    case 419:
      ACCEPT_TOKEN(anon_sym_LBRACE);
      END_STATE();
    case 420:
      ACCEPT_TOKEN(anon_sym_RBRACE);
      END_STATE();
    case 421:
      ACCEPT_TOKEN(anon_sym_template);
      END_STATE();
    case 422:
      ACCEPT_TOKEN(anon_sym_template);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 423:
      ACCEPT_TOKEN(anon_sym_constructor);
      END_STATE();
    case 424:
      ACCEPT_TOKEN(anon_sym_DOT);
      END_STATE();
    case 425:
      ACCEPT_TOKEN(anon_sym_RBRACE_SEMI);
      END_STATE();
    case 426:
      ACCEPT_TOKEN(anon_sym_field);
      END_STATE();
    case 427:
      ACCEPT_TOKEN(anon_sym_repeatable);
      END_STATE();
    case 428:
      ACCEPT_TOKEN(anon_sym_as);
      END_STATE();
    case 429:
      ACCEPT_TOKEN(anon_sym_AMP);
      END_STATE();
    case 430:
      ACCEPT_TOKEN(anon_sym_LBRACK);
      END_STATE();
    case 431:
      ACCEPT_TOKEN(anon_sym_RBRACK);
      END_STATE();
    case 432:
      ACCEPT_TOKEN(anon_sym_declare);
      END_STATE();
    case 433:
      ACCEPT_TOKEN(anon_sym_new);
      END_STATE();
    case 434:
      ACCEPT_TOKEN(anon_sym_new);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 435:
      ACCEPT_TOKEN(anon_sym_override);
      END_STATE();
    case 436:
      ACCEPT_TOKEN(anon_sym_duplicate);
      END_STATE();
    case 437:
      ACCEPT_TOKEN(anon_sym_import);
      END_STATE();
    case 438:
      ACCEPT_TOKEN(anon_sym_import);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 439:
      ACCEPT_TOKEN(anon_sym_component);
      END_STATE();
    case 440:
      ACCEPT_TOKEN(anon_sym_api_export);
      END_STATE();
    case 441:
      ACCEPT_TOKEN(anon_sym_files);
      END_STATE();
    case 442:
      ACCEPT_TOKEN(anon_sym_DASH_GT);
      END_STATE();
    case 443:
      ACCEPT_TOKEN(anon_sym_scene);
      END_STATE();
    case 444:
      ACCEPT_TOKEN(anon_sym_Title);
      END_STATE();
    case 445:
      ACCEPT_TOKEN(anon_sym_Flags);
      END_STATE();
    case 446:
      ACCEPT_TOKEN(anon_sym_Size);
      END_STATE();
    case 447:
      ACCEPT_TOKEN(anon_sym_Button);
      END_STATE();
    case 448:
      ACCEPT_TOKEN(anon_sym_Label);
      if (lookahead == 'e') ADVANCE(102);
      END_STATE();
    case 449:
      ACCEPT_TOKEN(anon_sym_TextArea);
      END_STATE();
    case 450:
      ACCEPT_TOKEN(anon_sym_Image);
      END_STATE();
    case 451:
      ACCEPT_TOKEN(anon_sym_TextField);
      END_STATE();
    case 452:
      ACCEPT_TOKEN(anon_sym_Checkbox);
      END_STATE();
    case 453:
      ACCEPT_TOKEN(anon_sym_List);
      END_STATE();
    case 454:
      ACCEPT_TOKEN(anon_sym_ScrollArea);
      END_STATE();
    case 455:
      ACCEPT_TOKEN(anon_sym_Grid);
      END_STATE();
    case 456:
      ACCEPT_TOKEN(anon_sym_LabeledList);
      END_STATE();
    case 457:
      ACCEPT_TOKEN(anon_sym_Canvas);
      END_STATE();
    case 458:
      ACCEPT_TOKEN(anon_sym_Sprite);
      END_STATE();
    case 459:
      ACCEPT_TOKEN(anon_sym_PopupButton);
      END_STATE();
    case 460:
      ACCEPT_TOKEN(anon_sym_Slider);
      END_STATE();
    case 461:
      ACCEPT_TOKEN(anon_sym_Table);
      END_STATE();
    case 462:
      ACCEPT_TOKEN(anon_sym_Box);
      END_STATE();
    case 463:
      ACCEPT_TOKEN(anon_sym_Radio);
      END_STATE();
    case 464:
      ACCEPT_TOKEN(anon_sym_TabBar);
      END_STATE();
    case 465:
      ACCEPT_TOKEN(anon_sym_Separator);
      END_STATE();
    case 466:
      ACCEPT_TOKEN(anon_sym_Spacer);
      END_STATE();
    case 467:
      ACCEPT_TOKEN(anon_sym_Position);
      END_STATE();
    case 468:
      ACCEPT_TOKEN(anon_sym_Frame);
      END_STATE();
    case 469:
      ACCEPT_TOKEN(anon_sym_Value);
      END_STATE();
    case 470:
      ACCEPT_TOKEN(anon_sym_Action);
      END_STATE();
    case 471:
      ACCEPT_TOKEN(sym_variable);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(471);
      END_STATE();
    case 472:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'a') ADVANCE(484);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('b' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 473:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'e') ADVANCE(485);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 474:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'e') ADVANCE(422);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 475:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'e') ADVANCE(478);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 476:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'l') ADVANCE(472);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 477:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'm') ADVANCE(480);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 478:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'm') ADVANCE(481);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 479:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'o') ADVANCE(482);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 480:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'p') ADVANCE(479);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 481:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'p') ADVANCE(476);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 482:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'r') ADVANCE(483);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 483:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 't') ADVANCE(438);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 484:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 't') ADVANCE(474);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 485:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (lookahead == 'w') ADVANCE(434);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 486:
      ACCEPT_TOKEN(sym_identifier_literal);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'Z') ||
          lookahead == '_' ||
          ('a' <= lookahead && lookahead <= 'z')) ADVANCE(486);
      END_STATE();
    case 487:
      ACCEPT_TOKEN(sym_integer_literal);
      if (lookahead == '%') ADVANCE(490);
      if (lookahead == 'x') ADVANCE(298);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(488);
      END_STATE();
    case 488:
      ACCEPT_TOKEN(sym_integer_literal);
      if (lookahead == '%') ADVANCE(490);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(488);
      END_STATE();
    case 489:
      ACCEPT_TOKEN(sym_hex_literal);
      if (('0' <= lookahead && lookahead <= '9') ||
          ('A' <= lookahead && lookahead <= 'F') ||
          ('a' <= lookahead && lookahead <= 'f')) ADVANCE(489);
      END_STATE();
    case 490:
      ACCEPT_TOKEN(sym_percentage_literal);
      END_STATE();
    case 491:
      ACCEPT_TOKEN(anon_sym_POUNDauto);
      END_STATE();
    case 492:
      ACCEPT_TOKEN(anon_sym_POUND);
      if (lookahead == 'a') ADVANCE(281);
      END_STATE();
    case 493:
      ACCEPT_TOKEN(anon_sym_DQUOTE);
      END_STATE();
    case 494:
      ACCEPT_TOKEN(aux_sym_string_literal_token1);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(494);
      if (lookahead != 0 &&
          lookahead != '\n' &&
          lookahead != '"' &&
          lookahead != '\\') ADVANCE(495);
      END_STATE();
    case 495:
      ACCEPT_TOKEN(aux_sym_string_literal_token1);
      if (lookahead != 0 &&
          lookahead != '\n' &&
          lookahead != '"' &&
          lookahead != '\\') ADVANCE(495);
      END_STATE();
    case 496:
      ACCEPT_TOKEN(anon_sym_TILDE);
      END_STATE();
    case 497:
      ACCEPT_TOKEN(anon_sym_PLUS);
      END_STATE();
    case 498:
      ACCEPT_TOKEN(anon_sym_DASH);
      END_STATE();
    case 499:
      ACCEPT_TOKEN(anon_sym_DASH);
      if (lookahead == '>') ADVANCE(442);
      END_STATE();
    case 500:
      ACCEPT_TOKEN(anon_sym_STAR);
      END_STATE();
    case 501:
      ACCEPT_TOKEN(anon_sym_SLASH);
      END_STATE();
    case 502:
      ACCEPT_TOKEN(anon_sym_CARET);
      END_STATE();
    case 503:
      ACCEPT_TOKEN(anon_sym_PIPE);
      END_STATE();
    case 504:
      ACCEPT_TOKEN(anon_sym_LT_LT);
      END_STATE();
    case 505:
      ACCEPT_TOKEN(anon_sym_GT_GT);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 0},
  [2] = {.lex_state = 0},
  [3] = {.lex_state = 0},
  [4] = {.lex_state = 0},
  [5] = {.lex_state = 0},
  [6] = {.lex_state = 0},
  [7] = {.lex_state = 0},
  [8] = {.lex_state = 0},
  [9] = {.lex_state = 0},
  [10] = {.lex_state = 0},
  [11] = {.lex_state = 0},
  [12] = {.lex_state = 6},
  [13] = {.lex_state = 6},
  [14] = {.lex_state = 0},
  [15] = {.lex_state = 0},
  [16] = {.lex_state = 0},
  [17] = {.lex_state = 0},
  [18] = {.lex_state = 2},
  [19] = {.lex_state = 2},
  [20] = {.lex_state = 0},
  [21] = {.lex_state = 0},
  [22] = {.lex_state = 0},
  [23] = {.lex_state = 0},
  [24] = {.lex_state = 0},
  [25] = {.lex_state = 3},
  [26] = {.lex_state = 3},
  [27] = {.lex_state = 3},
  [28] = {.lex_state = 4},
  [29] = {.lex_state = 7},
  [30] = {.lex_state = 7},
  [31] = {.lex_state = 4},
  [32] = {.lex_state = 7},
  [33] = {.lex_state = 7},
  [34] = {.lex_state = 4},
  [35] = {.lex_state = 4},
  [36] = {.lex_state = 4},
  [37] = {.lex_state = 4},
  [38] = {.lex_state = 4},
  [39] = {.lex_state = 4},
  [40] = {.lex_state = 4},
  [41] = {.lex_state = 4},
  [42] = {.lex_state = 4},
  [43] = {.lex_state = 4},
  [44] = {.lex_state = 4},
  [45] = {.lex_state = 4},
  [46] = {.lex_state = 4},
  [47] = {.lex_state = 7},
  [48] = {.lex_state = 4},
  [49] = {.lex_state = 4},
  [50] = {.lex_state = 4},
  [51] = {.lex_state = 4},
  [52] = {.lex_state = 4},
  [53] = {.lex_state = 7},
  [54] = {.lex_state = 7},
  [55] = {.lex_state = 7},
  [56] = {.lex_state = 7},
  [57] = {.lex_state = 7},
  [58] = {.lex_state = 7},
  [59] = {.lex_state = 7},
  [60] = {.lex_state = 7},
  [61] = {.lex_state = 7},
  [62] = {.lex_state = 7},
  [63] = {.lex_state = 7},
  [64] = {.lex_state = 7},
  [65] = {.lex_state = 7},
  [66] = {.lex_state = 7},
  [67] = {.lex_state = 7},
  [68] = {.lex_state = 7},
  [69] = {.lex_state = 7},
  [70] = {.lex_state = 7},
  [71] = {.lex_state = 7},
  [72] = {.lex_state = 5},
  [73] = {.lex_state = 5},
  [74] = {.lex_state = 7},
  [75] = {.lex_state = 5},
  [76] = {.lex_state = 5},
  [77] = {.lex_state = 5},
  [78] = {.lex_state = 7},
  [79] = {.lex_state = 7},
  [80] = {.lex_state = 7},
  [81] = {.lex_state = 7},
  [82] = {.lex_state = 3},
  [83] = {.lex_state = 7},
  [84] = {.lex_state = 5},
  [85] = {.lex_state = 5},
  [86] = {.lex_state = 3},
  [87] = {.lex_state = 5},
  [88] = {.lex_state = 7},
  [89] = {.lex_state = 7},
  [90] = {.lex_state = 7},
  [91] = {.lex_state = 7},
  [92] = {.lex_state = 7},
  [93] = {.lex_state = 7},
  [94] = {.lex_state = 7},
  [95] = {.lex_state = 7},
  [96] = {.lex_state = 5},
  [97] = {.lex_state = 7},
  [98] = {.lex_state = 7},
  [99] = {.lex_state = 5},
  [100] = {.lex_state = 7},
  [101] = {.lex_state = 7},
  [102] = {.lex_state = 7},
  [103] = {.lex_state = 7},
  [104] = {.lex_state = 6},
  [105] = {.lex_state = 0},
  [106] = {.lex_state = 7},
  [107] = {.lex_state = 6},
  [108] = {.lex_state = 6},
  [109] = {.lex_state = 6},
  [110] = {.lex_state = 0},
  [111] = {.lex_state = 0},
  [112] = {.lex_state = 6},
  [113] = {.lex_state = 0},
  [114] = {.lex_state = 6},
  [115] = {.lex_state = 0},
  [116] = {.lex_state = 7},
  [117] = {.lex_state = 6},
  [118] = {.lex_state = 6},
  [119] = {.lex_state = 6},
  [120] = {.lex_state = 0},
  [121] = {.lex_state = 0},
  [122] = {.lex_state = 0},
  [123] = {.lex_state = 7},
  [124] = {.lex_state = 0},
  [125] = {.lex_state = 6},
  [126] = {.lex_state = 0},
  [127] = {.lex_state = 0},
  [128] = {.lex_state = 7},
  [129] = {.lex_state = 7},
  [130] = {.lex_state = 7},
  [131] = {.lex_state = 0},
  [132] = {.lex_state = 7},
  [133] = {.lex_state = 0},
  [134] = {.lex_state = 7},
  [135] = {.lex_state = 7},
  [136] = {.lex_state = 7},
  [137] = {.lex_state = 7},
  [138] = {.lex_state = 7},
  [139] = {.lex_state = 7},
  [140] = {.lex_state = 7},
  [141] = {.lex_state = 7},
  [142] = {.lex_state = 7},
  [143] = {.lex_state = 7},
  [144] = {.lex_state = 7},
  [145] = {.lex_state = 0},
  [146] = {.lex_state = 7},
  [147] = {.lex_state = 2},
  [148] = {.lex_state = 7},
  [149] = {.lex_state = 7},
  [150] = {.lex_state = 7},
  [151] = {.lex_state = 0},
  [152] = {.lex_state = 6},
  [153] = {.lex_state = 0},
  [154] = {.lex_state = 6},
  [155] = {.lex_state = 0},
  [156] = {.lex_state = 0},
  [157] = {.lex_state = 6},
  [158] = {.lex_state = 0},
  [159] = {.lex_state = 0},
  [160] = {.lex_state = 0},
  [161] = {.lex_state = 0},
  [162] = {.lex_state = 0},
  [163] = {.lex_state = 6},
  [164] = {.lex_state = 6},
  [165] = {.lex_state = 0},
  [166] = {.lex_state = 7},
  [167] = {.lex_state = 6},
  [168] = {.lex_state = 6},
  [169] = {.lex_state = 0},
  [170] = {.lex_state = 6},
  [171] = {.lex_state = 6},
  [172] = {.lex_state = 0},
  [173] = {.lex_state = 6},
  [174] = {.lex_state = 6},
  [175] = {.lex_state = 0},
  [176] = {.lex_state = 6},
  [177] = {.lex_state = 6},
  [178] = {.lex_state = 6},
  [179] = {.lex_state = 6},
  [180] = {.lex_state = 6},
  [181] = {.lex_state = 6},
  [182] = {.lex_state = 0},
  [183] = {.lex_state = 6},
  [184] = {.lex_state = 0},
  [185] = {.lex_state = 0},
  [186] = {.lex_state = 0},
  [187] = {.lex_state = 0},
  [188] = {.lex_state = 0},
  [189] = {.lex_state = 0},
  [190] = {.lex_state = 0},
  [191] = {.lex_state = 6},
  [192] = {.lex_state = 6},
  [193] = {.lex_state = 0},
  [194] = {.lex_state = 6},
  [195] = {.lex_state = 1},
  [196] = {.lex_state = 6},
  [197] = {.lex_state = 6},
  [198] = {.lex_state = 2},
  [199] = {.lex_state = 6},
  [200] = {.lex_state = 0},
  [201] = {.lex_state = 6},
  [202] = {.lex_state = 0},
  [203] = {.lex_state = 0},
  [204] = {.lex_state = 0},
  [205] = {.lex_state = 6},
  [206] = {.lex_state = 6},
  [207] = {.lex_state = 6},
  [208] = {.lex_state = 6},
  [209] = {.lex_state = 0},
  [210] = {.lex_state = 0},
  [211] = {.lex_state = 0},
  [212] = {.lex_state = 6},
  [213] = {.lex_state = 0},
  [214] = {.lex_state = 6},
  [215] = {.lex_state = 0},
  [216] = {.lex_state = 0},
  [217] = {.lex_state = 0},
  [218] = {.lex_state = 0},
  [219] = {.lex_state = 0},
  [220] = {.lex_state = 0},
  [221] = {.lex_state = 1},
  [222] = {.lex_state = 1},
  [223] = {.lex_state = 0},
  [224] = {.lex_state = 6},
  [225] = {.lex_state = 0},
  [226] = {.lex_state = 6},
  [227] = {.lex_state = 0},
  [228] = {.lex_state = 0},
  [229] = {.lex_state = 6},
  [230] = {.lex_state = 0},
  [231] = {.lex_state = 0},
  [232] = {.lex_state = 0},
  [233] = {.lex_state = 0},
  [234] = {.lex_state = 0},
  [235] = {.lex_state = 0},
  [236] = {.lex_state = 6},
  [237] = {.lex_state = 0},
  [238] = {.lex_state = 6},
  [239] = {.lex_state = 0},
  [240] = {.lex_state = 0},
  [241] = {.lex_state = 0},
  [242] = {.lex_state = 6},
  [243] = {.lex_state = 0},
  [244] = {.lex_state = 0},
  [245] = {.lex_state = 4},
  [246] = {.lex_state = 4},
  [247] = {.lex_state = 6},
  [248] = {.lex_state = 6},
  [249] = {.lex_state = 2},
  [250] = {.lex_state = 0},
  [251] = {.lex_state = 6},
  [252] = {.lex_state = 0},
  [253] = {.lex_state = 0},
  [254] = {.lex_state = 0},
  [255] = {.lex_state = 0},
  [256] = {.lex_state = 6},
  [257] = {.lex_state = 2},
  [258] = {.lex_state = 0},
  [259] = {.lex_state = 0},
  [260] = {.lex_state = 0},
  [261] = {.lex_state = 6},
  [262] = {.lex_state = 4},
  [263] = {.lex_state = 0},
  [264] = {.lex_state = 0},
  [265] = {.lex_state = 0},
  [266] = {.lex_state = 0},
  [267] = {.lex_state = 0},
  [268] = {.lex_state = 0},
  [269] = {.lex_state = 0},
  [270] = {.lex_state = 0},
  [271] = {.lex_state = 0},
  [272] = {.lex_state = 0},
  [273] = {.lex_state = 0},
  [274] = {.lex_state = 0},
  [275] = {.lex_state = 4},
  [276] = {.lex_state = 6},
  [277] = {.lex_state = 0},
  [278] = {.lex_state = 4},
  [279] = {.lex_state = 0},
  [280] = {.lex_state = 0},
  [281] = {.lex_state = 0},
  [282] = {.lex_state = 0},
  [283] = {.lex_state = 0},
  [284] = {.lex_state = 0},
  [285] = {.lex_state = 0},
  [286] = {.lex_state = 6},
  [287] = {.lex_state = 0},
  [288] = {.lex_state = 4},
  [289] = {.lex_state = 0},
  [290] = {.lex_state = 0},
  [291] = {.lex_state = 0},
  [292] = {.lex_state = 0},
  [293] = {.lex_state = 0},
  [294] = {.lex_state = 0},
  [295] = {.lex_state = 6},
  [296] = {.lex_state = 0},
  [297] = {.lex_state = 0},
  [298] = {.lex_state = 0},
  [299] = {.lex_state = 0},
  [300] = {.lex_state = 0},
  [301] = {.lex_state = 0},
  [302] = {.lex_state = 0},
  [303] = {.lex_state = 0},
  [304] = {.lex_state = 0},
  [305] = {.lex_state = 0},
  [306] = {.lex_state = 0},
  [307] = {.lex_state = 0},
  [308] = {.lex_state = 0},
  [309] = {.lex_state = 0},
  [310] = {.lex_state = 0},
  [311] = {.lex_state = 0},
  [312] = {.lex_state = 0},
  [313] = {.lex_state = 0},
  [314] = {.lex_state = 0},
  [315] = {.lex_state = 6},
  [316] = {.lex_state = 0},
  [317] = {.lex_state = 0},
  [318] = {.lex_state = 0},
  [319] = {.lex_state = 0},
  [320] = {.lex_state = 0},
  [321] = {.lex_state = 0},
  [322] = {.lex_state = 0},
  [323] = {.lex_state = 0},
  [324] = {.lex_state = 0},
  [325] = {.lex_state = 0},
  [326] = {.lex_state = 0},
  [327] = {.lex_state = 0},
  [328] = {.lex_state = 0},
  [329] = {.lex_state = 0},
  [330] = {.lex_state = 0},
  [331] = {.lex_state = 0},
  [332] = {.lex_state = 0},
  [333] = {.lex_state = 0},
  [334] = {.lex_state = 0},
  [335] = {.lex_state = 0},
  [336] = {.lex_state = 0},
  [337] = {.lex_state = 0},
  [338] = {.lex_state = 0},
  [339] = {.lex_state = 0},
  [340] = {.lex_state = 0},
  [341] = {.lex_state = 0},
  [342] = {.lex_state = 0},
  [343] = {.lex_state = 0},
  [344] = {.lex_state = 0},
  [345] = {.lex_state = 0},
  [346] = {.lex_state = 0},
  [347] = {.lex_state = 0},
  [348] = {.lex_state = 0},
  [349] = {.lex_state = 0},
  [350] = {.lex_state = 6},
  [351] = {.lex_state = 0},
  [352] = {.lex_state = 0},
  [353] = {.lex_state = 6},
  [354] = {.lex_state = 0},
  [355] = {.lex_state = 0},
  [356] = {.lex_state = 0},
  [357] = {.lex_state = 0},
  [358] = {.lex_state = 0},
  [359] = {.lex_state = 0},
  [360] = {.lex_state = 0},
  [361] = {.lex_state = 6},
  [362] = {.lex_state = 0},
  [363] = {.lex_state = 6},
  [364] = {.lex_state = 0},
  [365] = {.lex_state = 6},
  [366] = {.lex_state = 0},
  [367] = {.lex_state = 0},
  [368] = {.lex_state = 0},
  [369] = {.lex_state = 2},
  [370] = {.lex_state = 0},
  [371] = {.lex_state = 0},
  [372] = {.lex_state = 0},
  [373] = {.lex_state = 0},
  [374] = {.lex_state = 0},
  [375] = {.lex_state = 0},
  [376] = {.lex_state = 0},
  [377] = {.lex_state = 6},
  [378] = {.lex_state = 0},
  [379] = {.lex_state = 0},
  [380] = {.lex_state = 6},
  [381] = {.lex_state = 0},
  [382] = {.lex_state = 0},
  [383] = {.lex_state = 0},
  [384] = {.lex_state = 0},
  [385] = {.lex_state = 0},
  [386] = {.lex_state = 6},
  [387] = {.lex_state = 0},
  [388] = {.lex_state = 0},
  [389] = {.lex_state = 0},
  [390] = {.lex_state = 0},
  [391] = {.lex_state = 2},
  [392] = {.lex_state = 2},
  [393] = {.lex_state = 6},
  [394] = {.lex_state = 6},
  [395] = {.lex_state = 2},
  [396] = {.lex_state = 0},
  [397] = {.lex_state = 6},
  [398] = {.lex_state = 6},
  [399] = {.lex_state = 6},
  [400] = {.lex_state = 0},
  [401] = {.lex_state = 0},
  [402] = {.lex_state = 0},
  [403] = {.lex_state = 0},
  [404] = {.lex_state = 0},
  [405] = {.lex_state = 0},
  [406] = {.lex_state = 0},
  [407] = {.lex_state = 0},
  [408] = {.lex_state = 0},
  [409] = {.lex_state = 0},
  [410] = {.lex_state = 0},
  [411] = {.lex_state = 0},
  [412] = {.lex_state = 0},
  [413] = {.lex_state = 2},
  [414] = {.lex_state = 0},
  [415] = {.lex_state = 0},
  [416] = {.lex_state = 6},
  [417] = {.lex_state = 0},
  [418] = {.lex_state = 6},
  [419] = {.lex_state = 6},
  [420] = {.lex_state = 0},
  [421] = {.lex_state = 0},
  [422] = {.lex_state = 6},
  [423] = {.lex_state = 2},
  [424] = {.lex_state = 0},
  [425] = {.lex_state = 0},
  [426] = {.lex_state = 0},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [anon_sym_CSTR] = ACTIONS(1),
    [anon_sym_PSTR] = ACTIONS(1),
    [anon_sym_CHAR] = ACTIONS(1),
    [anon_sym_DBYT] = ACTIONS(1),
    [anon_sym_DWRD] = ACTIONS(1),
    [anon_sym_DLNG] = ACTIONS(1),
    [anon_sym_DQAD] = ACTIONS(1),
    [anon_sym_HBYT] = ACTIONS(1),
    [anon_sym_HWRD] = ACTIONS(1),
    [anon_sym_HLNG] = ACTIONS(1),
    [anon_sym_HQAD] = ACTIONS(1),
    [anon_sym_BOOL] = ACTIONS(1),
    [anon_sym_BBIT] = ACTIONS(1),
    [anon_sym_RECT] = ACTIONS(1),
    [anon_sym_HEXD] = ACTIONS(1),
    [anon_sym_RSRC] = ACTIONS(1),
    [anon_sym_LSTB] = ACTIONS(1),
    [anon_sym_LSTC] = ACTIONS(1),
    [anon_sym_LSTE] = ACTIONS(1),
    [anon_sym_OCNT] = ACTIONS(1),
    [aux_sym_binary_type_token1] = ACTIONS(1),
    [anon_sym_Nested] = ACTIONS(1),
    [anon_sym_Reference] = ACTIONS(1),
    [anon_sym_LT] = ACTIONS(1),
    [anon_sym_GT] = ACTIONS(1),
    [sym_comment] = ACTIONS(1),
    [aux_sym_decorator_token1] = ACTIONS(1),
    [anon_sym_LPAREN] = ACTIONS(1),
    [anon_sym_COMMA] = ACTIONS(1),
    [anon_sym_RPAREN] = ACTIONS(1),
    [anon_sym_SEMI] = ACTIONS(1),
    [anon_sym_ATout] = ACTIONS(1),
    [anon_sym_ATproject] = ACTIONS(1),
    [anon_sym_ATmodule] = ACTIONS(1),
    [anon_sym_ATauthor] = ACTIONS(1),
    [anon_sym_ATcopyright] = ACTIONS(1),
    [anon_sym_ATversion] = ACTIONS(1),
    [anon_sym_ATlicense] = ACTIONS(1),
    [anon_sym_ATemail] = ACTIONS(1),
    [anon_sym_ATformat] = ACTIONS(1),
    [anon_sym_requires] = ACTIONS(1),
    [anon_sym_set] = ACTIONS(1),
    [anon_sym_rsrc] = ACTIONS(1),
    [anon_sym_classic] = ACTIONS(1),
    [anon_sym_extended] = ACTIONS(1),
    [anon_sym_kestrel] = ACTIONS(1),
    [anon_sym_rez] = ACTIONS(1),
    [anon_sym_ATimport] = ACTIONS(1),
    [anon_sym_ATvar] = ACTIONS(1),
    [anon_sym_ATconst] = ACTIONS(1),
    [anon_sym_EQ] = ACTIONS(1),
    [anon_sym_ATfunction] = ACTIONS(1),
    [anon_sym_type] = ACTIONS(1),
    [anon_sym_COLON] = ACTIONS(1),
    [anon_sym_LBRACE] = ACTIONS(1),
    [anon_sym_RBRACE] = ACTIONS(1),
    [anon_sym_template] = ACTIONS(1),
    [anon_sym_constructor] = ACTIONS(1),
    [anon_sym_DOT] = ACTIONS(1),
    [anon_sym_field] = ACTIONS(1),
    [anon_sym_repeatable] = ACTIONS(1),
    [anon_sym_as] = ACTIONS(1),
    [anon_sym_AMP] = ACTIONS(1),
    [anon_sym_LBRACK] = ACTIONS(1),
    [anon_sym_RBRACK] = ACTIONS(1),
    [anon_sym_declare] = ACTIONS(1),
    [anon_sym_new] = ACTIONS(1),
    [anon_sym_override] = ACTIONS(1),
    [anon_sym_duplicate] = ACTIONS(1),
    [anon_sym_import] = ACTIONS(1),
    [anon_sym_component] = ACTIONS(1),
    [anon_sym_api_export] = ACTIONS(1),
    [anon_sym_files] = ACTIONS(1),
    [anon_sym_DASH_GT] = ACTIONS(1),
    [anon_sym_scene] = ACTIONS(1),
    [anon_sym_Title] = ACTIONS(1),
    [anon_sym_Flags] = ACTIONS(1),
    [anon_sym_Size] = ACTIONS(1),
    [anon_sym_Button] = ACTIONS(1),
    [anon_sym_Label] = ACTIONS(1),
    [anon_sym_TextArea] = ACTIONS(1),
    [anon_sym_Image] = ACTIONS(1),
    [anon_sym_TextField] = ACTIONS(1),
    [anon_sym_Checkbox] = ACTIONS(1),
    [anon_sym_List] = ACTIONS(1),
    [anon_sym_ScrollArea] = ACTIONS(1),
    [anon_sym_Grid] = ACTIONS(1),
    [anon_sym_LabeledList] = ACTIONS(1),
    [anon_sym_Canvas] = ACTIONS(1),
    [anon_sym_Sprite] = ACTIONS(1),
    [anon_sym_PopupButton] = ACTIONS(1),
    [anon_sym_Slider] = ACTIONS(1),
    [anon_sym_Table] = ACTIONS(1),
    [anon_sym_Box] = ACTIONS(1),
    [anon_sym_Radio] = ACTIONS(1),
    [anon_sym_TabBar] = ACTIONS(1),
    [anon_sym_Separator] = ACTIONS(1),
    [anon_sym_Spacer] = ACTIONS(1),
    [anon_sym_Position] = ACTIONS(1),
    [anon_sym_Frame] = ACTIONS(1),
    [anon_sym_Value] = ACTIONS(1),
    [anon_sym_Action] = ACTIONS(1),
    [sym_variable] = ACTIONS(1),
    [sym_hex_literal] = ACTIONS(1),
    [anon_sym_POUNDauto] = ACTIONS(1),
    [anon_sym_POUND] = ACTIONS(1),
    [anon_sym_DQUOTE] = ACTIONS(1),
    [anon_sym_TILDE] = ACTIONS(1),
    [anon_sym_PLUS] = ACTIONS(1),
    [anon_sym_DASH] = ACTIONS(1),
    [anon_sym_STAR] = ACTIONS(1),
    [anon_sym_SLASH] = ACTIONS(1),
    [anon_sym_CARET] = ACTIONS(1),
    [anon_sym_PIPE] = ACTIONS(1),
    [anon_sym_LT_LT] = ACTIONS(1),
    [anon_sym_GT_GT] = ACTIONS(1),
  },
  [1] = {
    [sym_source_file] = STATE(412),
    [sym__top_level] = STATE(3),
    [sym_decorator] = STATE(106),
    [sym_directive] = STATE(3),
    [sym_out_directive] = STATE(411),
    [sym_metadata_directive] = STATE(411),
    [sym_format_directive] = STATE(411),
    [sym_import_directive] = STATE(411),
    [sym_variable_directive] = STATE(411),
    [sym_function_directive] = STATE(411),
    [sym_type_definition] = STATE(3),
    [sym_resource_declaration] = STATE(3),
    [sym_component] = STATE(3),
    [sym_scene] = STATE(3),
    [aux_sym_source_file_repeat1] = STATE(3),
    [aux_sym_type_definition_repeat1] = STATE(106),
    [ts_builtin_sym_end] = ACTIONS(3),
    [sym_comment] = ACTIONS(5),
    [aux_sym_decorator_token1] = ACTIONS(7),
    [anon_sym_ATout] = ACTIONS(9),
    [anon_sym_ATproject] = ACTIONS(11),
    [anon_sym_ATmodule] = ACTIONS(11),
    [anon_sym_ATauthor] = ACTIONS(11),
    [anon_sym_ATcopyright] = ACTIONS(11),
    [anon_sym_ATversion] = ACTIONS(11),
    [anon_sym_ATlicense] = ACTIONS(11),
    [anon_sym_ATemail] = ACTIONS(11),
    [anon_sym_ATformat] = ACTIONS(13),
    [anon_sym_ATimport] = ACTIONS(15),
    [anon_sym_ATvar] = ACTIONS(17),
    [anon_sym_ATconst] = ACTIONS(17),
    [anon_sym_ATfunction] = ACTIONS(19),
    [anon_sym_type] = ACTIONS(21),
    [anon_sym_declare] = ACTIONS(23),
    [anon_sym_component] = ACTIONS(25),
    [anon_sym_scene] = ACTIONS(27),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 16,
    ACTIONS(29), 1,
      ts_builtin_sym_end,
    ACTIONS(31), 1,
      sym_comment,
    ACTIONS(34), 1,
      aux_sym_decorator_token1,
    ACTIONS(37), 1,
      anon_sym_ATout,
    ACTIONS(43), 1,
      anon_sym_ATformat,
    ACTIONS(46), 1,
      anon_sym_ATimport,
    ACTIONS(52), 1,
      anon_sym_ATfunction,
    ACTIONS(55), 1,
      anon_sym_type,
    ACTIONS(58), 1,
      anon_sym_declare,
    ACTIONS(61), 1,
      anon_sym_component,
    ACTIONS(64), 1,
      anon_sym_scene,
    ACTIONS(49), 2,
      anon_sym_ATvar,
      anon_sym_ATconst,
    STATE(106), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(411), 6,
      sym_out_directive,
      sym_metadata_directive,
      sym_format_directive,
      sym_import_directive,
      sym_variable_directive,
      sym_function_directive,
    ACTIONS(40), 7,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
    STATE(2), 7,
      sym__top_level,
      sym_directive,
      sym_type_definition,
      sym_resource_declaration,
      sym_component,
      sym_scene,
      aux_sym_source_file_repeat1,
  [68] = 16,
    ACTIONS(7), 1,
      aux_sym_decorator_token1,
    ACTIONS(9), 1,
      anon_sym_ATout,
    ACTIONS(13), 1,
      anon_sym_ATformat,
    ACTIONS(15), 1,
      anon_sym_ATimport,
    ACTIONS(19), 1,
      anon_sym_ATfunction,
    ACTIONS(21), 1,
      anon_sym_type,
    ACTIONS(23), 1,
      anon_sym_declare,
    ACTIONS(25), 1,
      anon_sym_component,
    ACTIONS(27), 1,
      anon_sym_scene,
    ACTIONS(67), 1,
      ts_builtin_sym_end,
    ACTIONS(69), 1,
      sym_comment,
    ACTIONS(17), 2,
      anon_sym_ATvar,
      anon_sym_ATconst,
    STATE(106), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(411), 6,
      sym_out_directive,
      sym_metadata_directive,
      sym_format_directive,
      sym_import_directive,
      sym_variable_directive,
      sym_function_directive,
    ACTIONS(11), 7,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
    STATE(2), 7,
      sym__top_level,
      sym_directive,
      sym_type_definition,
      sym_resource_declaration,
      sym_component,
      sym_scene,
      aux_sym_source_file_repeat1,
  [136] = 6,
    ACTIONS(71), 1,
      anon_sym_RBRACE,
    ACTIONS(77), 1,
      anon_sym_Label,
    STATE(6), 1,
      aux_sym_scene_body_repeat1,
    STATE(356), 2,
      sym_scene_property,
      sym_scene_element,
    ACTIONS(73), 3,
      anon_sym_Title,
      anon_sym_Flags,
      anon_sym_Size,
    ACTIONS(75), 20,
      anon_sym_Button,
      anon_sym_TextArea,
      anon_sym_Image,
      anon_sym_TextField,
      anon_sym_Checkbox,
      anon_sym_List,
      anon_sym_ScrollArea,
      anon_sym_Grid,
      anon_sym_LabeledList,
      anon_sym_Canvas,
      anon_sym_Sprite,
      anon_sym_PopupButton,
      anon_sym_Slider,
      anon_sym_Table,
      anon_sym_Box,
      anon_sym_Radio,
      anon_sym_TabBar,
      anon_sym_Separator,
      anon_sym_Spacer,
      anon_sym_Position,
  [177] = 6,
    ACTIONS(77), 1,
      anon_sym_Label,
    ACTIONS(79), 1,
      anon_sym_RBRACE,
    STATE(4), 1,
      aux_sym_scene_body_repeat1,
    STATE(356), 2,
      sym_scene_property,
      sym_scene_element,
    ACTIONS(73), 3,
      anon_sym_Title,
      anon_sym_Flags,
      anon_sym_Size,
    ACTIONS(75), 20,
      anon_sym_Button,
      anon_sym_TextArea,
      anon_sym_Image,
      anon_sym_TextField,
      anon_sym_Checkbox,
      anon_sym_List,
      anon_sym_ScrollArea,
      anon_sym_Grid,
      anon_sym_LabeledList,
      anon_sym_Canvas,
      anon_sym_Sprite,
      anon_sym_PopupButton,
      anon_sym_Slider,
      anon_sym_Table,
      anon_sym_Box,
      anon_sym_Radio,
      anon_sym_TabBar,
      anon_sym_Separator,
      anon_sym_Spacer,
      anon_sym_Position,
  [218] = 6,
    ACTIONS(81), 1,
      anon_sym_RBRACE,
    ACTIONS(89), 1,
      anon_sym_Label,
    STATE(6), 1,
      aux_sym_scene_body_repeat1,
    STATE(356), 2,
      sym_scene_property,
      sym_scene_element,
    ACTIONS(83), 3,
      anon_sym_Title,
      anon_sym_Flags,
      anon_sym_Size,
    ACTIONS(86), 20,
      anon_sym_Button,
      anon_sym_TextArea,
      anon_sym_Image,
      anon_sym_TextField,
      anon_sym_Checkbox,
      anon_sym_List,
      anon_sym_ScrollArea,
      anon_sym_Grid,
      anon_sym_LabeledList,
      anon_sym_Canvas,
      anon_sym_Sprite,
      anon_sym_PopupButton,
      anon_sym_Slider,
      anon_sym_Table,
      anon_sym_Box,
      anon_sym_Radio,
      anon_sym_TabBar,
      anon_sym_Separator,
      anon_sym_Spacer,
      anon_sym_Position,
  [259] = 5,
    ACTIONS(96), 1,
      anon_sym_RBRACE,
    STATE(380), 1,
      sym_binary_type,
    ACTIONS(94), 2,
      anon_sym_Nested,
      anon_sym_Reference,
    STATE(8), 2,
      sym_binary_template_field,
      aux_sym_binary_template_repeat1,
    ACTIONS(92), 21,
      anon_sym_CSTR,
      anon_sym_PSTR,
      anon_sym_CHAR,
      anon_sym_DBYT,
      anon_sym_DWRD,
      anon_sym_DLNG,
      anon_sym_DQAD,
      anon_sym_HBYT,
      anon_sym_HWRD,
      anon_sym_HLNG,
      anon_sym_HQAD,
      anon_sym_BOOL,
      anon_sym_BBIT,
      anon_sym_RECT,
      anon_sym_HEXD,
      anon_sym_RSRC,
      anon_sym_LSTB,
      anon_sym_LSTC,
      anon_sym_LSTE,
      anon_sym_OCNT,
      aux_sym_binary_type_token1,
  [297] = 5,
    ACTIONS(104), 1,
      anon_sym_RBRACE,
    STATE(380), 1,
      sym_binary_type,
    ACTIONS(101), 2,
      anon_sym_Nested,
      anon_sym_Reference,
    STATE(8), 2,
      sym_binary_template_field,
      aux_sym_binary_template_repeat1,
    ACTIONS(98), 21,
      anon_sym_CSTR,
      anon_sym_PSTR,
      anon_sym_CHAR,
      anon_sym_DBYT,
      anon_sym_DWRD,
      anon_sym_DLNG,
      anon_sym_DQAD,
      anon_sym_HBYT,
      anon_sym_HWRD,
      anon_sym_HLNG,
      anon_sym_HQAD,
      anon_sym_BOOL,
      anon_sym_BBIT,
      anon_sym_RECT,
      anon_sym_HEXD,
      anon_sym_RSRC,
      anon_sym_LSTB,
      anon_sym_LSTC,
      anon_sym_LSTE,
      anon_sym_OCNT,
      aux_sym_binary_type_token1,
  [335] = 5,
    ACTIONS(106), 1,
      anon_sym_RBRACE,
    STATE(380), 1,
      sym_binary_type,
    ACTIONS(94), 2,
      anon_sym_Nested,
      anon_sym_Reference,
    STATE(7), 2,
      sym_binary_template_field,
      aux_sym_binary_template_repeat1,
    ACTIONS(92), 21,
      anon_sym_CSTR,
      anon_sym_PSTR,
      anon_sym_CHAR,
      anon_sym_DBYT,
      anon_sym_DWRD,
      anon_sym_DLNG,
      anon_sym_DQAD,
      anon_sym_HBYT,
      anon_sym_HWRD,
      anon_sym_HLNG,
      anon_sym_HQAD,
      anon_sym_BOOL,
      anon_sym_BBIT,
      anon_sym_RECT,
      anon_sym_HEXD,
      anon_sym_RSRC,
      anon_sym_LSTB,
      anon_sym_LSTC,
      anon_sym_LSTE,
      anon_sym_OCNT,
      aux_sym_binary_type_token1,
  [373] = 2,
    ACTIONS(108), 1,
      anon_sym_Label,
    ACTIONS(81), 24,
      anon_sym_RBRACE,
      anon_sym_Title,
      anon_sym_Flags,
      anon_sym_Size,
      anon_sym_Button,
      anon_sym_TextArea,
      anon_sym_Image,
      anon_sym_TextField,
      anon_sym_Checkbox,
      anon_sym_List,
      anon_sym_ScrollArea,
      anon_sym_Grid,
      anon_sym_LabeledList,
      anon_sym_Canvas,
      anon_sym_Sprite,
      anon_sym_PopupButton,
      anon_sym_Slider,
      anon_sym_Table,
      anon_sym_Box,
      anon_sym_Radio,
      anon_sym_TabBar,
      anon_sym_Separator,
      anon_sym_Spacer,
      anon_sym_Position,
  [403] = 1,
    ACTIONS(110), 24,
      anon_sym_CSTR,
      anon_sym_PSTR,
      anon_sym_CHAR,
      anon_sym_DBYT,
      anon_sym_DWRD,
      anon_sym_DLNG,
      anon_sym_DQAD,
      anon_sym_HBYT,
      anon_sym_HWRD,
      anon_sym_HLNG,
      anon_sym_HQAD,
      anon_sym_BOOL,
      anon_sym_BBIT,
      anon_sym_RECT,
      anon_sym_HEXD,
      anon_sym_RSRC,
      anon_sym_LSTB,
      anon_sym_LSTC,
      anon_sym_LSTE,
      anon_sym_OCNT,
      aux_sym_binary_type_token1,
      anon_sym_Nested,
      anon_sym_Reference,
      anon_sym_RBRACE,
  [430] = 2,
    ACTIONS(112), 2,
      anon_sym_GT,
      anon_sym_DASH,
    ACTIONS(114), 20,
      aux_sym_decorator_token1,
      anon_sym_LPAREN,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACE,
      anon_sym_AMP,
      anon_sym_LBRACK,
      anon_sym_RBRACK,
      anon_sym_DASH_GT,
      sym_identifier_literal,
      anon_sym_DQUOTE,
      anon_sym_PLUS,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [457] = 2,
    ACTIONS(116), 2,
      anon_sym_GT,
      anon_sym_DASH,
    ACTIONS(118), 20,
      aux_sym_decorator_token1,
      anon_sym_LPAREN,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACE,
      anon_sym_AMP,
      anon_sym_LBRACK,
      anon_sym_RBRACK,
      anon_sym_DASH_GT,
      sym_identifier_literal,
      anon_sym_DQUOTE,
      anon_sym_PLUS,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [484] = 2,
    ACTIONS(120), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(122), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [509] = 2,
    ACTIONS(124), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(126), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [534] = 2,
    ACTIONS(128), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(130), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [559] = 2,
    ACTIONS(132), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(134), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [584] = 13,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(138), 1,
      anon_sym_new,
    ACTIONS(140), 1,
      anon_sym_import,
    ACTIONS(144), 1,
      sym_identifier_literal,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    STATE(66), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(414), 2,
      sym_new_resource,
      sym_resource_declaration_field_value_import,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [631] = 13,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(138), 1,
      anon_sym_new,
    ACTIONS(140), 1,
      anon_sym_import,
    ACTIONS(144), 1,
      sym_identifier_literal,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    STATE(68), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(387), 2,
      sym_new_resource,
      sym_resource_declaration_field_value_import,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [678] = 2,
    ACTIONS(156), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(158), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [703] = 2,
    ACTIONS(160), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(162), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [728] = 2,
    ACTIONS(164), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(166), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [753] = 2,
    ACTIONS(168), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(170), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [778] = 2,
    ACTIONS(172), 6,
      ts_builtin_sym_end,
      sym_comment,
      anon_sym_type,
      anon_sym_declare,
      anon_sym_component,
      anon_sym_scene,
    ACTIONS(174), 14,
      aux_sym_decorator_token1,
      anon_sym_ATout,
      anon_sym_ATproject,
      anon_sym_ATmodule,
      anon_sym_ATauthor,
      anon_sym_ATcopyright,
      anon_sym_ATversion,
      anon_sym_ATlicense,
      anon_sym_ATemail,
      anon_sym_ATformat,
      anon_sym_ATimport,
      anon_sym_ATvar,
      anon_sym_ATconst,
      anon_sym_ATfunction,
  [803] = 13,
    ACTIONS(176), 1,
      anon_sym_LPAREN,
    ACTIONS(179), 1,
      anon_sym_template,
    ACTIONS(182), 1,
      anon_sym_RBRACE_SEMI,
    ACTIONS(187), 1,
      sym_identifier_literal,
    ACTIONS(190), 1,
      sym_integer_literal,
    ACTIONS(193), 1,
      anon_sym_POUNDauto,
    ACTIONS(196), 1,
      anon_sym_POUND,
    ACTIONS(199), 1,
      anon_sym_DQUOTE,
    ACTIONS(202), 1,
      anon_sym_TILDE,
    STATE(25), 1,
      aux_sym_type_constructor_body_repeat1,
    STATE(100), 1,
      sym_expression,
    ACTIONS(184), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [849] = 13,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(144), 1,
      sym_identifier_literal,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(205), 1,
      anon_sym_template,
    ACTIONS(207), 1,
      anon_sym_RBRACE_SEMI,
    STATE(27), 1,
      aux_sym_type_constructor_body_repeat1,
    STATE(100), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [895] = 13,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(144), 1,
      sym_identifier_literal,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(205), 1,
      anon_sym_template,
    ACTIONS(209), 1,
      anon_sym_RBRACE_SEMI,
    STATE(25), 1,
      aux_sym_type_constructor_body_repeat1,
    STATE(100), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [941] = 11,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(71), 1,
      sym_expression,
    STATE(305), 1,
      sym_comma_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [981] = 2,
    ACTIONS(213), 1,
      anon_sym_GT,
    ACTIONS(215), 16,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_as,
      anon_sym_AMP,
      anon_sym_LBRACK,
      anon_sym_DQUOTE,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1003] = 2,
    ACTIONS(217), 1,
      anon_sym_GT,
    ACTIONS(219), 16,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_as,
      anon_sym_AMP,
      anon_sym_LBRACK,
      anon_sym_DQUOTE,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1025] = 11,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(81), 1,
      sym_expression,
    STATE(355), 1,
      sym_comma_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1065] = 2,
    ACTIONS(221), 1,
      anon_sym_GT,
    ACTIONS(223), 16,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_as,
      anon_sym_AMP,
      anon_sym_LBRACK,
      anon_sym_DQUOTE,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1087] = 2,
    ACTIONS(225), 1,
      anon_sym_GT,
    ACTIONS(227), 16,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_as,
      anon_sym_AMP,
      anon_sym_LBRACK,
      anon_sym_DQUOTE,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1109] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(67), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1146] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(79), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1183] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(93), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1220] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(70), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1257] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(56), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1294] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(69), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1331] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(91), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1368] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(64), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1405] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(65), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1442] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(63), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1479] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(57), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1516] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(98), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1553] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(97), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1590] = 4,
    ACTIONS(229), 1,
      anon_sym_GT,
    ACTIONS(231), 1,
      anon_sym_LPAREN,
    STATE(58), 1,
      sym_argument_list,
    ACTIONS(233), 13,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1615] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(61), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1652] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(59), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1689] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(60), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1726] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(94), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1763] = 10,
    ACTIONS(136), 1,
      anon_sym_LPAREN,
    ACTIONS(146), 1,
      sym_integer_literal,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(154), 1,
      anon_sym_TILDE,
    ACTIONS(211), 1,
      sym_identifier_literal,
    STATE(92), 1,
      sym_expression,
    ACTIONS(142), 2,
      sym_variable,
      sym_percentage_literal,
    STATE(62), 6,
      sym_resource_reference_literal,
      sym_string_literal,
      sym_unary_expression,
      sym_binary_expression,
      sym_call_expression,
      sym_parenthesized_expression,
  [1800] = 2,
    ACTIONS(235), 1,
      anon_sym_GT,
    ACTIONS(237), 14,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACE,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1820] = 2,
    ACTIONS(239), 1,
      anon_sym_GT,
    ACTIONS(241), 14,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACE,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1840] = 2,
    ACTIONS(243), 1,
      anon_sym_GT,
    ACTIONS(245), 13,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1859] = 5,
    ACTIONS(247), 1,
      anon_sym_GT,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
    ACTIONS(249), 7,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_CARET,
      anon_sym_PIPE,
  [1884] = 2,
    ACTIONS(257), 1,
      anon_sym_GT,
    ACTIONS(259), 13,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1903] = 2,
    ACTIONS(261), 1,
      anon_sym_GT,
    ACTIONS(263), 13,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1922] = 4,
    ACTIONS(247), 1,
      anon_sym_GT,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(249), 9,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [1945] = 7,
    ACTIONS(247), 1,
      anon_sym_GT,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
    ACTIONS(249), 5,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_PIPE,
  [1974] = 6,
    ACTIONS(247), 1,
      anon_sym_GT,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
    ACTIONS(249), 6,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_CARET,
      anon_sym_PIPE,
  [2001] = 2,
    ACTIONS(229), 1,
      anon_sym_GT,
    ACTIONS(233), 13,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2020] = 2,
    ACTIONS(247), 1,
      anon_sym_GT,
    ACTIONS(249), 13,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_STAR,
      anon_sym_SLASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2039] = 3,
    ACTIONS(247), 1,
      anon_sym_GT,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(249), 11,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_AMP,
      anon_sym_PLUS,
      anon_sym_DASH,
      anon_sym_CARET,
      anon_sym_PIPE,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2060] = 9,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(271), 1,
      anon_sym_SEMI,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    STATE(193), 1,
      aux_sym_resource_declaration_field_value_repeat1,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2091] = 9,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(275), 1,
      anon_sym_SEMI,
    STATE(204), 1,
      aux_sym_resource_declaration_field_value_repeat1,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2122] = 9,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(277), 1,
      anon_sym_SEMI,
    STATE(182), 1,
      aux_sym_resource_declaration_field_value_repeat1,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2153] = 9,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(279), 1,
      anon_sym_SEMI,
    STATE(219), 1,
      aux_sym_resource_declaration_field_value_repeat1,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2184] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
    ACTIONS(281), 3,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
  [2211] = 9,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(283), 1,
      anon_sym_RPAREN,
    STATE(188), 1,
      aux_sym_resource_declaration_field_value_repeat1,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2242] = 8,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(285), 1,
      anon_sym_COMMA,
    ACTIONS(287), 1,
      anon_sym_RPAREN,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2270] = 8,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(291), 1,
      sym_identifier_literal,
    ACTIONS(293), 1,
      sym_integer_literal,
    STATE(225), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2298] = 8,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(295), 1,
      sym_identifier_literal,
    STATE(233), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2326] = 7,
    ACTIONS(297), 1,
      aux_sym_decorator_token1,
    ACTIONS(299), 1,
      anon_sym_RBRACE,
    ACTIONS(301), 1,
      anon_sym_new,
    ACTIONS(303), 1,
      anon_sym_override,
    ACTIONS(305), 1,
      anon_sym_duplicate,
    STATE(102), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(80), 4,
      sym_new_resource,
      sym_override_resource,
      sym_duplicate_resource,
      aux_sym_resource_declaration_body_repeat1,
  [2352] = 8,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(307), 1,
      sym_identifier_literal,
    STATE(187), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2380] = 8,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(309), 1,
      sym_identifier_literal,
    STATE(231), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2408] = 8,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(311), 1,
      sym_identifier_literal,
    STATE(189), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2436] = 7,
    ACTIONS(297), 1,
      aux_sym_decorator_token1,
    ACTIONS(313), 1,
      anon_sym_RBRACE,
    ACTIONS(315), 1,
      anon_sym_template,
    ACTIONS(317), 1,
      anon_sym_constructor,
    ACTIONS(319), 1,
      anon_sym_field,
    STATE(166), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(89), 4,
      sym_binary_template,
      sym_type_constructor,
      sym_field_definition,
      aux_sym_type_definition_body_repeat1,
  [2462] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
    ACTIONS(321), 2,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [2488] = 7,
    ACTIONS(297), 1,
      aux_sym_decorator_token1,
    ACTIONS(301), 1,
      anon_sym_new,
    ACTIONS(303), 1,
      anon_sym_override,
    ACTIONS(305), 1,
      anon_sym_duplicate,
    ACTIONS(323), 1,
      anon_sym_RBRACE,
    STATE(102), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(83), 4,
      sym_new_resource,
      sym_override_resource,
      sym_duplicate_resource,
      aux_sym_resource_declaration_body_repeat1,
  [2514] = 8,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(285), 1,
      anon_sym_COMMA,
    ACTIONS(325), 1,
      anon_sym_RPAREN,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2542] = 2,
    ACTIONS(329), 4,
      anon_sym_template,
      sym_identifier_literal,
      sym_integer_literal,
      anon_sym_POUND,
    ACTIONS(327), 7,
      anon_sym_LPAREN,
      anon_sym_RBRACE_SEMI,
      sym_variable,
      sym_percentage_literal,
      anon_sym_POUNDauto,
      anon_sym_DQUOTE,
      anon_sym_TILDE,
  [2558] = 7,
    ACTIONS(331), 1,
      aux_sym_decorator_token1,
    ACTIONS(334), 1,
      anon_sym_RBRACE,
    ACTIONS(336), 1,
      anon_sym_new,
    ACTIONS(339), 1,
      anon_sym_override,
    ACTIONS(342), 1,
      anon_sym_duplicate,
    STATE(102), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(83), 4,
      sym_new_resource,
      sym_override_resource,
      sym_duplicate_resource,
      aux_sym_resource_declaration_body_repeat1,
  [2584] = 8,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(345), 1,
      sym_identifier_literal,
    STATE(227), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2612] = 8,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(347), 1,
      sym_identifier_literal,
    STATE(202), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2640] = 2,
    ACTIONS(351), 4,
      anon_sym_template,
      sym_identifier_literal,
      sym_integer_literal,
      anon_sym_POUND,
    ACTIONS(349), 7,
      anon_sym_LPAREN,
      anon_sym_RBRACE_SEMI,
      sym_variable,
      sym_percentage_literal,
      anon_sym_POUNDauto,
      anon_sym_DQUOTE,
      anon_sym_TILDE,
  [2656] = 8,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(353), 1,
      sym_identifier_literal,
    STATE(235), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2684] = 7,
    ACTIONS(297), 1,
      aux_sym_decorator_token1,
    ACTIONS(315), 1,
      anon_sym_template,
    ACTIONS(317), 1,
      anon_sym_constructor,
    ACTIONS(319), 1,
      anon_sym_field,
    ACTIONS(355), 1,
      anon_sym_RBRACE,
    STATE(166), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(78), 4,
      sym_binary_template,
      sym_type_constructor,
      sym_field_definition,
      aux_sym_type_definition_body_repeat1,
  [2710] = 7,
    ACTIONS(357), 1,
      aux_sym_decorator_token1,
    ACTIONS(360), 1,
      anon_sym_RBRACE,
    ACTIONS(362), 1,
      anon_sym_template,
    ACTIONS(365), 1,
      anon_sym_constructor,
    ACTIONS(368), 1,
      anon_sym_field,
    STATE(166), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(89), 4,
      sym_binary_template,
      sym_type_constructor,
      sym_field_definition,
      aux_sym_type_definition_body_repeat1,
  [2736] = 3,
    ACTIONS(371), 1,
      aux_sym_decorator_token1,
    STATE(90), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    ACTIONS(374), 8,
      anon_sym_type,
      anon_sym_field,
      anon_sym_declare,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
      anon_sym_component,
      anon_sym_scene,
  [2754] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(376), 1,
      anon_sym_GT,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2779] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(378), 1,
      anon_sym_SEMI,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2804] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(380), 1,
      anon_sym_SEMI,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2829] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(382), 1,
      anon_sym_GT,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2854] = 2,
    ACTIONS(386), 1,
      anon_sym_LPAREN,
    ACTIONS(384), 9,
      aux_sym_decorator_token1,
      anon_sym_type,
      anon_sym_field,
      anon_sym_declare,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
      anon_sym_component,
      anon_sym_scene,
  [2869] = 7,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(388), 1,
      anon_sym_POUNDauto,
    ACTIONS(390), 1,
      anon_sym_POUND,
    STATE(163), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2894] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(392), 1,
      anon_sym_SEMI,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2919] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(394), 1,
      anon_sym_SEMI,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2944] = 7,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(293), 1,
      sym_integer_literal,
    ACTIONS(388), 1,
      anon_sym_POUNDauto,
    ACTIONS(390), 1,
      anon_sym_POUND,
    STATE(164), 1,
      sym_value,
    STATE(104), 2,
      sym_resource_reference_literal,
      sym_string_literal,
    ACTIONS(289), 3,
      sym_variable,
      sym_hex_literal,
      sym_percentage_literal,
  [2969] = 7,
    ACTIONS(265), 1,
      anon_sym_AMP,
    ACTIONS(267), 1,
      anon_sym_CARET,
    ACTIONS(273), 1,
      anon_sym_PIPE,
    ACTIONS(396), 1,
      anon_sym_EQ,
    ACTIONS(251), 2,
      anon_sym_PLUS,
      anon_sym_DASH,
    ACTIONS(253), 2,
      anon_sym_STAR,
      anon_sym_SLASH,
    ACTIONS(255), 2,
      anon_sym_LT_LT,
      anon_sym_GT_GT,
  [2994] = 1,
    ACTIONS(398), 9,
      aux_sym_decorator_token1,
      anon_sym_type,
      anon_sym_field,
      anon_sym_declare,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
      anon_sym_component,
      anon_sym_scene,
  [3006] = 6,
    ACTIONS(297), 1,
      aux_sym_decorator_token1,
    ACTIONS(301), 1,
      anon_sym_new,
    ACTIONS(303), 1,
      anon_sym_override,
    ACTIONS(305), 1,
      anon_sym_duplicate,
    STATE(90), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
    STATE(150), 3,
      sym_new_resource,
      sym_override_resource,
      sym_duplicate_resource,
  [3028] = 1,
    ACTIONS(400), 9,
      aux_sym_decorator_token1,
      anon_sym_type,
      anon_sym_field,
      anon_sym_declare,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
      anon_sym_component,
      anon_sym_scene,
  [3040] = 1,
    ACTIONS(402), 8,
      anon_sym_GT,
      aux_sym_decorator_token1,
      anon_sym_COMMA,
      anon_sym_RPAREN,
      anon_sym_SEMI,
      anon_sym_LBRACK,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [3051] = 4,
    ACTIONS(404), 1,
      anon_sym_RBRACE,
    STATE(110), 1,
      aux_sym_scene_element_body_repeat1,
    STATE(330), 1,
      sym_scene_element_property,
    ACTIONS(406), 4,
      anon_sym_Label,
      anon_sym_Frame,
      anon_sym_Value,
      anon_sym_Action,
  [3067] = 6,
    ACTIONS(297), 1,
      aux_sym_decorator_token1,
    ACTIONS(408), 1,
      anon_sym_type,
    ACTIONS(410), 1,
      anon_sym_declare,
    ACTIONS(412), 1,
      anon_sym_component,
    ACTIONS(414), 1,
      anon_sym_scene,
    STATE(90), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3087] = 5,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(418), 1,
      anon_sym_RBRACE,
    ACTIONS(420), 1,
      sym_identifier_literal,
    STATE(109), 2,
      sym_resource_declaration_field_value,
      aux_sym_resource_declaration_values_repeat1,
    STATE(154), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3105] = 5,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(420), 1,
      sym_identifier_literal,
    ACTIONS(422), 1,
      anon_sym_RBRACE,
    STATE(107), 2,
      sym_resource_declaration_field_value,
      aux_sym_resource_declaration_values_repeat1,
    STATE(154), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3123] = 5,
    ACTIONS(424), 1,
      aux_sym_decorator_token1,
    ACTIONS(427), 1,
      anon_sym_RBRACE,
    ACTIONS(429), 1,
      sym_identifier_literal,
    STATE(109), 2,
      sym_resource_declaration_field_value,
      aux_sym_resource_declaration_values_repeat1,
    STATE(154), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3141] = 4,
    ACTIONS(432), 1,
      anon_sym_RBRACE,
    STATE(111), 1,
      aux_sym_scene_element_body_repeat1,
    STATE(330), 1,
      sym_scene_element_property,
    ACTIONS(406), 4,
      anon_sym_Label,
      anon_sym_Frame,
      anon_sym_Value,
      anon_sym_Action,
  [3157] = 4,
    ACTIONS(434), 1,
      anon_sym_RBRACE,
    STATE(111), 1,
      aux_sym_scene_element_body_repeat1,
    STATE(330), 1,
      sym_scene_element_property,
    ACTIONS(436), 4,
      anon_sym_Label,
      anon_sym_Frame,
      anon_sym_Value,
      anon_sym_Action,
  [3173] = 5,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(439), 1,
      anon_sym_RBRACE,
    ACTIONS(441), 1,
      sym_identifier_literal,
    STATE(119), 1,
      aux_sym_field_definition_values_repeat1,
    STATE(178), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3190] = 5,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(443), 1,
      anon_sym_RBRACE,
    STATE(198), 1,
      sym_string_literal,
    STATE(203), 1,
      sym_named_file_reference,
    STATE(124), 2,
      sym_component_file_reference,
      aux_sym_component_files_repeat1,
  [3207] = 5,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(445), 1,
      anon_sym_RBRACK,
    ACTIONS(447), 1,
      sym_identifier_literal,
    STATE(118), 1,
      aux_sym_field_definition_value_symbol_list_repeat1,
    STATE(177), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3224] = 6,
    ACTIONS(449), 1,
      anon_sym_SEMI,
    ACTIONS(451), 1,
      anon_sym_EQ,
    ACTIONS(453), 1,
      anon_sym_as,
    ACTIONS(455), 1,
      anon_sym_LBRACK,
    STATE(153), 1,
      sym_field_explicit_type,
    STATE(348), 1,
      sym_field_definition_value_symbol_list,
  [3243] = 1,
    ACTIONS(457), 6,
      aux_sym_decorator_token1,
      anon_sym_SEMI,
      anon_sym_RBRACE,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
  [3252] = 5,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(441), 1,
      sym_identifier_literal,
    ACTIONS(459), 1,
      anon_sym_RBRACE,
    STATE(112), 1,
      aux_sym_field_definition_values_repeat1,
    STATE(178), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3269] = 5,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(447), 1,
      sym_identifier_literal,
    ACTIONS(461), 1,
      anon_sym_RBRACK,
    STATE(125), 1,
      aux_sym_field_definition_value_symbol_list_repeat1,
    STATE(177), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3286] = 5,
    ACTIONS(463), 1,
      aux_sym_decorator_token1,
    ACTIONS(466), 1,
      anon_sym_RBRACE,
    ACTIONS(468), 1,
      sym_identifier_literal,
    STATE(119), 1,
      aux_sym_field_definition_values_repeat1,
    STATE(178), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3303] = 5,
    ACTIONS(471), 1,
      anon_sym_RBRACE,
    ACTIONS(473), 1,
      anon_sym_api_export,
    ACTIONS(476), 1,
      anon_sym_files,
    STATE(120), 1,
      aux_sym_component_body_repeat1,
    STATE(337), 2,
      sym_component_types,
      sym_component_files,
  [3320] = 5,
    ACTIONS(479), 1,
      anon_sym_RBRACE,
    ACTIONS(481), 1,
      anon_sym_DQUOTE,
    STATE(198), 1,
      sym_string_literal,
    STATE(203), 1,
      sym_named_file_reference,
    STATE(121), 2,
      sym_component_file_reference,
      aux_sym_component_files_repeat1,
  [3337] = 5,
    ACTIONS(484), 1,
      anon_sym_RBRACE,
    ACTIONS(486), 1,
      anon_sym_api_export,
    ACTIONS(488), 1,
      anon_sym_files,
    STATE(120), 1,
      aux_sym_component_body_repeat1,
    STATE(337), 2,
      sym_component_types,
      sym_component_files,
  [3354] = 1,
    ACTIONS(490), 6,
      aux_sym_decorator_token1,
      anon_sym_SEMI,
      anon_sym_RBRACE,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
  [3363] = 5,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(492), 1,
      anon_sym_RBRACE,
    STATE(198), 1,
      sym_string_literal,
    STATE(203), 1,
      sym_named_file_reference,
    STATE(121), 2,
      sym_component_file_reference,
      aux_sym_component_files_repeat1,
  [3380] = 5,
    ACTIONS(494), 1,
      aux_sym_decorator_token1,
    ACTIONS(497), 1,
      anon_sym_RBRACK,
    ACTIONS(499), 1,
      sym_identifier_literal,
    STATE(125), 1,
      aux_sym_field_definition_value_symbol_list_repeat1,
    STATE(177), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3397] = 6,
    ACTIONS(453), 1,
      anon_sym_as,
    ACTIONS(455), 1,
      anon_sym_LBRACK,
    ACTIONS(502), 1,
      anon_sym_SEMI,
    ACTIONS(504), 1,
      anon_sym_EQ,
    STATE(158), 1,
      sym_field_explicit_type,
    STATE(317), 1,
      sym_field_definition_value_symbol_list,
  [3416] = 5,
    ACTIONS(486), 1,
      anon_sym_api_export,
    ACTIONS(488), 1,
      anon_sym_files,
    ACTIONS(506), 1,
      anon_sym_RBRACE,
    STATE(122), 1,
      aux_sym_component_body_repeat1,
    STATE(337), 2,
      sym_component_types,
      sym_component_files,
  [3433] = 1,
    ACTIONS(508), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3441] = 1,
    ACTIONS(510), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
  [3449] = 1,
    ACTIONS(512), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3457] = 1,
    ACTIONS(434), 5,
      anon_sym_RBRACE,
      anon_sym_Label,
      anon_sym_Frame,
      anon_sym_Value,
      anon_sym_Action,
  [3465] = 1,
    ACTIONS(514), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3473] = 1,
    ACTIONS(516), 5,
      anon_sym_rsrc,
      anon_sym_classic,
      anon_sym_extended,
      anon_sym_kestrel,
      anon_sym_rez,
  [3481] = 1,
    ACTIONS(518), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3489] = 1,
    ACTIONS(520), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3497] = 1,
    ACTIONS(522), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
  [3505] = 1,
    ACTIONS(524), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3513] = 1,
    ACTIONS(526), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3521] = 1,
    ACTIONS(528), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
  [3529] = 1,
    ACTIONS(530), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3537] = 1,
    ACTIONS(532), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3545] = 1,
    ACTIONS(534), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3553] = 1,
    ACTIONS(536), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3561] = 1,
    ACTIONS(538), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3569] = 5,
    ACTIONS(540), 1,
      anon_sym_SEMI,
    ACTIONS(542), 1,
      anon_sym_LBRACE,
    ACTIONS(544), 1,
      anon_sym_repeatable,
    STATE(184), 1,
      sym_field_repeatable,
    STATE(304), 1,
      sym_field_definition_values,
  [3585] = 1,
    ACTIONS(546), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3593] = 3,
    ACTIONS(548), 1,
      anon_sym_LT,
    ACTIONS(552), 1,
      anon_sym_AMP,
    ACTIONS(550), 3,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACK,
  [3605] = 1,
    ACTIONS(554), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3613] = 1,
    ACTIONS(556), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_template,
      anon_sym_constructor,
      anon_sym_field,
  [3621] = 1,
    ACTIONS(334), 5,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      anon_sym_new,
      anon_sym_override,
      anon_sym_duplicate,
  [3629] = 5,
    ACTIONS(542), 1,
      anon_sym_LBRACE,
    ACTIONS(544), 1,
      anon_sym_repeatable,
    ACTIONS(558), 1,
      anon_sym_SEMI,
    STATE(210), 1,
      sym_field_repeatable,
    STATE(384), 1,
      sym_field_definition_values,
  [3645] = 3,
    ACTIONS(562), 1,
      anon_sym_AMP,
    ACTIONS(564), 1,
      sym_identifier_literal,
    ACTIONS(560), 3,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACK,
  [3657] = 4,
    ACTIONS(455), 1,
      anon_sym_LBRACK,
    ACTIONS(566), 1,
      anon_sym_SEMI,
    ACTIONS(568), 1,
      anon_sym_EQ,
    STATE(374), 1,
      sym_field_definition_value_symbol_list,
  [3670] = 3,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(570), 1,
      sym_identifier_literal,
    STATE(168), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3681] = 3,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(572), 1,
      anon_sym_SEMI,
    STATE(165), 2,
      sym_string_literal,
      aux_sym_out_directive_repeat1,
  [3692] = 3,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(574), 1,
      anon_sym_SEMI,
    STATE(159), 2,
      sym_string_literal,
      aux_sym_out_directive_repeat1,
  [3703] = 3,
    ACTIONS(576), 1,
      anon_sym_RBRACE,
    ACTIONS(578), 1,
      sym_identifier_literal,
    STATE(170), 2,
      sym_component_type_reference,
      aux_sym_component_types_repeat1,
  [3714] = 4,
    ACTIONS(455), 1,
      anon_sym_LBRACK,
    ACTIONS(580), 1,
      anon_sym_SEMI,
    ACTIONS(582), 1,
      anon_sym_EQ,
    STATE(344), 1,
      sym_field_definition_value_symbol_list,
  [3727] = 3,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(584), 1,
      anon_sym_SEMI,
    STATE(165), 2,
      sym_string_literal,
      aux_sym_out_directive_repeat1,
  [3738] = 3,
    ACTIONS(586), 1,
      anon_sym_COMMA,
    STATE(160), 1,
      aux_sym_resource_declaration_field_value_repeat1,
    ACTIONS(281), 2,
      anon_sym_RPAREN,
      anon_sym_SEMI,
  [3749] = 2,
    ACTIONS(591), 1,
      anon_sym_AMP,
    ACTIONS(589), 3,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACK,
  [3758] = 2,
    ACTIONS(595), 1,
      anon_sym_AMP,
    ACTIONS(593), 3,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACK,
  [3767] = 2,
    ACTIONS(599), 1,
      anon_sym_COMMA,
    ACTIONS(597), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [3776] = 2,
    ACTIONS(603), 1,
      anon_sym_COMMA,
    ACTIONS(601), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [3785] = 3,
    ACTIONS(605), 1,
      anon_sym_SEMI,
    ACTIONS(607), 1,
      anon_sym_DQUOTE,
    STATE(165), 2,
      sym_string_literal,
      aux_sym_out_directive_repeat1,
  [3796] = 3,
    ACTIONS(297), 1,
      aux_sym_decorator_token1,
    ACTIONS(610), 1,
      anon_sym_field,
    STATE(90), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3807] = 3,
    ACTIONS(612), 1,
      anon_sym_RBRACE,
    ACTIONS(614), 1,
      sym_identifier_literal,
    STATE(167), 2,
      sym_component_type_reference,
      aux_sym_component_types_repeat1,
  [3818] = 3,
    ACTIONS(374), 1,
      sym_identifier_literal,
    ACTIONS(617), 1,
      aux_sym_decorator_token1,
    STATE(168), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3829] = 4,
    ACTIONS(231), 1,
      anon_sym_LPAREN,
    ACTIONS(620), 1,
      anon_sym_LBRACE,
    STATE(280), 1,
      sym_argument_list,
    STATE(351), 1,
      sym_resource_declaration_values,
  [3842] = 3,
    ACTIONS(578), 1,
      sym_identifier_literal,
    ACTIONS(622), 1,
      anon_sym_RBRACE,
    STATE(167), 2,
      sym_component_type_reference,
      aux_sym_component_types_repeat1,
  [3853] = 1,
    ACTIONS(227), 4,
      aux_sym_decorator_token1,
      anon_sym_COMMA,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [3860] = 3,
    ACTIONS(626), 1,
      anon_sym_COMMA,
    STATE(172), 1,
      aux_sym_decorator_repeat1,
    ACTIONS(624), 2,
      anon_sym_GT,
      anon_sym_RPAREN,
  [3871] = 1,
    ACTIONS(219), 4,
      aux_sym_decorator_token1,
      anon_sym_COMMA,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [3878] = 1,
    ACTIONS(215), 4,
      aux_sym_decorator_token1,
      anon_sym_COMMA,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [3885] = 3,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(629), 1,
      anon_sym_SEMI,
    STATE(155), 2,
      sym_string_literal,
      aux_sym_out_directive_repeat1,
  [3896] = 1,
    ACTIONS(223), 4,
      aux_sym_decorator_token1,
      anon_sym_COMMA,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [3903] = 3,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(631), 1,
      sym_identifier_literal,
    STATE(168), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3914] = 3,
    ACTIONS(416), 1,
      aux_sym_decorator_token1,
    ACTIONS(633), 1,
      sym_identifier_literal,
    STATE(168), 2,
      sym_decorator,
      aux_sym_type_definition_repeat1,
  [3925] = 1,
    ACTIONS(635), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [3931] = 1,
    ACTIONS(637), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [3937] = 1,
    ACTIONS(639), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [3943] = 3,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(641), 1,
      anon_sym_SEMI,
    STATE(160), 1,
      aux_sym_resource_declaration_field_value_repeat1,
  [3953] = 1,
    ACTIONS(643), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [3959] = 3,
    ACTIONS(542), 1,
      anon_sym_LBRACE,
    ACTIONS(645), 1,
      anon_sym_SEMI,
    STATE(324), 1,
      sym_field_definition_values,
  [3969] = 3,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    STATE(216), 1,
      sym_resource_reference_literal,
  [3979] = 3,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    STATE(382), 1,
      sym_resource_reference_literal,
  [3989] = 3,
    ACTIONS(455), 1,
      anon_sym_LBRACK,
    ACTIONS(647), 1,
      anon_sym_SEMI,
    STATE(403), 1,
      sym_field_definition_value_symbol_list,
  [3999] = 3,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(649), 1,
      anon_sym_RPAREN,
    STATE(160), 1,
      aux_sym_resource_declaration_field_value_repeat1,
  [4009] = 3,
    ACTIONS(651), 1,
      anon_sym_COMMA,
    ACTIONS(653), 1,
      anon_sym_RPAREN,
    STATE(217), 1,
      aux_sym_decorator_repeat1,
  [4019] = 3,
    ACTIONS(651), 1,
      anon_sym_COMMA,
    ACTIONS(655), 1,
      anon_sym_GT,
    STATE(172), 1,
      aux_sym_decorator_repeat1,
  [4029] = 1,
    ACTIONS(657), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4035] = 1,
    ACTIONS(659), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [4041] = 3,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(661), 1,
      anon_sym_SEMI,
    STATE(160), 1,
      aux_sym_resource_declaration_field_value_repeat1,
  [4051] = 1,
    ACTIONS(663), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4057] = 3,
    ACTIONS(665), 1,
      anon_sym_DQUOTE,
    ACTIONS(667), 1,
      aux_sym_string_literal_token1,
    STATE(221), 1,
      aux_sym_string_literal_repeat1,
  [4067] = 1,
    ACTIONS(669), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4073] = 1,
    ACTIONS(671), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4079] = 2,
    ACTIONS(675), 1,
      anon_sym_DASH_GT,
    ACTIONS(673), 2,
      anon_sym_LPAREN,
      anon_sym_SEMI,
  [4087] = 1,
    ACTIONS(677), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4093] = 1,
    ACTIONS(679), 3,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACK,
  [4099] = 1,
    ACTIONS(681), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4105] = 3,
    ACTIONS(651), 1,
      anon_sym_COMMA,
    ACTIONS(683), 1,
      anon_sym_GT,
    STATE(190), 1,
      aux_sym_decorator_repeat1,
  [4115] = 3,
    ACTIONS(685), 1,
      anon_sym_LPAREN,
    ACTIONS(687), 1,
      anon_sym_SEMI,
    STATE(307), 1,
      sym_file_reference_attributes,
  [4125] = 3,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(689), 1,
      anon_sym_SEMI,
    STATE(160), 1,
      aux_sym_resource_declaration_field_value_repeat1,
  [4135] = 1,
    ACTIONS(691), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4141] = 1,
    ACTIONS(693), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4147] = 1,
    ACTIONS(695), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACK,
      sym_identifier_literal,
  [4153] = 1,
    ACTIONS(697), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4159] = 3,
    ACTIONS(699), 1,
      anon_sym_COMMA,
    ACTIONS(701), 1,
      anon_sym_RPAREN,
    STATE(237), 1,
      aux_sym_type_constructor_repeat1,
  [4169] = 3,
    ACTIONS(542), 1,
      anon_sym_LBRACE,
    ACTIONS(703), 1,
      anon_sym_SEMI,
    STATE(301), 1,
      sym_field_definition_values,
  [4179] = 1,
    ACTIONS(471), 3,
      anon_sym_RBRACE,
      anon_sym_api_export,
      anon_sym_files,
  [4185] = 1,
    ACTIONS(705), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4191] = 3,
    ACTIONS(707), 1,
      anon_sym_COMMA,
    ACTIONS(710), 1,
      anon_sym_RPAREN,
    STATE(213), 1,
      aux_sym_file_reference_attributes_repeat1,
  [4201] = 1,
    ACTIONS(712), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4207] = 3,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    STATE(357), 1,
      sym_resource_reference_literal,
  [4217] = 3,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(714), 1,
      anon_sym_RPAREN,
    STATE(409), 1,
      sym_string_literal,
  [4227] = 3,
    ACTIONS(651), 1,
      anon_sym_COMMA,
    ACTIONS(716), 1,
      anon_sym_RPAREN,
    STATE(172), 1,
      aux_sym_decorator_repeat1,
  [4237] = 3,
    ACTIONS(718), 1,
      anon_sym_COMMA,
    ACTIONS(721), 1,
      anon_sym_RPAREN,
    STATE(218), 1,
      aux_sym_type_constructor_repeat1,
  [4247] = 3,
    ACTIONS(269), 1,
      anon_sym_COMMA,
    ACTIONS(723), 1,
      anon_sym_SEMI,
    STATE(160), 1,
      aux_sym_resource_declaration_field_value_repeat1,
  [4257] = 2,
    ACTIONS(727), 1,
      anon_sym_EQ,
    ACTIONS(725), 2,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [4265] = 3,
    ACTIONS(729), 1,
      anon_sym_DQUOTE,
    ACTIONS(731), 1,
      aux_sym_string_literal_token1,
    STATE(221), 1,
      aux_sym_string_literal_repeat1,
  [4275] = 3,
    ACTIONS(734), 1,
      anon_sym_DQUOTE,
    ACTIONS(736), 1,
      aux_sym_string_literal_token1,
    STATE(195), 1,
      aux_sym_string_literal_repeat1,
  [4285] = 1,
    ACTIONS(738), 3,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACK,
  [4291] = 2,
    ACTIONS(740), 1,
      anon_sym_LPAREN,
    ACTIONS(384), 2,
      aux_sym_decorator_token1,
      sym_identifier_literal,
  [4299] = 3,
    ACTIONS(455), 1,
      anon_sym_LBRACK,
    ACTIONS(742), 1,
      anon_sym_SEMI,
    STATE(390), 1,
      sym_field_definition_value_symbol_list,
  [4309] = 1,
    ACTIONS(744), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4315] = 3,
    ACTIONS(455), 1,
      anon_sym_LBRACK,
    ACTIONS(746), 1,
      anon_sym_SEMI,
    STATE(388), 1,
      sym_field_definition_value_symbol_list,
  [4325] = 1,
    ACTIONS(748), 3,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACK,
  [4331] = 1,
    ACTIONS(750), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4337] = 3,
    ACTIONS(752), 1,
      anon_sym_COMMA,
    ACTIONS(754), 1,
      anon_sym_RPAREN,
    STATE(232), 1,
      aux_sym_file_reference_attributes_repeat1,
  [4347] = 1,
    ACTIONS(624), 3,
      anon_sym_GT,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [4353] = 3,
    ACTIONS(752), 1,
      anon_sym_COMMA,
    ACTIONS(756), 1,
      anon_sym_RPAREN,
    STATE(213), 1,
      aux_sym_file_reference_attributes_repeat1,
  [4363] = 3,
    ACTIONS(455), 1,
      anon_sym_LBRACK,
    ACTIONS(758), 1,
      anon_sym_SEMI,
    STATE(362), 1,
      sym_field_definition_value_symbol_list,
  [4373] = 1,
    ACTIONS(760), 3,
      anon_sym_SEMI,
      anon_sym_EQ,
      anon_sym_LBRACK,
  [4379] = 3,
    ACTIONS(651), 1,
      anon_sym_COMMA,
    ACTIONS(762), 1,
      anon_sym_RPAREN,
    STATE(239), 1,
      aux_sym_decorator_repeat1,
  [4389] = 3,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    ACTIONS(764), 1,
      sym_identifier_literal,
    STATE(407), 1,
      sym_string_literal,
  [4399] = 3,
    ACTIONS(699), 1,
      anon_sym_COMMA,
    ACTIONS(766), 1,
      anon_sym_RPAREN,
    STATE(218), 1,
      aux_sym_type_constructor_repeat1,
  [4409] = 1,
    ACTIONS(768), 3,
      aux_sym_decorator_token1,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4415] = 3,
    ACTIONS(651), 1,
      anon_sym_COMMA,
    ACTIONS(770), 1,
      anon_sym_RPAREN,
    STATE(172), 1,
      aux_sym_decorator_repeat1,
  [4425] = 3,
    ACTIONS(148), 1,
      anon_sym_POUNDauto,
    ACTIONS(150), 1,
      anon_sym_POUND,
    STATE(316), 1,
      sym_resource_reference_literal,
  [4435] = 2,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    STATE(415), 1,
      sym_string_literal,
  [4442] = 2,
    ACTIONS(772), 1,
      anon_sym_RPAREN,
    ACTIONS(774), 1,
      sym_identifier_literal,
  [4449] = 1,
    ACTIONS(776), 2,
      anon_sym_requires,
      anon_sym_set,
  [4454] = 2,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    STATE(401), 1,
      sym_string_literal,
  [4461] = 2,
    ACTIONS(778), 1,
      sym_identifier_literal,
    ACTIONS(780), 1,
      sym_integer_literal,
  [4468] = 2,
    ACTIONS(782), 1,
      sym_identifier_literal,
    ACTIONS(784), 1,
      sym_integer_literal,
  [4475] = 2,
    ACTIONS(786), 1,
      sym_identifier_literal,
    STATE(255), 1,
      sym_type_identifier,
  [4482] = 1,
    ACTIONS(398), 2,
      aux_sym_decorator_token1,
      sym_identifier_literal,
  [4487] = 2,
    ACTIONS(788), 1,
      anon_sym_LT,
    STATE(398), 1,
      sym_component_configuration,
  [4494] = 2,
    ACTIONS(790), 1,
      anon_sym_LBRACE,
    STATE(137), 1,
      sym_type_constructor_body,
  [4501] = 1,
    ACTIONS(400), 2,
      aux_sym_decorator_token1,
      sym_identifier_literal,
  [4506] = 1,
    ACTIONS(792), 2,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [4511] = 2,
    ACTIONS(620), 1,
      anon_sym_LBRACE,
    STATE(376), 1,
      sym_resource_declaration_values,
  [4518] = 2,
    ACTIONS(794), 1,
      anon_sym_LBRACE,
    ACTIONS(796), 1,
      anon_sym_DOT,
  [4525] = 2,
    ACTIONS(798), 1,
      anon_sym_LBRACE,
    STATE(385), 1,
      sym_resource_declaration_body,
  [4532] = 2,
    ACTIONS(786), 1,
      sym_identifier_literal,
    STATE(266), 1,
      sym_type_identifier,
  [4539] = 2,
    ACTIONS(788), 1,
      anon_sym_LT,
    STATE(377), 1,
      sym_component_configuration,
  [4546] = 1,
    ACTIONS(721), 2,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [4551] = 2,
    ACTIONS(790), 1,
      anon_sym_LBRACE,
    STATE(141), 1,
      sym_type_constructor_body,
  [4558] = 1,
    ACTIONS(800), 2,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [4563] = 1,
    ACTIONS(802), 2,
      anon_sym_RBRACE,
      sym_identifier_literal,
  [4568] = 2,
    ACTIONS(804), 1,
      sym_identifier_literal,
    ACTIONS(806), 1,
      sym_integer_literal,
  [4575] = 2,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    STATE(270), 1,
      sym_string_literal,
  [4582] = 2,
    ACTIONS(808), 1,
      anon_sym_COMMA,
    ACTIONS(810), 1,
      anon_sym_DOT,
  [4589] = 2,
    ACTIONS(812), 1,
      anon_sym_LBRACE,
    STATE(364), 1,
      sym_component_body,
  [4596] = 2,
    ACTIONS(798), 1,
      anon_sym_LBRACE,
    STATE(296), 1,
      sym_resource_declaration_body,
  [4603] = 2,
    ACTIONS(790), 1,
      anon_sym_LBRACE,
    STATE(149), 1,
      sym_type_constructor_body,
  [4610] = 1,
    ACTIONS(710), 2,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [4615] = 2,
    ACTIONS(814), 1,
      anon_sym_GT,
    ACTIONS(816), 1,
      anon_sym_COMMA,
  [4622] = 2,
    ACTIONS(818), 1,
      anon_sym_LBRACE,
    STATE(352), 1,
      sym_type_definition_body,
  [4629] = 2,
    ACTIONS(620), 1,
      anon_sym_LBRACE,
    STATE(306), 1,
      sym_resource_declaration_values,
  [4636] = 2,
    ACTIONS(231), 1,
      anon_sym_LPAREN,
    STATE(281), 1,
      sym_argument_list,
  [4643] = 2,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    STATE(283), 1,
      sym_string_literal,
  [4650] = 2,
    ACTIONS(812), 1,
      anon_sym_LBRACE,
    STATE(334), 1,
      sym_component_body,
  [4657] = 2,
    ACTIONS(820), 1,
      sym_identifier_literal,
    ACTIONS(822), 1,
      sym_integer_literal,
  [4664] = 2,
    ACTIONS(824), 1,
      sym_identifier_literal,
    STATE(268), 1,
      sym_key_value_pair,
  [4671] = 2,
    ACTIONS(826), 1,
      anon_sym_LBRACE,
    STATE(310), 1,
      sym_scene_element_body,
  [4678] = 2,
    ACTIONS(828), 1,
      sym_identifier_literal,
    ACTIONS(830), 1,
      sym_integer_literal,
  [4685] = 1,
    ACTIONS(832), 2,
      anon_sym_SEMI,
      anon_sym_LBRACE,
  [4690] = 2,
    ACTIONS(620), 1,
      anon_sym_LBRACE,
    STATE(320), 1,
      sym_resource_declaration_values,
  [4697] = 2,
    ACTIONS(620), 1,
      anon_sym_LBRACE,
    STATE(318), 1,
      sym_resource_declaration_values,
  [4704] = 2,
    ACTIONS(834), 1,
      anon_sym_LBRACE,
    STATE(313), 1,
      sym_scene_body,
  [4711] = 2,
    ACTIONS(818), 1,
      anon_sym_LBRACE,
    STATE(312), 1,
      sym_type_definition_body,
  [4718] = 2,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    STATE(336), 1,
      sym_string_literal,
  [4725] = 2,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    STATE(410), 1,
      sym_string_literal,
  [4732] = 2,
    ACTIONS(824), 1,
      sym_identifier_literal,
    STATE(230), 1,
      sym_key_value_pair,
  [4739] = 1,
    ACTIONS(836), 2,
      anon_sym_RBRACE,
      anon_sym_DQUOTE,
  [4744] = 2,
    ACTIONS(838), 1,
      sym_identifier_literal,
    ACTIONS(840), 1,
      sym_integer_literal,
  [4751] = 1,
    ACTIONS(842), 2,
      anon_sym_RBRACE,
      anon_sym_DQUOTE,
  [4756] = 2,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    STATE(291), 1,
      sym_string_literal,
  [4763] = 1,
    ACTIONS(844), 2,
      anon_sym_LPAREN,
      anon_sym_SEMI,
  [4768] = 2,
    ACTIONS(846), 1,
      anon_sym_GT,
    ACTIONS(848), 1,
      anon_sym_COMMA,
  [4775] = 2,
    ACTIONS(152), 1,
      anon_sym_DQUOTE,
    STATE(383), 1,
      sym_string_literal,
  [4782] = 2,
    ACTIONS(834), 1,
      anon_sym_LBRACE,
    STATE(359), 1,
      sym_scene_body,
  [4789] = 1,
    ACTIONS(850), 1,
      sym_identifier_literal,
  [4793] = 1,
    ACTIONS(852), 1,
      anon_sym_SEMI,
  [4797] = 1,
    ACTIONS(854), 1,
      anon_sym_SEMI,
  [4801] = 1,
    ACTIONS(856), 1,
      anon_sym_SEMI,
  [4805] = 1,
    ACTIONS(858), 1,
      anon_sym_SEMI,
  [4809] = 1,
    ACTIONS(860), 1,
      anon_sym_EQ,
  [4813] = 1,
    ACTIONS(862), 1,
      anon_sym_SEMI,
  [4817] = 1,
    ACTIONS(864), 1,
      anon_sym_SEMI,
  [4821] = 1,
    ACTIONS(866), 1,
      anon_sym_LPAREN,
  [4825] = 1,
    ACTIONS(868), 1,
      anon_sym_SEMI,
  [4829] = 1,
    ACTIONS(287), 1,
      anon_sym_RPAREN,
  [4833] = 1,
    ACTIONS(870), 1,
      anon_sym_SEMI,
  [4837] = 1,
    ACTIONS(872), 1,
      anon_sym_SEMI,
  [4841] = 1,
    ACTIONS(874), 1,
      anon_sym_SEMI,
  [4845] = 1,
    ACTIONS(876), 1,
      anon_sym_SEMI,
  [4849] = 1,
    ACTIONS(878), 1,
      anon_sym_SEMI,
  [4853] = 1,
    ACTIONS(880), 1,
      anon_sym_EQ,
  [4857] = 1,
    ACTIONS(882), 1,
      anon_sym_SEMI,
  [4861] = 1,
    ACTIONS(884), 1,
      anon_sym_SEMI,
  [4865] = 1,
    ACTIONS(886), 1,
      anon_sym_SEMI,
  [4869] = 1,
    ACTIONS(888), 1,
      sym_identifier_literal,
  [4873] = 1,
    ACTIONS(890), 1,
      anon_sym_as,
  [4877] = 1,
    ACTIONS(580), 1,
      anon_sym_SEMI,
  [4881] = 1,
    ACTIONS(892), 1,
      anon_sym_SEMI,
  [4885] = 1,
    ACTIONS(894), 1,
      anon_sym_SEMI,
  [4889] = 1,
    ACTIONS(896), 1,
      anon_sym_SEMI,
  [4893] = 1,
    ACTIONS(846), 1,
      anon_sym_GT,
  [4897] = 1,
    ACTIONS(898), 1,
      anon_sym_EQ,
  [4901] = 1,
    ACTIONS(900), 1,
      anon_sym_SEMI,
  [4905] = 1,
    ACTIONS(902), 1,
      anon_sym_SEMI,
  [4909] = 1,
    ACTIONS(904), 1,
      anon_sym_LPAREN,
  [4913] = 1,
    ACTIONS(906), 1,
      anon_sym_LPAREN,
  [4917] = 1,
    ACTIONS(908), 1,
      anon_sym_LBRACE,
  [4921] = 1,
    ACTIONS(910), 1,
      anon_sym_SEMI,
  [4925] = 1,
    ACTIONS(912), 1,
      anon_sym_EQ,
  [4929] = 1,
    ACTIONS(914), 1,
      anon_sym_SEMI,
  [4933] = 1,
    ACTIONS(916), 1,
      anon_sym_DOT,
  [4937] = 1,
    ACTIONS(918), 1,
      anon_sym_SEMI,
  [4941] = 1,
    ACTIONS(920), 1,
      anon_sym_SEMI,
  [4945] = 1,
    ACTIONS(922), 1,
      anon_sym_SEMI,
  [4949] = 1,
    ACTIONS(924), 1,
      anon_sym_LBRACE,
  [4953] = 1,
    ACTIONS(926), 1,
      anon_sym_RPAREN,
  [4957] = 1,
    ACTIONS(928), 1,
      anon_sym_SEMI,
  [4961] = 1,
    ACTIONS(930), 1,
      anon_sym_SEMI,
  [4965] = 1,
    ACTIONS(932), 1,
      anon_sym_EQ,
  [4969] = 1,
    ACTIONS(934), 1,
      anon_sym_LPAREN,
  [4973] = 1,
    ACTIONS(936), 1,
      anon_sym_LBRACE,
  [4977] = 1,
    ACTIONS(938), 1,
      anon_sym_SEMI,
  [4981] = 1,
    ACTIONS(940), 1,
      anon_sym_COMMA,
  [4985] = 1,
    ACTIONS(942), 1,
      anon_sym_SEMI,
  [4989] = 1,
    ACTIONS(944), 1,
      anon_sym_SEMI,
  [4993] = 1,
    ACTIONS(946), 1,
      anon_sym_SEMI,
  [4997] = 1,
    ACTIONS(948), 1,
      anon_sym_EQ,
  [5001] = 1,
    ACTIONS(566), 1,
      anon_sym_SEMI,
  [5005] = 1,
    ACTIONS(950), 1,
      anon_sym_LPAREN,
  [5009] = 1,
    ACTIONS(952), 1,
      sym_identifier_literal,
  [5013] = 1,
    ACTIONS(954), 1,
      anon_sym_SEMI,
  [5017] = 1,
    ACTIONS(956), 1,
      anon_sym_SEMI,
  [5021] = 1,
    ACTIONS(958), 1,
      sym_identifier_literal,
  [5025] = 1,
    ACTIONS(960), 1,
      anon_sym_SEMI,
  [5029] = 1,
    ACTIONS(325), 1,
      anon_sym_RPAREN,
  [5033] = 1,
    ACTIONS(962), 1,
      anon_sym_SEMI,
  [5037] = 1,
    ACTIONS(964), 1,
      anon_sym_GT,
  [5041] = 1,
    ACTIONS(966), 1,
      anon_sym_SEMI,
  [5045] = 1,
    ACTIONS(968), 1,
      anon_sym_SEMI,
  [5049] = 1,
    ACTIONS(970), 1,
      anon_sym_SEMI,
  [5053] = 1,
    ACTIONS(972), 1,
      sym_identifier_literal,
  [5057] = 1,
    ACTIONS(974), 1,
      anon_sym_SEMI,
  [5061] = 1,
    ACTIONS(976), 1,
      sym_identifier_literal,
  [5065] = 1,
    ACTIONS(978), 1,
      anon_sym_SEMI,
  [5069] = 1,
    ACTIONS(980), 1,
      sym_identifier_literal,
  [5073] = 1,
    ACTIONS(982), 1,
      anon_sym_EQ,
  [5077] = 1,
    ACTIONS(984), 1,
      anon_sym_SEMI,
  [5081] = 1,
    ACTIONS(986), 1,
      anon_sym_LPAREN,
  [5085] = 1,
    ACTIONS(988), 1,
      anon_sym_LT,
  [5089] = 1,
    ACTIONS(990), 1,
      anon_sym_SEMI,
  [5093] = 1,
    ACTIONS(992), 1,
      anon_sym_LBRACE,
  [5097] = 1,
    ACTIONS(994), 1,
      anon_sym_SEMI,
  [5101] = 1,
    ACTIONS(996), 1,
      anon_sym_DOT,
  [5105] = 1,
    ACTIONS(998), 1,
      anon_sym_SEMI,
  [5109] = 1,
    ACTIONS(814), 1,
      anon_sym_GT,
  [5113] = 1,
    ACTIONS(1000), 1,
      anon_sym_SEMI,
  [5117] = 1,
    ACTIONS(1002), 1,
      sym_identifier_literal,
  [5121] = 1,
    ACTIONS(1004), 1,
      anon_sym_SEMI,
  [5125] = 1,
    ACTIONS(1006), 1,
      anon_sym_SEMI,
  [5129] = 1,
    ACTIONS(1008), 1,
      sym_identifier_literal,
  [5133] = 1,
    ACTIONS(1010), 1,
      anon_sym_COLON,
  [5137] = 1,
    ACTIONS(1012), 1,
      anon_sym_GT,
  [5141] = 1,
    ACTIONS(1014), 1,
      anon_sym_RPAREN,
  [5145] = 1,
    ACTIONS(1016), 1,
      anon_sym_SEMI,
  [5149] = 1,
    ACTIONS(1018), 1,
      anon_sym_SEMI,
  [5153] = 1,
    ACTIONS(1020), 1,
      sym_identifier_literal,
  [5157] = 1,
    ACTIONS(279), 1,
      anon_sym_SEMI,
  [5161] = 1,
    ACTIONS(1022), 1,
      anon_sym_SEMI,
  [5165] = 1,
    ACTIONS(1024), 1,
      anon_sym_SEMI,
  [5169] = 1,
    ACTIONS(1026), 1,
      anon_sym_SEMI,
  [5173] = 1,
    ACTIONS(1028), 1,
      anon_sym_LT,
  [5177] = 1,
    ACTIONS(1030), 1,
      anon_sym_LT,
  [5181] = 1,
    ACTIONS(1032), 1,
      sym_identifier_literal,
  [5185] = 1,
    ACTIONS(1034), 1,
      sym_identifier_literal,
  [5189] = 1,
    ACTIONS(1036), 1,
      sym_integer_literal,
  [5193] = 1,
    ACTIONS(1038), 1,
      anon_sym_DOT,
  [5197] = 1,
    ACTIONS(1040), 1,
      sym_identifier_literal,
  [5201] = 1,
    ACTIONS(1042), 1,
      sym_identifier_literal,
  [5205] = 1,
    ACTIONS(1044), 1,
      sym_identifier_literal,
  [5209] = 1,
    ACTIONS(1046), 1,
      anon_sym_GT,
  [5213] = 1,
    ACTIONS(1048), 1,
      anon_sym_RPAREN,
  [5217] = 1,
    ACTIONS(1050), 1,
      anon_sym_COLON,
  [5221] = 1,
    ACTIONS(1052), 1,
      anon_sym_SEMI,
  [5225] = 1,
    ACTIONS(1054), 1,
      anon_sym_GT,
  [5229] = 1,
    ACTIONS(1056), 1,
      anon_sym_EQ,
  [5233] = 1,
    ACTIONS(1058), 1,
      anon_sym_EQ,
  [5237] = 1,
    ACTIONS(1060), 1,
      anon_sym_SEMI,
  [5241] = 1,
    ACTIONS(1062), 1,
      anon_sym_SEMI,
  [5245] = 1,
    ACTIONS(1064), 1,
      anon_sym_RPAREN,
  [5249] = 1,
    ACTIONS(1066), 1,
      anon_sym_SEMI,
  [5253] = 1,
    ACTIONS(1068), 1,
      anon_sym_SEMI,
  [5257] = 1,
    ACTIONS(1070), 1,
      ts_builtin_sym_end,
  [5261] = 1,
    ACTIONS(1072), 1,
      anon_sym_LT,
  [5265] = 1,
    ACTIONS(275), 1,
      anon_sym_SEMI,
  [5269] = 1,
    ACTIONS(1074), 1,
      anon_sym_RPAREN,
  [5273] = 1,
    ACTIONS(1076), 1,
      sym_identifier_literal,
  [5277] = 1,
    ACTIONS(1078), 1,
      anon_sym_SEMI,
  [5281] = 1,
    ACTIONS(1080), 1,
      sym_identifier_literal,
  [5285] = 1,
    ACTIONS(1082), 1,
      sym_identifier_literal,
  [5289] = 1,
    ACTIONS(1084), 1,
      anon_sym_SEMI,
  [5293] = 1,
    ACTIONS(1086), 1,
      sym_variable,
  [5297] = 1,
    ACTIONS(1088), 1,
      sym_identifier_literal,
  [5301] = 1,
    ACTIONS(1090), 1,
      sym_integer_literal,
  [5305] = 1,
    ACTIONS(1092), 1,
      anon_sym_SEMI,
  [5309] = 1,
    ACTIONS(1094), 1,
      anon_sym_DOT,
  [5313] = 1,
    ACTIONS(1096), 1,
      anon_sym_DOT,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(2)] = 0,
  [SMALL_STATE(3)] = 68,
  [SMALL_STATE(4)] = 136,
  [SMALL_STATE(5)] = 177,
  [SMALL_STATE(6)] = 218,
  [SMALL_STATE(7)] = 259,
  [SMALL_STATE(8)] = 297,
  [SMALL_STATE(9)] = 335,
  [SMALL_STATE(10)] = 373,
  [SMALL_STATE(11)] = 403,
  [SMALL_STATE(12)] = 430,
  [SMALL_STATE(13)] = 457,
  [SMALL_STATE(14)] = 484,
  [SMALL_STATE(15)] = 509,
  [SMALL_STATE(16)] = 534,
  [SMALL_STATE(17)] = 559,
  [SMALL_STATE(18)] = 584,
  [SMALL_STATE(19)] = 631,
  [SMALL_STATE(20)] = 678,
  [SMALL_STATE(21)] = 703,
  [SMALL_STATE(22)] = 728,
  [SMALL_STATE(23)] = 753,
  [SMALL_STATE(24)] = 778,
  [SMALL_STATE(25)] = 803,
  [SMALL_STATE(26)] = 849,
  [SMALL_STATE(27)] = 895,
  [SMALL_STATE(28)] = 941,
  [SMALL_STATE(29)] = 981,
  [SMALL_STATE(30)] = 1003,
  [SMALL_STATE(31)] = 1025,
  [SMALL_STATE(32)] = 1065,
  [SMALL_STATE(33)] = 1087,
  [SMALL_STATE(34)] = 1109,
  [SMALL_STATE(35)] = 1146,
  [SMALL_STATE(36)] = 1183,
  [SMALL_STATE(37)] = 1220,
  [SMALL_STATE(38)] = 1257,
  [SMALL_STATE(39)] = 1294,
  [SMALL_STATE(40)] = 1331,
  [SMALL_STATE(41)] = 1368,
  [SMALL_STATE(42)] = 1405,
  [SMALL_STATE(43)] = 1442,
  [SMALL_STATE(44)] = 1479,
  [SMALL_STATE(45)] = 1516,
  [SMALL_STATE(46)] = 1553,
  [SMALL_STATE(47)] = 1590,
  [SMALL_STATE(48)] = 1615,
  [SMALL_STATE(49)] = 1652,
  [SMALL_STATE(50)] = 1689,
  [SMALL_STATE(51)] = 1726,
  [SMALL_STATE(52)] = 1763,
  [SMALL_STATE(53)] = 1800,
  [SMALL_STATE(54)] = 1820,
  [SMALL_STATE(55)] = 1840,
  [SMALL_STATE(56)] = 1859,
  [SMALL_STATE(57)] = 1884,
  [SMALL_STATE(58)] = 1903,
  [SMALL_STATE(59)] = 1922,
  [SMALL_STATE(60)] = 1945,
  [SMALL_STATE(61)] = 1974,
  [SMALL_STATE(62)] = 2001,
  [SMALL_STATE(63)] = 2020,
  [SMALL_STATE(64)] = 2039,
  [SMALL_STATE(65)] = 2060,
  [SMALL_STATE(66)] = 2091,
  [SMALL_STATE(67)] = 2122,
  [SMALL_STATE(68)] = 2153,
  [SMALL_STATE(69)] = 2184,
  [SMALL_STATE(70)] = 2211,
  [SMALL_STATE(71)] = 2242,
  [SMALL_STATE(72)] = 2270,
  [SMALL_STATE(73)] = 2298,
  [SMALL_STATE(74)] = 2326,
  [SMALL_STATE(75)] = 2352,
  [SMALL_STATE(76)] = 2380,
  [SMALL_STATE(77)] = 2408,
  [SMALL_STATE(78)] = 2436,
  [SMALL_STATE(79)] = 2462,
  [SMALL_STATE(80)] = 2488,
  [SMALL_STATE(81)] = 2514,
  [SMALL_STATE(82)] = 2542,
  [SMALL_STATE(83)] = 2558,
  [SMALL_STATE(84)] = 2584,
  [SMALL_STATE(85)] = 2612,
  [SMALL_STATE(86)] = 2640,
  [SMALL_STATE(87)] = 2656,
  [SMALL_STATE(88)] = 2684,
  [SMALL_STATE(89)] = 2710,
  [SMALL_STATE(90)] = 2736,
  [SMALL_STATE(91)] = 2754,
  [SMALL_STATE(92)] = 2779,
  [SMALL_STATE(93)] = 2804,
  [SMALL_STATE(94)] = 2829,
  [SMALL_STATE(95)] = 2854,
  [SMALL_STATE(96)] = 2869,
  [SMALL_STATE(97)] = 2894,
  [SMALL_STATE(98)] = 2919,
  [SMALL_STATE(99)] = 2944,
  [SMALL_STATE(100)] = 2969,
  [SMALL_STATE(101)] = 2994,
  [SMALL_STATE(102)] = 3006,
  [SMALL_STATE(103)] = 3028,
  [SMALL_STATE(104)] = 3040,
  [SMALL_STATE(105)] = 3051,
  [SMALL_STATE(106)] = 3067,
  [SMALL_STATE(107)] = 3087,
  [SMALL_STATE(108)] = 3105,
  [SMALL_STATE(109)] = 3123,
  [SMALL_STATE(110)] = 3141,
  [SMALL_STATE(111)] = 3157,
  [SMALL_STATE(112)] = 3173,
  [SMALL_STATE(113)] = 3190,
  [SMALL_STATE(114)] = 3207,
  [SMALL_STATE(115)] = 3224,
  [SMALL_STATE(116)] = 3243,
  [SMALL_STATE(117)] = 3252,
  [SMALL_STATE(118)] = 3269,
  [SMALL_STATE(119)] = 3286,
  [SMALL_STATE(120)] = 3303,
  [SMALL_STATE(121)] = 3320,
  [SMALL_STATE(122)] = 3337,
  [SMALL_STATE(123)] = 3354,
  [SMALL_STATE(124)] = 3363,
  [SMALL_STATE(125)] = 3380,
  [SMALL_STATE(126)] = 3397,
  [SMALL_STATE(127)] = 3416,
  [SMALL_STATE(128)] = 3433,
  [SMALL_STATE(129)] = 3441,
  [SMALL_STATE(130)] = 3449,
  [SMALL_STATE(131)] = 3457,
  [SMALL_STATE(132)] = 3465,
  [SMALL_STATE(133)] = 3473,
  [SMALL_STATE(134)] = 3481,
  [SMALL_STATE(135)] = 3489,
  [SMALL_STATE(136)] = 3497,
  [SMALL_STATE(137)] = 3505,
  [SMALL_STATE(138)] = 3513,
  [SMALL_STATE(139)] = 3521,
  [SMALL_STATE(140)] = 3529,
  [SMALL_STATE(141)] = 3537,
  [SMALL_STATE(142)] = 3545,
  [SMALL_STATE(143)] = 3553,
  [SMALL_STATE(144)] = 3561,
  [SMALL_STATE(145)] = 3569,
  [SMALL_STATE(146)] = 3585,
  [SMALL_STATE(147)] = 3593,
  [SMALL_STATE(148)] = 3605,
  [SMALL_STATE(149)] = 3613,
  [SMALL_STATE(150)] = 3621,
  [SMALL_STATE(151)] = 3629,
  [SMALL_STATE(152)] = 3645,
  [SMALL_STATE(153)] = 3657,
  [SMALL_STATE(154)] = 3670,
  [SMALL_STATE(155)] = 3681,
  [SMALL_STATE(156)] = 3692,
  [SMALL_STATE(157)] = 3703,
  [SMALL_STATE(158)] = 3714,
  [SMALL_STATE(159)] = 3727,
  [SMALL_STATE(160)] = 3738,
  [SMALL_STATE(161)] = 3749,
  [SMALL_STATE(162)] = 3758,
  [SMALL_STATE(163)] = 3767,
  [SMALL_STATE(164)] = 3776,
  [SMALL_STATE(165)] = 3785,
  [SMALL_STATE(166)] = 3796,
  [SMALL_STATE(167)] = 3807,
  [SMALL_STATE(168)] = 3818,
  [SMALL_STATE(169)] = 3829,
  [SMALL_STATE(170)] = 3842,
  [SMALL_STATE(171)] = 3853,
  [SMALL_STATE(172)] = 3860,
  [SMALL_STATE(173)] = 3871,
  [SMALL_STATE(174)] = 3878,
  [SMALL_STATE(175)] = 3885,
  [SMALL_STATE(176)] = 3896,
  [SMALL_STATE(177)] = 3903,
  [SMALL_STATE(178)] = 3914,
  [SMALL_STATE(179)] = 3925,
  [SMALL_STATE(180)] = 3931,
  [SMALL_STATE(181)] = 3937,
  [SMALL_STATE(182)] = 3943,
  [SMALL_STATE(183)] = 3953,
  [SMALL_STATE(184)] = 3959,
  [SMALL_STATE(185)] = 3969,
  [SMALL_STATE(186)] = 3979,
  [SMALL_STATE(187)] = 3989,
  [SMALL_STATE(188)] = 3999,
  [SMALL_STATE(189)] = 4009,
  [SMALL_STATE(190)] = 4019,
  [SMALL_STATE(191)] = 4029,
  [SMALL_STATE(192)] = 4035,
  [SMALL_STATE(193)] = 4041,
  [SMALL_STATE(194)] = 4051,
  [SMALL_STATE(195)] = 4057,
  [SMALL_STATE(196)] = 4067,
  [SMALL_STATE(197)] = 4073,
  [SMALL_STATE(198)] = 4079,
  [SMALL_STATE(199)] = 4087,
  [SMALL_STATE(200)] = 4093,
  [SMALL_STATE(201)] = 4099,
  [SMALL_STATE(202)] = 4105,
  [SMALL_STATE(203)] = 4115,
  [SMALL_STATE(204)] = 4125,
  [SMALL_STATE(205)] = 4135,
  [SMALL_STATE(206)] = 4141,
  [SMALL_STATE(207)] = 4147,
  [SMALL_STATE(208)] = 4153,
  [SMALL_STATE(209)] = 4159,
  [SMALL_STATE(210)] = 4169,
  [SMALL_STATE(211)] = 4179,
  [SMALL_STATE(212)] = 4185,
  [SMALL_STATE(213)] = 4191,
  [SMALL_STATE(214)] = 4201,
  [SMALL_STATE(215)] = 4207,
  [SMALL_STATE(216)] = 4217,
  [SMALL_STATE(217)] = 4227,
  [SMALL_STATE(218)] = 4237,
  [SMALL_STATE(219)] = 4247,
  [SMALL_STATE(220)] = 4257,
  [SMALL_STATE(221)] = 4265,
  [SMALL_STATE(222)] = 4275,
  [SMALL_STATE(223)] = 4285,
  [SMALL_STATE(224)] = 4291,
  [SMALL_STATE(225)] = 4299,
  [SMALL_STATE(226)] = 4309,
  [SMALL_STATE(227)] = 4315,
  [SMALL_STATE(228)] = 4325,
  [SMALL_STATE(229)] = 4331,
  [SMALL_STATE(230)] = 4337,
  [SMALL_STATE(231)] = 4347,
  [SMALL_STATE(232)] = 4353,
  [SMALL_STATE(233)] = 4363,
  [SMALL_STATE(234)] = 4373,
  [SMALL_STATE(235)] = 4379,
  [SMALL_STATE(236)] = 4389,
  [SMALL_STATE(237)] = 4399,
  [SMALL_STATE(238)] = 4409,
  [SMALL_STATE(239)] = 4415,
  [SMALL_STATE(240)] = 4425,
  [SMALL_STATE(241)] = 4435,
  [SMALL_STATE(242)] = 4442,
  [SMALL_STATE(243)] = 4449,
  [SMALL_STATE(244)] = 4454,
  [SMALL_STATE(245)] = 4461,
  [SMALL_STATE(246)] = 4468,
  [SMALL_STATE(247)] = 4475,
  [SMALL_STATE(248)] = 4482,
  [SMALL_STATE(249)] = 4487,
  [SMALL_STATE(250)] = 4494,
  [SMALL_STATE(251)] = 4501,
  [SMALL_STATE(252)] = 4506,
  [SMALL_STATE(253)] = 4511,
  [SMALL_STATE(254)] = 4518,
  [SMALL_STATE(255)] = 4525,
  [SMALL_STATE(256)] = 4532,
  [SMALL_STATE(257)] = 4539,
  [SMALL_STATE(258)] = 4546,
  [SMALL_STATE(259)] = 4551,
  [SMALL_STATE(260)] = 4558,
  [SMALL_STATE(261)] = 4563,
  [SMALL_STATE(262)] = 4568,
  [SMALL_STATE(263)] = 4575,
  [SMALL_STATE(264)] = 4582,
  [SMALL_STATE(265)] = 4589,
  [SMALL_STATE(266)] = 4596,
  [SMALL_STATE(267)] = 4603,
  [SMALL_STATE(268)] = 4610,
  [SMALL_STATE(269)] = 4615,
  [SMALL_STATE(270)] = 4622,
  [SMALL_STATE(271)] = 4629,
  [SMALL_STATE(272)] = 4636,
  [SMALL_STATE(273)] = 4643,
  [SMALL_STATE(274)] = 4650,
  [SMALL_STATE(275)] = 4657,
  [SMALL_STATE(276)] = 4664,
  [SMALL_STATE(277)] = 4671,
  [SMALL_STATE(278)] = 4678,
  [SMALL_STATE(279)] = 4685,
  [SMALL_STATE(280)] = 4690,
  [SMALL_STATE(281)] = 4697,
  [SMALL_STATE(282)] = 4704,
  [SMALL_STATE(283)] = 4711,
  [SMALL_STATE(284)] = 4718,
  [SMALL_STATE(285)] = 4725,
  [SMALL_STATE(286)] = 4732,
  [SMALL_STATE(287)] = 4739,
  [SMALL_STATE(288)] = 4744,
  [SMALL_STATE(289)] = 4751,
  [SMALL_STATE(290)] = 4756,
  [SMALL_STATE(291)] = 4763,
  [SMALL_STATE(292)] = 4768,
  [SMALL_STATE(293)] = 4775,
  [SMALL_STATE(294)] = 4782,
  [SMALL_STATE(295)] = 4789,
  [SMALL_STATE(296)] = 4793,
  [SMALL_STATE(297)] = 4797,
  [SMALL_STATE(298)] = 4801,
  [SMALL_STATE(299)] = 4805,
  [SMALL_STATE(300)] = 4809,
  [SMALL_STATE(301)] = 4813,
  [SMALL_STATE(302)] = 4817,
  [SMALL_STATE(303)] = 4821,
  [SMALL_STATE(304)] = 4825,
  [SMALL_STATE(305)] = 4829,
  [SMALL_STATE(306)] = 4833,
  [SMALL_STATE(307)] = 4837,
  [SMALL_STATE(308)] = 4841,
  [SMALL_STATE(309)] = 4845,
  [SMALL_STATE(310)] = 4849,
  [SMALL_STATE(311)] = 4853,
  [SMALL_STATE(312)] = 4857,
  [SMALL_STATE(313)] = 4861,
  [SMALL_STATE(314)] = 4865,
  [SMALL_STATE(315)] = 4869,
  [SMALL_STATE(316)] = 4873,
  [SMALL_STATE(317)] = 4877,
  [SMALL_STATE(318)] = 4881,
  [SMALL_STATE(319)] = 4885,
  [SMALL_STATE(320)] = 4889,
  [SMALL_STATE(321)] = 4893,
  [SMALL_STATE(322)] = 4897,
  [SMALL_STATE(323)] = 4901,
  [SMALL_STATE(324)] = 4905,
  [SMALL_STATE(325)] = 4909,
  [SMALL_STATE(326)] = 4913,
  [SMALL_STATE(327)] = 4917,
  [SMALL_STATE(328)] = 4921,
  [SMALL_STATE(329)] = 4925,
  [SMALL_STATE(330)] = 4929,
  [SMALL_STATE(331)] = 4933,
  [SMALL_STATE(332)] = 4937,
  [SMALL_STATE(333)] = 4941,
  [SMALL_STATE(334)] = 4945,
  [SMALL_STATE(335)] = 4949,
  [SMALL_STATE(336)] = 4953,
  [SMALL_STATE(337)] = 4957,
  [SMALL_STATE(338)] = 4961,
  [SMALL_STATE(339)] = 4965,
  [SMALL_STATE(340)] = 4969,
  [SMALL_STATE(341)] = 4973,
  [SMALL_STATE(342)] = 4977,
  [SMALL_STATE(343)] = 4981,
  [SMALL_STATE(344)] = 4985,
  [SMALL_STATE(345)] = 4989,
  [SMALL_STATE(346)] = 4993,
  [SMALL_STATE(347)] = 4997,
  [SMALL_STATE(348)] = 5001,
  [SMALL_STATE(349)] = 5005,
  [SMALL_STATE(350)] = 5009,
  [SMALL_STATE(351)] = 5013,
  [SMALL_STATE(352)] = 5017,
  [SMALL_STATE(353)] = 5021,
  [SMALL_STATE(354)] = 5025,
  [SMALL_STATE(355)] = 5029,
  [SMALL_STATE(356)] = 5033,
  [SMALL_STATE(357)] = 5037,
  [SMALL_STATE(358)] = 5041,
  [SMALL_STATE(359)] = 5045,
  [SMALL_STATE(360)] = 5049,
  [SMALL_STATE(361)] = 5053,
  [SMALL_STATE(362)] = 5057,
  [SMALL_STATE(363)] = 5061,
  [SMALL_STATE(364)] = 5065,
  [SMALL_STATE(365)] = 5069,
  [SMALL_STATE(366)] = 5073,
  [SMALL_STATE(367)] = 5077,
  [SMALL_STATE(368)] = 5081,
  [SMALL_STATE(369)] = 5085,
  [SMALL_STATE(370)] = 5089,
  [SMALL_STATE(371)] = 5093,
  [SMALL_STATE(372)] = 5097,
  [SMALL_STATE(373)] = 5101,
  [SMALL_STATE(374)] = 5105,
  [SMALL_STATE(375)] = 5109,
  [SMALL_STATE(376)] = 5113,
  [SMALL_STATE(377)] = 5117,
  [SMALL_STATE(378)] = 5121,
  [SMALL_STATE(379)] = 5125,
  [SMALL_STATE(380)] = 5129,
  [SMALL_STATE(381)] = 5133,
  [SMALL_STATE(382)] = 5137,
  [SMALL_STATE(383)] = 5141,
  [SMALL_STATE(384)] = 5145,
  [SMALL_STATE(385)] = 5149,
  [SMALL_STATE(386)] = 5153,
  [SMALL_STATE(387)] = 5157,
  [SMALL_STATE(388)] = 5161,
  [SMALL_STATE(389)] = 5165,
  [SMALL_STATE(390)] = 5169,
  [SMALL_STATE(391)] = 5173,
  [SMALL_STATE(392)] = 5177,
  [SMALL_STATE(393)] = 5181,
  [SMALL_STATE(394)] = 5185,
  [SMALL_STATE(395)] = 5189,
  [SMALL_STATE(396)] = 5193,
  [SMALL_STATE(397)] = 5197,
  [SMALL_STATE(398)] = 5201,
  [SMALL_STATE(399)] = 5205,
  [SMALL_STATE(400)] = 5209,
  [SMALL_STATE(401)] = 5213,
  [SMALL_STATE(402)] = 5217,
  [SMALL_STATE(403)] = 5221,
  [SMALL_STATE(404)] = 5225,
  [SMALL_STATE(405)] = 5229,
  [SMALL_STATE(406)] = 5233,
  [SMALL_STATE(407)] = 5237,
  [SMALL_STATE(408)] = 5241,
  [SMALL_STATE(409)] = 5245,
  [SMALL_STATE(410)] = 5249,
  [SMALL_STATE(411)] = 5253,
  [SMALL_STATE(412)] = 5257,
  [SMALL_STATE(413)] = 5261,
  [SMALL_STATE(414)] = 5265,
  [SMALL_STATE(415)] = 5269,
  [SMALL_STATE(416)] = 5273,
  [SMALL_STATE(417)] = 5277,
  [SMALL_STATE(418)] = 5281,
  [SMALL_STATE(419)] = 5285,
  [SMALL_STATE(420)] = 5289,
  [SMALL_STATE(421)] = 5293,
  [SMALL_STATE(422)] = 5297,
  [SMALL_STATE(423)] = 5301,
  [SMALL_STATE(424)] = 5305,
  [SMALL_STATE(425)] = 5309,
  [SMALL_STATE(426)] = 5313,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [7] = {.entry = {.count = 1, .reusable = false}}, SHIFT(95),
  [9] = {.entry = {.count = 1, .reusable = false}}, SHIFT(175),
  [11] = {.entry = {.count = 1, .reusable = false}}, SHIFT(285),
  [13] = {.entry = {.count = 1, .reusable = false}}, SHIFT(243),
  [15] = {.entry = {.count = 1, .reusable = false}}, SHIFT(236),
  [17] = {.entry = {.count = 1, .reusable = false}}, SHIFT(421),
  [19] = {.entry = {.count = 1, .reusable = false}}, SHIFT(419),
  [21] = {.entry = {.count = 1, .reusable = true}}, SHIFT(418),
  [23] = {.entry = {.count = 1, .reusable = true}}, SHIFT(247),
  [25] = {.entry = {.count = 1, .reusable = true}}, SHIFT(249),
  [27] = {.entry = {.count = 1, .reusable = true}}, SHIFT(413),
  [29] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2),
  [31] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(2),
  [34] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(95),
  [37] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(175),
  [40] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(285),
  [43] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(243),
  [46] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(236),
  [49] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(421),
  [52] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(419),
  [55] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(418),
  [58] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(247),
  [61] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(249),
  [64] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_source_file_repeat1, 2), SHIFT_REPEAT(413),
  [67] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source_file, 1),
  [69] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [71] = {.entry = {.count = 1, .reusable = true}}, SHIFT(424),
  [73] = {.entry = {.count = 1, .reusable = true}}, SHIFT(347),
  [75] = {.entry = {.count = 1, .reusable = true}}, SHIFT(349),
  [77] = {.entry = {.count = 1, .reusable = false}}, SHIFT(349),
  [79] = {.entry = {.count = 1, .reusable = true}}, SHIFT(346),
  [81] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_scene_body_repeat1, 2),
  [83] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_scene_body_repeat1, 2), SHIFT_REPEAT(347),
  [86] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_scene_body_repeat1, 2), SHIFT_REPEAT(349),
  [89] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_scene_body_repeat1, 2), SHIFT_REPEAT(349),
  [92] = {.entry = {.count = 1, .reusable = true}}, SHIFT(361),
  [94] = {.entry = {.count = 1, .reusable = true}}, SHIFT(369),
  [96] = {.entry = {.count = 1, .reusable = true}}, SHIFT(417),
  [98] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_binary_template_repeat1, 2), SHIFT_REPEAT(361),
  [101] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_binary_template_repeat1, 2), SHIFT_REPEAT(369),
  [104] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_binary_template_repeat1, 2),
  [106] = {.entry = {.count = 1, .reusable = true}}, SHIFT(378),
  [108] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_scene_body_repeat1, 2),
  [110] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_template_field, 3, .production_id = 25),
  [112] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_string_literal, 2),
  [114] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_string_literal, 2),
  [116] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_string_literal, 3),
  [118] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_string_literal, 3),
  [120] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component, 6, .production_id = 16),
  [122] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_component, 6, .production_id = 16),
  [124] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component, 5, .production_id = 11),
  [126] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_component, 5, .production_id = 11),
  [128] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_definition, 6, .production_id = 15),
  [130] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_type_definition, 6, .production_id = 15),
  [132] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene, 8, .production_id = 22),
  [134] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_scene, 8, .production_id = 22),
  [136] = {.entry = {.count = 1, .reusable = true}}, SHIFT(31),
  [138] = {.entry = {.count = 1, .reusable = false}}, SHIFT(169),
  [140] = {.entry = {.count = 1, .reusable = false}}, SHIFT(156),
  [142] = {.entry = {.count = 1, .reusable = true}}, SHIFT(62),
  [144] = {.entry = {.count = 1, .reusable = false}}, SHIFT(47),
  [146] = {.entry = {.count = 1, .reusable = false}}, SHIFT(62),
  [148] = {.entry = {.count = 1, .reusable = true}}, SHIFT(33),
  [150] = {.entry = {.count = 1, .reusable = false}}, SHIFT(262),
  [152] = {.entry = {.count = 1, .reusable = true}}, SHIFT(222),
  [154] = {.entry = {.count = 1, .reusable = true}}, SHIFT(44),
  [156] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration, 4, .production_id = 8),
  [158] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_resource_declaration, 4, .production_id = 8),
  [160] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_directive, 2),
  [162] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_directive, 2),
  [164] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration, 5, .production_id = 12),
  [166] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_resource_declaration, 5, .production_id = 12),
  [168] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_definition, 7, .production_id = 19),
  [170] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_type_definition, 7, .production_id = 19),
  [172] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene, 7, .production_id = 18),
  [174] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_scene, 7, .production_id = 18),
  [176] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(31),
  [179] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(396),
  [182] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32),
  [184] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(62),
  [187] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(47),
  [190] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(62),
  [193] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(33),
  [196] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(262),
  [199] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(222),
  [202] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_constructor_body_repeat1, 2, .production_id = 32), SHIFT_REPEAT(44),
  [205] = {.entry = {.count = 1, .reusable = false}}, SHIFT(396),
  [207] = {.entry = {.count = 1, .reusable = true}}, SHIFT(132),
  [209] = {.entry = {.count = 1, .reusable = true}}, SHIFT(128),
  [211] = {.entry = {.count = 1, .reusable = true}}, SHIFT(47),
  [213] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_resource_reference_literal, 4, .production_id = 13),
  [215] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_reference_literal, 4, .production_id = 13),
  [217] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_resource_reference_literal, 2, .production_id = 5),
  [219] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_reference_literal, 2, .production_id = 5),
  [221] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_resource_reference_literal, 6, .production_id = 20),
  [223] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_reference_literal, 6, .production_id = 20),
  [225] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_resource_reference_literal, 1),
  [227] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_reference_literal, 1),
  [229] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_expression, 1),
  [231] = {.entry = {.count = 1, .reusable = true}}, SHIFT(37),
  [233] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_expression, 1),
  [235] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_argument_list, 4),
  [237] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_argument_list, 4),
  [239] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_argument_list, 3),
  [241] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_argument_list, 3),
  [243] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_parenthesized_expression, 3),
  [245] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_parenthesized_expression, 3),
  [247] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_binary_expression, 3, .production_id = 14),
  [249] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_expression, 3, .production_id = 14),
  [251] = {.entry = {.count = 1, .reusable = true}}, SHIFT(41),
  [253] = {.entry = {.count = 1, .reusable = true}}, SHIFT(43),
  [255] = {.entry = {.count = 1, .reusable = true}}, SHIFT(49),
  [257] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_unary_expression, 2, .production_id = 10),
  [259] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_unary_expression, 2, .production_id = 10),
  [261] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_call_expression, 2, .production_id = 9),
  [263] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_call_expression, 2, .production_id = 9),
  [265] = {.entry = {.count = 1, .reusable = true}}, SHIFT(38),
  [267] = {.entry = {.count = 1, .reusable = true}}, SHIFT(48),
  [269] = {.entry = {.count = 1, .reusable = true}}, SHIFT(39),
  [271] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_property, 3, .production_id = 24),
  [273] = {.entry = {.count = 1, .reusable = true}}, SHIFT(50),
  [275] = {.entry = {.count = 1, .reusable = true}}, SHIFT(205),
  [277] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_element_property, 3, .production_id = 24),
  [279] = {.entry = {.count = 1, .reusable = true}}, SHIFT(180),
  [281] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_resource_declaration_field_value_repeat1, 2),
  [283] = {.entry = {.count = 1, .reusable = true}}, SHIFT(54),
  [285] = {.entry = {.count = 1, .reusable = true}}, SHIFT(28),
  [287] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_comma_expression, 3, .production_id = 17),
  [289] = {.entry = {.count = 1, .reusable = true}}, SHIFT(104),
  [291] = {.entry = {.count = 1, .reusable = true}}, SHIFT(225),
  [293] = {.entry = {.count = 1, .reusable = false}}, SHIFT(104),
  [295] = {.entry = {.count = 1, .reusable = true}}, SHIFT(233),
  [297] = {.entry = {.count = 1, .reusable = true}}, SHIFT(95),
  [299] = {.entry = {.count = 1, .reusable = true}}, SHIFT(370),
  [301] = {.entry = {.count = 1, .reusable = true}}, SHIFT(169),
  [303] = {.entry = {.count = 1, .reusable = true}}, SHIFT(272),
  [305] = {.entry = {.count = 1, .reusable = true}}, SHIFT(368),
  [307] = {.entry = {.count = 1, .reusable = true}}, SHIFT(187),
  [309] = {.entry = {.count = 1, .reusable = true}}, SHIFT(231),
  [311] = {.entry = {.count = 1, .reusable = true}}, SHIFT(189),
  [313] = {.entry = {.count = 1, .reusable = true}}, SHIFT(302),
  [315] = {.entry = {.count = 1, .reusable = true}}, SHIFT(327),
  [317] = {.entry = {.count = 1, .reusable = true}}, SHIFT(326),
  [319] = {.entry = {.count = 1, .reusable = true}}, SHIFT(325),
  [321] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_key_value_pair, 3, .production_id = 51),
  [323] = {.entry = {.count = 1, .reusable = true}}, SHIFT(345),
  [325] = {.entry = {.count = 1, .reusable = true}}, SHIFT(55),
  [327] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_type_constructor_body_repeat1, 6, .production_id = 52),
  [329] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_type_constructor_body_repeat1, 6, .production_id = 52),
  [331] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_resource_declaration_body_repeat1, 2), SHIFT_REPEAT(95),
  [334] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_resource_declaration_body_repeat1, 2),
  [336] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_resource_declaration_body_repeat1, 2), SHIFT_REPEAT(169),
  [339] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_resource_declaration_body_repeat1, 2), SHIFT_REPEAT(272),
  [342] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_resource_declaration_body_repeat1, 2), SHIFT_REPEAT(368),
  [345] = {.entry = {.count = 1, .reusable = true}}, SHIFT(227),
  [347] = {.entry = {.count = 1, .reusable = true}}, SHIFT(202),
  [349] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_type_constructor_body_repeat1, 4, .production_id = 43),
  [351] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_type_constructor_body_repeat1, 4, .production_id = 43),
  [353] = {.entry = {.count = 1, .reusable = true}}, SHIFT(235),
  [355] = {.entry = {.count = 1, .reusable = true}}, SHIFT(309),
  [357] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_definition_body_repeat1, 2), SHIFT_REPEAT(95),
  [360] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_type_definition_body_repeat1, 2),
  [362] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_definition_body_repeat1, 2), SHIFT_REPEAT(327),
  [365] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_definition_body_repeat1, 2), SHIFT_REPEAT(326),
  [368] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_definition_body_repeat1, 2), SHIFT_REPEAT(325),
  [371] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_definition_repeat1, 2), SHIFT_REPEAT(95),
  [374] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_type_definition_repeat1, 2),
  [376] = {.entry = {.count = 1, .reusable = false}}, SHIFT(315),
  [378] = {.entry = {.count = 1, .reusable = true}}, SHIFT(82),
  [380] = {.entry = {.count = 1, .reusable = true}}, SHIFT(86),
  [382] = {.entry = {.count = 1, .reusable = false}}, SHIFT(393),
  [384] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_decorator, 1, .production_id = 1),
  [386] = {.entry = {.count = 1, .reusable = true}}, SHIFT(77),
  [388] = {.entry = {.count = 1, .reusable = true}}, SHIFT(171),
  [390] = {.entry = {.count = 1, .reusable = false}}, SHIFT(246),
  [392] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_function_directive, 4, .production_id = 6),
  [394] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_variable_directive, 4, .production_id = 6),
  [396] = {.entry = {.count = 1, .reusable = true}}, SHIFT(36),
  [398] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_decorator, 5, .production_id = 1),
  [400] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_decorator, 4, .production_id = 1),
  [402] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_value, 1),
  [404] = {.entry = {.count = 1, .reusable = true}}, SHIFT(328),
  [406] = {.entry = {.count = 1, .reusable = true}}, SHIFT(329),
  [408] = {.entry = {.count = 1, .reusable = true}}, SHIFT(397),
  [410] = {.entry = {.count = 1, .reusable = true}}, SHIFT(256),
  [412] = {.entry = {.count = 1, .reusable = true}}, SHIFT(257),
  [414] = {.entry = {.count = 1, .reusable = true}}, SHIFT(392),
  [416] = {.entry = {.count = 1, .reusable = true}}, SHIFT(224),
  [418] = {.entry = {.count = 1, .reusable = true}}, SHIFT(299),
  [420] = {.entry = {.count = 1, .reusable = true}}, SHIFT(322),
  [422] = {.entry = {.count = 1, .reusable = true}}, SHIFT(323),
  [424] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_resource_declaration_values_repeat1, 2), SHIFT_REPEAT(224),
  [427] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_resource_declaration_values_repeat1, 2),
  [429] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_resource_declaration_values_repeat1, 2), SHIFT_REPEAT(322),
  [432] = {.entry = {.count = 1, .reusable = true}}, SHIFT(358),
  [434] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_scene_element_body_repeat1, 2),
  [436] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_scene_element_body_repeat1, 2), SHIFT_REPEAT(329),
  [439] = {.entry = {.count = 1, .reusable = true}}, SHIFT(319),
  [441] = {.entry = {.count = 1, .reusable = true}}, SHIFT(126),
  [443] = {.entry = {.count = 1, .reusable = true}}, SHIFT(372),
  [445] = {.entry = {.count = 1, .reusable = true}}, SHIFT(338),
  [447] = {.entry = {.count = 1, .reusable = true}}, SHIFT(339),
  [449] = {.entry = {.count = 1, .reusable = true}}, SHIFT(183),
  [451] = {.entry = {.count = 1, .reusable = true}}, SHIFT(72),
  [453] = {.entry = {.count = 1, .reusable = true}}, SHIFT(152),
  [455] = {.entry = {.count = 1, .reusable = true}}, SHIFT(114),
  [457] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_new_resource, 3),
  [459] = {.entry = {.count = 1, .reusable = true}}, SHIFT(360),
  [461] = {.entry = {.count = 1, .reusable = true}}, SHIFT(367),
  [463] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 2, .production_id = 36), SHIFT_REPEAT(224),
  [466] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 2, .production_id = 36),
  [468] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 2, .production_id = 36), SHIFT_REPEAT(126),
  [471] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_component_body_repeat1, 2),
  [473] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_component_body_repeat1, 2), SHIFT_REPEAT(341),
  [476] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_component_body_repeat1, 2), SHIFT_REPEAT(340),
  [479] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_component_files_repeat1, 2),
  [481] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_component_files_repeat1, 2), SHIFT_REPEAT(222),
  [484] = {.entry = {.count = 1, .reusable = true}}, SHIFT(314),
  [486] = {.entry = {.count = 1, .reusable = true}}, SHIFT(341),
  [488] = {.entry = {.count = 1, .reusable = true}}, SHIFT(340),
  [490] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_new_resource, 4),
  [492] = {.entry = {.count = 1, .reusable = true}}, SHIFT(308),
  [494] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_field_definition_value_symbol_list_repeat1, 2, .production_id = 50), SHIFT_REPEAT(224),
  [497] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_value_symbol_list_repeat1, 2, .production_id = 50),
  [499] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_field_definition_value_symbol_list_repeat1, 2, .production_id = 50), SHIFT_REPEAT(339),
  [502] = {.entry = {.count = 1, .reusable = true}}, SHIFT(208),
  [504] = {.entry = {.count = 1, .reusable = true}}, SHIFT(73),
  [506] = {.entry = {.count = 1, .reusable = true}}, SHIFT(342),
  [508] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_constructor_body, 3, .production_id = 31),
  [510] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_duplicate_resource, 9, .production_id = 39),
  [512] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_template, 4),
  [514] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_constructor_body, 2),
  [516] = {.entry = {.count = 1, .reusable = true}}, SHIFT(389),
  [518] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition, 6, .production_id = 11),
  [520] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition, 8, .production_id = 46),
  [522] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_duplicate_resource, 8, .production_id = 34),
  [524] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_constructor, 5),
  [526] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_template, 5),
  [528] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_override_resource, 4),
  [530] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition, 5, .production_id = 11),
  [532] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_constructor, 6),
  [534] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition, 7, .production_id = 38),
  [536] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition, 7, .production_id = 16),
  [538] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition, 6, .production_id = 16),
  [540] = {.entry = {.count = 1, .reusable = true}}, SHIFT(144),
  [542] = {.entry = {.count = 1, .reusable = true}}, SHIFT(117),
  [544] = {.entry = {.count = 1, .reusable = true}}, SHIFT(391),
  [546] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition, 7, .production_id = 37),
  [548] = {.entry = {.count = 1, .reusable = true}}, SHIFT(85),
  [550] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_explicit_type, 2, .production_id = 45),
  [552] = {.entry = {.count = 1, .reusable = true}}, SHIFT(228),
  [554] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition, 6, .production_id = 33),
  [556] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_constructor, 4),
  [558] = {.entry = {.count = 1, .reusable = true}}, SHIFT(140),
  [560] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_explicit_type, 1),
  [562] = {.entry = {.count = 1, .reusable = true}}, SHIFT(234),
  [564] = {.entry = {.count = 1, .reusable = true}}, SHIFT(147),
  [566] = {.entry = {.count = 1, .reusable = true}}, SHIFT(179),
  [568] = {.entry = {.count = 1, .reusable = true}}, SHIFT(75),
  [570] = {.entry = {.count = 1, .reusable = true}}, SHIFT(300),
  [572] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_out_directive, 2),
  [574] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_field_value_import, 1),
  [576] = {.entry = {.count = 1, .reusable = true}}, SHIFT(298),
  [578] = {.entry = {.count = 1, .reusable = true}}, SHIFT(297),
  [580] = {.entry = {.count = 1, .reusable = true}}, SHIFT(238),
  [582] = {.entry = {.count = 1, .reusable = true}}, SHIFT(84),
  [584] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_field_value_import, 2),
  [586] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_resource_declaration_field_value_repeat1, 2), SHIFT_REPEAT(39),
  [589] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_explicit_type, 5, .production_id = 45),
  [591] = {.entry = {.count = 1, .reusable = true}}, SHIFT(200),
  [593] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_explicit_type, 6, .production_id = 45),
  [595] = {.entry = {.count = 1, .reusable = true}}, SHIFT(223),
  [597] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_value_symbol_list_repeat1, 3, .production_id = 24),
  [599] = {.entry = {.count = 1, .reusable = true}}, SHIFT(192),
  [601] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_value_symbol_list_repeat1, 4, .production_id = 55),
  [603] = {.entry = {.count = 1, .reusable = true}}, SHIFT(207),
  [605] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_out_directive_repeat1, 2),
  [607] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_out_directive_repeat1, 2), SHIFT_REPEAT(222),
  [610] = {.entry = {.count = 1, .reusable = true}}, SHIFT(303),
  [612] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_component_types_repeat1, 2),
  [614] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_component_types_repeat1, 2), SHIFT_REPEAT(297),
  [617] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_definition_repeat1, 2), SHIFT_REPEAT(224),
  [620] = {.entry = {.count = 1, .reusable = true}}, SHIFT(108),
  [622] = {.entry = {.count = 1, .reusable = true}}, SHIFT(333),
  [624] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_decorator_repeat1, 2),
  [626] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_decorator_repeat1, 2), SHIFT_REPEAT(76),
  [629] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_out_directive, 1),
  [631] = {.entry = {.count = 1, .reusable = true}}, SHIFT(366),
  [633] = {.entry = {.count = 1, .reusable = true}}, SHIFT(115),
  [635] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 4, .production_id = 6),
  [637] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_field_value, 4, .production_id = 23),
  [639] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 5, .production_id = 6),
  [641] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_element_property, 4, .production_id = 29),
  [643] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 3, .production_id = 6),
  [645] = {.entry = {.count = 1, .reusable = true}}, SHIFT(143),
  [647] = {.entry = {.count = 1, .reusable = true}}, SHIFT(199),
  [649] = {.entry = {.count = 1, .reusable = true}}, SHIFT(53),
  [651] = {.entry = {.count = 1, .reusable = true}}, SHIFT(76),
  [653] = {.entry = {.count = 1, .reusable = true}}, SHIFT(103),
  [655] = {.entry = {.count = 1, .reusable = true}}, SHIFT(162),
  [657] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 5, .production_id = 54),
  [659] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_value_symbol_list_repeat1, 4, .production_id = 24),
  [661] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_property, 4, .production_id = 29),
  [663] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_field_value, 6, .production_id = 26),
  [665] = {.entry = {.count = 1, .reusable = false}}, SHIFT(13),
  [667] = {.entry = {.count = 1, .reusable = true}}, SHIFT(221),
  [669] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 6, .production_id = 53),
  [671] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 6, .production_id = 54),
  [673] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_named_file_reference, 1, .production_id = 28),
  [675] = {.entry = {.count = 1, .reusable = true}}, SHIFT(290),
  [677] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 6, .production_id = 56),
  [679] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_explicit_type, 6, .production_id = 57),
  [681] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 5, .production_id = 53),
  [683] = {.entry = {.count = 1, .reusable = true}}, SHIFT(161),
  [685] = {.entry = {.count = 1, .reusable = true}}, SHIFT(286),
  [687] = {.entry = {.count = 1, .reusable = true}}, SHIFT(287),
  [689] = {.entry = {.count = 1, .reusable = true}}, SHIFT(194),
  [691] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_field_value, 5, .production_id = 26),
  [693] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_field_value, 5, .production_id = 23),
  [695] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_value_symbol_list_repeat1, 5, .production_id = 55),
  [697] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 2, .production_id = 1),
  [699] = {.entry = {.count = 1, .reusable = true}}, SHIFT(416),
  [701] = {.entry = {.count = 1, .reusable = true}}, SHIFT(250),
  [703] = {.entry = {.count = 1, .reusable = true}}, SHIFT(134),
  [705] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 5, .production_id = 47),
  [707] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_file_reference_attributes_repeat1, 2), SHIFT_REPEAT(276),
  [710] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_file_reference_attributes_repeat1, 2),
  [712] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 7, .production_id = 56),
  [714] = {.entry = {.count = 1, .reusable = true}}, SHIFT(253),
  [716] = {.entry = {.count = 1, .reusable = true}}, SHIFT(101),
  [718] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_type_constructor_repeat1, 2), SHIFT_REPEAT(416),
  [721] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_type_constructor_repeat1, 2),
  [723] = {.entry = {.count = 1, .reusable = true}}, SHIFT(206),
  [725] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_key_value_pair, 1, .production_id = 40),
  [727] = {.entry = {.count = 1, .reusable = true}}, SHIFT(35),
  [729] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_string_literal_repeat1, 2),
  [731] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_string_literal_repeat1, 2), SHIFT_REPEAT(221),
  [734] = {.entry = {.count = 1, .reusable = false}}, SHIFT(12),
  [736] = {.entry = {.count = 1, .reusable = true}}, SHIFT(195),
  [738] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_explicit_type, 7, .production_id = 58),
  [740] = {.entry = {.count = 1, .reusable = true}}, SHIFT(87),
  [742] = {.entry = {.count = 1, .reusable = true}}, SHIFT(191),
  [744] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 4, .production_id = 1),
  [746] = {.entry = {.count = 1, .reusable = true}}, SHIFT(201),
  [748] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_explicit_type, 3, .production_id = 48),
  [750] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 4, .production_id = 47),
  [752] = {.entry = {.count = 1, .reusable = true}}, SHIFT(276),
  [754] = {.entry = {.count = 1, .reusable = true}}, SHIFT(354),
  [756] = {.entry = {.count = 1, .reusable = true}}, SHIFT(379),
  [758] = {.entry = {.count = 1, .reusable = true}}, SHIFT(229),
  [760] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_explicit_type, 2, .production_id = 44),
  [762] = {.entry = {.count = 1, .reusable = true}}, SHIFT(251),
  [764] = {.entry = {.count = 1, .reusable = true}}, SHIFT(408),
  [766] = {.entry = {.count = 1, .reusable = true}}, SHIFT(259),
  [768] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_field_definition_values_repeat1, 3, .production_id = 1),
  [770] = {.entry = {.count = 1, .reusable = true}}, SHIFT(248),
  [772] = {.entry = {.count = 1, .reusable = true}}, SHIFT(267),
  [774] = {.entry = {.count = 1, .reusable = true}}, SHIFT(209),
  [776] = {.entry = {.count = 1, .reusable = true}}, SHIFT(133),
  [778] = {.entry = {.count = 1, .reusable = true}}, SHIFT(426),
  [780] = {.entry = {.count = 1, .reusable = true}}, SHIFT(174),
  [782] = {.entry = {.count = 1, .reusable = true}}, SHIFT(425),
  [784] = {.entry = {.count = 1, .reusable = true}}, SHIFT(173),
  [786] = {.entry = {.count = 1, .reusable = true}}, SHIFT(254),
  [788] = {.entry = {.count = 1, .reusable = true}}, SHIFT(399),
  [790] = {.entry = {.count = 1, .reusable = true}}, SHIFT(26),
  [792] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_repeatable, 8),
  [794] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_identifier, 1, .production_id = 4),
  [796] = {.entry = {.count = 1, .reusable = true}}, SHIFT(386),
  [798] = {.entry = {.count = 1, .reusable = true}}, SHIFT(74),
  [800] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_repeatable, 6),
  [802] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_type_reference, 2, .production_id = 4),
  [804] = {.entry = {.count = 1, .reusable = true}}, SHIFT(373),
  [806] = {.entry = {.count = 1, .reusable = true}}, SHIFT(30),
  [808] = {.entry = {.count = 1, .reusable = true}}, SHIFT(40),
  [810] = {.entry = {.count = 1, .reusable = true}}, SHIFT(365),
  [812] = {.entry = {.count = 1, .reusable = true}}, SHIFT(127),
  [814] = {.entry = {.count = 1, .reusable = true}}, SHIFT(260),
  [816] = {.entry = {.count = 1, .reusable = true}}, SHIFT(394),
  [818] = {.entry = {.count = 1, .reusable = true}}, SHIFT(88),
  [820] = {.entry = {.count = 1, .reusable = true}}, SHIFT(331),
  [822] = {.entry = {.count = 1, .reusable = true}}, SHIFT(29),
  [824] = {.entry = {.count = 1, .reusable = true}}, SHIFT(220),
  [826] = {.entry = {.count = 1, .reusable = true}}, SHIFT(105),
  [828] = {.entry = {.count = 1, .reusable = true}}, SHIFT(375),
  [830] = {.entry = {.count = 1, .reusable = true}}, SHIFT(269),
  [832] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_repeatable, 4),
  [834] = {.entry = {.count = 1, .reusable = true}}, SHIFT(5),
  [836] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_file_reference, 2),
  [838] = {.entry = {.count = 1, .reusable = true}}, SHIFT(321),
  [840] = {.entry = {.count = 1, .reusable = true}}, SHIFT(292),
  [842] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_file_reference, 3),
  [844] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_named_file_reference, 3, .production_id = 41),
  [846] = {.entry = {.count = 1, .reusable = true}}, SHIFT(279),
  [848] = {.entry = {.count = 1, .reusable = true}}, SHIFT(278),
  [850] = {.entry = {.count = 1, .reusable = true}}, SHIFT(294),
  [852] = {.entry = {.count = 1, .reusable = true}}, SHIFT(22),
  [854] = {.entry = {.count = 1, .reusable = true}}, SHIFT(261),
  [856] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_types, 3),
  [858] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_values, 3),
  [860] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [862] = {.entry = {.count = 1, .reusable = true}}, SHIFT(146),
  [864] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_definition_body, 3),
  [866] = {.entry = {.count = 1, .reusable = true}}, SHIFT(241),
  [868] = {.entry = {.count = 1, .reusable = true}}, SHIFT(142),
  [870] = {.entry = {.count = 1, .reusable = true}}, SHIFT(129),
  [872] = {.entry = {.count = 1, .reusable = true}}, SHIFT(289),
  [874] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_files, 7, .production_id = 27),
  [876] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_definition_body, 2),
  [878] = {.entry = {.count = 1, .reusable = true}}, SHIFT(332),
  [880] = {.entry = {.count = 1, .reusable = true}}, SHIFT(52),
  [882] = {.entry = {.count = 1, .reusable = true}}, SHIFT(23),
  [884] = {.entry = {.count = 1, .reusable = true}}, SHIFT(24),
  [886] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_body, 3),
  [888] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_configuration, 5, .production_id = 13),
  [890] = {.entry = {.count = 1, .reusable = true}}, SHIFT(185),
  [892] = {.entry = {.count = 1, .reusable = true}}, SHIFT(139),
  [894] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition_values, 3, .production_id = 35),
  [896] = {.entry = {.count = 1, .reusable = true}}, SHIFT(123),
  [898] = {.entry = {.count = 1, .reusable = true}}, SHIFT(19),
  [900] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_values, 2),
  [902] = {.entry = {.count = 1, .reusable = true}}, SHIFT(135),
  [904] = {.entry = {.count = 1, .reusable = true}}, SHIFT(293),
  [906] = {.entry = {.count = 1, .reusable = true}}, SHIFT(242),
  [908] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [910] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_element_body, 2),
  [912] = {.entry = {.count = 1, .reusable = true}}, SHIFT(34),
  [914] = {.entry = {.count = 1, .reusable = true}}, SHIFT(131),
  [916] = {.entry = {.count = 1, .reusable = true}}, SHIFT(395),
  [918] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_element, 6, .production_id = 42),
  [920] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_types, 4),
  [922] = {.entry = {.count = 1, .reusable = true}}, SHIFT(14),
  [924] = {.entry = {.count = 1, .reusable = true}}, SHIFT(113),
  [926] = {.entry = {.count = 1, .reusable = true}}, SHIFT(335),
  [928] = {.entry = {.count = 1, .reusable = true}}, SHIFT(211),
  [930] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition_value_symbol_list, 2),
  [932] = {.entry = {.count = 1, .reusable = true}}, SHIFT(96),
  [934] = {.entry = {.count = 1, .reusable = true}}, SHIFT(284),
  [936] = {.entry = {.count = 1, .reusable = true}}, SHIFT(157),
  [938] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_body, 2),
  [940] = {.entry = {.count = 1, .reusable = true}}, SHIFT(51),
  [942] = {.entry = {.count = 1, .reusable = true}}, SHIFT(226),
  [944] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_body, 3),
  [946] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_body, 2),
  [948] = {.entry = {.count = 1, .reusable = true}}, SHIFT(42),
  [950] = {.entry = {.count = 1, .reusable = true}}, SHIFT(244),
  [952] = {.entry = {.count = 1, .reusable = true}}, SHIFT(311),
  [954] = {.entry = {.count = 1, .reusable = true}}, SHIFT(116),
  [956] = {.entry = {.count = 1, .reusable = true}}, SHIFT(16),
  [958] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_type, 4, .production_id = 30),
  [960] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_file_reference_attributes, 3),
  [962] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [964] = {.entry = {.count = 1, .reusable = true}}, SHIFT(295),
  [966] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_element_body, 3),
  [968] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [970] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition_values, 2),
  [972] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_binary_type, 1),
  [974] = {.entry = {.count = 1, .reusable = true}}, SHIFT(212),
  [976] = {.entry = {.count = 1, .reusable = true}}, SHIFT(282),
  [978] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [980] = {.entry = {.count = 1, .reusable = true}}, SHIFT(343),
  [982] = {.entry = {.count = 1, .reusable = true}}, SHIFT(99),
  [984] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_field_definition_value_symbol_list, 3, .production_id = 49),
  [986] = {.entry = {.count = 1, .reusable = true}}, SHIFT(240),
  [988] = {.entry = {.count = 1, .reusable = true}}, SHIFT(422),
  [990] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_resource_declaration_body, 2),
  [992] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_type_identifier, 3, .production_id = 7),
  [994] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_files, 6, .production_id = 27),
  [996] = {.entry = {.count = 1, .reusable = true}}, SHIFT(275),
  [998] = {.entry = {.count = 1, .reusable = true}}, SHIFT(181),
  [1000] = {.entry = {.count = 1, .reusable = true}}, SHIFT(136),
  [1002] = {.entry = {.count = 1, .reusable = true}}, SHIFT(274),
  [1004] = {.entry = {.count = 1, .reusable = true}}, SHIFT(130),
  [1006] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_file_reference_attributes, 4),
  [1008] = {.entry = {.count = 1, .reusable = true}}, SHIFT(420),
  [1010] = {.entry = {.count = 1, .reusable = true}}, SHIFT(273),
  [1012] = {.entry = {.count = 1, .reusable = true}}, SHIFT(363),
  [1014] = {.entry = {.count = 1, .reusable = true}}, SHIFT(151),
  [1016] = {.entry = {.count = 1, .reusable = true}}, SHIFT(148),
  [1018] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [1020] = {.entry = {.count = 1, .reusable = true}}, SHIFT(371),
  [1022] = {.entry = {.count = 1, .reusable = true}}, SHIFT(196),
  [1024] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_format_directive, 3),
  [1026] = {.entry = {.count = 1, .reusable = true}}, SHIFT(197),
  [1028] = {.entry = {.count = 1, .reusable = true}}, SHIFT(288),
  [1030] = {.entry = {.count = 1, .reusable = true}}, SHIFT(215),
  [1032] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_component_configuration, 7, .production_id = 21),
  [1034] = {.entry = {.count = 1, .reusable = true}}, SHIFT(404),
  [1036] = {.entry = {.count = 1, .reusable = true}}, SHIFT(32),
  [1038] = {.entry = {.count = 1, .reusable = true}}, SHIFT(350),
  [1040] = {.entry = {.count = 1, .reusable = true}}, SHIFT(381),
  [1042] = {.entry = {.count = 1, .reusable = true}}, SHIFT(265),
  [1044] = {.entry = {.count = 1, .reusable = true}}, SHIFT(264),
  [1046] = {.entry = {.count = 1, .reusable = true}}, SHIFT(353),
  [1048] = {.entry = {.count = 1, .reusable = true}}, SHIFT(277),
  [1050] = {.entry = {.count = 1, .reusable = true}}, SHIFT(263),
  [1052] = {.entry = {.count = 1, .reusable = true}}, SHIFT(214),
  [1054] = {.entry = {.count = 1, .reusable = true}}, SHIFT(252),
  [1056] = {.entry = {.count = 1, .reusable = true}}, SHIFT(46),
  [1058] = {.entry = {.count = 1, .reusable = true}}, SHIFT(45),
  [1060] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_import_directive, 2, .production_id = 3),
  [1062] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_import_directive, 2, .production_id = 2),
  [1064] = {.entry = {.count = 1, .reusable = true}}, SHIFT(271),
  [1066] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_metadata_directive, 2),
  [1068] = {.entry = {.count = 1, .reusable = true}}, SHIFT(21),
  [1070] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
  [1072] = {.entry = {.count = 1, .reusable = true}}, SHIFT(186),
  [1074] = {.entry = {.count = 1, .reusable = true}}, SHIFT(145),
  [1076] = {.entry = {.count = 1, .reusable = true}}, SHIFT(258),
  [1078] = {.entry = {.count = 1, .reusable = true}}, SHIFT(138),
  [1080] = {.entry = {.count = 1, .reusable = true}}, SHIFT(402),
  [1082] = {.entry = {.count = 1, .reusable = true}}, SHIFT(405),
  [1084] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [1086] = {.entry = {.count = 1, .reusable = true}}, SHIFT(406),
  [1088] = {.entry = {.count = 1, .reusable = true}}, SHIFT(400),
  [1090] = {.entry = {.count = 1, .reusable = true}}, SHIFT(176),
  [1092] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_scene_body, 3),
  [1094] = {.entry = {.count = 1, .reusable = true}}, SHIFT(245),
  [1096] = {.entry = {.count = 1, .reusable = true}}, SHIFT(423),
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#define extern __declspec(dllexport)
#endif

extern const TSLanguage *tree_sitter_KDL(void) {
  static const TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .state_count = STATE_COUNT,
    .large_state_count = LARGE_STATE_COUNT,
    .production_id_count = PRODUCTION_ID_COUNT,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .parse_table = &ts_parse_table[0][0],
    .small_parse_table = ts_small_parse_table,
    .small_parse_table_map = ts_small_parse_table_map,
    .parse_actions = ts_parse_actions,
    .symbol_names = ts_symbol_names,
    .field_names = ts_field_names,
    .field_map_slices = ts_field_map_slices,
    .field_map_entries = ts_field_map_entries,
    .symbol_metadata = ts_symbol_metadata,
    .public_symbol_map = ts_symbol_map,
    .alias_map = ts_non_terminal_alias_map,
    .alias_sequences = &ts_alias_sequences[0][0],
    .lex_modes = ts_lex_modes,
    .lex_fn = ts_lex,
    .primary_state_ids = ts_primary_state_ids,
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
