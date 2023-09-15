/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Copyright (c) 2023 ASPEED Technology Inc.
 */

/**
 * @file
 * @brief Hash shell commands.
 */

#include <stdlib.h>
#include <zephyr/device.h>
#include <zephyr/crypto/crypto.h>
#include <zephyr/crypto/hash.h>
#include <zephyr/shell/shell.h>

#ifdef CONFIG_CRYPTO_ASPEED
#define HASH_DRV_NAME CONFIG_CRYPTO_ASPEED_HASH_DRV_NAME
#endif

struct hash_testvec {
	const char *plaintext;
	const char *digest;
	unsigned int psize;
};

static const struct hash_testvec sha512_tv_template[] = {
	{
		.plaintext = "",
		.psize	= 0,
		.digest	= "\xcf\x83\xe1\x35\x7e\xef\xb8\xbd"
		"\xf1\x54\x28\x50\xd6\x6d\x80\x07"
		"\xd6\x20\xe4\x05\x0b\x57\x15\xdc"
		"\x83\xf4\xa9\x21\xd3\x6c\xe9\xce"
		"\x47\xd0\xd1\x3c\x5d\x85\xf2\xb0"
		"\xff\x83\x18\xd2\x87\x7e\xec\x2f"
		"\x63\xb9\x31\xbd\x47\x41\x7a\x81"
		"\xa5\x38\x32\x7a\xf9\x27\xda\x3e",
	}, {
		.plaintext = "abc",
		.psize	= 3,
		.digest	= "\xdd\xaf\x35\xa1\x93\x61\x7a\xba"
		"\xcc\x41\x73\x49\xae\x20\x41\x31"
		"\x12\xe6\xfa\x4e\x89\xa9\x7e\xa2"
		"\x0a\x9e\xee\xe6\x4b\x55\xd3\x9a"
		"\x21\x92\x99\x2a\x27\x4f\xc1\xa8"
		"\x36\xba\x3c\x23\xa3\xfe\xeb\xbd"
		"\x45\x4d\x44\x23\x64\x3c\xe8\x0e"
		"\x2a\x9a\xc9\x4f\xa5\x4c\xa4\x9f",
	}, {
		.plaintext = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		.psize	= 56,
		.digest	= "\x20\x4a\x8f\xc6\xdd\xa8\x2f\x0a"
		"\x0c\xed\x7b\xeb\x8e\x08\xa4\x16"
		"\x57\xc1\x6e\xf4\x68\xb2\x28\xa8"
		"\x27\x9b\xe3\x31\xa7\x03\xc3\x35"
		"\x96\xfd\x15\xc1\x3b\x1b\x07\xf9"
		"\xaa\x1d\x3b\xea\x57\x78\x9c\xa0"
		"\x31\xad\x85\xc7\xa7\x1d\xd7\x03"
		"\x54\xec\x63\x12\x38\xca\x34\x45",
	}, {
		.plaintext = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
		"hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
		.psize	= 112,
		.digest	= "\x8e\x95\x9b\x75\xda\xe3\x13\xda"
		"\x8c\xf4\xf7\x28\x14\xfc\x14\x3f"
		"\x8f\x77\x79\xc6\xeb\x9f\x7f\xa1"
		"\x72\x99\xae\xad\xb6\x88\x90\x18"
		"\x50\x1d\x28\x9e\x49\x00\xf7\xe4"
		"\x33\x1b\x99\xde\xc4\xb5\x43\x3a"
		"\xc7\xd3\x29\xee\xb6\xdd\x26\x54"
		"\x5e\x96\xe5\x5b\x87\x4b\xe9\x09",
	}, {
		.plaintext = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcd"
		"efghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",
		.psize	= 104,
		.digest	= "\x93\x0d\x0c\xef\xcb\x30\xff\x11"
		"\x33\xb6\x89\x81\x21\xf1\xcf\x3d"
		"\x27\x57\x8a\xfc\xaf\xe8\x67\x7c"
		"\x52\x57\xcf\x06\x99\x11\xf7\x5d"
		"\x8f\x58\x31\xb5\x6e\xbf\xda\x67"
		"\xb2\x78\xe6\x6d\xff\x8b\x84\xfe"
		"\x2b\x28\x70\xf7\x42\xa5\x80\xd8"
		"\xed\xb4\x19\x87\x23\x28\x50\xc9",
	}, {
		.plaintext = "\x08\x9f\x13\xaa\x41\xd8\x4c\xe3"
		"\x7a\x11\x85\x1c\xb3\x27\xbe\x55"
		"\xec\x60\xf7\x8e\x02\x99\x30\xc7"
		"\x3b\xd2\x69\x00\x74\x0b\xa2\x16"
		"\xad\x44\xdb\x4f\xe6\x7d\x14\x88"
		"\x1f\xb6\x2a\xc1\x58\xef\x63\xfa"
		"\x91\x05\x9c\x33\xca\x3e\xd5\x6c"
		"\x03\x77\x0e\xa5\x19\xb0\x47\xde"
		"\x52\xe9\x80\x17\x8b\x22\xb9\x2d"
		"\xc4\x5b\xf2\x66\xfd\x94\x08\x9f"
		"\x36\xcd\x41\xd8\x6f\x06\x7a\x11"
		"\xa8\x1c\xb3\x4a\xe1\x55\xec\x83"
		"\x1a\x8e\x25\xbc\x30\xc7\x5e\xf5"
		"\x69\x00\x97\x0b\xa2\x39\xd0\x44"
		"\xdb\x72\x09\x7d\x14\xab\x1f\xb6"
		"\x4d\xe4\x58\xef\x86\x1d\x91\x28"
		"\xbf\x33\xca\x61\xf8\x6c\x03\x9a"
		"\x0e\xa5\x3c\xd3\x47\xde\x75\x0c"
		"\x80\x17\xae\x22\xb9\x50\xe7\x5b"
		"\xf2\x89\x20\x94\x2b\xc2\x36\xcd"
		"\x64\xfb\x6f\x06\x9d\x11\xa8\x3f"
		"\xd6\x4a\xe1\x78\x0f\x83\x1a\xb1"
		"\x25\xbc\x53\xea\x5e\xf5\x8c\x00"
		"\x97\x2e\xc5\x39\xd0\x67\xfe\x72"
		"\x09\xa0\x14\xab\x42\xd9\x4d\xe4"
		"\x7b\x12\x86\x1d\xb4\x28\xbf\x56"
		"\xed\x61\xf8\x8f\x03\x9a\x31\xc8"
		"\x3c\xd3\x6a\x01\x75\x0c\xa3\x17"
		"\xae\x45\xdc\x50\xe7\x7e\x15\x89"
		"\x20\xb7\x2b\xc2\x59\xf0\x64\xfb"
		"\x92\x06\x9d\x34\xcb\x3f\xd6\x6d"
		"\x04\x78\x0f\xa6\x1a\xb1\x48\xdf"
		"\x53\xea\x81\x18\x8c\x23\xba\x2e"
		"\xc5\x5c\xf3\x67\xfe\x95\x09\xa0"
		"\x37\xce\x42\xd9\x70\x07\x7b\x12"
		"\xa9\x1d\xb4\x4b\xe2\x56\xed\x84"
		"\x1b\x8f\x26\xbd\x31\xc8\x5f\xf6"
		"\x6a\x01\x98\x0c\xa3\x3a\xd1\x45"
		"\xdc\x73\x0a\x7e\x15\xac\x20\xb7"
		"\x4e\xe5\x59\xf0\x87\x1e\x92\x29"
		"\xc0\x34\xcb\x62\xf9\x6d\x04\x9b"
		"\x0f\xa6\x3d\xd4\x48\xdf\x76\x0d"
		"\x81\x18\xaf\x23\xba\x51\xe8\x5c"
		"\xf3\x8a\x21\x95\x2c\xc3\x37\xce"
		"\x65\xfc\x70\x07\x9e\x12\xa9\x40"
		"\xd7\x4b\xe2\x79\x10\x84\x1b\xb2"
		"\x26\xbd\x54\xeb\x5f\xf6\x8d\x01"
		"\x98\x2f\xc6\x3a\xd1\x68\xff\x73"
		"\x0a\xa1\x15\xac\x43\xda\x4e\xe5"
		"\x7c\x13\x87\x1e\xb5\x29\xc0\x57"
		"\xee\x62\xf9\x90\x04\x9b\x32\xc9"
		"\x3d\xd4\x6b\x02\x76\x0d\xa4\x18"
		"\xaf\x46\xdd\x51\xe8\x7f\x16\x8a"
		"\x21\xb8\x2c\xc3\x5a\xf1\x65\xfc"
		"\x93\x07\x9e\x35\xcc\x40\xd7\x6e"
		"\x05\x79\x10\xa7\x1b\xb2\x49\xe0"
		"\x54\xeb\x82\x19\x8d\x24\xbb\x2f"
		"\xc6\x5d\xf4\x68\xff\x96\x0a\xa1"
		"\x38\xcf\x43\xda\x71\x08\x7c\x13"
		"\xaa\x1e\xb5\x4c\xe3\x57\xee\x85"
		"\x1c\x90\x27\xbe\x32\xc9\x60\xf7"
		"\x6b\x02\x99\x0d\xa4\x3b\xd2\x46"
		"\xdd\x74\x0b\x7f\x16\xad\x21\xb8"
		"\x4f\xe6\x5a\xf1\x88\x1f\x93\x2a"
		"\xc1\x35\xcc\x63\xfa\x6e\x05\x9c"
		"\x10\xa7\x3e\xd5\x49\xe0\x77\x0e"
		"\x82\x19\xb0\x24\xbb\x52\xe9\x5d"
		"\xf4\x8b\x22\x96\x2d\xc4\x38\xcf"
		"\x66\xfd\x71\x08\x9f\x13\xaa\x41"
		"\xd8\x4c\xe3\x7a\x11\x85\x1c\xb3"
		"\x27\xbe\x55\xec\x60\xf7\x8e\x02"
		"\x99\x30\xc7\x3b\xd2\x69\x00\x74"
		"\x0b\xa2\x16\xad\x44\xdb\x4f\xe6"
		"\x7d\x14\x88\x1f\xb6\x2a\xc1\x58"
		"\xef\x63\xfa\x91\x05\x9c\x33\xca"
		"\x3e\xd5\x6c\x03\x77\x0e\xa5\x19"
		"\xb0\x47\xde\x52\xe9\x80\x17\x8b"
		"\x22\xb9\x2d\xc4\x5b\xf2\x66\xfd"
		"\x94\x08\x9f\x36\xcd\x41\xd8\x6f"
		"\x06\x7a\x11\xa8\x1c\xb3\x4a\xe1"
		"\x55\xec\x83\x1a\x8e\x25\xbc\x30"
		"\xc7\x5e\xf5\x69\x00\x97\x0b\xa2"
		"\x39\xd0\x44\xdb\x72\x09\x7d\x14"
		"\xab\x1f\xb6\x4d\xe4\x58\xef\x86"
		"\x1d\x91\x28\xbf\x33\xca\x61\xf8"
		"\x6c\x03\x9a\x0e\xa5\x3c\xd3\x47"
		"\xde\x75\x0c\x80\x17\xae\x22\xb9"
		"\x50\xe7\x5b\xf2\x89\x20\x94\x2b"
		"\xc2\x36\xcd\x64\xfb\x6f\x06\x9d"
		"\x11\xa8\x3f\xd6\x4a\xe1\x78\x0f"
		"\x83\x1a\xb1\x25\xbc\x53\xea\x5e"
		"\xf5\x8c\x00\x97\x2e\xc5\x39\xd0"
		"\x67\xfe\x72\x09\xa0\x14\xab\x42"
		"\xd9\x4d\xe4\x7b\x12\x86\x1d\xb4"
		"\x28\xbf\x56\xed\x61\xf8\x8f\x03"
		"\x9a\x31\xc8\x3c\xd3\x6a\x01\x75"
		"\x0c\xa3\x17\xae\x45\xdc\x50\xe7"
		"\x7e\x15\x89\x20\xb7\x2b\xc2\x59"
		"\xf0\x64\xfb\x92\x06\x9d\x34\xcb"
		"\x3f\xd6\x6d\x04\x78\x0f\xa6\x1a"
		"\xb1\x48\xdf\x53\xea\x81\x18\x8c"
		"\x23\xba\x2e\xc5\x5c\xf3\x67\xfe"
		"\x95\x09\xa0\x37\xce\x42\xd9\x70"
		"\x07\x7b\x12\xa9\x1d\xb4\x4b\xe2"
		"\x56\xed\x84\x1b\x8f\x26\xbd\x31"
		"\xc8\x5f\xf6\x6a\x01\x98\x0c\xa3"
		"\x3a\xd1\x45\xdc\x73\x0a\x7e\x15"
		"\xac\x20\xb7\x4e\xe5\x59\xf0\x87"
		"\x1e\x92\x29\xc0\x34\xcb\x62\xf9"
		"\x6d\x04\x9b\x0f\xa6\x3d\xd4\x48"
		"\xdf\x76\x0d\x81\x18\xaf\x23\xba"
		"\x51\xe8\x5c\xf3\x8a\x21\x95\x2c"
		"\xc3\x37\xce\x65\xfc\x70\x07\x9e"
		"\x12\xa9\x40\xd7\x4b\xe2\x79\x10"
		"\x84\x1b\xb2\x26\xbd\x54\xeb\x5f"
		"\xf6\x8d\x01\x98\x2f\xc6\x3a\xd1"
		"\x68\xff\x73\x0a\xa1\x15\xac\x43"
		"\xda\x4e\xe5\x7c\x13\x87\x1e\xb5"
		"\x29\xc0\x57\xee\x62\xf9\x90\x04"
		"\x9b\x32\xc9\x3d\xd4\x6b\x02\x76"
		"\x0d\xa4\x18\xaf\x46\xdd\x51\xe8"
		"\x7f\x16\x8a\x21\xb8\x2c\xc3\x5a"
		"\xf1\x65\xfc\x93\x07\x9e\x35\xcc"
		"\x40\xd7\x6e\x05\x79\x10\xa7\x1b"
		"\xb2\x49\xe0\x54\xeb\x82\x19\x8d"
		"\x24\xbb\x2f\xc6\x5d\xf4\x68\xff"
		"\x96\x0a\xa1\x38\xcf\x43\xda\x71"
		"\x08\x7c\x13\xaa\x1e\xb5\x4c",
		.psize     = 1023,
		.digest    = "\x76\xc9\xd4\x91\x7a\x5f\x0f\xaa"
		"\x13\x39\xf3\x01\x7a\xfa\xe5\x41"
		"\x5f\x0b\xf8\xeb\x32\xfc\xbf\xb0"
		"\xfa\x8c\xcd\x17\x83\xe2\xfa\xeb"
		"\x1c\x19\xde\xe2\x75\xdc\x34\x64"
		"\x5f\x35\x9c\x61\x2f\x10\xf9\xec"
		"\x59\xca\x9d\xcc\x25\x0c\x43\xba"
		"\x85\xa8\xf8\xfe\xb5\x24\xb2\xee",
	}
};

