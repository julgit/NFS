/*
 * connection_manager.cpp
 *
 *  Created on: 23 paź 2013
 *      Author: root
 */

#include "connection_manager.h"
#include <algorithm>
#include <boost/bind.hpp>

namespace ftpsrv {

void connection_manager::start(connection_ptr c)
{
  connections_.insert(c);
  //std::cout <<"x";
  c->start();
}

void connection_manager::stop(connection_ptr c)
{
  connections_.erase(c);
  c->stop();
}

void connection_manager::stop_all()
{
  std::for_each(connections_.begin(), connections_.end(),
      boost::bind(&connection::stop, _1));
  connections_.clear();
}

} /* namespace ftpsrv */
