#!/bin/sh

if ! dpkg -s make gcc bison flex python3 libssl-dev bc libxml2 xz-utils zstd unzip curl wget >/dev/null 2>&1
then
	apt update && apt install -y make gcc bison flex python3 libssl-dev bc libxml2 xz-utils zstd unzip curl wget
fi

#! test -f /opt/kernel-android11.zip && wget -O /opt/kernel-android11.zip https://github.com/danya2271/kuroneko_r_mt6765/archive/refs/heads/rebase.zip
! test -f /opt/kernel-android11.zip && wget -O /opt/kernel-android11.zip https://github.com/danya2271/kuroneko_r_mt6765/archive/refs/heads/upstream.zip
! test -d /opt/kuroneko_r_mt6765-upstream && mkdir -p /opt/kuroneko_r_mt6765-upstream && unzip -d /opt /opt/kernel-android11.zip

cd /opt/kuroneko_r_mt6765-upstream
bash /opt/kuroneko_r_mt6765-upstream/prepare_compiler.sh

cat << EOF > clang.sh
#!/bin/bash

make CC=$(pwd)/clang/bin/clang \
LD=$(pwd)/clang/bin/ld.lld \
NM=$(pwd)/clang/bin/llvm-nm \
AR=$(pwd)/clang/bin/llvm-ar \
OBJCOPY=$(pwd)/clang/bin/llvm-objcopy \
OBJDUMP=$(pwd)/clang/bin/llvm-objdump \
STRIP=$(pwd)/clang/bin/llvm-strip \
CROSS_COMPILE_ARM32=$(pwd)/gcc-linaro-13.0.0-2022.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- \
CROSS_COMPILE=$(pwd)/gcc-linaro-13.0.0-2022.10-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- \
O=out ARCH=arm64 blossom_defconfig

make CC=$(pwd)/clang/bin/clang \
LD=$(pwd)/clang/bin/ld.lld \
NM=$(pwd)/clang/bin/llvm-nm \
AR=$(pwd)/clang/bin/llvm-ar \
OBJCOPY=$(pwd)/clang/bin/llvm-objcopy \
OBJDUMP=$(pwd)/clang/bin/llvm-objdump \
STRIP=$(pwd)/clang/bin/llvm-strip \
CROSS_COMPILE_ARM32=$(pwd)/gcc-linaro-13.0.0-2022.10-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf- \
CROSS_COMPILE=$(pwd)/gcc-linaro-13.0.0-2022.10-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- \
O=out ARCH=arm64 
EOF

sh clang.sh
