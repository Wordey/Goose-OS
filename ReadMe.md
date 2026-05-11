# GooseOS
Hello everyone this is my GooseOS project. It's support UEFI and BIOS(legacy). Tested on Lenovo b570(2011 year).
![icon](docs/icon.png)

# What about compiling?
if you need iso image of GooseOS than you can only can compile because pre-compiled os images doesn't exist.

for Ubuntu
```bash
    sudo apt install clang git
    git clone https://github.com/Wordey/GooseOS.git
    cd GooseOS/tools
    make
    ./make_disk
```
for Arch
```bash
    sudo pacman -S clang git
    git clone https://github.com/Wordey/GooseOS.git
    cd GooseOS/tools
    make
    ./make_disk`
```

# So, what's the GooseOS
GooseOS is OS support UEFI and BIOS(legacy).

For now i do only tool make_disk but i go to code boot.c. For legacy BIOS i do not very big loader now it "Hello, World!" loader for legacy BIOS...

Now if you run the xxd on image created by make_disk you see the 0s and the basic protective MBR values.