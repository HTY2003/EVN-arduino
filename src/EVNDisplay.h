#ifndef EVNDisplay_h
#define EVNDisplay_h

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#define OLED_ADDR 0x3C
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128
#define LOGO_HEIGHT 54
#define LOGO_WIDTH 95
#define NO_OF_ROWS (SCREEN_HEIGHT / 8)

class EVNDisplay
{
public:
  EVNDisplay(uint8_t bus);
  void init(bool rotate = false);
  void splashEVN();
  void rotate();
  void clear();
  void setRowName(uint8_t row, char const* name);


  template <typename T> //template so func can receive any data type as input
  void setRowInfo(uint8_t row, T info)
  {
    uint8_t rowc = constrain(row, 0, NO_OF_ROWS - 1);
    //clear previous row data in buffer
    _display->setDrawColor(0);
    _display->drawBox(_rownamelen[rowc] * 8, rowc * 8, (SCREEN_WIDTH - (_rownamelen[rowc] * 8)), 8);
    // write row data(placed after row name) to buffer
    _display->setDrawColor(1);
    _display->setCursor(_rownamelen[rowc] * 8, (rowc + 1) * 8);
    _display->print(info);
  }


  void updateRowName(uint8_t row);
  void updateRowInfo(uint8_t row);

private:
  uint8_t _bus;
  bool _rotate = false;
  U8G2* _display;
  uint8_t _rownamelen[NO_OF_ROWS] = { 0 }, _rownamelen_prev[NO_OF_ROWS] = { 0 };
};

static const unsigned char logo[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFE, 0xFF, 0xFF, 0xE1, 0xFF, 0x00, 0xFE, 0xC7, 0xFF, 0x07, 0xE0, 0x3F,
  0xFC, 0xFF, 0xFF, 0xC1, 0x3F, 0x00, 0xF8, 0x03, 0xFF, 0x07, 0xC0, 0x1F,
  0xF8, 0x01, 0xF8, 0x81, 0x1F, 0x00, 0xF0, 0x01, 0x0E, 0x0E, 0x80, 0x0F,
  0xF0, 0x01, 0xF0, 0x81, 0x1F, 0x00, 0xF0, 0x00, 0x0E, 0x0E, 0x00, 0x07,
  0xF0, 0x01, 0xE0, 0x01, 0x1F, 0x00, 0xF0, 0x00, 0x1E, 0x1C, 0x00, 0x07,
  0xF0, 0x01, 0xC0, 0x01, 0x1F, 0x00, 0xF0, 0x00, 0x1E, 0x1C, 0x00, 0x07,
  0xF0, 0x01, 0xC0, 0x01, 0x1F, 0x00, 0x70, 0x00, 0x3E, 0x38, 0x00, 0x07,
  0xF0, 0x01, 0xC0, 0x01, 0x3E, 0x00, 0x78, 0x00, 0x3E, 0x78, 0x00, 0x07,
  0xF0, 0x01, 0x00, 0x00, 0x3E, 0x00, 0x78, 0x00, 0x7E, 0x70, 0x00, 0x07,
  0xF0, 0x01, 0x00, 0x00, 0x3E, 0x00, 0x38, 0x00, 0xFE, 0xE0, 0x00, 0x07,
  0xF0, 0x01, 0x00, 0x00, 0x7C, 0x00, 0x3C, 0x00, 0xFE, 0xE0, 0x00, 0x07,
  0xF0, 0x01, 0x00, 0x00, 0x7C, 0x00, 0x1C, 0x00, 0xDE, 0xC1, 0x01, 0x07,
  0xF0, 0x01, 0x0C, 0x00, 0x7C, 0x00, 0x1C, 0x00, 0xDE, 0xC1, 0x03, 0x07,
  0xF0, 0x01, 0x0E, 0x00, 0x78, 0x00, 0x1E, 0x00, 0x9E, 0x83, 0x03, 0x07,
  0xF0, 0x01, 0x0E, 0x00, 0xF8, 0x00, 0x0E, 0x00, 0x9E, 0x83, 0x07, 0x07,
  0xF0, 0x81, 0x0F, 0x00, 0xF8, 0x00, 0x0E, 0x00, 0x1E, 0x07, 0x07, 0x07,
  0xF0, 0xFF, 0x0F, 0x00, 0xF8, 0x00, 0x0F, 0x00, 0x1E, 0x0F, 0x0E, 0x07,
  0xF0, 0xFF, 0x0F, 0x00, 0xF0, 0x00, 0x07, 0x00, 0x1E, 0x0E, 0x0E, 0x07,
  0xF0, 0x01, 0x0F, 0x00, 0xF0, 0x01, 0x07, 0x00, 0x1E, 0x1C, 0x1C, 0x07,
  0xF0, 0x01, 0x0E, 0x00, 0xF0, 0x81, 0x03, 0x00, 0x1E, 0x1C, 0x1C, 0x07,
  0xF0, 0x01, 0x0C, 0x00, 0xE0, 0x81, 0x03, 0x00, 0x1E, 0x38, 0x38, 0x07,
  0xF0, 0x01, 0x0C, 0x00, 0xE0, 0xC3, 0x03, 0x00, 0x1E, 0x78, 0x38, 0x07,
  0xF0, 0x01, 0x00, 0x00, 0xE0, 0xC3, 0x01, 0x00, 0x1E, 0x70, 0x70, 0x07,
  0xF0, 0x01, 0x00, 0x00, 0xC0, 0xC3, 0x01, 0x00, 0x1E, 0xF0, 0x60, 0x07,
  0xF0, 0x01, 0x00, 0x00, 0xC0, 0xE3, 0x01, 0x00, 0x1E, 0xE0, 0xE0, 0x07,
  0xF0, 0x01, 0x00, 0x00, 0xC0, 0xE7, 0x00, 0x00, 0x1E, 0xC0, 0xC1, 0x07,
  0xF0, 0x01, 0xC0, 0x01, 0x80, 0xE7, 0x00, 0x00, 0x1E, 0xC0, 0xC1, 0x07,
  0xF0, 0x01, 0xC0, 0x01, 0x80, 0x77, 0x00, 0x00, 0x1E, 0x80, 0x83, 0x07,
  0xF0, 0x01, 0xC0, 0x01, 0x80, 0x7F, 0x00, 0x00, 0x1E, 0x80, 0x87, 0x07,
  0xF0, 0x01, 0xE0, 0x01, 0x80, 0x7F, 0x00, 0x00, 0x1E, 0x00, 0x07, 0x07,
  0xF0, 0x01, 0xE0, 0x01, 0x00, 0x3F, 0x00, 0x00, 0x1E, 0x00, 0x0F, 0x07,
  0xF0, 0x01, 0xF0, 0x01, 0x00, 0x3F, 0x00, 0x00, 0x1E, 0x00, 0x0E, 0x07,
  0xF8, 0x01, 0xF8, 0x01, 0x00, 0x3F, 0x00, 0x00, 0x1E, 0x00, 0x1C, 0x07,
  0xFC, 0xFF, 0xFF, 0x01, 0x00, 0x1F, 0x00, 0x80, 0x7F, 0x00, 0xFC, 0x07,
  0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x1E, 0x00, 0xC0, 0xFF, 0x00, 0xF8, 0x07,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


#endif