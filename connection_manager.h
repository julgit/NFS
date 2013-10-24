/*
 * connection_manager.h
 *
 *  Created on: 23 paź 2013
 *      Author: root
 */

#ifndef CONNECTION_MANAGER_H_
#define CONNECTION_MANAGER_H_

#include <set>
#include <boost/noncopyable.hpp>
#include "connection.h"

namespace ftpsrv {

class connection_manager
  : private boost::noncopyable
{
public:
  /// Add the specified connection to the manager and start it.
  void start(connection_ptr c);

  /// Stop the specified connection.
  void stop(connection_ptr c);

  /// Stop all connections.
  void stop_all();

//private:
  /// The managed connections.
  std::set<connection_ptr> connections_;
};

} /* namespace ftpsrv */

#endif /* CONNECTION_MANAGER_H_ */
