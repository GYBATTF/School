package main

import (
	"fmt"
	"io/ioutil"
	"strconv"
	"strings"
)

const defaultMemSize uint = 4096
const defaultStackSize uint = 512

type state struct {
	labels        map[int64]string
	reg           [32]uint64
	program       []opcode
	mem, stack    []uint8
	flags         [7]bool
	binary        []byte
	out           string
	halted, l, bl bool
	execs, loads  uint
	stores, pc    uint
}

func mkstate(memsize, stacksize uint) (s state) {
	if memsize != defaultMemSize {
		memsize += 16 - (memsize % 16)
	}
	if stacksize != defaultStackSize {
		stacksize += 16 - (stacksize % 16)
	}
	s.reg[28] = uint64(stacksize)
	s.reg[29] = uint64(stacksize)
	s.mem = make([]byte, memsize)
	s.stack = make([]byte, stacksize)
	s.labels = make(map[int64]string)
	return
}

func (s state) emulate() {
	for ; !s.halted && s.pc < uint(len(s.program)); s.pc, s.execs, s.reg[31] = s.pc+1, s.execs+1, 0 {
		s.program[s.pc].run(&s)
	}
}

func (s *state) setFlags(reg uint8) {
	s.flags[0] = int(s.reg[reg]) == 0
	s.flags[1] = int(s.reg[reg]) >= 0
	s.flags[2] = int(s.reg[reg]) > 0
	s.flags[3] = int(s.reg[reg]) <= 0
	s.flags[4] = int(s.reg[reg]) < 0
	s.flags[5] = int(s.reg[reg]) != 0
}

func (s state) print() {
	out := "Registers:\n"
	out += printreg(s.reg)
	out += "\nStack:\n"
	out += hexdump(s.stack)
	out += "\nMain Memory:\n"
	out += hexdump(s.mem)
	out += "\nProgram:\n"
	out += s.decompile(true)
	out += "\nExtra:\n"
	out += fmt.Sprintf("Instructions executed: %-d\n", s.execs)
	out += fmt.Sprintf("         Loads issued: %-d\n", s.loads)
	out += fmt.Sprintf("        Stores issued: %-d\n", s.stores)

	printText(out, s.out)
}

func (s state) printBin() {
	if s.out == "" {
		psize := len(s.binary)
		psize += 16 - (psize % 16)
		p := make([]byte, psize-1)

		for i, v := range s.binary {
			p[i] = v
		}

		fmt.Println(hexdump(p))
	} else {
		err := ioutil.WriteFile(s.out, s.binary, 0644)
		if err != nil {
			fmt.Println("ERROR, CANNOT WRITE FILE")
		}
	}
}

func printText(s, out string) {
	if out == "" {
		fmt.Println(s)
	} else {
		err := ioutil.WriteFile(out, []byte(s), 0644)
		if err != nil {
			fmt.Println("ERROR, CANNOT WRITE FILE")
		}
	}
}

func (s *state) decompile(arrow bool) (out string) {
	for i, o := range s.program {
		label, exists := s.labels[int64(i)]
		if exists && s.l {
			if arrow {
				out += "   "
			}
			out += fmt.Sprintln(label + ":")
		}
		if uint(i) == s.pc && arrow {
			out += "-->"
		} else if arrow || s.l {
			out += "   "
		}
		if arrow && s.l {
			out += "   "
		}
		out += num2name(o.print(s))
	}

	out = strings.TrimRight(out, "\n")
	return
}

func num2name(asm string) string {
	asm = strings.Replace(asm, "X16", "IP0", -1)
	asm = strings.Replace(asm, "X17", "IP1", -1)
	asm = strings.Replace(asm, "X28", "SP", -1)
	asm = strings.Replace(asm, "X29", "FP", -1)
	asm = strings.Replace(asm, "X30", "LR", -1)
	asm = strings.Replace(asm, "X31", "XZR", -1)
	return asm
}

func printreg(r [32]uint64) (out string) {
	for i := range r {
		pad, label := "", "     "
		switch {
		case i < 10:
			pad = " "
		case i == 16:
			label = "(IP0)"
		case i == 17:
			label = "(IP1)"
		case i == 28:
			label = "(SP) "
		case i == 29:
			label = "(FP) "
		case i == 30:
			label = "(LR) "
		case i == 31:
			label = "(XZR)"
		}
		out += fmt.Sprintf("%s X%-d:%s 0x%016x (%d)\n", label, i, pad, r[i], r[i])
	}
	out = strings.TrimSpace(out)
	return
}

func hexdump(mem []uint8) (out string) {
	for i := 0; i < len(mem)-(len(mem)%16); i += 16 {
		out += fmt.Sprintf(
			"%08x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
			int32(i), mem[i+0], mem[i+1], mem[i+2], mem[i+3], mem[i+4], mem[i+5], mem[i+6],
			mem[i+7], mem[i+8], mem[i+9], mem[i+10], mem[i+11], mem[i+12], mem[i+13], mem[i+14], mem[i+15],
			printableChar(mem[i+0]), printableChar(mem[i+1]), printableChar(mem[i+2]), printableChar(mem[i+3]),
			printableChar(mem[i+4]), printableChar(mem[i+5]), printableChar(mem[i+6]), printableChar(mem[i+7]),
			printableChar(mem[i+8]), printableChar(mem[i+9]), printableChar(mem[i+10]), printableChar(mem[i+11]),
			printableChar(mem[i+12]), printableChar(mem[i+13]), printableChar(mem[i+14]), printableChar(mem[i+15]))
	}
	out += fmt.Sprintf("%08x\n", int32(len(mem)))
	out = strings.TrimSpace(out)
	return
}

func printableChar(c uint8) uint8 {
	if strconv.IsPrint(int32(c)) {
		return c
	}
	return '.'
}
