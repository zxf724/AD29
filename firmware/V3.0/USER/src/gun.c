#include "gun.h"
#include "analysis_protocol.h"
#include "ananlysis_data.h"
#include "delay.h"
#include "moto.h"
#include "string.h"

extern uint8_t g_start_cmd[7];
extern uint8_t g_bar_code[7];
extern mError errorDef;

uint8_t Set_Gun() {
  const uint8_t kCmd_Start_Gun[] = {0x53, 0x43, 0x41, 0x4E, 0x20, 0x4F, 0x4E};
  if (strcmp((const char*)kCmd_Start_Gun, (const char*)g_start_cmd) == 0) {
    if (!Open_Gun()) {
      return 0;
    } else {
      return 0xff;
    }
  }
  return 1;
}

uint8_t Open_Gun() { return 0; }

void Report_Bar_Code() {
  if (errorDef.bar_code_state) {
    Report_State(CMD_REBACK, g_bar_code, strlen((const char*)g_bar_code));
    delay_ms(5);
    if (!errorDef.android_state)
      errorDef.error_count++;
    else {
      errorDef.bar_code_state = 0;
      errorDef.error_count = 0;
      errorDef.android_state = 0;
    }
    if (errorDef.error_count >= 100) {
      errorDef.bar_code_state = 0;
      errorDef.android_state = 0;
      errorDef.error_count = 0;
    }
  }
}
