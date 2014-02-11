//
//  scoped_fd.hpp
//  OpenVPN
//
//  Copyright (c) 2012 OpenVPN Technologies, Inc. All rights reserved.
//

// A scoped file descriptor that is automatically closed by its destructor.

#ifndef OPENVPN_COMMON_SCOPED_FD_H
#define OPENVPN_COMMON_SCOPED_FD_H

#include <unistd.h> // for close()

#include <boost/noncopyable.hpp>

namespace openvpn {

  class ScopedFD : boost::noncopyable
  {
  public:
    typedef int base_type;

    ScopedFD() : fd(undefined()) {}

    explicit ScopedFD(const int fd_arg)
      : fd(fd_arg) {}

    static int undefined() { return -1; }

    int release()
    {
      const int ret = fd;
      fd = -1;
      //OPENVPN_LOG("**** SFD RELEASE=" << ret);
      return ret;
    }

    bool defined() const
    {
      return fd >= 0;
    }

    int operator()() const
    {
      return fd;
    }

    void reset(const int fd_arg)
    {
      close();
      fd = fd_arg;
      //OPENVPN_LOG("**** SFD RESET=" << fd);
    }

    // unusual semantics: replace fd without closing it first
    void replace(const int fd_arg)
    {
      //OPENVPN_LOG("**** SFD REPLACE " << fd << " -> " << fd_arg);
      fd = fd_arg;
    }

    int close()
    {
      if (defined())
	{
	  const int ret = ::close(fd);
	  fd = -1;
	  //OPENVPN_LOG("**** SFD CLOSE=" << ret);
	  return ret;
	}
      else
	return 0;
    }

    ~ScopedFD()
    {
      close();
    }

  private:
    int fd;
  };

} // namespace openvpn

#endif // OPENVPN_COMMON_SCOPED_FD_H
