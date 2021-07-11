FROM ubuntu:latest as base
RUN cp /etc/apt/sources.list /etc/apt/sources.list.bak && \
    echo "deb http://mirrors.163.com/ubuntu/ focal main restricted universe multiverse\n" > etc/apt/sources.list && \
    echo "deb http://mirrors.163.com/ubuntu/ focal-security main restricted universe multiverse\n" >> etc/apt/sources.list && \
    echo "deb http://mirrors.163.com/ubuntu/ focal-updates main restricted universe multiverse\n" >> etc/apt/sources.list && \
    echo "deb http://mirrors.163.com/ubuntu/ focal-backports main restricted universe multiverse\n" >> etc/apt/sources.list && \
    echo "deb-src http://mirrors.163.com/ubuntu/ focal main restricted universe multiverse\n" >> etc/apt/sources.list && \
    echo "deb-src http://mirrors.163.com/ubuntu/ focal-security main restricted universe multiverse\n" >> etc/apt/sources.list && \
    echo "deb-src http://mirrors.163.com/ubuntu/ focal-updates main restricted universe multiverse\n" >> etc/apt/sources.list && \
    echo "deb-src http://mirrors.163.com/ubuntu/ focal-backports main restricted universe multiverse\n" >> etc/apt/sources.list 
RUN echo "happy"
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt-get -y install sudo
RUN sudo apt-get -y install build-essential 
RUN sudo apt-get -y install gcc-multilib 
RUN sudo apt-get -y install curl

# 安装tzdata 会出现需要命令行交互的情况，
RUN ln -snf /usr/share/zoneinfo/$CONTAINER_TIMEZONE /etc/localtime && echo $CONTAINER_TIMEZONE > /etc/timezone
RUN sudo apt-get -y install tzdata
RUN sudo apt-get -y install cmake
RUN sudo apt-get -y install libopencv-dev 
RUN sudo apt-get -y install libeigen3-dev
RUN sudo apt-get -y install git
RUN sudo apt-get -y install libglu1-mesa-dev 
RUN sudo apt-get -y install freeglut3-dev 
RUN sudo apt-get -y install xorg-dev  

# 设置网络
RUN sudo apt-get -y install openssh-server
RUN sudo apt-get -y install net-tools
RUN echo "Port 22" >> /etc/ssh/sshd_config
RUN echo "PermitRootLogin yes" >> /etc/ssh/sshd_config 
RUN sudo echo "service ssh start" >> /root/.bashrc

RUN sudo apt-get -y install software-properties-common 
RUN sudo apt-get -y install apt-transport-https 
RUN sudo apt-get -y install wget

# （可选）安装vim
RUN sudo apt-get -y install vim

# 图片转换（ppm转jpg）需要用到PIL库
RUN sudo apt-get -y install python3

# 镜像内安装 vscode
RUN wget -q https://packages.microsoft.com/keys/microsoft.asc -O- | sudo apt-key add -
RUN sudo add-apt-repository "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main"
RUN sudo apt-get -y install code
RUN sudo echo "export DISPLAY=192.168.2.190:0.0" >> /root/.bashrc

RUN ssh-keygen -t rsa -C "ronweasleyding@163.com" -f "/root/.ssh/id_rsa" -N ""
RUN sudo echo "alias python=\"python3\"" >> /root/.bashrc
RUN alias python=\"python3\"
RUN sudo apt-get -y install python3-pip
RUN sudo python3 -m pip install pillow black
RUN sudo apt-get -y install gdb
RUN sudo apt-get install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libglfw3-dev  
RUN sudo apt-get install libglew-dev


# 然后 passwd设置密码