/*

 TPM support for gpt(zfs)boot / loader.

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

#include <machine/endian.h>
#include <sys/param.h>

#include "tpm.h"
#include <btxv86.h>

static uint8_t tpm_enabled = 0xff;

uint32_t 
tcg_status_check(void)
{
	if (tpm_enabled == 0xff) {
		v86.addr = 0x1a;
		v86.eax = 0xbb00;

		v86int();

		tpm_enabled = v86.eax;
	}
	return (tpm_enabled);
}

uint32_t 
tcg_hash_extend(uint32_t addr, uint32_t len, uint32_t pcr) 
{
	struct TCG_HashAll tcg_hash_all;
	struct TCG_PassTroughtToTPM tcg_passtrough_to_tpm;
	char _unused[0x22];

	tcg_hash_all.ipb_length = 0x0010;
	tcg_hash_all._reserved = 0x0000;
	tcg_hash_all.hash_data_ptr = addr;
	tcg_hash_all.hash_data_len = len;
	tcg_hash_all.algorithm_id = TPM_ALGO_SHA1;

	tcg_passtrough_to_tpm.ipb_length = 0x002a;
	tcg_passtrough_to_tpm._reserved1 = 0x0000;
	tcg_passtrough_to_tpm.opb_length = 0x0022;
	tcg_passtrough_to_tpm._reserved2 = 0x0000;
	tcg_passtrough_to_tpm.tpm_tag = __htons(TPM_TAG_RQU_COMMAND);
	tcg_passtrough_to_tpm.input_size = __htonl(0x00000022);
	tcg_passtrough_to_tpm.cmd_ordinal = __htonl(TPM_ORD_Extend);
	tcg_passtrough_to_tpm.pcr = __htonl(pcr);

	v86.addr = 0x1a;
	v86.eax = 0xbb05;
	v86.ebx = TCG_MAGIC;
	v86.ecx = 0x00000000;
	v86.edx = 0x00000000;
	v86.es = VTOPSEG(&tcg_hash_all);
	v86.edi = VTOPOFF(&tcg_hash_all);
	v86.ds = VTOPSEG(&tcg_passtrough_to_tpm.hash);
	v86.esi = VTOPOFF(&tcg_passtrough_to_tpm.hash);

	v86int();

	if (v86.eax != TCG_PC_OK)
		return (v86.eax);

	v86.addr = 0x1a;
	v86.eax = 0xbb02;
	v86.ebx = TCG_MAGIC;
	v86.ecx = 0x00000000;
	v86.edx = 0x00000000;
	v86.es = VTOPSEG(&tcg_passtrough_to_tpm);
	v86.edi = VTOPOFF(&tcg_passtrough_to_tpm);
	v86.ds = VTOPSEG(&_unused);
	v86.esi = VTOPOFF(&_unused);

	v86int();

	return (v86.eax);
}
