#ifndef INC_SCANNER_H
#define INC_SCANNER_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLE2902.h>

extern void make_angry(void);
void scanner_start(void);
int scanner_get_count(void);

#endif /* INC_SCANNER_H */