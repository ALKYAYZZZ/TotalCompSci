router_addr='localhost'
router_port='1413'

master_port='1415'
slave_port='1416'

#echo "Starting Master Server"
./tsd -h 'localhost' -p $master_port -r $router_addr -t $router_port &
m_pid=$!
echo "PID of Master: "$m_pid

#echo "Starting Slave Server"
./tsd -h 'localhost' -p $slave_port -r $router_addr -t $router_port -s $m_pid &

s_pid=$!
echo "PID of Slave: "$s_pid
