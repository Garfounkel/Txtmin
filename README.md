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

## Test-suite
```
mkdir _build && cd _build && cmake .. && make
./TextMiningCompiler words.txt dict.bin
cd ../tests
python app-test-suite.py App/ [REF_APP] [REF_BIN] ../_build/TextMiningApp ../_build/dict.bin
```


## FAQ (in French)
> Décrivez les choix de design de votre programme:

Nous utilisons un Patricia Trie pour stocker les mots et leurs fréquences. Le
Patricia Trie est plus performant qu'un trie et offre les mêmes avantages (cf
question 4). Nous avons choisi d'exporter la partie concernant le stockage des
bords du trie dans des classes séparées afin d'éviter d'implémenter séparemment
une version dynamique et une version statique. En effet, le compilateur doit
lire une liste de mots, construire le trie associé et le sérialiser sur le
disque, alors que l'app ne doit que lire un trie sérialisé pour comparer des
mots via une distance de Damerau-Levenshtein. Autrement dit, l'app charge le
trie en lecture seule, ce qui peut être utilisé à des fins d'optimisation.
Typiquement, nous utilisons le syscall `mmap` lorsqu'on doit charger une grosse
chaîne de caractères dans un fichier dans l'application, alors que cette
approche est moins pratique lorsqu'on construit un trie à partir d'une liste
de mots. Concrètement, cela signifie que l'on peut choisir si on veut créer
un trie dynamique, stockant le contenu des bords dans une string modifiable,
ou si on veut créer un trie statique, lisant le contenu des bords dans une
région "mmappée" de la mémoire, et ce grâce au paramètre de template du trie.
Le code est donc quasiment le même entre le trie statique et dynamique, les
quelques différences étant gérées grâce à un "type trait" et des classes de
stockage à part (une pour chaque type). Cela implique que tout ce qui fonctionne
sur un trie statique, fonctionne aussi sur un trie dynamique et nous permet donc
de proposer à l'application de se retrancher sur un trie dynamique si l'appel
système à `mmap` échoue (ce qui peut arriver). Cela simplifie aussi beaucoup
la sérialisation et déserialisation puisqu'elles correspondent au même code.

En somme, il n'y a qu'un seul type de trie, le stockage des bords peut être
dynamique ou statique, et le trie peut s'adapter grâce à un peu de métaprogrammation.
Ce système nous offre une certaine flexibilité agréable sans perte en performances.


> Listez l’ensemble des tests effectués sur votre programme (en plus des units tests)

Nous avons utilisé une test-suite composé d'un script en python qui compare la sortie standard de la ref avec celle de notre programme pour un certain nombre de queries.
Nous avons également écrit une fonction permettant d'écrire notre Patricia Trie au format dot pour pouvoir le visualiser sous forme d'image. Cela nous a été très utile pour débugger la création de notre Patricia.

> Avez-vous détecté des cas où la correction par distance ne fonctionnait pas (même avec une distance élevée) ?

Dans certains cas, les poids que nous utilisons pour la distance de Damerau-Levenshtein ne sont pas les mieux adaptés, en effet, la transposition de deux lettres adjacentes est plus courante que la suppression. Par example le mot `tets` veut probablement dire `test` et pas `tes`.

> Quelle est la structure de données que vous avez implémentée dans votre projet, pourquoi ?

Nous avons d'abord implémenté un simple Trie qui permet une recherche très rapide dans du texte étant donné que notre but premier était d'obtenir des bonnes performances sur la correction orthographique. Nous nous sommes assez vite rendu compte que le Trie nous ne permettrait pas de répondre à la contrainte de mémoire qui était de faire tourner nous programme sur une machine avec 512Mb de RAM. Nous avons donc implémentés un Patricia Trie qui a pour but de réduire considèrablement la taille de notre arbre en compressant les branches dont chaque noeuds possède un seul fils.

> Proposez un réglage automatique de la distance pour un programme qui prend juste une chaîne de caractères en entrée, donner le processus d’évaluation ainsi que les résultats.

Prenons par exemple le mot `shushi` en entrée. Dans un premier temps on fait un `approx 0 shushi` pour vérifier si le mot existe dans notre dictionnaire. Si nous n'avons pas de résultats alors un `approx 1 shushi` nous permet de trouver le mot `sushi` qui, dans ce cas, est la bonne ortographe. Si nous n'avions pas trouvé avec un `approx 1` alors nous aurions testé avec un `approx 2` et ainsi de suite jusqu'a une distance maximum. Pour calculer la distance maximum nous pourions par example utiliser la longueur du mot divisée par 4 ce qui revient à permettre à l'utilisateur de faire une faute tous les 4 lettres.

> Comment comptez vous améliorer les performances de votre programme

Nous pourrions compresser beaucoup plus notre arbre lors de la sérialisation. En effet nous avons un grand nombre de 0 inutiles dû au fait que nous stockons nos fréquences, nos offsets et nos longueurs sur des entiers non signés, or nous n'utilisons pas forcément l'intégralité des bits de chacun de ces champs. Nous pourrions donc implémenter une sérialisation et une désérialisation pouvant compresser les entiers non signés en les terminants par un marqueur spécial.

> Que manque-t-il à votre correcteur orthographique pour qu’il soit à l’état de l’art ?

Nous ne prenons pas en compte les fautes de syntaxe et de grammaire, en effet nous pourrions prendre une phrase en entrèe et faire une analyse syntaxique pour en détecter les fautes d'accord ainsi que dans l'ordre des mots. Nous ne prenons pas non plus en compte la position des charactères sur le clavier. Or l'utilisateur étant humain, il se pourrait qu'il écrive le mot `qyuq` en voulant écrire le mot `suis` (en ayant les mains décallées d'une touche sur un clavier azerty par exemple).
