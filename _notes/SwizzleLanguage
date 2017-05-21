# Swizzle Language

This is an informal description of the Swizzle DSL for describing messages on the wire.

## Keywords

- import
- namespace
- using 
- struct 
- enum 
- const 

## Built-in Types

- u8 
- i8
- u16
- i16
- u32
- i32
- u64
- i64
- bitfield
- variable_block

## Tokens

- {				// l_brace
- }				// r_brace
- [				// l_bracket
- ]				// r_bracket
- ;				// end_statement
- .				// dot
- :				// colon
- /				// slash
- =				// equal
- ,				// comma
- string_literal 		// “blah blah”
- char_literal 			// ‘\0’
- numeric_literal		// 40
- hex_literal 			// 0x04

## Language Syntax

    // comment

    // multi-line \
       comment 

    import <namespace>::<type_name>;  // must be declared before namespace

    namespace <namespace>;  // e.g. exegy::fix

    enum <name> : <underlying_type> { 
        value = <literal>,		// literal should be validated against declared type.
    } 

    bitfield <name> : <underlying_type> {
        <field_name> : <numeric_literal>,  // single bit field to be treated as boolean 0 is false, 1 is true 
        <field_name> : <numeric_literal>..<numeric_literal>,  // field is field from the first numeric literal to the next
    }

    using <type_name> = <type_name>;  // type_name on left hand side must be new valid alias and type_name 
				      // on right hand side must already be defined or be built-in.

    struct <name> {
        <type_name> <field_name>;			// normal field
        <type_name>[<numeric_literal>] <field_name>;	// array of type
        <type_name>[<size_field>] <field_name>;		// vector of type sized by <size_field>
        <type_name>[<field>.<size_field>] <field_name>;	// vector of type sized by <size_field> embedded in another field

        <field_label>: <type_name> <field_name>;  // optionally labeled field 
	
        @<attribute>
        <type_name> <field_name>;  // field with flag attribute

        @<attribute>{<string>}
        <type_name> <field_name>;  // field with code-block attribute

        @<attribute>=<value>
        <type_name> <field_name>;  // field with key/value attribute

        const <type_name> <field_name> = <literal>;  // a constant literal should be validated against type

        variable_block : <field_name> {	  // variable length block containing a 
            case <value>: <type_name>,	  // type defined by value in <field_name>
            case <value>: <type_name>,
            case <value>: <type_name>,
        }
    }

## Example Attributes

    @justified=left
    @justified=right
    @padded=' '
    @padded='\0'
    @ignore 
    @optional
    @little_endian
    @big_endian

