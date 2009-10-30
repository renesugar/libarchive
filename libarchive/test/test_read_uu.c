/*-
 * Copyright (c) 2003-2007 Tim Kientzle
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "test.h"

static unsigned char archive[] = {
0x62,0x65,0x67,0x69,0x6e,0x20,0x36,0x34,
0x34,0x20,0x74,0x65,0x73,0x74,0x5f,0x72,
0x65,0x61,0x64,0x5f,0x75,0x75,0x2e,0x5a,
0x0a,0x4d,0x27,0x59,0x56,0x30,0x2b,0x40,
0x60,0x28,0x27,0x24,0x42,0x50,0x48,0x2c,
0x26,0x23,0x22,0x21,0x2c,0x4a,0x37,0x2c,
0x42,0x50,0x28,0x34,0x28,0x38,0x24,0x26,
0x5f,0x34,0x4a,0x60,0x24,0x22,0x60,0x2c,
0x30,0x38,0x24,0x46,0x25,0x34,0x4f,0x29,
0x41,0x51,0x28,0x5c,0x32,0x2f,0x28,0x23,
0x37,0x26,0x40,0x23,0x25,0x43,0x0a,0x4d,
0x21,0x40,0x54,0x38,0x2d,0x23,0x23,0x2e,
0x4c,0x60,0x24,0x22,0x51,0x4c,0x40,0x3a,
0x2d,0x46,0x28,0x60,0x60,0x25,0x27,0x23,
0x48,0x3c,0x56,0x3b,0x2e,0x27,0x2f,0x4a,
0x21,0x25,0x21,0x47,0x23,0x49,0x54,0x50,
0x3c,0x42,0x53,0x22,0x48,0x29,0x2e,0x46,
0x53,0x3c,0x5a,0x24,0x31,0x28,0x54,0x3e,
0x2f,0x31,0x41,0x32,0x0a,0x49,0x48,0x55,
0x22,0x30,0x25,0x39,0x3d,0x2a,0x47,0x34,
0x4a,0x55,0x4a,0x4d,0x36,0x4b,0x36,0x2b,
0x2d,0x4a,0x57,0x3c,0x4a,0x55,0x4a,0x5d,
0x3e,0x4f,0x38,0x2c,0x2e,0x2a,0x27,0x34,
0x4e,0x56,0x4b,0x2d,0x46,0x53,0x3a,0x2d,
0x2e,0x4a,0x37,0x3c,0x4e,0x56,0x4b,0x3d,
0x4e,0x57,0x39,0x50,0x24,0x60,0x0a,0x60,
0x0a,0x65,0x6e,0x64,0x0a
};

static unsigned char archive64[] = {
"begin-base64 644 test_read_uu.Z\n"
"H52QLgAIHEiwoMGDCBMqXMiwIUIYEG/UqAECAMQYEmFUvJhxI8SPIDXGgDFjBg0YNDDOsAECxsga\n"
"NmIAAFHDoc2bOHPqBFBnDp0wcizCoJOmzc6ERI0ePRhSo1CQFZdKnUq1qtWrWLNq3cq1q9evYMOK\n"
"HUu2rNmzaNOqXcu2rdu3ZwE=\n"
"====\n"
};

static void
test_read_uu_sub(unsigned char *uudata, size_t uusize)
{
	struct archive_entry *ae;
	struct archive *a;
	assert((a = archive_read_new()) != NULL);
	assertEqualIntA(a, ARCHIVE_OK,
	    archive_read_support_compression_all(a));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_support_format_all(a));
	assertEqualIntA(a, ARCHIVE_OK,
	    read_open_memory(a, uudata, uusize, 2));
	assertEqualIntA(a, ARCHIVE_OK, archive_read_next_header(a, &ae));
	failure("archive_compression_name(a)=\"%s\"",
	    archive_compression_name(a));
	assertEqualInt(archive_compression(a), ARCHIVE_COMPRESSION_COMPRESS);
	failure("archive_format_name(a)=\"%s\"", archive_format_name(a));
	assertEqualInt(archive_format(a), ARCHIVE_FORMAT_TAR_USTAR);
	assertEqualIntA(a, ARCHIVE_OK, archive_read_close(a));
#if ARCHIVE_VERSION_NUMBER < 2000000
	archive_read_finish(a);
#else
	assertEqualInt(ARCHIVE_OK, archive_read_finish(a));
#endif
}

DEFINE_TEST(test_read_uu)
{
	/* Read the traditional uuencoded data. */
	test_read_uu_sub(archive, sizeof(archive));
	/* Read the Base64 encoding data. */
	test_read_uu_sub(archive64, sizeof(archive64)-1);
}

