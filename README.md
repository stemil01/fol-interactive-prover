# An interactive theorem prover for first-order logic

This is an interactive theorem prover for first-order logic based on natural
deduction.
After a first-order formula is provided, an interactive prompt is opened, expecting
natural deduction rules to be applied.
The program terminates once all the goals are resolved, proving the input formula.

## Building the project

To try out the prover, you should have `Make`, `Flex`, `GNU Bison`, and `GCC` on
your system.
Then run
```
make
```
from the project root directory which will create the `build` directory.
To start the prover, run
```
./build/folip
```

## Usage

Once the prover is started, it expects a first-order formula surrounded by
double quotes.
For example:
```
Formula: "(?X. !Y. p(X, Y)) -> (!Y. ?X. p(X, Y))"
```
Then, an interactive prompt is entered, expecting one of the following commands
from the user:
```
available options:
        help                     - print this help
        exit                     - exit the interactive console
        rules                    - print all the available rules
        goals                    - print the remaining goals
        revert                   - revert to the state before the last rule was applied
        clear                    - clear the screen
        apply RULE ["TERM"]      - apply the rule RULE with an optional TERM substitution
                                   on the first goal
```

Then the full proof of the formula above can be:
```
1. :  |- (?X. !Y. p(X, Y) -> !Y. ?X. p(X, Y))
> apply impI
1. : ?X. !Y. p(X, Y) |- !Y. ?X. p(X, Y)
> apply allI
1. Y: ?X. !Y. p(X, Y) |- ?X. p(X, Y)
> apply exE
1. X Y: !Y. p(X, Y) |- ?X. p(X, Y)
> apply exI "X"
1. X Y: !Y. p(X, Y) |- p(X, Y)
> apply allE "Y"
1. X Y: p(X, Y) |- p(X, Y)
> apply assumption
All goals resolved.
Proof complete.
```
After each applied rule, the current stack of goals is displayed until it becomes
empty.
The variables before the symbol `:` represent "arbitrary variables", that is,
variables that are universally quantified on the metalogical level.

## Project structure

The project is organized as follows:
- `doc` - contains TeX sources for a paper about the project in Serbian
- `parsers` - contains `Flex` and `GNU Bison` sources for parsing the input
formulae
- `src` - contains C++ sources of the project
