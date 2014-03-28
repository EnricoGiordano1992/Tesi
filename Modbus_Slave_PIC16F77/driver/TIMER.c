#include "../Includes.h"

void timer_settings(void)
{
	// Impostazione del registro OPTION
	OPTION_REG=0b11000100;
	// bit 0 -> Prescaler Rate Select bit 0
	// bit 1 -> Prescaler Rate Select bit 0
	// bit 2 -> Prescaler Rate Select bit 0 (1:32)
	// bit 3 -> Prescaler assegnato al Timer0
	// bit 4 -> Non importa
	// bit 5 -> Clock per Timer0 derivato da ciclo di clock interno
	// bit 6 -> Non importa
	// bit 7 -> Resistenze di pull-up su porta B disattivate

	// Impostazione Interrupt
	INTCON=0b00000000;

	// bit 0 -> RBIF - Flag interrupt su porte B
	// bit 1 -> INTF - Flag interrupt su RB0/INT
	// bit 2 -> T0IF - Flag interrupt su Timer0
	// bit 3 -> RBIE, Interrupt su porte B disattivato
	// bit 4 -> INTE, Interrupt su porta RB0/INT disattivato
	// bit 5 -> TMR0IE, Interrupt su Timer0 disattivato
	// bit 6 -> PEIE, Interrupt di periferica disattivato
	// bit 7 -> GIE, Gestione Interrupt disattivata

	TMR0=100; // Imposto Timer0 a 100   (1 tick per 1 ms)

        //NB: non esiste flag per far on/off del timer!
	}