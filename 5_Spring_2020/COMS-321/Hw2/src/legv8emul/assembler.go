package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func assemble(file string, memsize, stacksize uint) (s state) {
	s = mkstate(memsize, stacksize)
	lines := make([]string, 0)

	f, err := os.Open(file)
	inerr(err)
	defer f.Close()

	scanner := bufio.NewScanner(f)
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		comment := strings.LastIndex(line, "//")
		if line == "" || comment == 0 {
			continue
		} else if comment != -1 {
			line = strings.TrimSpace(string([]rune(line)[:comment]))
		}

		if strings.Contains(line, ":") {
			s.labels[int64(len(lines))] = strings.Replace(line, ":", "", -1)
			continue
		}

		line = strings.Replace(line, "IP0", "X16", -1)
		line = strings.Replace(line, "IP1", "X17", -1)
		line = strings.Replace(line, "SP", "X28", -1)
		line = strings.Replace(line, "FP", "X29", -1)
		line = strings.Replace(line, "LR", "X30", -1)
		line = strings.Replace(line, "XZR", "X31", -1)
		lines = append(lines, line)
	}
	inerr(scanner.Err())

	s.program = make([]opcode, len(lines))
	s.binary = make([]byte, 0)

	for i, e := range lines {
		oparr, opcode := asm2op(e, int64(i), s.labels)
		s.program[i] = opcode
		s.binary = append(s.binary, oparr...)
	}

	return
}

func inerr(err error) {
	if err != nil {
		fmt.Println("ERROR, COULD NOT PROPERLY READ INPUT")
		os.Exit(-1)
	}
}

