# *Os_Projects* 

These two mini project were our os course projects in the university.

Author : Ali Ghadiri, Afrooz Sheikholeslami

Language : C

# **Project 1, Add system call to xv6 kernel**

In this project we tried to add a system call named getreadcount() to xv6 kernal which counts the number of executions of read() system call since we boot the OS.

# **PreRequirements**

For This Project You Need below Requirements :
- C compiler installed on your device.
- Qemu emulator.
- xv6 kernel codes.

# **Usage**
## Step0 : Cloning

First of All Clone the Project : 

```shell
$ git clone https://github.com/AliGhadirii/Os_Projects.git
```

## Step1 : Install Qemu in your device

You can simply install qemu with following scripts in linux shell:
```shell
$ sudo apt-get install qemu-kvm
```
## Step2 : Install xv6 and run it on qemu
```shell
$ sudo apt-get install git
$ git clone https://github.com/mit-pdos/xv6-public.git
```
then 
```shell
$ cd xv6-public
$ make qemu-nox
```

# Project 2, green thread
In this project we tend to write thread library named green thread which acts like pthread library in c.

# **PreRequirements**

For This Project You Need below Requirements :
- C compiler installed on your device.

# **Usage**
## Step0 : Cloning

First of All Clone the Project : 

```shell
$ git clone https://github.com/AliGhadirii/Os_Projects.git
```

## Step1 : make files

You can simply make files with following  command
```shell
$ make
```
## Step2 : Now we have green_test and posix_test, so we can run both files to compare the result
```shell
$ ./green_test
$ ./posix_test
```

# **Support**

Reach out to me at one of the following places!

- Telegram at <a href="https://t.me/Aliighadiri" target="_blank">@Aliighadiri</a>
- Gmail at <a href="mailto:ali.ghadiri78@gmail.com" target="_blank">ali.ghadiri78@gmail.com</a>



