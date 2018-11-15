/* Arduino Includes */

#include <Keyboard.h>
#include <AStar32U4.h>

/* ADL Includes */

#include "adl.h"
#include "adl-debouncer.h"
#include "adl-oneshot-timer.h"
#include "adl-oneshot-task.h"
#include "adl-task.h"
#include "binary-output.h"

/* Enums, Typedefs, Constants */

static const char CHARACTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static const uint8_t COL_PINS[6] = {2,1,0,12,11,10};
static const uint8_t LED_PIN = 13;
static const uint8_t DEBOUNCE_TICKS = 3;

static bool readC1() { return digitalRead(COL_PINS[0]); }
static bool readC2() { return digitalRead(COL_PINS[1]); }
static bool readC3() { return digitalRead(COL_PINS[2]); }
static bool readC4() { return digitalRead(COL_PINS[3]); }
static bool readC5() { return digitalRead(COL_PINS[4]); }
static bool readC6() { return digitalRead(COL_PINS[5]); }

static ADLDebouncer s_debouncers[6][6] = {
	{
		ADLDebouncer(readC1, DEBOUNCE_TICKS),
		ADLDebouncer(readC2, DEBOUNCE_TICKS),
		ADLDebouncer(readC3, DEBOUNCE_TICKS),
		ADLDebouncer(readC4, DEBOUNCE_TICKS),
		ADLDebouncer(readC5, DEBOUNCE_TICKS),
		ADLDebouncer(readC6, DEBOUNCE_TICKS),
	},
	{
		ADLDebouncer(readC1, DEBOUNCE_TICKS),
		ADLDebouncer(readC2, DEBOUNCE_TICKS),
		ADLDebouncer(readC3, DEBOUNCE_TICKS),
		ADLDebouncer(readC4, DEBOUNCE_TICKS),
		ADLDebouncer(readC5, DEBOUNCE_TICKS),
		ADLDebouncer(readC6, DEBOUNCE_TICKS),
	},
	{
		ADLDebouncer(readC1, DEBOUNCE_TICKS),
		ADLDebouncer(readC2, DEBOUNCE_TICKS),
		ADLDebouncer(readC3, DEBOUNCE_TICKS),
		ADLDebouncer(readC4, DEBOUNCE_TICKS),
		ADLDebouncer(readC5, DEBOUNCE_TICKS),
		ADLDebouncer(readC6, DEBOUNCE_TICKS),
	},
	{
		ADLDebouncer(readC1, DEBOUNCE_TICKS),
		ADLDebouncer(readC2, DEBOUNCE_TICKS),
		ADLDebouncer(readC3, DEBOUNCE_TICKS),
		ADLDebouncer(readC4, DEBOUNCE_TICKS),
		ADLDebouncer(readC5, DEBOUNCE_TICKS),
		ADLDebouncer(readC6, DEBOUNCE_TICKS),
	},
	{
		ADLDebouncer(readC1, DEBOUNCE_TICKS),
		ADLDebouncer(readC2, DEBOUNCE_TICKS),
		ADLDebouncer(readC3, DEBOUNCE_TICKS),
		ADLDebouncer(readC4, DEBOUNCE_TICKS),
		ADLDebouncer(readC5, DEBOUNCE_TICKS),
		ADLDebouncer(readC6, DEBOUNCE_TICKS),
	},
	{
		ADLDebouncer(readC1, DEBOUNCE_TICKS),
		ADLDebouncer(readC2, DEBOUNCE_TICKS),
		ADLDebouncer(readC3, DEBOUNCE_TICKS),
		ADLDebouncer(readC4, DEBOUNCE_TICKS),
		ADLDebouncer(readC5, DEBOUNCE_TICKS),
		ADLDebouncer(readC6, DEBOUNCE_TICKS),
	},
};

static uint8_t s_row = 0;
static BinaryOutput* s_pRowSelect = NULL;

/* Private Functions */

static void set_row(uint8_t row)
{
	s_pRowSelect->set(row);
}

static void keypress(uint8_t r, uint8_t c)
{
	char key = CHARACTERS[(r*6)+c];
	Keyboard.press(key);
	Keyboard.release(key);
}

static void s_debounce_task_fn(ADLTask& thisTask, void * pTaskData)
{
	(void)thisTask; (void)pTaskData;
	for (uint8_t c = 0; c<6;c++)
	{
		s_debouncers[s_row][c].tick();
		if (s_debouncers[s_row][c].check_low_and_clear())
		{
			keypress(s_row, c);
		}
	}
	s_row = (s_row < 5) ? s_row+1 : 0;
	set_row(s_row);
}
static ADLTask s_debounce_task(5, s_debounce_task_fn, NULL);

/* Arduino Functions */

void adl_custom_setup(DeviceBase * pdevices[], int ndevices, ParameterBase * pparams[], int nparams)
{
    (void)pdevices; (void)ndevices; (void)pparams; (void)nparams;

    s_pRowSelect = (BinaryOutput*)pdevices[0];

    pinMode(LED_PIN, OUTPUT);

	Serial.begin(115200);

    Keyboard.begin();
}

void adl_custom_loop(DeviceBase * pdevices[], int ndevices, ParameterBase * pparams[], int nparams)
{
    (void)pdevices; (void)ndevices; (void)pparams; (void)nparams;
	s_debounce_task.run();
}
