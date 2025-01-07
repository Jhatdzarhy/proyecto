// consumidor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include "mensaje.h"
#include "libco.h"

#define BROKER_ADDRESS "localhost"
#define BROKER_PORT 1883
#define TOPIC "mi_topic/subtopic"

// Función que procesa el mensaje recibido y genera una respuesta
void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message) {
    if (message->payloadlen) {
        printf("Mensaje recibido: %s\n", (char *)message->payload);

        int matricula;
        char nombre[50], primerApellido[50], segundoApellido[50], materia[50], calificacion[5], carrera[50];
        sscanf((char *)message->payload, "MATRICULA: %d, NOMBRE: %49[^,], PrimerApellido: %49[^,], SegundoApellido: %49[^,], Materia: %49[^,], Calificacion: %4[^,], Carrera: %49[^,]",
               &matricula, nombre, primerApellido, segundoApellido, materia, calificacion, carrera);

        // Verificar si los valores se han leído correctamente
        printf("Valores leídos: Matricula=%d, Nombre=%s, PrimerApellido=%s, SegundoApellido=%s, Materia=%s, Calificacion=%s, Carrera=%s\n", 
               matricula, nombre, primerApellido, segundoApellido, materia, calificacion, carrera);

        // Formatear la consulta SQL
        char query[512];
        snprintf(query, sizeof(query),
                 "INSERT INTO datos (matricula, nombre, primerApellido, segundoApellido, materia, calificacion, carrera) VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s') "
                 "ON DUPLICATE KEY UPDATE nombre='%s', primerApellido='%s', segundoApellido='%s', materia='%s', calificacion='%s', carrera='%s'",
                 matricula, nombre, primerApellido, segundoApellido, materia, calificacion, carrera,
                 nombre, primerApellido, segundoApellido, materia, calificacion, carrera);

        // Mensaje de depuración antes de insertar
        printf("Ejecutando consulta: %s\n", query);

        // Insertar datos en la base de datos
        insert_to_db(query);
    } else {
        printf("Mensaje recibido vacío o con error.\n");
    }
}

int main() {
    struct mosquitto *mosq;

    // Inicializar la librería Mosquitto
    mosquitto_lib_init();
    mosq = mosquitto_new("Consumidor", true, NULL);

    if (!mosq) {
        fprintf(stderr, "Error creando el cliente Mosquitto.\n");
        return 1;
    }

    // Conectar al broker MQTT
    int ret = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60);
    if (ret != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar con el broker MQTT: %s\n", mosquitto_strerror(ret));
        return 1;
    } else {
        printf("Conectado al broker MQTT.\n");
    }

    // Suscribirse al topic
    mosquitto_subscribe(mosq, NULL, TOPIC, 0);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}

