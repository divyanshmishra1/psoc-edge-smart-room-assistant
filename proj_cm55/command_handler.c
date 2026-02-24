/******************************************************************************
 * File Name : command_handler.c
 *
 * Description :
 * Custom voice command handler for smart room control.
 * Processes detected intents from the DeepCraft voice assistant and
 * maps them to hardware actions (LEDs, UART prints for relay control).
 *
 * Supported Commands:
 *   - Turn On/Off Light (Green LED)
 *   - Turn On/Off Projector (UART output)
 *   - Turn On/Off AC (UART output)
 *   - Start/Stop Meeting (Blue LED indicator)
 *   - Set Temperature <value> degrees (UART output)
 *   - Book Room <value> hours/minutes (UART output)
 *******************************************************************************/

/*******************************************************************************
 * Header Files
 *******************************************************************************/
#include "command_handler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cybsp.h"
#include "led_pwm.h"

/******************************************************************************
 * Macros
 *****************************************************************************/
#define SEPARATOR_LINE "--------------------------------------------"

/* Working buffer size for normalized command text */
#ifndef COMMAND_HANDLER_MAX_COMMAND_TEXT
#define COMMAND_HANDLER_MAX_COMMAND_TEXT (256u)
#endif

/******************************************************************************
 * Global Variables — Device States
 *****************************************************************************/
static device_state_t light_state = DEVICE_OFF;
static device_state_t projector_state = DEVICE_OFF;
static device_state_t ac_state = DEVICE_OFF;
static device_state_t meeting_state = DEVICE_OFF;
static int current_temp = 24; /* Default temperature in degrees */

/******************************************************************************
 * Static Function Prototypes
 *****************************************************************************/
static void handle_turn_on_light(void);
static void handle_turn_off_light(void);
static void handle_turn_on_projector(void);
static void handle_turn_off_projector(void);
static void handle_turn_on_ac(void);
static void handle_turn_off_ac(void);
static void handle_start_meeting(void);
static void handle_stop_meeting(void);
static void handle_set_temperature(va_data_t *va_data);
static void handle_book_room(va_data_t *va_data);

static int sanitize_command_text(const char *in, char *out, size_t out_size);
static void strip_leading_wake_word(char *s);
static bool extract_first_int(const char *s, int *out_value);
static bool contains_token(const char *s, const char *token);
static bool starts_with_token(const char *s, const char *token);

/*******************************************************************************
 * Function Name: command_handler_init
 *******************************************************************************
 * Summary:
 * Initialize the command handler. Prints the list of supported commands
 * to the UART terminal.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void command_handler_init(void) {
  printf("\r\n%s\r\n", SEPARATOR_LINE);
  printf(" Smart Room Voice Control — Initialized\r\n");
  printf("%s\r\n\r\n", SEPARATOR_LINE);

  printf("Device States:\r\n");
  printf("  Light     : OFF\r\n");
  printf("  Projector : OFF\r\n");
  printf("  AC        : OFF (Temp: %d°C)\r\n", current_temp);
  printf("  Meeting   : NOT ACTIVE\r\n\r\n");

  command_handler_print_commands();
}

/*******************************************************************************
 * Function Name: command_handler_print_commands
 *******************************************************************************
 * Summary:
 * Prints the list of all supported voice commands to the UART terminal.
 *
 * Parameters:
 *  void
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void command_handler_print_commands(void) {
  printf("Wake word: OK Infineon\r\n\r\n");
  printf("Supported Commands:\r\n");
  printf("%s\r\n", SEPARATOR_LINE);

  printf(" LIGHT CONTROL:\r\n");
  printf("   Turn On Light / Turn On The Light\r\n");
  printf("   Switch On Light / Switch On The Light\r\n");
  printf("   Turn Off Light / Turn Off The Light\r\n");
  printf("   Switch Off Light / Switch Off The Light\r\n\r\n");

  printf(" PROJECTOR CONTROL:\r\n");
  printf("   Turn On Projector / Turn On The Projector\r\n");
  printf("   Switch On Projector / Switch On The Projector\r\n");
  printf("   Turn Off Projector / Turn Off The Projector\r\n");
  printf("   Switch Off Projector / Switch Off The Projector\r\n\r\n");

  printf(" AC CONTROL:\r\n");
  printf("   Turn On AC / Turn On The AC\r\n");
  printf("   Switch On AC / Switch On The AC\r\n");
  printf("   Turn Off AC / Turn Off The AC\r\n");
  printf("   Switch Off AC / Switch Off The AC\r\n\r\n");

  printf(" MEETING CONTROL:\r\n");
  printf("   Start The Meeting\r\n");
  printf("   Stop The Meeting\r\n\r\n");

  printf(" TEMPERATURE:\r\n");
  printf("   Set Temperature <number> degree(s)\r\n\r\n");

  printf(" ROOM BOOKING:\r\n");
  printf("   Book Room <number> hour(s)\r\n");
  printf("   Book Room <number> minute(s)\r\n");

  printf("%s\r\n\r\n", SEPARATOR_LINE);

  printf("Example: \"OK Infineon Turn On Light\"\r\n\r\n");
}

/*******************************************************************************
 * Function Name: command_handler_process
 *******************************************************************************
 * Summary:
 * Process a detected voice command intent and execute the corresponding
 * hardware action.
 *
 * Parameters:
 *  intent_index: The intent index detected by the DeepCraft NLU engine
 *  va_data:      Pointer to the VA data structure containing variables/units
 *
 * Return:
 *  void
 *
 *******************************************************************************/
