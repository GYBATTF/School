package main

import (
	"fmt"
	"io/ioutil"
	"os"
)

type opcode struct {
	print func(s *state) string
	run   func(s *state)
}

func disassemble(file string, memsize, stacksize uint) (s state) {
	s = mkstate(memsize, stacksize)

	dat, err := ioutil.ReadFile(file)

	if err != nil {
		panic(err)
	}

	s.program = make([]opcode, len(dat)/4)

	for i, j, jump := 0, 0, int64(0); i <= len(dat)-4 && len(dat) != 0; i, j = i+4, j+1 {
		s.program[j], jump = decode(dat[i:i+4], j)

		if _, e := s.labels[jump]; jump != -1 && !e {
			s.labels[jump] = fmt.Sprintf("label_%d", len(s.labels))
		}
	}

	s.binary = dat

	return
}

func decode(opcode []byte, num int) (opcode, int64) {
	op, opstring := slice2op(opcode)
	for _, shift := range []uint{21, 22, 24, 26} {
		if o, j, success := parts2op(mkparts(op, shift, num)); success {
			return o, j
		}
	}
	panic(fmt.Errorf("ERROR, OP #%d %sIS NOT A VALID OPCODE", num, opstring))
}

func parts2op(p parts) (o opcode, jumpAddr int64, success bool) {
	success = true
	jumpAddr = -1
	var command string
	o.print = func(s *state) string {
		return command + "\n"
	}

	switch p.opcode {
	case opcodes["STURW"]:
		command = fmt.Sprintf("STURW X%d, [X%d, #%d]", p.rt, p.rn, p.dta)
		o.run = func(s *state) {
			stur(s, p, 32)
		}
	case opcodes["LDURSW"]:
		command = fmt.Sprintf("LDURSW X%d, [X%d, #%d]", p.rt, p.rn, p.dta)
		o.run = func(s *state) {
			ldur(s, p, 32)
		}
	case opcodes["STURB"]:
		command = fmt.Sprintf("STURB X%d, [X%d, #%d]", p.rt, p.rn, p.dta)
		o.run = func(s *state) {
			stur(s, p, 8)
		}
	case opcodes["STURH"]:
		command = fmt.Sprintf("STURH X%d, [X%d, #%d]", p.rt, p.rn, p.dta)
		o.run = func(s *state) {
			stur(s, p, 16)
		}
	case opcodes["LDURH"]:
		command = fmt.Sprintf("LDURH X%d, [X%d, #%d]", p.rt, p.rn, p.dta)
		o.run = func(s *state) {
			ldur(s, p, 16)
		}
	case opcodes["LDURB"]:
		command = fmt.Sprintf("LDURB X%d, [X%d, #%d]", p.rt, p.rn, p.dta)
		o.run = func(s *state) {
			ldur(s, p, 8)
		}
	case opcodes["U/SDIV"]:
		if p.shamt == 0x2 {
			command = fmt.Sprintf("SDIV X%d, X%d, X%d", p.rd, p.rn, p.rm)
			o.run = func(s *state) {
				s.reg[p.rd] = uint64(int64(s.reg[p.rn]) / int64(s.reg[p.rm]))
			}
		} else if p.shamt == 0x3 {
			command = fmt.Sprintf("UDIV X%d, X%d, X%d", p.rd, p.rn, p.rm)
			o.run = func(s *state) {
				s.reg[p.rd] = s.reg[p.rn] / s.reg[p.rm]
			}
		}
		success = p.shamt == 0x2 || p.shamt == 0x3
	case opcodes["MUL"]:
		command = fmt.Sprintf("MUL X%d, X%d, X%d", p.rd, p.rn, p.rm)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] * s.reg[p.rm]
		}
		// success = p.shamt == 0x1f
	case opcodes["LSR"]:
		command = fmt.Sprintf("LSR X%d, X%d, #%d", p.rd, p.rn, p.shamt)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] >> p.shamt
		}
	case opcodes["LSL"]:
		command = fmt.Sprintf("LSL X%d, X%d, #%d", p.rd, p.rn, p.shamt)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] << p.shamt
		}
	case opcodes["EOR"]:
		command = fmt.Sprintf("EOR X%d, X%d, X%d", p.rd, p.rn, p.rm)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] ^ s.reg[p.rm]
		}
	case opcodes["EORI"]:
		command = fmt.Sprintf("EORI X%d, X%d, #%d", p.rd, p.rn, p.imm)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] ^ uint64(p.imm)
		}
	case opcodes["ANDI"]:
		command = fmt.Sprintf("ANDI X%d, X%d, #%d", p.rd, p.rn, p.imm)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] & uint64(p.imm)
		}
	case opcodes["AND"]:
		command = fmt.Sprintf("AND X%d, X%d, X%d", p.rd, p.rn, p.rm)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] & s.reg[p.rm]
		}
	case opcodes["ORRI"]:
		command = fmt.Sprintf("ORRI X%d, X%d, #%d", p.rd, p.rn, p.imm)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] | uint64(p.imm)
		}
	case opcodes["ORR"]:
		command = fmt.Sprintf("ORR X%d, X%d, X%d", p.rd, p.rn, p.rm)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] | s.reg[p.rm]
		}
	case opcodes["ADD"]:
		command = fmt.Sprintf("ADD X%d, X%d, X%d", p.rd, p.rn, p.rm)
		o.run = func(s *state) {
			s.reg[p.rd] = uint64(int64(s.reg[p.rn]) + int64(s.reg[p.rm]))
		}
	case opcodes["ADDI"]:
		command = fmt.Sprintf("ADDI X%d, X%d, #%d", p.rd, p.rn, p.imm)
		o.run = func(s *state) {
			s.reg[p.rd] = s.reg[p.rn] + uint64(p.imm)
		}
	case opcodes["DUMP"]:
		command = "DUMP"
		o.run = func(s *state) {
			s.print()
		}
	case opcodes["SUBS"]:
		command = fmt.Sprintf("SUBS X%d, X%d, X%d", p.rd, p.rn, p.rm)
		o.run = func(s *state) {
			s.reg[p.rd] = uint64(int64(s.reg[p.rn]) - int64(s.reg[p.rm]))
			s.setFlags(p.rd)
		}
	case opcodes["PRNL"]:
		command = "PRNL"
		o.run = func(s *state) {
			fmt.Println()
		}
	case opcodes["SUB"]:
		command = fmt.Sprintf("SUB X%d, X%d, X%d", p.rd, p.rn, p.rm)
		o.run = func(s *state) {
			s.reg[p.rd] = uint64(int64(s.reg[p.rn]) - int64(s.reg[p.rm]))
		}
	case opcodes["PRNT"]:
		command = fmt.Sprintf("PRNT X%d", p.rd)
		o.run = func(s *state) {
			fmt.Printf("X%d: 0x%016x (%d)\n", p.rd, s.reg[p.rd], s.reg[p.rd])
		}
	case opcodes["SUBI"]:
		command = fmt.Sprintf("SUBI X%d, X%d, #%d", p.rd, p.rn, p.imm)
		o.run = func(s *state) {
			s.reg[p.rd] = uint64(int64(s.reg[p.rn]) - p.imm)
		}
	case opcodes["SUBIS"]:
		command = fmt.Sprintf("SUBIS X%d, X%d, #%d", p.rd, p.rn, p.imm)
		o.run = func(s *state) {
			s.reg[p.rd] = uint64(int64(s.reg[p.rn]) - p.imm)
			s.setFlags(p.rd)
		}
	case opcodes["LDUR"]:
		command = fmt.Sprintf("LDUR X%d, [X%d, #%d]", p.rt, p.rn, p.dta)
		o.run = func(s *state) {
			ldur(s, p, 64)
		}
	case opcodes["STUR"]:
		command = fmt.Sprintf("STUR X%d, [X%d, #%d]", p.rt, p.rn, p.dta)
		o.run = func(s *state) {
			stur(s, p, 64)
		}
	case opcodes["HALT"]:
		command = "HALT"
		o.run = func(s *state) {
			s.print()
			s.halted = true
		}
	case opcodes["BR"]:
		command = fmt.Sprintf("BR X%d", p.rn)
		o.run = func(s *state) {
			s.pc = uint(int32(s.reg[p.rn]))
			if s.pc == 0 {
				s.pc--
			}
		}
	case opcodes["CBZ"]:
		jumpAddr = p.cbaj
		o.print = func(s *state) string {
			if s.bl {
				return fmt.Sprintf("CBZ X%d, %s\n", p.rt, s.labels[jumpAddr])
			}
			return fmt.Sprintf("CBZ X%d, %d\n", p.rt, p.cba)
		}
		o.run = func(s *state) {
			if s.reg[p.rt] == 0 {
				s.pc = uint(int64(s.pc) + p.cba - 1)
			}
		}
	case opcodes["CBNZ"]:
		jumpAddr = p.cbaj
		o.print = func(s *state) string {
			if s.bl {
				return fmt.Sprintf("CBNZ X%d, %s\n", p.rt, s.labels[jumpAddr])
			}
			return fmt.Sprintf("CBNZ X%d, %d\n", p.rt, p.cba)
		}
		o.run = func(s *state) {
			if s.reg[p.rt] != 0 {
				s.pc = uint(int64(s.pc) + p.cba - 1)
			}
		}
	case opcodes["BL"]:
		jumpAddr = p.braj
		o.print = func(s *state) string {
			if s.bl {
				return fmt.Sprintf("BL %s\n", s.labels[jumpAddr])
			}
			return fmt.Sprintf("BL %d\n", p.bra)
		}
		o.run = func(s *state) {
			s.reg[30] = uint64(s.pc)
			s.pc = uint(int64(s.pc) + p.bra - 1)
		}
	case opcodes["B"]:
		jumpAddr = p.braj
		o.print = func(s *state) string {
			if s.bl {
				return fmt.Sprintf("B %s\n", s.labels[jumpAddr])
			}
			return fmt.Sprintf("B %d\n", p.bra)
		}
		o.run = func(s *state) {
			s.pc = uint(int64(s.pc) + p.bra - 1)
		}
	case opcodes["B.cond"]:
		jumpAddr = p.cbaj
		o.print = func(s *state) string {
			if s.bl {
				return fmt.Sprintf("B.%s %s\n", cond[p.rt], s.labels[jumpAddr])
			}
			return fmt.Sprintf("B.%s %d\n", cond[p.rt], p.cba)
		}
		o.run = func(s *state) {
			if s.flags[cind[p.rt]] {
				s.pc = uint(int64(s.pc) + p.cba - 1)
			}
		}
	default:
		success = false
	}
	return
}

