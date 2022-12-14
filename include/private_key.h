#ifndef PRIVATE_KEY_H
#define PRIVATE_KEY_H

// TEST KEY. DO NOT USE FOR PRODUCTION

unsigned char example_key_der[] = {
  0x30, 0x82, 0x02, 0x5d, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xce,
  0xe6, 0x25, 0x6f, 0x3c, 0xfb, 0xda, 0x38, 0x2b, 0xd7, 0x62, 0xcc, 0x04,
  0x9c, 0x58, 0xc8, 0xc8, 0x91, 0x14, 0x93, 0xb4, 0x0e, 0xa4, 0x09, 0x78,
  0xd8, 0xcb, 0xfe, 0x13, 0x30, 0x6d, 0xd8, 0x22, 0x21, 0xf6, 0xdf, 0x7e,
  0xc3, 0xc6, 0x92, 0x2f, 0xc6, 0x84, 0x43, 0xff, 0xb4, 0xa1, 0x6b, 0x13,
  0x5f, 0x36, 0xa7, 0xc3, 0x99, 0x32, 0x03, 0x89, 0x50, 0x30, 0x1e, 0xad,
  0xc6, 0x36, 0xe7, 0x73, 0x48, 0x09, 0x28, 0x4f, 0xb9, 0x46, 0xbf, 0xdd,
  0x76, 0x10, 0xc3, 0x07, 0x33, 0x9b, 0xd0, 0x8d, 0xb2, 0x24, 0xd4, 0xcb,
  0x2e, 0x90, 0x06, 0xbf, 0xf1, 0xfa, 0xae, 0x06, 0x5c, 0xec, 0x5d, 0xe8,
  0x61, 0x06, 0x4a, 0x3a, 0x2e, 0x2b, 0x1e, 0x60, 0xf2, 0xc4, 0x09, 0xca,
  0xe6, 0x27, 0x64, 0x31, 0x9c, 0xbd, 0x2d, 0x3a, 0x56, 0x27, 0x6d, 0x23,
  0x67, 0x21, 0x11, 0x6f, 0x50, 0xca, 0x11, 0x02, 0x03, 0x01, 0x00, 0x01,
  0x02, 0x81, 0x80, 0x7b, 0x28, 0x2e, 0x12, 0x58, 0x27, 0xc6, 0xce, 0xf6,
  0xf1, 0xe0, 0x02, 0x77, 0xa0, 0x25, 0x8d, 0x67, 0x2e, 0x4d, 0x24, 0x5a,
  0xe2, 0xf8, 0x2c, 0x17, 0x3e, 0x5d, 0xb7, 0x60, 0xee, 0xcc, 0x04, 0x02,
  0xd5, 0x5a, 0xe1, 0xd0, 0xd0, 0x72, 0xcc, 0x24, 0x1a, 0x34, 0x33, 0x51,
  0xeb, 0xd0, 0xc6, 0x2f, 0x22, 0xd7, 0x22, 0xe7, 0xe0, 0xb2, 0x0f, 0xbe,
  0xd5, 0xf7, 0xbe, 0xdb, 0x4c, 0x08, 0xf3, 0x8b, 0xb2, 0x04, 0x7e, 0x45,
  0x2d, 0x7e, 0xff, 0x98, 0xc2, 0x4f, 0xce, 0xa4, 0x98, 0x06, 0x08, 0x36,
  0x2e, 0x6c, 0xd3, 0xc6, 0x1c, 0x29, 0x26, 0x96, 0xcd, 0xeb, 0x40, 0xa5,
  0xf5, 0xf1, 0x1c, 0xd6, 0x21, 0xbd, 0x1b, 0x2b, 0xba, 0x0f, 0xba, 0x69,
  0xf4, 0xb9, 0x39, 0x78, 0xbc, 0xfe, 0x95, 0x3a, 0xb9, 0xbf, 0x85, 0x9e,
  0x86, 0xfb, 0x39, 0x5c, 0xd1, 0xf0, 0x37, 0xbc, 0x40, 0xfc, 0x51, 0x02,
  0x41, 0x00, 0xfd, 0xd7, 0x02, 0xf9, 0xc6, 0xf6, 0x24, 0x0e, 0x57, 0x9e,
  0xb1, 0xf0, 0x55, 0x9a, 0x10, 0xa8, 0x65, 0xf9, 0x55, 0x54, 0xe7, 0x99,
  0x0f, 0xd5, 0x5d, 0xe4, 0xff, 0x70, 0x68, 0xc1, 0xbe, 0x58, 0x78, 0x2c,
  0x84, 0xb7, 0xd2, 0x8a, 0xde, 0xa5, 0x6e, 0x01, 0x12, 0xc8, 0x58, 0x02,
  0x46, 0x7d, 0x43, 0xd7, 0x5b, 0x43, 0xfb, 0x97, 0x20, 0x22, 0x87, 0x71,
  0x87, 0xd3, 0x44, 0x8c, 0xfd, 0xbd, 0x02, 0x41, 0x00, 0xd0, 0xa8, 0xdf,
  0xcd, 0xc8, 0x55, 0x12, 0x80, 0xf6, 0xb8, 0x1c, 0x55, 0xa7, 0x6a, 0xd7,
  0xad, 0x7f, 0xab, 0xed, 0xc5, 0x19, 0xfa, 0x9a, 0x89, 0x11, 0x6f, 0xc9,
  0xf2, 0xa9, 0x03, 0x99, 0x0b, 0xe4, 0xda, 0x17, 0x02, 0x11, 0xb7, 0x80,
  0x3b, 0x7d, 0x30, 0xae, 0xa9, 0x8b, 0xc8, 0xc6, 0x39, 0x9c, 0x73, 0xa5,
  0xe3, 0x16, 0xe2, 0x15, 0xed, 0xf8, 0x38, 0xff, 0xce, 0x71, 0x0e, 0x10,
  0xe5, 0x02, 0x41, 0x00, 0xb1, 0xc8, 0xfe, 0xf7, 0x8c, 0x47, 0x66, 0xf7,
  0x78, 0x9c, 0xd8, 0x89, 0xb8, 0x9a, 0xc0, 0x62, 0x01, 0x92, 0x01, 0x17,
  0x07, 0x62, 0xa7, 0xb9, 0x4c, 0x1b, 0x10, 0x61, 0x5d, 0xad, 0x9c, 0xb0,
  0x7f, 0xf2, 0xc6, 0x3d, 0xad, 0x43, 0xc0, 0x2e, 0xe3, 0x7d, 0xf2, 0xf6,
  0xc8, 0xd5, 0x47, 0x23, 0x82, 0xf9, 0x79, 0x9d, 0x82, 0xbf, 0xd5, 0x2c,
  0xf9, 0xea, 0x25, 0x34, 0x6e, 0x45, 0xc5, 0x8d, 0x02, 0x40, 0x5d, 0x25,
  0x86, 0x03, 0x0f, 0x13, 0x2b, 0x17, 0x77, 0x0b, 0xe9, 0x5a, 0x33, 0x4a,
  0x76, 0xcd, 0x74, 0xd9, 0x03, 0x63, 0xa1, 0x9d, 0x45, 0xaf, 0x3a, 0xa1,
  0x74, 0xbd, 0x66, 0xc5, 0xbc, 0x64, 0x9a, 0xdc, 0xe0, 0xb8, 0x83, 0xc0,
  0x2e, 0xf6, 0x5f, 0x84, 0x83, 0xf4, 0x1b, 0xfa, 0x9c, 0xc2, 0xcb, 0x1c,
  0xb5, 0x49, 0x12, 0xc6, 0x0a, 0x94, 0x18, 0xe3, 0x19, 0x0e, 0xc7, 0x59,
  0x48, 0x21, 0x02, 0x41, 0x00, 0xaa, 0x5d, 0x55, 0xc3, 0xee, 0xf7, 0x45,
  0xbd, 0xa5, 0x00, 0x32, 0xb9, 0xa1, 0x71, 0x49, 0xd5, 0x8c, 0x32, 0xe0,
  0xc7, 0xd5, 0xf0, 0x64, 0xa9, 0xb5, 0xaf, 0x1b, 0x25, 0xdf, 0x34, 0xed,
  0xd4, 0xa6, 0xe1, 0x77, 0xfe, 0x9b, 0xc3, 0xed, 0x9b, 0x74, 0xca, 0xbf,
  0x6d, 0xa4, 0x85, 0x5a, 0x37, 0xd8, 0xf3, 0xad, 0xae, 0x91, 0x4f, 0xa1,
  0x30, 0x24, 0xef, 0x3c, 0x4f, 0x49, 0xec, 0x34, 0xa5
};
unsigned int example_key_der_len = 609;

#endif //PRIVATE_KEY_H