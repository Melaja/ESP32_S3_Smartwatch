/**
 * @file nimconfig.h
 * @brief NimBLE configuration for ESP32 Smartwatch
 *
 * This file overrides default NimBLE settings to minimize code size.
 * We only need BLE Client functionality for ANCS/AMS.
 */

#pragma once

// ============================================================================
// CORE SETTINGS - Minimize for client-only usage
// ============================================================================

// Disable BLE Server (we only need client for ANCS)
#define CONFIG_BT_NIMBLE_ROLE_CENTRAL_DISABLED 0
#define CONFIG_BT_NIMBLE_ROLE_OBSERVER_DISABLED 0
#define CONFIG_BT_NIMBLE_ROLE_PERIPHERAL_DISABLED 1
#define CONFIG_BT_NIMBLE_ROLE_BROADCASTER_DISABLED 1

// Maximum connections (1 is enough for iPhone)
#define CONFIG_BT_NIMBLE_MAX_CONNECTIONS 1

// Maximum bonded devices
#define CONFIG_BT_NIMBLE_MAX_BONDS 3

// ============================================================================
// DISABLE UNUSED FEATURES
// ============================================================================

// Disable extended advertising (not needed for ANCS)
#define CONFIG_BT_NIMBLE_EXT_ADV 0

// Disable 2M and coded PHY (saves flash)
#define CONFIG_BT_NIMBLE_50_FEATURE_SUPPORT 0

// Disable GATT caching (saves RAM)
#define CONFIG_BT_NIMBLE_GATT_CACHING 0

// ============================================================================
// MEMORY OPTIMIZATION
// ============================================================================

// Reduce ATT MTU size
#define CONFIG_BT_NIMBLE_ATT_PREFERRED_MTU 256

// Reduce task stack size
#define CONFIG_BT_NIMBLE_HOST_TASK_STACK_SIZE 4096

// Reduce number of GATT connections for client
#define CONFIG_BT_NIMBLE_MAX_CCCDS 8

// ============================================================================
// DEBUG SETTINGS - Disable for release
// ============================================================================

// Disable debug logging to save flash
#define CONFIG_BT_NIMBLE_DEBUG 0
#define CONFIG_BT_NIMBLE_LOG_LEVEL 0

// Disable NimBLE debug output
#define CONFIG_NIMBLE_CPP_LOG_LEVEL 0

// ============================================================================
// SECURITY SETTINGS - Required for ANCS
// ============================================================================

// Enable security manager (required for iOS pairing)
#define CONFIG_BT_NIMBLE_SM_ENABLED 1

// Enable bonding
#define CONFIG_BT_NIMBLE_SM_BONDING 1

// Enable secure connections
#define CONFIG_BT_NIMBLE_SM_SC_ENABLED 1
