```
# install packages for build
sudo apt install build-essential mtools

# install qemu
sudo apt install qemu-system-x86

# install rust
curl https://sh.rustup.rs -sSf | sh
rustup install nightly-2024-04-14
rustup default nightly-2024-04-14-x86_64-unknown-linux-gnu

rustup component add rust-src --toolchain nightly-2024-04-14-x86_64-unknown-linux-gnu
```
