import paho.mqtt.client as mqtt
import utilsSQL as uSQL
import json
import base64

BROKER_ADDRESS = "192.168.1.50"
#BROKER_ADDRESS = "192.168.204.122"

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

def on_message(client, userdata, message):
	print "Message topic: " + message.topic
	print "Message received: "  + message.payload #message.topic message.qos message.retain
	
	if message.topic == "colegio/mesa":
		infoVotante = uSQL.getInfoVotante(message.payload)	# dni voter
		print "Data from DB: %s" % (infoVotante,)
		votanteSerializado = crearVotanteJson(infoVotante)
		print("Publishing to colegio/servidor")
		client.publish("colegio/servidor", json.dumps(votanteSerializado))
	elif message.topic == "colegio/display":
		resultado = json.loads(message.payload)
		print(resultado["dni"])
		if resultado["ok"] == 1:
			uSQL.actualizarHaVotado(resultado["dni"])

def crearVotanteJson(infoVotante):
	data = {}
	data["dni"]	= infoVotante[4]
	data["primerNombre"] = infoVotante[1]
	data["primerApellido"] = infoVotante[2]
	data["segundoApellido"] = infoVotante[3]
	data["sexo"] = infoVotante[5]
	data["haVotado"] = infoVotante[7]
	print "Voter serialized: " + json.dumps(data)
	f=open("images/"+ data["dni"] + ".png", "rb")
	fileContent = f.read()
	byteArr = bytes(fileContent)
	encoded = base64.encodestring(byteArr)
	data["foto"] = encoded
	return data

print("creating new instance") 
client = mqtt.Client("P1") #create new instance
client.on_connect = on_connect
client.on_message = on_message

print("Connecting to broker...")
client.connect(BROKER_ADDRESS) #connect to broker
print("Subscribing to topics","colegio/mesa | colegio/display")
client.subscribe([("colegio/mesa", 2),("colegio/display", 2)])

client.loop_forever()
