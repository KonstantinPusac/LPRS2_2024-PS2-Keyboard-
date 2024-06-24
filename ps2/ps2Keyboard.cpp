#include "ps2Keyboard.h"

#define PS2_PIN_CLOCK A5
#define PS2_PIN_DATA A4
#define PS2_BUF_LEN 128

volatile unsigned char _buf[PS2_BUF_LEN];
volatile uint8_t _bufbeg = 0;
volatile uint8_t _bufend = 0;
unsigned char scan_code_to_ascii[128];
unsigned char scan_code_to_ascii_shifted[128];

bool shift = false;
bool extended = false;

void ps2_init(void) 
{
  pinMode(PS2_PIN_CLOCK, INPUT_PULLUP);
  pinMode(PS2_PIN_DATA, INPUT_PULLUP);
  init_scan_code_to_ascii();
  Serial.println("PS2 initialized");
}

void init_scan_code_to_ascii() 
{
  for (int i = 0; i < 128; i++) 
  {
    scan_code_to_ascii[i] = 0;
    scan_code_to_ascii_shifted[i] = 0;
  }
  // Mapiranja za slova i znakova
  scan_code_to_ascii[0x1C] = 'a'; scan_code_to_ascii[0x32] = 'b';
  scan_code_to_ascii[0x21] = 'c'; scan_code_to_ascii[0x23] = 'd';
  scan_code_to_ascii[0x24] = 'e'; scan_code_to_ascii[0x2B] = 'f';
  scan_code_to_ascii[0x34] = 'g'; scan_code_to_ascii[0x33] = 'h';
  scan_code_to_ascii[0x43] = 'i'; scan_code_to_ascii[0x3B] = 'j';
  scan_code_to_ascii[0x42] = 'k'; scan_code_to_ascii[0x4B] = 'l';
  scan_code_to_ascii[0x3A] = 'm'; scan_code_to_ascii[0x31] = 'n';
  scan_code_to_ascii[0x44] = 'o'; scan_code_to_ascii[0x4D] = 'p';
  scan_code_to_ascii[0x10] = 'q'; scan_code_to_ascii[0x2D] = 'r';
  scan_code_to_ascii[0x1B] = 's'; scan_code_to_ascii[0x2C] = 't';
  scan_code_to_ascii[0x3C] = 'u'; scan_code_to_ascii[0x2A] = 'v';
  scan_code_to_ascii[0x1D] = 'w'; scan_code_to_ascii[0x22] = 'x';
  scan_code_to_ascii[0x35] = 'y'; scan_code_to_ascii[0x1A] = 'z';
  scan_code_to_ascii[0x45] = '0'; scan_code_to_ascii[0x16] = '1';
  scan_code_to_ascii[0x1E] = '2'; scan_code_to_ascii[0x26] = '3';
  scan_code_to_ascii[0x25] = '4'; scan_code_to_ascii[0x2E] = '5';
  scan_code_to_ascii[0x36] = '6'; scan_code_to_ascii[0x3D] = '7';
  scan_code_to_ascii[0x3E] = '8'; scan_code_to_ascii[0x46] = '9';
  scan_code_to_ascii[0x0E] = '`'; scan_code_to_ascii[0x4E] = '-';
  scan_code_to_ascii[0x55] = '='; scan_code_to_ascii[0x54] = '[';
  scan_code_to_ascii[0x5B] = ']'; scan_code_to_ascii[0x4C] = ';';
  scan_code_to_ascii[0x52] = '\''; scan_code_to_ascii[0x41] = ',';
  scan_code_to_ascii[0x49] = '.'; scan_code_to_ascii[0x4A] = '/';
  scan_code_to_ascii[0x5D] = '\\'; scan_code_to_ascii[0x29] = ' ';
  scan_code_to_ascii[0x5A] = '\n';

  /* Mapiranja za Shift
  scan_code_to_ascii_shifted[0x1C] = 'A'; scan_code_to_ascii_shifted[0x32] = 'B';
  scan_code_to_ascii_shifted[0x21] = 'C'; scan_code_to_ascii_shifted[0x23] = 'D';
  scan_code_to_ascii_shifted[0x24] = 'E'; scan_code_to_ascii_shifted[0x2B] = 'F';
  scan_code_to_ascii_shifted[0x34] = 'G'; scan_code_to_ascii_shifted[0x33] = 'H'; 
  scan_code_to_ascii_shifted[0x43] = 'I'; scan_code_to_ascii_shifted[0x3B] = 'J';
  scan_code_to_ascii_shifted[0x42] = 'K'; scan_code_to_ascii_shifted[0x4B] = 'L';
  scan_code_to_ascii_shifted[0x3A] = 'M'; scan_code_to_ascii_shifted[0x31] = 'N';
  scan_code_to_ascii_shifted[0x44] = 'O'; scan_code_to_ascii_shifted[0x4D] = 'P';
  scan_code_to_ascii_shifted[0x15] = 'Q'; scan_code_to_ascii_shifted[0x2D] = 'R';
  scan_code_to_ascii_shifted[0x1B] = 'S'; scan_code_to_ascii_shifted[0x2C] = 'T';
  scan_code_to_ascii_shifted[0x3C] = 'U'; scan_code_to_ascii_shifted[0x2A] = 'V';
  scan_code_to_ascii_shifted[0x1D] = 'W'; scan_code_to_ascii_shifted[0x22] = 'X';
  scan_code_to_ascii_shifted[0x35] = 'Y'; scan_code_to_ascii_shifted[0x1A] = 'Z';

  // Mapiranja za brojeve sa Shift
  scan_code_to_ascii_shifted[0x45] = ')'; scan_code_to_ascii_shifted[0x16] = '!';
  scan_code_to_ascii_shifted[0x1E] = '@'; scan_code_to_ascii_shifted[0x26] = '#';
  scan_code_to_ascii_shifted[0x25] = '$'; scan_code_to_ascii_shifted[0x2E] = '%';
  scan_code_to_ascii_shifted[0x36] = '^'; scan_code_to_ascii_shifted[0x3D] = '&';
  scan_code_to_ascii_shifted[0x3E] = '*'; scan_code_to_ascii_shifted[0x46] = '(';
  scan_code_to_ascii_shifted[0x0E] = '~'; scan_code_to_ascii_shifted[0x4E] = '_';
  scan_code_to_ascii_shifted[0x55] = '+'; scan_code_to_ascii_shifted[0x54] = '{';
  scan_code_to_ascii_shifted[0x5B] = '}'; scan_code_to_ascii_shifted[0x4C] = ':';
  scan_code_to_ascii_shifted[0x52] = '"'; scan_code_to_ascii_shifted[0x41] = '<';
  scan_code_to_ascii_shifted[0x49] = '>'; scan_code_to_ascii_shifted[0x4A] = '?';
  scan_code_to_ascii_shifted[0x5D] = '|';*/
}

