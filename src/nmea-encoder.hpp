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

#ifndef NMEA_ENCODER
#define NMEA_ENCODER

class NmeaEncoder {
  public:
    NmeaEncoder() noexcept;
    ~NmeaEncoder() = default;
    NmeaEncoder(const NmeaEncoder &) = delete;
    NmeaEncoder(NmeaEncoder &&) = delete;
    NmeaEncoder &operator=(const NmeaEncoder &) = delete;
    NmeaEncoder &operator=(NmeaEncoder &&) = delete;

    std::string encodeGpwpl() noexcept;

  private:
    std::string appendChecksum(std::string const &) const;
};

#endif