func ldur(s *state, p parts, numBits uint8) {
	s.loads++
	s.reg[p.rt] = 0
	var bank *[]uint8
	if p.rn == 28 {
		bank = &s.stack
	} else {
		bank = &s.mem
	}
	for i, j := uint8(0), int64(s.reg[p.rn])+p.dta; i < (numBits / 8); i, j = i+1, j+1 {
		if j < 0 || j > int64(len(*bank)-1) {
			s.print()
			fmt.Printf("Address 0x%08x out of bounds\n", uint32(j))
			os.Exit(-1)
		} else {
			s.reg[p.rt] = (s.reg[p.rt] << uint(i*8)) | uint64((*bank)[j])
		}
	}
}

func stur(s *state, p parts, numBits int) {
	s.stores++
	var bank *[]uint8
	if p.rn == 28 {
		bank = &s.stack
	} else {
		bank = &s.mem
	}
	for i, j := (numBits/8)-1, int64(s.reg[p.rn])+p.dta; i >= 0; i, j = i-1, j+1 {
		if j < 0 || j > int64(len(*bank)-1) {
			s.print()
			fmt.Printf("Address 0x%08x out of bounds\n", uint32(j))
			os.Exit(-1)
		} else {
			(*bank)[j] = uint8(s.reg[p.rt]>>uint(i*8)) & 0xff
		}
	}
}

func slice2op(b []byte) (o uint32, s string) {
	for i := 0; i < len(b); i++ {
		o |= uint32(b[i]) << uint(8*(len(b)-i-1))
		for j := uint(7); j < 8; j-- {
			if j == 3 {
				s += " "
			}
			if (b[i]>>j)&0x1 == 0 {
				s += "0"
			} else {
				s += "1"
			}
		}
		s += " "
	}
	return
}
