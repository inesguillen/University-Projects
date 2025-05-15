import os
import json
import datetime
from cryptography import x509
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization, hashes
from cryptography.x509 import NameOID
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.x509 import load_pem_x509_certificate
from cryptography.hazmat.backends import default_backend


class GenerarAC1:
    # AC1 ES UN CERTIFICADO QUE SE CREA APARTE
    def __init__(self):
        pass
    def generar_certificado_ac1(self):
        print("Entramos en generar_certificado_ac1")
        # generamos un certificado autofirmado por nosotros mismos
        # usamos el estándar x.509
        # para nuestro proyecto hemos mantenido estructura sencilla, simplemente usaremos el nombre del certificado

        # clave privada y certificado raíz para mi autoridad de certificación raíz
        # la clave la generamos mediante rsa (cifrado asimétrico)
        clave_privada_ac1 = rsa.generate_private_key(
            public_exponent=65537,
            key_size=2048
        )  # con el estándar RSA para el exponente y un tamaño de 2048 bits

        clave_privada_ac1 = clave_privada_ac1

        subject = issuer = x509.Name([
            x509.NameAttribute(NameOID.COMMON_NAME, "AC1"),
        ])
        # subject = issuer porque somos los que la pedimos y los que la damos a la vez
        certificate = x509.CertificateBuilder().subject_name(
            subject
        ).issuer_name(
            issuer
        ).public_key(
            clave_privada_ac1.public_key()
        ).serial_number(
            x509.random_serial_number()
        ).not_valid_before(
            datetime.datetime.utcnow()
        ).not_valid_after(
            datetime.datetime.utcnow() + datetime.timedelta(days=3650) #10 años
            # la validez sera desde el momento en el que lo pedimos hasta que pasen los días de validez
        ).add_extension(
            x509.BasicConstraints(ca=True, path_length=None), critical=True
            # añadimos en la extensión que somos la Autoridad de Certificacion (AC, CA en inglés)
            # crítica, pues es como se suelen poner las extensiones relacionadas con la estructura de la AC
        ).sign(
            clave_privada_ac1, hashes.SHA256()
            # firmamos el certificado, asegurando que es confiable
            # con SHA25 ñadimos un "resumen" único para los datos firmados, que ayuda a
            # garantizar la integridad del certificado
        )

        print("Hemos llegado al final :)")

        return certificate, clave_privada_ac1