static const struct hash_testvec sha384_tv_template[] = {
	{
		.plaintext = "",
		.psize	= 0,
		.digest	= "\x38\xb0\x60\xa7\x51\xac\x96\x38"
		"\x4c\xd9\x32\x7e\xb1\xb1\xe3\x6a"
		"\x21\xfd\xb7\x11\x14\xbe\x07\x43"
		"\x4c\x0c\xc7\xbf\x63\xf6\xe1\xda"
		"\x27\x4e\xde\xbf\xe7\x6f\x65\xfb"
		"\xd5\x1a\xd2\xf1\x48\x98\xb9\x5b",
	}, {
		.plaintext = "abc",
		.psize	= 3,
		.digest	= "\xcb\x00\x75\x3f\x45\xa3\x5e\x8b"
		"\xb5\xa0\x3d\x69\x9a\xc6\x50\x07"
		"\x27\x2c\x32\xab\x0e\xde\xd1\x63"
		"\x1a\x8b\x60\x5a\x43\xff\x5b\xed"
		"\x80\x86\x07\x2b\xa1\xe7\xcc\x23"
		"\x58\xba\xec\xa1\x34\xc8\x25\xa7",
	}, {
		.plaintext = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		.psize	= 56,
		.digest	= "\x33\x91\xfd\xdd\xfc\x8d\xc7\x39"
		"\x37\x07\xa6\x5b\x1b\x47\x09\x39"
		"\x7c\xf8\xb1\xd1\x62\xaf\x05\xab"
		"\xfe\x8f\x45\x0d\xe5\xf3\x6b\xc6"
		"\xb0\x45\x5a\x85\x20\xbc\x4e\x6f"
		"\x5f\xe9\x5b\x1f\xe3\xc8\x45\x2b",
	}, {
		.plaintext = "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmn"
		"hijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu",
		.psize	= 112,
		.digest	= "\x09\x33\x0c\x33\xf7\x11\x47\xe8"
		"\x3d\x19\x2f\xc7\x82\xcd\x1b\x47"
		"\x53\x11\x1b\x17\x3b\x3b\x05\xd2"
		"\x2f\xa0\x80\x86\xe3\xb0\xf7\x12"
		"\xfc\xc7\xc7\x1a\x55\x7e\x2d\xb9"
		"\x66\xc3\xe9\xfa\x91\x74\x60\x39",
	}, {
		.plaintext = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcd"
		"efghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz",
		.psize	= 104,
		.digest	= "\x3d\x20\x89\x73\xab\x35\x08\xdb"
		"\xbd\x7e\x2c\x28\x62\xba\x29\x0a"
		"\xd3\x01\x0e\x49\x78\xc1\x98\xdc"
		"\x4d\x8f\xd0\x14\xe5\x82\x82\x3a"
		"\x89\xe1\x6f\x9b\x2a\x7b\xbc\x1a"
		"\xc9\x38\xe2\xd1\x99\xe8\xbe\xa4",
	}, {
		.plaintext = "\x08\x9f\x13\xaa\x41\xd8\x4c\xe3"
		"\x7a\x11\x85\x1c\xb3\x27\xbe\x55"
		"\xec\x60\xf7\x8e\x02\x99\x30\xc7"
		"\x3b\xd2\x69\x00\x74\x0b\xa2\x16"
		"\xad\x44\xdb\x4f\xe6\x7d\x14\x88"
		"\x1f\xb6\x2a\xc1\x58\xef\x63\xfa"
		"\x91\x05\x9c\x33\xca\x3e\xd5\x6c"
		"\x03\x77\x0e\xa5\x19\xb0\x47\xde"
		"\x52\xe9\x80\x17\x8b\x22\xb9\x2d"
		"\xc4\x5b\xf2\x66\xfd\x94\x08\x9f"
		"\x36\xcd\x41\xd8\x6f\x06\x7a\x11"
		"\xa8\x1c\xb3\x4a\xe1\x55\xec\x83"
		"\x1a\x8e\x25\xbc\x30\xc7\x5e\xf5"
		"\x69\x00\x97\x0b\xa2\x39\xd0\x44"
		"\xdb\x72\x09\x7d\x14\xab\x1f\xb6"
		"\x4d\xe4\x58\xef\x86\x1d\x91\x28"
		"\xbf\x33\xca\x61\xf8\x6c\x03\x9a"
		"\x0e\xa5\x3c\xd3\x47\xde\x75\x0c"
		"\x80\x17\xae\x22\xb9\x50\xe7\x5b"
		"\xf2\x89\x20\x94\x2b\xc2\x36\xcd"
		"\x64\xfb\x6f\x06\x9d\x11\xa8\x3f"
		"\xd6\x4a\xe1\x78\x0f\x83\x1a\xb1"
		"\x25\xbc\x53\xea\x5e\xf5\x8c\x00"
		"\x97\x2e\xc5\x39\xd0\x67\xfe\x72"
		"\x09\xa0\x14\xab\x42\xd9\x4d\xe4"
		"\x7b\x12\x86\x1d\xb4\x28\xbf\x56"
		"\xed\x61\xf8\x8f\x03\x9a\x31\xc8"
		"\x3c\xd3\x6a\x01\x75\x0c\xa3\x17"
		"\xae\x45\xdc\x50\xe7\x7e\x15\x89"
		"\x20\xb7\x2b\xc2\x59\xf0\x64\xfb"
		"\x92\x06\x9d\x34\xcb\x3f\xd6\x6d"
		"\x04\x78\x0f\xa6\x1a\xb1\x48\xdf"
		"\x53\xea\x81\x18\x8c\x23\xba\x2e"
		"\xc5\x5c\xf3\x67\xfe\x95\x09\xa0"
		"\x37\xce\x42\xd9\x70\x07\x7b\x12"
		"\xa9\x1d\xb4\x4b\xe2\x56\xed\x84"
		"\x1b\x8f\x26\xbd\x31\xc8\x5f\xf6"
		"\x6a\x01\x98\x0c\xa3\x3a\xd1\x45"
		"\xdc\x73\x0a\x7e\x15\xac\x20\xb7"
		"\x4e\xe5\x59\xf0\x87\x1e\x92\x29"
		"\xc0\x34\xcb\x62\xf9\x6d\x04\x9b"
		"\x0f\xa6\x3d\xd4\x48\xdf\x76\x0d"
		"\x81\x18\xaf\x23\xba\x51\xe8\x5c"
		"\xf3\x8a\x21\x95\x2c\xc3\x37\xce"
		"\x65\xfc\x70\x07\x9e\x12\xa9\x40"
		"\xd7\x4b\xe2\x79\x10\x84\x1b\xb2"
		"\x26\xbd\x54\xeb\x5f\xf6\x8d\x01"
		"\x98\x2f\xc6\x3a\xd1\x68\xff\x73"
		"\x0a\xa1\x15\xac\x43\xda\x4e\xe5"
		"\x7c\x13\x87\x1e\xb5\x29\xc0\x57"
		"\xee\x62\xf9\x90\x04\x9b\x32\xc9"
		"\x3d\xd4\x6b\x02\x76\x0d\xa4\x18"
		"\xaf\x46\xdd\x51\xe8\x7f\x16\x8a"
		"\x21\xb8\x2c\xc3\x5a\xf1\x65\xfc"
		"\x93\x07\x9e\x35\xcc\x40\xd7\x6e"
		"\x05\x79\x10\xa7\x1b\xb2\x49\xe0"
		"\x54\xeb\x82\x19\x8d\x24\xbb\x2f"
		"\xc6\x5d\xf4\x68\xff\x96\x0a\xa1"
		"\x38\xcf\x43\xda\x71\x08\x7c\x13"
		"\xaa\x1e\xb5\x4c\xe3\x57\xee\x85"
		"\x1c\x90\x27\xbe\x32\xc9\x60\xf7"
		"\x6b\x02\x99\x0d\xa4\x3b\xd2\x46"
		"\xdd\x74\x0b\x7f\x16\xad\x21\xb8"
		"\x4f\xe6\x5a\xf1\x88\x1f\x93\x2a"
		"\xc1\x35\xcc\x63\xfa\x6e\x05\x9c"
		"\x10\xa7\x3e\xd5\x49\xe0\x77\x0e"
		"\x82\x19\xb0\x24\xbb\x52\xe9\x5d"
		"\xf4\x8b\x22\x96\x2d\xc4\x38\xcf"
		"\x66\xfd\x71\x08\x9f\x13\xaa\x41"
		"\xd8\x4c\xe3\x7a\x11\x85\x1c\xb3"
		"\x27\xbe\x55\xec\x60\xf7\x8e\x02"
		"\x99\x30\xc7\x3b\xd2\x69\x00\x74"
		"\x0b\xa2\x16\xad\x44\xdb\x4f\xe6"
		"\x7d\x14\x88\x1f\xb6\x2a\xc1\x58"
		"\xef\x63\xfa\x91\x05\x9c\x33\xca"
		"\x3e\xd5\x6c\x03\x77\x0e\xa5\x19"
		"\xb0\x47\xde\x52\xe9\x80\x17\x8b"
		"\x22\xb9\x2d\xc4\x5b\xf2\x66\xfd"
		"\x94\x08\x9f\x36\xcd\x41\xd8\x6f"
		"\x06\x7a\x11\xa8\x1c\xb3\x4a\xe1"
		"\x55\xec\x83\x1a\x8e\x25\xbc\x30"
		"\xc7\x5e\xf5\x69\x00\x97\x0b\xa2"
		"\x39\xd0\x44\xdb\x72\x09\x7d\x14"
		"\xab\x1f\xb6\x4d\xe4\x58\xef\x86"
		"\x1d\x91\x28\xbf\x33\xca\x61\xf8"
		"\x6c\x03\x9a\x0e\xa5\x3c\xd3\x47"
		"\xde\x75\x0c\x80\x17\xae\x22\xb9"
		"\x50\xe7\x5b\xf2\x89\x20\x94\x2b"
		"\xc2\x36\xcd\x64\xfb\x6f\x06\x9d"
		"\x11\xa8\x3f\xd6\x4a\xe1\x78\x0f"
		"\x83\x1a\xb1\x25\xbc\x53\xea\x5e"
		"\xf5\x8c\x00\x97\x2e\xc5\x39\xd0"
		"\x67\xfe\x72\x09\xa0\x14\xab\x42"
		"\xd9\x4d\xe4\x7b\x12\x86\x1d\xb4"
		"\x28\xbf\x56\xed\x61\xf8\x8f\x03"
		"\x9a\x31\xc8\x3c\xd3\x6a\x01\x75"
		"\x0c\xa3\x17\xae\x45\xdc\x50\xe7"
		"\x7e\x15\x89\x20\xb7\x2b\xc2\x59"
		"\xf0\x64\xfb\x92\x06\x9d\x34\xcb"
		"\x3f\xd6\x6d\x04\x78\x0f\xa6\x1a"
		"\xb1\x48\xdf\x53\xea\x81\x18\x8c"
		"\x23\xba\x2e\xc5\x5c\xf3\x67\xfe"
		"\x95\x09\xa0\x37\xce\x42\xd9\x70"
		"\x07\x7b\x12\xa9\x1d\xb4\x4b\xe2"
		"\x56\xed\x84\x1b\x8f\x26\xbd\x31"
		"\xc8\x5f\xf6\x6a\x01\x98\x0c\xa3"
		"\x3a\xd1\x45\xdc\x73\x0a\x7e\x15"
		"\xac\x20\xb7\x4e\xe5\x59\xf0\x87"
		"\x1e\x92\x29\xc0\x34\xcb\x62\xf9"
		"\x6d\x04\x9b\x0f\xa6\x3d\xd4\x48"
		"\xdf\x76\x0d\x81\x18\xaf\x23\xba"
		"\x51\xe8\x5c\xf3\x8a\x21\x95\x2c"
		"\xc3\x37\xce\x65\xfc\x70\x07\x9e"
		"\x12\xa9\x40\xd7\x4b\xe2\x79\x10"
		"\x84\x1b\xb2\x26\xbd\x54\xeb\x5f"
		"\xf6\x8d\x01\x98\x2f\xc6\x3a\xd1"
		"\x68\xff\x73\x0a\xa1\x15\xac\x43"
		"\xda\x4e\xe5\x7c\x13\x87\x1e\xb5"
		"\x29\xc0\x57\xee\x62\xf9\x90\x04"
		"\x9b\x32\xc9\x3d\xd4\x6b\x02\x76"
		"\x0d\xa4\x18\xaf\x46\xdd\x51\xe8"
		"\x7f\x16\x8a\x21\xb8\x2c\xc3\x5a"
		"\xf1\x65\xfc\x93\x07\x9e\x35\xcc"
		"\x40\xd7\x6e\x05\x79\x10\xa7\x1b"
		"\xb2\x49\xe0\x54\xeb\x82\x19\x8d"
		"\x24\xbb\x2f\xc6\x5d\xf4\x68\xff"
		"\x96\x0a\xa1\x38\xcf\x43\xda\x71"
		"\x08\x7c\x13\xaa\x1e\xb5\x4c",
		.psize     = 1023,
		.digest    = "\x4d\x97\x23\xc8\xea\x7a\x7c\x15"
		"\xb8\xff\x97\x9c\xf5\x13\x4f\x31"
		"\xde\x67\xf7\x24\x73\xcd\x70\x1c"
		"\x03\x4a\xba\x8a\x87\x49\xfe\xdc"
		"\x75\x29\x62\x83\xae\x3f\x17\xab"
		"\xfd\x10\x4d\x8e\x17\x1c\x1f\xca",
	}
};

