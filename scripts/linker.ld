OUTPUT_ARCH(i8086)

main_address = 4096M - 4K;

MEMORY {
	ROM (rx) : org = 4096M - 512K, len = 512K
}

SECTIONS {
	ENTRY(entry)

	.begin : {
		*(begin)
	} >ROM
	.main main_address : {
		*(main)
	}
	.reset 4096M - 0x10 : {
		*(reset)
	}
	.igot 0 : {
		*(.igot.plt)
	}
}
