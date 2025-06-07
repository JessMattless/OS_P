
# OS_P

OS_P is an operating system I began development on when I was 17 during my second year of college. It began as a passion project for my love of low-level programming and software internals though I quickly realised I had bitten off more than I could chew, and put the project aside until I had more knowledge on the subject.

After recently picking it up again I am more determined than ever to make something special with this project, and I am excited to learn and see how far I can get with it.
## Run Locally

I made this project using VSCode on windows, the way I run/test it is through running QEMU through the console on windows, but I run all make steps through a linux terminal on WSL.

Clone the project

```bash
  git clone https://github.com/JessMattless/OS_P.git
```

Go to the project directory

```bash
  cd OS_P
```

Install dependencies

- GCC
- NASM
- QEMU

once QEMU is installed, and the make file has completed, all you need to do is either edit the included .bat/.sh files to point to your QEMU install, or put your QEMU install in the working directory.

Then run either

```bash
  .\runqemu.bat
```

Or


```bash
  .\runqemu.sh
```

## Support

If you have any issues running the program, please let me know and I will help you out!

