#ifndef _SFSM_H_
#define _SFSM_H_

#include "types.h"

#define SFSM_REG_BANK_BASE 0
#define SFSM_REG_BANK_IDLE 1
#define SFSM_REG_BANK_INTR 2
#define SFSM_REG_BANK_NUSD 3

/* SFSM macro definitions */
#define SFSM_BEGIN_STATES_DECL \
	enum SFSM_States_t {
		
#define SFSM_END_STATES_DECL \
	} _sfsm__next_state_;

#define SFSM_DECL_STATE(state) \
	SFSM_STATE__##state,
	

#define SFSM_BEGIN_STATE_CHANGE_PROC \
sfsm_lbl__proc_state_change: \
	_sfsm__is_state_change_requested_ = BFALSE; \	
	switch (_sfsm__next_state_) {

#define SFSM_END_STATE_CHANGE_PROC \
	} \
	goto sfsm_lbl_main_loop;

#define SFSM_BEGIN_STATE_CHANGE(state) \
	case SFSM_STATE__##state: {

#define SFSM_END_STATE_CHANGE \
	} break;

#define SFSM_DO_MAIN_LOOP() \
	for (;;) { \
sfsm_lbl_main_loop: \
		if (_sfsm__is_state_change_requested_) \
			goto sfsm_lbl__proc_state_change; \	
	}	

#define SFSM_STATE_CHANGED \
	goto sfsm_label_idle;

/* SFSM declarations */
BBOOLEAN _sfsm__is_state_change_requested_ = BFALSE;

#define SFSM_BEGIN_MAIN_ROUTINE \
	void main(void) using SFSM_REG_BANK_IDLE \
	{

#define SFSM_END_MAIN_ROUTINE \
	SFSM_DO_MAIN_LOOP() \
	}

#endif /* _SFSM_H_ */
