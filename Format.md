# Reglas de Formato y Estilo del Código

_Copia de documento provisto en el material del curso_

- Todas las funciones tendrán un nombre descriptivo, utilizando “camel case”: `CalcularBase(void)`.

- Aunque NO es requerido, se recomienda usar nombres en inglés, en caso de poder reutilizar su código a futuro, o para mostrarlo a futuros empleadores o profesores.

- Todas las variables utilizarán un prefijo según la siguiente tabla:

  | Prefijo | Uso                         |
  | ------- | --------------------------- |
  | `g_`    | Variables globales          |
  | `i_`    | Entradas                    |
  | `o_`    | Salidas                     |
  | `l_`    | Variables locales           |
  | `m_`    | Miembros de clase           |

- Todas las variables utilizarán un calificador dependiendo de su tipo:

  | Prefijo | Uso          |
  | ------- | ------------ |
  | `u8`    | UInt8        |
  | `f`     | float        |
  | `b`     | bool         |
  | `i`     | integer      |
  | `e`     | enum         |
  | `p`     | puntero      |
  | `a`     | array        |
  | `st`    | struct       |

  Por ejemplo:

  | Ejemplo             | Uso                                            |
  | ------------------- | ---------------------------------------------- |
  | `i_pDataBuffer`     | Puntero a un buffer de entrada, entrada        |
  | `l_u16BufferNumber` | Variable local de tipo UInt16                  |
  | `m_eErrorType`      | Enumeración de tipo de error, miembro de clase |

- Se revisará tanto el fondo como la forma de programación en los proyectos. Se pondrá especial énfasis en la documentación del código, incluyendo sangrado, nombres de las variables y comentarios relevantes y puntuales.

- Se permite la utilización de bibliotecas existentes, siempre y cuando el estudiante pueda demostrar el dominio de dicha biblioteca.
