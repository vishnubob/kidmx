#include "rpidmx.h"

void RPIDMX::_add_pulse(uint32_t pinmask, bool highlow, uint32_t duration_us)
{
    gpioPulse_t pulse;
    if (highlow)
    {
        pulse.gpioOn = pinmask;
        pulse.gpioOff = 0;
    } else
    {
        pulse.gpioOn = 0;
        pulse.gpioOff = pinmask;
    }
    pulse.usDelay = duration_us;
    _pulses.push_back(pulse);
}

void RPIDMX::_add_byte_pulses(uint32_t pinmask, uint8_t _byte)
{
    // start bit
   _add_pulse(pinmask, PULSE_LOW);
   for (int bit = 0; bit < 8; bit += 1)
   {
        bool highlow = (bool)(_byte & (1 << bit));
        _add_pulse(pinmask, highlow);
   }
    // two stop bits
    _add_pulse(pinmask, PULSE_HIGH, BIT_US * 2);
}

void RPIDMX::_initialize_pulses()
{
    uint32_t pinmask = 0;
    uint32_t packet_cnt = DMX_CHANNEL_COUNT;

    for (pin_vec_t::iterator itr = _pin_map.begin(); itr != _pin_map.end(); itr += 1)
    {
        pinmask |= (1 << *itr);
    }

   _add_pulse(pinmask, PULSE_LOW, BREAK_US);
   _add_pulse(pinmask, PULSE_HIGH, MAB_US);

    // DMX start code
    _add_byte_pulses(pinmask, DMX_START_CODE);
    while(packet_cnt--)
    {
       _add_byte_pulses(pinmask, 0);
    }

   _add_pulse(pinmask, PULSE_HIGH, POSTPACKET_IDLE_US);
}

void RPIDMX::_update_pulse(uint32_t index, uint32_t pinmask, bool highlow)
{
    //std::cout << "update index " << index << " pinmask " << (uint32_t) pinmask << " highlow " << highlow << std::endl;
    if (highlow)
    {
        _pulses[index].gpioOn |= pinmask;
        _pulses[index].gpioOff &= ~pinmask;
    } else
    {
        _pulses[index].gpioOn &= ~pinmask;
        _pulses[index].gpioOff |= pinmask;
    }
}

void RPIDMX::_update_channel(uint32_t channel, uint32_t pinmask, uint8_t value)
{
    uint32_t channel_index = (channel * DMX_BYTE_LEN) + DMX_PULSE_OFFSET;

    for (int bit = 0; bit < 8; bit += 1)
    {
        bool highlow = (value & (1 << bit));
        _update_pulse(channel_index + bit, pinmask, highlow);
    }
}

void RPIDMX::update(uint32_t universe_index, channel_vec_t &channels)
{
    uint32_t pinmask = (1 << _pin_map[universe_index]);

    for(uint32_t channel = 0; channel < channels.size(); channel += 1)
    {
        if (_universe[universe_index][channel] != channels[channel])
        {
            //std::cout << "update " << channel << " with " << (uint32_t) channels[channel] << " pinmask " << pinmask << std::endl;
            _universe[universe_index][channel] = channels[channel];
            _update_channel(channel, pinmask, channels[channel]);
        }
    }
}

void RPIDMX::refresh()
{
    if (gpioWaveTxBusy())
    {
        // DMA is working, need to wait until it's idle
        return;
    }

    if (_current_wave_id != -1)
    {
        gpioWaveDelete(_current_wave_id);
    }

    //std::cout << "refresh" << std::endl;
    //gpioWaveAddNew();
    gpioWaveAddGeneric(_pulses.size(), _pulses.data());
    _current_wave_id = gpioWaveCreate();
    gpioWaveTxSend(_current_wave_id, PI_WAVE_MODE_ONE_SHOT);
}

RPIDMX::RPIDMX(pin_vec_t &pin_map) 
    : _pin_map(pin_map), _current_wave_id(-1)
{
    channel_vec_t default_channels(DMX_CHANNEL_COUNT, 0);

    if (gpioInitialise() < 0)
    {
        throw std::runtime_error("gpioInitialise() failed.");
    }

    // configure each DMX pin as outputs
    for (pin_vec_t::iterator itr = _pin_map.begin(); itr != _pin_map.end(); itr += 1)
    {
        if (gpioSetMode(*itr, PI_OUTPUT) != 0)
        {
            throw std::runtime_error("gpioSetMode() failed.");
        }
        _universe.push_back(default_channels);
    }

    _initialize_pulses();
}
