# btsnoop_time_convert
Convert between btsnoop time and local time

Each time starting the Bluetooth under Android OS, the file name of old btsnoop file will be appended with a timestamp. Sometimes I got a btsnoop folder with hundreds of btsnoop logs and don't know which one to check even I know the time of issue happening. So I write this simple tool to covert the time which is printed in Android log and the time which is used in btsnoop file name. Usage is simple, just give the timestamp in one of the two formats and it will translate it into the other one.

usage: ./btsnoop_time_convert <btsnoop time or local time>

   ex: ./btsnoop_time_convert 63701068761984000
       ./btsnoop_time_convert "2018 07-29 05:19:21.984"
