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



> Listez l’ensemble des tests effectués sur votre programme (en plus des units tests)



> Avez-vous détecté des cas où la correction par distance ne fonctionnait pas (même avec une distance élevée) ?

Dans certains cas, les poids que nous utilisons pour la distance de Damerau-Levenshtein ne sont pas les mieux adaptés, en effet, la transposition de deux lettres adjacentes est plus courante

> Quelle est la structure de données que vous avez implémentée dans votre projet, pourquoi ?



> Proposez un réglage automatique de la distance pour un programme qui prend juste une chaîne de caractères en entrée, donner le processus d’évaluation ainsi que les résultats.

Prenons par exemple le mot `shushi` en entrée. Dans un premier temps on fait un `approx 0 shushi` pour vérifier si le mot existe dans notre dictionnaire. Si nous n'avons pas de résultats alors un `approx 1 shushi` nous permet de trouver le mot `sushi` qui, dans ce cas, est la bonne ortographe. Si nous n'avions pas trouvé avec un `approx 1` alors nous aurions testé avec un `approx 2` et ainsi de suite jusqu'a une distance maximum. Pour calculer la distance maximum nous pourions par example utiliser la longueur du mot divisée par 4 ce qui revient à permettre à l'utilisateur de faire une faute tous les 4 lettres.

> Comment comptez vous améliorer les performances de votre programme

Nous pourrions compresser beaucoup plus notre arbre lors de la sérialisation. En effet nous avons un grand nombre de 0 inutiles dû au fait que nous stockons nos fréquences, nos offsets et nos longueurs sur des entiers non signés, or nous n'utilisons pas forcément l'intégralité des bits de chacun de ces champs. Nous pourrions donc implémenter une sérialisation et une désérialisation pouvant compresser les entiers non signés en les terminants par un marqueur spécial.

> Que manque-t-il à votre correcteur orthographique pour qu’il soit à l’état de l’art ?

Nous ne prenons pas en compte les fautes de syntax et de grammaire, en effet nous pourrions prendre une phrase en entrèe et faire une analyse syntaxique pour en détecter les fautes d'accord ainsi que dans l'ordre des mots. Nous ne prenons pas non plus en compte la position des charactères sur le clavier. Or l'utilisateur étant humain, il se pourrait qu'il écrive le mot `qyuq` en voulant écrire le mot `suis` (en ayant les mains décallées d'une touche sur un clavier azerty par exemple).
