# TODO
- add support for attributes on inherited classes 
- implement inheritance for: 
	- struct 
	- enum 
	- bitfield

- implement test coverage showing const is correctly applied to fields, and correctly disallowed on variable_block's
- implement test coverage showing case values don't overflow the switching type.
- implement test coverage showing the switching type is an integer type. 

- implement an AST validator as tree automata. It will validate relationships only, i.e. attribute block needs to be appended to an attribute node, EnumField are only under Enum, etc. This will be one of the "standard" plugins available for users, it will be distributed with the tool.
- open all unit tests and review headers to ensure I've included only things we're using. 

## Later 

- implement single inheritance for structs
- implement initialization lists so arrays can be initialized
