# Txtmin
Text mining - Fast and stable orthographic corrections.
The project was designed with performances in mind, we used a compact Patricia Trie to facilitate the research of misspelled words with a dynamic Damerau-Levenshtein distance.

## Compilation
Produces two binaries `TextMiningCompiler` and `TextMiningApp` in `_build`:
```
mkdir _build
cd _build
cmake ..
make
```

* `TextMiningCompiler` is used to compile the Patricia Trie from a file containing a list of words (each word on it's own line followed by it's frequency) into a serialized binary.
* `TextMiningApp` is used to search for misspelled words (queries taken from stdin) in the compiled dictionary. Queries must be of the form `approx n word` where `n` is the maximum search distance and `word` the searched word.

## Usage
* To compile a dictionary into a serialized Patricia Trie (produces `dict.bin`):
```
./TextMiningCompiler words.txt dict.bin
```

* To search for a word at a distance 0 in the compiled dictionary `dict.bin`:
```
echo "approx 0 word" | ./TextMiningApp dict.bin
```

* To draw a dot file representing the Patricia Trie (produces a png image `output.png` that you can display with your favorite viewer):
```
./TextMiningCompiler foo.txt dict.bin --dot_output input.dot
dot -Tpng input.dot -o output.png
```

## Example usage
```
./TextMiningCompiler words.txt dict.bin
./TextMiningApp dict.bin
approx 0 word
[{"word":"word","freq":14411625,"distance":0}]
approx 1 coupedumonde
[{"word":"coupedumonde","freq":3002,"distance":0},{"word":"croupedumonde","freq":462,"distance":1}]
```

## FAQ (in French)
> Decrivez les choix de design de votre programme:

test

> Listez l’ensemble des tests effectués sur votre programme (en plus des units tests)
Avez-vous détecté des cas où la correction par distance ne fonctionnait pas (même avec une distance élevée) ?

> Quelle est la structure de données que vous avez implémentée dans votre projet, pourquoi ?

> Proposez un réglage automatique de la distance pour un programme qui prend juste une chaîne de caractères en entrée, donner le processus d’évaluation ainsi que les résultats.

> Comment comptez vous améliorer les performances de votre programme

Nous pourrions compresser beaucoup notre arbre lors de la sérialisation

> Que manque-t-il à votre correcteur orthographique pour qu’il soit à l’état de l’art ?

Nous ne prenons pas en compte les fautes de syntax et de grammaire, en effet nous pourrions prendre une phrase en entrèe et faire une analyse syntaxique pour en détecter les fautes d'accord ainsi que dans l'ordre des mots. Nous ne prenons pas non plus en compte la position des charactères sur le clavier. Or l'utilisateur étant humain, il se pourrait qu'il écrive le mot `qyuq` en voulant écrire le mot `suis` (en ayant les mains décallées d'un caractère sur un clavier azerty par exemple).
