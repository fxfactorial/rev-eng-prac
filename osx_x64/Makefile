prog := rev_eng
cc := clang
flags := -ObjC -framework Foundation -framework CFNetwork
sdk := $(shell xcrun --sdk macosx --show-sdk-path)

$(prog):$(prog).c
	$(cc) $(flags) -isysroot $(sdk) -S $<
	$(cc) $(flags) -isysroot $(sdk) $< -o $@

.PHONY:clean

clean:
	rm -f $(prog) *.{o,out,s}
