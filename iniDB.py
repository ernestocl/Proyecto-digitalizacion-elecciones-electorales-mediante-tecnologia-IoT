#!/usr/bin/python
# coding=utf-8
import MySQLdb

#conectar a la bd
db = MySQLdb.connect("localhost","root","root","TFM" )

#inicializar cursor
cursor = db.cursor()

#crear tabla de votantes
cursor.execute("DROP TABLE IF EXISTS votantes")

sql = """
		CREATE TABLE votantes (id INT NOT NULL PRIMARY KEY AUTO_INCREMENT, 
				nombre VARCHAR(20),
				apellido1 VARCHAR(20),
				apellido2 VARCHAR(20),
				dni VARCHAR(20),
				sexo VARCHAR(1),
				idColegio VARCHAR(8),
				haVotado BOOLEAN NOT NULL DEFAULT 0,
				fechaVotacion TIMESTAMP NULL 
	);
	"""
cursor.execute(sql)

#insertar a la tabla
try:
	query="""
		INSERT INTO votantes
		(`nombre`,`apellido1`,`apellido2`,`dni`,`sexo`,`idColegio`)
		VALUES
		('ALEX', 'PEREZ', 'GONZALEZ','11111111A','H','00001193')
		"""
	cursor.execute(query)
	db.commit()
	
	query="""
		INSERT INTO votantes
		(`nombre`,`apellido1`,`apellido2`,`dni`,`sexo`,`idColegio`)
		VALUES
		('MARIA', 'RIERA', 'FUENTE','12345678B','M', '00001193')
		"""
	cursor.execute(query)
	db.commit()
	
	query="""
		INSERT INTO votantes
		(`nombre`,`apellido1`,`apellido2`,`dni`,`sexo`, `idColegio`)
		VALUES
		('JAVIER', 'SANCHEZ', 'FORNT','87654321C','H', '00001193')
		"""
	cursor.execute(query)
	db.commit()
	
	query="""
		INSERT INTO votantes
		(`nombre`,`apellido1`,`apellido2`,`dni`,`sexo`,`idColegio`)
		VALUES
		('TERESA', 'MORALES', 'PRAT','12348765D','M', '00001193')
		"""
	cursor.execute(query)
	db.commit()
except:     
	db.rollback()
    
   
db.close()
