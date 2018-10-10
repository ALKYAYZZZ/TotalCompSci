#!/bin/bash
DEFAULT_PORT=3011
PORT=$DEFAULT_PORT
HOST="localhost:3010"
USAGE="Usage: run.sh [-h] route_host:route_port [-p] port"

while getopts ":h:p:" opt; do
    case $opt in
    h)
        HOST=$OPTARG
    ;;
    p)
        PORT=$OPTARG
    ;;
    \?)
        echo $USAGE
        exit 1
    ;;
    :)
        echo $USAGE
        exit 1
    ;;
    esac
done

# use new port each time to avoid conflicts when slave dies and master doesn't
while true;
do
    ./server $HOST $PORT
    let "TEMP_PORT = ($PORT + 1) % 65536"
    if [ $TEMP_PORT -gt $DEFAULT_PORT ]
    then
        PORT=$TEMP_PORT
    else
        PORT=$DEFAULT_PORT
    fi
    sleep 5
done
