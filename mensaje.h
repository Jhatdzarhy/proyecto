//mensaje.h
#ifndef MENSAJE_H
#define MENSAJE_H

//Definicion de la estructura del mensaje
typedef struct {
    int matricula;
    char *nombre;
    char *primerApellido;
    char *segundoApellido;
    char *materia;
    char *calificacion;
    char *carrera;
} Mensaje;

#endif
