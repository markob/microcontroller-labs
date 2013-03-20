#include "sfsm.h"

#include <reg52.h>

/* SFSM states declaration */
SFSM_BEGIN_EVENTS_DECL
SFSM_DECL_EVENT(COMMAND_RECEIVED)
SFSM_DECL_EVENT(SEND_DATA_REQUEST)
SFSM_DECL_EVENT(DATA_SENT)
SFSM_DECL_EVENT(NEW_STATE_REQUEST)
SFSM_DECL_EVENT(NEW_STEP_REQUEST)
SFSM_DECL_EVENT(STATE_CHANGED)
SFSM_DECL_EVENT(STEP_COMPLETED)
SFSM_END_EVENTS_DECL

SFSM_BEGIN_MAIN_ROUTINE

/* do all initialization for SFSM */

/* do all remaining initialization */

/* state change processing block */
SFSM_BEGIN_STATE_CHANGE_PROC

/** here is a list of events for actual task
 * command received
 * controller: state change request
 * controller: new step request
 * controller: state shanged
 * controller: step completed
 */

SFSM_BEGIN_EVENT_HANDLER(COMMAND_RECEIVED)
	P0 = 0x00;
SFSM_END_STATE_CHANGE

SFSM_BEGIN_STATE_CHANGE(LED_ON)
	P0 = 0xFF;
SFSM_END_STATE_CHANGE

SFSM_END_STATE_CHANGE_PROC

SFSM_END_MAIN_ROUTINE
