# database_project

## Description
This project constructed a MongoDB database instance, according to the properties of the origianl data, and created an interface to interact with the database. The original data comes from NA62 group in CERN, which is not public. So the data used in this project will not be public either. The whole system is built up in Linux environment: RedHat 9.4, which is a virtual environment. This documentation will illustrate the detailed steps about how to build this project, from environment setting to how to use the database and interface.


## Environment Installation and Setting
Install a virtual machine software package, can be downloaded here [virtualbox](https://www.virtualbox.org/) 

Download the RedHat 9.4 installation file here [RedHat9.4](https://access.redhat.com/downloads/content/479/ver=/rhel---9/9.4/x86_64/product-software)

Create the virtual machine using the .iso file downloaded above. From now on, it will be called 'RHEL9.4'.

Network setup for RHEL9.4:

VirtualBox control panel -> Tools -> Networks -> Host-only Networks panel -> Create -> IPv4 Prefix: 192.168.56.1 -> DHCP Server tab -> verify the Server Address, Lower Address Bound and Upper Address Bound -> apply;

Select RHEL9.4 in the VirtualBox control panel  -> Network -> Adapter 1 -> Attached to NAT -> Adapter 2 -> Attached to Host-only Adapter -> Name: VirtualBox Host-Only Ethernet Adapter -> ok.


## Install Dependencies
### Install docker on red hat 9.4:
Register Your System: If you’re using Red Hat Enterprise Linux (RHEL), you need to register your system with Red Hat Subscription Management:

`sudo subscription-manager register`

`sudo subscription-manager refresh`

Enable Repositories: After registering, enable the repositories:

`sudo subscription-manager repos --enable=rhel-7-server-rpms`

Install docker using the convenient script: ( from docker documentation)

`curl -fsSL https://get.docker.com -o get-docker.sh`

`sudo sh get-docker.sh`

Install docker deamon:

`sudo yum install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin`

add user to docker group: 

`sudo usermod -aG docker user1`

apply the changes :

`newgrp docker`

When start again after last shutdown from the red hat, the docker is disabled automatically. Need to use this command line to start docker again:

`sudo systemctl start docker`
### install g++ and cmake
Check the g++ version: 

`g++ --version`

Install g++ :    

`sudo yum install gcc-c++`      (11.4.1 installed)

Check the cmake version: 

`cmake --version`

Install cmake: 

`sudo yum install cmake`    (3.26.5 installed)

Install denpendancy before install c++ driver:

`sudo yum groupinstall "Development Tools"`
`sudo yum install wget tar bzip2`
`sudo yum install -y openssl-devel`

### Install boost:
download:

`wget https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.tar.bz2`

`tar --bzip2 -xf boost_1_80_0.tar.bz2`

`cd boost_1_80_0`

`./bootstrap.sh`

`sudo ./b2 install`

### Install MongoDB C++ Driver
Install C driver

`wget https://github.com/mongodb/mongo-c-driver/releases/download/1.27.4/mongo-c-driver-1.27.4.tar.gz`

`tar -xzf mongo-c-driver-1.27.4.tar.gz`

`cd mongo-c-driver-1.27.4`

`mkdir cmake-build`

`cd cmake-build`

`cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..`

`make`

`sudo make install`

Install C++ driver

`cd ~`

`git clone -b r3.10.1 https://github.com/mongodb/mongo-cxx-driver.git`

`cd mongo-cxx-driver`

`cd build`

`cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..`

`make`

`sudo make install`

### Install asio

`git clone https://github.com/chriskohlhoff/asio.git`

`cd asio/asio/include`

`sudo cp -r asio /usr/local/include/`

### Install crow

`git clone https://github.com/CrowCpp/Crow.git`

Run mkdir build inside of crow's source directory： 

`mkdir build`

Navigate to the new "build" directory: 
`cd build `

run the following: 
`cmake .. -DCROW_ENABLE_SSL=ON -DCMAKE_BUILD_TYPE=Debug -DASIO_INCLUDE_DIR=/home/user1/asio/asio/include`

`make`


## VS Code remote connect
Use VS Code to open the project folder on the remote machine.

VScode side bar -> extensions -> input 'Remote - SSH' -> install this extension -> Ctrl+Shift+P -> input 'Remote-SSH: Open SSH Configuration File...' -> create a ssh config file like this 

"Host <vm_name>

  HostName <vm_ip_address> 
  
  Port <port_number> # default 22
  
  User <username>"
  
Ctrl+Shift+P -> Remote-SSH: Connect to Host... -> select the host just added in the ssh config file -> connect to the vm.


## Usage
Use examples liberally, and show the expected output if you can. It's helpful to have inline the smallest example of usage that you can demonstrate, while providing links to more sophisticated examples if they are too long to reasonably include in the README.

## Support
If you have any question or ideas about this project, welcome to send an email: cv23649@bristol.ac.uk

## Contributing
State if you are open to contributions and what your requirements are for accepting them.

For people who want to make changes to your project, it's helpful to have some documentation on how to get started. Perhaps there is a script that they should run or some environment variables that they need to set. Make these steps explicit. These instructions could also be useful to your future self.

You can also document commands to lint the code or run tests. These steps help to ensure high code quality and reduce the likelihood that the changes inadvertently break something. Having instructions for running tests is especially helpful if it requires external setup, such as starting a Selenium server for testing in a browser.

## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
[VISITE LICENSE](https://github.com/yujuan11/database_project/blob/main/LICENSE)

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.
