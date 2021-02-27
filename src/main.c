#define MAINWORK_GLOBALS
#include "include.h"
#include "wifi.h"

volatile uint8_t is_read_time = 0;
static void Bsp_Init(void)
{
    Uart1Init();
    //Uart4Init();
    wifi_protocol_init();//wifi
	I2C_init(4);          //I2C???
	OLED_Init();         //OLED???
	OLED_Clear();        //OLED??
	timer0_init();       //??????	
    SHT_Init();
}
void welcome()
{
	//OLED_DrawBMP(0,0,16,1,UI);
    
    I2C_init(4);
	OLED_ShowStr(0,1," --CH3-Orange--",16);
    //sht30_read();
	OLED_ShowCHinese(0,4,0);
    OLED_ShowCHinese(16,4,1);
	OLED_ShowCHinese(72,4,2);
	OLED_ShowCHinese(88,4,1);
	OLED_ShowCHinese(0,6,3);
	OLED_ShowCHinese(16,6,4);
	OLED_ShowCHinese(32,6,0);
	OLED_ShowCHinese(48,6,1);
    
}
/*void delay500ms(void)   //Îó²î 0us
{
    unsigned char a,b,c;
    for(c=23;c>0;c--)
        for(b=152;b>0;b--)
            for(a=70;a>0;a--);
}*/
void wifi_work_state_led(void)
{
  uint8_t wifi_state = mcu_get_wifi_work_state();
	switch(wifi_state)
	{
	case SMART_CONFIG_STATE:            //smartconfig配置状态
	  OLED_ShowStr(0,0,"  S! ",8);
		break;
		
	case AP_STATE:                      //AP配置状态
	  OLED_ShowStr(0,0,"  A! ",8);
		break;
		
	case WIFI_NOT_CONNECTED:            //WIFI配置成功但未连上路由器
		OLED_DrawBMP(0,0,24,1,UI1);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_CONNECTED:                //WIFI配置成功且连上路由器
		OLED_DrawBMP(0,0,24,1,UI2);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_CONN_CLOUD:               //WIFI已经连接上云服务器
		OLED_DrawBMP(0,0,24,1,UI3);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case WIFI_LOW_POWER:                //WIFI处于低功耗模式
		OLED_DrawBMP(0,0,24,1,UI4);
		OLED_ShowStr(24,0,"  ",8);
		break;
		
	case SMART_AND_AP_STATE:            //WIFI smartconfig&AP 模式
		OLED_ShowStr(0,0,"  SA ",8);
		break;
		
	default:                            //未知状态
		OLED_ShowStr(0,0,"error",8);
	 break;
	}
}
void main()
{
	Bsp_Init();
	
	
    /*mcu_set_wifi_mode(0);
    if((mcu_get_wifi_work_state() == SMART_CONFIG_STATE) || (mcu_get_wifi_work_state() == AP_STATE))
	{
        I2C_init(4);
		OLED_ShowStr(24,1,"reset wifi",16);
	}*/
    welcome();
    sht30_read(); 
    while(1)
    {
        wifi_uart_service();
        wifi_work_state_led();
       if(is_read_time)
       {
           sht30_read();
           is_read_time=0;
       }

    }

    return ;
}