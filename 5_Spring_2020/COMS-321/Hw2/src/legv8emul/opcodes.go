package main

var (
	opcodes = map[string]uint32{
		"EORI": 0x348, "BR": 0x6b0, "MUL": 0x4d8, "B": 0x5, "BL": 0x25, "ORRI": 0x2c8,
		"SMULH": 0x4da, "UMULH": 0x4de, "STURW": 0x5c0, "AND": 0x450, "STURB": 0x1c0,
		"LSR": 0x69a, "ANDI": 0x490, "PRNT": 0x7fd, "LDURSW": 0x5c4, "U/SDIV": 0x4d6,
		"STURH": 0x3c0, "SUBIS": 0x3c4, "CBNZ": 0xb5, "ADDI": 0x244, "LDUR": 0x7c2,
		"B.cond": 0x54, "STUR": 0x7c0, "CBZ": 0xb4, "LDURH": 0x3c2, "HALT": 0x7ff,
		"ORR": 0x550, "ADD": 0x458, "SUBI": 0x344, "DUMP": 0x7fe, "SUBS": 0x758,
		"SUB": 0x658, "LSL": 0x69b, "PRNL": 0x7fc, "LDURB": 0x1c2, "EOR": 0x650}

	cond = [...]string{"EQ", "NE", "HS", "LO", "MI", "PL", "VS", "VC", "HI", "LS", "GE", "LT", "GT", "LE"}
	cind = [...]int{0, 5, 1, 4, 4, 1, 6, 6, 2, 3, 1, 4, 2, 3}
)

type parts struct {
	opcode         uint32
	rd, rn, rm, rt uint8
	shamt, op      uint8
	imm, dta       int64
	cba, cbaj      int64
	bra, braj      int64
}

func mkparts(op uint32, shift uint, num int) (p parts) {
	p.opcode = op >> shift
	p.rd, p.rm, p.rn, p.rt, p.shamt = sam(op, 0, 0x1f), sam(op, 16, 0x1f), sam(op, 5, 0x1f), sam(op, 0, 0x1f), sam(op, 10, 0x3f)
	p.op, p.imm, p.dta, p.bra, p.cba = sam(op, 10, 0x3), smae(op, 10, 12), smae(op, 12, 9), smae(op, 0, 25), smae(op, 5, 19)
	p.braj, p.cbaj = int64(num)+p.bra, int64(num)+p.cba
	return
}

func (p parts) r() (op uint32) {
	op |= p.opcode << 21
	op |= uint32(p.rm&0x1f) << 16
	op |= uint32(p.shamt&0x3f) << 10
	op |= uint32(p.rn&0x1f) << 5
	op |= uint32(p.rd & 0x1f)
	return
}

func (p parts) i() (op uint32) {
	op |= p.opcode << 22
	op |= uint32(p.imm&0xfff) << 10
	op |= uint32(p.rn&0x1f) << 5
	op |= uint32(p.rd & 0x1f)
	return
}

func (p parts) d() (op uint32) {
	op |= p.opcode << 21
	op |= uint32(p.dta&0x1ff) << 12
	op |= uint32(p.op&0x3) << 10
	op |= uint32(p.rn&0x1f) << 5
	op |= uint32(p.rt & 0x1f)
	return
}

func (p parts) b() (op uint32) {
	op |= p.opcode << 26
	op |= uint32(p.bra & 0x3ffffff)
	return
}

func (p parts) cb() (op uint32) {
	op |= p.opcode << 24
	op |= uint32(p.cba&0x7ffff) << 5
	op |= uint32(p.rt & 0x1f)
	return
}

func sam(o uint32, shift, mask uint8) uint8 {
	return uint8(o>>shift) & mask
}

func smae(o, shift uint32, numBits uint64) int64 {
	mask := uint32(0)
	for i := uint64(0); i < numBits; i++ {
		mask |= 1 << i
	}
	rtn := int64((o >> shift) & mask)
	if (rtn & (1 << (numBits - 1))) > 1 {
		for i := uint64(63); i >= numBits; i-- {
			rtn |= int64(1) << i
		}
	}
	return rtn
}
