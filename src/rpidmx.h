#ifndef _RPIDMX_H
#define _RPIDMX_H

/*
(C) 2014 Giles Hall <giles@polymerase.org>

based on code by
    Jonathan Andrews (jon HAT jonshouse DOOOT co DOOOT uk)
*/

#include <iostream>
#include <stdexcept>
#include <vector>
#include <pigpio.h>

// Timings
#define BREAK_US		    120
#define MAB_US			    12
#define	POSTPACKET_IDLE_US	50
#define BIT_US			    4

// Pulse types
#define PULSE_LOW           0
#define PULSE_HIGH          1

// DMX
#define DMX_CHANNEL_COUNT   512
#define DMX_START_CODE      0

// 1 start pulse + 8 bits + 1 stop pulse
#define DMX_BYTE_LEN        10 
// 2 pulses (BREAK, MAB) + DMX_BYTE_LEN
#define DMX_PULSE_OFFSET    3 + DMX_BYTE_LEN 

typedef std::vector<uint8_t> pin_vec_t;
typedef std::vector<uint8_t> channel_vec_t;
typedef std::vector<channel_vec_t> universe_vec_t;
typedef std::vector<gpioPulse_t> pulse_vec_t;

class RPIDMX 
{
public:
    RPIDMX(pin_vec_t &pin_map);
    void update(uint32_t universe_index, channel_vec_t &channels);
    void refresh();

private:
    void _initialize_pulses();
    void _add_pulse(uint32_t pinmask, bool highlow, uint32_t duration_us=BIT_US);
    void _add_byte_pulses(uint32_t pinmask, uint8_t _byte);
    void _update_pulse(uint32_t index, uint32_t pinmask, bool highlow);
    void _update_channel(uint32_t channel, uint32_t pinmask, uint8_t value);

private:
    int _current_wave_id;
    pulse_vec_t _pulses;
    universe_vec_t _universe;
    pin_vec_t _pin_map;
};

#endif // _RPIDMX_H
