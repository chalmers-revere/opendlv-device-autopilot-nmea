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

#ifndef SERIAL_PORT
#define SERIAL_PORT

#include <libserialport.h>

namespace serial {

  enum class mode {
    Read,
    ReadWrite,
    Write
  };

  class Port {
    public:
      Port(std::string const &, int32_t const, serial::mode) noexcept;
      ~Port() noexcept;
      Port(Port const &) = delete;
      Port(Port &&) = delete;
      Port &operator=(Port const &) = delete;
      Port &operator=(Port &&) = delete;

      void read(std::function<void(std::string const)>) noexcept;
      void write(std::string const &) noexcept;

    private:
      void readRunner();

      std::function<void(std::string const)> m_readDeligate;
      struct sp_port *m_port;
      std::thread m_readThread;
      bool m_isReading;
  };

  inline Port::Port(std::string const &portname, int32_t const baudrate, serial::mode mode) noexcept:
    m_readDeligate(),
    m_port(),
    m_readThread(),
    m_isReading(false)
    {
      sp_return res = sp_get_port_by_name(portname.c_str(), &m_port);
      if (res != SP_OK) {
        std::cerr << "Could not find port " << portname << "." << std::endl;
        return;
      }

      enum sp_mode m;
      switch (mode) {
        case mode::Read:
          {
            m = SP_MODE_READ;
            break;
          }
        case mode::Write:
          {
            m = SP_MODE_WRITE;
            break;
          }
        default:
          {
            m = SP_MODE_READ_WRITE;
          }
      }

      res = sp_open(m_port, m);
      if (res != SP_OK) {
        std::cerr << "Could not open port " << portname << "." << std::endl;
      }

      sp_set_baudrate(m_port, baudrate);
    }

  inline Port::~Port() noexcept
  {
    m_isReading = false;
    sp_close(m_port);
    m_readThread.join();
  }

  inline void Port::read(std::function<void(std::string const)> readDeligate) noexcept
  {
    m_isReading = true;
    m_readDeligate = readDeligate;
    m_readThread = std::thread(&Port::readRunner, this);
  }

  inline void Port::readRunner()
  {
    uint32_t const BUFFER_SIZE = 512;
    char buf[BUFFER_SIZE];
    while (m_isReading) {
      enum sp_return ret = sp_blocking_read(m_port, buf, BUFFER_SIZE, 1000);
      uint32_t byteCount = ret;
      std::string data(buf, byteCount);
      m_readDeligate(data);
    }
  }

  inline void Port::write(std::string const &data) noexcept
  {
    std::string d(data);
    sp_nonblocking_write(m_port, static_cast<void*>(&d), data.length());
  }

}

#endif
