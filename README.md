# Description

Customized Linux Kernel for Redmi 9C to run containers.

# Usage

You should run the `build-from-container.sh`, and it will build setup the right toolchain to compile the kernel.

Optionally you can apply the `show_kernel_flags.patch` to be sure that the configs are detected correctly on runtime. NOTE: this will pop-up a warning on boot, about something wrong with your phone.

# Status

Docker: currently the phone will freeze and reboot in a while, after completely pulling a docker image.

LXC: phone freezes after trying to start a container.

# References

https://gist.github.com/FreddieOliveira/efe850df7ff3951cb62d74bd770dce27#docker-on-android-
https://github.com/Morakhiyasaiyam/Docker-native-on-Termux-on-Android?tab=readme-ov-file#docker-native-on-termux-on-android
https://github.com/George-Seven/Termux-LXC-Guide?tab=readme-ov-file#description
