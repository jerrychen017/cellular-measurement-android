# Bandwidth Measurement

An Android application aimed towards testing real time capabilities over 5G and LTE networks. It integrates [cellular-measurement][https://github.com/jerrychen017/cellular-measurement] to measure cellular bandwidth and network latency. It also includes interactive functionality to test real-time interactivity. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

1. Server with three open UDP ports. One port for running the interactive server, and two ports for running bandwidth measurement server. 
If you want to run bandwidth measurement over TCP, make sure TCP ports are open. 

2. An Android phone or a computer with Android Studio installed (so you can run on emulator). 

### How to run

1. Get server programs from [cellular-measurement][https://github.com/jerrychen017/cellular-measurement].  
They are located under the directory ```executables/```

2. Run server programs  
To run `bandwidth_server`:
```
./bandwidth_server <server_send_port> <server_recv_port>
```
e.g. 
```
./bandwidth_server 4576 4577
```
To run `interactive_server`:
```
./interactive_server <interactive_port> <max_num_users> 
```
e.g.
```
./interactive_server 4578 10 
```
 3. Run the Android App  
 <img width="200" src="./res/demo-main.png"> <img width="200" src="./res/demo-config.png">
 * `Echo` button sends a UDP packet to the server and measures the RTT (round-trip-time) between client (your phone) and server.
 * `Bandwidth Measurement` button measures upload/download speed (app will start graphing).
 * `Stop` button stops the bandwidth measurement. 
 * `Configuration` button brings you to the configuration page where you can modify parameters. 

#### Before running `Bandwidth Measurement`, go to `Configuration` and make sure `Upload Port` is the same as `<server_recv_port>` and `Download Port` is the same as `<server_send_port>`

## Built With

* [Android Studio](https://developer.android.com/studio) - IDE for Android Development
* [Gradle](https://gradle.org/) - Dependency Management
* [Android NDK](https://developer.android.com/ndk) - Android Native Development Kit

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/jerrychen017/cellular-measurement-android/tags). 

## Authors

* **[Jerry Chen](https://github.com/jerrychen017)**
* **[Jason Zhang](https://github.com/jz1242)**
* **[Daniel Qian](https://github.com/kuhfzgbt)**

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details