#include "scanner.hpp"

/**
Bluetooth Low Energy Bad Device List
**/

#define END_BAD_DEVICES {{0xff, 0xff, 0xff}, {0x00}}
volatile int g_scanner_threshold = -80;

typedef struct {
  /* Device OUI. */
  uint8_t oui[3];

  /* Device name's mask (0xFF -> wildcard). */
  uint8_t name[32];
} bad_device_t;

bad_device_t badDevicesList[] = {
  {
    {0x80, 0xe1, 0x26},
    {0x46, 0x6c, 0x69, 0x70, 0x70, 0x65, 0x72, 0x20, 0x00}
  },
  {
    {0x80, 0xe1, 0x27},
    {0x46, 0x6c, 0x69, 0x70, 0x70, 0x65, 0x72, 0x20, 0x00}
  },
  {
    {0x0c, 0xfa, 0x22},
    {0x46, 0x6c, 0x69, 0x70, 0x70, 0x65, 0x72, 0x20, 0x00}
  },
  END_BAD_DEVICES
};


// Declare task handle
TaskHandle_t ScannerTaskHandle = NULL;
int scanTime = 5;  //In seconds
BLEScan *pBLEScan;
int nb_devices = 0;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    uint8_t oui[3];
    uint8_t *p_mask;
    uint8_t *p_name = NULL;
    bad_device_t *p_list = badDevicesList;

    /* Extract the last 4 bytes of BD address. */
    memcpy(oui, advertisedDevice.getAddress().getNative()+3, 3);

    while ((p_list->oui[0] != 0xFF) || (p_list->oui[1] !=0xFF) || (p_list->oui[2] != 0xFF))
    {
      /* Check OUI first. */
      if ((p_list->oui[0] == oui[2]) && (p_list->oui[1] == oui[1]) && (p_list->oui[2] == oui[0]))
      {
        if (g_scanner_threshold < advertisedDevice.getRSSI())
        {
          /* OUI matches, make sure name's mask is found before reporting. */
          p_mask = p_list->name;
          p_name = (uint8_t *)advertisedDevice.getName().c_str();
          while (*p_mask != 0x00)
          {
            if (*p_name == 0x00)
              break;

            if ((*p_mask != 0xFF) && (*p_name != *p_mask))
              break;

            p_name++;
            p_mask++;
          }

          /* Check if we passed. */
          if (*p_mask == 0x00)
          {
            /* Bad device found ! */
            make_angry();
          }
        }
      }

      p_list++;
    }
  }
};

void scanner_set_threshold(int threshold)
{
  g_scanner_threshold = threshold;
}

void BleScannerTask(void *parameter) {
  /* Initialize our scanner. */
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  /* Scan devices. */
  for (;;) { // Infinite loop
    BLEScanResults *foundDevices = pBLEScan->start(scanTime, false);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void scanner_start(void)
{
  xTaskCreate(
    BleScannerTask,         // Task function
    "BleScannerTask",       // Task name
    30000,             // Stack size (bytes)
    NULL,              // Parameters
    1,                 // Priority
    &ScannerTaskHandle
  );
}
