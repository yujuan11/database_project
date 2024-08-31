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
### docker run a mongo instance
pull a mongo image

`docker pull mongo:4.4`

when run the instance, create a root user using environment varibles and enable the authentication at the same time.

`docker run -d --name mongodb -p 27017:27017 -v /my/own/datadir:/data/db -e MONGO_INITDB_ROOT_USERNAME=admin -e MONGO_INITDB_ROOT_PASSWORD=admin_123 mongo:4.4 --auth`


### Input data

`cd scr`

`python3 data_input.py`

### Setup user authentication
into the mongodb 
`docker exec -it mongodb mongo`

switch to admin database

`use admin`

first authenticate as root user

`db.auth('admin','admin_123')`

switch to target database

`use data`

create a user assigning read and write rights

`db.createUser({ user:"user1", pwd:"user1_123", roles: [{role:"readWrite",db:"data"}] })`

create a user only assigning read right

`db.createUser({ user:"user2", pwd:"user2_123", roles: [{role:"read",db:"data"}] })`


### Run the interface
navigate to database_project 

`cd build`

`./db_project`

If the executable file cannot run successfully, try to rebuild it.
navigate to database_project 
`rm -rf build`

`mkdir build`

`cd build`

`cmake ..`

`make`

`./db_project`


#### login
After the interface successfully running, use http url to login.

use a browser point to : 'http://0.0.0.0:18080/login`

![image](https://github.com/user-attachments/assets/5746f283-8a28-4a62-be8e-b78ae9406562)


#### Select a operation

After inputing username and password, select an operation.

![image](https://github.com/user-attachments/assets/f8bec3a2-212f-4128-b072-a7001c84693f)


#### Query

In query page, input query parameters.

![image](https://github.com/user-attachments/assets/2e008926-aa24-49de-894d-1535173f631c)

Get the query result.

![image](https://github.com/user-attachments/assets/43d8045d-9f3f-4594-b0dd-1501cc067d24)


#### Update

In update page, input update parameters.

![image](https://github.com/user-attachments/assets/3e39ffd5-fb89-47f1-a5fb-749f016977a7)

Get the update result.

![image](https://github.com/user-attachments/assets/956a50e8-66e3-4c6e-bbe6-060ce111deec)



#### Insert

In insert page, input insert parameters.

![image](https://github.com/user-attachments/assets/3af22a21-577d-4ec0-8576-d4c0673e4813)

Get the insert result.

![image](https://github.com/user-attachments/assets/76bb41e6-da9b-4870-b99f-3655b0a2df14)



#### Delete

In delete page, input delete parameters.

![image](https://github.com/user-attachments/assets/00768ddb-bda6-45fb-b12f-f3240ce647fc)

Get the delete result.

![image](https://github.com/user-attachments/assets/9bb6495f-ee29-4a16-abcd-e104c78211cd)

If the document is not found.

![image](https://github.com/user-attachments/assets/6b641f1e-5bf5-456e-85f3-cd788a618fc5)



### How to analyse the performance of index using test.cpp to get the database response time 
 

## Support
If you have any question or ideas about this project, welcome to send an email: cv23649@bristol.ac.uk


## Authors and acknowledgment
Show your appreciation to those who have contributed to the project.

## License
[VISITE LICENSE](https://github.com/yujuan11/database_project/blob/main/LICENSE)

## Project status
If you have run out of energy or time for your project, put a note at the top of the README saying that development has slowed down or stopped completely. Someone may choose to fork your project or volunteer to step in as a maintainer or owner, allowing your project to keep going. You can also make an explicit request for maintainers.
