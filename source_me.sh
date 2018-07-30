function foo() {
  ./TextMiningCompiler $1 output.dot && dot -Tpng output.dot -o output.png && feh output.png
}
