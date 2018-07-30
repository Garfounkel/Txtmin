function foo() {
  ./TextMiningCompiler $1 output.bin --dot_output output.dot && dot -Tpng output.dot -o output.png && feh output.png
}
