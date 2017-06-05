# TODO

- add state for attribute handling, this will be needed to handle block attributes correctly. 
- add nodes to AST when processing import statements
- add nodes to AST when processing namespace statements
- once safe_numerics is accepted to boost, adapt it to improve validation of literal values.
- implement an AST validator as tree automata. It will validate relationships only, i.e. attribute block needs to be appended to an attribute node, EnumField are only under Enum, etc. This will be one of the "standard" plugins available for users, it will be distributed with the tool.
- implement a print AST plugin, it will be a "standard" plugin distributed with the tool.
- comments will cause enum/struct/bitfields to be non-empty, need the empty logic to be smarter.
- open all unit tests and review headers to ensure I've included only things we're using. 