static const struct hash_testvec sha256_tv_template[] = {
	{
		.plaintext = "",
		.psize	= 0,
		.digest	= "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14"
		"\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24"
		"\x27\xae\x41\xe4\x64\x9b\x93\x4c"
		"\xa4\x95\x99\x1b\x78\x52\xb8\x55",
	}, {
		.plaintext = "abc",
		.psize	= 3,
		.digest	= "\xba\x78\x16\xbf\x8f\x01\xcf\xea"
		"\x41\x41\x40\xde\x5d\xae\x22\x23"
		"\xb0\x03\x61\xa3\x96\x17\x7a\x9c"
		"\xb4\x10\xff\x61\xf2\x00\x15\xad",
	}, {
		.plaintext = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
		.psize	= 56,
		.digest	= "\x24\x8d\x6a\x61\xd2\x06\x38\xb8"
		"\xe5\xc0\x26\x93\x0c\x3e\x60\x39"
		"\xa3\x3c\xe4\x59\x64\xff\x21\x67"
		"\xf6\xec\xed\xd4\x19\xdb\x06\xc1",
	}, {
		.plaintext = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-",
		.psize	= 64,
		.digest = "\xb5\xfe\xad\x56\x7d\xff\xcb\xa4"
		"\x2c\x32\x29\x32\x19\xbb\xfb\xfa"
		"\xd6\xff\x94\xa3\x72\x91\x85\x66"
		"\x3b\xa7\x87\x77\x58\xa3\x40\x3a",
	}, {
		.plaintext = "\x08\x9f\x13\xaa\x41\xd8\x4c\xe3"
		"\x7a\x11\x85\x1c\xb3\x27\xbe\x55"
		"\xec\x60\xf7\x8e\x02\x99\x30\xc7"
		"\x3b\xd2\x69\x00\x74\x0b\xa2\x16"
		"\xad\x44\xdb\x4f\xe6\x7d\x14\x88"
		"\x1f\xb6\x2a\xc1\x58\xef\x63\xfa"
		"\x91\x05\x9c\x33\xca\x3e\xd5\x6c"
		"\x03\x77\x0e\xa5\x19\xb0\x47\xde"
		"\x52\xe9\x80\x17\x8b\x22\xb9\x2d"
		"\xc4\x5b\xf2\x66\xfd\x94\x08\x9f"
		"\x36\xcd\x41\xd8\x6f\x06\x7a\x11"
		"\xa8\x1c\xb3\x4a\xe1\x55\xec\x83"
		"\x1a\x8e\x25\xbc\x30\xc7\x5e\xf5"
		"\x69\x00\x97\x0b\xa2\x39\xd0\x44"
		"\xdb\x72\x09\x7d\x14\xab\x1f\xb6"
		"\x4d\xe4\x58\xef\x86\x1d\x91\x28"
		"\xbf\x33\xca\x61\xf8\x6c\x03\x9a"
		"\x0e\xa5\x3c\xd3\x47\xde\x75\x0c"
		"\x80\x17\xae\x22\xb9\x50\xe7\x5b"
		"\xf2\x89\x20\x94\x2b\xc2\x36\xcd"
		"\x64\xfb\x6f\x06\x9d\x11\xa8\x3f"
		"\xd6\x4a\xe1\x78\x0f\x83\x1a\xb1"
		"\x25\xbc\x53\xea\x5e\xf5\x8c\x00"
		"\x97\x2e\xc5\x39\xd0\x67\xfe\x72"
		"\x09\xa0\x14\xab\x42\xd9\x4d\xe4"
		"\x7b\x12\x86\x1d\xb4\x28\xbf\x56"
		"\xed\x61\xf8\x8f\x03\x9a\x31\xc8"
		"\x3c\xd3\x6a\x01\x75\x0c\xa3\x17"
		"\xae\x45\xdc\x50\xe7\x7e\x15\x89"
		"\x20\xb7\x2b\xc2\x59\xf0\x64\xfb"
		"\x92\x06\x9d\x34\xcb\x3f\xd6\x6d"
		"\x04\x78\x0f\xa6\x1a\xb1\x48\xdf"
		"\x53\xea\x81\x18\x8c\x23\xba\x2e"
		"\xc5\x5c\xf3\x67\xfe\x95\x09\xa0"
		"\x37\xce\x42\xd9\x70\x07\x7b\x12"
		"\xa9\x1d\xb4\x4b\xe2\x56\xed\x84"
		"\x1b\x8f\x26\xbd\x31\xc8\x5f\xf6"
		"\x6a\x01\x98\x0c\xa3\x3a\xd1\x45"
		"\xdc\x73\x0a\x7e\x15\xac\x20\xb7"
		"\x4e\xe5\x59\xf0\x87\x1e\x92\x29"
		"\xc0\x34\xcb\x62\xf9\x6d\x04\x9b"
		"\x0f\xa6\x3d\xd4\x48\xdf\x76\x0d"
		"\x81\x18\xaf\x23\xba\x51\xe8\x5c"
		"\xf3\x8a\x21\x95\x2c\xc3\x37\xce"
		"\x65\xfc\x70\x07\x9e\x12\xa9\x40"
		"\xd7\x4b\xe2\x79\x10\x84\x1b\xb2"
		"\x26\xbd\x54\xeb\x5f\xf6\x8d\x01"
		"\x98\x2f\xc6\x3a\xd1\x68\xff\x73"
		"\x0a\xa1\x15\xac\x43\xda\x4e\xe5"
		"\x7c\x13\x87\x1e\xb5\x29\xc0\x57"
		"\xee\x62\xf9\x90\x04\x9b\x32\xc9"
		"\x3d\xd4\x6b\x02\x76\x0d\xa4\x18"
		"\xaf\x46\xdd\x51\xe8\x7f\x16\x8a"
		"\x21\xb8\x2c\xc3\x5a\xf1\x65\xfc"
		"\x93\x07\x9e\x35\xcc\x40\xd7\x6e"
		"\x05\x79\x10\xa7\x1b\xb2\x49\xe0"
		"\x54\xeb\x82\x19\x8d\x24\xbb\x2f"
		"\xc6\x5d\xf4\x68\xff\x96\x0a\xa1"
		"\x38\xcf\x43\xda\x71\x08\x7c\x13"
		"\xaa\x1e\xb5\x4c\xe3\x57\xee\x85"
		"\x1c\x90\x27\xbe\x32\xc9\x60\xf7"
		"\x6b\x02\x99\x0d\xa4\x3b\xd2\x46"
		"\xdd\x74\x0b\x7f\x16\xad\x21\xb8"
		"\x4f\xe6\x5a\xf1\x88\x1f\x93\x2a"
		"\xc1\x35\xcc\x63\xfa\x6e\x05\x9c"
		"\x10\xa7\x3e\xd5\x49\xe0\x77\x0e"
		"\x82\x19\xb0\x24\xbb\x52\xe9\x5d"
		"\xf4\x8b\x22\x96\x2d\xc4\x38\xcf"
		"\x66\xfd\x71\x08\x9f\x13\xaa\x41"
		"\xd8\x4c\xe3\x7a\x11\x85\x1c\xb3"
		"\x27\xbe\x55\xec\x60\xf7\x8e\x02"
		"\x99\x30\xc7\x3b\xd2\x69\x00\x74"
		"\x0b\xa2\x16\xad\x44\xdb\x4f\xe6"
		"\x7d\x14\x88\x1f\xb6\x2a\xc1\x58"
		"\xef\x63\xfa\x91\x05\x9c\x33\xca"
		"\x3e\xd5\x6c\x03\x77\x0e\xa5\x19"
		"\xb0\x47\xde\x52\xe9\x80\x17\x8b"
		"\x22\xb9\x2d\xc4\x5b\xf2\x66\xfd"
		"\x94\x08\x9f\x36\xcd\x41\xd8\x6f"
		"\x06\x7a\x11\xa8\x1c\xb3\x4a\xe1"
		"\x55\xec\x83\x1a\x8e\x25\xbc\x30"
		"\xc7\x5e\xf5\x69\x00\x97\x0b\xa2"
		"\x39\xd0\x44\xdb\x72\x09\x7d\x14"
		"\xab\x1f\xb6\x4d\xe4\x58\xef\x86"
		"\x1d\x91\x28\xbf\x33\xca\x61\xf8"
		"\x6c\x03\x9a\x0e\xa5\x3c\xd3\x47"
		"\xde\x75\x0c\x80\x17\xae\x22\xb9"
		"\x50\xe7\x5b\xf2\x89\x20\x94\x2b"
		"\xc2\x36\xcd\x64\xfb\x6f\x06\x9d"
		"\x11\xa8\x3f\xd6\x4a\xe1\x78\x0f"
		"\x83\x1a\xb1\x25\xbc\x53\xea\x5e"
		"\xf5\x8c\x00\x97\x2e\xc5\x39\xd0"
		"\x67\xfe\x72\x09\xa0\x14\xab\x42"
		"\xd9\x4d\xe4\x7b\x12\x86\x1d\xb4"
		"\x28\xbf\x56\xed\x61\xf8\x8f\x03"
		"\x9a\x31\xc8\x3c\xd3\x6a\x01\x75"
		"\x0c\xa3\x17\xae\x45\xdc\x50\xe7"
		"\x7e\x15\x89\x20\xb7\x2b\xc2\x59"
		"\xf0\x64\xfb\x92\x06\x9d\x34\xcb"
		"\x3f\xd6\x6d\x04\x78\x0f\xa6\x1a"
		"\xb1\x48\xdf\x53\xea\x81\x18\x8c"
		"\x23\xba\x2e\xc5\x5c\xf3\x67\xfe"
		"\x95\x09\xa0\x37\xce\x42\xd9\x70"
		"\x07\x7b\x12\xa9\x1d\xb4\x4b\xe2"
		"\x56\xed\x84\x1b\x8f\x26\xbd\x31"
		"\xc8\x5f\xf6\x6a\x01\x98\x0c\xa3"
		"\x3a\xd1\x45\xdc\x73\x0a\x7e\x15"
		"\xac\x20\xb7\x4e\xe5\x59\xf0\x87"
		"\x1e\x92\x29\xc0\x34\xcb\x62\xf9"
		"\x6d\x04\x9b\x0f\xa6\x3d\xd4\x48"
		"\xdf\x76\x0d\x81\x18\xaf\x23\xba"
		"\x51\xe8\x5c\xf3\x8a\x21\x95\x2c"
		"\xc3\x37\xce\x65\xfc\x70\x07\x9e"
		"\x12\xa9\x40\xd7\x4b\xe2\x79\x10"
		"\x84\x1b\xb2\x26\xbd\x54\xeb\x5f"
		"\xf6\x8d\x01\x98\x2f\xc6\x3a\xd1"
		"\x68\xff\x73\x0a\xa1\x15\xac\x43"
		"\xda\x4e\xe5\x7c\x13\x87\x1e\xb5"
		"\x29\xc0\x57\xee\x62\xf9\x90\x04"
		"\x9b\x32\xc9\x3d\xd4\x6b\x02\x76"
		"\x0d\xa4\x18\xaf\x46\xdd\x51\xe8"
		"\x7f\x16\x8a\x21\xb8\x2c\xc3\x5a"
		"\xf1\x65\xfc\x93\x07\x9e\x35\xcc"
		"\x40\xd7\x6e\x05\x79\x10\xa7\x1b"
		"\xb2\x49\xe0\x54\xeb\x82\x19\x8d"
		"\x24\xbb\x2f\xc6\x5d\xf4\x68\xff"
		"\x96\x0a\xa1\x38\xcf\x43\xda\x71"
		"\x08\x7c\x13\xaa\x1e\xb5\x4c",
		.psize     = 1023,
		.digest    = "\xc5\xce\x0c\xca\x01\x4f\x53\x3a"
		"\x32\x32\x17\xcc\xd4\x6a\x71\xa9"
		"\xf3\xed\x50\x10\x64\x8e\x06\xbe"
		"\x9b\x4a\xa6\xbb\x05\x89\x59\x51",
	}
};

