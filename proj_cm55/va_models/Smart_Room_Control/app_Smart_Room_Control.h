#ifndef APP_SMART_ROOM_CONTROL_h
#define APP_SMART_ROOM_CONTROL_h

#include "Smart_Room_Control.h"

/*******************************************************************************
 * Intent Indices
 * These map to the intents defined in the DeepCraft cloud tool.
 * IMPORTANT: When you generate the real model from DeepCraft, verify these
 * indices match the generated app header. Update command_handler.h if needed.
 ******************************************************************************/
#define SMART_ROOM_CONTROL_INTENT_TurnOnLight 0x0
#define SMART_ROOM_CONTROL_INTENT_TurnOffLight 0x1
#define SMART_ROOM_CONTROL_INTENT_TurnOnProjector 0x2
#define SMART_ROOM_CONTROL_INTENT_TurnOffProjector 0x3
#define SMART_ROOM_CONTROL_INTENT_TurnOnAC 0x4
#define SMART_ROOM_CONTROL_INTENT_TurnOffAC 0x5
#define SMART_ROOM_CONTROL_INTENT_StartMeeting 0x6
#define SMART_ROOM_CONTROL_INTENT_StopMeeting 0x7
#define SMART_ROOM_CONTROL_INTENT_SetTemperature 0x8
#define SMART_ROOM_CONTROL_INTENT_BookRoom 0x9

/*******************************************************************************
 * Variable Indices
 ******************************************************************************/
#define SMART_ROOM_CONTROL_VARIABLE_Temperature_ 0x0
#define SMART_ROOM_CONTROL_VARIABLE_Duration_ 0x1

/*******************************************************************************
 * Unit Indices
 ******************************************************************************/
#define SMART_ROOM_CONTROL_UNIT_degree 0x0
#define SMART_ROOM_CONTROL_UNIT_degrees 0x1
#define SMART_ROOM_CONTROL_UNIT_percent 0x2
#define SMART_ROOM_CONTROL_UNIT_level 0x3
#define SMART_ROOM_CONTROL_UNIT_levels 0x4
#define SMART_ROOM_CONTROL_UNIT_hour 0x5
#define SMART_ROOM_CONTROL_UNIT_hours 0x6
#define SMART_ROOM_CONTROL_UNIT_minute 0x7
#define SMART_ROOM_CONTROL_UNIT_minutes 0x8
#define SMART_ROOM_CONTROL_UNIT_second 0x9
#define SMART_ROOM_CONTROL_UNIT_seconds 0xa
#define SMART_ROOM_CONTROL_UNIT_day 0xb
#define SMART_ROOM_CONTROL_UNIT_days 0xc
#define SMART_ROOM_CONTROL_UNIT_ 0xd
#define SMART_ROOM_CONTROL_UNIT_AM 0xe
#define SMART_ROOM_CONTROL_UNIT_PM 0xf

#endif // APP_SMART_ROOM_CONTROL_h
