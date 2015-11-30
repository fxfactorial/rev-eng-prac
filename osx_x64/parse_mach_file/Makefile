prog := parse
sdk := -isysroot $(shell xcrun --sdk macosx --show-sdk-path)
frameworks := -framework Foundation
extras := -ObjC
# warn := -Weverything

ocaml_pkgs := -package cstruct,cstruct.syntax
ocaml_syntax := -syntax camlp4
ocaml_prg := caml_parser_mach
ocaml_cc := ocamlfind ocamlopt

$(prog): $(prog).c
	clang $(extras) $(sdk) $(frameworks) $(warn) $< -o $@

$(ocaml_prg):$(ocaml_prg).ml
	$(ocaml_cc) $(ocaml_pkgs) $(ocaml_syntax) -linkpkg $< -o $@

clean:
	rm -f $(prog) $(ocaml_prg) *.{cmi,cmt,cmx,o,out}
