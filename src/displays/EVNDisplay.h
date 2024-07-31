#ifndef EVNDisplay_h
#define EVNDisplay_h

#include <Arduino.h>
#include <Wire.h>
#include "../EVNAlpha.h"
#include "U8g2/src/U8x8lib.h"

class EVNDisplay
{
private:
  static const uint8_t OLED_ADDR = 0x3C;
  static const uint16_t SCREEN_HEIGHT = 64;
  static const uint16_t SCREEN_WIDTH = 128;
  static const uint16_t NO_OF_ROWS = (SCREEN_HEIGHT / 8);
  static const uint16_t MAX_CHAR = (SCREEN_WIDTH / 8);

public:
  EVNDisplay(uint8_t port, bool flip_180deg = false);
  bool begin();
  void splashEVN();
  void rotate();
  void clear();
  void clearLine(uint8_t row);

  template <typename T>
  void writeData(uint8_t row, T data)
  {
    if (_sensor_started)
    {
      EVNAlpha::sharedPorts().setPort(_port);
      uint8_t rowc = constrain(row, 0, NO_OF_ROWS - 1);

      //convert to String, so that it can be used with u8x8.print()
      String datas = String(data);

      //trim the string if it's too long
      if (datas.length() > (MAX_CHAR - _rownamelen[rowc]))
      {
        datas = datas.substring(0, (MAX_CHAR - _rownamelen[rowc]));
      }

      //pad the pixels after data with ' '
      for (int i = _rownamelen[rowc] + datas.length(); i < MAX_CHAR; i++)
      {
        _display8x8->drawGlyph(i, rowc, ' ');
      }

      //start writing only from after the row name (saves time)
      _display8x8->setCursor(_rownamelen[rowc], rowc);
      _display8x8->print(datas);
    }
  };

  template <typename T>
  void writeLabel(uint8_t row, T label)
  {
    if (_sensor_started)
    {
      EVNAlpha::sharedPorts().setPort(_port);
      uint8_t rowc = constrain(row, 0, NO_OF_ROWS - 1);

      //convert to String, so that it can be used with u8x8.print()
      String labels = String(label);

      //trim the string if it's too long
      if (labels.length() > MAX_CHAR)
      {
        labels = labels.substring(0, MAX_CHAR);
      }

      // if new name is shorter than old name, wipe out pixels from old name
      uint8_t namelen = labels.length();
      if (namelen < _rownamelen[rowc])
      {
        for (int i = namelen; i < _rownamelen[rowc]; i++)
        {
          _display8x8->drawGlyph(i, rowc, ' ');
        }
      }

      //set new row name length
      _rownamelen[rowc] = namelen;

      //print new row name to display
      _display8x8->setCursor(0, rowc);
      _display8x8->print(labels);
    }
  };

  //alias for writeLabel
  template <typename T>
  void print(uint8_t row, T line)
  {
    this->writeLabel(row, line);
  };

private:
  bool _sensor_started;
  uint8_t _port;
  bool _flip_180deg;
  U8X8* _display8x8;
  uint8_t _rownamelen[NO_OF_ROWS] = { 0 };
};

//must be the same size as SCREENWIDTH * SCREENHEIGHT
//optimized for column read (one byte is 8 pixels top to bottom)
//generated with https://notisrac.github.io/FileToCArray/
// and https://javl.github.io/image2cpp/ for colour inversion
static const uint8_t PROGMEM logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0x1c, 0x3c, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x1c,
0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x3c, 0x7c, 0xfc,
0xfc, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x0c, 0x1c, 0x3c, 0xfc, 0xfc, 0xfc, 0xfc,
0xfc, 0xfc, 0xfc, 0x1c, 0x0c, 0x0c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x1c, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0x7c, 0x3c, 0x1c, 0x0c, 0x04,
0x00, 0x00, 0x00, 0x04, 0x0c, 0x0c, 0x1c, 0xfc, 0xfc, 0xfc, 0xfc, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c,
0x1c, 0x7c, 0xfc, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x04, 0x0c, 0x0c, 0x3c, 0xfc, 0xfc, 0xfc, 0xfc, 0x3c, 0x1c, 0x0c, 0x0c, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
0x0f, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x7f, 0xff,
0xff, 0xff, 0xff, 0xf8, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xc0, 0xf8, 0xff, 0xff, 0x7f, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0xf8, 0xe0, 0x80,
0x00, 0x00, 0x01, 0x07, 0x0f, 0x3f, 0xfe, 0xfc, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0,
0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe0, 0xf0, 0xfe, 0xfe, 0xfe, 0xfe, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
0x1f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xc0, 0xf8, 0xff, 0xff, 0x7f, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0x0f, 0x3f,
0x7f, 0xfc, 0xf0, 0xe0, 0x80, 0x00, 0x00, 0x03, 0x07, 0x1f, 0x7f, 0xfe, 0xf8, 0xe0, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x0f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0,
0xfe, 0xff, 0x7f, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x07, 0x1f, 0x3f, 0xfe, 0xf8, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3f, 0x7e,
0xfc, 0xf0, 0xc0, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
0xf8, 0xf8, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xc0, 0xf0, 0xfc, 0xff, 0x7f,
0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x1f, 0x7f, 0xfc, 0xf8, 0xe0, 0x80, 0x00, 0x00,
0x01, 0x07, 0x1f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x30, 0x30, 0x3c, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x38,
0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x3c, 0x3e, 0x3f,
0x3f, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x0f, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x20, 0x30, 0x30, 0x38, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x38, 0x30, 0x30, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0f, 0x3f, 0x3f, 0x3c,
0x38, 0x38, 0x38, 0x38, 0x38, 0x3f, 0x3f, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#endif