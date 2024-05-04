#include "LED/ServiceLed.h"

bool isLedOn = false;

static GPIOPortPin* pin = nullptr;
static Timer timer;

static uint32_t ledBlinkDurationMs = 0;
static uint16_t ledOnMs = 0;
static uint16_t ledOffMs = 0;
