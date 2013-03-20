#ifndef _FSM_H_
#define _FSM_H_

#include "types.h"

#define	FSM_REG_BANK_NORM 0
#define FSM_REG_BANK_IDLE 1
#define FSM_REG_BANK_INTR 2
#define FSM_REG_BANK_NUSD 3

typedef void (*FSM_InitTask_t) (void);
typedef void (*FSM_StateChangedCallback_t) (void);

/* public functions declarations */
/**
 * @brief request for change state
 *
 */
void FSM_ChangeState(UINT8 state);

#endif /* _FSM_H_ */
