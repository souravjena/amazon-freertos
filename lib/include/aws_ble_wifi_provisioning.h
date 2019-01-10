/*
 * Amazon FreeRTOS
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file aws_ble_wifi_provisioning.h
 * @brief WiFi provisioning Gatt service.
 */

#ifndef _AWS_BLE_WIFI_PROVISIONING_H_
#define _AWS_BLE_WIFI_PROVISIONING_H_

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "aws_ble.h"
#include "aws_wifi.h"

/**
 * @brief GATT Service, characteristic and descriptor UUIDs used by the WiFi provisioning service.
 */
#define wifiProvSVC_UUID_BASE               { 0x00, 0x00, 0x1B, 0xE1, 0x14, 0xC6, 0x83, 0xAA, 0x9A, 0x4F, 0x9F, 0x4B, 0x87, 0xA1, 0x13, 0x31 }
#define wifiProvSVC_UUID                    ( 0xFF00 )
#define wifiProvLIST_NETWORK_CHAR_UUID      ( 0xFF01 )
#define wifiProvSAVE_NETWORK_CHAR_UUID      ( 0xFF02 )
#define wifiProvEDIT_NETWORK_CHAR_UUID      ( 0xFF03 )
#define wifiProvDELETE_NETWORK_CHAR_UUID    ( 0xFF04 )
#define wifiProvCLIENT_CHAR_CFG_UUID        ( 0x2902 )

/**
 * @brief Number of characteristics, descriptors and included services used by WiFi provisioning.
 */
#define wifiProvNUM_CHARS                   ( 4 )
#define wifiProvNUM_DESCRS                  ( 4 )
#define wifiProvNum_INCL_SERVICES           ( 0 )

/**
 * @brief GATT characteristics used by the WiFi provisioning service.
 */
typedef enum
{
    eListNetworkChar = 0, /**< eListNetworkChar Used by the GATT client to list the saved networks and scanned networks */
    eSaveNetworkChar,     /**< eSaveNetworkChar Used by the GATT client to provision a new WiFi network on the device */
    eEditNetworkChar,     /**< eEditNetworkChar Used by the GATT client to change the priority order of the saved networks on the device */
    eDeleteNetworkChar,   /**< eDeleteNetworkChar Used by the GATT client to delete the saved WiFi network on the device */
    eMaxChars             /**< eMaxChars */
} WifiProvCharacteristic_t;

/**
 * @brief GATT descriptors used by the WiFi provisioning service.
 */
typedef enum
{
    eListNetworkCharCCFGDescr = 0, /**< eListNetworkCharCCFGDescr Client Characteristic Configuration descriptor to enable notifications to send List Network response */
    eSaveNetworkCharCCFGDescr,     /**< eSaveNetworkCharCCFGDescr Client Characteristic Configuration descriptor to enable notifications to send Save Network response */
    eEditNetworkCharCCFGDescr,     /**< eEditNetworkCharCCFGDescr Client Characteristic Configuration descriptor to enable notifications to send Edit Network response */
    eDeleteNetworkCharCCFGDescr,   /**< eDeleteNetworkCharCCFGDescr Client Characteristic Configuration descriptor to enable notifications to send Delete Network response */
    eMaxDescriptors                /**< eMaxDescriptors */
} WifiProvDescriptor_t;


/**
 * @brief Tokens used within the JSON messages exchanged between GATT client and server.
 */

#define wifiProvMAX_NETWORKS_KEY    "maxNetworks"
#define wifiProvSCAN_TIMEOUT_KEY    "timeout"
#define wifiProvKEY_MGMT_KEY        "security"
#define wifiProvSSID_KEY            "ssid"
#define wifiProvBSSID_KEY           "bssid"
#define wifiFREQ_KEY                "freqMhz"
#define wifiRSSI_KEY                "rssi"
#define wifiProvPSK_KEY             "psk"
#define wifiProvSTATUS_KEY          "status"
#define wifiProvHIDDEN_KEY          "hidden"
#define wifiProvCONNECTED_KEY       "connected"
#define wifiProvINDEX_KEY           "index"
#define wifiProvNEWINDEX_KEY        "newIndex"

#define wifiProvNUM_NETWORK_INFO_MESG_PARAMS  ( 8 )
#define wifiProvNUM_STATUS_MESG_PARAMS        ( 1 )

/**
 * @brief List Network request sent by the GATT client to list saved and scanned networks.
 */
typedef struct ListNetworkRequest
{
    int16_t sMaxNetworks; /**< Max Networks to scan in one request */
    int16_t sTimeoutMs;   /**< Timeout in MS for scanning */
} ListNetworkRequest_t;

/**
 * @brief Sent by the GATT client to provision a new WiFi network.
 */
typedef struct AddNetworkRequest
{
    WIFINetworkProfile_t xNetwork; /**< The configuration for the new WIFI network */
    int16_t sSavedIdx;             /**< Index if its an already saved WIFI network in the flash */
} AddNetworkRequest_t;

/**
 * @brief Sent by the GATT client to change the saved WiFi networks priority order.
 */
typedef struct EditNetworkRequest
{
    int16_t sCurIdx; /**< Current priority of the saved WiFi network */
    int16_t sNewIdx; /**< New priority of the saved WiFi network */
} EditNetworkRequest_t;

/**
 * @brief Sent by the GATT client to delete a saved WIFI network from flash.
 *
 */
typedef struct DeleteNetworkRequest
{
    int16_t sIdx; /**< Index/priority of the saved WiFi network */
} DeleteNetworkRequest_t;

/**
 * @brief Response type used to send a WIFI network
 */

