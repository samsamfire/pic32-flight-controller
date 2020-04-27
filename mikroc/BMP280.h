
#include <stdint.h>
int32_t adc_T, adc_P, t_fine;

// BMP280 sensor modes, register ctrl_meas mode[1:0]
typedef enum
{
  MODE_SLEEP  = 0x00,  // sleep mode
  MODE_FORCED = 0x01,  // forced mode
  MODE_NORMAL = 0x03   // normal mode
} BMP280_mode;

// oversampling setting. osrs_t[2:0], osrs_p[2:0]
typedef enum
{
  SAMPLING_SKIPPED = 0x00,  //skipped, output set to 0x80000
  SAMPLING_X1      = 0x01,  // oversampling x1
  SAMPLING_X2      = 0x02,  // oversampling x2
  SAMPLING_X4      = 0x03,  // oversampling x4
  SAMPLING_X8      = 0x04,  // oversampling x8
  SAMPLING_X16     = 0x05   // oversampling x16
} BMP280_sampling;

// filter setting filter[2:0]
typedef enum
{
  FILTER_OFF = 0x00,  // filter off
  FILTER_2   = 0x01,  // filter coefficient = 2
  FILTER_4   = 0x02,  // filter coefficient = 4
  FILTER_8   = 0x03,  // filter coefficient = 8
  FILTER_16  = 0x04   // filter coefficient = 16
} BMP280_filter;

// standby (inactive) time in ms (used in normal mode), t_sb[2:0]
typedef enum
{
  STANDBY_0_5   =  0x00,  // standby time = 0.5 ms
  STANDBY_62_5  =  0x01,  // standby time = 62.5 ms
  STANDBY_125   =  0x02,  // standby time = 125 ms
  STANDBY_250   =  0x03,  // standby time = 250 ms
  STANDBY_500   =  0x04,  // standby time = 500 ms
  STANDBY_1000  =  0x05,  // standby time = 1000 ms
  STANDBY_2000  =  0x06,  // standby time = 2000 ms
  STANDBY_4000  =  0x07   // standby time = 4000 ms
} standby_time;

struct
{
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;

  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;
} BMP280_calib;

void BMP280_Write8(uint8_t reg_addr, uint8_t _data);
uint8_t BMP280_Read8(uint8_t reg_addr);
uint16_t BMP280_Read16(uint8_t reg_addr);
void BMP280_Configure(BMP280_mode mode, BMP280_sampling T_sampling,
                      BMP280_sampling P_sampling, BMP280_filter filter, standby_time standby);
uint8_t BMP280_begin(BMP280_mode mode,
                  BMP280_sampling T_sampling,
                  BMP280_sampling P_sampling,
                  BMP280_filter filter,
                  standby_time  standby);
uint8_t BMP280_ForcedMeasurement();
void BMP280_Update();
uint8_t BMP280_readTemperature(int32_t *temp);
uint8_t BMP280_readPressure(uint32_t *pres);