func asm2op(asm string, num int64, labels map[int64]string) ([]byte, opcode) {
	labelLocs := make(map[string]int64)
	for k, v := range labels {
		labelLocs[v] = k
	}

	var code uint32
	switch match, parts := strings.ToUpper(asm), new(parts); true {
	case strings.Contains(match, "B "):
		asm = strings.TrimSpace(string([]rune(asm)[1:]))
		parts.opcode = opcodes["B"]
		parts.bra = labelLocs[asm] - num
		parts.braj = labelLocs[asm]
		code = parts.b()
	case strings.Contains(match, "BL "):
		asm = strings.TrimSpace(string([]rune(asm)[2:]))
		parts.opcode = opcodes["BL"]
		parts.bra = labelLocs[asm] - num
		parts.braj = labelLocs[asm]
		code = parts.b()
	case strings.Contains(match, "B."):
		asm = strings.TrimSpace(string([]rune(asm)[2:]))
		parts.opcode = opcodes["B.cond"]
		var condition string
		var label string
		fmt.Sscanf(asm, "%s %s", &condition, &label)
		parts.cba = labelLocs[label] - num
		parts.cbaj = labelLocs[label]
		parts.rt = 0x1f
		for i, e := range cond {
			if e == condition {
				parts.rt = uint8(i)
			}
		}
		code = parts.cb()
	case strings.Contains(match, "CBZ "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["CBZ"]
		var label string
		fmt.Sscanf(asm, "X%d, %s", &parts.rt, &label)
		parts.cba = labelLocs[label] - num
		parts.cbaj = labelLocs[label]
		code = parts.cb()
	case strings.Contains(match, "CBNZ "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["CBNZ"]
		var label string
		fmt.Sscanf(asm, "X%d, %s", &parts.rt, &label)
		parts.cba = labelLocs[label] - num
		parts.cbaj = labelLocs[label]
		code = parts.cb()
	case strings.Contains(match, "STURH "):
		asm = strings.TrimSpace(string([]rune(asm)[5:]))
		parts.opcode = opcodes["STURH"]
		fmt.Sscanf(asm, "X%d, [X%d, #%d]", &parts.rt, &parts.rn, &parts.dta)
		code = parts.d()
	case strings.Contains(match, "STUR "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["STUR"]
		fmt.Sscanf(asm, "X%d, [X%d, #%d]", &parts.rt, &parts.rn, &parts.dta)
		code = parts.d()
	case strings.Contains(match, "LDURB "):
		asm = strings.TrimSpace(string([]rune(asm)[5:]))
		parts.opcode = opcodes["LDURB"]
		fmt.Sscanf(asm, "X%d, [X%d, #%d]", &parts.rt, &parts.rn, &parts.dta)
		code = parts.d()
	case strings.Contains(match, "STURB "):
		asm = strings.TrimSpace(string([]rune(asm)[5:]))
		parts.opcode = opcodes["STURB"]
		fmt.Sscanf(asm, "X%d, [X%d, #%d]", &parts.rt, &parts.rn, &parts.dta)
		code = parts.d()
	case strings.Contains(match, "LDURSW "):
		asm = strings.TrimSpace(string([]rune(asm)[6:]))
		parts.opcode = opcodes["LDURSW"]
		fmt.Sscanf(asm, "X%d, [X%d, #%d]", &parts.rt, &parts.rn, &parts.dta)
		code = parts.d()
	case strings.Contains(match, "LDURH "):
		asm = strings.TrimSpace(string([]rune(asm)[5:]))
		parts.opcode = opcodes["LDURH"]
		fmt.Sscanf(asm, "X%d, [X%d, #%d]", &parts.rt, &parts.rn, &parts.dta)
		code = parts.d()
	case strings.Contains(match, "STURW "):
		asm = strings.TrimSpace(string([]rune(asm)[5:]))
		parts.opcode = opcodes["STURW"]
		fmt.Sscanf(asm, "X%d, [X%d, #%d]", &parts.rt, &parts.rn, &parts.dta)
		code = parts.d()
	case strings.Contains(match, "LDUR "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["LDUR"]
		fmt.Sscanf(asm, "X%d, [X%d, #%d]", &parts.rt, &parts.rn, &parts.dta)
		code = parts.d()
	case strings.Contains(match, "ADDI "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["ADDI"]
		fmt.Sscanf(asm, "X%d, X%d, #%d", &parts.rd, &parts.rn, &parts.imm)
		code = parts.i()
	case strings.Contains(match, "SUBIS "):
		asm = strings.TrimSpace(string([]rune(asm)[5:]))
		parts.opcode = opcodes["SUBIS"]
		fmt.Sscanf(asm, "X%d, X%d, #%d", &parts.rd, &parts.rn, &parts.imm)
		code = parts.i()
	case strings.Contains(match, "EORI "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["EORI"]
		fmt.Sscanf(asm, "X%d, X%d, #%d", &parts.rd, &parts.rn, &parts.imm)
		code = parts.i()
	case strings.Contains(match, "ORRI "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["ORRI"]
		fmt.Sscanf(asm, "X%d, X%d, #%d", &parts.rd, &parts.rn, &parts.imm)
		code = parts.i()
	case strings.Contains(match, "ANDI "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["ANDI"]
		fmt.Sscanf(asm, "X%d, X%d, #%d", &parts.rd, &parts.rn, &parts.imm)
		code = parts.i()
	case strings.Contains(match, "SUBI "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["SUBI"]
		fmt.Sscanf(asm, "X%d, X%d, #%d", &parts.rd, &parts.rn, &parts.imm)
		code = parts.i()
	case strings.Contains(match, "BR "):
		asm = strings.TrimSpace(string([]rune(asm)[2:]))
		parts.opcode = opcodes["BR"]
		fmt.Sscanf(asm, "X%d", &parts.rn)
		code = parts.r()
	case strings.Contains(match, "HALT"):
		parts.opcode = opcodes["HALT"]
		code = parts.r()
	case strings.Contains(match, "DUMP"):
		parts.opcode = opcodes["DUMP"]
		code = parts.r()
	case strings.Contains(match, "PRNL"):
		parts.opcode = opcodes["PRNL"]
		code = parts.r()
	case strings.Contains(match, "ADD "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["ADD"]
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "SMULH "):
		asm = strings.TrimSpace(string([]rune(asm)[5:]))
		parts.opcode = opcodes["SMULH"]
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "UMULH "):
		asm = strings.TrimSpace(string([]rune(asm)[5:]))
		parts.opcode = opcodes["UMULH"]
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "MUL "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["MUL"]
		parts.shamt = 0x1f
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "AND "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["AND"]
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "ORR "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["ORR"]
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "SUBS "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["SUBS"]
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "SUB "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["SUB"]
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "EOR "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["EOR"]
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "SDIV "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["S/UDIV"]
		parts.shamt = 0x2
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "UDIV "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["S/UDIV"]
		parts.shamt = 0x3
		fmt.Sscanf(asm, "X%d, X%d, X%d", &parts.rd, &parts.rn, &parts.rm)
		code = parts.r()
	case strings.Contains(match, "PRNT "):
		asm = strings.TrimSpace(string([]rune(asm)[4:]))
		parts.opcode = opcodes["PRNT"]
		fmt.Sscanf(asm, "X%d", &parts.rd)
		code = parts.r()
	case strings.Contains(match, "LSR "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["LSR"]
		fmt.Sscanf(asm, "X%d, X%d, #%d", &parts.rd, &parts.rn, &parts.shamt)
		code = parts.r()
	case strings.Contains(match, "LSL "):
		asm = strings.TrimSpace(string([]rune(asm)[3:]))
		parts.opcode = opcodes["LSL"]
		fmt.Sscanf(asm, "X%d, X%d, #%d", &parts.rd, &parts.rn, &parts.shamt)
		code = parts.r()
	default:
		panic(fmt.Errorf("ERROR, OPCODE \"%s\" ON LINE %d NOT VALID", asm, num))
	}

	oparr := make([]byte, 4)
	for i, j := 0, 3; i < 4; i, j = i+1, j-1 {
		oparr[i] = byte((code >> uint(j*8)) & 0xff)
	}

	opcode, _ := decode(oparr, int(num))
	return oparr, opcode
}
