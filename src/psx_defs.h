#pragma once

typedef enum {
  PSX_Digital   = 0x41,
  PSX_AnalogRed = 0x73
} PSX_Identity_t;

typedef enum {
  PSX_Select      = 0x0001,
  PSX_R3          = 0x0002,
  PSX_L3          = 0x0004,
  PSX_Start       = 0x0008,
  PSX_DPad_Up     = 0x0010,
  PSX_DPad_Right  = 0x0020,
  PSX_DPad_Down   = 0x0040,
  PSX_DPad_Left   = 0x0080,
  PSX_L2          = 0x0100,
  PSX_R2          = 0x0200,
  PSX_L1          = 0x0400,
  PSX_R1          = 0x0800,
  PSX_Triangle    = 0x1000,
  PSX_Circle      = 0x2000,
  PSX_Cross       = 0x4000,
  PSX_Square      = 0x8000
} PSX_Button_t;

typedef struct {
  union {
    struct {
      uint16_t  psbuttons;
      union {
        struct {
          uint8_t rx, ry, lx, ly;
        };
        uint8_t axis[4];
      };
    };
    uint8_t raw[6];
  };
} PSX_Input_t;

// Packet describes both digital and red analog controllers
typedef struct {
  uint8_t index;      // The byte we're currently writing
  bool    shouldPoll; // Whether more data should be polled
  union {
    struct {
      PSX_Identity_t identity;   // Our controller type
      const uint8_t  padding_5a; // Should always be 0x5a
      PSX_Input_t    input;      // Our input
    };
    uint8_t raw[8]; // Identity + 5A + up to 3 words
  };
} PSX_Controller_t;

int *but_cur;

int *get_bits(uint16_t n, int bitswanted){
  int *bits = malloc(sizeof(int) * bitswanted);

  int k;
  for(k=0; k<bitswanted; k++){
    int mask =  1 << k;
    int masked_n = n & mask;
    int thebit = masked_n >> k;
    bits[k] = thebit;
  }

  return bits;
}

// Storage for generic console input report
PSX_Controller_t _psx = {
  .identity   = PSX_Digital,
  .padding_5a = 0x5A
};

// PlayStation controller mapping
void CALLBACK_OnPlaystationInputRequest(PSX_Input_t *input, uint16_t but) {
  const PSX_Button_t button_mapping[] = {
    PSX_Triangle,
    PSX_Circle,
    PSX_R1,
    PSX_Cross,
    PSX_L1,
    PSX_Square,
	PSX_R2,
	PSX_DPad_Up,
	PSX_L2,
    PSX_Start,
    PSX_Select
  };

  but_cur = get_bits(but, 11);
  for (uint8_t i = 0; i < 11; i++)
    if (but_cur[i]) input->psbuttons |= button_mapping[i]; //sw_val[i]
}