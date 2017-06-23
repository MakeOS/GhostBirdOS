# Ghost Bird File System Reader #

A tool for writing data to virtual image inited by Ghost Bird File System.

## Command ##

Here are some command:

- -v:Show version information
- -ws:Write sector
- -rs:Read sector
- -sp:

### -ws ###
    gfr -ws src_file dst_file LBA

the location can be filled with value as follow:

- mbr:
- pt:
- LBA: sector number, start at 1