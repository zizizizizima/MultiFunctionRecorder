#include "DS_18B20.h"

 /**************************************************************************************
 * ��  �� : ΢����ʱ��ʼ��
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;	// ʹ��DWT����
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk ; 					// ʹ��Cortex-M DWT CYCCNT�Ĵ���
	DWT->CYCCNT = 0; 																// DWT CYCCNT�Ĵ���������0
}

 /**************************************************************************************
 * ��  �� : ΢����ʱ��ʼ��
 * ��  �� : uint32_t
 * ����ֵ : ��
 **************************************************************************************/
void delay_us(uint32_t us) {
		volatile uint32_t cycles = (SystemCoreClock / 1000000L) * us;
		volatile uint32_t start = DWT->CYCCNT;
		do  {
		} while(DWT->CYCCNT - start < cycles);
}

 /**************************************************************************************
 * ��  �� : ����DS18B20�õ���I/O��
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
static void DS18B20_GPIO_Config(void)
{		
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitStruct.Pin = DS18B20_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
	
	DS18B20_DATA_OUT(1);	 //DS18B20�������ų�ʼ������Ϊ�ߵ�ƽ���
}

 /**************************************************************************************
 * ��  �� : ����ʹDS18B20-DATA���ű�Ϊ����ģʽ
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
static void DS18B20_Mode_IPU(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DS18B20_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

 /**************************************************************************************
 * ��  �� : ����ʹDS18B20-DATA���ű�Ϊ���ģʽ
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
static void DS18B20_Mode_Out_PP(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DS18B20_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
}

 /**************************************************************************************
 * ��  �� : �������ӻ����͸�λ����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
static void DS18B20_Rst(void)
{
	DS18B20_Mode_Out_PP();     //��������Ϊ������� 
	
	DS18B20_DATA_OUT(LOW);     //�������ٲ���480us�ĵ͵�ƽ��λ�ź�
	delay_us(750);
	DS18B20_DATA_OUT(HIGH);   //�����ڲ�����λ�źź��轫��������
	delay_us(15);   //�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
}

 /**************************************************************************************
 * ��  �� : ���ӻ����������صĴ�������
 * ��  �� : ��
 * ����ֵ : 0���ɹ�   1��ʧ��  
 **************************************************************************************/
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	DS18B20_Mode_IPU();    //��������Ϊ��������
	
	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		delay_us(1);
	}	

	if( pulse_time >=100 )  //����100us�󣬴������嶼��û�е���
		return 1;             //��ȡʧ��
	else                 //����100us�󣬴������嵽��
		pulse_time = 0;    //�����ʱ����
	
	while( !DS18B20_DATA_IN() && pulse_time<240 )  // �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us
	{
		pulse_time++;
		delay_us(1);
	}	
	if( pulse_time >=240 ) // �������嵽�����Ҵ��ڵ�ʱ�䳬����240us
		return 1;        //��ȡʧ��
	else
		return 0;
}

 /**************************************************************************************
 * ��  �� : ��DS18B20��ȡһ��bit
 * ��  �� : ��
 * ����ֵ : uint8_t 
 **************************************************************************************/
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_DATA_OUT(LOW);
	delay_us(10);
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_Mode_IPU();
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
	delay_us(45);
	
	return dat;
}

 /**************************************************************************************
 * ��  �� : ��DS18B20��һ���ֽڣ���λ����
 * ��  �� : ��
 * ����ֵ : uint8_t  
 **************************************************************************************/
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		//��DS18B20��ȡһ��bit
		dat = (dat) | (j<<i);
	}
	
	return dat;																																																																																
}

 /**************************************************************************************
 * ��  �� : дһ���ֽڵ�DS18B20����λ����
 * ��  �� : uint8_t
 * ����ֵ : ��  
 **************************************************************************************/
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			delay_us(8);   //1us < �����ʱ < 15us
			
			DS18B20_DATA_OUT(HIGH);
			delay_us(58);    //58us+8us>60us
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);  
			/* 60us < Tx 0 < 120us */
			delay_us(70);
			
			DS18B20_DATA_OUT(HIGH);			
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			delay_us(2);
		}
	}
}

 /**************************************************************************************
 * ��  �� : ��ʼDS18B20
 * ��  �� : ��
 * ����ֵ : ��  
 **************************************************************************************/
void DS18B20_Start(void)
{
	DS18B20_Rst();	           //�������ӻ����͸�λ����
	DS18B20_Presence();	       //���ӻ����������صĴ�������
	DS18B20_Write_Byte(0XCC);		 // ���� ROM 
	DS18B20_Write_Byte(0X44);		 // ��ʼת�� 
}

 /**************************************************************************************
 * ��  �� : DS18B20��ʼ������
 * ��  �� : ��
 * ����ֵ : uint8_t
 **************************************************************************************/
uint8_t ds18b20_init(void)
{
	DWT_Init();
	
	DS18B20_GPIO_Config();   
	DS18B20_Rst();
	
	return DS18B20_Presence();
}

 /**************************************************************************************
 * ��  �� : ��DS18B20��ȡ�¶�ֵ
 * ��  �� : ��  
 * ����ֵ : float 
 **************************************************************************************/
float ds18b20_read(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(0X44);				/* ��ʼת�� */
	
	DS18B20_Rst();
  DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* ���� ROM */
  DS18B20_Write_Byte(0XBE);				/* ���¶�ֵ */
	
	tplsb = DS18B20_Read_Byte();
	tpmsb = DS18B20_Read_Byte();
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = (s_tem * 0.0625);	  
	return f_tem; 	
}


/*************************************END OF FILE******************************/

