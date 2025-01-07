// LibreriaConsulta.c
#include <stdlib.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include "libco.h"

// Función para insertar datos en la base de datos
void insert_to_db(const char *query) {
    MYSQL *conn; // Variable de conexión para MySQL

    char *server = "localhost"; // Dirección del servidor
    char *user = "root"; // Usuario para consultar la base de datos
    char *password = "pamela"; // Contraseña para el usuario en cuestión
    char *database = "alumnos"; // Nombre de la base de datos a consultar

    conn = mysql_init(NULL); // Inicialización de la conexión

    // Conectar a la base de datos
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return;
    }

    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return;
    }

    printf("Conexión a la base de datos exitosa.\n");

    // Enviar consulta SQL
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error al ejecutar la consulta: %s\n", mysql_error(conn));
    } else {
        printf("Consulta ejecutada correctamente.\n");
    }

    // Cerrar la conexión
    mysql_close(conn);
}
