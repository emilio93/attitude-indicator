_IE1119 Laboratorio de Sistemas Incrustados_

# Laboratorio 2
## Horizonte Virtual para navegación

_Copia del Enunciado_
### Descripción

Después del éxito comercial de su luz nocturna, Circuititos S.A. decidió diversificar su negocio,y comenzar a ofrecer servicios de consultoría en sistemas incrustados. Una empresa de diseño deequipos para aeronáutica decide asignarle a Circuititos S.A. un pequeño componente con el fin deprobar la ejecución de la empresa, bajo los estrictos estándares de la industria aérea. Usted, comoingeniero en jefe deberá ejecutar el proyecto, apegándose a los más altos estándares de programación,de usted depende el éxito de este nuevo emprendimiento. El sistema a desarrollar es un indicador de horizonte digital (parte de un indicador de actitud),que muestra la inclinación de un avión con respecto al horizonte, similar a éste:

*Imagen

Para tal efecto, se utilizará la tarjeta de desarrollo del MSP432, junto con el kit de expansión.

### Tabla de Requerimientos

| Requerimiento | Descripción | Porcentaje |
| ------------- | ----------- | ---------- |
| Tamaño del indicador | El indicador deberá utilizar la totalidad de la pantalla LCD. | 5.00% |
| Condición inicial | El indicador de horizonte apuntará siempre al ángulo que indique el acelerómetro en el instante de encendido. | 10.00% |
| Horizonte a nivel | Con el acelerómetro perfectamente nivelado, la pantalla mostrará 50% de la pantalla en azul, 50% en café. | 10.00% |
| Rango de operación (pitch) | El indicador funcionará en un rango de 180°, con -90° siendo el indicador apuntando directamente al suelo (100% café), 0° siendo un horizonte perfectamente nivelado (50% azul) y con 90° siendo el indicador apuntando directamente hacia el cielo (100% azul). | 20.00% |
| Velocidad de operación | El sistema deberá ser capaz de mostrar cambios de 90°/s como mínimo. |20.00% |
| Estándar de programación | El código deberá utilizar el estándar proporcionado por el profesor. | 10.00% |
| Arquitectura del software | El problema deberá ser resuelto utilizando una arquitectura basada en “Scheduling”, como la vista en clase. | 25.00% |
| (Extra) Inclinación lateral (roll) | Desplegar el horizonte dinámicamente usando el “roll”. | 5.00% |
| (Extra)(Extra) Indicador de inclinación lateral | Desplegar una línea blanca que indique el “roll”. | 5.00% |

