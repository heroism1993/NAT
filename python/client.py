import socket
import struct
import binascii
import ctypes
import os
import sys
import string
import platform

#if sys.arg < 3 : exit(1)
option = sys.argv[1]
name = sys.argv[2]
if sys.argv[3]: 
  port = string.atoi(sys.argv[3]) 
  #print port

sockfd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sockfd.connect(("192.168.1.100",8601)) #server addr and port


if(option == "register"):
  buf = struct.Struct('I20sBBBBII') 
  send_values = ( 0, name ,0,0,0,0,0,0) #(op,name,a1,a2,a3,a4,port,result) #modify to network byte ordered if needed 
  prebuffer = ctypes.create_string_buffer(buf.size) 
  buf.pack_into(prebuffer,0,*send_values) #pack to buffer
  sockfd.send(prebuffer)
  
  recv_buffer = sockfd.recv(1024)
  (rcv_op,rcv_name,a1,a2,a3,a4,rcv_port,result) = buf.unpack(recv_buffer)
  if(result == 0):# if success
    print "Register success!"
    my_addr,my_port = sockfd.getsockname() #local
    #sockfd.close()
    systype = platform.system()
    if(sysstr =="Windows"):	
      cmdline = "netsh interface portproxy add v4tov4  listenaddress=%s listenport=%d connectaddress=%s  connectport=%d" % (my_addr,my_port,my_addr,port)
    elif(sysstr == "Linux"):
      cmdline = "/sbin/iptables iptables -t nat -A PREROUTING -p tcp --dport %d -j REDIRECT --to-port %d NULL" % (my_port,port)
    else:
      #other system  
    print cmdline
    os.system(cmdline)
  elif(result == 1): # same name err
    print "please try again"
  elif(result == 0x2000) #NON_ALPHABET_NOT_ALLOWED_ERROR
    print "NON_ALPHABET_NOT_ALLOWED_ERROR"
  elif(result == 0x02) #ALREADY_REGISTERED_ERROR
    print "ALREADY_REGISTERED_ERROR"
    #sockfd.close()
    return 1
    break
elif (option == "request"):
  buf = struct.Struct('I20sBBBBII')
  send_values = ( 2, name ,0,0,0,0,0,0) #socket.htonl(2) to network byte ordered
  prebuffer = ctypes.create_string_buffer(buf.size) 
  buf.pack_into(prebuffer,0,*send_values)
  sockfd.send(prebuffer)
  
  recv_buffer = sockfd.recv(1024)
  (rcv_op,rcv_name,a1,a2,a3,a4,rcv_port,result) = buf.unpack(recv_buffer) #modify from network byte ordered to host ordered if needed 
  if(result == 0): # if success
    req_addr = "%d.%d.%d.%d" % (a1,a2,a3,a4)
    req_port = rcv_port
  elif(result == 0x10): # not found
    print "the name not found"
  else:
    print "unkown error happened"
elif (option == "deregister"):
  buf = struct.Struct('I20sBBBBII')
  send_values = ( 8, name ,0,0,0,0,0,0) #socket.htonl(8) to network byte ordered #maybe need  add more information
  prebuffer = ctypes.create_string_buffer(buf.size) 
  buf.pack_into(prebuffer,0,*send_values)
  sockfd.send(prebuffer)
  
  recv_buffer = sockfd.recv(1024)
  (rcv_op,rcv_name,a1,a2,a3,a4,rcv_port,result) = buf.unpack(recv_buffer) #modify from network byte ordered to host ordered if needed 
  if(result == 0): # if success
    print "deregister success"
  #elif(): error  
 
#sockfd.close() 