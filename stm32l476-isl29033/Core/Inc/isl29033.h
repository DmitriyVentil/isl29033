#ifndef ISL29033_H_
#define ISL29033_H_

#include "stm32l4xx_hal.h"

// i2c для ISL29033
extern I2C_HandleTypeDef hi2c3;
#define ISL29033_I2C  				&hi2c3
// Адресс микросхемы
#define ISL29033_ADR 				0x88
//Чтение ножки прерывания
#define ISL29033_intRead()         	HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)
//Регистры
typedef enum
{
	COMMAND1_REG,
	COMMAND2_REG,
	DATA_LSB_REG,
	DATA_MSB_REG,
	INT_LT_LSB_REG,
	INT_LT_MSB_REG,
	INT_HT_LSB_REG,
	INT_HT_MSB_REG
} ISL29033_Registers;
//Режимы работы ISL29033
typedef enum
{
	Standby=0x00,
	ALS_continuous=0xA0,
	IR_continuous=0xC0
} ISL29033_mode;
// Количество циклов обработки
typedef enum
{
	cycles_1=0x00,
	cycles_4=0x01,
	cycles_8=0x02,
	cycles_16=0x03
} ISL29033_NumOfCycles;
// Разрешение в битах
typedef enum
{
	Resolution_16b=0x00,			//16 Бит
	Resolution_12b=0x04,			//12 бит
	Resolution_8b=0x08,				// 8 бит
	Resolution_4b=0x0C				// 4 бита
} ISL29033_ADCRes;
// Чувствтительность датчика ISL29033
typedef enum
{
	Luxrange_1=0x00,             	//125 люкс
	Luxrange_2=0x01,             	//500 люкс
	Luxrange_3=0x02,            	//2000 люкс
	Luxrange_4=0x03              	//8000 люкс
} ISL29033_LUXrange;

// Режим низкого энергопотребления ISL29033
void ISL29033_Standby(void);
// Функция установки нижнего и верхнего порога срабатывания прерывания(INT) ISL29033
void ISL29033_thresholds(uint16_t INT_LT,uint16_t INT_HT);
// Инициализация ISL29033
void ISL29033_init(ISL29033_mode mode,ISL29033_NumOfCycles NumOfCycles,
		           ISL29033_ADCRes ADCRes,ISL29033_LUXrange LUXrange);
// Очистка флага INT ISL29033
void ISL29033_Clearflag(void);
//Чтение значения освещенности ISL29033
uint16_t ISL29033_DataReadADC(void);
//Перевод значения из АЦП в Люксы
void ISL29033_ADCtoLUX(ISL29033_ADCRes ADCRes,ISL29033_LUXrange LUXrange,uint16_t *data);

#endif /* ISL29033_H_ */
