# TODO

- better edge-case coverage for safe_istringstream unit tests
- validate enum assignment works 
- implement an AST validator as tree automata. It will validate relationships only, i.e. attribute block needs to be appended to an attribute node, EnumField are only under Enum, etc. This will be one of the "standard" plugins available for users, it will be distributed with the tool.
- implement a print AST plugin, it will be a "standard" plugin distributed with the tool.
- open all unit tests and review headers to ensure I've included only things we're using. 
