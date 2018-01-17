# TODO
- bool is not being recognized as a type
- struct field member start needs to validate the type exists, e.g. 
    struct Struct1 {
         10:    heartbeat; 	// oops, forgot type, but error is on ;
    }
- 'using' clause is not putting types into Type Cache correctly, e.g. 
- need to ensure field labels are not duplicated, they must be unique, e.g. 
    struct Struct1 {
	10: u8 u1;
	10: u8 u2;	// oops, duplicate field ID
    }

- implement test coverage showing case values don't overflow the switching type.
- implement test coverage showing the switching type is an integer type. 

- implement an AST validator as tree automata. It will validate relationships only, i.e. attribute block needs to be appended to an attribute node, EnumField are only under Enum, etc. This will be one of the "standard" plugins available for users, it will be distributed with the tool.
- implement a print AST plugin, it will be a "standard" plugin distributed with the tool.
- open all unit tests and review headers to ensure I've included only things we're using. 

## Later 

- implement initialization lists so arrays can be initialized
