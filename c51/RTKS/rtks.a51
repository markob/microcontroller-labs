;;; PART 1

;before using configure RAMTOP, NOOFTSKS, STACKSIZE
;RAMTOP is FFH except for 8051 (7FH) & 750/1/2/3(3F)
;NOOFTSKS is 1 up to 8
;STACKSIZE is 2 + 2*<nested call level in tasks>
PUBLIC TRUN, Q, QTOP, TSIG, TSIGWAIT, TTIME
PUBLIC TSTACK, RAMTOP, _WAITT, _WAITS
PUBLIC _SCHECK, _SIGNL, _CREAT, _DEFER, INITK
PUBLIC RTKST

;;; PART 2

THIGH EQU 0F0H ;setting for 1 msec
TLOW EQU 0H
RAMTOP EQU 0FFH	;set for 87c750 - 64 bytes
NOOFTSKS EQU 3 ;can be up to 8 tasks
STACKSIZE EQU 4	; 2 + 2*<# of calls>

MSKS SEGMENT CODE
RSEG MSKS
MSK: DB 0,1,2,4,8,16,32,64,128 ;select sig bit

TBLS SEGMENT DATA
RSEG TBLS 
QTOP:DS 1 ;address of newest ready task
TRUN:
Q: DS NOOFTSKS+1 ;empties mask with 0
TSTACK: DS NOOFTSKS+1 ;SP for each task
TTIME: DS NOOFTSKS+1 ;remaining time:
					 ;0=ready
					 ;ff=not timed
TSIG: DS 1 ;up to 8 tasks waiting for signals 
TSIGWAIT: DS 1 ;tasks waiting for signals

ISEG AT RAMTOP-((NOOFTSKS+1)*STACKSIZE)-3
;reserve spase at top for main() startup
STKRESERVE: DS NOOFTSKS*STACKSIZE

CSEG AT 0BH ;interrupt address for timer0
LJMP TINT

;;; PART 3

RTKS SEGMENT CODE
RSEG RTKS
USING 1 ;force reservation of bank 1
_SCHECK: ;entry for signal check;
		 ;_ means function with reg passed params
		 ;return 0 or 1 in R7
		 ;no bank switch
	MOV A, TRUN
	MOV DPTR,#MSK
	MOVC A,@A+DPTR ;fetch sig mask byte
	ANL A,TSIG
	JNZ V1
	MOV R7,A ;sig not yet set - R7=0
	RET
V1:	MOV R7,#1 ;sig set R7=1
	RET
	
;;; PART 4

_SIGNL: ;entry to set signal bit for task
		;no bank switch - no context switch
	MOV DPTR,#MSK
	MOV A,R7 ;task# passed in R7
	MOVC A,@A+DPTR ;fetch sig mask byte
	MOV B,A ;save mask
	ANL A,TSIGWAIT
	JZ G1
	;task already waiting for signal
	MOV A,B ;restore mask
	CPL A
	ANL TSIGWAIT,A ;clear waiting bit
	ANL TSIG,A ;ensure cleared signal bit
	;put task on queue
	INC QTOP
	MOV R0,QTOP
	MOV A,R7
	MOV @R0,A ;set ready task on queue
	RET ;running task keeps control
	;task not yet waiting - signal arrived first
G1: MOV A,B ;restore mask
	ORL TSIG,A ;set signal bit
	RET
	
;;; PART 5

_WAITS: ;entry wait for timeout or signal;
		;time passed in R7
	MOV R5,PSW ;save registerbank
	;fetch signal mask byte
	MOV DPTR,#MSK
	MOV A,TRUN
	MOVC A,@A+DPTR
	MOV B,A ;save mask in B
	;signal already there?
	ANL A,TSIG
	JZ U1 ;if no signal task waits
	;signal already there - put task on queue
	MOV A,B ;reload mask
	XRL TSIG,A ;clear signal bit
	INC QTOP
	MOV R0,QTOP
	MOV @R0,TRUN ;put task on queue (no wait)
	SJMP QSHFT ;off to start next task in queue
	;task waits for signal
U1: MOV A,B
	ORL TSIGWAIT,A ;set sig waiting bit
	SJMP U2

;;; PART 6
_WAITT: ;entry point - wait for time;
		;time passed in R7
	MOV R5,PSW ;save registerbank
U2: MOV A,#TTIME
	ADD A,TRUN
	MOV R0,A
	MOV A,R7
	MOV @R0,A ;R7 - timeout put in table
	SJMP QSHFT ;task not on queue - waiting

;;; PART 7
_DEFER: ;entry to put running at queue's end
	MOV R5,PSW ;save registerbank
	ORL PSW,#8 ;set to registerbank 1
	INC QTOP
	ADD A,QTOP
	MOV R0,A
	MOV @R0,TRUN ;put TRUN at end of queue

;;; PART 8

QSHFT: ;shift queue down by one
	   ;get new TRUN task from queue
	   ;use registerbank 1
	MOV PSW,#8; set registerbank 1
	MOV A,TRUN
	MOV A,#TSTACK
	ADD A,TRUN
	MOV R0,A
	MOV @R0,SP
	;shift queue down by one;
	;work from top down
