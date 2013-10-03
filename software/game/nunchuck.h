#ifndef NUNCHUCK_H_
#define NUNCHUCK_H_

// Nunchuck clock frequency is 100kHz

typedef struct {
  char port;
  char values[6];
  bool c, z;
} Nunchuck;

void initialize_nunchuck(Nunchuck *nunchuck) {
  send(nunchuck->port, 0xA4);
  send(nunchuck->port, 0x40);
  send(nunchuck->port, 0x00);
}

void read_nunchuck(Nunchuck *nunchuck) {
  send(nunchuck->port, 0x00);

  for(int i = 0; i < 6; i++) {
    values[i] = decode(read(nunchuck->port));
  }

  values[5] = values[5] & 0x03;
  switch (values[5]) {
    case 0x00: 
      nunchuck->c = true;
      nunchuck->z = true;
      break;
    case 0x01: 
      nunchuck->c = true;
      nunchuck->z = false;
      break;
    case 0x02: 
      nunchuck->c = false;
      nunchuck->z = true;
      break;
    default: 
      nunchuck->c = false;
      nunchuck->z = false;
      break;
  }
}

char decode(char value) {
  return (value ^ 0x17) + 0x17;
}

void debug(Nnunchuck *nunchuck) {
  printf ("\n\nSx = %d, Sy = %d", nunchuck->values[0], nunchuck->values[1]);
  printf("\nAx = %d, Ay = %d, Az = %d, C = %x, Z = %x", nunchuck->values[2], nunchuck->values[3], 
              nunchuck->values[4], nunchuck->c, nunchuck->z);
}

// I2C methods

void send(char port, char data) {

}

char read(char port) {
  char value;
  return value;
}

#endif