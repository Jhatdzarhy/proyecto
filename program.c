// program.c
#include <stdio.h>
#include "libco.h"

int main() {
    const char *query = "INSERT INTO datos (matricula, nombre, primerApellido, segundoApellido, materia, calificacion, carrera) VALUES (1785963240, 'Cristina', 'Avila', 'Portillo', 'Matematicas', '96', 'Contaduria') ON DUPLICATE KEY UPDATE nombre='Juan', primerApellido='Perez', segundoApellido='Lopez', materia='Matematicas', calificacion='A', carrera='Ingenieria'";
    
    insert_to_db(query);
    return 0;
}
