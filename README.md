# FlexUEFIToolkit Example
## Kernel Module

```
make
sudo insmod FlexUEFIToolkitK.ko
sudo rmmod FlexUEFIToolkitK
```


Example User Application
```
g++ FlexUEFIToolkit.c -o FlexUEFIToolkit
./FlexUEFIToolkit
```

## Convert uppercase letters to lowercase

```bash
gcc convert_letter.c -o convert_letter
./convert_letter
```

## Read Flash

```bash
gcc read_flash.c -o read_flash
./read_flash
```

Validation:

```bash
hexdump OVMF.fd | less
```



## Write Flash

```bash
gcc write_flash.c -o write_flash
./write_flash.c
```

Please place firmware as ·`OVMF.old.fd` . Cost 4min in WSL or 1min in Linux.

## Change Boot Order

```bash
gcc change_boot_order.c -o change_boot_order
./change_boot_order
```

Validation:

```bash
efibootmgr
```

