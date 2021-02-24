#include "isl29033.h"
#include "math.h"
//Чтение регистра ISL29033
uint8_t ISL29033_RegRead(ISL29033_Registers DataReg)
{
	HAL_I2C_Master_Transmit(ISL29033_I2C,ISL29033_ADR,&DataReg,1,10);
	HAL_I2C_Master_Receive(ISL29033_I2C,ISL29033_ADR,&DataReg,1,10);
	return DataReg;
}
//Запись в регистр ISL29033
void ISL29033_RegWrite(ISL29033_Registers DataReg,uint8_t Data)
{
	uint8_t Data_t[2]={DataReg,Data};
	HAL_I2C_Master_Transmit(ISL29033_I2C,ISL29033_ADR,Data_t,2,10);
}
//Настройка первого регистра конфигурации
void ISL29033_config1(ISL29033_mode mode,ISL29033_NumOfCycles NumOfCycles)
{
	ISL29033_RegWrite(COMMAND1_REG,mode|NumOfCycles);
}
//Настройка второго регистра конфигурации
void ISL29033_config2(ISL29033_ADCRes ADCRes,ISL29033_LUXrange LUXrange)
{
	ISL29033_RegWrite(COMMAND2_REG,ADCRes|LUXrange);
}
// Режим низкого энергопотребления ISL29033
void ISL29033_Standby(void)
{
	ISL29033_config1(STANDBY,CYCLES_1);
}
// Функция установки нижнего и верхнего порога срабатывания прерывания(INT) ISL29033
void ISL29033_thresholds(uint16_t INT_LT,uint16_t INT_HT)
{
	ISL29033_RegWrite(INT_LT_LSB_REG,(uint8_t)INT_LT);
	ISL29033_RegWrite(INT_LT_MSB_REG,(uint8_t)(INT_LT>>8));
	ISL29033_RegWrite(INT_HT_LSB_REG,(uint8_t)INT_HT);
	ISL29033_RegWrite(INT_HT_MSB_REG,(uint8_t)(INT_HT>>8));
}
// Инициализация ISL29033
void ISL29033_init(ISL29033_mode mode,ISL29033_NumOfCycles NumOfCycles,
		           ISL29033_ADCRes ADCRes,ISL29033_LUXrange LUXrange)
{
	ISL29033_Standby();
	HAL_Delay(1);
	ISL29033_thresholds(0x0000,0xFFFF);
	ISL29033_config1(mode,NumOfCycles);
	ISL29033_config2(ADCRes,LUXrange);
}
// Очистка флага INT ISL29033
void ISL29033_Clearflag(void)
{
	ISL29033_RegRead(COMMAND1_REG);
}
//Чтение значения освещенности ISL29033
uint16_t ISL29033_DataReadADC(void)
{
	return ISL29033_RegRead(DATA_LSB_REG)+(ISL29033_RegRead(DATA_MSB_REG)<<8);
}
//Перевод значения с АЦП в Люксы
void ISL29033_ADCtoLUX(ISL29033_ADCRes ADCRes,ISL29033_LUXrange LUXrange,uint16_t *data)
{
	switch(ADCRes)
	{
	case  RESOLUTION_16b:
		*data=*data*pow(4,LUXrange)*125/65536;
		break;
	case  RESOLUTION_12b:
		*data=*data*pow(4,LUXrange)*125/4096;
		break;
	case  RESOLUTION_8b:
		*data=*data*pow(4,LUXrange)*125/256;
		break;
	case  RESOLUTION_4b:
		*data=*data*pow(4,LUXrange)*125/16;
		break;
	}
}

