#include "scanner.hpp"

// Declare task handle
TaskHandle_t ScannerTaskHandle = NULL;
int scanTime = 5;  //In seconds
BLEScan *pBLEScan;
int nb_devices = 0;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onReszult(BLEAdvertisedDevice advertisedDevice) {
    nb_devices++;
  }
};

int scanner_get_count(void)
{
  return nb_devices;
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
