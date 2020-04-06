#include <asf.h>

#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"



// LED1 OLED
#define LED1_OLED_PIO      PIOA
#define LED1_OLED_PIO_ID   ID_PIOA
#define LED1_OLED_IDX      0
#define LED1_OLED_IDX_MASK (1 << LED1_OLED_IDX)

// LED2 OLED
#define LED2_OLED_PIO      PIOC
#define LED2_OLED_PIO_ID   ID_PIOC
#define LED2_OLED_IDX      30
#define LED2_OLED_IDX_MASK (1 << LED2_OLED_IDX)

// LED3 OLED
#define LED3_OLED_PIO      PIOB
#define LED3_OLED_PIO_ID   ID_PIOB
#define LED3_OLED_IDX      2
#define LED3_OLED_IDX_MASK (1 << LED3_OLED_IDX)


// BUT OLED 1
#define BUT1_PIO           PIOD
#define BUT1_PIO_ID        ID_PIOD
#define BUT1_PIO_IDX       28
#define BUT1_PIO_IDX_MASK  (1u << BUT1_PIO_IDX)

// BUT OLED 2
#define BUT2_PIO           PIOC
#define BUT2_PIO_ID        ID_PIOC
#define BUT2_PIO_IDX       31
#define BUT2_PIO_IDX_MASK  (1u << BUT2_PIO_IDX)

// BUT OLED 3
#define BUT3_PIO		   PIOA
#define BUT3_PIO_ID		   ID_PIOA
#define BUT3_PIO_IDX	   19
#define BUT3_PIO_IDX_MASK  (1u << BUT3_PIO_IDX)

volatile char b1 = 0;
volatile char b2 = 0;
volatile char b3 = 0;
volatile char b4 = 0;
volatile char flag_tc1 = 0;
volatile char flag_tc2 = 0;
volatile char flag_tc3 = 0;


/**
*  Handle Interrupcao botao
*/


void Button1_Handler(void)
{
	b1 = 1;
}

void Button2_Handler(void)
{
	b2 = 1;
}

void Button3_Handler(void)
{
	b3 = 1;
}

void Button4_Handler(void)
{
	b4 = 1;
}



void TC1_Handler(void){
	volatile uint32_t ul_dummy;

	/****************************************************************
	* Devemos indicar ao TC que a interrupção foi satisfeita.
	******************************************************************/
	ul_dummy = tc_get_status(TC0, 1);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	/** Muda o estado do LED */
	flag_tc1 = 1;
}

void TC2_Handler(void){
	volatile uint32_t ul_dummy;

	/****************************************************************
	* Devemos indicar ao TC que a interrupção foi satisfeita.
	******************************************************************/
	ul_dummy = tc_get_status(TC0, 1);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	/** Muda o estado do LED */
	flag_tc2 = 1;
}

void TC3_Handler(void){
	volatile uint32_t ul_dummy;

	/****************************************************************
	* Devemos indicar ao TC que a interrupção foi satisfeita.
	******************************************************************/
	ul_dummy = tc_get_status(TC0, 1);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	/** Muda o estado do LED */
	flag_tc3 = 1;
}


/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void LED_init();

void pisca_led1(int n, int t);
void pisca_led2(int n, int t);
void pisca_led3(int n, int t);

void TC_init1(Tc * TC, int ID_TC, int TC_CHANNEL, int freq);



/************************************************************************/
/* Funcoes                                                              */
/************************************************************************/

void LED_init(){
	pmc_enable_periph_clk(LED1_OLED_PIO_ID);
	pio_set_output(LED1_OLED_PIO , LED1_OLED_IDX_MASK, 1, 0, 0);

	pmc_enable_periph_clk(LED2_OLED_PIO_ID);
	pio_set_output(LED2_OLED_PIO , LED2_OLED_IDX_MASK, 0, 0, 0);
	
	pmc_enable_periph_clk(LED3_OLED_PIO_ID);
	pio_set_output(LED3_OLED_PIO , LED3_OLED_IDX_MASK, 1, 0, 0);
}

