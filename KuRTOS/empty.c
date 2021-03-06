/***********************************************************************/
/*                                                                     */
/*  FILE        :Empty.c                                               */
/*  DATE        :Mon, Jan 08, 2007                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :Other                                                 */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.0).     */
/*                                                                     */
/***********************************************************************/
         
#include "qsk_bsp.h"    
#include "qsk_lcd.h"          
#include "schedule.h"   
#include "sched_policy.h" 
#include "sched_rm.h" 
#include "sched_wrr.h"
#include "context_switch.h"
#include "device_manager.h"
#include "clock.h"
#include "itoa.h"
#include "led_lib.h"
#include "sched.h"  
#include "message_queue.h"
#include "msgq_lib.h"
#include "switch_lib.h"
#include "semaphore.h"
#include "spinlock.h"
#include "mailbox.h"
// prototypes

void mcu_init(void);
void dummy();	              
void program1();
void program2();
void program3(); 
semaphore s;
spinlock lock;
/* *******************************************************************
 Description:
 Entry:
 Returns:
 
 Notes:
 
***********************************************************************/ 

#define TOP(x)	((x)+sizeof(x)-sizeof(OS_STK))

display_with_lock(int line, char *arg)
{
	P(&s);
	if(line == 1) { 	DisplayString(LCD_LINE1, arg); }
	else { 	DisplayString(LCD_LINE2, arg); }	
	V(&s);
}

void main(void)
{
	OS_STK *stk;
	int i = 0;
	mcu_init(); 
	InitDisplay();   
	init_device_with_support();
	init_message_queue_devices();
	init_tick();
	init_scheduler(RMN);   
	initSemaphore(&s, 1);
	init_lock(&lock);
	SAVE_ISP(stk);
//	*stk = tempStk; 

   //	DisplayString(LCD_LINE1, "###");   

/*
	task_init(dummy, (OS_STK *) (stk-50));
	task_init(program1, (OS_STK *) (stk-150));
	task_init(program2, (OS_STK *) (stk-250));
	task_init(program3, (OS_STK *) (stk-350)); 
*/	 
	
		 

	task_init_rate_monotonic(0, 1, dummy, (OS_STK *) (stk-50));
	task_init_rate_monotonic(10, 2, program1, (OS_STK *) (stk-150));
	task_init_rate_monotonic(15, 4, program2, (OS_STK *) (stk-250));
	task_init_rate_monotonic(35, 2, program3, (OS_STK *) (stk-350)); 
		 
/*
	task_init_sched_wrr((INT32U)(-1), dummy, (OS_STK *) (stk-50));
	
  // 	DisplayString(LCD_LINE1, "###1"); 
 	task_init_sched_wrr(1, program1, (OS_STK *) (stk-150));
	
  // 	DisplayString(LCD_LINE1, "###2"); 
	task_init_sched_wrr(2, program2, (OS_STK *) (stk-250));
	task_init_sched_wrr(3, program3, (OS_STK *) (stk-350)); 
*/
 	InitTimerA0();
	InitTimerA1();    
	
}
   


   
void program1() 
{
		int j  = 0;	
		
		int s1 = -1;
		int s2 = -1;
		int s3 = -1;
		int ret = -1;
		int ch_one = -1;
		int recv_buf = -1;
		int sbuf1 = -1;
		int sbuf2 = -1;
		int sbuf3 = -1;
		
		char p[9] = "Program1";
		char rcv_p[9] = "RECV_CH1";
		char sw_one[9]="SWITCH1E";
		char sw_two[9]="SWITCH2E";
		char sw_three[9]="SWITCH3E";
		char fail[9] = "FAIL";
		
		s1 = request_switch(SWITCH1);
		s2 = request_switch(SWITCH2);
		s3 = request_switch(SWITCH3);
		
		request_led(GLED);
		led_on(GLED);
		  
		ch_one =	request_channel(1);


		if( s1 != SWITCH1 ) 
   		{
			fail[8]= fail[5];
			fail[5]= '-' ;
			fail[6]= 'S' ;
			fail[7]= '1' ;
			display_with_lock(2, fail);
		}
		if( s2 != SWITCH2 )
		{
			fail[8]= fail[5];
			fail[5]= '-' ;
			fail[6]= 'S' ;
			fail[7]= '2' ;
			display_with_lock(2, fail);			
		}
		if( s3 != SWITCH3 )
		{
			fail[8]= fail[5];
			fail[5]= '-' ;
			fail[6]= 'S' ;
			fail[7]= '3' ;
			display_with_lock(2, fail);			
		}
		if( ch_one != 1 )
		{
			fail[8]= fail[5];
			fail[5]= '-' ;
			fail[6]= 'C' ;
			fail[7]= '1' ;
			display_with_lock(2, fail);
		} 
	
		while(1) 
		{
			
		sbuf1 = -1;
		sbuf2 = -1;
		sbuf3 = -1;
		get_switch(s1, &sbuf1);
		get_switch(s2, &sbuf2);
		get_switch(s3, &sbuf3);	
		
			ret = recv_msg(1, &recv_buf);
		if(recv_buf == 2)
		{
					display_with_lock(1, rcv_p);
					recv_buf = -1;
		}
		else
		{
					display_with_lock(1, p);
		}			
		if( sbuf1 == 1 ){
			/*  Send Message */ 
			ret = send_msg(ch_one, 1);
			display_with_lock(2, sw_one);
		}
		if( sbuf2 == 1)
		{
			send_mail(3, 1);
			display_with_lock(2, sw_two);
		}
		if( sbuf3 == 1)
		{			

			led_switch(GLED);
			display_with_lock(2, sw_three);	
		}

	//	periodic_wait();
		}
}

 
void program2()
{
	int j = 0;
	int buf = 55;
	int mailbuf = -1;
	int ch_one = -1;
	int ret = -1;
	int fd = -1;
	int red = RLED;
	char p[9]  = "Program2";
	char ledmsg[9] = "LED RED ";
	int led_service = SW;
		ch_one =	request_channel(1);
		request_led(RLED);
		led_on(RLED);
		while(1)
		{
			
			
		display_with_lock(1, p);

			if( ch_one != 1 ) { request_channel(1); }
			else {
			ret = recv_msg(1, &buf);
				while( ret != 0 )
				{
					mailbuf = buf;
					ret = recv_msg(1, &buf);
				}
			if( buf == 1 || mailbuf == 1)
			{
				display_with_lock(2, ledmsg);
				send_msg(ch_one, 2);
				led_switch(RLED);
				buf = -1;	
				mailbuf = -1;
			}
			}

			for(j = 0; j < 30000; j++ )
			{
				_asm("nop");
			}

	//	periodic_wait();
		}
}  

void program3()
{
	int j = 0;
	int buf = -1;
	int ch_two = -1;
	int fd = -1;
	int red = YLED;
	int ret = -1;
	char p[9]  = "Program3";
	char ywlled[9] = "YWL LED ";
	int led_service = SW;
	
	
	
		request_led(YLED);
		led_on(YLED);
		
		while(1)
		{
			
	
		display_with_lock(1, p);
			recv_mail(&buf);
			
			if( buf == 1 )
			{
			
			display_with_lock(2, ywlled);
				led_switch(YLED);
				buf = -1;	
			}
			
		for(j = 0; j < 30000; j++ )
			{
				_asm("nop");
			}
		//	periodic_wait();
		}
}

void dummy()
{  
	while(1);
}   