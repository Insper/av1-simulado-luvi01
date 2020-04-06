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

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void LED_init();



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
};


int main (void)
{
	board_init();
	sysclk_init();
	delay_init();

  // Init OLED
	gfx_mono_ssd1306_init();
	LED_init();
	
	
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

	}
}