void command_handler_process(int intent_index, va_data_t *va_data) {
  printf("\r\n>>> Processing Command (Intent: %d) <<<\r\n", intent_index);

  switch (intent_index) {
  case INTENT_TurnOnLight:
    handle_turn_on_light();
    break;

  case INTENT_TurnOffLight:
    handle_turn_off_light();
    break;

  case INTENT_TurnOnProjector:
    handle_turn_on_projector();
    break;

  case INTENT_TurnOffProjector:
    handle_turn_off_projector();
    break;

  case INTENT_TurnOnAC:
    handle_turn_on_ac();
    break;

  case INTENT_TurnOffAC:
    handle_turn_off_ac();
    break;

  case INTENT_StartMeeting:
    handle_start_meeting();
    break;

  case INTENT_StopMeeting:
    handle_stop_meeting();
    break;

  case INTENT_SetTemperature:
    handle_set_temperature(va_data);
    break;

  case INTENT_BookRoom:
    handle_book_room(va_data);
    break;

  default:
    printf("[WARN] Unknown intent index: %d\r\n", intent_index);
    break;
  }

  /* Print current device states summary */
  printf("\r\n--- Device Status ---\r\n");
  printf("  Light: %s | Projector: %s | AC: %s (Temp: %d°C) | Meeting: %s\r\n",
         light_state == DEVICE_ON ? "ON" : "OFF",
         projector_state == DEVICE_ON ? "ON" : "OFF",
         ac_state == DEVICE_ON ? "ON" : "OFF", current_temp,
         meeting_state == DEVICE_ON ? "ACTIVE" : "INACTIVE");
  printf("---------------------\r\n\r\n");
}

