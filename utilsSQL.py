import MySQLdb

#conectar a la bd
db = MySQLdb.connect("localhost","root","root","TFM" )

#inicializar cursor
cursor = db.cursor()

def getInfoVotante(dni):
	try:
		query = "SELECT * FROM votantes WHERE dni='" + dni + "';"
		print query
		cursor.execute(query)		
		row = cursor.fetchone()	
		return row
	except:
		print "Error retrieving voter data"

def actualizarHaVotado(dni):
	try:
		query = "UPDATE votantes SET haVotado=1, fechaVotacion=now() WHERE dni ='" + dni + "';"
		print query
		cursor.execute(query)
		db.commit()
		row = cursor.fetchone()
	except:
		print "Error actualizando el votante"

if __name__ == "__main__":
	getInfoVotante("11111111A")