void pisca_led1(int n, int t){
	for (int i=0;i<n;i++){
		pio_clear(LED1_OLED_PIO, LED1_OLED_IDX_MASK);
		delay_ms(t);
		pio_set(LED1_OLED_PIO, LED1_OLED_IDX_MASK);
		delay_ms(t);
	}
}

void pisca_led2(int n, int t){
	for (int i=0;i<n;i++){
		pio_clear(LED2_OLED_PIO, LED2_OLED_IDX_MASK);
		delay_ms(t);
		pio_set(LED2_OLED_PIO, LED2_OLED_IDX_MASK);
		delay_ms(t);
	}
}

void pisca_led3(int n, int t){
	for (int i=0;i<n;i++){
		pio_clear(LED3_OLED_PIO, LED3_OLED_IDX_MASK);
		delay_ms(t);
		pio_set(LED3_OLED_PIO, LED3_OLED_IDX_MASK);
		delay_ms(t);
	}
}


//**
//* Configura TimerCounter (TC) para gerar uma interrupcao no canal (ID_TC e TC_CHANNEL)
//* na taxa de especificada em freq.
//*/
void TC_init1(Tc * TC, int ID_TC, int TC_CHANNEL, int freq){
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	/* Configura o PMC */
	/* O TimerCounter é meio confuso
	o uC possui 3 TCs, cada TC possui 3 canais
	TC0 : ID_TC0, ID_TC1, ID_TC2
	TC1 : ID_TC3, ID_TC4, ID_TC5
	TC2 : ID_TC6, ID_TC7, ID_TC8
	*/
	pmc_enable_periph_clk(ID_TC);

	/** Configura o TC para operar em  4Mhz e interrupçcão no RC compare */
	tc_find_mck_divisor(freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC, TC_CHANNEL, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC, TC_CHANNEL, (ul_sysclk / ul_div) / freq);

	/* Configura e ativa interrupçcão no TC canal 0 */
	/* Interrupção no C */
	NVIC_EnableIRQ((IRQn_Type) ID_TC);
	tc_enable_interrupt(TC, TC_CHANNEL, TC_IER_CPCS);

	/* Inicializa o canal 0 do TC */
	tc_start(TC, TC_CHANNEL);
}







int main (void)
{
	board_init();
	sysclk_init();
	delay_init();

  // Init OLED
	gfx_mono_ssd1306_init();
	LED_init();
	
	TC_init1(TC0, ID_TC1, 1, 5);
	TC_init1(TC1, ID_TC3, 1, 10);
	TC_init1(TC2, ID_TC6, 1, 1);
	
	
	//Interrupts
	pio_handler_set(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, PIO_IT_RISE_EDGE, Button1_Handler);
	pio_handler_set(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_IT_FALL_EDGE, Button2_Handler);
	pio_handler_set(BUT3_PIO, BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_IT_RISE_EDGE, Button3_Handler);
	
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	
	//NVIC
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_EnableIRQ(BUT3_PIO_ID);
	
	b1 = 0;
	b2 = 0;
	b3 = 0;
	b4 = 0;
  
  // Escreve na tela um circulo e um texto
	gfx_mono_draw_filled_circle(20, 16, 16, GFX_PIXEL_SET, GFX_WHOLE);
	gfx_mono_draw_string("mundo", 50,16, &sysfont);

  /* Insert application code here, after the board has been initialized. */
	while(1) {
		if(flag_tc1 || flag_tc2 || flag_tc3){
			if(flag_tc1){
				pisca_led1(1,10);
				flag_tc1 = 0;
			}
			if(flag_tc2){
				pisca_led2(1,10);
				flag_tc2 = 0;
			}
			if(flag_tc3){
				pisca_led3(1,10);
				flag_tc3 = 0;
			}
		}

	}
}
