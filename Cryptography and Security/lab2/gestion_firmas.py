import json

from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import serialization, hashes
import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from gestion_archivos import GestionArchivos
import datetime
from cryptography import x509
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.x509 import NameOID, load_pem_x509_certificate



#generación, almacenamiento, firma y verificación de firmas usando claves asimétricas
#cada usuario tiene su clave pública y privada que se generan al iniciar sesión por primera vez
#la clave pública: para firmar documentos
#la clave privada: para verificar las firmas (si corresponde a la persona que lo ha subido)
#la privada se almacena cifrada con la clave derivada de la contraseña del usuario
#el motivo por el que la almacenamos es para mantener la misma siempre
# en teoría la pública no haría falta almacenarla porque se deduce a partir de la privada, pero es por si alguien externo
#quiere comprobar la firma, para que la pueda ver sin necesitar la privada.

class GestionFirmas:
    def __init__(self, usuario, clave_derivada, gestion_archivos, certificado_ac1, clave_privada_ac1):
        #a partir de la contraseña del usuario
        self.usuario = usuario
        self.gestion_archivos = gestion_archivos
        self.clave_derivada = clave_derivada

        # Crear directorio para usuario si no existe
        self.usuario_dir = os.path.join("usuarios", usuario)
        os.makedirs(self.usuario_dir, exist_ok=True)

        # guardamos las claves y el salt de cada usuario en su carpeta correspondiente de usuario
        self.salt_file = os.path.join(self.usuario_dir, "salt.bin")
        self.public_key_file = os.path.join(self.usuario_dir, "public_key.pem")
        self.private_key_file = os.path.join(self.usuario_dir, "private_key.pem.enc")
        #self.salt_file = f"{usuario}_salt.bin"

        #self.public_key_file = f"{usuario}_public_key.pem" #aquí guardará la clave pública del usuario
        #formato PEM porque es lo que se usa en criptografía para las claves
        #self.private_key_file = f"{usuario}_private_key.pem.enc" #se almacena la clave privada cifrada
        self.private_key = None #inicializa la clave privada porque al principio no existe
        #contiene la clave privada en memoria temporalmente, pero no se almacena directamente
        self.salt = self.cargar_salt()

        # Generar claves (pública y privada) si no existe
        if not os.path.exists(self.public_key_file) or not os.path.exists(self.private_key_file):
            print("No se encontraron claves existentes. Generando nuevas claves...")
            self.generar_claves()

        print("Claves existentes encontradas. Cargando clave privada...")
        self.cargar_clave_privada(clave_derivada) #si ya existe, solo se carga la privada


        # ##### COSITAS DE CERTIFICADOS #####
        self.certificado_ac1 = certificado_ac1
        self.clave_privada_ac1 = clave_privada_ac1

        #vamos a generar el certificado de nuestro usuario
        usuario_info = self.cargar_usuario("usuarios.json", usuario)
        self.certificado_usuario = self.generar_certificado_usuario(usuario_info)
        self.guardar_certificado(self.certificado_usuario, usuario)


    def cargar_salt(self):
        """with open(self.salt_file, "rb") as file:
            return file.read()"""
        if not os.path.exists(self.salt_file):  # Verifica si el archivo 'salt.bin' existe
            print("Salt no encontrado para el usuario. Generando uno nuevo...")
            salt = os.urandom(16)  # Generar un salt aleatorio de 16 bytes
            with open(self.salt_file, "wb") as file:
                file.write(salt)  # Guardar el salt generado en el archivo
            return salt
        else:
            with open(self.salt_file, "rb") as file:
                return file.read()  # Si existe, cargar el salt desde el archivo


    #Claves usuario
    def generar_claves(self):
        """
        Genera un par de claves (privada y pública), cifra la clave privada
        y guarda la clave pública en un archivo.
        """
        private_key = rsa.generate_private_key(
            public_exponent=65537,
            key_size=2048,
            backend=default_backend()
        ) #utiliza rsa para generar la clave privada del usuario con un tamaño de 2048 bits
        #rsa tiene métodos ya implementados para generar la clave privada y luego sacar la pública a partir de esa privada

        #-----------------------CLAVE PÚBLICA------serializar y generar
        # Serializar: convertir la clave a un formato que se pueda almacenar (PEM)
        public_key = private_key.public_key() #generar la pública a partir de la privada con un método de RSA

        with open(self.public_key_file, "wb") as public_file: #abrimos el archivo donde se guarda la clave pública del
            #usuario en forma de escritura binaria(wb)
            #public_file representa el archivo abierto en modo escitura binaria.

            #escribimos la clave pública en el archivo
            public_file.write(public_key.public_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PublicFormat.SubjectPublicKeyInfo
            )) #public_bytes() convierte la clave pública a un formato serializado
            #recibe como parámetros encoding que especifica el formato de codificación (PEM en Base 64)
            #y format que especifica el formato público de la clave
            #SubjectPublicKeyInfo: estñándar que define la estructura de la clave pública (contiene clave y tipo de algoritmo)

        #---------------------------CLAVE PRIVADA---- Serializar, cifrar y guardar

        with open(self.private_key_file, "wb") as private_file: #abrir el archivo de la clave privada del usuario en wb
            private_bytes = private_key.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=serialization.BestAvailableEncryption(self.clave_derivada)
            )#usamos la clave privada generada anteriormente para convertirla en bytes codificados en formato PEM
            #format: PKCS8(Public Key Cryptography Standards #8) define como se estructura la clave privada
            #incluye info del algortimo (RSA) usado y la clave privada
            #encryption_algorithm() especifica que se use el mejor algoritmo para cifrar la clave privada con la clave
            #derivada de la contraseña del usuario
            private_file.write(private_bytes) #guardar la clave privada cifrada en el archivo

        print(f"Claves generadas: {self.public_key_file} (pública), {self.private_key_file} (privada cifrada)")

        # cargamos la clave en self.private_key pero sin cifrar, RECORDEMOS QUE ES TEMPORAL
    def cargar_clave_privada(self, clave_derivada):
        if not os.path.exists(self.private_key_file):
            raise FileNotFoundError(f"No se encontró el archivo de clave privada: {self.private_key_file}")
        with open(self.private_key_file, "rb") as private_file:  # abre el archivo en lectura binaria
            self.private_key = serialization.load_pem_private_key(
                private_file.read(),
                password=clave_derivada,
            )  # serialization.load_pem_private_key() carga la clave privada desde el archivo PEM
            # recibe como parámetros la lectura del archivo
            # password: especifica la clave que se usará para descifrar
            if self.private_key is None:
                raise ValueError("No se pudo cargar la clave privada.")

    def firmar_archivo(self, ruta_archivo): #recibe la ruta del archivo que se va a firmar
        """
        Genera una firma digital para el archivo indicado y la guarda en un archivo .sig.
        """
        if not os.path.exists(ruta_archivo) or not os.path.isfile(ruta_archivo):
            raise ValueError("Ruta no válida.")
        #Extraemos el nombre del archivo para ver si está en el sistema
        nombre_archivo = os.path.basename(ruta_archivo)
        archivos_usuario = self.gestion_archivos.ver_archivos()
        if archivos_usuario is None:
            raise ValueError("Error al cargar los archivos del usuario.")

        if nombre_archivo not in archivos_usuario:
            print("El archivo no está registrado en el sistema, por favor suba primero el archivo")
            subir = input("¿Deseas subir el archivo? (s/n):").strip().lower()
            if subir == "s":
                self.gestion_archivos.subir_archivos()
            else:
                print("No se puede continuar sin subir el archivo")
                return

        # Leer el contenido del archivo
        with open(ruta_archivo, "rb") as archivo: #lo abre en lectura binaria
            datos = archivo.read() #guarda su contenido binario en datos

        #generamos el hash del archivo para firmar con el hash ya que es más corto
        hash_archivo = hashes.Hash(hashes.SHA256()) #esto es para ir agregando datos en fragmentos del archivo grande
        #usa SHA-256 que genera un resumen de 256 bits
        hash_archivo.update(datos) #va agregando los datos en binario del archivo
        resumen = hash_archivo.finalize() #finaliza el proceso de generación del hash y guarda el reusltado final

        # Generar la firma del resumen
        #utilizamos PSS (Probabilistic Signature Scheme): esquema de firma digital diseñado para RSA (para padding)
        #se utiliza PSS para poder incluir el salt dentro del padding
        firma = self.private_key.sign( #utiliza el metodo sign de la clave privada almacenada en self.private_key
            resumen, #utiliza el resumen: versión condensada del archivo.
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()), #genera el relleno con el algoritmo SHA-256
                salt_length=len(self.salt) #salt aleatorio para la firma, de tamaño max permitido para la clave
            ),
            hashes.SHA256()
        )
        #por qué se usa padding en firma? porque la clave tiene 2048 bits mientras que el resumen solo tiene 256.
        #por que se usa salt? para que no se genere la misma firma si el archivo es el mismo

        # Guardar la firma en un archivo
        #QUE SE GUARDE CON EL NOMBRE EN VEZ DE CON LA RUTA
        archivo_firma = ruta_archivo + ".sig"
        with open(archivo_firma, "wb") as firma_file:
            firma_file.write(firma)

        print(f"Firma generada y guardada en: {archivo_firma}")

    def verificar_firma(self, ruta_archivo, ruta_certificado): #recibe el archivo para verificar su firma y la ruta del certificado del usuario
        """
        Verifica la firma digital de un archivo utilizando la clave pública.
        """
        if not os.path.exists(ruta_archivo) or not os.path.isfile(ruta_archivo):
            raise ValueError("Ruta no válida.")
        # Leer la clave pública, igual que se lee la privada
        if not os.path.exists(ruta_certificado) or not os.path.isfile(ruta_certificado):
           raise ValueError("Certificado no encontrado")

        with open(ruta_certificado, "rb") as cert_file:
            certificado = x509.load_pem_x509_certificate(cert_file.read(), default_backend())
            # Extraer la clave pública del certificado
        public_key = certificado.public_key()

        # Leer el archivo y la firma
        #AQUI ESTARÍA BIEN QUE SEA CON EL NOMBRE DEL ARCHIVO EN VEZ DE UNA RUTA Y QUE SE CREE UN DIRECTORIO COMO EL
        # DE LOS DATOS DEL USUARIO PARA METER TODAS LAS FIRMAS EN LA MISMA CARPETA, CLAU HAZ TU MAGIA
        archivo_firma = ruta_archivo + ".sig"
        if not os.path.exists(archivo_firma):
            print("La firma no existe.")
            return False

        with open(ruta_archivo, "rb") as archivo, open(archivo_firma, "rb") as firma_file:
            #abrimos el archivo en lectura binaria para crear de nuevo su firma y comparar
            #abrimos también la firma en lectura binaria
            datos = archivo.read()
            firma = firma_file.read()

            # Generar el hash del archivo de nuevo, igual que antes
        hash_archivo = hashes.Hash(hashes.SHA256())
        hash_archivo.update(datos)
        resumen = hash_archivo.finalize()

        # Verificar la firma del hash con un método de rsa que recibe la firma original y el resumen con el padding
        # para crear de nuevo la firma y compararlas
        try:
            public_key.verify(
                firma,
                resumen,
                padding.PSS(
                    mgf=padding.MGF1(hashes.SHA256()),
                    salt_length=len(self.salt)
                ),
                hashes.SHA256()
            )
            print("Firma válida. El archivo no ha sido alterado.")
            return True
        except Exception:
            print("La firma no es válida o el archivo ha sido modificado.")
            return False

    def cargar_usuario(self, archivo, usuario):
        if os.path.exists(archivo):
            with open(archivo, 'r') as file:
                datos = json.load(file)  # Cargar todo el JSON
                return datos["usuarios"].get(usuario, None)  # Buscar solo el usuario específico
        else:
            return None  # Si no existe el archivo, devolvemos None

