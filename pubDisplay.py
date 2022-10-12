import paho.mqtt.client as mqtt

broker_address="192.168.1.50"
image_path="images/"
#broker_address="iot.eclipse.org" #use external broker

client = mqtt.Client("P1") #create new instance
client.connect(broker_address) #connect to broker

selectedImage = "dsfasdfas.png"
f=open(image_path + selectedImage,"rb")
fileContent = f.read()
byteArr = bytearray(fileContent)
client.publish("colegio/display",byteArr,0) #publish

client.loop_forever()
