Proyecto DavidBuzz

Protocolo

Cliente-Servidor

1º Paso
El cliente se conecta al servidor, enviando en la siguiente informacion en dos mensajes diferentes:
- Nombre.
	Un array de caracteres de máximo 32 caracteres que indique el nombre a usar.
- Tipo de usuario.
	Un caracter talque --> 'A' Mentor | 'B' Mentorizado | 'C' Sin asignar
2º Paso
El servidor registra al cliente y le responde indicando que se ha registrado correctamente con un mensaje compuesto de:
- ID_Cliente.
	Un integer que indique su id dentro del servidor.
- Nombre.
	El nombre con el que se ha registrado.
- Tipo de usuario.
	El tipo de usuario del que se ha registrado.
3* Paso
El servidor envia un mensaje con:
- Si el usuario es un Mentor.
	El vector con todos los usuarios que necesitan ser Mentorizados.
- Si el usuario es un Mentorizado.
	Los usuarios que le estan mentorizando.
