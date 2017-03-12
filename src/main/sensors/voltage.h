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

#define VBAT_RESDIVVAL_DEFAULT 10
#define VBAT_RESDIVMULTIPLIER_DEFAULT 1
#define VBAT_SCALE_MIN 0
#define VBAT_SCALE_MAX 255

#define VBATT_LPF_FREQ  1.0f

typedef enum {
    VOLTAGE_METER_NONE = 0,
    VOLTAGE_METER_ADC,
    VOLTAGE_METER_ESC
} voltageMeterSource_e;


typedef enum {
    VOLTAGE_SENSOR_TYPE_ADC_RESISTOR_DIVIDER = 0,
    VOLTAGE_SENSOR_TYPE_ESC
} voltageSensorType_e;

#ifndef MAX_VOLTAGE_METER_ADC
#define MAX_VOLTAGE_METER_ADC 1 // VBAT - some boards have external, 12V and 5V meters.
#endif

typedef enum {
    VOLTAGE_SENSOR_ADC_VBAT = 0,
    // VOLTAGE_SENSOR_ADC_5V = 1,
    // VOLTAGE_SENSOR_ADC_12V = 2
} voltageSensorADC_e;

// WARNING - do not mix usage of VOLTAGE_METER_* and VOLTAGE_METER_*, they are separate concerns.

typedef struct voltageSensorADCConfig_s {
    uint8_t vbatscale;                      // adjust this to match battery voltage to reported value
    uint8_t vbatresdivval;                  // resistor divider R2 (default NAZE 10(K))
    uint8_t vbatresdivmultiplier;           // multiplier for scale (e.g. 2.5:1 ratio with multiplier of 4 can use '100' instead of '25' in ratio) to get better precision
} voltageSensorADCConfig_t;

PG_DECLARE_ARRAY(voltageSensorADCConfig_t, MAX_VOLTAGE_METER_ADC, voltageSensorADCConfig);

typedef struct voltageMeterADCState_s {
    uint16_t vbat;                // battery voltage in 0.1V steps (filtered)
    uint16_t vbatRaw;
    uint16_t vbatLatestADC;       // most recent unsmoothed raw reading from vbat ADC
    biquadFilter_t vbatFilterState;
} voltageMeterADCState_t; // TODO rename to voltageMeter_t

extern voltageMeterADCState_t voltageMeterADCStates[MAX_VOLTAGE_METER_ADC];


void voltageMeterADCInit(void);
void voltageMeterADCUpdate(void);

voltageMeterADCState_t *getVoltageMeterADC(uint8_t index);
uint16_t getVoltageForADCChannel(uint8_t channel);
uint16_t getLatestVoltageForADCChannel(uint8_t channel);

const voltageSensorADCConfig_t *getVoltageMeterADCConfig(const uint8_t channel);