# ######## CERTIFICADOOOOOOOOOS YUPIIIIIIIIIIIIIIIIIII #########


    def generar_certificado_usuario(self, usuario_info):
        print("GENERANDO CERTIFICADO...")
        # la clave privada del usuario se genera al inciar sesion
        #creamos csr (certificate signing request), nuestro usuario solicita un certificado
        # el certificado de nuestro usuario incluirá su nombre completo, mail y centro de salud
        csr = x509.CertificateSigningRequestBuilder().subject_name(x509.Name([
            x509.NameAttribute(NameOID.ORGANIZATION_NAME, "Centro Médico"),
            x509.NameAttribute(NameOID.COMMON_NAME, usuario_info["nombre"] + " " + usuario_info["apellido1"] + " " + usuario_info["apellido2"]),
            x509.NameAttribute(NameOID.EMAIL_ADDRESS, usuario_info["mail"])
        ])).sign(self.private_key, hashes.SHA256()) # clave privada usuario ya en el __init__

        # generamos el certificado firmado por ac1
        certificado_usuario = x509.CertificateBuilder().subject_name(
            csr.subject
        ).issuer_name(
            self.certificado_ac1.subject  # AC1 debe "validar" nuestro certificado
        ).public_key(
            csr.public_key()
        ).serial_number(
            x509.random_serial_number()
        ).not_valid_before(
            datetime.datetime.utcnow()
        ).not_valid_after(
            datetime.datetime.utcnow() + datetime.timedelta(days=365)
            # El certificado será válido por los días especificados
        ).sign(
            self.clave_privada_ac1, hashes.SHA256()  # Firmamos el certificado con la clave privada de la AC1
        )

        return certificado_usuario

    def cargar_certificado(self, ruta):
        with open(ruta, "rb") as f:
            # los certificados se abren como PEM, con esta función de la bilbioteca cryptography podremos
            # abrirlos fácilmente
            certificado = load_pem_x509_certificate(f.read(), default_backend())

            validacion = self.verificar_certificado(certificado)
            if validacion == True:
                print("Certificado válido.")
                return certificado
            else:
                print("Certificado inválido.")
                return None



    def verificar_certificado(self, ruta_certificado_usuario):
        # cargamos ac1
        certificado_ac1 = self.cargar_certificado(self.certificado_ac1)
        # Cargar el certificado del usuario
        certificado_usuario = self.cargar_certificado(ruta_certificado_usuario)

        # primero verificamos que no esté caducado:
        valido = self.verificar_validez(certificado_usuario)
        if not valido:
            print("\n#### El certificado está caducado, por tanto no es válido. ####\n")
            return False

        try:
            # verificamos la firma del certificado del usuario con la clave de ac1
            certificado_ac1.public_key().verify(
                certificado_usuario.signature,
                certificado_usuario.tbs_certificate_bytes,
                padding.PKCS1v15(),  # padding de rsa
                hashes.SHA256()  # hash usado para firmar
            )
            print("\n####verificar_certificado: El certificado es válido :)####\n")
            return True # el certificado es válido
        except Exception as e:
            print("\n####verificar_certificado: El certificado NO es válido :(####\n")
            return False


    # VERIFICAR FECHA EXPIRACIÓN
    # lo usaremos para verificar si un certificado está caducado o no. Esto nos servirá para valorar
    # Si debemos expedir un nuevo certificado para un cliente o no (si no está caducado no debería salir)
    def verificar_validez(self, certificado):
        ahora = datetime.datetime.utcnow()
        #valido_desde = certificado.not_valid_before
        #no es necesario el "valido desde", pues son válidos en el momento que se crean,
        # y se crean cuando se registra el usuario, por lo que inmediatamente podría usarlo
        valido_hasta = certificado.not_valid_after

        if ahora <= valido_hasta:
            #certificado válido
            return True
        else:
            #ahora > valido_hasta
            # el certificado ha caducado
            return False


    def renovar_certificado_usuario(self, usuario, info_usuario):
        # Si el certificado está caducado, lo renueva
        #usamos f"" par apoder interpolar variables directamente entre {}
        ruta_certificado = f"certificados/{usuario}_cert.pem"
        ruta_clave_privada = f"certificados/{usuario}_private_key.pem"

        #verificamos la validez del certificado
        if self.verificar_validez(self.certificado_usuario):
            # si tenemos True, el certificado ES VÁLIDO (no está caducado)
            print("El certificado sigue siendo válido, no es necesario renovarlo\n")
            validez = self.certificado_usuario.not_valid_after
            print("###### Válido hasta " + str(validez) + "######\n")
        else:
            print("Renovando su certificado un año (365 días)...\n")

            # la clave privada ya está cargada en main, por eso la tomamos como parámetro
            # cargamosla info del usuario
            if os.path.exists("usuarios.json"):
                with open("usuarios.json", 'r') as file:
                    datos_usuarios = json.load(file)

            datos_mi_usuario = datos_usuarios["usuarios"][usuario] # carga el trozo de json de este usuario

            #generamos nuevo certificado
            nuevo_certificado = self.generar_certificado_usuario(info_usuario)
            with open(ruta_certificado, "wb") as f:
                # sobreescribimos el certificado caducado
                f.write(nuevo_certificado.public_bytes(serialization.Encoding.PEM))

            print("Renovación exitosa!")
            return

    def guardar_certificado(self, certificado, nombre_usuario):
        # Guardamos el certificado en formato PEM
        ruta_certificado = f"certificados/{nombre_usuario}_cert.pem"

        with open(ruta_certificado, "wb") as f:
            # Convertimos el certificado a formato PEM
            f.write(certificado.public_bytes(serialization.Encoding.PEM))

        print(f"Certificado guardado en: {ruta_certificado}")


    # INTENTO DE INICIO DE SESIÓN, NO FUNCIONA
    def autenticar_con_certificado(self):
        """
        Autenticar al usuario usando su certificado.
        """
        certificado = self.cargar_certificado()
        if certificado and self.verificar_certificado():
            print("Inicio de sesión exitoso con certificado.")
            return True
        else:
            print("Error en la autenticación con certificado.")
            return False


