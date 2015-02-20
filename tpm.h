#ifndef _TPM_H
#define _TPM_H

/*

 Copyright (c) 2014 Konrad Jopek

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:
 1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

*/


#include <sys/cdefs.h>

#include <sys/param.h>

/* TPM Return codes */
#define TCG_PC_OK			0x00
#define TCG_PC_TPMERROR			0x01
#define	TCG_PC_LOGOVERFLOW		0x02
#define TCG_PC_UNSUPPORTED		0x03

/* TCG Magic - typical content of EBX register */
#define TCG_MAGIC			0x41504354

#define TPM_TAG_RQU_COMMAND		0x00C1
#define TPM_TAG_RQU_AUTH1_COMMAND	0x00C2 
#define TPM_TAG_RQU_AUTH2_COMMAND	0x00C3
#define TPM_TAG_RSP_COMMAND		0x00C4
#define TPM_TAG_RSP_AUTH1_COMMAND	0x00C5
#define TPM_TAG_RSP_AUTH2_COMMAND	0x00C6

#define TPM_ORD_Extend			0x00000014

#define	TPM_ALGO_SHA1			0x00000004

struct TCG_HashAll {
	uint16_t	ipb_length;
	uint16_t	_reserved;
	uint32_t	hash_data_ptr;
	uint32_t	hash_data_len;
	uint32_t	algorithm_id;
};

struct TCG_PassTroughtToTPM {
	uint16_t	ipb_length;
	uint16_t	_reserved1; /* XXX: should be 0 */
	uint16_t	opb_length;
	uint16_t	_reserved2; /* XXX: should be 0 */
	uint16_t	tpm_tag;
	uint32_t	input_size;
	uint32_t	cmd_ordinal;
	uint32_t	pcr;
	/* add-on */
	uint8_t		hash[20];

};

uint32_t tcg_status_check(void);
uint32_t tcg_hash_extend(uint32_t addr, uint32_t len, uint32_t pcr);

#endif /* !_TPM_H */
