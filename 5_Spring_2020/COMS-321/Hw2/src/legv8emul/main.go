package main

import (
	"flag"
)

func main() {
	asm := flag.Bool("a", false, "Compiles a legv8 assembly file into binary")
	mems := flag.Uint("m", defaultMemSize, "Specify memory size in bytes, will round up to a multiple of 16")
	stacks := flag.Uint("s", defaultStackSize, "Specify stack size in bytes, will round up to a multiple of 16")
	decompile := flag.Bool("d", false, "Prints the decompiled program with branches printed as offsets and exits")
	labels := flag.Bool("l", false, "Prints the decompiled program with labels and exits")
	branchLabels := flag.Bool("r", false, "Prints the decompiled program with branches labeled and exits")
	bin := flag.Bool("b", false, "Interprets the input file as a binary")
	out := flag.String("o", "", "Specifies the file where output is to be printed (default stdout)")
	file := flag.String("f", "", "Sepcifies the binary file to run ***REQUIRED***")
	flag.Parse()

	var s state
	if *file == "" {
		flag.PrintDefaults()
	} else if *bin {
		s = disassemble(*file, *mems, *stacks)
	} else {
		s = assemble(*file, *mems, *stacks)
	}

	s.out = *out
	s.l = *labels
	s.bl = *branchLabels

	switch {
	case *decompile:
		printText(s.decompile(false), s.out)
	case *asm:
		s.printBin()
	default:
		s.emulate()
	}
}