typedef struct WifiNetworkInfo
{
    WIFIReturnCode_t xStatus;
	const char* pcSSID;
	size_t xSSIDLength;
	const uint8_t* pucBSSID;
	size_t xBSSIDLength;
	WIFISecurity_t xSecurity;
	int8_t cRSSI;
	uint8_t ucHidden;
	uint8_t ucConnected;
    int32_t sSavedIdx;
} WifiNetworkInfo_t;

/**
 * @brief Events Used by the WIFI provisioning service.
 */
typedef enum
{
    eWIFIPROVStarted = 0x01,   /**< eWIFIPROVInit  Initialized the WiFi provisioning service */                                              //!< eWIFIPROVStarted
    eWIFIPROVConnect = 0x02,   /**< eWIFIPROVConnect Set When WiFi provisioning service connects to one of the saved networks in the flash *///!< eWIFIPROVConnect
    eWIFIPROVConnected = 0x04, /**< eWIFIPROVConnected Set when successfully connected to a WiFi Network */                                  //!< eWIFIPROVConnected
    eWIFIPROVStopped = 0x08,   /**<  eWIFIProvStopped Set when WiFi provisioning is stopped */                                               //!< eWIFIPROVStopped
    eWIFIPROVDeleted = 0x10,   /**<  eWIFIProvStopped Set when WiFi provisioning is deleted */                                               //!< eWIFIPROVDeleted
    eWIFIPROVFailed = 0x20     /**< eWIFIPROVFailed Set When WiFi provisioning failed */                                                     //!< eWIFIPROVFailed
} WifiProvEvent_t;

#define ALL_EVENTS    ( eWIFIPROVStarted | eWIFIPROVConnect | eWIFIPROVConnected | eWIFIPROVStopped | eWIFIPROVDeleted | eWIFIPROVFailed )

/**
 * @brief Structure used for WiFi provisioning service.
 */
typedef struct WifiProvService
{
    struct BLEService * pxGattService;
    uint16_t usNotifyClientEnabled;
    uint16_t usBLEConnId;
    EventGroupHandle_t xEventGroup;
    SemaphoreHandle_t xLock;
    uint16_t usNumNetworks;
    uint16_t usNextConnectIdx;
    int16_t sConnectedIdx;
    BaseType_t xInit;
    TaskHandle_t xConnectTask;
} WifiProvService_t;

#define wifiProvIS_SUCCESS( btstatus )    ( ( btstatus ) == eBTStatusSuccess )


#define wifiProvINVALID_NETWORK_RSSI      ( -100 )


#define wifiProvINVALID_NETWORK_INDEX     ( -1 )


/**
 * @brief Maximum number of WiFi networks that can be provisioned
 */
#define wifiProvMAX_SAVED_NETWORKS    ( 8 )

/**
 * @brief Delay between connecting to the saved list of WiFi networks
 */
#define wifiProvSAVED_NETWORKS_CONNECTION_INTERVAL_MS ( 1000 )

/**
 * @brief Base priority for all the tasks
 */
#define wifiProvTASK_PRIORITY_BASE      ( tskIDLE_PRIORITY )

/**
 * @brief Task priority for background task to connect to saved networks.
 */
#define wifiProvCONNECT_AP_TASK_PRIORITY  ( wifiProvTASK_PRIORITY_BASE )

/**
 * @brief Priority for the task to list all the WiFi networks.
 */
#define wifiProvLIST_NETWORK_TASK_PRIORITY  ( wifiProvTASK_PRIORITY_BASE + 1 )

/**
 * @brief Priority for the task to modify WiFi networks.
 */
#define wifiProvMODIFY_NETWORK_TASK_PRIORITY  ( wifiProvTASK_PRIORITY_BASE + 2 )


/**
 * @brief Initialize the wifi provisioning service.
 *
 * Creates GATT service and characteristics required for WiFi provisioning over BLE. User needs to call it once
 * before starting the BLE advertisement.
 *
 * @return pdTRUE if the initialization succeeded.
 *         pdFALSE if the initialization failed.
 */
BaseType_t WIFI_PROVISION_Init( void );

/**
 * @brief Starts WiFi provisioning service.
 *
 *  Starts background task to connect to one of the configured WiFi networks, if its not already connected.
 *
 * @return pdTRUE if successfully started
 *         pdFALSE if start failed
 */
BaseType_t WIFI_PROVISION_Start( void );

/**
 *
 **@brief Blocks until WiFi is connected or given wait time is reached.
 *
 * @param[in] xWaitTicks Time in FreeRTOS ticks to wait until the WiFi is connected.
 * @return pdTRUE if the WiFi is connected
 *         pdFALSE if the WiFi provisioning is in progress even after specified wait time has reached or has failed.
 *
 */
BaseType_t WIFI_PROVISION_IsConnected( uint32_t xWaitTicks );


/**
 * @brief Gets the connected network profile.
 * @param[out] pxNetwork The network profile for the connected network
 * @return pdTRUE if network is connected and the connected network profile is found.
 *         pdFALSE if the network is not connected.
 */
BaseType_t WIFI_PROVISION_GetConnectedNetwork( WIFINetworkProfile_t * pxNetwork );

/**
 * @brief Stops WIFI provisioning service
 *
 * Stops GATT service, Pauses the background task which connects to saved WiFi networks.
 *
 * @return pdTRUE if GATT service is stopped successufly
 */
BaseType_t WIFI_PROVISION_Stop( void );

/**
 * @brief Tear down WIFI provisioning service
 *
 * Deletes the GATT service and deletes the background task which connects to saved WiFi networks.
 * @return pdTRUE if delete succeeds, pdFALSE otherwise
 */
BaseType_t WIFI_PROVISION_Delete( void );

#endif /* _AWS_BLE_WIFI_PROVISIONING_H_ */