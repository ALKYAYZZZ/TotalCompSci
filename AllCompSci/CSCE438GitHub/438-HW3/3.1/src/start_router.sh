router_addr='localhost'
router_port='1413'
slave_port='1412'

#echo "Starting Routing Server"
./routing_server -h $router_addr -p $router_port &

m_pid=$!
echo "PID of Router: "$m_pid

#NOTE: Don't need a slave since we are allowed to assume the routing server never crashes
#echo "Starting Slave Server"
#./tsd -h 'localhost' -p $slave_port -r $router_addr -t $router_port -s $m_pid &
#
#s_pid=$!
#echo "PID of Slave: "$s_pid
