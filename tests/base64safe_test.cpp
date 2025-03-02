// This file is part of base64safe
// Copyright (c) 2025 Christopher Feghali
// SPDX-License-Identifier: 0BSD

#include "base64safe.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(Base64SafeTest, BasicEncode) {
  EXPECT_EQ(base64safe::encode("hello"), "aGVsbG8=");
  EXPECT_EQ(base64safe::encode("Base64"), "QmFzZTY0");
  EXPECT_EQ(base64safe::encode("test"), "dGVzdA==");
}

TEST(Base64SafeTest, BasicDecode) {
  EXPECT_EQ(base64safe::decode("aGVsbG8="), "hello");
  EXPECT_EQ(base64safe::decode("QmFzZTY0"), "Base64");
  EXPECT_EQ(base64safe::decode("dGVzdA=="), "test");
}

TEST(Base64SafeTest, RoundTrip) {
  std::string original =
      "This is a test string with numbers 12345 and symbols #!@";
  std::string encoded = base64safe::encode(original);
  std::string decoded = base64safe::decode(encoded);
  EXPECT_EQ(original, decoded);
}

TEST(Base64SafeTest, EncodeContainsUnderscore) {
  std::string input = "???";
  std::string encoded = base64safe::encode(input);
  EXPECT_EQ(encoded, "Pz8_");
}

TEST(Base64SafeTest, DecodeUnderscore) {
  std::string encoded = "Pz8_";
  std::string decoded = base64safe::decode(encoded);
  EXPECT_EQ(decoded, "???");
}

TEST(Base64SafeTest, EncodeContainsDash) {
  std::string input = "\xFA\xFB\xFC"; // Binary string
  std::string encoded = base64safe::encode(input);
  EXPECT_EQ(encoded, "-vv8"); // Expected URL-safe base64 encoding
}

TEST(Base64SafeTest, DecodeDash) {
  std::string encoded = "-vv8";
  std::string decoded = base64safe::decode(encoded);
  EXPECT_EQ(decoded,
            "\xFA\xFB\xFC"); // Check if decoding returns the original binary
}

TEST(Base64SafeTest, EmptyString) {
  EXPECT_EQ(base64safe::encode(""), "");
  EXPECT_EQ(base64safe::decode(""), "");
}

TEST(Base64SafeTest, BinaryData) {
  std::vector<uint8_t> binary = {0x00, 0x01, 0x02, 0xFF};
  std::string binary_string(binary.begin(), binary.end());
  std::string encoded = base64safe::encode(binary_string);
  std::string decoded = base64safe::decode(encoded);
  EXPECT_EQ(decoded, binary_string);
}

TEST(Base64SafeTest, AllByteValues) {
  std::vector<uint8_t> all_bytes(256);
  for (int i = 0; i < 256; ++i) {
    all_bytes[i] = static_cast<uint8_t>(i);
  }
  std::string all_bytes_string(all_bytes.begin(), all_bytes.end());
  std::string encoded = base64safe::encode(all_bytes_string);
  std::string decoded = base64safe::decode(encoded);
  EXPECT_EQ(decoded, all_bytes_string);
}

TEST(Base64SafeTest, InvalidDecode) {
  EXPECT_THROW(base64safe::decode("invalid!base64"), std::runtime_error);
  EXPECT_THROW(base64safe::decode("aGVsbG8"),
               std::runtime_error); // Invalid length
  EXPECT_THROW(base64safe::decode("aGVsbG8==="),
               std::runtime_error); // Invalid padding
}

TEST(Base64SafeTest, PaddingCases) {
  EXPECT_EQ(base64safe::encode("A"), "QQ==");
  EXPECT_EQ(base64safe::encode("AB"), "QUI=");
  EXPECT_EQ(base64safe::encode("ABC"), "QUJD");
  EXPECT_EQ(base64safe::decode("QQ=="), "A");
  EXPECT_EQ(base64safe::decode("QUI="), "AB");
  EXPECT_EQ(base64safe::decode("QUJD"), "ABC");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
