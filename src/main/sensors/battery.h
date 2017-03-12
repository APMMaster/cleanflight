/*
 * This file is part of Cleanflight.
 *
 * Cleanflight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cleanflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cleanflight.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config/parameter_group.h"

#include "common/filter.h"
#include "sensors/current.h"
#include "sensors/voltage.h"

#define VBAT_RESDIVVAL_DEFAULT 10
#define VBAT_RESDIVMULTIPLIER_DEFAULT 1
#define VBAT_SCALE_MIN 0
#define VBAT_SCALE_MAX 255

typedef struct batteryConfig_s {
    // voltage
    uint8_t vbatmaxcellvoltage;             // maximum voltage per cell, used for auto-detecting battery voltage in 0.1V units, default is 43 (4.3V)
    uint8_t vbatmincellvoltage;             // minimum voltage per cell, this triggers battery critical alarm, in 0.1V units, default is 33 (3.3V)
    uint8_t vbatwarningcellvoltage;         // warning voltage per cell, this triggers battery warning alarm, in 0.1V units, default is 35 (3.5V)
    uint8_t batteryNotPresentLevel;         // Below this level battery is considered as not present

    voltageMeterSource_e voltageMeterSource; // source of battery voltage meter used, either ADC or ESC

    // current
    uint16_t batteryCapacity;               // mAh
    currentMeterSource_e currentMeterSource; // source of battery current meter used, either ADC, Virtual or ESC

    // warnings / alerts
    bool useVBatAlerts;                     // Issue alerts based on VBat readings
    bool useConsumptionAlerts;              // Issue alerts based on total power consumption
    uint8_t consumptionWarningPercentage;   // Percentage of remaining capacity that should trigger a battery warning
    uint8_t vbathysteresis;                 // hysteresis for alarm, default 1 = 0.1V

    // FIXME this doesn't belong in here since it's a concern of MSP, not of the battery code.
    uint8_t multiwiiCurrentMeterOutput;     // if set to 1 output the amperage in milliamp steps instead of 0.01A steps via msp

} batteryConfig_t;

/* CF1.x
typedef struct batteryConfig_s {
    uint8_t vbatmaxcellvoltage;             // maximum voltage per cell, used for auto-detecting battery voltage in 0.1V units, default is 43 (4.3V)
    uint8_t vbatmincellvoltage;             // minimum voltage per cell, this triggers battery critical alarm, in 0.1V units, default is 33 (3.3V)
    uint8_t vbatwarningcellvoltage;         // warning voltage per cell, this triggers battery warning alarm, in 0.1V units, default is 35 (3.5V)
    uint16_t batteryCapacity;               // mAh
    uint8_t amperageMeterSource;             // see amperageMeter_e - used for telemetry, led strip, legacy MSP.
    uint8_t vbathysteresis;                 // hysteresis for alarm, default 1 = 0.1V
} batteryConfig_t;
*/

PG_DECLARE(batteryConfig_t, batteryConfig);

typedef enum {
    BATTERY_OK = 0,
    BATTERY_WARNING,
    BATTERY_CRITICAL,
    BATTERY_NOT_PRESENT
} batteryState_e;

//extern uint16_t vbatRaw;
//extern uint16_t vbatLatest;
extern uint8_t batteryCellCount;
extern uint16_t batteryWarningVoltage;
//extern int32_t amperageLatest;
//extern int32_t amperage;
//extern int32_t mAhDrawn;

batteryState_e getBatteryState(void);
const  char * getBatteryStateString(void);
void batteryUpdate(void);
void batteryInit(void);

struct rxConfig_s;

float calculateVbatPidCompensation(void);
uint8_t calculateBatteryPercentage(void);
uint16_t getVbat(void);
uint16_t getVbatLatest(void);

int32_t getAmperage(void);
int32_t getAmperageLatest(void);
int32_t getMAhDrawn(void);

void batteryUpdateCurrentMeter(int32_t lastUpdateAt);
