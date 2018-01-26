# Swizzle Language

This is an informal description of the Swizzle DSL for describing messages on the wire.

## Keywords

- import
- namespace
- using 
- struct 
- enum 
- const 
- extern

## Built-in Types

- u8 
- i8
- u16
- i16
- u32
- i32
- u64
- i64
- f32
- f64
- bitfield
- bool
- variable_block

## Tokens

- **{**         
    l_brace
- **}**
    r_brace
- **[**
    l_bracket
- **]**
    r_bracket
- **;**
    end_statement
- **.**
    dot
- **:**
    colon
- **/**
    slash
- **=**
    equal
- **,**
    comma
- **"blah blah"**
    string_literal
- **'a'**
    char_literal
- **40**
    numeric_literal
- **0x04**
    hex_literal

## Language Syntax

    // comment

    // multi-line \
       comment note the continuation \
       back-slash is followed by newline

    // import a type defined in the same namespace (same folder on disk)
    import <type_name>;

    // import a type from a different namespace (different folder on disk)
    import <namespace>::<type_name>;

    // extern is used to declare types which will be used but not defined using swizzle
    extern <type_name>;
    extern <namespace>::<type_name>;
    extern <namespace>::<namespace>::<type_name>; 

    namespace <namespace>;                  // e.g. foo 
    namespace <namespace>::<namespace>;     // nested namespaces, e.g. foo::bar

    enum <name> : <underlying_type> { 
        value,                              // defaults start value to 0
        value = <hex_literal>,              // literal should be validated against declared type.
        value = <char_literal>,
        Value = <numeric_literal>,
    } 

    // attribute applied to enum node
    @<attribute>
    enum <name> : <underlying_type> {
        value,
    }

    // key/value attribute applied to enum node
    @<attribute>=<value>
    enum <name> : <underlying_type> {
        value,
    }

    // attribute block applied to enum node
    @<attribute>{<content>}
    enum <name> : <underlying_type> {
        value,
    }

    // define field of bits where the bits (or groups) of bits 
    // are named.
    bitfield <name> : <underlying_type> {
        // single bit should be treated as boolean
        <field_name> : <numeric_literal>,

        // multi-bit field should be treated as integer from 0 to 
        <field_name> : <numeric_literal>..<numeric_literal>,
    }

    // attribute applied to bitfield node
    @<attribute>
    bitfield <name> : <underlying_type> {
        <field_name> : <numeric_literal>,
    }

    // key/value attribute applied to bitfield node
    @<attribute>=<value>
    bitfield <name> : <underlying_type> {
        <field_name> : <numeric_literal>,
    }

    // attribute block applied to bitfield node
    @<attribute>{<content>}
    bitfield <name> : <underlying_type> {
        <field_name> : <numeric_literal>,
    }

    // type_name on left hand side must be new valid alias 
    // the type_name on the right hand side must already be defined or be built-in.
    using <type_name> = <namespace>::<namespace>::<type_name>;
    using <type_name> = <type_name>; 

    struct <name> {
        <type_name> <field_name>;
        <namespace>::<namespace>::<type_name> <field_name>;

        // type array sized by literal value
        <type_name>[<numeric_literal>] <field_name>;	

        // vector of type sized by <size_field>
        <type_name>[<size_field>] <field_name>;		

        // vector of type sized by <size_field> embedded in another field
        <type_name>[<field>.<size_field>] <field_name>;	

        // apply label to struct field node
        <numeric_literal>: <type_name> <field_name>;
	
        // apply attribute to struct field node
        @<attribute>
        <type_name> <field_name>;

        // apply block attribute to struct field node
        @<attribute>{<content>}
        <type_name> <field_name>;

        // apply key/value attribute to struct field node
        @<attribute>=<value>
        <type_name> <field_name>;

        // define a constant value for the structure to use
        // the literal should be validated against the type 
        // if possible.
        const <type_name> <field_name> = <literal>;

        // variable_block indicates content in the message which 
        // will vary based on the value of another field. The <field_name>
        // field is the who's value indicates the contents that follow, the 
        // case statements link specific values to other structures. 
        variable_block : <field_name> {
            case <literal>: <type_name>,
            case <literal>: <type_name>,
        }

        variable_block : <field_name>.<field_name> {
            case <literal>: <namespace>::<namespace>::<type_name>,
        }
    }

    // attribute applied to struct node
    @<attribute>
    struct <name> {
        <type_name> <field_name>;
    }

    // apply key/value attribute to struct node
    @<attribute>=<value>
    struct <name> {
        <type_name> <field_name>;
    }

    // apply attribute block to struct node
    @<attribute>{<content>}
    struct <name> {
        <type_name> <field_name>;
    }

## Example Attributes

It should be noted, Swizzle doesn't process attributes beyond attaching them to the correct node in the AST. It is up to the backend processing the AST to define and interpret attributes.

Here are some attributes a backend might define:

    @validate{size != 0}
    @justified="left"
    @justified="right"
    @padded=' '
    @padded='\0'
    @ignore 
    @optional
    @little_endian
    @big_endian
    @inbound
    @outbound

