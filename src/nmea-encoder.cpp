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

#include "cluon-complete.hpp"
#include "nmea-encoder.hpp"

#include <cmath>
#include <cstring>
#include <array>
#include <sstream>
#include <string>

NmeaEncoder::NmeaEncoder() noexcept
{
}

std::string NmeaEncoder::appendChecksum(std::string const &message) const
{
  char const *s = message.c_str();
  int32_t c = 0;

  while (*s) {
    c ^= *s++;
  }

  std::stringstream stream;
  stream << "$" << message << "*" << std::hex << c;

  std::string tmp = stream.str();
  std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);

  std::string nmea = tmp + static_cast<char>(13) + static_cast<char>(10);

  return nmea;
}

std::string NmeaEncoder::encodeGpwpl() noexcept
{

// Example 1
//sendMessage("GPWPL,5741,24,N,1154,32,E,WP1");
//    sendMessage("GPWCV,6.0,N,WP1");
//
//    Example 2
    //sendMessage("GPWPL,5740,40,N,1150,20,E,WP2");
   // sendMessage("GPWPL,5740,09,N,1149,86,E,WP3");
   // sendMessage("GPWCV,6.0,N,WP2");
   // sendMessage("GPWCV,6.0,N,WP3");
   // sendMessage("GPWNC,0.4,N,0.7,K,WP3,WP2");
  //  sendMessage("GPBWW,211.0,T,211.0,M,WP3,WP2");
  return appendChecksum("");
}
