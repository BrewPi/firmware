
sudo add-apt-repository -y ppa:team-gcc-arm-embedded/ppa &&
sudo apt-get -qq update &&
sudo apt-get -qq install  "gcc-arm-embedded" &&
arm-none-eabi-gcc --version ;