bool command_handler_process_text(const char *command_text, const va_data_t *va_data)
{
  if (command_text == NULL)
  {
    return false;
  }

  /* Normalize into a small working buffer: lowercase, collapse spaces, remove punctuation */
  char s[COMMAND_HANDLER_MAX_COMMAND_TEXT] = {0};
  if (sanitize_command_text(command_text, s, sizeof(s)) <= 0)
  {
    return false;
  }

  /* Remove wake-word prefix if the ASR/NLU returns it in the string */
  strip_leading_wake_word(s);

  printf("\r\n>>> Processing Command (Text) <<<\r\n");
  printf("Normalized: \"%s\"\r\n", s);

  /* Fast-path: light/projector/ac are all "turn|switch on|off (the)? <device>" */
  const bool is_turn = starts_with_token(s, "turn");
  const bool is_switch = starts_with_token(s, "switch");
  const bool is_on = contains_token(s, "on");
  const bool is_off = contains_token(s, "off");

  if ((is_turn || is_switch) && (is_on ^ is_off))
  {
    if (contains_token(s, "light"))
    {
      if (is_on) { handle_turn_on_light(); } else { handle_turn_off_light(); }
      return true;
    }
    if (contains_token(s, "projector"))
    {
      if (is_on) { handle_turn_on_projector(); } else { handle_turn_off_projector(); }
      return true;
    }
    /* accept "ac" as a token, but avoid matching it inside other words */
    if (contains_token(s, "ac"))
    {
      if (is_on) { handle_turn_on_ac(); } else { handle_turn_off_ac(); }
      return true;
    }
  }

  if (starts_with_token(s, "start") && contains_token(s, "meeting"))
  {
    handle_start_meeting();
    return true;
  }

  if (starts_with_token(s, "stop") && contains_token(s, "meeting"))
  {
    handle_stop_meeting();
    return true;
  }

  if (starts_with_token(s, "set") && contains_token(s, "temperature"))
  {
    /* Prefer explicit number in text; fallback to va_data if present */
    int temp_value = 0;
    bool have_value = extract_first_int(s, &temp_value);
    va_data_t tmp = {0};
    if (!have_value && (va_data != NULL) && (va_data->num_var > 0))
    {
      tmp.num_var = 1;
      tmp.variable[0].value = va_data->variable[0].value;
      tmp.variable[0].unit_idx = va_data->variable[0].unit_idx;
      handle_set_temperature(&tmp);
      return true;
    }
    if (have_value)
    {
      tmp.num_var = 1;
      tmp.variable[0].value = temp_value;
      tmp.variable[0].unit_idx = -1;
      handle_set_temperature(&tmp);
      return true;
    }
    printf("[TEMP] Error: No temperature value provided\r\n");
    return false;
  }

  if (starts_with_token(s, "book") && contains_token(s, "room"))
  {
    int duration_value = 0;
    bool have_value = extract_first_int(s, &duration_value);

    /* Derive unit from command text */
    int unit_idx = -1;
    if (contains_token(s, "hour") || contains_token(s, "hours")) { unit_idx = UNIT_HOURS; }
    else if (contains_token(s, "minute") || contains_token(s, "minutes")) { unit_idx = UNIT_MINUTES; }

    va_data_t tmp = {0};

    if (!have_value && (va_data != NULL) && (va_data->num_var > 0))
    {
      tmp.num_var = 1;
      tmp.variable[0].value = va_data->variable[0].value;
      tmp.variable[0].unit_idx = va_data->variable[0].unit_idx;
      handle_book_room(&tmp);
      return true;
    }

    if (have_value)
    {
      tmp.num_var = 1;
      tmp.variable[0].value = duration_value;
      tmp.variable[0].unit_idx = unit_idx;
      handle_book_room(&tmp);
      return true;
    }

    printf("[BOOKING] Error: No duration value provided\r\n");
    return false;
  }

  printf("[WARN] Unsupported command text: \"%s\"\r\n", s);
  return false;
}

/*******************************************************************************
 *                          LIGHT HANDLERS
 *******************************************************************************/
static void handle_turn_on_light(void) {
  if (light_state == DEVICE_ON) {
    printf("[LIGHT] Already ON\r\n");
    return;
  }

  light_state = DEVICE_ON;

  /* Turn on the Green LED to indicate light ON */
  led_pwm_set_brightness(LED_PWM_GREEN_LED, LED_PWM_MAX_BRIGHTNESS);
  led_pwm_on(LED_PWM_GREEN_LED);

  printf("[LIGHT] --> ON (Green LED active)\r\n");
}

