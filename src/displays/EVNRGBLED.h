#ifndef EVNRGBLED_H
#define EVNRGBLED_H

#include <Arduino.h>
#include <hardware/clocks.h>
#include <hardware/pio.h>
#include "PIOProgram.h"

#include "../ws2812/ws2812.pio.h"
#include "../evn_alpha_pins.h"

static PIOProgram _rgbLedPgm(&ws2812_program);

class EVNRGBLED
{
public:

    const static uint16_t MAX_LEDS = 48;

    EVNRGBLED(uint8_t port, uint8_t num_leds = 8)
    {
        _num_leds = constrain(num_leds, 0, MAX_LEDS);

        uint8_t portc = constrain(port, 1, 4);

        switch (portc)
        {
        case 1:
            _pin = SERVO_PORT_1;
            break;
        case 2:
            _pin = SERVO_PORT_2;
            break;
        case 3:
            _pin = SERVO_PORT_3;
            break;
        case 4:
            _pin = SERVO_PORT_4;
            break;
        }

        _pio = nullptr;
        _sm = -1;
        _offset = -1;
        _attached = false;
    };

    bool begin()
    {
        if (!_rgbLedPgm.prepare(&_pio, &_sm, &_offset))
            return _attached;

        pio_add_program(_pio, &ws2812_program);
        ws2812_program_init(_pio, _sm, _offset, _pin, 800000, false);
        this->clear();

        _attached = true;
        return _attached;
    };

    void writeAll(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, bool show = true, bool blocking = true)
    {
        for (int i = 0; i < _num_leds; i++)
        {
            if (_buffer[i][0] != r || _buffer[i][1] != g || _buffer[i][2] != b)
            {
                _buffer[i][0] = r;
                _buffer[i][1] = g;
                _buffer[i][2] = b;
                _buffer_changed = true;
            }
        }

        if (show) this->update(blocking);
    };

    void write(uint16_t led, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, bool show = true, bool blocking = true)
    {
        if (_buffer[led][0] != r || _buffer[led][1] != g || _buffer[led][2] != b)
        {
            _buffer[led][0] = r;
            _buffer[led][1] = g;
            _buffer[led][2] = b;
            _buffer_changed = true;
        }

        if (show) this->update(blocking);
    };

    void clear(bool show = true, bool blocking = true)
    {
        for (int i = 0; i < _num_leds; i++)
        {
            if (_buffer[i][0] != 0 || _buffer[i][1] != 0 || _buffer[i][2] != 0)
            {
                _buffer[i][0] = 0;
                _buffer[i][1] = 0;
                _buffer[i][2] = 0;
                _buffer_changed = true;
            }
        }

        if (show) this->update(blocking);
    };

    void update(bool blocking = false)
    {
        if (_buffer_changed)
        {
            if (blocking)
                ensure_new_frame();

            for (int i = 0; i < _num_leds; i++)
                put_pixel(urgb_u32(_buffer[i][0], _buffer[i][1], _buffer[i][2]));

            _buffer_changed = false;
        }
    };

private:
    static uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
        return
            ((uint32_t)(r) << 8) |
            ((uint32_t)(g) << 16) |
            (uint32_t)(b);
    };

    void put_pixel(uint32_t pixel_grb) {
        pio_sm_put_blocking(_pio, _sm, pixel_grb << 8u);
    };

    void ensure_new_frame()
    {
        // ensure that 50us have elapsed since sending last int32 from the previous frame
        // TODO: Find ways to make this non-blocking

        while (!pio_sm_is_tx_fifo_empty(_pio, _sm));
        delayMicroseconds(52);
    }

    PIO _pio;
    int _sm;
    int _offset;
    bool _attached;

    bool _buffer_changed = true;
    uint64_t _last_transmission_us;

    uint16_t _num_leds;
    uint8_t _pin;
    uint8_t _buffer[MAX_LEDS][3] = { 0 };
};

#endif