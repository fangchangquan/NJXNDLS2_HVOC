#include "adc.h"

s16 table_t[]   ={1000,950, 900, 850, 800, 750, 700, 650, 600,  550, 500,  450,   400,  350,  300,  250,  200,  150,  100,   50,    0, -50,  -100, -150,-200};
u16 table_data[]={3652,4096,4600,5190,5828,6580,7433,8398,9484,10693,12033,13501,15533,16798,18601,20480,22409,24370,26316,28228,30075,31699,33313,34682,33781};

volTypeDef vol;

DefSensor  sensor;
	
uchar abc=0;
u32 ADC_value[6]={0};
u16 ADC_value1[NUM3]={0};
u16 ADC_value2[NUM3]={0};
u16 ADC_value3[NUM3]={0};
u16 ADC_value4[NUM3]={0};
u16 ADC_value5[NUM3]={0};
u16 ADC_value6[NUM3]={0};

s16 filter_data[N0]={0};
u16 value=0;

s16 t_temp=0;
u16 vol_dat[100]={0};


__IO u32 ADC_ConverValue[6]={0};

//===============================================================
/*********************************************************************************
** 函数名称: ADC_GPIO_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void ADC_GPIO_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE);
	
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_5|GPIO_Pin_6;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
   GPIO_Init(GPIOA,&GPIO_InitStructure);
	 
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
   GPIO_Init(GPIOC,&GPIO_InitStructure);
   
}

//===============================================================
/*********************************************************************************
** 函数名称:ADC_DMA_Init
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void ADC_DMA_Init(void)
{
DMA_InitTypeDef DMA_InitStructure;
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
DMA_DeInit(DMA2_Stream0);


//DMA_Cmd(DMA2_Stream0, DISABLE);
DMA_InitStructure.DMA_Channel = DMA_Channel_0; 	
DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&(ADC1->DR);//ADC1_DR_Address;
DMA_InitStructure.DMA_Memory0BaseAddr=(u32)&ADC_ConverValue;
DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;
DMA_InitStructure.DMA_BufferSize=(u32)6;//(3*100);
DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;//Word;//HalfWord
DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
DMA_InitStructure.DMA_Priority=DMA_Priority_High;
DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;                  //DMA FIFO 
DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
DMA_Init(DMA2_Stream0,&DMA_InitStructure);

//DMA_Cmd(DMA2_Stream0,ENABLE);

// DMA_DoubleBufferModeConfig(DMA1_Stream0,(uint32_t)(u16 *)(ADC_ConverValue),DMA_Memory_0);    //DMA_Memory_0?????
// DMA_DoubleBufferModeCmd(DMA1_Stream0,ENABLE);	


}
//===============================================================
/*********************************************************************************
** 函数名称: ADC_Start
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void ADC_Start(FunctionalState stat)
{

ADC_Cmd(ADC1,stat);
}

//===============================================================
/*********************************************************************************
** 函数名称: ADC_Initial
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void ADC_Initial(void)
{
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;	
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);


ADC_GPIO_Init();
ADC_DMA_Init();
	
RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);                 //ADC1??  
RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE); 
	
ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                    //????:????(??????ADC?)
ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;                 //?????
ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;     //ADC_DMAAccessMode_3
ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//5
ADC_CommonInit(&ADC_CommonInitStructure);
	
ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  
ADC_InitStructure.ADC_ScanConvMode=ENABLE;          //DISABLE;
ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;		//ENABLE
ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
ADC_InitStructure.ADC_ExternalTrigConv =0;
ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
ADC_InitStructure.ADC_NbrOfConversion=6;
ADC_Init(ADC1,&ADC_InitStructure);

ADC_Start(ENABLE);

ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

//ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);

ADC_DMACmd(ADC1, ENABLE);	

ADC_Cmd(ADC1, ENABLE);

DMA_Cmd(DMA2_Stream0,ENABLE);

#if ADC1_CHANNEL0_EN>0
ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_144Cycles);
#endif	

#if ADC1_CHANNEL1_EN>0
ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_144Cycles);
#endif	

#if ADC1_CHANNEL2_EN>0
ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_144Cycles);
#endif	

#if ADC1_CHANNEL5_EN>0
ADC_RegularChannelConfig(ADC1,ADC_Channel_5,4,ADC_SampleTime_480Cycles);
#endif	

#if ADC1_CHANNEL6_EN>0	
ADC_RegularChannelConfig(ADC1,ADC_Channel_6,5,ADC_SampleTime_480Cycles);
#endif

#if ADC1_CHANNEL15_EN>0
ADC_RegularChannelConfig(ADC1,ADC_Channel_15,6,ADC_SampleTime_480Cycles);
#endif	



ADC_SoftwareStartConv(ADC1);

}
//===============================================================
/*********************************************************************************
** 函数名称: Get_ADC
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：5ms一次
*********************************************************************************/
void Get_ADC(void)										//查询的方法
{
	static u16 index=0;
	
	if(index>=ADC_DATA)				  //100ADC_DATA
	{
		 index=0;
		
		 if(abc<NUM3) //NUM3=10
		 {   
			   #if ADC1_CHANNEL0_EN>0
			   ADC_value1[abc]=(ADC_value[0])/ADC_COUNT;	  //*10	
			   #endif
			   #if ADC1_CHANNEL1_EN>0
			   ADC_value2[abc]=(ADC_value[1])/ADC_COUNT;	  //*10	
			   #endif
			   #if ADC1_CHANNEL2_EN>0
			   ADC_value3[abc]=(ADC_value[2])/ADC_COUNT;	  //*10	
			   #endif
			   #if ADC1_CHANNEL5_EN>0
			   ADC_value4[abc]=(ADC_value[3])/ADC_COUNT;	  //*10	
			   #endif
			   #if ADC1_CHANNEL6_EN>0
			   ADC_value5[abc]=(ADC_value[4])/ADC_COUNT;	  //*10	
			   #endif
			   #if ADC1_CHANNEL15_EN>0
			   ADC_value6[abc]=(ADC_value[5])/ADC_COUNT;	  //*10	
			   #endif
			 
				 abc++;
			}
		 else
		 {
			   abc=0;
			   #if ADC1_CHANNEL0_EN>0
			   ADC_value[0]=0;
			   #endif
			   #if ADC1_CHANNEL1_EN>0
			   ADC_value[1]=0;
			   #endif
			   #if ADC1_CHANNEL2_EN>0
			   ADC_value[2]=0;
			   #endif
			   #if ADC1_CHANNEL5_EN>0
			   ADC_value[3]=0;
			   #endif
			   #if ADC1_CHANNEL6_EN>0
			   ADC_value[4]=0;
			   #endif
			   #if ADC1_CHANNEL15_EN>0
			   ADC_value[5]=0;
			   #endif
		 }
		 #if ADC1_CHANNEL0_EN>0
		 ADC_ConverValue[0]=0;
		 #endif
		 #if ADC1_CHANNEL1_EN>0
		 ADC_ConverValue[1]=0;
		 #endif
		 #if ADC1_CHANNEL2_EN>0
		 ADC_ConverValue[2]=0;
		 #endif
		 #if ADC1_CHANNEL5_EN>0
		 ADC_ConverValue[3]=0;
		 #endif
		 #if ADC1_CHANNEL6_EN>0
		 ADC_ConverValue[4]=0;
		 #endif
		 #if ADC1_CHANNEL15_EN>0
		 ADC_ConverValue[5]=0;
		 #endif
		 
		 
	}
	else
	{   
		#if ADC1_CHANNEL0_EN>0				
		ADC_value[0]+=ADC_ConverValue[0];
	  #endif
			
		#if ADC1_CHANNEL1_EN>0
	  ADC_value[1]+=ADC_ConverValue[1];
		//ADC_ConverValue[1]=ADC_GetConversionValue(ADC1);
	  #endif
		
		#if ADC1_CHANNEL2_EN>0				
		ADC_value[2]+=ADC_ConverValue[2];
	  #endif
			
		#if ADC1_CHANNEL5_EN>0
	  ADC_value[3]+=ADC_ConverValue[3];
	  #endif
		
    #if ADC1_CHANNEL6_EN>0				
		ADC_value[4]+=ADC_ConverValue[4];
	  #endif
			
		#if ADC1_CHANNEL15_EN>0
	  ADC_value[5]+=ADC_ConverValue[5];
	  #endif		
		index++;
	} 										
}
//===============================================================
/*********************************************************************************
** 函数名称: Filter_Temp
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
u16 Filter_Temp(u16 queue[],uchar n)
{
u32 sum=0;
u32 max=0,min=0;
uchar i;
max=queue[0];
min=queue[0];
sum=queue[0];
for(i=0;i<n-1;i++)
{
	if(queue[i]>max) max=queue[i];
	else if(queue[i]<min) min=queue[i];
	sum=sum+queue[i];
	queue[i+1]=queue[i];
}
i=n-2;
sum=sum-max-min+i/2;
sum=sum/i;
return (s16)sum;
}

u16 Filter_Temp_1(u16 queue[],uchar n)
{
u32 sum=0;
uchar i;

for(i=0;i<n;i++)
{
	sum=sum+queue[i];
}
sum=sum/n;
return (s16)sum;
}
//===============================================================
/*********************************************************************************
** 函数名称: Linear_onepart
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
s16 Linear_onepart(u16 xn,u16 x1,u16 x2,s16 y1,s16 y2)	  //单区间线性插值，已知x1<=x2，直线两端点坐标（x1,y1)和(x2,y2)，已知该线段任意一点X坐标xn
{
s32 yn; //被求点的Y坐标
u16 tmp,y3;  //暂存变量
y3=y2;
if(xn<x1)
{
if(xn==0)
yn=0;
else
yn=y1;
}
else if(xn>x2)
{
yn=y2;
}
else
{
if(y2>=y1)
yn=(y2-y1);	 //取绝对值
else
yn=(y1-y2);
if(xn>=x1)	
tmp=xn-x1;
else
tmp=x1-xn;
yn=yn*tmp;
if(x2>=x1)
tmp=x2-x1;
else
tmp=x1-x2;
if(yn>=0)
yn=yn+(tmp/2);	 //四舍五入	
else
yn=yn-tmp/2;
yn=yn/tmp;
if(y3>y1)
yn=y1+yn;
else
yn=y1-yn;
}
return((s16)yn);	//转成整形值
}
//===============================================================
/*********************************************************************************
** 函数名称: linear_allpart
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
s16 linear_allpart(u16 xn,u16 queue_x[],s16 queue_y[],uchar n)	  //整个区间线性插值,p;标定点x坐标构成数组序列，q：标定点y坐标构成数组序列,n:序列长度
{														              //已知x1<=x2<=x3...<=xn
uchar i;
s16 yn;
for(i=1;i<(n-1);i++)
{
if(xn<=queue_x[i])
break;
}
yn=Linear_onepart(xn,queue_x[i-1],queue_x[i],queue_y[i-1],queue_y[i]);	  //调单区间线性插值函数
return yn;
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_AD
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
u16 Get_AD(uchar value_temp)
{
s16 val=0;
u16 sdat1=0,sdat2,sdat3=0,sdat4=0,sdat5=0,sdat6=0;
s16 tmp1=0;
	
sdat1=Filter_Temp(ADC_value1,NUM3);
sdat2=Filter_Temp(ADC_value2,NUM3);
sdat3=Filter_Temp(ADC_value3,NUM3);
sdat4=Filter_Temp(ADC_value4,NUM3);
sdat5=Filter_Temp(ADC_value5,NUM3);
sdat6=Filter_Temp(ADC_value6,NUM3);
	
tmp1=linear_allpart(sdat2,table_data,table_t,N3);
	
switch(value_temp)
{
case PMV      :val=sdat1;	             
			         break;
case TEMP     :val=tmp1;
			         break;
case VOL      :val=sdat3;	             
			         break;
case O3V      :val=sdat4;	             
			         break;
case NO2V     :val=sdat5;	             
			         break;
case SO2V     :val=sdat6;	             
			         break;
default	      :break;
}

return val;
}
//===============================================================
/*********************************************************************************
** 函数名称: Glide_average_value_filter
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
u16 Glide_average_value_filter(uchar glide_temp)	//递推平均滤波
{
	uchar i;
	static uchar filter_count=0;									        //sum:连续采样之和，value:求得平均值
	s16 value;
	s32 sum;
	sum=0;
	value=0;
	for(i=0;i<N0;i++)
	{
		filter_data[i]=Get_AD(glide_temp);	    //采集数据放到数组
	}
	if(filter_count<N0)
	{
		filter_data[filter_count++]=Get_AD(glide_temp); 	   //filter_data[filter_count++]=Get_ad(glide_temp); 
		if(filter_count==N0)
		filter_count=0;	
	}		    
	for(i=0;i<N0;i++)
	{				 										  
		sum+=filter_data[i];						//求和
	}
	sum=(sum/N0);
	value=(s16)sum;								//求平均              
	return value;
} 
//===============================================================
/*********************************************************************************
** 函数名称: Get_PM_Data
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_PM_Data(void)
{
	u32 temp=0;
	
	temp=Glide_average_value_filter(PMV);
	temp=(temp*3300)/40960;
  sensor.pm.pm_dat=(u16)temp;
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_Temp_Data
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_Temp_Data(void)
{	
  	
   t_temp=Glide_average_value_filter(TEMP);
	 
	  	
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_Vol_Data
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_Vol_Data(void)
{	
  u32 temp=0;
	static uchar vol_cnt=0;
	
	temp=Glide_average_value_filter(VOL);
	temp=(temp*3300)/40960;
	vol.vol_dat=(u16)(temp-20);//100
	if(vol_cnt<100)
	{ 
		
		 if(vol.vol_dat<5000)
		 {
		    vol_dat[vol_cnt]=vol.vol_dat;
			  vol.vol_now=Filter_Temp(vol_dat,100);
		 }
		 
		 vol_cnt++;
	}
	else
	{
		 vol_cnt=0;
		
		 vol.vol_now=Filter_Temp(vol_dat,100);
	}
	
	
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_O3_Data
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_O3_Data(void)
{
 	u32 temp=0;
	
	temp=Glide_average_value_filter(O3V);
	temp=(temp*3300)/40960;
	sensor.o3.o3_dat=(u16)temp;
	
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_No2_Data
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_No2_Data(void)
{
	 u32 temp=0;
	 
	 temp=Glide_average_value_filter(NO2V);
	 temp=(temp*3300)/40960;
	 sensor.no2.no2_dat=(u16)temp;	
}
//===============================================================
/*********************************************************************************
** 函数名称: Get_So2_Data
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
void Get_So2_Data(void)
{
	 u32 temp=0;
	
	 temp=Glide_average_value_filter(SO2V);
	 temp=(temp*3300)/40960;
	 sensor.so2.so2_dat=(u16)temp;
	 
}

//===============================================================
/*********************************************************************************
** 函数名称: Get_Tmp_Vol_O3_NO2_SO2_Data
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无10ms
*********************************************************************************/
void Get_Tmp_Vol_O3_NO2_SO2_Data(void)
{
	 
	  Get_Temp_Data();
		 
		Get_Vol_Data();
		  
		Get_O3_Data();
	
	  Get_No2_Data();
	 
	  Get_So2_Data();
}
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/
//===============================================================
/*********************************************************************************
** 函数名称: Double_Led_Charge_Control
** 编写者  ：CZM
** 函数功能：调节等离子体强度
** 输入参数：无
** 输出参数：无
** 备    注：无
*********************************************************************************/









