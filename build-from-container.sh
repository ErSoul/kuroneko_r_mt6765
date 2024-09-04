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

sh clang.sh
