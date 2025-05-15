el CMakeLists entregado tiene la parte de clang-tidy comentada por recomendación del coordinador de la asignatura

clang-tidy daba un error interno en avignon, solo en determinadas ocasiones sin razon aparente, por lo que consideramos
que lo más prudente es dejarlo como comentario para evitar problemas

clang-tidy funciona correctamente en nuestras máquinas sin el -19, debido a un problema con las versiones. Solicitamos
ayuda respecto a esto y otro profesor nos dijo de quitar el -19 para trabajar en local.

Tras la depuración con clang-tidy, hemos dejado unos pocos warnings con respecto a conversión de datos, pues seguir las
indicaciones resultaba en errores en la compilación

Las carpetas 'build' y 'cmake-build' han sido eliminadas de la entrega final