S5: MOV A,QTOP
	MOV R0,#0 ;masr old QTOP empty
S1: XCH A,R0
	XCH A,@R0 ;put task# one lower on queue
	DEC R0
	XCH A,R0
	CJNE A,#TRUN-1,S1
	DEC QTOP
	;point to new task
S2: MOV A,#TSTACK
	ADD A,TRUN
	MOV R0,A
	MOV SP,@R0
	;tasks to run? - if not then go idle
	MOV A,TRUN
	JZ S3 ;0=idle task
	MOV PSW,R5 ;restore new task's bank
	RETI
	;entering idle state
S3: MOV QTOP,#Q ;leave at running for idle
	MOV TSTACK,#STKRESERVE
	MOV SP,#STKRESERVE
IDLE: ;no tasks to run until task times out
	SJMP IDLE ;	hold for timer interrupt

;;; PART 9

TINT: ;entry only from timer interrupt
	MOV TH0,#THIGH ;reset timer
	MOV TL0,#TLOW
	MOV IE,#82H ;reenable timer0 interrupt
	MOV R5,PSW ;save in unused register
	MOV PSW,#8 ;set to registerbank 1
	MOV R7,A ;only non-banked register used here
	MOV R6,#0 ;clear new task ready flag
	MOV R0,#TTIME+NOOFTSKS ;-> TTIME tbl
	MOV R2,#NOOFTSKS ;do all tasks
	;loop through tasks adjusting TTIME table values
K2: MOV A,@R0 ;get time for task
	CJNE A,0FFH,K4
	SJMP K1; FF=task past 0 or not timing
	;reduce delay by one
K4: JNZ K5 ;task not timed out yet
	;put newly timed out task on queue (READY)
	INC QTOP ;make room for one more task
	MOV R1,QTOP
	MOV A,R2
	MOV @R1,A ;put task# in queue
	MOV R6,#1 ;mark if any task comes ready
	MOV DPTR,#MSK
	MOV A,TRUN
	MOVC A,@A+DPTR ;fetch sig mask byte
	XRL TSIG,A ;clear waiting for signal bit
	MOV A,@R0
K5: DEC A ;decrement last to process 0
	MOV @R0,A ;save decremented time
K1: DEC R0 ;move down one in table
	DJNZ R2,K2 ;though all tasks yet?
	;if idle set new TRUN task
	MOV A,R6
	JZ K3 ;no new task added to queue
	MOV A,R7 ;restore accumulator
	SJMP QSHFT
K3: MOV A,R7 ;restore accumulator
	MOV PSW,R5 ;restore registerbank
	RETI

;;; PART 10

_CREAT: ;create a task;
		;task# passed in R7;
		;start addr in R4,R5
		;task put at top (back end) of queue
	INC QTOP
	MOV R0,QTOP
C2: MOV A,R7
	MOV @R0,A ;task# put on queue at top
	;point to task's SP storage space
	MOV A,#TSTACK
	ADD A,R7
	MOV R0,A ;R0 points to task's SP
	MOV A,@R0 ;get SP
	MOV R1,A
	;put addr of new task on its stack
	INC R1
	MOV A,R5
	MOV @R1,A
	INC R1
	MOV A,R4
	MOV @R1,A
	;save task's stack pointer
	MOV A,R1
	MOV @R0,A
	RET

;;; PART 11

INITK: ;set up variables
	;microcontroller specific initialization should be here
	;timers initialization
	MOV TMOD,#011H ;T0, T1 16 overflow
	MOV TCON,#054H ;T0, T1, IE1 edge trig
	;RTKS table initialization
	MOV R7,#NOOFTSKS+1 ;add idle task
	MOV R0,#TTIME ;point to timer array
	MOV R1,#Q ;point to queue
	;initialize stack ptrs & timeouts
I1: MOV @R0,#0FFH ;no timer action
	MOV @R1,#0 ;idle task in queue
	INC R0
	INC R1
	DJNZ R7,I1 ;set up all tasks
	MOV @R1,#0 ;queue includes running
	;allocate fixed task stacks of designated size
	MOV R0,#TSTACK ;point to stacks
	MOV R7,#NOOFTSKS+1
	MOV A,#STKRESERVE ;base of task stacks
I2: MOV @R0,A ;set task's stack pointer
	ADD A,#STACKSIZE
	INC R0
	DJNZ R7,I2
	;other initialization
	MOV QTOP,#Q-1 ;CREAT increments first
	MOV TSIG,#0 ;clear all signals
	MOV TSIGWAIT,#0 ;no signal waits pending
	RET

;;; PART 12

RTKST:
	MOV TH0,#THIGH
	MOV TL0,#TLOW
	SETB TR0 ;start timer
	MOV IE,#82H ;enable timer0 interrupt
	;start with task#1
	MOV R0,#TSTACK
	INC R0
	MOV SP,@R0
	POP PSW
	RETI
END

