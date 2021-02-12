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
	ISL29033_RegWrite(COMMAND_REG1,mode|NumOfCycles);
}
//Настройка второго регистра конфигурации
void ISL29033_config2(ISL29033_ADCRes ADCRes,ISL29033_LUXrange LUXrange)
{
	ISL29033_RegWrite(COMMAND_REG2,ADCRes|LUXrange);
}
// Режим низкого энергопотребления ISL29033
void ISL29033_Standby(void)
{
	ISL29033_config1(Standby,cycles_1);
}
// Инициализация ISL29033
void ISL29033_init(ISL29033_mode mode,ISL29033_NumOfCycles NumOfCycles,
		           ISL29033_ADCRes ADCRes,ISL29033_LUXrange LUXrange)
{
	ISL29033_Standby();
	HAL_Delay(1);
	ISL29033_config1(mode,NumOfCycles);
	ISL29033_config2(ADCRes,LUXrange);
}
//Чтение значения освещенности ISL29033
uint16_t ISL29033_DataReadADC(void)
{
	return ISL29033_RegRead(DATA_REG1)+(ISL29033_RegRead(DATA_REG2)<<8);
}
//Перевод значения с АЦП в Люксы
void ISL29033_ADCtoLUX(ISL29033_ADCRes ADCRes,ISL29033_LUXrange LUXrange,uint16_t *data)
{
	switch(ADCRes)
	{
	case  Resolution_16b:
		*data=*data*pow(4,LUXrange)*125/65536;
		break;
	case  Resolution_12b:
		*data=*data*pow(4,LUXrange)*125/4096;
		break;
	case  Resolution_8b:
		*data=*data*pow(4,LUXrange)*125/256;
		break;
	case  Resolution_4b:
		*data=*data*pow(4,LUXrange)*125/16;
		break;
	}
}