static void handle_turn_off_light(void) {
  if (light_state == DEVICE_OFF) {
    printf("[LIGHT] Already OFF\r\n");
    return;
  }

  light_state = DEVICE_OFF;

  /* Turn off the Green LED */
  led_pwm_off(LED_PWM_GREEN_LED);

  printf("[LIGHT] --> OFF (Green LED off)\r\n");
}

/*******************************************************************************
 *                          PROJECTOR HANDLERS
 *******************************************************************************/
static void handle_turn_on_projector(void) {
  if (projector_state == DEVICE_ON) {
    printf("[PROJECTOR] Already ON\r\n");
    return;
  }

  projector_state = DEVICE_ON;

  /*
   * TODO: Add GPIO control here to drive a relay for the projector.
   * Example: Cy_GPIO_Write(PROJECTOR_RELAY_PORT, PROJECTOR_RELAY_NUM, 1);
   */

  printf("[PROJECTOR] --> ON\r\n");
}

static void handle_turn_off_projector(void) {
  if (projector_state == DEVICE_OFF) {
    printf("[PROJECTOR] Already OFF\r\n");
    return;
  }

  projector_state = DEVICE_OFF;

  /*
   * TODO: Add GPIO control here to drive a relay for the projector.
   * Example: Cy_GPIO_Write(PROJECTOR_RELAY_PORT, PROJECTOR_RELAY_NUM, 0);
   */

  printf("[PROJECTOR] --> OFF\r\n");
}

/*******************************************************************************
 *                          AC HANDLERS
 *******************************************************************************/
static void handle_turn_on_ac(void) {
  if (ac_state == DEVICE_ON) {
    printf("[AC] Already ON (Temp: %d°C)\r\n", current_temp);
    return;
  }

  ac_state = DEVICE_ON;

  /*
   * TODO: Add GPIO control here to drive a relay for the AC unit.
   * Example: Cy_GPIO_Write(AC_RELAY_PORT, AC_RELAY_NUM, 1);
   */

  printf("[AC] --> ON (Temp: %d°C)\r\n", current_temp);
}

static void handle_turn_off_ac(void) {
  if (ac_state == DEVICE_OFF) {
    printf("[AC] Already OFF\r\n");
    return;
  }

  ac_state = DEVICE_OFF;

  /*
   * TODO: Add GPIO control here to drive a relay for the AC unit.
   * Example: Cy_GPIO_Write(AC_RELAY_PORT, AC_RELAY_NUM, 0);
   */

  printf("[AC] --> OFF\r\n");
}

/*******************************************************************************
 *                          MEETING HANDLERS
 *******************************************************************************/
static void handle_start_meeting(void) {
  if (meeting_state == DEVICE_ON) {
    printf("[MEETING] Already in progress\r\n");
    return;
  }

  meeting_state = DEVICE_ON;

  /* Use Blue LED solid ON to indicate meeting is active */
  led_pwm_set_brightness(LED_PWM_BLUE_LED, LED_PWM_MAX_BRIGHTNESS);
  led_pwm_on(LED_PWM_BLUE_LED);

  printf("[MEETING] --> STARTED (Blue LED solid ON)\r\n");
}

static void handle_stop_meeting(void) {
  if (meeting_state == DEVICE_OFF) {
    printf("[MEETING] No meeting in progress\r\n");
    return;
  }

  meeting_state = DEVICE_OFF;

  /* Turn off Blue LED to indicate meeting ended */
  led_pwm_off(LED_PWM_BLUE_LED);

  printf("[MEETING] --> STOPPED (Blue LED off)\r\n");
}

/*******************************************************************************
 *                      TEMPERATURE HANDLER
 *******************************************************************************/
static void handle_set_temperature(va_data_t *va_data) {
  if (va_data == NULL || va_data->num_var == 0) {
    printf("[TEMP] Error: No temperature value provided\r\n");
    return;
  }

  int temp_value = va_data->variable[0].value;

  /* Basic range validation */
  if (temp_value < 16 || temp_value > 30) {
    printf("[TEMP] Warning: Temperature %d°C is outside typical range "
           "(16-30°C)\r\n",
           temp_value);
  }

  current_temp = temp_value;

  printf("[TEMP] Temperature set to: %d°C\r\n", current_temp);

  /* If AC is ON, report the new temperature setting */
  if (ac_state == DEVICE_ON) {
    printf("[AC] Updated - running at %d°C\r\n", current_temp);
  } else {
    printf("[AC] Note: AC is currently OFF. Temperature will apply when AC is "
           "turned ON.\r\n");
  }
}

