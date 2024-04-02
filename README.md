Moved to https://git.opendlv.org.

## OpenDLV Microservice to interface with NMEA-based autopilot system

### Build:

```
docker build -f Dockerfile.amd64 -t chalmersrevere/opendlv-device-autopilot-nmea-amd64:v0.0.1 
```

### Run:

```
docker run -ti --device=/dev/ttyACM0 chalmersrevere/opendlv-device-autopilot-nmea-amd64:v0.0.1 opendlv-device-autopilot-nmea --cid=111 --nmea-port=/dev/ttyACM0  --nmea-baudrate=4800 --freq=1 --verbose
```


## License

* This project is released under the terms of the GNU GPLv3 License

