# An interactive theorem prover for first-order logic

## Comments

- Equality and its negation are removed from the original parser for first-order formulas.
- `notE` rule is of the following form: `G |- A` then `G, ~ A |- false`
- `impE` rule is of the following form: `G |- A` then `G, A -> B |- B`
- `conjunct1` and `conjunct2` rules just replace a conjunction on the left hand side with one of the conjuncts
- `disjE` rule is of the following form: `G, A |- C` and `G, B |- C`, then `G, A | B |- C`
- `iffI` rule is of the following form `G |- A -> B` and `G |- B -> A`, then `G |- A <-> B`
- `iffE` rule just replaces an equivalence on the left hand side with two implications
- *Isabelle* renames variables when applying rules `allE` and `exI`, and we choose to mimic that behavior instead of reporting an error if the substitution term contains bound variables
