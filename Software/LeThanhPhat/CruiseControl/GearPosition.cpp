#include "GearPosition.h"
/* State machine for gear shifts with interlocks
 * - current_gear_: validated gear reported to LabVIEW
 * - requested_gear_: driver request from buttons (debounced)
 * - transition validation is done in is_shift_allowed()
 */

#include "Encoder.h"
#include "PedalRead.h"

namespace
{
uint8_t pin_p_ = 0;
uint8_t pin_r_ = 0;
uint8_t pin_n_ = 0;
uint8_t pin_d_ = 0;

int8_t current_gear_ = GEAR_P;
int8_t requested_gear_ = GEAR_P;

const uint8_t DEBOUNCE_READS = 5;

uint8_t samples_p_ = 0;
uint8_t samples_r_ = 0;
uint8_t samples_n_ = 0;
uint8_t samples_d_ = 0;

/* track stable active state to detect press events (buttons active-low) */
bool active_p_ = false;
bool active_r_ = false;
bool active_n_ = false;
bool active_d_ = false;

/* Thresholds and helpers */
const uint16_t BRAKE_FULL_THRESHOLD = 4000; /* ADC 12-bit full press threshold */

static bool is_brake_full(void)
{
    return pedal_read_get_brake_value() >= BRAKE_FULL_THRESHOLD;
}

static float current_rpm(void)
{
    return Encoder_getRPM();
}

/* Centralized shift validation function
 * - from: current gear
 * - to: requested gear
 * - rpm: current rpm
 * - brake_full: boolean brake state
 * Returns true only if transition allowed at this moment
 */
static bool is_shift_allowed(int8_t from, int8_t to, float rpm, bool brake_full)
{
    if (from == to)
        return false; /* no change */

    /* When RPM != 0, disallow any shift originating from D or R */
    if (rpm != 0.0f && (from == GEAR_D || from == GEAR_R))
        return false;

    /* P->D, P->R, N->D, N->R require brake full and rpm == 0 */
    if ((from == GEAR_P && (to == GEAR_D || to == GEAR_R)) || (from == GEAR_N && (to == GEAR_D || to == GEAR_R)))
    {
        return (rpm == 0.0f) && brake_full;
    }

    /* other transitions allowed */
    return true;
}

/* Apply shift: update current gear */
static void apply_shift(int8_t to)
{
    current_gear_ = to;
}

/* Handle a single press event: validate immediately and apply or ignore */
static void handle_request_event(int8_t req)
{
    if (req == current_gear_)
        return; /* no change requested */

    float rpm = current_rpm();
    bool brake_full = is_brake_full();

    if (is_shift_allowed(current_gear_, req, rpm, brake_full))
    {
        apply_shift(req);
    }
    else
    {
        /* reject: do nothing, do not remember request */
    }
}

} /* end anonymous namespace */

void gear_begin(uint8_t pin_p, uint8_t pin_r, uint8_t pin_n, uint8_t pin_d)
{
    pin_p_ = pin_p;
    pin_r_ = pin_r;
    pin_n_ = pin_n;
    pin_d_ = pin_d;

    pinMode(pin_p_, INPUT_PULLUP);
    pinMode(pin_r_, INPUT_PULLUP);
    pinMode(pin_n_, INPUT_PULLUP);
    pinMode(pin_d_, INPUT_PULLUP);

    current_gear_ = GEAR_P;
    requested_gear_ = GEAR_P;
}

/* Debounce buttons, set request, validate and apply transition */
void gear_update(void)
{
    /* debounce reads: active low buttons */
    if (digitalRead(pin_p_) == LOW) samples_p_++; else if (samples_p_) samples_p_--;
    if (digitalRead(pin_r_) == LOW) samples_r_++; else if (samples_r_) samples_r_--;
    if (digitalRead(pin_n_) == LOW) samples_n_++; else if (samples_n_) samples_n_--;
    if (digitalRead(pin_d_) == LOW) samples_d_++; else if (samples_d_) samples_d_--;

    /* handle press events when debounce crosses threshold (active-low) */
    if (samples_p_ >= DEBOUNCE_READS)
    {
        samples_p_ = DEBOUNCE_READS;
        samples_r_ = samples_n_ = samples_d_ = 0;
        if (!active_p_)
        {
            active_p_ = true; /* press event */
            handle_request_event(GEAR_P);
        }
    }
    else
    {
        if (samples_p_ > 0)
            ;
        else if (active_p_)
            active_p_ = false; /* release */
    }

    if (samples_r_ >= DEBOUNCE_READS)
    {
        samples_r_ = DEBOUNCE_READS;
        samples_p_ = samples_n_ = samples_d_ = 0;
        if (!active_r_)
        {
            active_r_ = true;
            handle_request_event(GEAR_R);
        }
    }
    else
    {
        if (samples_r_ > 0)
            ;
        else if (active_r_)
            active_r_ = false;
    }

    if (samples_n_ >= DEBOUNCE_READS)
    {
        samples_n_ = DEBOUNCE_READS;
        samples_p_ = samples_r_ = samples_d_ = 0;
        if (!active_n_)
        {
            active_n_ = true;
            handle_request_event(GEAR_N);
        }
    }
    else
    {
        if (samples_n_ > 0)
            ;
        else if (active_n_)
            active_n_ = false;
    }

    if (samples_d_ >= DEBOUNCE_READS)
    {
        samples_d_ = DEBOUNCE_READS;
        samples_p_ = samples_r_ = samples_n_ = 0;
        if (!active_d_)
        {
            active_d_ = true;
            handle_request_event(GEAR_D);
        }
    }
    else
    {
        if (samples_d_ > 0)
            ;
        else if (active_d_)
            active_d_ = false;
    }
}

int8_t gear_get_position(void)
{
    return current_gear_;
}

