#ifndef FOL_HPP
#define FOL_HPP

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <variant>
#include <vector>

// Definisemo strukture za opisivanje termova
// Term moze biti
// 1. Promenljiva (npr. x)
// 2. Funkcijski term (npr. f(x, y), f(g(x), h(y, z)), itd.)
//    - Funkcijski term se sastoji od funkcijskog simbola (npr. f) i argumenata koji su termovi

struct Variable;
struct Function;
using Term = std::variant<Variable, Function>;
using TermPtr = std::shared_ptr<Term>;

struct Variable { std::string name; };
struct Function {
    std::string symbol;
    std::vector<TermPtr> args;
};

// Definisemo strukture za opisivanje formula logike prvog reda
// Formula moze biti
// 1. Atomicna formula (npr. P(x, y), Q(f(x), g(y)), itd.)
//    - Atomicna formula se sastoji od relacijskog simbola (npr. P) i argumenata koji su termovi
// 2. Negacija potformule
// 3. Binarna formula
// 4. Kvantifikator (npr. (A x) F(x) gde je F potformula)

struct Atom;
struct Not;
struct Binary;
struct Quantifier;
using Formula = std::variant<Atom, Not, Binary, Quantifier>;
using FormulaPtr = std::shared_ptr<Formula>;

struct Atom {
    std::string symbol;
    std::vector<TermPtr> args;
};
struct Not { FormulaPtr subformula; };
struct Binary {
    enum Type { And, Or, Impl, Eq } type;
    FormulaPtr l, r;
};
struct Quantifier {
    enum Type { All, Exists } type;
    std::string var;
    FormulaPtr subformula;
};

// Pomocne funkcije

// ptr Pravi pokazivac na formulu/term
TermPtr ptr(const Term& term);
FormulaPtr ptr(const Formula& formula);

FormulaPtr falseConst();
FormulaPtr trueConst();

// is Proverava da li je formula/term odredjenog tipa
template<typename T> bool is(const TermPtr& term);
template<typename T> bool is(const FormulaPtr& formula);

// as Pretvara formulu/term u odredjeni tip
template<typename T> T as(const TermPtr& term);
template<typename T> T as(const FormulaPtr& formula);


// Definisemo strukture za opisivanje signature i interpretacije
// Signatura je skup funkcijskih i relacijskih simbola
// Svakom simbolu pridruzujemo arnost (broj argumenata)
// Npr. za simbol f arnosti 2, f(x, y) je ispravan funkcijski term
struct Signature {
    std::map<std::string, unsigned> functions;
    std::map<std::string, unsigned> relations;
};

// Interpretacija (ili L-struktura za signaturu L) se sastoji iz
// - Domena ili univerzuma (skupa elemenata, npr. skup celih brojeva)
// - Funkcija i relacija nad tim domenom koje odgovaraju simbolima iz signature
using Domain = std::set<unsigned>;
using DomainFunction = std::function<unsigned(const std::vector<unsigned>&)>;
using DomainRelation = std::function<bool(const std::vector<unsigned>&)>;

struct LStructure {
    Signature signature;
    Domain domain;
    std::map<std::string, DomainFunction> functions;
    std::map<std::string, DomainRelation> relations;
};

// Provera signature formule
bool checkSignature(const TermPtr& term, const Signature& s);
bool checkSignature(const FormulaPtr& formula, const Signature& s);

// Evaluacija formule
// Prosledjujemo formulu, L-strukturu i valuaciju promenljivih
using Valuation = std::map<std::string, unsigned>;
unsigned evaluate(const TermPtr& term, const LStructure& s, const Valuation& val);
bool evaluate(const FormulaPtr& formula, const LStructure& s, const Valuation& val);

// Dohvatanje svih promenljivih koje se pojavljuju u formuli
// Postoje dve varijante provere koje zelimo da implementiramo
// 1. Dohvatanje svih promenljivih koje se pojavljuju u formuli
// 2. Dohvatanje svih *slobodnih* promenljivih koje se pojavljuju u formuli
// Pojavljivanje promenljive u formuli je slobodno ako se ne nalazi unutar kvantifikatora
// Npr. u formuli "Ex Q(x, y)" promenljiva y je slobodna, a promenljiva x nije
// Npr. u formuli "P(x) & Ex Q(x, y)" promenljive x i y su slobodne
//   - Pojavljivanje promenljive x u potformuli "Ex Q(x, y)" je vezano
//   - Pojavljivanje promenljive x u potformuli "P(x)" je slobodno
//   - Prema tome, promenljiva x je slobodna u formuli "P(x) & Ex Q(x, y)"
//   - Ovo znaci da vrednost formule zavisi od vrednosti promenljive x u valuaciji
//
// Ovo nam je neophodno prilikom supstitucije
void getVariables(const TermPtr& term, std::set<std::string>& vars);
void getVariables(const FormulaPtr& formula, std::set<std::string>& vars, bool includeBound);
bool containsVariable(const TermPtr& term, const std::string& var);
bool containsVariable(const FormulaPtr& formula, const std::string& var, bool includeBound);

// Odredjivanje promenljive koja se ne pojavljuje ni u termu ni u formuli
std::string uniqueVar(const FormulaPtr& formula, const TermPtr& term);

// Smena promenljive termom
// U slucaju kvantifikatora:
// 1. Ako je promenljiva kvantifikatora promenljiva koju zelimo da zamenimo, ne menjamo nista
//    - To je zato sto je promenljiva kvantifikatora vezana i ne zavisi od vrednosti promenljive u valuaciji
//    - Mozemo se uveriti u to time sto ako promenimo naziv vezane promenljive, znacenje formule se ne menja
//    - Npr. ako treba da primenimo smenu [x->f(y)] na formulu Ax P(x), rezultat treba da bude Ax P(x)
//    - Formule Ax P(x) i Az P(z) su ekvivalentne, a primena supstitucije [x->f(y)] na formulu Az P(z) nema efekta
// 2. Ako se promenljiva kvantifikatora pojavljuje u termu supstitucije, moramo da vrsimo preimenovanje
//    - Npr. ako bismo direktno primenili [x->f(y)] na formulu Ay P(x, y), rezultat bi bio Ay P(f(y), y)
//    - To je pogresno zato sto f(y) postaje vezano za kvantifikator Ay sto menja znacenje formule
//    - Pravilno bi bilo da prvo preimenujemo neku od ovih promenljivih, npr. Az P(x, z), i onda primenimo smenu
//    - Rezultat je onda Az P(f(y), z), sto je pravilno jer ne menja znacenje promenljive y (i samim tim formule)
// Ovo ce nam biti neophodno prilikom metode rezolucije
TermPtr substitute(const TermPtr& term, const std::string& var, const TermPtr& subterm);
FormulaPtr substitute(const FormulaPtr& formula, const std::string& var, const TermPtr& term);

// Provera formula na jednakost
bool operator==(const TermPtr &s, const TermPtr &t);
bool operator==(const FormulaPtr &p, const FormulaPtr &q);

bool operator!=(const TermPtr &s, const TermPtr &t);
bool operator!=(const FormulaPtr &p, const FormulaPtr &q);

// Ispis formule
void print(std::ostream &os, const TermPtr& term);
void print(std::ostream &os, const FormulaPtr& formula);

std::ostream &operator<<(std::ostream &os, const FormulaPtr &formula);

#endif //FOL_HPP
