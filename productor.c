// productor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include "mensaje.h"

#define BROKER_ADDRESS "localhost"
#define BROKER_PORT 1883
#define TOPIC "mi_topic/subtopic"

// Función para enviar un mensaje al broker MQTT
void enviar_mensaje(struct mosquitto *mosq, Mensaje *mensaje) {
    char payload[256];
    snprintf(payload, sizeof(payload), 
             "MATRICULA: %d, NOMBRE: %s, PrimerApellido: %s, SegundoApellido: %s, Materia: %s, Calificacion: %s, Carrera: %s",
             mensaje->matricula, mensaje->nombre, mensaje->primerApellido, mensaje->segundoApellido, mensaje->materia, mensaje->calificacion, mensaje->carrera);

    int ret = mosquitto_publish(mosq, NULL, TOPIC, strlen(payload), payload, 0, false);
    if (ret != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error publicando el mensaje: %s\n", mosquitto_strerror(ret));
    }
}

int main() {
    struct mosquitto *mosq;
    Mensaje mensaje;

    // Solicitar al usuario que ingrese los datos
    printf("Ingrese los datos del mensaje:\n");

    printf("Matricula: ");
    scanf("%d", &mensaje.matricula);

    printf("Nombre: ");
    mensaje.nombre = malloc(50);
    scanf("%s", mensaje.nombre);

    printf("Primer Apellido: ");
    mensaje.primerApellido = malloc(50);
    scanf("%s", mensaje.primerApellido);

    printf("Segundo Apellido: ");
    mensaje.segundoApellido = malloc(50);
    scanf("%s", mensaje.segundoApellido);

    printf("Materia: ");
    mensaje.materia = malloc(50);
    scanf("%s", mensaje.materia);

    printf("Calificación: ");
    mensaje.calificacion = malloc(5);
    scanf("%s", mensaje.calificacion);

    printf("Carrera: ");
    mensaje.carrera = malloc(50);
    scanf("%s", mensaje.carrera);

    // Inicializar la librería Mosquitto
    mosquitto_lib_init();
    mosq = mosquitto_new("Productor", true, NULL);

    if (!mosq) {
        fprintf(stderr, "Error creando el cliente Mosquitto.\n");
        return 1;
    }

    // Conectar al broker MQTT
    int ret = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60);
    if (ret != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error al conectar con el broker MQTT: %s\n", mosquitto_strerror(ret));
        return 1;
    }

    // Enviar mensaje
    enviar_mensaje(mosq, &mensaje);

    // Liberar memoria asignada
    free(mensaje.nombre);
    free(mensaje.primerApellido);
    free(mensaje.segundoApellido);
    free(mensaje.materia);
    free(mensaje.calificacion);
    free(mensaje.carrera);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}

