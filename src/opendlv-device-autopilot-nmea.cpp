/*
 * Copyright (C) 2018 Ola Benderius
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>


#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"

#include "serial-port.hpp"

#include "nmea-decoder.hpp"
#include "nmea-encoder.hpp"

int32_t main(int32_t argc, char **argv) {
  int32_t retCode{0};
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if ( (0 == commandlineArguments.count("nmea-port")) || (0 == commandlineArguments.count("cid") || 0 == commandlineArguments.count("freq")) ) {
    std::cerr << argv[0] << " sneds NMEA latitude--longitude waypoints to an NMEA-based Autopilot." << std::endl;
    std::cerr << "Usage:   " << argv[0] << " --nmea-port=<port> [--nmea-baudrate=<baudrate, default 4800>] --cid=<OpenDLV session> [--id=<Identifier of output messages>] --freq=<frequency of output messages> [--verbose]" << std::endl;
    std::cerr << "Example: " << argv[0] << " --nmea-port=/dev/ttyUSB0 --cid=111 --freq=0.2" << std::endl;
    retCode = 1;
  } else {
    uint16_t const CID{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))};
   // uint32_t const ID{(commandlineArguments["id"].size() != 0) ? static_cast<uint32_t>(std::stoi(commandlineArguments["id"])) : 0};
    uint32_t const BAUDRATE{(commandlineArguments["nmea-baudrate"].size() != 0) ? static_cast<uint32_t>(std::stoi(commandlineArguments["nmea-baudrate"])) : 0};
    bool const VERBOSE{commandlineArguments.count("verbose") != 0};
    float const FREQ = std::stof(commandlineArguments["freq"]);
    std::string const PORT(commandlineArguments["nmea-port"]);

    auto serialReceived{[&VERBOSE](std::string const message)
      {
        if (VERBOSE) {
          std::cout << "Got message: " << message << std::endl;
        }
      }};

    serial::Port serialPort(PORT, BAUDRATE, serial::mode::ReadWrite);
    serialPort.read(serialReceived);

    cluon::OD4Session od4{CID};

    auto atFrequency{[&od4, &VERBOSE, &serialPort]() -> bool
      {
        std::string message = "Hello world";

        if (VERBOSE) {
          std::cout << "Sending message: " << message << std::endl;
        }
        
        serialPort.write(message);

        return true;
      }};

    od4.timeTrigger(FREQ, atFrequency);
  }
  return retCode;
}
