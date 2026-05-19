/*
 * Modbus CRC-16 Standard Test Vectors
 * Author: 臧德运 (Zang Deyun)
 * License: MIT
 */

#ifndef TEST_VECTORS_H
#define TEST_VECTORS_H

#include <stdint.h>

/* Standard Modbus CRC-16 test vectors */
typedef struct {
    const uint8_t *data;
    uint16_t len;
    uint16_t expected_crc;
    const char *description;
} test_vector_t;

/* Test vector 1: Single byte */
static const uint8_t test_data_1[] = {0x01};

/* Test vector 2: Two bytes */
static const uint8_t test_data_2[] = {0x01, 0x03};

/* Test vector 3: Three bytes */
static const uint8_t test_data_3[] = {0x01, 0x03, 0x00};

/* Test vector 4: Standard Modbus read request */
static const uint8_t test_data_4[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0A};

/* Test vector 5: Standard Modbus write request */
static const uint8_t test_data_5[] = {0x01, 0x10, 0x00, 0x01, 0x00, 0x02, 0x04, 0x00, 0x0A, 0x00, 0x14};

/* Test vector 6: Empty frame (edge case) */
static const uint8_t test_data_6[] = {};

/* Test vector 7: Maximum length single byte pattern */
static const uint8_t test_data_7[] = {0xFF, 0xFF, 0xFF, 0xFF};

/* Test vector 8: Alternating pattern */
static const uint8_t test_data_8[] = {0xAA, 0x55, 0xAA, 0x55};

/* Test vector array */
static const test_vector_t test_vectors[] = {
    {test_data_1, sizeof(test_data_1), 0x807E, "Single byte: 0x01"},
    {test_data_2, sizeof(test_data_2), 0x0979, "Two bytes: 0x01, 0x03"},
    {test_data_3, sizeof(test_data_3), 0xFB87, "Three bytes: 0x01, 0x03, 0x00"},
    {test_data_4, sizeof(test_data_4), 0x????, "Modbus read request (fill in expected)"},
    {test_data_5, sizeof(test_data_5), 0x????, "Modbus write request (fill in expected)"},
    {test_data_6, 0, 0xFFFF, "Empty frame (edge case)"},
    {test_data_7, sizeof(test_data_7), 0x????, "All 0xFF pattern (fill in expected)"},
    {test_data_8, sizeof(test_data_8), 0x????, "Alternating pattern (fill in expected)"},
};

#define NUM_TEST_VECTORS (sizeof(test_vectors) / sizeof(test_vectors[0]))

#endif /* TEST_VECTORS_H */
