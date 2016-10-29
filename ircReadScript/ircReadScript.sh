#!/bin/bash

read -r server port
read -r nickname
read -r username
read -r channel

exec 3<>"/dev/tcp/$server/$port" || echo 'could not connect to server'
#printf >&3 'NICK %s\r\nUSER %s 0 * :%s\r\n' "$nickname" "$realname"

printf >&3 '%s\r\n' "$nickname"
printf >&3 '%s\r\n' "$username"
printf >&3 '%s\r\n' "$channel"

while read -r line;
do
    if [[ "$line" =~ ^"PING " ]]; then
        printf >&3 '%s\r\n' "${line/PING/PONG}"
    else
        printf '%s\n' "$line"
    fi
done <&3