prog := rev_eng_ios
cc := clang
arch := -arch armv7
flags := -ObjC -framework Foundation -framework CFNetwork -Wl,-no_pie
sdk := $(shell xcrun --sdk iphoneos --show-sdk-path)

$(prog):$(prog).c
	$(cc) $(arch) $(flags) -isysroot $(sdk) -S $<
	$(cc) $(arch) $(flags) -isysroot $(sdk) $< -o $@
	scp $(prog) ipad:~/

.PHONY:clean

clean:
	rm -f $(prog) *.{o,out,s}
