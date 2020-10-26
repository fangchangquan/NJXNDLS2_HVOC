#include "humidity.h"
#include "i2c.h"
#include "adc.h"


TphDef tph;
uchar bmpcnt_t=0,bmpcnt_p=0;

int32_t t_fine;

double var_h;
	 
//===============================================================
/*********************************************************************************
** 函数名称: Read_One_Register
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
u8 Read_One_Register(uchar addr)
{
	 u8 temp=0;
	
	 temp=Bme280_Read_OneByte(addr);
	
	 return temp;
}
//===============================================================
/*********************************************************************************
** 函数名称: Read_Two_Register
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
short Read_Two_Register(uchar addr)
{
	 short temp=0;
	 
	 
	 temp=(short)Bme280_Read_OneByte(addr+1);
	 temp<<=8;
	 temp|=(short)Bme280_Read_OneByte(addr);
	 
	 return temp;
}
//===============================================================
/*********************************************************************************
** 函数名称: Read_TPH_Register
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Read_TPH_Register(void)
{
	 
	 tph.dig_t1=Read_Two_Register(0x88);
	 tph.dig_t2=Read_Two_Register(0x8A);
   tph.dig_t3=Read_Two_Register(0x8C);
 	
	 tph.dig_p1=(u16)Read_Two_Register(0x8E);
	 tph.dig_p2=Read_Two_Register(0x90);
	 tph.dig_p3=Read_Two_Register(0x92);
	 tph.dig_p4=Read_Two_Register(0x94);
	 tph.dig_p5=Read_Two_Register(0x96);
	 tph.dig_p6=Read_Two_Register(0x98);
	 tph.dig_p7=Read_Two_Register(0x9A);
	 tph.dig_p8=Read_Two_Register(0x9C);
	 tph.dig_p9=Read_Two_Register(0x9E);
	
	 tph.dig_h1=Read_One_Register(0xA1);
	 tph.dig_h2=Read_Two_Register(0xE1);
	 tph.dig_h3=Read_One_Register(0xE3);
	 tph.dig_h4=(short)(Read_One_Register(0xE4)<<4)|(short)((Read_One_Register(0xE5)&0x0f));
	 tph.dig_h5=(short)(Read_One_Register(0xE6)<<4)|(short)((Read_One_Register(0xE5)&0xf0)>>4);
	 tph.dig_h6=(signed char)Read_One_Register(0xE7);
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Bmp280_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Bmp280_Init(void)
{
   Bme280_Write_OneByte(0xe0,0xb6);
	 Delay_Ms(10);
	 Bme280_Write_OneByte(0xf2,0x07);
	 Delay_Ms(10);
	 Bme280_Write_OneByte(0xf4,0xff);
	 Delay_Ms(10);
	 Bme280_Write_OneByte(0xf5,5<<2);
	 Delay_Ms(10);
	 
	 Read_TPH_Register();
	 
	 Delay_Ms(500);
	
	 tph.id=Bme280_Read_OneByte(0xd0);
}

//===============================================================
/*********************************************************************************
** 函数名称: Bmp180_Get_UT
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
long  Bmp180_Get_UT(void)				  //读取未补偿温度值UT
{ 	
	  u8 msb,lsb,xlsb;
	  long ut;
		
	  msb=Bme280_Read_OneByte(TEM_ADDR);
	  lsb=Bme280_Read_OneByte(TEM_ADDR+1);
	  xlsb=Bme280_Read_OneByte(TEM_ADDR+2);
	  ut=(long)(((uint32)msb<<12)|((uint32)lsb<<4)|((uint32)xlsb>>4));
	  	
	  return ut;	
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Bmp180_Get_UP
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
long  Bmp180_Get_UP(void)				  //读取未补偿压力值UP
{ 	
	  u8 msb,lsb,xlsb;
	  s32 up;
		
	  msb=Bme280_Read_OneByte(PRESS_ADDR);
	  lsb=Bme280_Read_OneByte(PRESS_ADDR+1);
	  xlsb=Bme280_Read_OneByte(PRESS_ADDR+2);
	  up=(long)(((uint32)msb<<12)|((uint32)lsb<<4)|((uint32)xlsb>>4));  
		
	  return up;	
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Bmp180_Get_UH
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
long  Bmp180_Get_UH(void)				  //读取未补偿湿度值UH
{ 	
	  u8 msb,lsb;
	  long uh;
		
	  msb=Bme280_Read_OneByte(HUM_ADDR);
	  lsb=Bme280_Read_OneByte(HUM_ADDR+1);

	  uh=(long)(msb<<8);
	  uh|=(long)lsb;
		
	  return uh;	
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Bme280_Compensate_Temperature
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
int32 Bme280_Compensate_Temperature(int32 adc_t)//*100
{
	 int32_t var1,var2,t=0;
	
	 var1 = ((((adc_t>>3) - ((int32)tph.dig_t1<<1))) * ((int32)tph.dig_t2)) >> 11;
   var2 = (((((adc_t>>4) - ((int32)tph.dig_t1)) * ((adc_t>>4) - ((int32)tph.dig_t1))) >> 12) * ((int32)tph.dig_t3)) >> 14;
	
   t_fine = var1 + var2;
   t = (t_fine * 5 + 128) >> 8;

   return t;
   
}
//===============================================================
/*********************************************************************************
** 函数名称: Bme280_Compensate_Temperature_Float
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
int32_t Bme280_Compensate_Temperature_Float(int32_t adc_t)//100
{
	  double var1,var2;
	  double t=0;
		
	  var1 = (((double)adc_t)/16384.0-((double)tph.dig_t1)/1024.0)*((double)tph.dig_t2);
    var2 = ((((double)adc_t)/131072.0-((double)tph.dig_t1)/8192.0)*(((double)adc_t)
                /131072.0-((double)tph.dig_t1)/8192.0))*((double)tph.dig_t3);

    t_fine = (long)(var1+var2);

    t = ((var1+var2))/5120.0;
		
		return t;

}
//===============================================================
/*********************************************************************************
** 函数名称: Bme280_Compensate_Pressure
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
int32 Bme280_Compensate_Pressure(int32 adc_p)
{
	int32_t var1,var2,p;
	
	var1 = (((int32)t_fine)>>1) - (int32)64000;
  var2 = (((var1>>2) * (var1>>2))>>11) * ((int32)tph.dig_p6);
  var2 = var2 + ((var1*((int32)tph.dig_p5))<<1);
  var2 = (var2>>2) + (((int32)tph.dig_p4)<<16);
  var1 = (((tph.dig_p3*(((var1>>2)*(var1>>2))>>13))>>3) + ((((int32)tph.dig_p2)*var1)>>1))>>18;
  var1 = (((32768+var1)*((int32)tph.dig_p1))>>15);
  if (var1 == 0)
  {
    return 0;
  }
 
  p= (uint32)((uint32)(((uint32)1048576)-adc_p)-(uint32)(var2>>12))*3125;
  if(p<0x80000000)
	{
		 p=(p<<1)/((uint32)var1);
	}
	else
	{
		 p=(p/((uint32)(var1)))*2;
	}
 
  var1 = (((int32)tph.dig_p9) *((int32) (((p>>3) * (p>>3))>>13))) >> 12;
  var2 = (((int32)(p>>2))*((int32)tph.dig_p8)) >> 13;
  p = (uint32)((int32)p+ ((var1 + var2+tph.dig_p7)>>4));
 
  return p;


}
//===============================================================
/*********************************************************************************
** 函数名称: Bme280_Compensate_Pressure_Float
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
int32_t Bme280_Compensate_Pressure_Float(int32_t adc_p)
{
	  int32_t var1,var2,p;
	
	  var1 = ((double)t_fine/2.0)-64000.0;
    var2 = var1*var1*((double)tph.dig_p6)/32768.0;
    var2 = var2 +var1*((double)tph.dig_p5)*2.0;
    var2 = (var2/4.0)+(((double)tph.dig_p4)*65536.0);
    var1 = (((double)tph.dig_p3)*var1*var1/524288.0+((double)tph.dig_p2)*var1)/524288.0;
    var1 = (1.0+var1/32768.0)*((double)tph.dig_p1);
    p = 1048576.0-(double)adc_p;
    p = (p-(var2/4096.0))*6250.0/var1;
    var1 = ((double)tph.dig_p9)*p*p/2147483648.0;
    var2 = p*((double)tph.dig_p8)/32768.0;
    p = p+(var1+var2+((double)tph.dig_p7))/16.0;

    return p;
}
//===============================================================
/*********************************************************************************
** 函数名称: Bme280_Compensate_Hum
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
unsigned long Bme280_Compensate_Hum(long adc_h)
{
    signed long int v_x1;

    v_x1 = (t_fine - ((signed long int)76800));
    v_x1 = (((((adc_h << 14) - (((signed long int)tph.dig_h4) << 20) - (((signed long int)tph.dig_h5) * v_x1)) +
              ((signed long int)16384)) >> 15) * (((((((v_x1 * ((signed long int)tph.dig_h6)) >> 10) *
                      (((v_x1 * ((signed long int)tph.dig_h3)) >> 11) + ((signed long int) 32768))) >> 10) + (( signed long int)2097152)) *
                      ((signed long int) tph.dig_h2) + 8192) >> 14));
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((signed long int)tph.dig_h1)) >> 4));
    v_x1 = (v_x1 < 0 ? 0 : v_x1);
    v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
    return (unsigned long int)(v_x1 >> 12);
}

//===============================================================
/*********************************************************************************
** 函数名称: Bme280_Compensate_Humidity
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
double Bme280_Compensate_Humidity(int32_t adc_h)
{

	 
	 var_h=((double)t_fine)-76800.0;
	 var_h=(adc_h-(((double)tph.dig_h4)*64.0+((double)tph.dig_h5)/16384.0*var_h))*(((double)tph.dig_h2)/65536.0*(1.0+((double)tph.dig_h6)/67108864.0*var_h*(1.0+((double)tph.dig_h3)/67108864.0*var_h)));
   var_h=var_h*(1.0-((double)tph.dig_h1)*var_h/524288.0);
	 if(var_h>100.0)
	 {
		  var_h=100.0;
	 }
	 else if(var_h<0.0)
	 {
		  var_h=0.0;
	 }
	 
	 return var_h;
	
}

//===============================================================
/*********************************************************************************
** 函数名称: Get_Temperature_Value
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_Temperature_Value(void)
{

	 sensor.tem.ut=Bmp180_Get_UT();
	
	 sensor.tem.tem_now=Bme280_Compensate_Temperature(sensor.tem.ut);
	
	 sensor.tem.tem_now /= 10; 

//	 sensor.tem.tem_now=Bme280_Compensate_Temperature_Float(sensor.tem.ut);
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_Press_Value
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_Press_Value(void)
{
	 
	sensor.press.press_up=Bmp180_Get_UP();
	
	sensor.press.press_now=((Bme280_Compensate_Pressure(sensor.press.press_up))/10);//Pa/10  高位值溢出，除以10，否则不能正确显示
	
//	sensor.press.press_now=Bme280_Compensate_Pressure_Float(sensor.press.press_up);
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_Humidity_Value
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_Humidity_Value(void)
{
	 
	sensor.hum.hum_uh=Bmp180_Get_UH();
	
//	sensor.hum.hum_now=Bme280_Compensate_Hum(sensor.hum.hum_uh)/1024;
	sensor.hum.hum_now=(Bme280_Compensate_Humidity(sensor.hum.hum_uh));
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Bme280_Compensate_Humidity
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/

//===============================================================
/*********************************************************************************
** 函数名称: Bme280_Compensate_Humidity
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/


