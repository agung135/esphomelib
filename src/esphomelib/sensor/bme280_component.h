//
//  bme280_component.h
//  esphomelib
//
//  Created by Otto Winter on 10.05.18.
//  Copyright © 2018 Otto Winter. All rights reserved.
//

#ifndef ESPHOMELIB_SENSOR_BME280_COMPONENT_H
#define ESPHOMELIB_SENSOR_BME280_COMPONENT_H

#include "esphomelib/sensor/sensor.h"
#include "esphomelib/i2c_component.h"
#include "esphomelib/defines.h"

#ifdef USE_BME280

namespace esphomelib {

namespace sensor {

struct BME280CalibrationData {
  uint16_t t1; // 0x88 - 0x89
  int16_t t2;  // 0x8A - 0x8B
  int16_t t3;  // 0x8C - 0x8D

  uint16_t p1; // 0x8E - 0x8F
  int16_t p2;  // 0x90 - 0x91
  int16_t p3;  // 0x92 - 0x93
  int16_t p4;  // 0x94 - 0x95
  int16_t p5;  // 0x96 - 0x97
  int16_t p6;  // 0x98 - 0x99
  int16_t p7;  // 0x9A - 0x9B
  int16_t p8;  // 0x9C - 0x9D
  int16_t p9;  // 0x9E - 0x9F

  uint8_t h1;  // 0xA1
  int16_t h2;  // 0xE1 - 0xE2
  uint8_t h3;  // 0xE3
  int16_t h4;  // 0xE4 - 0xE5[3:0]
  int16_t h5;  // 0xE5[7:4] - 0xE6
  int8_t h6;   // 0xE7
};

enum BME280Oversampling {
  BME280_OVERSAMPLING_NONE = 0b000,
  BME280_OVERSAMPLING_1X = 0b001,
  BME280_OVERSAMPLING_2X = 0b010,
  BME280_OVERSAMPLING_4X = 0b011,
  BME280_OVERSAMPLING_8X = 0b100,
  BME280_OVERSAMPLING_16X = 0b101,
};

enum BME280IIRFilter {
  BME280_IIR_FILTER_OFF = 0b000,
  BME280_IIR_FILTER_2X = 0b001,
  BME280_IIR_FILTER_4X = 0b010,
  BME280_IIR_FILTER_8X = 0b011,
  BME280_IIR_FILTER_16X = 0b100,
};

using BME280TemperatureSensor = sensor::EmptyPollingParentSensor<1, ICON_EMPTY, UNIT_C>;
using BME280PressureSensor = sensor::EmptyPollingParentSensor<1, ICON_GAUGE, UNIT_HPA>;
using BME280HumiditySensor = sensor::EmptyPollingParentSensor<1, ICON_WATER_PERCENT, UNIT_PERCENT>;

class BME280Component : public PollingComponent, public I2CDevice {
 public:
  BME280Component(I2CComponent *parent,
                  const std::string &temperature_name, const std::string &pressure_name,
                  const std::string &humidity_name,
                  uint8_t address = 0x77, uint32_t update_interval = 15000);

  void set_temperature_oversampling(BME280Oversampling temperature_over_sampling);
  void set_pressure_oversampling(BME280Oversampling pressure_over_sampling);
  void set_humidity_oversampling(BME280Oversampling humidity_over_sampling);
  void set_iir_filter(BME280IIRFilter iir_filter);

  BME280TemperatureSensor *get_temperature_sensor() const;
  BME280PressureSensor *get_pressure_sensor() const;
  BME280HumiditySensor *get_humidity_sensor() const;

  void setup() override;
  void loop() override;
  float get_setup_priority() const override;
  void update() override;

 protected:
  float read_temperature_(int32_t *t_fine);
  float read_pressure_(int32_t t_fine);
  float read_humidity_(int32_t t_fine);
  uint8_t read_u8(uint8_t register_);
  uint16_t read_u16_le(uint8_t register_);
  int16_t read_s16_le(uint8_t register_);

  BME280CalibrationData calibration_;
  BME280Oversampling temperature_oversampling_{BME280_OVERSAMPLING_16X};
  BME280Oversampling pressure_oversampling_{BME280_OVERSAMPLING_16X};
  BME280Oversampling humidity_oversampling_{BME280_OVERSAMPLING_16X};
  BME280IIRFilter iir_filter_{BME280_IIR_FILTER_OFF};
  BME280TemperatureSensor *temperature_sensor_;
  BME280PressureSensor *pressure_sensor_;
  BME280HumiditySensor *humidity_sensor_;
};

} // namespace sensor

} // namespace esphomelib

#endif //USE_BME280

#endif //ESPHOMELIB_SENSOR_BME280_COMPONENT_H
