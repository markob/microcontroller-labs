#include "fsm.h"
#include "clock.h"

#define SIZE_OF_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))

static FSM_InitTask_t _fsm__tasks_ [] = {
#ifdef _CLOCK_H_
	_fsm_clock__init_,
#endif
};

/*static FSM_StateChangedCallback_t _fsm_state_callbacks_ [] = {
};*/

static BBOOLEAN _fsm__state_change_is_needed_ = BFALSE;
static UINT8 _fsm__state_;

/* private functions declarations */

/* public functions implementation */

void FSM_ChangeState(UINT8 state) using FSM_REG_BANK_NORM
{
	_fsm__state_ = state;
	_fsm__state_change_is_needed_ = BTRUE;
	
	while (1 == _fsm__state_change_is_needed_);		
}

/* private functions implementation */

static void _fsm__change_state_(void) using FSM_REG_BANK_IDLE
{
	switch (_fsm__state_) {
	default:
		break;
	}

	/* reset state change request flag */
	_fsm__state_change_is_needed_ = BFALSE;
}

static void main(void) using FSM_REG_BANK_IDLE
{
	UINT8 index;

	/* init everything needed */
	for (index = 0; index < SIZE_OF_ARRAY(_fsm__tasks_); index++) {
		_fsm__tasks_[index]();
	}

	for (;;) {
		/* just check state change here and do appropriate action if it was changed */
		if (BTRUE == _fsm__state_change_is_needed_) {
			_fsm__change_state_();			
		}
	}
}
