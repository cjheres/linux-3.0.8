#!/bin/bash
MAC=`ifconfig wfd0 | grep wfd0 | tr -s ' ' | cut -d ' ' -f5`
LAA=`echo $MAC | cut -d ':' -f1`
LAA=$(($LAA+2))
MAC26=`echo $MAC | cut -d ':' -f2-6`
MACLAA=0$LAA:$MAC26
sed "s/00:50:43:00:00:00/$MAC/" config/wifidirect.conf > tmp$$
mv tmp$$ config/wifidirect.conf
sed "s/02:50:43:00:00:00/$MACLAA/" config/wifidirect.conf > tmp$$
mv tmp$$ config/wifidirect.conf
