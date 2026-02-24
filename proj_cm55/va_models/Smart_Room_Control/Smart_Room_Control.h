/***************************************************************************************
 * \file Smart_Room_Control.h
 ****************************************************************************************
 * NOTE: This is a PLACEHOLDER file. Replace with the auto-generated file from
 * the DEEPCRAFT Voice Assistant cloud tool when your model is ready.
 * \copyright
 * Copyright 2025, Infineon Technologies.
 * All rights reserved.
 ****************************************************************************************/

#ifndef SMART_ROOM_CONTROL_H
#define SMART_ROOM_CONTROL_H

#include <stdint.h>

#define SMART_ROOM_CONTROL_NUM_INTENTS 10
#define SMART_ROOM_CONTROL_NUM_COMMANDS 29
#define SMART_ROOM_CONTROL_NUM_VARIABLES 2
#define SMART_ROOM_CONTROL_NUM_VARIABLE_PHRASES 2
#define SMART_ROOM_CONTROL_NUM_UNIT_PHRASES 16
#define SMART_ROOM_CONTROL_INTENT_MAP_ARRAY_TOTAL_SIZE 64
#define SMART_ROOM_CONTROL_UNIT_PHRASE_MAP_ARRAY_TOTAL_SIZE 38

extern const char
    *Smart_Room_Control_intent_name_list[SMART_ROOM_CONTROL_NUM_INTENTS];

extern const char
    *Smart_Room_Control_variable_name_list[SMART_ROOM_CONTROL_NUM_VARIABLES];

extern const char *Smart_Room_Control_variable_phrase_list
    [SMART_ROOM_CONTROL_NUM_VARIABLE_PHRASES];

extern const char
    *Smart_Room_Control_unit_phrase_list[SMART_ROOM_CONTROL_NUM_UNIT_PHRASES];

extern const int Smart_Room_Control_intent_map_array
    [SMART_ROOM_CONTROL_INTENT_MAP_ARRAY_TOTAL_SIZE];

extern const int
    Smart_Room_Control_intent_map_array_sizes[SMART_ROOM_CONTROL_NUM_COMMANDS];

extern const int
    Smart_Room_Control_variable_phrase_sizes[SMART_ROOM_CONTROL_NUM_VARIABLES];

extern const int Smart_Room_Control_unit_phrase_map_array
    [SMART_ROOM_CONTROL_UNIT_PHRASE_MAP_ARRAY_TOTAL_SIZE];

extern const int Smart_Room_Control_unit_phrase_map_array_sizes
    [SMART_ROOM_CONTROL_NUM_COMMANDS];

#endif // SMART_ROOM_CONTROL_H