/*******************************************************************************
 *                          ROOM BOOKING HANDLER
 *******************************************************************************/
static void handle_book_room(va_data_t *va_data) {
  if (va_data == NULL || va_data->num_var == 0) {
    printf("[BOOKING] Error: No duration value provided\r\n");
    return;
  }

  int duration_value = va_data->variable[0].value;
  int unit_idx = va_data->variable[0].unit_idx;

  const char *unit_str = "unit(s)";

  /* Determine the unit string from the unit index */
  if (unit_idx == UNIT_HOUR || unit_idx == UNIT_HOURS) {
    unit_str = (duration_value == 1) ? "hour" : "hours";
  } else if (unit_idx == UNIT_MINUTE || unit_idx == UNIT_MINUTES) {
    unit_str = (duration_value == 1) ? "minute" : "minutes";
  }

  printf("[BOOKING] Room booked for: %d %s\r\n", duration_value, unit_str);
  printf("[BOOKING] Booking confirmed!\r\n");
}

/* ----------------------------- text parsing helpers ----------------------------- */

static int sanitize_command_text(const char *in, char *out, size_t out_size)
{
  if ((in == NULL) || (out == NULL) || (out_size < 2u))
  {
    return 0;
  }

  size_t j = 0;
  bool prev_space = true;

  for (size_t i = 0; in[i] != '\0' && j + 1 < out_size; i++)
  {
    unsigned char c = (unsigned char)in[i];

    if (isalnum(c))
    {
      out[j++] = (char)tolower(c);
      prev_space = false;
    }
    else
    {
      /* treat punctuation as a word separator */
      if (!prev_space && (j + 1 < out_size))
      {
        out[j++] = ' ';
        prev_space = true;
      }
    }
  }

  /* trim trailing space */
  while (j > 0 && out[j - 1] == ' ')
  {
    j--;
  }
  out[j] = '\0';
  return (int)j;
}

static void strip_leading_wake_word(char *s)
{
  if (s == NULL) return;

  /* common wake word variant: "ok infineon ..." */
  const char *ww = "ok infineon";
  const size_t ww_len = strlen(ww);

  char *start = s;
  while (*start == ' ') start++;

  if (strncmp(start, ww, ww_len) == 0)
  {
    char *p = start + ww_len;
    while (*p == ' ') p++;
    memmove(s, p, strlen(p) + 1);
  }
}

static bool extract_first_int(const char *s, int *out_value)
{
  if ((s == NULL) || (out_value == NULL)) return false;

  for (size_t i = 0; s[i] != '\0'; i++)
  {
    if (isdigit((unsigned char)s[i]))
    {
      int v = 0;
      while (isdigit((unsigned char)s[i]))
      {
        v = (v * 10) + (s[i] - '0');
        i++;
      }
      *out_value = v;
      return true;
    }
  }
  return false;
}

static bool starts_with_token(const char *s, const char *token)
{
  if ((s == NULL) || (token == NULL)) return false;
  const size_t n = strlen(token);
  if (strncmp(s, token, n) != 0) return false;
  return (s[n] == '\0' || s[n] == ' ');
}

static bool contains_token(const char *s, const char *token)
{
  if ((s == NULL) || (token == NULL)) return false;
  const size_t n = strlen(token);
  const char *p = s;

  while ((p = strstr(p, token)) != NULL)
  {
    const bool left_ok = (p == s) || (p[-1] == ' ');
    const bool right_ok = (p[n] == '\0') || (p[n] == ' ');
    if (left_ok && right_ok) return true;
    p += n;
  }
  return false;
}

/* [] END OF FILE */
