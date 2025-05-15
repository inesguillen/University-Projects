Las funciones que se implementan en este programa servidor-cliente son: 

CONNECT <user>
DISCONNECT <user>

REGISTER <user>
UNREGISTER <user>

PUBLISH <filename> <description> 
DELETE <filename>

LIST_USERS 
LIST_CONTENT <user>

GET_FILE <user> <filename> <localfilename>


<h2> COMO EJECUTAR </h2>

Necesitaremos un mínimo de 4 terminales para:
- Servidor principal 
- Servidor web
- Cliente(s)
- Servidor RPC


Importante: El servidor RPC se encuentra en un subdirectorio distinto (server_rpc), por lo que su terminal debe estar ubicada en dicha carpeta para su ejecución.

Desde el directorio raíz del proyecto ejecutamos los siguientes comandos:

<br>export LOG_RPC_IP=localhost
<br>make

Esto compila todos los archivos .c necesarios, incluyendo aquellos que hacen uso del servicio RPC.


Ahora, en cada terminal ejecutamos lo siguiente:
- Servidor principal: ./server -p <num_puerto>
- Servidor web: python3 web_service.py
- Cliente(s): python3 client.py -s localhost -p <num_puerto>
- Servidor RPC: 
        Primero accedemos al subdirectorio mediante cd server_rpc 
        Después, ejecutamos ./server_rpc_server

Podemos tener varios clientes abiertos en distintas terminales. 

Una vez todos los servidores estén activos, el sistema estará listo para recibir y procesar comandos desde las terminales de los clientes, permitiendo realizar todas las operaciones disponibles.

