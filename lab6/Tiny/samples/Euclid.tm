* TINY Compilation to TM Code
* File: samples/Euclid.tm
* Standard prelude:
  0:     LD  6,0(0) 	load maxaddress from location 0
  1:     ST  0,0(0) 	clear location 0
* End of standard prelude.
  2:     IN  0,0,0 	read integer value
  3:     ST  0,0(5) 	read: store value
  4:     IN  0,0,0 	read integer value
  5:     ST  0,1(5) 	read: store value
* -> assign
* -> Op
* -> Id
  6:     LD  0,0(5) 	load id value
* <- Id
  7:     ST  0,0(6) 	op: push left
* -> Id
  8:     LD  0,1(5) 	load id value
* <- Id
  9:     LD  1,0(6) 	op: load left
 10:    MUL  0,1,0 	op *
* <- Op
 11:     ST  0,2(5) 	assign: store value
* <- assign
* -> if
* -> Op
* -> Id
 12:     LD  0,0(5) 	load id value
* <- Id
 13:     ST  0,0(6) 	op: push left
* -> Id
 14:     LD  0,1(5) 	load id value
* <- Id
 15:     LD  1,0(6) 	op: load left
 16:    SUB  0,1,0 	op <
 17:    JLT  0,2(7) 	br if true
 18:    LDC  0,0(0) 	false case
 19:    LDA  7,1(7) 	unconditional jmp
 20:    LDC  0,1(0) 	true case
* <- Op
* if: jump to else belongs here
* -> assign
* -> Id
 22:     LD  0,0(5) 	load id value
* <- Id
 23:     ST  0,3(5) 	assign: store value
* <- assign
* -> assign
* -> Id
 24:     LD  0,1(5) 	load id value
* <- Id
 25:     ST  0,0(5) 	assign: store value
* <- assign
* -> assign
* -> Id
 26:     LD  0,3(5) 	load id value
* <- Id
 27:     ST  0,1(5) 	assign: store value
* <- assign
* if: jump to end belongs here
 21:    JEQ  0,7(7) 	if: jmp to else
 28:    LDA  7,0(7) 	jmp to end
* <- if
* -> repeat
* repeat: jump after body comes back here
* -> assign
* -> Op
* -> Id
 29:     LD  0,0(5) 	load id value
* <- Id
 30:     ST  0,0(6) 	op: push left
* -> Op
* -> Op
* -> Id
 31:     LD  0,0(5) 	load id value
* <- Id
 32:     ST  0,-1(6) 	op: push left
* -> Id
 33:     LD  0,1(5) 	load id value
* <- Id
 34:     LD  1,-1(6) 	op: load left
 35:    DIV  0,1,0 	op /
* <- Op
 36:     ST  0,-1(6) 	op: push left
* -> Id
 37:     LD  0,1(5) 	load id value
* <- Id
 38:     LD  1,-1(6) 	op: load left
 39:    MUL  0,1,0 	op *
* <- Op
 40:     LD  1,0(6) 	op: load left
 41:    SUB  0,1,0 	op -
* <- Op
 42:     ST  0,3(5) 	assign: store value
* <- assign
* -> assign
* -> Id
 43:     LD  0,1(5) 	load id value
* <- Id
 44:     ST  0,0(5) 	assign: store value
* <- assign
* -> assign
* -> Id
 45:     LD  0,3(5) 	load id value
* <- Id
 46:     ST  0,1(5) 	assign: store value
* <- assign
* -> Op
* -> Id
 47:     LD  0,1(5) 	load id value
* <- Id
 48:     ST  0,0(6) 	op: push left
* -> Const
 49:    LDC  0,0(0) 	load const
* <- Const
 50:     LD  1,0(6) 	op: load left
 51:    SUB  0,1,0 	op ==
 52:    JEQ  0,2(7) 	br if true
 53:    LDC  0,0(0) 	false case
 54:    LDA  7,1(7) 	unconditional jmp
 55:    LDC  0,1(0) 	true case
* <- Op
 56:    JEQ  0,-28(7) 	repeat: jmp back to body
* <- repeat
* -> Id
 57:     LD  0,0(5) 	load id value
* <- Id
 58:    OUT  0,0,0 	write ac
* -> Op
* -> Id
 59:     LD  0,2(5) 	load id value
* <- Id
 60:     ST  0,0(6) 	op: push left
* -> Id
 61:     LD  0,0(5) 	load id value
* <- Id
 62:     LD  1,0(6) 	op: load left
 63:    DIV  0,1,0 	op /
* <- Op
 64:    OUT  0,0,0 	write ac
* End of execution.
 65:   HALT  0,0,0 	
