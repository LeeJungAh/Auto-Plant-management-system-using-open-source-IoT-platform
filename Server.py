import socket
import MySQLdb
import copy
import time


s = socket.socket()         # Create a socket object
host = '' # Get local machine name

myport = 65504               # Reserve a port for your service.

s.bind((host, myport)) 
s.listen(15)                 # Now wait for client connection.

def main():
  loop = True
  while loop:
    c, addr = s.accept()     # Establish connection with client.
    print 'Got connection from', addr
    db = MySQLdb.connect(host='127.0.0.1' ,user='root', passwd='Ekfrl13', db='strawberry')
      
  #  with db:
   #   cur = db.cursor()
#      cur.execute("INSERT INTO temphumi(temp,humi) VALUES (%s, %s)" ,(temp,humi))

    while loop:
      #data = c.recv(1024)
      #print data
      temp = c.recv(8)
      print temp
      humi = c.recv(8)
      print humi
      lightValue = c.recv(8)
      print lightValue
      pH = c.recv(8)
      print pH
      with db:
        cur = db.cursor()
        cur.execute("INSERT INTO temphumi(temp,humi) VALUES (%s, %s)",(temp,humi))
        cur.execute("INSERT INTO ph(pH) VALUES (%s)",[pH])
        cur.execute("INSERT INTO light(lightValue) VALUES (%s)",[lightValue])
     #   return(temp,humi)

    

    if data == "x":
      loop = False
      c.send('Thank you for connecting')
    c.close()
  pass


if __name__ == '__main__':
  main()
