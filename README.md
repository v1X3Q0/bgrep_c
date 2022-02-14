# bgrep_c
Binary grep tool to search a file or folder for byte arrays.

# usage
```shell
mariomain@raichu-off:~/repos/coreboot/build$ bgrep_c
Usage: bgrep_c [-f] [-d] filename [bytes]+
	-f only first occurance
	-d out format in decimal
	bytes space separated array of bytes
```
`filename` can be the name of either a file or a folder.
# output
```shell
mariomain@raichu-off:~/repos/coreboot/build$ bgrep_c coreboot.rom 0x82 0x46 0x00 0xf0 0x53 0xf8 0x10 0xee
coreboot.rom: found occurance at 0xfc
mariomain@raichu-off:~/repos/coreboot/build$ bgrep_c . 0x82 0x46 0x00 0xf0 0x53 0xf8 0x10 0xee
./cbfs/fallback/bootblock.debug: found occurance at 0x17c
./cbfs/fallback/bootblock.raw.elf: found occurance at 0x17c
./cbfs/fallback/bootblock.bin: found occurance at 0xfc
./cbfs/fallback/bootblock.elf: found occurance at 0x17c
./cbfs/fallback/bootblock.raw.bin: found occurance at 0xfc
./coreboot.rom: found occurance at 0xfc
./coreboot.pre: found occurance at 0xfc
```