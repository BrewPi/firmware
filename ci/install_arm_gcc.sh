
sudo add-apt-repository -y ppa:team-gcc-arm-embedded/ppa &&
sudo apt-get -qq update &&
sudo apt-get -qq install  "gcc-arm-none-eabi" &&
arm-none-eabi-gcc --version ;