void ps2_check_clock() 
{
  static uint8_t bitcount = 0;
  static uint8_t incoming = 0;
  static bool prev_clock_state = HIGH;
  
  bool current_clock_state = digitalRead(PS2_PIN_CLOCK);
  
  if (current_clock_state == LOW && prev_clock_state == HIGH) 
  {
    incoming |= (digitalRead(PS2_PIN_DATA) << bitcount);
    bitcount++;
    if (bitcount == 8) 
    {
      _decode(incoming);
      bitcount = 0;
      incoming = 0;
    }
  }
  
  prev_clock_state = current_clock_state;
}

void print_buffer() 
{
    Serial.print("Buffer: ");
    for (uint8_t i = _bufbeg; i != _bufend; i = (i + 1) % PS2_BUF_LEN) 
    {
        Serial.write(_buf[i]);
    }
    Serial.println();
}

void _decode(unsigned char sc) 
{
  //Serial.print("\n ----- \n");
  //Serial.print("Scancode: ");
  //Serial.println(sc, HEX);

  if (sc == 0xE0) 
  {
    extended = true;
    return;
  } 
  else if (sc == 0xF0) 
  {
    // Key release
    extended = false;
    return;
  } 

  
  if (sc == 0x12 || sc == 0x59) 
  {
    // Shift key press
    shift = true;
    Serial.println("Shift pressed");
    return;
  } 
  else if (sc == 0xF0 && (sc == 0x12 || sc == 0x59)) 
  {
    // Shift key release
    shift = false;
    Serial.println("Shift released");
    return;
  }

  if (!extended) 
  {
    // Decode character based on scancode
    unsigned char c = shift ? scan_code_to_ascii_shifted[sc] : scan_code_to_ascii[sc];
    if (c) 
    {
      ps2_buf_push(c);
      print_buffer();
      //Serial.write("Character: ");
      //Serial.(c);
    }
  }

  extended = false; 
}

void ps2_buf_push(unsigned char c) 
{
  _buf[_bufend++] = c;
  _bufend %= PS2_BUF_LEN;
}

unsigned char ps2_buf_pull(void) 
{
  if (_bufbeg == _bufend)
    return 0;
  unsigned char c = _buf[_bufbeg++];
  _bufbeg %= PS2_BUF_LEN;
  return c;
}

void ps2_buf_empty(void) 
{
  _bufbeg = _bufend = 0;
}