static int _sha_test(const struct shell *shell, const struct hash_testvec *tv, int tv_len,
		     enum hash_algo algo)
{
	const struct device *dev = device_get_binding(HASH_DRV_NAME);
	int ret;
	int i;
	uint8_t digest[64];

	struct hash_ctx ini;
	struct hash_pkt pkt;

	for (i = 0; i < tv_len; i++) {
		shell_fprintf(shell, SHELL_NORMAL, "tv[%d]:", i);

		pkt.in_buf = (uint8_t *)tv[i].plaintext;
		pkt.in_len = tv[i].psize;
		pkt.out_buf = digest;

		ret = hash_begin_session(dev, &ini, algo);
		if (ret) {
			shell_print(shell, "hash_begin_session error");
			return ret;
		}

		ret = hash_update(&ini, &pkt);
		if (ret) {
			shell_print(shell, "hash_update error");
			goto out;
		}

		/* final */
		ret = hash_compute(&ini, &pkt);
		if (ret) {
			shell_print(shell, "hash_compute error");
			goto out;
		}

		hash_free_session(dev, &ini);

		if (!memcmp(digest, tv[i].digest, ini.digest_size))
			shell_fprintf(shell, SHELL_NORMAL, "PASS\n");
		else
			shell_fprintf(shell, SHELL_NORMAL, "FAIL\n");
	}

	return 0;
out:
	hash_free_session(dev, &ini);
	return ret;
}

static int sha512_test(const struct shell *shell, size_t argc, char **argv)
{
	shell_print(shell, "sha512_test");
	return _sha_test(shell, sha512_tv_template, ARRAY_SIZE(sha512_tv_template),
			 CRYPTO_HASH_ALGO_SHA512);
}

static int sha384_test(const struct shell *shell, size_t argc, char **argv)
{
	shell_print(shell, "sha384_test");
	return _sha_test(shell, sha384_tv_template, ARRAY_SIZE(sha384_tv_template),
			 CRYPTO_HASH_ALGO_SHA384);
}

static int sha256_test(const struct shell *shell, size_t argc, char **argv)
{
	shell_print(shell, "sha256_test");
	return _sha_test(shell, sha256_tv_template, ARRAY_SIZE(sha256_tv_template),
			 CRYPTO_HASH_ALGO_SHA256);
}

static int sha_test(const struct shell *shell, size_t argc, char **argv)
{
	shell_print(shell, "sha256_test");
	_sha_test(shell, sha256_tv_template, ARRAY_SIZE(sha256_tv_template),
		  CRYPTO_HASH_ALGO_SHA256);

	shell_print(shell, "sha384_test");
	_sha_test(shell, sha384_tv_template, ARRAY_SIZE(sha384_tv_template),
		  CRYPTO_HASH_ALGO_SHA384);

	shell_print(shell, "sha512_test");
	_sha_test(shell, sha512_tv_template, ARRAY_SIZE(sha512_tv_template),
		  CRYPTO_HASH_ALGO_SHA512);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(hash_cmds,
			       SHELL_CMD_ARG(sha512, NULL, "", sha512_test, 1, 0),
			       SHELL_CMD_ARG(sha384, NULL, "", sha384_test, 1, 0),
			       SHELL_CMD_ARG(sha256, NULL, "", sha256_test, 1, 0),
			       SHELL_CMD_ARG(test, NULL, "", sha_test, 1, 0),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(hash, &hash_cmds, "Hash shell commands", NULL);
